//                                               -*- C++ -*-
/**
 *  @file  CompositeDistribution.cxx
 *  @brief The CompositeDistribution distribution
 *
 *  Copyright (C) 2005-2014 Airbus-EDF-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author schueller
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include <cstdlib>
#include <cmath>

#include "CompositeDistribution.hxx"
#include "SpecFunc.hxx"
#include "Uniform.hxx"
#include "PersistentObjectFactory.hxx"
#include "ResourceMap.hxx"
#include "MethodBoundNumericalMathEvaluationImplementation.hxx"
#include "Brent.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(CompositeDistribution);

static Factory<CompositeDistribution> RegisteredFactory("CompositeDistribution");

/* Default constructor */
CompositeDistribution::CompositeDistribution()
  : DistributionImplementation("CompositeDistribution")
  , function_(NumericalMathFunction("x", "x"))
  , antecedent_(Uniform(0.0, 1.0))
  , bounds_(0)
  , values_(0)
  , probabilities_(0)
  , increasing_(0)
  , solver_(Brent(SpecFunc::NumericalScalarEpsilon, SpecFunc::NumericalScalarEpsilon, SpecFunc::NumericalScalarEpsilon))
{
  setDimension(1);
  // Update the derivative attributes. It also recompute the range
  update();
}

/* Parameters constructor to use when the two bounds are finite */
CompositeDistribution::CompositeDistribution(const NumericalMathFunction & function,
					     const Distribution & antecedent)
  : DistributionImplementation("CompositeDistribution")
  , function_(function)
  , antecedent_(antecedent)
  , bounds_(0)
  , values_(0)
  , probabilities_(0)
  , increasing_(0)
  , solver_(Brent(SpecFunc::NumericalScalarEpsilon, SpecFunc::NumericalScalarEpsilon, SpecFunc::NumericalScalarEpsilon))
{
  // This method check everything and call the update() method.
  setFunctionAndAntecedent(function, antecedent);
}

/* Set the function and antecedent with check */
void CompositeDistribution::setFunctionAndAntecedent(const NumericalMathFunction & function,
						     const Distribution & antecedent)
{
  if (function.getInputDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the function must have an input dimension equal to 1, here input dimension=" << function.getInputDimension();
  if (function.getOutputDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the function must have an output dimension equal to 1, here input dimension=" << function.getOutputDimension();
  if (!function.getGradientImplementation()->isActualImplementation()) throw InvalidArgumentException(HERE) << "Error: the function must have a gradient. Consider using finite difference.";
  if (antecedent.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the antecedent must have dimension 1. Here dimension=" << antecedent.getDimension();
  function_ = function;
  antecedent_ = antecedent;
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  update();
}

struct DerivativeWrapper
{
  const NumericalMathFunction & function_;
  
  DerivativeWrapper(const NumericalMathFunction & function)
    : function_(function)
  {}

  NumericalPoint computeDerivative(const NumericalPoint & point) const
  {
    NumericalPoint value(1, function_.gradient(point)(0, 0));
    return value;
  }

};

/* Compute all the derivative attributes */
void CompositeDistribution::update()
{
  // First, compute the roots of the gradient
  const NumericalScalar xMin(antecedent_.getRange().getLowerBound()[0]);
  const NumericalScalar xMax(antecedent_.getRange().getUpperBound()[0]);
  bounds_ = NumericalPoint(1, xMin);
  values_ = function_(NumericalPoint(1, xMin));
  probabilities_ = NumericalPoint(1, antecedent_.computeCDF(xMin));
  NumericalScalar fMin(values_[0]);
  NumericalScalar fMax(values_[0]);
  const UnsignedInteger n(ResourceMap::GetAsUnsignedInteger("CompositeDistribution-StepNumber"));
  const DerivativeWrapper derivativeWrapper(function_);
  const NumericalMathFunction derivative(bindMethod<DerivativeWrapper, NumericalPoint, NumericalPoint>(derivativeWrapper, &DerivativeWrapper::computeDerivative, 1, 1));
  NumericalScalar a(xMin);
  NumericalScalar fpA(derivative(NumericalPoint(1, a))[0]);
  NumericalScalar b(a);
  NumericalScalar fpB(fpA);
  for (UnsignedInteger i = 0; i < n; ++i)
    {
      a = b;
      fpA = fpB;
      b = ((i + 1) * xMax + (n - i) * xMin) / (n + 1);
      fpB = derivative(NumericalPoint(1, b))[0];
      try
	{
	  const NumericalScalar root(solver_.solve(derivative, 0.0, a, b, fpA, fpB));
	  bounds_.add(root);
	  const NumericalScalar value(function_(NumericalPoint(1, root))[0]);
	  increasing_.add(value > values_[values_.getSize() - 1]);
	  values_.add(value);
	  probabilities_.add(antecedent_.computeCDF(root));
	  fMin = std::min(value, fMin);
	  fMax = std::max(value, fMax);
	}
      catch(...)
	{
	  // Nothing to do
	}
    }
  bounds_.add(xMax);
  const NumericalScalar value(function_(NumericalPoint(1, xMax))[0]);
  increasing_.add(value > values_[values_.getSize() - 1]);
  values_.add(value);
  probabilities_.add(NumericalPoint(1, antecedent_.computeCDF(xMax)));
  fMin = std::min(value, fMin);
  fMax = std::max(value, fMax);
  setRange(Interval(fMin, fMax));
}

/* Function accessors */
void CompositeDistribution::setFunction(const NumericalMathFunction & function)
{
  if (function != function_) setFunctionAndAntecedent(function, antecedent_);
}

NumericalMathFunction CompositeDistribution::getFunction() const
{
  return function_;
}

/* Antecedent accessors */
void CompositeDistribution::setAntecedent(const Distribution & antecedent)
{
  if (antecedent != antecedent_) setFunctionAndAntecedent(function_, antecedent);
}

Distribution CompositeDistribution::getAntecedent() const
{
  return antecedent_;
}

/* Solver accessors */
void CompositeDistribution::setSolver(const Solver & solver)
{
  if (solver != solver_)
    {
      solver_ = solver;
      update();
    }
}

Solver CompositeDistribution::getSolver() const
{
  return solver_;
}

/* Comparison operator */
Bool CompositeDistribution::operator ==(const CompositeDistribution & other) const
{
  if (this == &other) return true;
  return (antecedent_ == other.antecedent_) && (function_ == other.function_);
}

/* String converter */
String CompositeDistribution::__repr__() const
{
  OSS oss;
  oss << "class=" << CompositeDistribution::GetClassName()
      << " name=" << getName()
      << " function=" << function_
      << " antecedent=" << antecedent_
      << " bounds=" << bounds_
      << " values=" << values_
      << " probabilities=" << probabilities_
      << " increasing=" << increasing_;
  return oss;
}

String CompositeDistribution::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << " = f(" << antecedent_.__str__() << ") with f = \n" << function_;
  return oss;
}

/* Virtual constructor */
CompositeDistribution * CompositeDistribution::clone() const
{
  return new CompositeDistribution(*this);
}

/* Get one realization of the distribution */
NumericalPoint CompositeDistribution::getRealization() const
{
  return function_(antecedent_.getRealization());
}

/* Get the PDF of the distribution */
NumericalScalar CompositeDistribution::computePDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();
  NumericalScalar pdf(0.0);
  const NumericalScalar x(point[0]);
  NumericalScalar a(bounds_[0]);
  NumericalScalar fA(values_[0]);
  NumericalScalar b(a);
  NumericalScalar fB(b);
  for (UnsignedInteger i = 1; i < bounds_.getSize(); ++i)
    {
      a = b;
      fA = fB;
      b = bounds_[i];
      fB = values_[i];
      if (( increasing_[i - 1] && (fA <= x) && (x < fB)) ||
	  (!increasing_[i - 1] && (fB <= x) && (x < fA)))
	{
	  const NumericalPoint fInvX(1, solver_.solve(function_, x, a, b, fA, fB));
	  const NumericalScalar denom(fabs(function_.gradient(fInvX)(0, 0)));
	  pdf += antecedent_.computePDF(fInvX) / denom;
	}
    } // i
  return pdf;
}


/* Get the CDF of the distribution */
NumericalScalar CompositeDistribution::computeCDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();
  NumericalScalar cdf(0.0);
  const NumericalScalar x(point[0]);
  NumericalScalar a(bounds_[0]);
  NumericalScalar fA(values_[0]);
  NumericalScalar b(a);
  NumericalScalar fB(fA);
  for (UnsignedInteger i = 1; i < bounds_.getSize(); ++i)
    {
      a = b;
      fA = fB;
      b = bounds_[i];
      fB = values_[i];
      // The contribution of the current segment [a, b] to the probability P(X <= x) where X = f(antecedent) is
      // the probability of X\in f([a,b])\cap X <= x
      // If f is increasing, f([a, b]) = [f(a), f(b)] and the contribution is:
      // 0 if x <= f(a)
      // Fantecedent(b) - Fantecedent(a) if x >= f(b)
      // Fantecedent(t) - Fantecedent(a) if f(a) < x < f(b) where f(t) = x
      if (increasing_[i - 1])
	{
	  if (x >= fB) cdf += probabilities_[i] - probabilities_[i - 1];
	  else if (x > fA)
	    {
	      const NumericalPoint fInvX(1, solver_.solve(function_, x, a, b, fA, fB));
	      cdf += antecedent_.computeCDF(fInvX) - probabilities_[i - 1];
	    }
	} // increasing
      // If f is decreasing, f([a, b]) = [f(b), f(a)] and the contribution is:
      // 0 if x <= f(b)
      // Fantecedent(b) - Fantecedent(a) if x >= f(a)
      // Fantecedent(b) - Fantecedent(t) if f(b) < x < f(a) where f(t) = x
      else
	{
	  if (x >= fA) cdf += probabilities_[i] - probabilities_[i - 1];
	  else if (x > fB)
	    {
	      const NumericalPoint fInvX(1, solver_.solve(function_, x, a, b, fA, fB));
	      cdf += probabilities_[i] - antecedent_.computeCDF(fInvX);
	    }
	} // decreasing
    } // i
  return cdf;  
}

/* Parameters value and description accessor */
CompositeDistribution::NumericalPointWithDescriptionCollection CompositeDistribution::getParametersCollection() const
{
  return antecedent_.getParametersCollection();
}

/* Tell if the distribution is continuous */
Bool CompositeDistribution::isContinuous() const
{
  return antecedent_.isContinuous();
}

/* Tell if the distribution is integer valued */
Bool CompositeDistribution::isDiscrete() const
{
  return antecedent_.isDiscrete();
}

/* Method save() stores the object through the StorageManager */
void CompositeDistribution::save(Advocate & adv) const
{
  DistributionImplementation::save(adv);
  adv.saveAttribute( "function_", function_ );
  adv.saveAttribute( "antecedent_", antecedent_ );
  adv.saveAttribute( "bounds_", bounds_ );
  adv.saveAttribute( "values_", values_ );
  adv.saveAttribute( "probabilities_", probabilities_ );
  adv.saveAttribute( "increasing_", increasing_ );
  adv.saveAttribute( "solver_", solver_ );
}

/* Method load() reloads the object from the StorageManager */
void CompositeDistribution::load(Advocate & adv)
{
  DistributionImplementation::load(adv);
  adv.loadAttribute( "function_", function_ );
  adv.loadAttribute( "antecedent_", antecedent_ );
  adv.loadAttribute( "bounds_", bounds_ );
  adv.loadAttribute( "values_", values_ );
  adv.loadAttribute( "probabilities_", probabilities_ );
  adv.loadAttribute( "solver_", solver_ );
  update();
}

END_NAMESPACE_OPENTURNS
