//                                               -*- C++ -*-
/**
 *  @file  DistributionImplementation.cxx
 *  @brief Abstract top-level class for all distributions
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include <cmath>
#include <cstdlib>
#include <algorithm>

#include "PersistentObjectFactory.hxx"
#include "DistributionImplementation.hxx"
#include "Distribution.hxx"
#include "Exception.hxx"
#include "Log.hxx"
#include "Lapack.hxx"
#include "IdentityMatrix.hxx"
#include "Collection.hxx"
#include "RandomGenerator.hxx"
#include "Normal.hxx"
#include "IndependentCopula.hxx"
#include "MarginalTransformationEvaluation.hxx"
#include "MarginalTransformationGradient.hxx"
#include "MarginalTransformationHessian.hxx"
#include "RosenblattEvaluation.hxx"
#include "InverseRosenblattEvaluation.hxx"
#include "NumericalMathFunction.hxx"
#include "SklarCopula.hxx"
#include "SpecFunc.hxx"
#include "PlatformInfo.hxx"
#include "MethodBoundNumericalMathEvaluationImplementation.hxx"
#include "Contour.hxx"
#include "Curve.hxx"
#include "Staircase.hxx"
#include "Drawable.hxx"
#include "Graph.hxx"
#include "Brent.hxx"
#include "Box.hxx"
#include "Tuples.hxx"
#include "Combinations.hxx"
#include "TBB.hxx"
#include "GaussKronrod.hxx"
#include "TriangularMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(DistributionImplementation);

typedef NumericalMathFunctionImplementation::EvaluationImplementation EvaluationImplementation;
typedef NumericalMathFunctionImplementation::GradientImplementation   GradientImplementation;
typedef NumericalMathFunctionImplementation::HessianImplementation    HessianImplementation;
typedef Collection<Distribution>                                      DistributionCollection;

static Factory<DistributionImplementation> RegisteredFactory("DistributionImplementation");

/* Default constructor */
DistributionImplementation::DistributionImplementation()
  : PersistentObject()
  , mean_(NumericalPoint(0))
  , covariance_(CovarianceMatrix(0))
  , gaussNodesAndWeights_()
  , integrationNodesNumber_(ResourceMap::GetAsUnsignedInteger( "DistributionImplementation-DefaultIntegrationNodesNumber" ))
  , isAlreadyComputedMean_(false)
  , isAlreadyComputedCovariance_(false)
  , isAlreadyComputedGaussNodesAndWeights_(false)
  , pdfEpsilon_(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-DefaultPDFEpsilon" ))
  , cdfEpsilon_(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-DefaultCDFEpsilon" ))
  , quantileEpsilon_(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-DefaultQuantileEpsilon" ))
  , quantileIterations_(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-DefaultQuantileIteration" ))
  , isAlreadyComputedStandardDistribution_(false)
  , p_standardDistribution_()
  , isAlreadyCreatedGeneratingFunction_(false)
  , generatingFunction_(0)
  , dimension_(1)
  , weight_(1.0)
  // The range is empty by default
  , range_(Interval(1.0, -1.0))
  , description_(1)
  , isParallel_(ResourceMap::GetAsUnsignedInteger("DistributionImplementation-Parallel") != 0)
{
  description_[0] = "marginal 1";
}

/* Virtual constructor */
DistributionImplementation * DistributionImplementation::clone() const
{
  return new DistributionImplementation(*this);
}

/* Comparison operator */
Bool DistributionImplementation::operator ==(const DistributionImplementation & other) const
{
  if (this == &other) return true;
  return (dimension_ == other.dimension_) && (weight_ == other.weight_) && (range_ == other.range_);
}

/* Comparison operator */
Bool DistributionImplementation::operator !=(const DistributionImplementation & other) const
{
  return !operator==(other);
}

/* String converter */
String DistributionImplementation::__repr__() const
{
  OSS oss(true);
  oss << "class=" << DistributionImplementation::GetClassName()
      << " description=" << description_;
  return oss;
}

/* String converter */
String DistributionImplementation::__str__(const String & offset) const
{
  return __repr__();
}


/* Weight accessor */
void DistributionImplementation::setWeight(const NumericalScalar w)
{
  weight_ = w;
}

/* Weight accessor */
NumericalScalar DistributionImplementation::getWeight() const
{
  return weight_;
}


/* Dimension accessor */
UnsignedInteger DistributionImplementation::getDimension() const
{
  return dimension_;
}

/* Get the roughness, i.e. the L2-norm of the PDF */
NumericalScalar DistributionImplementation::getRoughness() const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::getRoughness() const";
}

/* Dimension accessor */
void DistributionImplementation::setDimension(const UnsignedInteger dim)
{
  if (dim == 0) throw InvalidArgumentException(HERE) << "Dimension argument must be an integer >= 1, here dim = " << dim;
  if (dim != dimension_)
  {
    dimension_ = dim;
    isAlreadyComputedMean_ = false;
    isAlreadyComputedCovariance_ = false;
    isAlreadyComputedGaussNodesAndWeights_ = false;
    // Check if the current description is compatible with the new dimension
    if (description_.getSize() != dim)
    {
      description_ = Description(dim);
      for (UnsignedInteger i = 0; i < dim; ++i) description_[i] = OSS() << "marginal " << i + 1;
    }
  }
}

/* Get one realization of the distributionImplementation */
NumericalPoint DistributionImplementation::getRealization() const
{
  // Use CDF inversion in the 1D case
  if (dimension_ == 1) return computeQuantile(RandomGenerator::Generate());
  NumericalPoint point(0);
  for (UnsignedInteger i = 0; i < dimension_; ++i) point.add(computeConditionalQuantile(RandomGenerator::Generate(), point));
  return point;
}

/* Get a numerical sample whose elements follow the distributionImplementation */
NumericalSample DistributionImplementation::getSample(const UnsignedInteger size) const
{
  NumericalSample returnSample(size, dimension_);
  for (UnsignedInteger i = 0; i < size; ++i)
    returnSample[i] = getRealization();
  returnSample.setName(getName());
  returnSample.setDescription(getDescription());
  return returnSample;
}

/* Get the DDF of the distributionImplementation */
NumericalPoint DistributionImplementation::computeDDF(const NumericalPoint & point) const
{
  NumericalPoint ddf(dimension_);
  const NumericalScalar cdfPoint(computeCDF(point));
  const NumericalScalar h(pow(cdfEpsilon_, 0.25));
  const NumericalScalar idenom(1.0 / sqrt(cdfEpsilon_));
  for (UnsignedInteger i = 0; i < dimension_; ++i)
  {
    NumericalPoint epsilon(dimension_, 0.0);
    epsilon[i] = h;
    ddf[i] = (computeCDF(point + epsilon) - 2.0 * cdfPoint + computeCDF(point - epsilon)) * idenom;
  }
  return ddf;
}

/* Get the PDF of the distributionImplementation */
NumericalScalar DistributionImplementation::computePDF(const NumericalPoint & point) const
{
  const NumericalPoint epsilon(dimension_, pow(cdfEpsilon_, 1.0 / 3.0));
  pdfEpsilon_ = epsilon[0];
  // Centered finite differences of CDF
  return (computeCDF(point + epsilon) - computeCDF(point - epsilon)) / (2.0 * pdfEpsilon_);
}

NumericalScalar DistributionImplementation::computeLogPDF(const NumericalPoint & point) const
{
  const NumericalScalar pdf(computePDF(point));
  NumericalScalar logPdf(-SpecFunc::MaxNumericalScalar);
  if ( pdf > 0.0 ) logPdf = log(pdf);
  return logPdf;
}

/* Get the CDF, complementary CDF and survival function of the distributionImplementation */
/* On a NumericalPoint */
NumericalScalar DistributionImplementation::computeCDF(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}

NumericalScalar DistributionImplementation::computeComplementaryCDF(const NumericalPoint & point) const
{
  return 0.5 + (0.5 - computeCDF(point));
}

/** Computation of the survival function:
    A_k = \{X_k > x_k\}, k=0..n-1
    P(\cap A_k) = 1 - \bar{P}(\cap A_k)
    = 1 + \sum_{j=0}^{n-1}(-1)^j\sum_{\{k_0,\dots,k_{j-1}\}\subset\{0,\dots,n-1\}P(\bar{A}_{k_0},\dots,\bar{A}_{k_{j-1}})
    so
    n=1:
    P(X_1>x_1) = 1 - P(X_1\leq x_1)

    n=2:
    P(X_1>x_1, X_2>x_2) = 1 - P(X_1\leq x_1) - P(X_2\leq x_2) + P(X_1\leq x_1, X_2\leq x_2)

    n=3:
    P(X_1>x_1, X_2>x_2, X_3>x_3) = 1 - P(X_1\leq x_1) - P(X_2\leq x_2) - P(X_3\leq x_3) + P(X_1\leq x_1, X_2\leq x_2) + P(X_1\leq x_1, X_3\leq x_3) + P(X_2\leq x_2, X_3\leq x_3) - P(X_1\leq x_1, X_2\leq x_2, X_3\leq x_3)
*/
NumericalScalar DistributionImplementation::computeSurvivalFunction(const NumericalPoint & point) const
{
  if (dimension_ == 1) return computeComplementaryCDF(point);
  const NumericalPoint lowerBounds(getRange().getLowerBound());
  const NumericalPoint upperBounds(getRange().getUpperBound());
  Bool allOutside(true);
  for (UnsignedInteger i = 0; i < dimension_; ++i)
  {
    if (point[i] >= upperBounds[i]) return 0.0;
    allOutside &= (point[i] <= lowerBounds[i]);
  }
  if (allOutside) return 1.0;

  // Use Poincaré's formula
  const NumericalScalar cdf(computeCDF(point));
  NumericalScalar value(1.0 + (dimension_ % 2 == 0 ? cdf : -cdf));
  NumericalScalar sign(-1.0);
  for (UnsignedInteger i = 1; i < dimension_; ++i)
  {
    NumericalScalar contribution(0.0);
    Combinations::IndicesCollection indices(Combinations(i, dimension_).generate());
    NumericalPoint subPoint(i);
    for (UnsignedInteger j = 0; j < indices.getSize(); ++j)
    {
      for (UnsignedInteger k = 0; k < i; ++k) subPoint[k] = point[indices[j][k]];
      contribution += getMarginal(indices[j])->computeCDF(subPoint);
    }
    value += sign * contribution;
    sign = -sign;
  }
  return value;
}

/* On a NumericalSample */
NumericalSample DistributionImplementation::computeCDFSequential(const NumericalSample & inSample) const
{
  const UnsignedInteger size(inSample.getSize());
  NumericalSample outSample(size, 1);
  for (UnsignedInteger i = 0; i < size; ++i) outSample[i][0] = computeCDF(inSample[i]);
  return outSample;
}

struct ComputeCDFPolicy
{
  const NumericalSample & input_;
  NumericalSample & output_;
  const DistributionImplementation & distribution_;

  ComputeCDFPolicy( const NumericalSample & input,
                    NumericalSample & output,
                    const DistributionImplementation & distribution)
    : input_(input)
    , output_(output)
    , distribution_(distribution)
  {}

  inline void operator()( const TBB::BlockedRange<UnsignedInteger> & r ) const
  {
    for (UnsignedInteger i = r.begin(); i != r.end(); ++i) output_[i][0] = distribution_.computeCDF(input_[i]);
  }

}; /* end struct ComputeCDFPolicy */

NumericalSample DistributionImplementation::computeCDFParallel(const NumericalSample & inSample) const
{
  if (inSample.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given sample has an invalid dimension. Expect a dimension " << dimension_ << ", got " << inSample.getDimension();
  const UnsignedInteger size(inSample.getSize());
  NumericalSample result(size, 1);
  const ComputeCDFPolicy policy( inSample, result, *this );
  TBB::ParallelFor( 0, size, policy );
  return result;
}

NumericalSample DistributionImplementation::computeCDF(const NumericalSample & inSample) const
{
  if (inSample.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given sample has an invalid dimension. Expect a dimension " << dimension_ << ", got " << inSample.getDimension();
  if (isParallel_) return computeCDFParallel(inSample);
  else return computeCDFSequential(inSample);
}

NumericalSample DistributionImplementation::computeComplementaryCDFSequential(const NumericalSample & inSample) const
{
  const UnsignedInteger size(inSample.getSize());
  NumericalSample outSample(size, 1);
  for (UnsignedInteger i = 0; i < size; ++i) outSample[i][0] = computeComplementaryCDF(inSample[i]);
  return outSample;
}

struct ComputeComplementaryCDFPolicy
{
  const NumericalSample & input_;
  NumericalSample & output_;
  const DistributionImplementation & distribution_;

  ComputeComplementaryCDFPolicy( const NumericalSample & input,
                                 NumericalSample & output,
                                 const DistributionImplementation & distribution)
    : input_(input)
    , output_(output)
    , distribution_(distribution)
  {}

  inline void operator()( const TBB::BlockedRange<UnsignedInteger> & r ) const
  {
    for (UnsignedInteger i = r.begin(); i != r.end(); ++i) output_[i][0] = distribution_.computeComplementaryCDF(input_[i]);
  }

}; /* end struct ComputeComplementaryCDFPolicy */

NumericalSample DistributionImplementation::computeComplementaryCDFParallel(const NumericalSample & inSample) const
{
  if (inSample.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given sample has an invalid dimension. Expect a dimension " << dimension_ << ", got " << inSample.getDimension();
  const UnsignedInteger size(inSample.getSize());
  NumericalSample result(size, 1);
  const ComputeComplementaryCDFPolicy policy( inSample, result, *this );
  TBB::ParallelFor( 0, size, policy );
  return result;
}

NumericalSample DistributionImplementation::computeComplementaryCDF(const NumericalSample & inSample) const
{
  if (inSample.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given sample has an invalid dimension. Expect a dimension " << dimension_ << ", got " << inSample.getDimension();
  if (isParallel_) return computeComplementaryCDFParallel(inSample);
  else return computeComplementaryCDFSequential(inSample);
}

NumericalSample DistributionImplementation::computeSurvivalFunctionSequential(const NumericalSample & inSample) const
{
  const UnsignedInteger size(inSample.getSize());
  NumericalSample outSample(size, 1);
  for (UnsignedInteger i = 0; i < size; ++i) outSample[i][0] = computeSurvivalFunction(inSample[i]);
  return outSample;
}

struct ComputeSurvivalFunctionPolicy
{
  const NumericalSample & input_;
  NumericalSample & output_;
  const DistributionImplementation & distribution_;

  ComputeSurvivalFunctionPolicy( const NumericalSample & input,
                                 NumericalSample & output,
                                 const DistributionImplementation & distribution)
    : input_(input)
    , output_(output)
    , distribution_(distribution)
  {}

  inline void operator()( const TBB::BlockedRange<UnsignedInteger> & r ) const
  {
    for (UnsignedInteger i = r.begin(); i != r.end(); ++i) output_[i][0] = distribution_.computeSurvivalFunction(input_[i]);
  }

}; /* end struct ComputeSurvivalFunctionPolicy */

NumericalSample DistributionImplementation::computeSurvivalFunctionParallel(const NumericalSample & inSample) const
{
  if (inSample.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given sample has an invalid dimension. Expect a dimension " << dimension_ << ", got " << inSample.getDimension();
  const UnsignedInteger size(inSample.getSize());
  NumericalSample result(size, 1);
  const ComputeSurvivalFunctionPolicy policy( inSample, result, *this );
  TBB::ParallelFor( 0, size, policy );
  return result;
}

NumericalSample DistributionImplementation::computeSurvivalFunction(const NumericalSample & inSample) const
{
  if (inSample.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given sample has an invalid dimension. Expect a dimension " << dimension_ << ", got " << inSample.getDimension();
  if (isParallel_) return computeSurvivalFunctionParallel(inSample);
  else return computeSurvivalFunctionSequential(inSample);
}

/* Compute the probability content of an interval */
NumericalScalar DistributionImplementation::computeProbability(const Interval & interval) const
{
  if (dimension_ == 1)
  {
    // Empty interval
    if (interval.isNumericallyEmpty()) return 0.0;
    NumericalScalar upperCDF(1.0);
    NumericalScalar lowerCDF(0.0);
    if (interval.getFiniteLowerBound()[0]) lowerCDF = computeCDF(interval.getLowerBound());
    // Use the tail probability if the left endpoint of the interval is greater than the median, for an improved accuracy
    const Bool tail(lowerCDF > 0.5);
    if (tail) lowerCDF = computeComplementaryCDF(interval.getLowerBound());
    if (interval.getFiniteUpperBound()[0]) upperCDF = tail ? computeComplementaryCDF(interval.getUpperBound()) : computeCDF(interval.getUpperBound());
    // If we used the tail probability
    if (tail) return lowerCDF - upperCDF;
    // Classical case
    return upperCDF - lowerCDF;
  }
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::computeProbability()";
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex DistributionImplementation::computeCharacteristicFunction(const NumericalScalar x) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "Error:  cannot use the computeCharacteristicFunction method with distributions of dimension > 1";
  NumericalComplex value(0.0);
  // In the continuous case, we use simple gauss integration with a fixed number of integration points. We divide the interval in order to have a sufficient number of integration points by interval. It is neither accurate nor efficient. We should use some specific integration methods such as Filon's or Levin's methods.
  if (isContinuous())
  {
    // The circular function will have x(b-a)/2\pi arches over [a, b], so we need a number of points of this order, we decide to take 8 points per arch
    const NumericalSample legendreNodesAndWeights(getGaussNodesAndWeights());
    // How many sub-intervals?
    // nPts = 8*x(b-a)/2\pi => (b-a)/2 = nPts * \pi / (8*x)
    const NumericalScalar xMin(range_.getLowerBound()[0]);
    const NumericalScalar xMax(range_.getUpperBound()[0]);
    const NumericalScalar delta(xMax - xMin);
    const UnsignedInteger intervalsNumber(std::max(1, static_cast<int>(round(2 * x * delta / integrationNodesNumber_))));
    // Here, we should adopt a specific method for the case of highly oscilating integrand, i.e. large x
    const NumericalScalar halfLength(0.5 * delta / intervalsNumber);
    for (UnsignedInteger n = 0; n < intervalsNumber; ++n)
    {
      const NumericalScalar a(xMin + 2.0 * n * halfLength);
      for (UnsignedInteger i = 0; i < integrationNodesNumber_; ++i)
      {
        const NumericalScalar xi(a + (1.0 + legendreNodesAndWeights[0][i]) * halfLength);
        value += legendreNodesAndWeights[1][i] * computePDF(xi) * exp(NumericalComplex(0.0, x * xi));
      }
    }
    // We factor out the scaling as all the sub intervals have the same length
    value *= halfLength;
  }
  else
  {
    // In the discrete case, we have a reasonably efficient algorithm both in term of speed and precision.
    if (isDiscrete())
    {
      const NumericalSample support(getSupport());
      const UnsignedInteger size(support.getSize());
      for (UnsignedInteger i = 0; i < size; ++i)
      {
        const NumericalScalar pt(support[i][0]);
        value += computePDF(pt) * exp(NumericalComplex(0.0, x * pt));
      }
    }
    // In the composite case, no default algorithm
    else
    {
      throw NotYetImplementedException(HERE) << "Error: no default algorithm to compute the characteristic function in the composite case.";
    }
  }
  return value;
}

NumericalComplex DistributionImplementation::computeCharacteristicFunction(const NumericalPoint & x) const
{
  if (dimension_ == 1) return computeCharacteristicFunction(x[0]);
  throw NotYetImplementedException(HERE);
}

NumericalComplex DistributionImplementation::computeLogCharacteristicFunction(const NumericalScalar x) const
{
  const NumericalComplex value(computeCharacteristicFunction(x));
  const NumericalComplex result(log(value));
  return result;
}

NumericalComplex DistributionImplementation::computeLogCharacteristicFunction(const NumericalPoint & x) const
{
  if (dimension_ == 1) return computeLogCharacteristicFunction(x[0]);
  throw NotYetImplementedException(HERE);
}

NumericalComplex DistributionImplementation::computeCharacteristicFunction(const UnsignedInteger index,
    const NumericalScalar step) const
{
  return computeCharacteristicFunction(index * step);
}

NumericalComplex DistributionImplementation::computeLogCharacteristicFunction(const UnsignedInteger index,
    const NumericalScalar step) const
{
  return computeLogCharacteristicFunction(index * step);
}

NumericalComplex DistributionImplementation::computeCharacteristicFunction(const Indices & indices,
    const NumericalPoint & step) const
{
  NumericalPoint point(dimension_);
  for (UnsignedInteger i = 0; i < dimension_; ++i) point[i] = indices[i] * step[i];
  return computeCharacteristicFunction(point);
}

NumericalComplex DistributionImplementation::computeLogCharacteristicFunction(const Indices & indices,
    const NumericalPoint & step) const
{
  NumericalPoint point(dimension_);
  for (UnsignedInteger i = 0; i < dimension_; ++i) point[i] = indices[i] * step[i];
  return computeLogCharacteristicFunction(point);
}

/* Get the generating function of the distribution, i.e. psi(z) = E(z^X) */
NumericalScalar DistributionImplementation::computeGeneratingFunction(const NumericalScalar z) const
{
  return computeGeneratingFunction(NumericalComplex(z, 0.0)).real();
}

NumericalScalar DistributionImplementation::computeLogGeneratingFunction(const NumericalScalar z) const
{
  return computeLogGeneratingFunction(NumericalComplex(z, 0.0)).real();
}

NumericalComplex DistributionImplementation::computeGeneratingFunction(const NumericalComplex & z) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "Error:  cannot use the computeCharacteristicFunction method with distributions of dimension > 1";
  if (!isDiscrete()) throw NotDefinedException(HERE) << "Error: cannot compute the generating function for non discrete distributions.";
  const NumericalSample support(getSupport());
  const UnsignedInteger size(support.getSize());
  NumericalComplex value(0.0);
  // If the distribution is integral, the generating function is either a polynomial if the support is finite, or can be well approximated by such a polynomial
  if (isAlreadyCreatedGeneratingFunction_) value = generatingFunction_(z);
  else
    // If isIntegral, then we have to create the generating function as a polynomial
    if (isIntegral())
    {
      NumericalPoint coefficients(size);
      for (UnsignedInteger i = 0; i < size; ++i) coefficients[i] = computePDF(support[i]);
      generatingFunction_ = UniVariatePolynomial(coefficients);
      isAlreadyCreatedGeneratingFunction_ = true;
      value = generatingFunction_(z);
    }
  // The distribution is discrete but not integral
    else
    {
      for (UnsignedInteger i = 0; i < size; ++i)
      {
        const NumericalScalar pt(support[i][0]);
        value += computePDF(pt) * pow(z, pt);
      }
    }
  return value;
}

NumericalComplex DistributionImplementation::computeLogGeneratingFunction(const NumericalComplex & z) const
{
  NumericalComplex value = computeGeneratingFunction(z);
  return log(value);
}

/* Get the DDF of the distributionImplementation */
NumericalSample DistributionImplementation::computeDDFSequential(const NumericalSample & inSample) const
{
  const UnsignedInteger size(inSample.getSize());
  NumericalSample outSample(size, 1);
  for (UnsignedInteger i = 0; i < size; ++i) outSample[i] = computeDDF(inSample[i]);
  return outSample;
}

/* Get the DDF of the distributionImplementation */
struct ComputeDDFPolicy
{
  const NumericalSample & input_;
  NumericalSample & output_;
  const DistributionImplementation & distribution_;

  ComputeDDFPolicy( const NumericalSample & input,
                    NumericalSample & output,
                    const DistributionImplementation & distribution)
    : input_(input)
    , output_(output)
    , distribution_(distribution)
  {}

  inline void operator()( const TBB::BlockedRange<UnsignedInteger> & r ) const
  {
    for (UnsignedInteger i = r.begin(); i != r.end(); ++i) output_[i] = distribution_.computeDDF(input_[i]);
  }

}; /* end struct ComputeDDFPolicy */

NumericalSample DistributionImplementation::computeDDFParallel(const NumericalSample & inSample) const
{
  if (inSample.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given sample has an invalid dimension. Expect a dimension " << dimension_ << ", got " << inSample.getDimension();
  const UnsignedInteger size(inSample.getSize());
  NumericalSample result(size, 1);
  const ComputeDDFPolicy policy( inSample, result, *this );
  TBB::ParallelFor( 0, size, policy );
  return result;
}

NumericalSample DistributionImplementation::computeDDF(const NumericalSample & inSample) const
{
  if (inSample.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given sample has an invalid dimension. Expect a dimension " << dimension_ << ", got " << inSample.getDimension();
  if (isParallel_) return computeDDFParallel(inSample);
  else return computeDDFSequential(inSample);
}

/* Get the PDF of the distributionImplementation */
NumericalSample DistributionImplementation::computePDFSequential(const NumericalSample & inSample) const
{
  const UnsignedInteger size(inSample.getSize());
  NumericalSample outSample(size, 1);
  for (UnsignedInteger i = 0; i < size; ++i) outSample[i][0] = computePDF(inSample[i]);
  return outSample;
}

/* Get the PDF of the distributionImplementation */
struct ComputePDFPolicy
{
  const NumericalSample & input_;
  NumericalSample & output_;
  const DistributionImplementation & distribution_;

  ComputePDFPolicy( const NumericalSample & input,
                    NumericalSample & output,
                    const DistributionImplementation & distribution)
    : input_(input)
    , output_(output)
    , distribution_(distribution)
  {}

  inline void operator()( const TBB::BlockedRange<UnsignedInteger> & r ) const
  {
    for (UnsignedInteger i = r.begin(); i != r.end(); ++i) output_[i][0] = distribution_.computePDF(input_[i]);
  }

}; /* end struct ComputePDFPolicy */

NumericalSample DistributionImplementation::computePDFParallel(const NumericalSample & inSample) const
{
  if (inSample.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given sample has an invalid dimension. Expect a dimension " << dimension_ << ", got " << inSample.getDimension();
  const UnsignedInteger size(inSample.getSize());
  NumericalSample result(size, 1);
  const ComputePDFPolicy policy( inSample, result, *this );
  TBB::ParallelFor( 0, size, policy );
  return result;
}

NumericalSample DistributionImplementation::computePDF(const NumericalSample & inSample) const
{
  if (inSample.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given sample has an invalid dimension. Expect a dimension " << dimension_ << ", got " << inSample.getDimension();
  if (isParallel_) return computePDFParallel(inSample);
  else return computePDFSequential(inSample);
}

/* Get the Log PDF of the distributionImplementation */
NumericalSample DistributionImplementation::computeLogPDFSequential(const NumericalSample & inSample) const
{
  const UnsignedInteger size(inSample.getSize());
  NumericalSample outSample(size, 1);
  for (UnsignedInteger i = 0; i < size; ++i) outSample[i][0] = computeLogPDF(inSample[i]);
  return outSample;
}

/* Get the LogPDF of the distributionImplementation */
struct ComputeLogPDFPolicy
{
  const NumericalSample & input_;
  NumericalSample & output_;
  const DistributionImplementation & distribution_;

  ComputeLogPDFPolicy( const NumericalSample & input,
                       NumericalSample & output,
                       const DistributionImplementation & distribution)
    : input_(input)
    , output_(output)
    , distribution_(distribution)
  {}

  inline void operator()( const TBB::BlockedRange<UnsignedInteger> & r ) const
  {
    for (UnsignedInteger i = r.begin(); i != r.end(); ++i) output_[i][0] = distribution_.computeLogPDF(input_[i]);
  }

}; /* end struct ComputeLogPDFPolicy */

NumericalSample DistributionImplementation::computeLogPDFParallel(const NumericalSample & inSample) const
{
  if (inSample.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given sample has an invalid dimension. Expect a dimension " << dimension_ << ", got " << inSample.getDimension();
  const UnsignedInteger size(inSample.getSize());
  NumericalSample result(size, 1);
  const ComputeLogPDFPolicy policy( inSample, result, *this );
  TBB::ParallelFor( 0, size, policy );
  return result;
}

NumericalSample DistributionImplementation::computeLogPDF(const NumericalSample & inSample) const
{
  if (inSample.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given sample has an invalid dimension. Expect a dimension " << dimension_ << ", got " << inSample.getDimension();
  if (isParallel_) return computeLogPDFParallel(inSample);
  else return computeLogPDFSequential(inSample);
}

/* Get the DDF of the distributionImplementation */
NumericalScalar DistributionImplementation::computeDDF(const NumericalScalar scalar) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "ERROR: cannot use the simplified interface of computeDDF with distributions of dimension > 1";
  return computeDDF(NumericalPoint(1, scalar))[0];
}

/* Get the PDF of the distributionImplementation */
NumericalScalar DistributionImplementation::computePDF(const NumericalScalar scalar) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "ERROR: cannot use the simplified interface of computePDF with distributions of dimension > 1";
  return computePDF(NumericalPoint(1, scalar));
}

NumericalScalar DistributionImplementation::computeLogPDF(const NumericalScalar scalar) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "ERROR: cannot use the simplified interface of computeLogPDF with distributions of dimension > 1";
  return computeLogPDF(NumericalPoint(1, scalar));
}

/* Get the CDF of the distributionImplementation */
NumericalScalar DistributionImplementation::computeCDF(const NumericalScalar scalar) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "ERROR: cannot use the simplified interface of computeCDF with distributions of dimension > 1";
  return computeCDF(NumericalPoint(1, scalar));
}

NumericalScalar DistributionImplementation::computeComplementaryCDF(const NumericalScalar scalar) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "ERROR: cannot use the simplified interface of computeComplementaryCDF with distributions of dimension > 1";
  return computeComplementaryCDF(NumericalPoint(1, scalar));
}

NumericalScalar DistributionImplementation::computeSurvivalFunction(const NumericalScalar scalar) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "ERROR: cannot use the simplified interface of computeSurvivalFunction with distributions of dimension > 1";
  return computeSurvivalFunction(NumericalPoint(1, scalar));
}

/* Compute the PDF of 1D distributions over a regular grid */
NumericalSample DistributionImplementation::computePDF(const NumericalScalar xMin,
    const NumericalScalar xMax,
    const UnsignedInteger pointNumber,
    NumericalSample & grid) const
{
  return computePDF(NumericalPoint(1, xMin), NumericalPoint(1, xMax), Indices(1, pointNumber), grid);
}

/* Compute the PDF of nD distributions over a regular grid */
NumericalSample DistributionImplementation::computePDF(const NumericalPoint & xMin,
    const NumericalPoint & xMax,
    const Indices & pointNumber,
    NumericalSample & grid) const
{
  if (xMin.getDimension() != xMax.getDimension()) throw InvalidArgumentException(HERE) << "Error: the two corner points must have the same dimension. Here, dim(xMin)=" << xMin.getDimension() << " and dim(xMax)=" << xMax.getDimension();
  if (xMin.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the corner points must have the same dimension as the distribution. Here, dim(xMin)=" << xMin.getDimension() << " and distribution dimension=" << dimension_;
  if (dimension_ != pointNumber.getSize()) throw InvalidArgumentException(HERE) << "Error: the discretization must match the distribution dimension. Here, dim(discretization)=" << pointNumber.getSize() << " and distribution dimension=" << dimension_;
  Tuples::IndicesCollection indices(Tuples(pointNumber).generate());
  const UnsignedInteger size(indices.getSize());
  NumericalSample inputSample(indices.getSize(), dimension_);
  for (UnsignedInteger i = 0; i < size; ++i)
    for (UnsignedInteger j = 0; j < dimension_; ++j) inputSample[i][j] = xMin[j] + indices[i][j] * (xMax[j] - xMin[j]) / (pointNumber[j] - 1.0);
  grid = inputSample;
  return computePDF(inputSample);
}

/* Compute the CDF of 1D distributions over a regular grid */
NumericalSample DistributionImplementation::computeCDF(const NumericalScalar xMin,
    const NumericalScalar xMax,
    const UnsignedInteger pointNumber,
    NumericalSample & grid) const
{
  return computeCDF(NumericalPoint(1, xMin), NumericalPoint(1, xMax), Indices(1, pointNumber), grid);
}

/* Compute the CDF of nD distributions over a regular grid */
NumericalSample DistributionImplementation::computeCDF(const NumericalPoint & xMin,
    const NumericalPoint & xMax,
    const Indices & pointNumber,
    NumericalSample & grid) const
{
  if (xMin.getDimension() != xMax.getDimension()) throw InvalidArgumentException(HERE) << "Error: the two corner points must have the same dimension. Here, dim(xMin)=" << xMin.getDimension() << " and dim(xMax)=" << xMax.getDimension();
  if (xMin.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the corner points must have the same dimension as the distribution. Here, dim(xMin)=" << xMin.getDimension() << " and distribution dimension=" << dimension_;
  if (dimension_ != pointNumber.getSize()) throw InvalidArgumentException(HERE) << "Error: the discretization must match the distribution dimension. Here, dim(discretization)=" << pointNumber.getSize() << " and distribution dimension=" << dimension_;
  Tuples::IndicesCollection indices(Tuples(pointNumber).generate());
  const UnsignedInteger size(indices.getSize());
  NumericalSample inputSample(indices.getSize(), dimension_);
  for (UnsignedInteger i = 0; i < size; ++i)
    for (UnsignedInteger j = 0; j < dimension_; ++j) inputSample[i][j] = xMin[j] + indices[i][j] * (xMax[j] - xMin[j]) / (pointNumber[j] - 1.0);
  grid = inputSample;
  return computeCDF(inputSample);
}

NumericalSample DistributionImplementation::computeComplementaryCDF(const NumericalScalar xMin,
    const NumericalScalar xMax,
    const UnsignedInteger pointNumber,
    NumericalSample & grid) const
{
  if (dimension_ != 1) throw InvalidArgumentException(HERE) << "Error: cannot compute the CDF over a regular 1D grid if the dimension is > 1";
  NumericalSample result(pointNumber, 2);
  NumericalScalar x(xMin);
  NumericalScalar step((xMax - xMin) / NumericalScalar(pointNumber - 1.0));
  grid = NumericalSample(pointNumber, 1);
  for (UnsignedInteger i = 0; i < pointNumber; ++i)
  {
    grid[i][0] = x;
    result[i][0] = x;
    result[i][1] = computeComplementaryCDF(x);
    x += step;
  }
  return result;
}

/*  Compute the quantile over a regular grid */
NumericalSample DistributionImplementation::computeQuantile(const NumericalScalar qMin,
    const NumericalScalar qMax,
    const UnsignedInteger pointNumber,
    const Bool tail) const
{
  NumericalSample grid;
  return computeQuantile(qMin, qMax, pointNumber, grid, tail);
}

NumericalSample DistributionImplementation::computeQuantile(const NumericalScalar qMin,
    const NumericalScalar qMax,
    const UnsignedInteger pointNumber,
    NumericalSample & grid,
    const Bool tail) const
{
  // First, build the regular grid for the quantile levels
  grid = NumericalSample(pointNumber, 1);
  for (UnsignedInteger i = 0; i < pointNumber; ++i) grid[i][0] = qMin + i * (qMax - qMin) / (pointNumber - 1.0);
  // Use possible parallelization
  return computeQuantile(grid.getImplementation()->getData(), tail);
}

/* Compute the quantile over a provided grid */
NumericalSample DistributionImplementation::computeQuantileSequential(const NumericalPoint & prob,
    const Bool tail) const
{
  const UnsignedInteger size(prob.getSize());
  NumericalSample result(size, dimension_);
  for ( UnsignedInteger i = 0; i < size; ++ i ) result[i] = computeQuantile(prob[i], tail);
  return result;
}

struct ComputeQuantilePolicy
{
  const NumericalPoint & prob_;
  NumericalSample & output_;
  Bool tail_;
  const DistributionImplementation & distribution_;

  ComputeQuantilePolicy( const NumericalPoint & prob,
                         NumericalSample & output,
                         const Bool tail,
                         const DistributionImplementation & distribution)
    : prob_(prob)
    , output_(output)
    , tail_(tail)
    , distribution_(distribution)
  {}

  inline void operator()( const TBB::BlockedRange<UnsignedInteger> & r ) const
  {
    for (UnsignedInteger i = r.begin(); i != r.end(); ++i) output_[i] = distribution_.computeQuantile(prob_[i], tail_);
  }

}; /* end struct ComputeQuantilePolicy */

NumericalSample DistributionImplementation::computeQuantileParallel(const NumericalPoint & prob,
    const Bool tail) const
{
  const UnsignedInteger size(prob.getSize());
  NumericalSample result(size, dimension_);
  const ComputeQuantilePolicy policy( prob, result, tail, *this );
  TBB::ParallelFor( 0, size, policy );
  return result;
}

NumericalSample DistributionImplementation::computeQuantile(const NumericalPoint & prob,
    const Bool tail) const
{
  if (isParallel_) return computeQuantileParallel(prob, tail);
  else return computeQuantileSequential(prob, tail);
}

/* Get the PDF gradient of the distribution */
NumericalPoint DistributionImplementation::computePDFGradient(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::computePDFGradient()";
}

/* Get the CDF gradient of the distribution */
NumericalPoint DistributionImplementation::computeCDFGradient(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::computeCDFGradient()";
}

/* Build a C1 interpolation of the CDF function for 1D continuous distributions */
Collection<PiecewiseHermiteEvaluationImplementation> DistributionImplementation::interpolatePDFCDF(const UnsignedInteger n)
{
  if (!isContinuous()) throw InternalException(HERE) << "Error: cannot interpolate the PDF and CDF of noncontinuous distributions.";
  if (dimension_ != 1) throw NotYetImplementedException(HERE) << "Error: cannot interpolate CDF for multidimensional distributions.";
  const NumericalScalar xMin(getRange().getLowerBound()[0]);
  const NumericalScalar xMax(getRange().getUpperBound()[0]);
  const NumericalScalar mu(getMean()[0]);
  // Here we use an absolute precision of 0.0 in order to force the algorithm to use all the available discretization points
  GaussKronrod algorithm(n - 1, cdfEpsilon_ * cdfEpsilon_, GaussKronrodRule::G3K7);
  const PDFWrapper pdfWrapper(this);
  const NumericalMathFunction fPDF(bindMethod<PDFWrapper, NumericalPoint, NumericalPoint>(pdfWrapper, &PDFWrapper::computePDF, 1, 1));
  NumericalScalar error(-1.0);
  NumericalPoint ai;
  NumericalPoint bi;
  NumericalPoint fi;
  NumericalPoint ei;
  algorithm.integrate(fPDF, xMin, mu, error, ai, bi, fi, ei);
  ai.add(mu);
  NumericalSample locationsCDF(ai.getSize(), 1);
  locationsCDF.getImplementation()->setData(ai);
  locationsCDF = locationsCDF.sort();
  algorithm.integrate(fPDF, mu, xMax, error, ai, bi, fi, ei);
  ai.add(xMax);
  NumericalSample locationsCCDF(ai.getSize(), 1);
  locationsCCDF.getImplementation()->setData(ai);
  locationsCCDF = locationsCCDF.sort();
  Collection<PiecewiseHermiteEvaluationImplementation> coll(4);
  const NumericalSample valuesCDF(computeCDF(locationsCDF));
  const NumericalSample valuesPDF(computePDF(locationsCDF));
  const NumericalSample valuesDDF(computeDDF(locationsCDF));
  coll[0] = PiecewiseHermiteEvaluationImplementation(locationsCDF.getImplementation()->getData(), valuesPDF, valuesDDF);
  coll[1] = PiecewiseHermiteEvaluationImplementation(locationsCDF.getImplementation()->getData(), valuesCDF, valuesPDF);
  const NumericalSample valuesCCDF(computeComplementaryCDF(locationsCCDF));
  const NumericalSample valuesCPDF(computePDF(locationsCCDF));
  NumericalSample derivativesCCDF(valuesCPDF);
  derivativesCCDF.scale(NumericalPoint(1, -1.0));
  const NumericalSample valuesCDDF(computeDDF(locationsCCDF));
  coll[2] = PiecewiseHermiteEvaluationImplementation(locationsCCDF.getImplementation()->getData(), valuesCPDF, valuesCDDF);
  coll[3] = PiecewiseHermiteEvaluationImplementation(locationsCCDF.getImplementation()->getData(), valuesCCDF, derivativesCCDF);
  return coll;
}

/* Compute the DDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar DistributionImplementation::computeConditionalDDF(const NumericalScalar x,
    const NumericalPoint & y) const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::computeConditionalDDF()";
}

/* Compute the PDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar DistributionImplementation::computeConditionalPDF(const NumericalScalar x,
    const NumericalPoint & y) const
{
  const UnsignedInteger conditioningDimension(y.getDimension());
  if (conditioningDimension >= dimension_) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional PDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return getMarginal(conditioningDimension)->computePDF(x);
  // General case
  Indices conditioning(conditioningDimension);
  for (UnsignedInteger i = 0; i < conditioningDimension; ++i) conditioning[i] = i;
  Indices conditioned(conditioning);
  conditioned.add(conditioningDimension);
  const Implementation conditioningDistribution(getMarginal(conditioning));
  const NumericalScalar pdfConditioning(conditioningDistribution->computePDF(y));
  if (pdfConditioning <= 0.0) return 0.0;
  NumericalPoint z(y);
  z.add(x);
  const Implementation conditionedDistribution(getMarginal(conditioned));
  const NumericalScalar pdfConditioned(conditionedDistribution->computePDF(z));
  pdfEpsilon_ = conditionedDistribution->getPDFEpsilon() + conditioningDistribution->getPDFEpsilon();
  return pdfConditioned / pdfConditioning;
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar DistributionImplementation::computeConditionalCDF(const NumericalScalar x,
    const NumericalPoint & y) const
{
  const UnsignedInteger conditioningDimension(y.getDimension());
  if (conditioningDimension >= dimension_) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional CDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return getMarginal(conditioningDimension)->computeCDF(x);
  // General case
  Indices conditioning(conditioningDimension);
  conditioning.fill();
  Indices conditioned(conditioning);
  conditioned.add(conditioningDimension);
  const Implementation conditioningDistribution(getMarginal(conditioning));
  const NumericalScalar pdfConditioning(conditioningDistribution->computePDF(y));
  if (pdfConditioning <= 0.0) return 0.0;
  const Implementation conditionedDistribution(getMarginal(conditioned));
  const NumericalScalar xMin(conditionedDistribution->getRange().getLowerBound()[conditioningDimension]);
  if (x <= xMin) return 0.0;
  const NumericalScalar xMax(conditionedDistribution->getRange().getUpperBound()[conditioningDimension]);
  if (x >= xMax) return 1.0;
  // Numerical integration with respect to x
  ConditionalPDFWrapper wrapper(y, conditionedDistribution);
  const NumericalMathFunction f(bindMethod<ConditionalPDFWrapper, NumericalPoint, NumericalPoint>(wrapper, &ConditionalPDFWrapper::computeConditionalPDF, 1, 1));
  GaussKronrod algo;
  NumericalPoint ai;
  NumericalPoint bi;
  NumericalPoint fi;
  NumericalPoint ei;
  const NumericalScalar value(algo.integrate(f, xMin, x, cdfEpsilon_, ai, bi, fi, ei));
  return std::min(1.0, std::max(0.0, value / pdfConditioning));
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) with reuse of expansive data */
NumericalScalar DistributionImplementation::computeConditionalCDFForQuantile(const NumericalScalar x,
    const NumericalPoint & y,
    const Implementation & conditioningDistribution,
    const Implementation & conditionedDistribution,
    const NumericalScalar xMin) const
{
  const UnsignedInteger conditioningDimension(y.getDimension());
  const NumericalScalar pdfConditioning(conditioningDistribution->computePDF(y));
  if (pdfConditioning <= 0.0) return 0.0;
  // Numerical integration with respect to x
  NumericalPoint z(y);
  z.add(x);
  pdfEpsilon_ = conditionedDistribution->getPDFEpsilon() + conditioningDistribution->getPDFEpsilon();
  const NumericalSample legendreNodesAndWeights(getGaussNodesAndWeights());
  const NumericalScalar halfLength(0.5 * (x - xMin));
  cdfEpsilon_ = conditioningDistribution->getPDFEpsilon();
  NumericalScalar value(0.0);
  for (UnsignedInteger i = 0; i < integrationNodesNumber_; ++i)
  {
    const NumericalScalar xi(xMin + (1.0 + legendreNodesAndWeights[0][i]) * halfLength);
    z[conditioningDimension] = xi;
    value += legendreNodesAndWeights[1][i] * conditionedDistribution->computePDF(z);
    cdfEpsilon_ += legendreNodesAndWeights[1][i] * conditionedDistribution->getPDFEpsilon();
  }
  value *= (halfLength / pdfConditioning);
  return value;
}

/* Compute the PDF and CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar DistributionImplementation::computeConditionalPDFAndCDF(const NumericalScalar x,
    const NumericalPoint & y,
    NumericalScalar & cdf,
    const Implementation & conditioningDistribution,
    const Implementation & conditionedDistribution,
    const NumericalScalar xMin) const
{
  const UnsignedInteger conditioningDimension(y.getDimension());
  const NumericalScalar pdfConditioning(conditioningDistribution->computePDF(y));
  if (pdfConditioning <= 0.0)
  {
    cdf = 0.0;
    return 0.0;
  }
  // Numerical integration with respect to x
  NumericalPoint z(y);
  z.add(x);
  const NumericalScalar pdfConditioned(conditionedDistribution->computePDF(z));
  cdf = 0.0;
  const NumericalSample legendreNodesAndWeights(getGaussNodesAndWeights());
  const NumericalScalar halfLength(0.5 * (x - xMin));
  cdfEpsilon_ = conditioningDistribution->getPDFEpsilon();
  for (UnsignedInteger i = 0; i < integrationNodesNumber_; ++i)
  {
    const NumericalScalar xi(xMin + (1.0 + legendreNodesAndWeights[0][i]) * halfLength);
    z[conditioningDimension] = xi;
    cdf += legendreNodesAndWeights[1][i] * conditionedDistribution->computePDF(z);
    cdfEpsilon_ += legendreNodesAndWeights[1][i] * conditionedDistribution->getPDFEpsilon();
  }
  cdf *= (halfLength / pdfConditioning);
  return pdfConditioned / pdfConditioning;
}

/* Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
NumericalScalar DistributionImplementation::computeConditionalQuantile(const NumericalScalar q,
    const NumericalPoint & y) const
{
  const UnsignedInteger conditioningDimension(y.getDimension());
  if (conditioningDimension >= dimension_) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile with a conditioning point of dimension greater or equal to the distribution dimension.";
  if ((q < 0.0) || (q > 1.0)) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile for a probability level outside of [0, 1]";
  // Initialize the conditional quantile with the quantile of the i-th marginal distribution
  const Implementation marginalDistribution(getMarginal(conditioningDimension));
  NumericalScalar quantile(marginalDistribution->computeQuantile(q)[0]);
  // Special case for bording values
  if ((q == 0.0) || (q == 1.0)) return quantile;
  // Special case when no contitioning or independent copula
  if ((conditioningDimension == 0) || hasIndependentCopula()) return quantile;
  //NumericalScalar step(marginalDistribution->getDispersionIndicator());
  Indices conditioning(conditioningDimension);
  for (UnsignedInteger i = 0; i < conditioningDimension; ++i) conditioning[i] = i;
  Indices conditioned(conditioning);
  conditioned.add(conditioningDimension);
  const Implementation conditioningDistribution(getMarginal(conditioning));
  const Implementation conditionedDistribution(getMarginal(conditioned));
  const NumericalScalar xMin(conditionedDistribution->getRange().getLowerBound()[conditioningDimension]);
  const NumericalScalar xMax(conditionedDistribution->getRange().getUpperBound()[conditioningDimension]);
  // Start with the largest bracketing interval
  NumericalScalar a(xMin);
  NumericalScalar b(xMax);
  Bool convergence(false);
  NumericalScalar residual(0.0);
  UnsignedInteger iteration(0);
  while (!convergence && (iteration < quantileIterations_))
  {
    NumericalScalar cdf(0.0);
    const NumericalScalar pdf(computeConditionalPDFAndCDF(quantile, y, cdf, conditioningDistribution, conditionedDistribution, xMin));
    // Do we have to perform a bisection step?
    if ((pdf == 0.0) || (quantile > b) || (quantile < a))
    {
      quantile = 0.5 * (a + b);
      cdf = computeConditionalCDFForQuantile(quantile, y, conditioningDistribution, conditionedDistribution, xMin);
      if (cdf > q) b = quantile;
      else a = quantile;
    }
    else
    {
      // No, so do a Newton step
      residual = (q - cdf) / pdf;
      quantile += residual;
    }
    convergence = fabs(residual) < quantileEpsilon_ * (1.0 + fabs(quantile)) || (fabs(cdf - q) < 2.0 * cdfEpsilon_) || (b - a < quantileEpsilon_ * (1.0 + fabs(quantile)));
    ++iteration;
  }
  return quantile;
}

/* Quantile computation for dimension=1 */
NumericalScalar DistributionImplementation::computeScalarQuantile(const NumericalScalar prob,
    const Bool tail) const
{
  LOGDEBUG(OSS() << "DistributionImplementation::computeScalarQuantile: prob=" << prob << " tail=" << (tail ? "true" : "false"));
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "Error: the method computeScalarQuantile is only defined for 1D distributions";
  // This test allows to check if one can trust the current range. If not, it means that we are here to compute the range and then we cannot rely on it!
  NumericalScalar lower(range_.getLowerBound()[0]);
  NumericalScalar upper(range_.getUpperBound()[0]);
  // This test allows to know if the range has already been computed. If not, it is the role of the computeScalarQuantile() to do it.
  if (lower > upper)
  {
    // Find a rough estimate of the lower bound and the upper bound
    NumericalScalar step(1.0);
    if (computeCDF(lower) >= cdfEpsilon_)
    {
      // negative lower bound
      lower -= step;
      while (computeCDF(lower) >= cdfEpsilon_)
      {
        step *= 2.0;
        lower -= step;
      }
    }
    else
    {
      // positive lower bound
      lower += step;
      while (computeCDF(lower) <= cdfEpsilon_)
      {
        step *= 2.0;
        lower += step;
      }
    }
    // Here, lower is a rough estimate of the lower bound
    // Go to the upper bound
    upper = lower;
    step = 1.0;
    while (computeComplementaryCDF(upper) >= cdfEpsilon_)
    {
      upper += step;
      step *= 2.0;
    }
  }
  if (prob < 0.0) return (tail ? upper : lower);
  if (prob >= 1.0) return (tail ? lower : upper);
  const NumericalScalar q(tail ? 1.0 - prob : prob);
  const CDFWrapper wrapper(this);
  const NumericalMathFunction f(bindMethod<CDFWrapper, NumericalPoint, NumericalPoint>(wrapper, &CDFWrapper::computeCDF, 1, 1));
  const NumericalScalar leftTau(lower);
  const NumericalScalar leftCDF(0.0);
  const NumericalScalar rightTau(upper);
  const NumericalScalar rightCDF(1.0);
  Brent solver(quantileEpsilon_, cdfEpsilon_, cdfEpsilon_, quantileIterations_);
  const NumericalScalar root(solver.solve(f, q, leftTau, rightTau, leftCDF, rightCDF));
  return root;
} // computeScalarQuantile

/* Generic implementation of the quantile computation */
NumericalPoint DistributionImplementation::computeQuantile(const NumericalScalar prob,
    const Bool tail) const
{
  LOGDEBUG(OSS() << "DistributionImplementation::computeQuantile: prob=" << prob << ", tail=" << (tail ? "true" : "false"));
  // Special case for bording values
  if (prob < 0.0) return (tail ? range_.getUpperBound() : range_.getLowerBound());
  if (prob >= 1.0) return (tail ? range_.getLowerBound() : range_.getUpperBound());
  const NumericalScalar q(tail ? 1.0 - prob : prob);
  // Special case for dimension 1
  if (dimension_ == 1) return NumericalPoint(1, computeScalarQuantile(q));
  // Extract the marginal distributions
  Collection<Implementation> marginals(dimension_);
  for (UnsignedInteger i = 0; i < dimension_; i++) marginals[i] = getMarginal(i);
  // The n-D quantile is defined as X(\tau) = (F_1^{-1}(\tau), ..., F_n^{-1}(\tau)),
  // with tau such as F(X(\tau)) = q.
  // As F(x) = C(F_1(x_1),...,F_n(x_n)), the constraint F(X(\tau)) = q reads:
  // C(\tau,...,\tau) = q
  // Bracketing of \tau using the Frechet Hoeffding bounds:
  // max(n\tau - n + 1, 0) <= C(\tau,...,\tau) <= \tau
  // from which we deduce that q <= \tau and \tau <= 1 - (1 - q) / n
  // Lower bound of the bracketing interval
  QuantileWrapper wrapper(marginals, this);
  const NumericalMathFunction f(bindMethod<QuantileWrapper, NumericalPoint, NumericalPoint>(wrapper, &QuantileWrapper::computeDiagonal, 1, 1));
  NumericalScalar leftTau(q);
  NumericalScalar leftCDF(f(NumericalPoint(1, leftTau))[0]);
  // Due to numerical precision issues, the theoretical bound can be slightly violated
  if (leftCDF > prob)
  {
    leftTau = 0.0;
    leftCDF = 0.0;
  }
  // Upper bound of the bracketing interval
  NumericalScalar rightTau(1.0 - (1.0 - q) / dimension_);
  NumericalScalar rightCDF(f(NumericalPoint(1, rightTau))[0]);
  // Due to numerical precision issues, the theoretical bound can be slightly violated
  if (rightCDF < prob)
  {
    rightTau = 1.0;
    rightCDF = 1.0;
  }
  LOGDEBUG(OSS() << "DistributionImplementation::computeQuantile: dimension=" << dimension_ << ", q=" << q << ", leftTau=" << leftTau << ", leftCDF=" << leftCDF << ", rightTau=" << rightTau << ", rightCDF=" << rightCDF);
  // Use Brent's method to compute the quantile efficiently for continuous distributions
  const Brent solver(quantileEpsilon_, cdfEpsilon_, cdfEpsilon_, quantileIterations_);
  const NumericalScalar tau(solver.solve(f, q, leftTau, rightTau, leftCDF, rightCDF));
  LOGINFO(OSS(false) << "tau=" << tau);
  return wrapper.diagonalToSpace(tau);
}

/* Get the mathematical and numerical range of the distribution.
   Its mathematical range is the smallest closed interval outside
   of which the PDF is zero, and the numerical range is the interval
   outside of which the PDF is rounded to zero in double precision */
Interval DistributionImplementation::getRange() const
{
  return range_;
}

void DistributionImplementation::setRange(const Interval & range)
{
  if (range.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given range has a dimension incompatible with the dimension of the distribution.";
  range_ = range;
}

/* Compute the numerical range of the distribution given the parameters values */
void DistributionImplementation::computeRange()
{
  const Interval::BoolCollection finiteLowerBound(dimension_, false);
  const Interval::BoolCollection finiteUpperBound(dimension_, false);
  // Initialize the range with inverted bounds in order to inform the generic implementation of the
  // computeScalarQuantile() method that it cannot trust the current range
  range_ = Interval(NumericalPoint(dimension_, 1.0), NumericalPoint(dimension_, -1.0), finiteLowerBound, finiteUpperBound);
  setRange(Interval(computeLowerBound(), computeUpperBound(), finiteLowerBound, finiteUpperBound));
}

/* Compute the lower bound of the range */
NumericalPoint DistributionImplementation::computeLowerBound() const
{
  // For a multivariate distribution, the range is the axes aligned box that fits to the marginal ranges
  NumericalPoint lowerBound(dimension_);
  // Here, we must separate the 1D case from the nD case as the getMarginal() method is generic for 1D case and
  // would involve a circular call to computeRange()
  if (dimension_ == 1) lowerBound[0] = computeScalarQuantile(cdfEpsilon_);
  else for (UnsignedInteger i = 0; i < dimension_; ++i) lowerBound[i] = getMarginal(i)->computeScalarQuantile(cdfEpsilon_);
  return lowerBound;
}

/* Compute the upper bound of the range */
NumericalPoint DistributionImplementation::computeUpperBound() const
{
  // For a multivariate distribution, the range is the axes aligned box that fits to the marginal ranges
  NumericalPoint upperBound(dimension_);
  if (dimension_ == 1) upperBound[0] = computeScalarQuantile(cdfEpsilon_, true);
  else for (UnsignedInteger i = 0; i < dimension_; ++i) upperBound[i] = getMarginal(i)->computeScalarQuantile(cdfEpsilon_, true);
  return upperBound;
}

/* Compute the mean of the distribution */
void DistributionImplementation::computeMean() const
{
  mean_ = computeShiftedMoment(1, NumericalPoint(getDimension(), 0.0));
  isAlreadyComputedMean_ = true;
}

/* Get the mean of the distribution */
NumericalPoint DistributionImplementation::getMean() const
{
  if (!isAlreadyComputedMean_) computeMean();
  return mean_;
}

/* Get the standard deviation of the distribution */
NumericalPoint DistributionImplementation::getStandardDeviation() const
{
  const NumericalPoint variance(getCenteredMoment(2));
  NumericalPoint result(dimension_);
  for (UnsignedInteger i = 0; i < dimension_; ++i) result[i] = sqrt(variance[i]);
  return result;
}

/* Get the skewness of the distribution */
NumericalPoint DistributionImplementation::getSkewness() const
{
  const NumericalPoint variance(getCenteredMoment(2));
  const NumericalPoint thirdMoment(getCenteredMoment(3));
  NumericalPoint result(dimension_);
  for (UnsignedInteger i = 0; i < dimension_; ++i) result[i] = thirdMoment[i] / pow(variance[i], 1.5);
  return result;
}

/* Get the kurtosis of the distribution */
NumericalPoint DistributionImplementation::getKurtosis() const
{
  const NumericalPoint variance(getCenteredMoment(2));
  const NumericalPoint fourthMoment(getCenteredMoment(4));
  NumericalPoint result(dimension_);
  for (UnsignedInteger i = 0; i < dimension_; ++i) result[i] = fourthMoment[i] / pow(variance[i], 2.0);
  return result;
}

/* Get the moments of the distribution */
NumericalPoint DistributionImplementation::getMoment(const UnsignedInteger n) const
{
  if (n == 0) return NumericalPoint(dimension_, 1.0);
  return computeShiftedMoment(n, NumericalPoint(dimension_, 0.0));
}

/* Get the centered moments of the distribution */
NumericalPoint DistributionImplementation::getCenteredMoment(const UnsignedInteger n) const
{
  if (n == 0) throw InvalidArgumentException(HERE) << "Error: the centered moments of order 0 are undefined.";
  if (n == 1) return NumericalPoint(dimension_, 0.0);
  return computeShiftedMoment(n, getMean());
}

/* Compute the covariance of the distribution */
void DistributionImplementation::computeCovariance() const
{
  if (isContinuous()) computeCovarianceContinuous();
  else if (isDiscrete()) computeCovarianceDiscrete();
  else computeCovarianceGeneral();
}

void DistributionImplementation::computeCovarianceContinuous() const
{
  // We need this to initialize the covariance matrix in two cases:
  // + this is the first call to this routine (which could be checked by testing the dimension of the distribution and the dimension of the matrix
  // + the copula has changed from a non-independent one to the independent copula
  covariance_ = CovarianceMatrix(dimension_);
  // First the diagonal terms, which are the marginal covariances
  // Marginal covariances
  const NumericalPoint variance(getCenteredMoment(2));
  for(UnsignedInteger component = 0; component < dimension_; ++component) covariance_(component, component) = variance[component];
  // Off-diagonal terms if the copula is not the independent copula
  if (!hasIndependentCopula())
  {
    // To ensure that the mean is up to date
    mean_ = getMean();
    // Performs the integration for each covariance in the strictly lower triangle of the covariance matrix
    // We simply use a product gauss quadrature
    // We first loop over the coefficients because the most expensive task is to get the 2D marginal distributions

    // Compute the conditional CDF by numerical integration of the conditional PDF. We use a fixed point Gauss integration.
    const NumericalSample nodesAndWeights(getGaussNodesAndWeights());
    Indices indices(2);
    for(UnsignedInteger rowIndex = 0; rowIndex < dimension_; ++rowIndex)
    {
      indices[0] = rowIndex;
      for(UnsignedInteger columnIndex = rowIndex + 1; columnIndex < dimension_; ++columnIndex)
      {
        indices[1] = columnIndex;
        const Implementation marginalDistribution(getMarginal(indices));
        if (!marginalDistribution->hasIndependentCopula())
        {
          const NumericalScalar aI(marginalDistribution->getRange().getLowerBound()[0]);
          const NumericalScalar bI(marginalDistribution->getRange().getUpperBound()[0]);
          const NumericalScalar aJ(marginalDistribution->getRange().getLowerBound()[1]);
          const NumericalScalar bJ(marginalDistribution->getRange().getUpperBound()[1]);
          const NumericalScalar halfLengthI(0.5 * (bI - aI));
          const NumericalScalar halfLengthJ(0.5 * (bJ - aJ));
          NumericalScalar covarianceIJ(0.0);
          // Then we loop over the integration points
          for(UnsignedInteger rowNodeIndex = 0; rowNodeIndex < integrationNodesNumber_; ++rowNodeIndex)
          {
            const NumericalScalar nodeI(aI + (1.0 + nodesAndWeights[0][rowNodeIndex]) * halfLengthI);
            const NumericalScalar xI(nodeI - mean_[rowNodeIndex]);
            const NumericalScalar weightI(nodesAndWeights[1][rowNodeIndex]);
            for(UnsignedInteger columnNodeIndex = 0; columnNodeIndex < integrationNodesNumber_; ++columnNodeIndex)
            {
              const NumericalScalar nodeJ(aJ + (1.0 + nodesAndWeights[0][columnIndex]) * halfLengthJ);
              const NumericalScalar xJ(nodeJ - mean_[columnNodeIndex]);
              const NumericalScalar weightJ(nodesAndWeights[1][columnNodeIndex]);
              NumericalPoint in(2);
              in[0] = nodeI;
              in[1] = nodeJ;
              covarianceIJ += weightI * weightJ * xI * xJ * marginalDistribution->computePDF(in);
            } // loop over J integration nodes
          } // loop over I integration nodes
          covariance_(rowIndex, columnIndex) = covarianceIJ * halfLengthI * halfLengthJ;
        }
      } // loop over column indices
    } // loop over row indices
  } // if !hasIndependentCopula
  isAlreadyComputedCovariance_ = true;
} // computeCovarianceContinuous

void DistributionImplementation::computeCovarianceDiscrete() const
{
  // We need this to initialize the covariance matrix in two cases:
  // + this is the first call to this routine (which could be checked by testing the dimension of the distribution and the dimension of the matrix
  // + the copula has changed from a non-independent one to the independent copula
  covariance_ = CovarianceMatrix(dimension_);
  // First the diagonal terms, which are the marginal covariances
  // Marginal covariances
  const NumericalPoint variance(getCenteredMoment(2));
  for(UnsignedInteger component = 0; component < dimension_; ++component) covariance_(component, component) = variance[component];
  // Off-diagonal terms if the copula is not the independent copula
  if (!hasIndependentCopula())
  {
    // To ensure that the mean is up to date
    mean_ = getMean();
    // Performs the integration for each covariance in the strictly lower triangle of the covariance matrix
    // We first loop over the coefficients because the most expensive task is to get the 2D marginal distributions
    Indices indices(2);
    for(UnsignedInteger rowIndex = 0; rowIndex < dimension_; ++rowIndex)
    {
      indices[0] = rowIndex;
      for(UnsignedInteger columnIndex = rowIndex + 1; columnIndex < dimension_; ++columnIndex)
      {
        indices[1] = columnIndex;
        const Implementation marginalDistribution(getMarginal(indices));
        if (!marginalDistribution->hasIndependentCopula())
        {
          const NumericalSample support(marginalDistribution->getSupport());
          const NumericalSample samplePDF(marginalDistribution->computePDF(support));
          NumericalScalar value(0.0);
          const UnsignedInteger size(support.getSize());
          for (UnsignedInteger i = 0; i < size; ++i) value += (support[i][0] - mean_[rowIndex]) * (support[i][1] - mean_[columnIndex]) * samplePDF[i][0];
          covariance_(rowIndex, columnIndex) = value;
        }
      } // loop over column indices
    } // loop over row indices
  } // if !hasIndependentCopula
  isAlreadyComputedCovariance_ = true;
}



void DistributionImplementation::computeCovarianceGeneral() const
{
  // We need this to initialize the covariance matrix in two cases:
  // + this is the first call to this routine (which could be checked by testing the dimension of the distribution and the dimension of the matrix
  // + the copula has changed from a non-independent one to the independent copula
  covariance_ = CovarianceMatrix(dimension_);
  // First the diagonal terms, which are the marginal covariances
  // To ensure that the mean is up to date
  mean_ = getMean();
  // Get the standard deviation
  const NumericalPoint standardDeviation(getStandardDeviation());
  for(UnsignedInteger component = 0; component < dimension_; ++component) covariance_(component, component) = standardDeviation[component] * standardDeviation[component];
  // Off-diagonal terms if the copula is not the independent copula
  if (!hasIndependentCopula())
  {
    const NumericalScalar delta(2.0);
    Indices indices(2);
    const int N(8 * 2 * 2 * 2 * 2 * 2);
    const NumericalScalar h(0.5 / 2 / 2 / 2 / 2 / 2);
    for(UnsignedInteger rowIndex = 0; rowIndex < dimension_; ++rowIndex)
    {
      indices[0] = rowIndex;
      const Implementation marginalI(getMarginal(rowIndex));
      const NumericalScalar mi(marginalI->computeQuantile(0.5)[0]);
      const NumericalScalar di(marginalI->computeQuantile(0.75)[0] - marginalI->computeQuantile(0.25)[0]);
      for(UnsignedInteger columnIndex = rowIndex + 1; columnIndex < dimension_; ++columnIndex)
      {
        indices[1] = columnIndex;
        const Implementation marginalDistribution(getMarginal(indices));
        if (!marginalDistribution->hasIndependentCopula())
        {
          const Implementation marginalJ(getMarginal(columnIndex));
          const NumericalScalar mj(marginalJ->computeQuantile(0.5)[0]);
          const NumericalScalar dj(marginalJ->computeQuantile(0.75)[0] - marginalJ->computeQuantile(0.25)[0]);
          NumericalPoint xij(2);
          xij[0] = mi;
          xij[1] = mj;
          NumericalScalar covarianceIJ(0.0);
          // Then we loop over the integration points
          for(int rowNodeIndex = -N; rowNodeIndex < N + 1; ++rowNodeIndex)
          {
            const NumericalScalar hi(h * rowNodeIndex);
            const NumericalScalar expHi(exp(hi));
            const NumericalScalar iexpHi(1.0 / expHi);
            const NumericalScalar sinhHi(0.5 * (expHi - iexpHi));
            const NumericalScalar expSinhHi(exp(sinhHi));
            const NumericalScalar iexpSinhHi(1.0 / expSinhHi);
            const NumericalScalar iTwoCoshSinhHi(1.0 / (expSinhHi + iexpSinhHi));
            const NumericalScalar xip(mi + expSinhHi * iTwoCoshSinhHi * di * delta);
            const NumericalScalar wi((expHi + iexpHi) * iTwoCoshSinhHi * iTwoCoshSinhHi);
            const NumericalScalar cdfip(marginalI->computeCDF(xip));
            for(int columnNodeIndex = -N; columnNodeIndex < N + 1; ++columnNodeIndex)
            {
              const NumericalScalar hj(h * columnNodeIndex);
              const NumericalScalar expHj(exp(hj));
              const NumericalScalar iexpHj(1.0 / expHj);
              const NumericalScalar sinhHj(0.5 * (expHj - iexpHj));
              const NumericalScalar expSinhHj(exp(sinhHj));
              const NumericalScalar iexpSinhHj(1.0 / expSinhHj);
              const NumericalScalar iTwoCoshSinhHj(1.0 / (expSinhHj + iexpSinhHj));
              const NumericalScalar xjp(mj + expSinhHj * iTwoCoshSinhHj * dj * delta);
              const NumericalScalar wj((expHj + iexpHj) * iTwoCoshSinhHj * iTwoCoshSinhHj);
              const NumericalScalar cdfjp(marginalJ->computeCDF(xjp));
              NumericalPoint inpp(2);
              inpp[0] = xip;
              inpp[1] = xjp;
              covarianceIJ += delta * delta * di * dj * h * h * wi * wj * (marginalDistribution->computeCDF(inpp) - cdfip * cdfjp);
            } // loop over J integration nodes
          } // loop over I integration nodes
          covariance_(rowIndex, columnIndex) = covarianceIJ;
        }
      } // loop over column indices
    } // loop over row indices
  } // if !hasIndependentCopula
  isAlreadyComputedCovariance_ = true;
} // computeCovarianceGeneral

/* Get the covariance of the distribution */
CovarianceMatrix DistributionImplementation::getCovariance() const
{
  if (!isAlreadyComputedCovariance_) computeCovariance();
  return covariance_;
}

/* Correlation matrix accessor */
CorrelationMatrix DistributionImplementation::getCorrelation() const
{
  // To make sure the covariance is up to date
  covariance_ = getCovariance();
  CorrelationMatrix R(dimension_);
  NumericalPoint sigma(dimension_);
  for (UnsignedInteger i = 0; i < dimension_; ++i)
  {
    const NumericalScalar sigmaI(sqrt(covariance_(i, i)));
    sigma[i] = sigmaI;
    if (sigmaI > 0.0)
      for (UnsignedInteger j = 0; j < i; ++j)
        if (sigma[j] > 0)
          R(i, j) = covariance_(i, j) / (sigmaI * sigma[j]);
  }
  return R;
}

CorrelationMatrix DistributionImplementation::getLinearCorrelation() const
{
  return getCorrelation();
}

CorrelationMatrix DistributionImplementation::getPearsonCorrelation() const
{
  return getCorrelation();
}

/* Get the Spearman correlation of the distribution */
CorrelationMatrix DistributionImplementation::getSpearmanCorrelation() const
{
  return getCopula()->getSpearmanCorrelation();
}

/* Get the Kendall concordance of the distribution */
CorrelationMatrix DistributionImplementation::getKendallTau() const
{
  return getCopula()->getKendallTau();
}

/* Get the shape matrix of the distribution, ie the correlation matrix
   of its copula if it is elliptical */
CorrelationMatrix DistributionImplementation::getShapeMatrix() const
{
  if (!hasEllipticalCopula()) throw NotDefinedException(HERE) << "Error: the shape matrix is defined only for distributions with elliptical copulas.";
  return getCopula()->getShapeMatrix();
}

/* Cholesky factor of the correlation matrix accessor */
SquareMatrix DistributionImplementation::getCholesky() const
{
  covariance_ = getCovariance();
  return covariance_.computeCholesky();
}

/* Inverse of the Cholesky factor of the correlation matrix accessor */
SquareMatrix DistributionImplementation::getInverseCholesky() const
{
  // Compute its Cholesky factor
  SquareMatrix cholesky(getCholesky());

  SquareMatrix inverseCholesky = TriangularMatrix(cholesky.getImplementation()).solveLinearSystem(IdentityMatrix(dimension_)).getImplementation();

  return inverseCholesky;
}

/* Compute the nodes and weights for a 1D gauss quadrature over [-1, 1] with respect to the Lebesgue measure */
void DistributionImplementation::computeGaussNodesAndWeights() const
{
  int integrationNodesNumber(integrationNodesNumber_);
  gaussNodesAndWeights_ = NumericalSample(2, integrationNodesNumber);
  // First, build a symmetric tridiagonal matrix whose eigenvalues are the nodes of the
  // gauss integration rule
  char jobz('V');
  int ljobz(1);
  NumericalPoint d(integrationNodesNumber);
  NumericalPoint e(integrationNodesNumber);
  for (UnsignedInteger i = 1; i < static_cast<UnsignedInteger>(integrationNodesNumber); ++i) e[i - 1] = 0.5 / sqrt(1.0 - pow(2.0 * i, -2));
  int ldz(integrationNodesNumber);
  SquareMatrix z(integrationNodesNumber);
  NumericalPoint work(2 * integrationNodesNumber - 2);
  int info;
  DSTEV_F77(&jobz, &integrationNodesNumber, &d[0], &e[0], &z(0, 0), &ldz, &work[0], &info, &ljobz);
  if (info != 0) throw InternalException(HERE) << "Lapack DSTEV: error code=" << info;
  for (UnsignedInteger i = 0; i < static_cast<UnsignedInteger>(integrationNodesNumber); ++i)
  {
    // Nodes
    gaussNodesAndWeights_[0][i] = d[i];
    // Weights
    gaussNodesAndWeights_[1][i] = 2.0 * pow(z(0, i), 2);
  }
  isAlreadyComputedGaussNodesAndWeights_ = true;
}

/* integrationNodesNumber accessors */
UnsignedInteger DistributionImplementation::getIntegrationNodesNumber() const
{
  return integrationNodesNumber_;
}

void DistributionImplementation::setIntegrationNodesNumber(const UnsignedInteger integrationNodesNumber) const
{
  if (integrationNodesNumber != integrationNodesNumber_)
  {
    isAlreadyComputedMean_ = false;
    isAlreadyComputedCovariance_ = false;
    isAlreadyComputedGaussNodesAndWeights_ = false;
    integrationNodesNumber_ = integrationNodesNumber;
  }
}

/* Gauss nodes and weights accessor */
NumericalSample DistributionImplementation::getGaussNodesAndWeights() const
{
  if (!isAlreadyComputedGaussNodesAndWeights_) computeGaussNodesAndWeights();
  return gaussNodesAndWeights_;
}

/* Gauss nodes and weights accessor */
NumericalPoint DistributionImplementation::getGaussNodesAndWeights(NumericalPoint & weights) const
{
  if (!isAlreadyComputedGaussNodesAndWeights_) computeGaussNodesAndWeights();
  weights = gaussNodesAndWeights_[1];
  return gaussNodesAndWeights_[0];
}


/* Get the moments of the standardized distribution */
NumericalPoint DistributionImplementation::getStandardMoment(const UnsignedInteger n) const
{
  return getStandardRepresentative()->getMoment(n);
}


/* Compute the shifted moments of the distribution */
NumericalPoint DistributionImplementation::computeShiftedMoment(const UnsignedInteger n,
    const NumericalPoint & shift) const
{
  if (isContinuous()) return computeShiftedMomentContinuous(n, shift);
  if (isDiscrete()) return computeShiftedMomentDiscrete(n, shift);
  return computeShiftedMomentGeneral(n, shift);
}

NumericalPoint DistributionImplementation::computeShiftedMomentContinuous(const UnsignedInteger n,
    const NumericalPoint & shift) const
{
  if (shift.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the shift dimension must match the distribution dimension.";
  if (n == 0) return NumericalPoint(dimension_, 1.0);
  setIntegrationNodesNumber(std::max(ResourceMap::GetAsUnsignedInteger( "ContinuousDistribution-DefaultIntegrationNodesNumber" ), 20 * n + 1));
  const NumericalSample nodesAndWeights(getGaussNodesAndWeights());
  const UnsignedInteger numberOfNodes(nodesAndWeights.getDimension());
  NumericalPoint moment(dimension_, 0.0);
  // For each component
  for(UnsignedInteger component = 0; component < dimension_; ++component)
  {
    const Implementation marginalDistribution(getMarginal(component));
    const NumericalScalar a(marginalDistribution->getRange().getLowerBound()[0]);
    const NumericalScalar b(marginalDistribution->getRange().getUpperBound()[0]);
    const NumericalScalar halfLength(0.5 * (b - a));
    const NumericalScalar shiftComponent(shift[component]);
    NumericalScalar value(0.0);
    for (UnsignedInteger i = 0; i < numberOfNodes; ++i)
    {
      const NumericalScalar w(nodesAndWeights[1][i]);
      const NumericalScalar xi(nodesAndWeights[0][i]);
      const NumericalScalar z(a + (1.0 + xi) * halfLength);
      value += w * pow(z - shiftComponent, static_cast<int>(n)) * marginalDistribution->computePDF(z);
    } // Integration nodes
    moment[component] = value * halfLength;
  } // End of each component
  return moment;
}

NumericalPoint DistributionImplementation::computeShiftedMomentDiscrete(const UnsignedInteger n,
    const NumericalPoint & shift) const
{
  if (n == 0) throw InvalidArgumentException(HERE) << "Error: the centered moments of order 0 are undefined.";
  if (shift.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the shift dimension must match the distribution dimension.";
  NumericalPoint moment(dimension_);
  const NumericalSample support(getSupport());
  const NumericalSample pdfSupport(computePDF(support));
  for (UnsignedInteger i = 0; i < support.getSize(); ++i)
    for (UnsignedInteger j = 0; j < dimension_; ++j)
      moment[j] += pow(support[i][j] - shift[j], static_cast<int>(n)) * pdfSupport[i][0];
  return moment;
}

NumericalPoint DistributionImplementation::computeShiftedMomentGeneral(const UnsignedInteger n,
    const NumericalPoint & shift) const
{
  if (n == 0) throw InvalidArgumentException(HERE) << "Error: the centered moments of order 0 are undefined.";
  if (shift.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the shift dimension must match the distribution dimension.";
  NumericalPoint moment(dimension_);
  const NumericalScalar epsilon(sqrt(quantileEpsilon_));
  const UnsignedInteger MaximumLevel(ResourceMap::GetAsUnsignedInteger( "DistributionImplementation-DefaultLevelNumber" ) + 3);
  // For each component
  for(UnsignedInteger component = 0; component < dimension_; ++component)
  {
    NumericalScalar h(0.5);
    UnsignedInteger N(6);
    const Implementation marginalDistribution(getMarginal(component));
    const NumericalScalar shiftComponent(shift[component]);
    // Central term
    moment[component] = h * 0.5 * pow(marginalDistribution->computeQuantile(0.5)[0], static_cast<int>(n));
    // First block
    for (UnsignedInteger j = 1; j <= N; ++j)
    {
      const NumericalScalar hj(h * j);
      const NumericalScalar expHj(exp(hj));
      const NumericalScalar iexpHj(1.0 / expHj);
      const NumericalScalar sinhHj(0.5 * (expHj - iexpHj));
      const NumericalScalar expSinhHj(exp(sinhHj));
      const NumericalScalar iexpSinhHj(1.0 / expSinhHj);
      const NumericalScalar iTwoCoshSinhHj(1.0 / (expSinhHj + iexpSinhHj));
      const NumericalScalar xjm(iexpSinhHj * iTwoCoshSinhHj);
      const NumericalScalar xjp(expSinhHj * iTwoCoshSinhHj);
      const NumericalScalar wj((expHj + iexpHj) * iTwoCoshSinhHj * iTwoCoshSinhHj);
      moment[component] += h * wj * (pow(marginalDistribution->computeQuantile(xjm)[0] - shiftComponent, static_cast<int>(n)) + pow(marginalDistribution->computeQuantile(xjp)[0] - shiftComponent, static_cast<int>(n)));
    } // End of first block
    //values[0] = moment[component];
    // Sequential addition of half-blocks
    NumericalScalar error(1.0);
    UnsignedInteger level(0);
    while( (error > epsilon) && (level < MaximumLevel))
    {
      ++level;
      h *= 0.5;
      moment[component] *= 0.5;
      NumericalScalar delta(0.0);
      for (UnsignedInteger j = 0; j <= N; ++j)
      {
        const NumericalScalar hj(h * (2 * j + 1));
        const NumericalScalar expHj(exp(hj));
        const NumericalScalar iexpHj(1.0 / expHj);
        const NumericalScalar sinhHj(0.5 * (expHj - iexpHj));
        const NumericalScalar expSinhHj(exp(sinhHj));
        const NumericalScalar iexpSinhHj(1.0 / expSinhHj);
        const NumericalScalar iTwoCoshSinhHj(1.0 / (expSinhHj + iexpSinhHj));
        const NumericalScalar xjm(iexpSinhHj * iTwoCoshSinhHj);
        const NumericalScalar xjp(expSinhHj * iTwoCoshSinhHj);
        NumericalScalar wj((expHj + iexpHj) * iTwoCoshSinhHj * iTwoCoshSinhHj);
        delta += h * wj * (pow(marginalDistribution->computeQuantile(xjm)[0] - shiftComponent, static_cast<int>(n)) + pow(marginalDistribution->computeQuantile(xjp)[0] - shiftComponent, static_cast<int>(n)));
      }
      error = fabs((delta - moment[component]) / (1.0 + fabs(delta)));
      moment[component] += delta;
      N *= 2;
    } // End of half-block
  } // End of each component
  return moment;
}

/* Check if the distribution is elliptical */
Bool DistributionImplementation::isElliptical() const
{
  return false;
}

/* Check if the distribution is a copula */
Bool DistributionImplementation::isCopula() const
{
  return false;
}

/* Check if the distribution is continuous */
Bool DistributionImplementation::isContinuous() const
{
  return true;
}

/* Check if the distribution is discrete */
Bool DistributionImplementation::isDiscrete() const
{
  return false;
}

/* Tell if the distribution is integer valued */
Bool DistributionImplementation::isIntegral() const
{
  return false;
}

/* Tell if the distribution has elliptical copula */
Bool DistributionImplementation::hasEllipticalCopula() const
{
  return true;
}

/* Tell if the distribution has independent copula */
Bool DistributionImplementation::hasIndependentCopula() const
{
  return true;
}

/* Get the support of a distribution that intersect a given interval */
NumericalSample DistributionImplementation::getSupport(const Interval & interval) const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::getSupport()";
}

/* Get the support on the whole range */
NumericalSample DistributionImplementation::getSupport() const
{
  return getSupport(getRange());
}

/* Compute the density generator of the elliptical generator, i.e.
 *  the function phi such that the density of the distribution can
 *  be written as p(x) = phi(t(x-mu)R(x-mu))
 */
NumericalScalar DistributionImplementation::computeDensityGenerator(const NumericalScalar betaSquare) const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::computeDensityGenerator()";
}

/* Compute the derivative of the density generator */
NumericalScalar DistributionImplementation::computeDensityGeneratorDerivative(const NumericalScalar betaSquare) const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::computeDensityGeneratorDerivative()";
}

/* Compute the seconde derivative of the density generator */
NumericalScalar DistributionImplementation::computeDensityGeneratorSecondDerivative(const NumericalScalar betaSquare) const
{
  throw NotYetImplementedException(HERE) << "in DistributionImplementation::computeDensityGeneratorSecondDerivative()";
}

/* Get the i-th marginal distribution */
DistributionImplementation::Implementation DistributionImplementation::getMarginal(const UnsignedInteger i) const
{
  if ((i != 0) || (dimension_ != 1)) throw NotYetImplementedException(HERE) << "in DistributionImplementation::getMarginal()";
  return clone();
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
DistributionImplementation::Implementation DistributionImplementation::getMarginal(const Indices & indices) const
{
  if ((indices.getSize() != 1) || (indices[0] != 0) || (dimension_ != 1)) throw NotYetImplementedException(HERE);
  return clone();
}

/* Get the copula of a distribution */
DistributionImplementation::Implementation DistributionImplementation::getCopula() const
{
  if (dimension_ == 1) return new IndependentCopula(1);
  return new SklarCopula(*this);
}

/* Get the isoprobabilist transformation */
DistributionImplementation::IsoProbabilisticTransformation DistributionImplementation::getIsoProbabilisticTransformation() const
{
  // Special case for dimension 1
  if (dimension_ == 1)
  {
    DistributionCollection collection(1);
    collection[0] = *this;
    // Get the marginal transformation evaluation implementation
    MarginalTransformationEvaluation evaluation(collection, DistributionCollection(1, Normal()));
    // We have to correct the direction because the output collection corresponds to the standard space, so there is no parameter to take into account.
    evaluation.setDirection(MarginalTransformationEvaluation::FROM);
    const EvaluationImplementation p_evaluation(evaluation.clone());
    // Get the marginal transformation gradient implementation
    const GradientImplementation p_gradient = new MarginalTransformationGradient(evaluation);
    // Get the marginal transformation hessian implementation
    const HessianImplementation p_hessian = new MarginalTransformationHessian(evaluation);
    InverseIsoProbabilisticTransformation inverseTransformation(p_evaluation, p_gradient, p_hessian);
    NumericalPointWithDescription parameters(getParametersCollection()[0]);
    const UnsignedInteger parametersDimension(parameters.getDimension());
    Description parametersDescription(parameters.getDescription());
    const String name(parameters.getName());
    for (UnsignedInteger i = 0; i < parametersDimension; i++) parametersDescription[i] = OSS() << name << "_" << parametersDescription[i];
    parameters.setDescription(parametersDescription);
    inverseTransformation.setParameters(parameters);
    return inverseTransformation;
  }
  // General case, Rosenblatt transformation
  return NumericalMathFunctionImplementation(new RosenblattEvaluation(clone()));
}

/* Get the inverse isoprobabilist transformation */
DistributionImplementation::InverseIsoProbabilisticTransformation DistributionImplementation::getInverseIsoProbabilisticTransformation() const
{
  // Special case for dimension 1
  if (dimension_ == 1)
  {
    DistributionCollection collection(1);
    collection[0] = *this;
    // Get the marginal transformation evaluation implementation
    MarginalTransformationEvaluation evaluation(DistributionCollection(1, Normal()), collection);
    // We have to correct the direction because the input collection corresponds to the standard space, so there is no parameter to take into account.
    evaluation.setDirection(MarginalTransformationEvaluation::TO);
    const EvaluationImplementation p_evaluation(evaluation.clone());
    // Get the marginal transformation gradient implementation
    const GradientImplementation p_gradient = new MarginalTransformationGradient(evaluation);
    // Get the marginal transformation hessian implementation
    const HessianImplementation p_hessian = new MarginalTransformationHessian(evaluation);
    InverseIsoProbabilisticTransformation inverseTransformation(p_evaluation, p_gradient, p_hessian);
    NumericalPointWithDescription parameters(getParametersCollection()[0]);
    const UnsignedInteger parametersDimension(parameters.getDimension());
    Description parametersDescription(parameters.getDescription());
    const String name(parameters.getName());
    for (UnsignedInteger i = 0; i < parametersDimension; i++) parametersDescription[i] = OSS() << name << "_" << parametersDescription[i];
    parameters.setDescription(parametersDescription);
    inverseTransformation.setParameters(parameters);
    return inverseTransformation;
  }
  // General case, inverse Rosenblatt transformation
  return NumericalMathFunctionImplementation(new InverseRosenblattEvaluation(clone()));
}

/* Get the standard distribution */
void DistributionImplementation::computeStandardDistribution() const
{
  Normal standardDistribution(dimension_);
  standardDistribution.setDescription(getDescription());
  p_standardDistribution_ = standardDistribution.clone();
  isAlreadyComputedStandardDistribution_ = true;
}

/* Get the standard distribution */
DistributionImplementation::Implementation DistributionImplementation::getStandardDistribution() const
{
  if (!isAlreadyComputedStandardDistribution_) computeStandardDistribution();
  return p_standardDistribution_;
}

/* Get the standard representative in the parametric family, associated with the standard moments */
DistributionImplementation::Implementation DistributionImplementation::getStandardRepresentative() const
{
  return clone();
}

/* Compute the radial distribution CDF */
NumericalScalar DistributionImplementation::computeRadialDistributionCDF(const NumericalScalar radius,
    const Bool tail) const
{
  throw NotYetImplementedException(HERE);
}


/* Draw the PDF of a discrete distribution */
Graph DistributionImplementation::drawDiscretePDF(const NumericalScalar xMin,
    const NumericalScalar xMax,
    const UnsignedInteger pointNumber) const
{
  if (getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot draw the PDF of a multidimensional discrete distribution this way.";
  if (xMax < xMin - ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon")) throw InvalidArgumentException(HERE) << "Error: cannot draw a PDF with xMax < xMin, here xmin=" << xMin << " and xmax=" << xMax;
  const String title(OSS() << getDescription()[0] << " PDF");
  const NumericalSample support(getSupport(Interval(xMin, xMax)));
  // First the vertical bars
  const String xName(getDescription()[0]);
  Graph graphPDF(title, xName, "PDF", true, "topright");
  NumericalPoint point(2);
  point[0] = xMin - ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon");
  NumericalSample data(0, 2);
  data.add(point);
  for (UnsignedInteger i = 0; i < support.getSize(); ++i)
  {
    point[0] = support[i][0];
    data.add(point);
    point[1] = computePDF(point[0]);
    data.add(point);
    point[1] = 0.0;
    data.add(point);
  }
  point[0] = xMax + ResourceMap::GetAsNumericalScalar("DiscreteDistribution-SupportEpsilon");
  point[1] = 0.0;
  data.add(point);
  graphPDF.add(Curve(data, "red", "solid", 2, title));
  NumericalPoint bb(graphPDF.getBoundingBox());
  bb[3] += 0.1 * (bb[3] - bb[2]);
  graphPDF.setBoundingBox(bb);
  return graphPDF;
}

/* Draw the PDF of the distribution when its dimension is 1 */
Graph DistributionImplementation::drawPDF(const NumericalScalar xMin,
    const NumericalScalar xMax,
    const UnsignedInteger pointNumber) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "Error: can draw a PDF only if dimension equals 1, here dimension=" << dimension_;
  if (xMax <= xMin) throw InvalidArgumentException(HERE) << "Error: cannot draw a PDF with xMax >= xMin, here xmin=" << xMin << " and xmax=" << xMax;
  if (pointNumber < 2) throw InvalidArgumentException(HERE) << "Error: cannot draw a PDF with a point number < 2";
  if (isDiscrete()) return drawDiscretePDF(xMin, xMax, pointNumber);
  // Discretization of the x axis
  const String title(OSS() << getDescription()[0] << " PDF");
  NumericalSample gridX;
  const NumericalSample gridY(computePDF(xMin, xMax, pointNumber, gridX));
  Curve curvePDF(gridX, gridY);
  curvePDF.setColor("red");
  curvePDF.setLineStyle("solid");
  curvePDF.setLineWidth(2);
  const String xName(getDescription()[0]);
  Graph graphPDF(title, xName, "PDF", true, "topright");
  graphPDF.add(curvePDF);
  NumericalPoint boundingBox(graphPDF.getBoundingBox());
  boundingBox[3] *= 1.1;
  graphPDF.setBoundingBox(boundingBox);
  return graphPDF;
}

/* Draw the PDF of the distribution when its dimension is 1 */
Graph DistributionImplementation::drawPDF(const UnsignedInteger pointNumber) const
{
  if (getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: this method is available only for 1D distributions";
  // For discrete distributions, use the numerical range to define the drawing range
  if (isDiscrete()) return drawPDF(range_.getLowerBound()[0] - 1.0, range_.getUpperBound()[0] + 1.0, pointNumber);
  const NumericalScalar xMin(computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))[0]);
  const NumericalScalar xMax(computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ))[0]);
  const NumericalScalar delta(2.0 * (xMax - xMin) * (1.0 - 0.5 * (ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ) - ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))));
  return drawPDF(xMin - delta, xMax + delta, pointNumber);
}

/* Draw the PDF of a 1D marginal */
Graph DistributionImplementation::drawMarginal1DPDF(const UnsignedInteger marginalIndex,
    const NumericalScalar xMin,
    const NumericalScalar xMax,
    const UnsignedInteger pointNumber) const
{
  Graph marginalGraph(getMarginal(marginalIndex)->drawPDF(xMin, xMax, pointNumber));
  marginalGraph.setTitle(OSS() << getDescription() << "->" << description_[marginalIndex] << " component PDF");
  return marginalGraph;
}

/* Draw the PDF of the distribution when its dimension is 2 */
Graph DistributionImplementation::drawPDF(const NumericalPoint & xMin,
    const NumericalPoint & xMax,
    const Indices & pointNumber) const
{
  if ((pointNumber[0] <= 2) || (pointNumber[1] <= 2)) throw InvalidArgumentException(HERE) << "Error: the discretization must have at least 2 points per component";
  NumericalPoint discretization(2);
  NumericalPoint scaling(2);
  NumericalPoint origin(2);
  const NumericalScalar nX(pointNumber[0] - 2);
  discretization[0] = nX;
  // Discretization of the first component
  NumericalSample x(Box(NumericalPoint(1, nX)).generate());
  origin[0] = xMin[0];
  scaling[0] = xMax[0] - xMin[0];
  x.scale(NumericalPoint(1, scaling[0]));
  x.translate(NumericalPoint(1, origin[0]));
  const NumericalScalar nY(pointNumber[1] - 2);
  discretization[1] = nY;
  // Discretization of the second component
  NumericalSample y(Box(NumericalPoint(1, nY)).generate());
  origin[1] = xMin[1];
  scaling[1] = xMax[1] - xMin[1];
  y.scale(NumericalPoint(1, scaling[1]));
  y.translate(NumericalPoint(1, origin[1]));
  // Discretization of the XY plane
  NumericalSample xy(Box(discretization).generate());
  xy.scale(scaling);
  xy.translate(origin);
  // Compute the output sample, using possible parallelism
  const NumericalSample z(computePDF(xy));
  const String xName(description_[0]);
  const String yName(description_[1]);
  const String title(OSS() << getDescription() << " iso-PDF");
  Graph graph(title, xName, yName, true, "topright");
  Contour isoValues(Contour(x, y, z, NumericalPoint(0), Description(0), true, title));
  isoValues.buildDefaultLevels();
  isoValues.buildDefaultLabels();
  graph.add(isoValues);
  return graph;
}

/* Draw the PDF of the distribution when its dimension is 2 */
Graph DistributionImplementation::drawPDF(const NumericalPoint & xMin,
    const NumericalPoint & xMax) const
{
  return drawPDF(xMin, xMax, Indices(2, ResourceMap::GetAsUnsignedInteger( "DistributionImplementation-DefaultPointNumber" )));
}

/* Draw the PDF of the distribution when its dimension is 2 */
Graph DistributionImplementation::drawPDF(const Indices & pointNumber) const
{
  NumericalPoint xMin(2);
  xMin[0] = getMarginal(0)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))[0];
  xMin[1] = getMarginal(1)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))[0];
  NumericalPoint xMax(2);
  xMax[0] = getMarginal(0)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ))[0];
  xMax[1] = getMarginal(1)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ))[0];
  const NumericalPoint delta(2.0 * (xMax - xMin) * (1.0 - 0.5 * (ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ) - ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))));
  return drawPDF(xMin - delta, xMax + delta, pointNumber);
}

/* Draw the PDF of a 2D marginal */
Graph DistributionImplementation::drawMarginal2DPDF(const UnsignedInteger firstMarginal,
    const UnsignedInteger secondMarginal,
    const NumericalPoint & xMin,
    const NumericalPoint & xMax,
    const Indices & pointNumber) const
{
  Indices indices(2);
  indices[0] = firstMarginal;
  indices[1] = secondMarginal;
  Graph marginalGraph(getMarginal(indices)->drawPDF(xMin, xMax, pointNumber));
  marginalGraph.setTitle(OSS() << getDescription() << "->[" << description_[firstMarginal] << ", " << description_[secondMarginal] << "] components iso-PDF");
  return marginalGraph;
}

/* Draw the PDF of the distribution when its dimension is 1 or 2 */
Graph DistributionImplementation::drawPDF() const
{
  UnsignedInteger dimension(getDimension());
  // Generic interface for the 1D and 2D cases
  if (dimension == 1) return drawPDF(ResourceMap::GetAsUnsignedInteger( "DistributionImplementation-DefaultPointNumber" ));
  if (dimension == 2) return drawPDF(Indices(2, ResourceMap::GetAsUnsignedInteger( "DistributionImplementation-DefaultPointNumber" )));
  throw InvalidDimensionException(HERE) << "Error: can draw a PDF only if dimension equals 1 or 2, here dimension=" << dimension;
}


/* Draw the CDF of a discrete distribution */
Graph DistributionImplementation::drawDiscreteCDF(const NumericalScalar xMin,
    const NumericalScalar xMax,
    const UnsignedInteger pointNumber) const
{
  // Value :    0    1/5  2/5  3/5    4/5    1
  // Data  : ------+-----+---+------+----+---------
  // Case 1: ------------------------------[----]--
  // Case 2: ------------------[---]---------------
  //         -[--]---------------------------------
  // Case 3: ----------[---]-----------------------
  //         ---[-----------------------------]----
  //         -------[-----------------]------------
  if (getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot draw the CDF of a multidimensional discrete distribution this way.";
  if (xMax <= xMin) throw InvalidArgumentException(HERE) << "Error: cannot draw a PDF with xMax >= xMin, here xmin=" << xMin << " and xmax=" << xMax;
  // Create the graph that will store the staircase representing the empirical CDF
  const String title(OSS() << getDescription()[0] << " CDF");
  const NumericalSample support(getSupport(Interval(xMin, xMax)));
  const UnsignedInteger size(support.getSize());
  const String xName(getDescription()[0]);
  Graph graphCDF(title, xName, "CDF", true, "topleft");
  NumericalSample data(size + 2, 2);
  data[0][0] = xMin;
  data[0][1] = computeCDF(xMin);
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    const NumericalScalar x(support[i][0]);
    data[i + 1][0] = x;
    data[i + 1][1] = computeCDF(x);
  }
  data[size + 1][0] = xMax;
  data[size + 1][1] = computeCDF(xMax);
  graphCDF.add(Staircase(data, "red", "solid", 2, "s", title));
  NumericalPoint bb(graphCDF.getBoundingBox());
  bb[3] += 0.1 * (bb[3] - bb[2]);
  graphCDF.setBoundingBox(bb);
  return graphCDF;
}

/* Draw the CDF of the distribution when its dimension is 1 */
Graph DistributionImplementation::drawCDF(const NumericalScalar xMin,
    const NumericalScalar xMax,
    const UnsignedInteger pointNumber) const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "Error: can draw a CDF only if dimension equals 1, here dimension=" << dimension_;
  if (xMax <= xMin) throw InvalidArgumentException(HERE) << "Error: cannot draw a CDF with xMax >= xMin, here xmin=" << xMin << " and xmax=" << xMax;
  if (pointNumber < 2) throw InvalidArgumentException(HERE) << "Error: cannot draw a CDF with a point number < 2";
  if (isDiscrete()) return drawDiscreteCDF(xMin, xMax, pointNumber);
  const String title(OSS() << getDescription()[0] << " CDF");
  NumericalSample dataX;
  const NumericalSample dataY(computeCDF(xMin, xMax, pointNumber, dataX));
  Curve curveCDF(dataX, dataY);
  curveCDF.setColor("red");
  curveCDF.setLineStyle("solid");
  curveCDF.setLineWidth(2);
  const String xName(getDescription()[0]);
  Graph graphCDF(title, xName, "CDF", true, "topleft");
  graphCDF.add(curveCDF);
  NumericalPoint bb(graphCDF.getBoundingBox());
  bb[3] += 0.1 * (bb[3] - bb[2]);
  graphCDF.setBoundingBox(bb);
  return graphCDF;
}

/* Draw the CDF of the distribution when its dimension is 1 */
Graph DistributionImplementation::drawCDF(const UnsignedInteger pointNumber) const
{
  if (getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: this method is available only for 1D distributions";
  // For discrete distributions, use the numerical range to define the drawing range
  if (isDiscrete()) return drawCDF(range_.getLowerBound()[0] - 1.0, range_.getUpperBound()[0] + 1.0, pointNumber);
  const NumericalScalar xMin(computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))[0]);
  const NumericalScalar xMax(computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ))[0]);
  const NumericalScalar delta(2.0 * (xMax - xMin) * (1.0 - 0.5 * (ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ) - ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))));
  return drawCDF(xMin - delta, xMax + delta, pointNumber);
}

/* Draw the CDF of a 1D marginal */
Graph DistributionImplementation::drawMarginal1DCDF(const UnsignedInteger marginalIndex,
    const NumericalScalar xMin,
    const NumericalScalar xMax,
    const UnsignedInteger pointNumber) const
{
  Graph marginalGraph(getMarginal(marginalIndex)->drawCDF(xMin, xMax, pointNumber));
  marginalGraph.setTitle(OSS() << getDescription() << "->" << description_[marginalIndex] << " component CDF");
  return marginalGraph;
}

/* Draw the CDF of the distribution when its dimension is 2 */
Graph DistributionImplementation::drawCDF(const NumericalPoint & xMin,
    const NumericalPoint & xMax,
    const Indices & pointNumber) const
{
  if ((pointNumber[0] <= 2) || (pointNumber[1] <= 2)) throw InvalidArgumentException(HERE) << "Error: the discretization must have at least 2 points per component";
  NumericalPoint discretization(2);
  NumericalPoint scaling(2);
  NumericalPoint origin(2);
  const NumericalScalar nX(pointNumber[0] - 2);
  discretization[0] = nX;
  // Discretization of the first component
  NumericalSample x(Box(NumericalPoint(1, nX)).generate());
  origin[0] = xMin[0];
  scaling[0] = xMax[0] - xMin[0];
  x.scale(NumericalPoint(1, scaling[0]));
  x.translate(NumericalPoint(1, origin[0]));
  const NumericalScalar nY(pointNumber[1] - 2);
  discretization[1] = nY;
  // Discretization of the second component
  NumericalSample y(Box(NumericalPoint(1, nY)).generate());
  origin[1] = xMin[1];
  scaling[1] = xMax[1] - xMin[1];
  y.scale(NumericalPoint(1, scaling[1]));
  y.translate(NumericalPoint(1, origin[1]));
  // Discretization of the XY plane
  NumericalSample xy(Box(discretization).generate());
  xy.scale(scaling);
  xy.translate(origin);
  // Compute the output sample, using possible parallelism
  const NumericalSample z(computeCDF(xy));
  const String xName(description_[0]);
  const String yName(description_[1]);
  const String title(OSS() << getDescription() << " iso-CDF");
  Graph graph(title, xName, yName, true, "topright");
  Contour isoValues(Contour(x, y, z, NumericalPoint(0), Description(0), true, title));
  isoValues.buildDefaultLevels();
  isoValues.buildDefaultLabels();
  graph.add(isoValues);
  return graph;
}

/* Draw the CDF of the distribution when its dimension is 2 */
Graph DistributionImplementation::drawCDF(const NumericalPoint & xMin,
    const NumericalPoint & xMax) const
{
  return drawCDF(xMin, xMax, Indices(2, ResourceMap::GetAsUnsignedInteger( "DistributionImplementation-DefaultPointNumber" )));
}

/* Draw the CDF of the distribution when its dimension is 2 */
Graph DistributionImplementation::drawCDF(const Indices & pointNumber) const
{
  NumericalPoint xMin(2);
  xMin[0] = getMarginal(0)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))[0];
  xMin[1] = getMarginal(1)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))[0];
  NumericalPoint xMax(2);
  xMax[0] = getMarginal(0)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ))[0];
  xMax[1] = getMarginal(1)->computeQuantile(ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ))[0];
  const NumericalPoint delta(2.0 * (xMax - xMin) * (1.0 - 0.5 * (ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMax" ) - ResourceMap::GetAsNumericalScalar( "DistributionImplementation-QMin" ))));
  return drawCDF(xMin - delta, xMax + delta, pointNumber);
}

/* Draw the CDF of a 2D marginal */
Graph DistributionImplementation::drawMarginal2DCDF(const UnsignedInteger firstMarginal,
    const UnsignedInteger secondMarginal,
    const NumericalPoint & xMin,
    const NumericalPoint & xMax,
    const Indices & pointNumber) const
{
  Indices indices(2);
  indices[0] = firstMarginal;
  indices[1] = secondMarginal;
  Graph marginalGraph(getMarginal(indices)->drawCDF(xMin, xMax, pointNumber));
  marginalGraph.setTitle(OSS() << getDescription() << "->[" << description_[firstMarginal] << ", " << description_[secondMarginal] << "] components iso-CDF");
  return marginalGraph;
}

/* Draw the CDF of the distribution when its dimension is 1 or 2 */
Graph DistributionImplementation::drawCDF() const
{
  const UnsignedInteger dimension(getDimension());
  // Generic interface for the 1D and 2D cases
  if (dimension == 1) return drawCDF(ResourceMap::GetAsUnsignedInteger( "DistributionImplementation-DefaultPointNumber" ));
  if (dimension == 2) return drawCDF(Indices(2, ResourceMap::GetAsUnsignedInteger( "DistributionImplementation-DefaultPointNumber" )));
  throw InvalidDimensionException(HERE) << "Error: can draw a CDF only if dimension equals 1 or 2, here dimension=" << dimension;
}

/* Draw the quantile of the distribution when its dimension is 1 or 2 */
Graph DistributionImplementation::drawQuantile(const UnsignedInteger pointNumber) const
{
  const NumericalScalar qMin(SpecFunc::NumericalScalarEpsilon);
  const NumericalScalar qMax(1.0 - qMin);
  return drawQuantile(qMin, qMax, pointNumber);
}

Graph DistributionImplementation::drawQuantile(const NumericalScalar qMin,
    const NumericalScalar qMax,
    const UnsignedInteger pointNumber) const
{
  // Generic interface for the 1D and 2D cases
  if (dimension_ == 1) return drawQuantile1D(qMin, qMax, pointNumber);
  if (dimension_ == 2) return drawQuantile2D(qMin, qMax, pointNumber);
  throw InvalidDimensionException(HERE) << "Error: can draw tha quantiles only if dimension equals 1 or 2, here dimension=" << dimension_;
}

/* Draw the quantile of the distribution when its dimension is 1 */
Graph DistributionImplementation::drawQuantile1D(const NumericalScalar qMin,
    const NumericalScalar qMax,
    const UnsignedInteger pointNumber) const
{
  const String title(OSS() << getDescription()[0] << " Quantile");
  NumericalSample dataX;
  const NumericalSample dataY(computeQuantile(qMin, qMax, pointNumber, dataX));
  Curve curveQuantile(dataX, dataY);
  curveQuantile.setColor("red");
  curveQuantile.setLineStyle("solid");
  curveQuantile.setLineWidth(2);
  const String xName(getDescription()[0]);
  Graph graphQuantile(title, "p", xName, true, "topleft");
  graphQuantile.add(curveQuantile);
  NumericalPoint bb(graphQuantile.getBoundingBox());
  const NumericalScalar delta(bb[3] - bb[2]);
  bb[2] -= 0.1 * delta;
  bb[3] += 0.1 * delta;
  graphQuantile.setBoundingBox(bb);
  return graphQuantile;
}

/* Draw the quantile of the distribution when its dimension is 2 */
Graph DistributionImplementation::drawQuantile2D(const NumericalScalar qMin,
    const NumericalScalar qMax,
    const UnsignedInteger pointNumber) const
{
  const String title(OSS() << getDescription() << " Quantile");
  const NumericalSample data(computeQuantile(qMin, qMax, pointNumber));
  Curve curveQuantile(data);
  curveQuantile.setColor("red");
  curveQuantile.setLineStyle("solid");
  curveQuantile.setLineWidth(2);
  const String xName(getDescription()[0]);
  const String yName(getDescription()[1]);
  Graph graphQuantile(title, xName, yName, true, "topleft");
  graphQuantile.add(drawCDF(data.getMin(), data.getMax()).getDrawable(0));
  graphQuantile.add(curveQuantile);
  NumericalPoint bb(graphQuantile.getBoundingBox());
  const NumericalScalar delta(bb[3] - bb[2]);
  bb[2] -= 0.1 * delta;
  bb[3] += 0.1 * delta;
  graphQuantile.setBoundingBox(bb);
  Description legends(2);
  legends[0] = "iso-CDF";
  legends[1] = "quantile";
  graphQuantile.setLegends(legends);
  return graphQuantile;
}

/* Parameters value and description accessor */
DistributionImplementation::NumericalPointWithDescriptionCollection DistributionImplementation::getParametersCollection() const
{
  // By default, assume an empty set of parameters for a 1D distribution or a copula
  return NumericalPointWithDescriptionCollection(1, NumericalPointWithDescription(0));
}

void DistributionImplementation::setParametersCollection(const NumericalPointWithDescriptionCollection & parametersCollection)
{
  // Get the actual collection of parameters to check the description and the size
  const NumericalPointWithDescriptionCollection actualParameters(getParametersCollection());
  const UnsignedInteger size(actualParameters.getSize());
  if (parametersCollection.getSize() != size) throw InvalidArgumentException(HERE) << "Error: the given parameters collection has an invalid size (" << parametersCollection.getSize() << "), it should be " << size;
  NumericalPointCollection coll(0);
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    const UnsignedInteger dimension(actualParameters[i].getDimension());
    if (parametersCollection[i].getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given parameters collection has an invalid dimension at index " << i;
    coll.add(parametersCollection[i]);
  }
  setParametersCollection(coll);
}

void DistributionImplementation::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  const UnsignedInteger size(parametersCollection.getSize());
  NumericalPointWithDescriptionCollection newParameters(size);
  for (UnsignedInteger i = 0; i < size; ++i) newParameters[i] = parametersCollection[i];
  setParametersCollection(newParameters);
}

void DistributionImplementation::setParametersCollection(const NumericalPoint & flattenCollection)
{
  const NumericalPointWithDescriptionCollection currentParameters(getParametersCollection());
  UnsignedInteger size(currentParameters.getSize());
  NumericalPointCollection parametersCollection;
  UnsignedInteger flattenSize = 0;
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    const UnsignedInteger subSize(currentParameters[i].getSize());
    NumericalPoint subCollection(subSize);
    // copy parameters into the collection if the flatten collection is large enough
    if ( (flattenSize + subSize) <= flattenCollection.getSize() )
    {
      for (UnsignedInteger j = 0; j < subSize; ++j) subCollection[j] = flattenCollection[flattenSize + j];
    }
    parametersCollection.add(subCollection);
    flattenSize += subSize;
  }
  if (flattenCollection.getSize() != flattenSize) throw InvalidArgumentException(HERE) << "Error: the given parameters collection has an invalid size (" << flattenCollection.getSize() << "), it should be " << flattenSize;
  setParametersCollection(parametersCollection);
}

/* Parameters number */
UnsignedInteger DistributionImplementation::getParametersNumber() const
{
  const NumericalPointWithDescriptionCollection parametersCollection(getParametersCollection());
  const UnsignedInteger size(parametersCollection.getSize());
  if (size == 1) return parametersCollection[0].getDimension();
  // The number of parameters is non-trivial to compute in the multidimensional case as a given parameter can appear in different marginal parameters description and even in the dependence description. Thus, we have to get the number of unique parameters based on their description.
  // First, aggregate all the descriptions
  DescriptionImplementation globalDescription(0);
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    const Description localDescription(parametersCollection[i].getDescription());
    for (UnsignedInteger j = 0; j < localDescription.getSize(); ++j) globalDescription.add(localDescription[j]);
  }
  std::sort(globalDescription.begin(), globalDescription.end());
  DescriptionImplementation::iterator iter(std::unique(globalDescription.begin(), globalDescription.end()));
  return static_cast<UnsignedInteger>(iter - globalDescription.begin());
}

/* Description accessor */
void DistributionImplementation::setDescription(const Description & description)
{
  const UnsignedInteger size(description.getSize());
  if (size != getDimension()) throw InvalidArgumentException(HERE) << "Error: the description must have the same size than the distribution dimension, here size=" << size << " and dimension=" << getDimension();
  // Check if the description is valid
  // First, copy the description
  Description test(description);
  // Second, sort the copy
  std::sort(test.begin(), test.end());
  // Third, move the duplicates at the end
  Description::const_iterator it = std::unique(test.begin(), test.end());
  // Fourth, check if there was any duplicate
  if (it != test.end())
  {
    LOGINFO(OSS() << "Warning! The description of the distribution " << getName() << " is " << description << " and cannot identify uniquely the marginal distribution. Appended unique identifier to fix it:");
    Description newDescription(description);
    for (UnsignedInteger i = 0; i < size; ++i) newDescription[i] = OSS() << "marginal_" << i + 1 << "_" << description[i];
    LOGINFO(OSS() << "the new description is " << newDescription);
    description_ = newDescription;
  }
  else description_ = description;
}

/* Description accessot */
Description DistributionImplementation::getDescription() const
{
  return description_;
}

/* Accessor to PDF computation precision */
NumericalScalar DistributionImplementation::getPDFEpsilon() const
{
  return pdfEpsilon_;
}

/* Accessor to CDF computation precision */
NumericalScalar DistributionImplementation::getCDFEpsilon() const
{
  return cdfEpsilon_;
}

/* Get a positon indicator for a 1D distribution */
NumericalScalar DistributionImplementation::getPositionIndicator() const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "Error: cannot get the position indicator of a distribution with dimension > 1";
  // Return the median of the distribution
  return computeQuantile(0.5)[0];
}

/* Get a dispersion indicator for a 1D distribution */
NumericalScalar DistributionImplementation::getDispersionIndicator() const
{
  if (dimension_ != 1) throw InvalidDimensionException(HERE) << "Error: cannot get the dispersion indicator of a distribution with dimension > 1";
  // Return the interquartile of the distribution
  return computeQuantile(0.75)[0] - computeQuantile(0.25)[0];
}



/* Method save() stores the object through the StorageManager */
void DistributionImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "mean_", mean_ );
  adv.saveAttribute( "covariance_", covariance_ );
  adv.saveAttribute( "gaussNodesAndWeights_", gaussNodesAndWeights_ );
  adv.saveAttribute( "integrationNodesNumber_", integrationNodesNumber_ );
  adv.saveAttribute( "isAlreadyComputedMean_", isAlreadyComputedMean_ );
  adv.saveAttribute( "isAlreadyComputedCovariance_", isAlreadyComputedCovariance_ );
  adv.saveAttribute( "isAlreadyComputedGaussNodesAndWeights_", isAlreadyComputedGaussNodesAndWeights_ );
  adv.saveAttribute( "dimension_", dimension_ );
  adv.saveAttribute( "weight_", weight_ );
  adv.saveAttribute( "range_", range_ );
  adv.saveAttribute( "description_", description_ );
}

/* Method load() reloads the object from the StorageManager */
void DistributionImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "mean_", mean_ );
  adv.loadAttribute( "covariance_", covariance_ );
  adv.loadAttribute( "gaussNodesAndWeights_", gaussNodesAndWeights_ );
  adv.loadAttribute( "integrationNodesNumber_", integrationNodesNumber_ );
  adv.loadAttribute( "isAlreadyComputedMean_", isAlreadyComputedMean_ );
  adv.loadAttribute( "isAlreadyComputedCovariance_", isAlreadyComputedCovariance_ );
  adv.loadAttribute( "isAlreadyComputedGaussNodesAndWeights_", isAlreadyComputedGaussNodesAndWeights_ );
  adv.loadAttribute( "dimension_", dimension_ );
  adv.loadAttribute( "weight_", weight_ );
  adv.loadAttribute( "range_", range_ );
  adv.loadAttribute( "description_", description_ );
}

END_NAMESPACE_OPENTURNS
