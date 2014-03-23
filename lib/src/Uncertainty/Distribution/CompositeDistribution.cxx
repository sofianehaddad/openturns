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
  , solver_(Brent(SpecFunc::NumericalScalarEpsilon, SpecFunc::NumericalScalarEpsilon, SpecFunc::NumericalScalarEpsilon))
{
  setDimension(1);
  // Update the derivative attributes. It also recompute the range
  update()
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
  , solver_(Brent(SpecFunc::NumericalScalarEpsilon, SpecFunc::NumericalScalarEpsilon, SpecFunc::NumericalScalarEpsilon))
{
  // This method check everything and call the update() method.
  setAntecedentAndFunction(antecedent, function);
}

/* Set the function and antecedent with check */
void CompositeDistribution::setFunctionAndAntecedent(const NumericalMathFunction & function,
						     const Distribution & antecedent)
{
  if (function.getInputDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the function must have an input dimension equal to 1, here input dimension=" << function.getInputDimension();
  if (function.getOutputDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the function must have an output dimension equal to 1, here input dimension=" << function.getOutputDimension();
  if (!function.getGradientImplementation()->isActualImplementation()) throw InvalidArgumentException(HERE) << "Error: the function must have a gradient. Consider using finite difference.";
  if (antecedent.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the antecedent must have dimension 1. Here dimension=" << antecedent.getInputDimension();
  function_ = function;
  antecedent_ = antecedent;
  update();
}

struct DerivativeWrapper
{
  const NumericalMathFunction & function_;
  
  DerivativeWrapper(const NumericalMathFunction & function_)
    : function_(function)
  {}

  NumericalPoint computeDerivative(const NumericalPoint & point) const
  {
    return NumericalPoint(1, function_.gradient(point)(0, 0));
  }

};

void set
/* Compute all the derivative attributes */
void update()
{
  // First, compute the roots of the gradient
  const NumericalScalar xMin(antecedent_.getRange().getLowerBound()[0]);
  bounds_ = NumericalPoint(1, xMin);
  const NumericalScalar xMax(antecedent_.getRange().getUpperBound()[0]);
  const UnsignedInteger n(ResourceMap::GetAsUnsignedInteger("CompositeDistribution-StepNumber"));
  const NumericalScalar step((xMax - xMin) / n);
  NumericalScalar a(xMin);
  NumericalScalar b(xMin);
  const DerivativeWrapper derivativeWrapper(function_);
  const NumericalMathFunction derivative(bindMethod<computeDerivative, NumericalPoint, NumericalPoint>(derivativeWrapper, &DerivativeWrapper::computeDerivative, 1, 1));
  derivativeRoots_ = NumericalSample(0, 1);
  for (UnsignedInteger i = 0; i < n; ++i)
    {
      a = b;
      b += step;
      try
	{
	  const NumericalScalar root(solver_.solve(derivative, 0.0, a, b));
	  bounds_.add(root);
	  values_.add(function_(NumericalPoint(1, root))[0]);
	}
      catch(...) continue;
    }
  bounds_.add(xMax);
  // Second, the contributions of each interval
  for (UnsignedInteger i = 1; i < bounds_.getSize() - 1; ++i) probabilities_.add(fabs(antecedent_.computeProbability(Interval(bounds_[i], bounds_[i - 1]))));
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
      << " antecedent=" << antecedent_
      << " function=" << function_
      << " derivativeRoots=" << derivativeRoots_
      << " cumulatedProbabilities=" << cumulatedProbabilities_;
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

/* Compute the numerical range of the distribution given the parameters values */
void CompositeDistribution::computeRange()
{
  const NumericalPoint lowerBound(1, lowerBound_);
  const NumericalPoint upperBound(1, upperBound_);
  const Interval::BoolCollection finiteLowerBound(1, finiteLowerBound_);
  const Interval::BoolCollection finiteUpperBound(1, finiteUpperBound_);
  setRange(distribution_.getRange().intersect(Interval(lowerBound, upperBound, finiteLowerBound, finiteUpperBound)));
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
  for (UnsignedInteger i = 0; i < bounds_.getSize(); 
}


/* Get the CDF of the distribution */
NumericalScalar CompositeDistribution::computeCDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  const NumericalScalar x(point[0]);
  if (x <= lowerBound_) return 0.0;
  if (x >= upperBound_) return 1.0;
  // If tail=true, don't call distribution_.computeCDF with tail=true in the next line!
  return normalizationFactor_ * (distribution_.computeCDF(point) - cdfLowerBound_);
}

NumericalScalar CompositeDistribution::computeComplementaryCDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  const NumericalScalar x(point[0]);
  if (x <= lowerBound_) return 1.0;
  if (x > upperBound_) return 0.0;
  // If tail=true, don't call distribution_.computeCDF with tail=true in the next line!
  return normalizationFactor_ * (cdfUpperBound_ - distribution_.computeCDF(point));
}

/* Get the PDFGradient of the distribution */
NumericalPoint CompositeDistribution::computePDFGradient(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  const NumericalScalar x(point[0]);
  if ((x <= lowerBound_) || (x > upperBound_)) return NumericalPoint(distribution_.getParametersCollection()[0].getDimension() + finiteLowerBound_ + finiteUpperBound_);
  const NumericalPoint pdfGradientX(distribution_.computePDFGradient(point));
  const NumericalPoint cdfGradientLowerBound(finiteLowerBound_ ? distribution_.computeCDFGradient(NumericalPoint(1, lowerBound_)) : NumericalPoint(distribution_.getParametersCollection()[0].getDimension()));
  const NumericalPoint cdfGradientUpperBound(finiteUpperBound_ ? distribution_.computeCDFGradient(NumericalPoint(1, upperBound_)) : NumericalPoint(distribution_.getParametersCollection()[0].getDimension()));
  const NumericalScalar pdfPoint(distribution_.computePDF(point));
  NumericalPoint pdfGradient(normalizationFactor_ * pdfGradientX - pdfPoint * normalizationFactor_ * normalizationFactor_ * (cdfGradientUpperBound - cdfGradientLowerBound));
  // If the lower bound is finite, add a component to the gradient
  if (finiteLowerBound_)
  {
    pdfGradient.add(pdfLowerBound_ * pdfPoint * normalizationFactor_ * normalizationFactor_);
  }
  // If the upper bound is finite, add a component to the gradient
  if (finiteUpperBound_)
  {
    pdfGradient.add(-pdfUpperBound_ * pdfPoint * normalizationFactor_ * normalizationFactor_);
  }
  return pdfGradient;
}

/* Get the CDFGradient of the distribution */
NumericalPoint CompositeDistribution::computeCDFGradient(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  const NumericalScalar x(point[0]);
  if ((x <= lowerBound_) || (x > upperBound_)) return NumericalPoint(distribution_.getParametersCollection()[0].getDimension() + finiteLowerBound_ + finiteUpperBound_);
  const NumericalPoint cdfGradientX(distribution_.computeCDFGradient(point));
  const NumericalPoint cdfGradientLowerBound(finiteLowerBound_ ? distribution_.computeCDFGradient(NumericalPoint(1, lowerBound_)) : NumericalPoint(distribution_.getParametersCollection()[0].getDimension()));
  const NumericalPoint cdfGradientUpperBound(finiteUpperBound_ ? distribution_.computeCDFGradient(NumericalPoint(1, upperBound_)) : NumericalPoint(distribution_.getParametersCollection()[0].getDimension()));
  const NumericalScalar cdfPoint(distribution_.computeCDF(point));
  NumericalPoint cdfGradient(normalizationFactor_ * (cdfGradientX - cdfGradientLowerBound) - (cdfPoint - cdfLowerBound_) * normalizationFactor_ * normalizationFactor_ * (cdfGradientUpperBound - cdfGradientLowerBound));
  // If the lower bound is finite, add a component to the gradient
  if (finiteLowerBound_)
  {
    cdfGradient.add(pdfLowerBound_ * normalizationFactor_ * ((cdfPoint - cdfLowerBound_) * normalizationFactor_ - 1.0));
  }
  // If the upper bound is finite, add a component to the gradient
  if (finiteUpperBound_)
  {
    cdfGradient.add(-pdfUpperBound_ * normalizationFactor_ * (cdfPoint - cdfLowerBound_) * normalizationFactor_);
  }
  return cdfGradient;
}

/* Get the quantile of the distribution */
NumericalScalar CompositeDistribution::computeScalarQuantile(const NumericalScalar prob,
    const Bool tail) const
{
  if (tail) return distribution_.computeQuantile(cdfUpperBound_ - prob * (cdfUpperBound_ - cdfLowerBound_))[0];
  return distribution_.computeQuantile(cdfLowerBound_ + prob * (cdfUpperBound_ - cdfLowerBound_))[0];
}

/* Parameters value and description accessor */
CompositeDistribution::NumericalPointWithDescriptionCollection CompositeDistribution::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(distribution_.getParametersCollection()[0]);
  Description description(point.getDescription());
  if (finiteLowerBound_)
  {
    point.add(lowerBound_);
    description.add("lowerBound");
  }
  if (finiteUpperBound_)
  {
    point.add(upperBound_);
    description.add("upperBound");
  }
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

/* distribution accessor */
void CompositeDistribution::setDistribution(const Distribution & distribution)
{
  if (distribution.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can truncate only distribution with dimension=1, here dimension=" << distribution.getDimension();
  distribution_ = distribution;
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  isAlreadyCreatedGeneratingFunction_ = false;
  computeRange();
}

Distribution CompositeDistribution::getDistribution() const
{
  return distribution_;
}

/* Realiation threshold accessor */
void CompositeDistribution::setThresholdRealization(const NumericalScalar thresholdRealization)
{
  if ((thresholdRealization < 0.0) || (thresholdRealization > 1.0)) throw InvalidArgumentException(HERE) << "Realization threshold must be in [0, 1], here thresholdRealization=" << thresholdRealization;
  thresholdRealization_ = thresholdRealization;
}

NumericalScalar CompositeDistribution::getThresholdRealization() const
{
  return thresholdRealization_;
}

/* Lower bound accessor */
void CompositeDistribution::setLowerBound(const NumericalScalar lowerBound)
{
  if ((finiteUpperBound_) && (lowerBound > upperBound_)) throw InvalidArgumentException(HERE) << "Error: the lower bound must be strictly less than the upper bound, here lower bound=" << lowerBound << " and upper bound=" << upperBound_;
  cdfLowerBound_ = distribution_.computeCDF(NumericalPoint(1, lowerBound));
  if (cdfLowerBound_ >= cdfUpperBound_) throw InvalidArgumentException(HERE) << "Error: the truncation interval does not contain a non-empty part of the support of the distribution";
  pdfLowerBound_ = distribution_.computePDF(NumericalPoint(1, lowerBound));
  lowerBound_ = lowerBound;
  finiteLowerBound_ = true;
  normalizationFactor_ = 1.0 / (cdfUpperBound_ - cdfLowerBound_);
  computeRange();
}

NumericalScalar CompositeDistribution::getLowerBound() const
{
  return lowerBound_;
}


/* Upper bound accessor */
void CompositeDistribution::setUpperBound(const NumericalScalar upperBound)
{
  if ((finiteLowerBound_) && (upperBound < lowerBound_)) throw InvalidArgumentException(HERE) << "Error: the upper bound must be strictly greater than the lower bound, here upper bound=" << upperBound << " and lower bound=" << lowerBound_;
  cdfUpperBound_ = distribution_.computeCDF(NumericalPoint(1, upperBound));
  if (cdfUpperBound_ <= cdfLowerBound_) throw InvalidArgumentException(HERE) << "Error: the truncation interval does not contain a non-empty part of the support of the distribution";
  pdfUpperBound_ = distribution_.computePDF(NumericalPoint(1, upperBound));
  upperBound_ = upperBound;
  finiteUpperBound_ = true;
  normalizationFactor_ = 1.0 / (cdfUpperBound_ - cdfLowerBound_);
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  isAlreadyCreatedGeneratingFunction_ = false;
  computeRange();
}

NumericalScalar CompositeDistribution::getUpperBound() const
{
  return upperBound_;
}

/* Lower bound finite flag accessor */
void CompositeDistribution::setFiniteLowerBound(const Bool finiteLowerBound)
{
  // A stange case: the new flag tells that the bound is finite, but no finite previous value has been given
  if (finiteLowerBound && !finiteLowerBound_) throw InvalidArgumentException(HERE) << "Error: cannot set a finite flag on a non finite previous value";
  // If we switched from a finite value to an infinite one, update everything
  if (!finiteLowerBound)
  {
    lowerBound_ = -SpecFunc::MaxNumericalScalar;
    pdfLowerBound_ = 0.0;
    cdfLowerBound_ = 0.0;
    finiteLowerBound_ = false;
    normalizationFactor_ = 1.0 / (cdfUpperBound_ - cdfLowerBound_);
  }
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  isAlreadyCreatedGeneratingFunction_ = false;
  computeRange();
}

Bool CompositeDistribution::getFiniteLowerBound() const
{
  return finiteLowerBound_;
}

/* Upper bound finite flag accessor */
void CompositeDistribution::setFiniteUpperBound(const Bool finiteUpperBound)
{
  // A strange case: the new flag tells that the bound is finite, but no finite previous value has been given
  if (finiteUpperBound && !finiteUpperBound_) throw InvalidArgumentException(HERE) << "Error: cannot set a finite flag on a non finite previous value";
  // If we switched from a finite value to an infinite one, update everything
  if (!finiteUpperBound)
  {
    upperBound_ = SpecFunc::MaxNumericalScalar;
    pdfUpperBound_ = 0.0;
    cdfUpperBound_ = 1.0;
    finiteUpperBound_ = false;
    normalizationFactor_ = 1.0 / (cdfUpperBound_ - cdfLowerBound_);
  }
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  isAlreadyCreatedGeneratingFunction_ = false;
  computeRange();
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
  adv.saveAttribute( "antecedent_", antecedent_ );
  adv.saveAttribute( "function_", function_ );
  adv.saveAttribute( "derivativeRoots_", derivativeRoots_ );
  adv.saveAttribute( "cumulatedProbabilities_", cumulatedProbabilities_ );
}

/* Method load() reloads the object from the StorageManager */
void CompositeDistribution::load(Advocate & adv)
{
  DistributionImplementation::load(adv);
  adv.loadAttribute( "antecedent_", antecedent_ );
  adv.loadAttribute( "function_", function_ );
  adv.loadAttribute( "derivativeRoots_", derivativeRoots_ );
  adv.loadAttribute( "cumulatedProbabilities_", cumulatedProbabilities_ );
  update();
}

END_NAMESPACE_OPENTURNS
