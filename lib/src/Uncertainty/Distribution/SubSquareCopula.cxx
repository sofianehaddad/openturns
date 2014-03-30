//                                               -*- C++ -*-
/**
 *  @file  SubSquareCopula.cxx
 *  @brief A class that implements an independent copula
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include <cmath>
#include "SubSquareCopula.hxx"
#include "Uniform.hxx"
#include "IdentityMatrix.hxx"
#include "NatafEllipticalCopulaEvaluation.hxx"
#include "NatafEllipticalCopulaGradient.hxx"
#include "NatafEllipticalCopulaHessian.hxx"
#include "InverseNatafEllipticalCopulaEvaluation.hxx"
#include "InverseNatafEllipticalCopulaGradient.hxx"
#include "InverseNatafEllipticalCopulaHessian.hxx"
#include "Indices.hxx"
#include "RandomGenerator.hxx"
#include "Exception.hxx"
#include "Description.hxx"
#include "DistFunc.hxx"
#include "PersistentObjectFactory.hxx"
#include "TBB.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(SubSquareCopula);

static Factory<SubSquareCopula> RegisteredFactory("SubSquareCopula");

/* Default constructor */
SubSquareCopula::SubSquareCopula(const UnsignedInteger dim)
  : CopulaImplementation("SubSquareCopula")
  , correlation_(dim)
  , subSquare_(dim)
{
  // The range is generic for all the copulas
  setDimension(dim);
  computeRange();
}

/* Default constructor */
SubSquareCopula::SubSquareCopula(const CorrelationMatrix & correlation)
  : CopulaImplementation("SubSquareCopula")
  , correlation_(correlation)
  , subSquare_(NumericalPoint(correlation.getNbRows(), 0.0), NumericalPoint(correlation.getNbRows(), 1.0), correlation)
{
  // The range is generic for all the copulas
  setDimension(correlation.getNbRows());
  computeRange();
}

/* Comparison operator */
Bool SubSquareCopula::operator ==(const SubSquareCopula & other) const
{
  if (this == &other) return true;
  return correlation_ == other.correlation_;
}

/* String converter */
String SubSquareCopula::__repr__() const
{
  OSS oss;
  oss << "class=" << SubSquareCopula::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " correlation=" << correlation_;
  return oss;
}

String SubSquareCopula::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(R = " << correlation_.__str__(offset) << ")";
  return oss;
}

/* Virtual constructor */
SubSquareCopula * SubSquareCopula::clone() const
{
  return new SubSquareCopula(*this);
}

/* Get one realization of the distribution */
NumericalPoint SubSquareCopula::getRealization() const
{
  UnsignedInteger dimension(getDimension());
  if (hasIndependentCopula()) return RandomGenerator::Generate(dimension);
  else
  {
    NumericalPoint realization(subSquare_.getRealization());
    for (UnsignedInteger i = 0; i < dimension; ++i) realization[i] = DistFunc::pSubSquare(realization[i]);
    return realization;
  }
}

/* Get a sample of the distribution */
NumericalSample SubSquareCopula::getSampleParallel(const UnsignedInteger size) const
{
  if (hasIndependentCopula())
  {
    const UnsignedInteger dimension(getDimension());
    NumericalSample result(size, dimension);
    const NumericalPoint rawData(RandomGenerator::Generate(dimension * size));
    result.getImplementation()->setData(rawData);
    result.setName(getName());
    result.setDescription(getDescription());
    return result;
  }
  else
  {
    const UnsignedInteger dimension(getDimension());
    const NumericalSample subSquareSample(subSquare_.getSample(size));
    NumericalSample result(size, dimension);
    const ComputeSamplePolicy policy( subSquareSample, result );
    TBB::ParallelFor( 0, size, policy );
    result.setName(getName());
    result.setDescription(getDescription());
    return result;
  } // Nonindependente copula
}

NumericalSample SubSquareCopula::getSample(const UnsignedInteger size) const
{
  if (isParallel_) return getSampleParallel(size);
  return DistributionImplementation::getSample(size);
}

/* Get the DDF of the distribution */
NumericalPoint SubSquareCopula::computeDDF(const NumericalPoint & point) const
{
  const UnsignedInteger dimension(getDimension());
  if (point.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << getDimension() << ", here dimension=" << point.getDimension();

  // Be careful to evaluate the copula only in the interior of its support
  for (UnsignedInteger i = 0; i < dimension; ++i)
  {
    // If outside of the support return 0.0
    if ((point[i] <= 0.0) || (point[i] >= 1.0)) return NumericalPoint(dimension, 0.0);
  }
  NumericalPoint x(dimension);
  NumericalPoint marginalPDF(dimension);
  NumericalScalar marginalPDFProduct(1.0);
  for (UnsignedInteger i = 0; i < dimension; ++i)
  {
    const NumericalScalar xi(DistFunc::qSubSquare(point[i]));
    x[i] = xi;
    // .398942280401432677939946059934 = 1 / sqrt(2.pi)
    const NumericalScalar pdfI(0.398942280401432677939946059934 * exp(-0.5 * xi * xi));
    marginalPDF[i] = pdfI;
    marginalPDFProduct *= pdfI;
  }
  const NumericalPoint ddfNorm(subSquare_.computeDDF(x));
  const NumericalScalar pdfNorm(subSquare_.computePDF(x));
  NumericalPoint ddf(dimension);
  for (UnsignedInteger i = 0; i < dimension; ++i) ddf[i] = (ddfNorm[i] + x[i] * pdfNorm) / (marginalPDFProduct * marginalPDF[i]);
  return ddf;
}

/* Get the PDF of the distribution */
NumericalScalar SubSquareCopula::computePDF(const NumericalPoint & point) const
{
  const UnsignedInteger dimension(getDimension());
  if (point.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << getDimension() << ", here dimension=" << point.getDimension();

  // Be careful to evaluate the copula only in the interior of its support
  for (UnsignedInteger i = 0; i < dimension; ++i)
  {
    // If outside of the support return 0.0
    if ((point[i] <= 0.0) || (point[i] >= 1.0)) return 0.0;
  }
  // Compute the subSquare point such that a subSquare distribution with this copula
  // and standard 1D subSquare marginals has the same CDF at this subSquare point
  // than the copula at the given point.
  // Compute the multiplicative factor between the copula PDF
  // and the PDF of the associated generic subSquare using the specific form of
  // the standard subSquare PDF
  NumericalPoint subSquarePoint(dimension);
  NumericalScalar value(0.0);
  for (UnsignedInteger i = 0; i < dimension; ++i)
  {
    const NumericalScalar yi(DistFunc::qSubSquare(point[i]));
    subSquarePoint[i] = yi;
    value += yi * yi;
  }
  // 0.398942280401432677939946059934 = 1 / sqrt(2.pi)
  value = pow(0.398942280401432677939946059934, dimension) * exp(-0.5 * value);
  return subSquare_.computePDF(subSquarePoint) / value;
}

/* Get the CDF of the distribution */
NumericalScalar SubSquareCopula::computeCDF(const NumericalPoint & point) const
{
  const UnsignedInteger dimension(getDimension());
  if (point.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << getDimension() << ", here dimension=" << point.getDimension();

  // Compute the subSquare point such that a subSquare distribution with this copula
  // and standard 1D subSquare marginals has the same CDF at this subSquare point
  // than the copula at the given point.
  // Be careful to evaluate the copula only in the interior of its support
  Indices indices;
  for (UnsignedInteger i = 0; i < dimension; ++i)
  {
    // If outside of the support, in the lower part, return 0.0
    if (point[i] <= 0.0) return 0.0;
    // If the current component is in the interior of the support, its index
    // is taken into account
    if (point[i] < 1.0) indices.add(i);
  }
  const UnsignedInteger activeDimension(indices.getSize());
  // Quick return if all the components are >= 1
  if (activeDimension == 0) return 1.0;
  NumericalPoint subSquarePoint(activeDimension);
  for (UnsignedInteger i = 0; i < activeDimension; ++i) subSquarePoint[i] = DistFunc::qSubSquare(point[indices[i]]);
  // In the usual case when the given point is in the interior of the support
  // use the associated subSquare distribution
  if (dimension == activeDimension) return subSquare_.computeCDF(subSquarePoint);
  // In the other case, we must use the appropriate marginal distribution
  else return subSquare_.getMarginal(indices)->computeCDF(subSquarePoint);
} // computeCDF

/* Get the survival function of the distribution */
NumericalScalar SubSquareCopula::computeSurvivalFunction(const NumericalPoint & point) const
{
  const UnsignedInteger dimension(getDimension());
  if (point.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << getDimension() << ", here dimension=" << point.getDimension();

  // Compute the subSquare point such that a subSquare distribution with this copula
  // and standard 1D subSquare marginals has the same CDF at this subSquare point
  // than the copula at the given point.
  // Be careful to evaluate the copula only in the interior of its support
  Indices indices;
  for (UnsignedInteger i = 0; i < dimension; ++i)
  {
    // If outside of the support, in the lower part, return 0.0
    if (point[i] <= 0.0) return 0.0;
    // If the current component is in the interior of the support, its index
    // is taken into account
    if (point[i] < 1.0) indices.add(i);
  }
  const UnsignedInteger activeDimension(indices.getSize());
  // Quick return if all the components are >= 1
  if (activeDimension == 0) return 1.0;
  NumericalPoint subSquarePoint(activeDimension);
  for (UnsignedInteger i = 0; i < activeDimension; ++i) subSquarePoint[i] = DistFunc::qSubSquare(point[indices[i]], true);
  // In the usual case when the given point is in the interior of the support
  // use the associated subSquare distribution
  if (dimension == activeDimension) return subSquare_.computeSurvivalFunction(subSquarePoint);
  // In the other case, we must use the appropriate marginal distribution
  else return Distribution(subSquare_.getMarginal(indices)).computeSurvivalFunction(subSquarePoint);
} // computeSurvivalFunction

/* Compute the probability content of an interval */
NumericalScalar SubSquareCopula::computeProbability(const Interval & interval) const
{
  const UnsignedInteger dimension(getDimension());
  if (interval.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given interval must have dimension=" << dimension << ", here dimension=" << interval.getDimension();

  // Reduce the given interval to the support of the distribution, which is the nD unit cube
  const Interval intersect(interval.intersect(Interval(dimension)));
  // If the intersection is empty
  if (intersect.isNumericallyEmpty()) return 0.0;
  const NumericalPoint lowerBoundIntersect(intersect.getLowerBound());
  const NumericalPoint upperBoundIntersect(intersect.getUpperBound());
  NumericalPoint lowerBound(dimension);
  NumericalPoint upperBound(dimension);
  Interval::BoolCollection finiteLowerBound(dimension);
  Interval::BoolCollection finiteUpperBound(dimension);
  for (UnsignedInteger i = 0; i < dimension; ++i)
  {
    if (lowerBoundIntersect[i] == 0.0)
    {
      finiteLowerBound[i] = false;
      lowerBound[i] = -1.0;
    }
    else
    {
      finiteLowerBound[i] = true;
      lowerBound[i] = DistFunc::qSubSquare(lowerBoundIntersect[i]);
    }
    if (upperBoundIntersect[i] == 1.0)
    {
      finiteUpperBound[i] = false;
      upperBound[i] = 1.0;
    }
    else
    {
      finiteUpperBound[i] = true;
      upperBound[i] = DistFunc::qSubSquare(upperBoundIntersect[i]);
    }
  }
  return subSquare_.computeProbability(Interval(lowerBound, upperBound, finiteLowerBound, finiteUpperBound));
}

/* Get the Spearman correlation of the distribution */
CorrelationMatrix SubSquareCopula::getSpearmanCorrelation() const
{
  const UnsignedInteger dimension(getDimension());
  CorrelationMatrix rho(dimension);
  for (UnsignedInteger i = 1; i < dimension; ++i)
    for (UnsignedInteger j = 0; j < i; ++j) rho(i, j) = asin(0.5 * correlation_(i, j)) * 6.0 / M_PI;
  return rho;
}

/* Get the Kendall concordance of the distribution */
CorrelationMatrix SubSquareCopula::getKendallTau() const
{
  const UnsignedInteger dimension(getDimension());
  CorrelationMatrix tau(dimension);
  for (UnsignedInteger i = 1; i < dimension; ++i)
    for (UnsignedInteger j = 0; j < i; ++j) tau(i, j) = asin(correlation_(i, j)) * 2.0 / M_PI;
  return tau;
}

/* Get the Shape matrix of the copula */
CorrelationMatrix SubSquareCopula::getShapeMatrix() const
{
  return correlation_;
}

/* Get the PDF gradient of the distribution */
NumericalPoint SubSquareCopula::computePDFGradient(const NumericalPoint & point) const
{
  if (point.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << getDimension() << ", here dimension=" << point.getDimension();

  throw NotYetImplementedException(HERE);
}

/* Get the CDF gradient of the distribution */
NumericalPoint SubSquareCopula::computeCDFGradient(const NumericalPoint & point) const
{
  if (point.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << getDimension() << ", here dimension=" << point.getDimension();

  throw NotYetImplementedException(HERE);
}

/* Compute the PDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1)
   For SubSquare distribution, the conditional distribution is also SubSquare, with mean and covariance
   such as:
   mean_cond = mean(x) + cov(x, y).cov(y, y)^(-1)(y - mean(y))
   cov_cond = cov(x, x) - cov(x, y).cov(y, y)^(-1)cov(x, y)
   This expression simplifies if we use the inverse of the Cholesky factor of the covariance matrix.
   See [Lebrun, Dutfoy, "Rosenblatt and Nataf transformation"]
*/
NumericalScalar SubSquareCopula::computeConditionalPDF(const NumericalScalar x,
    const NumericalPoint & y) const
{
  const UnsignedInteger conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional PDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return 1.0;
  // General case
  NumericalPoint u(conditioningDimension);
  for (UnsignedInteger i = 0; i < conditioningDimension; ++i) u[i] = DistFunc::qSubSquare(y[i]);
  return subSquare_.computeConditionalPDF(DistFunc::qSubSquare(x), u);
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar SubSquareCopula::computeConditionalCDF(const NumericalScalar x,
    const NumericalPoint & y) const
{
  const UnsignedInteger conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional CDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return x;
  // General case
  NumericalPoint u(conditioningDimension);
  for (UnsignedInteger i = 0; i < conditioningDimension; ++i) u[i] = DistFunc::qSubSquare(y[i]);
  return subSquare_.computeConditionalCDF(DistFunc::qSubSquare(x), u);
}

/* Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
NumericalScalar SubSquareCopula::computeConditionalQuantile(const NumericalScalar q,
    const NumericalPoint & y) const
{
  const UnsignedInteger conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile with a conditioning point of dimension greater or equal to the distribution dimension.";
  if ((q < 0.0) || (q > 1.0)) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile for a probability level outside of [0, 1]";
  if (q == 0.0) return 0.0;
  if (q == 1.0) return 1.0;
  // Special case when no contitioning or independent copula
  if ((conditioningDimension == 0) || hasIndependentCopula()) return q;
  // General case
  NumericalPoint u(conditioningDimension);
  for (UnsignedInteger i = 0; i < conditioningDimension; ++i) u[i] = DistFunc::qSubSquare(y[i]);
  return DistFunc::pSubSquare(subSquare_.computeConditionalQuantile(q, u));
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
SubSquareCopula::Implementation SubSquareCopula::getMarginal(const Indices & indices) const
{
  const UnsignedInteger dimension(getDimension());
  if (!indices.check(dimension - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal distribution must be in the range [0, dim-1] and  must be different";
  // Special case for dimension 1
  if (dimension == 1) return clone();
  // General case
  const UnsignedInteger outputDimension(indices.getSize());
  CorrelationMatrix R(outputDimension);
  // Extract the correlation matrix, the marginal standard deviations and means
  for (UnsignedInteger i = 0; i < outputDimension; ++i)
  {
    const UnsignedInteger index_i(indices[i]);
    for (UnsignedInteger j = 0; j <= i; ++j) R(i, j) = correlation_(index_i, indices[j]);
  }
  return new SubSquareCopula(R);
}

/* Get the isoprobabilist transformation */
SubSquareCopula::IsoProbabilisticTransformation SubSquareCopula::getIsoProbabilisticTransformation() const
{
  IsoProbabilisticTransformation transformation;
  transformation.setEvaluationImplementation(new NatafEllipticalCopulaEvaluation(getStandardDistribution(), subSquare_.getInverseCholesky()));
  transformation.setGradientImplementation(new NatafEllipticalCopulaGradient(getStandardDistribution(), subSquare_.getInverseCholesky()));
  transformation.setHessianImplementation(new NatafEllipticalCopulaHessian(getStandardDistribution(), subSquare_.getInverseCholesky()));

  return transformation;
}

/* Get the inverse isoprobabilist transformation */
SubSquareCopula::InverseIsoProbabilisticTransformation SubSquareCopula::getInverseIsoProbabilisticTransformation() const
{
  InverseIsoProbabilisticTransformation transformation;
  transformation.setEvaluationImplementation(new InverseNatafEllipticalCopulaEvaluation(getStandardDistribution(), subSquare_.getCholesky()));
  transformation.setGradientImplementation(new InverseNatafEllipticalCopulaGradient(getStandardDistribution(), subSquare_.getCholesky()));
  transformation.setHessianImplementation(new InverseNatafEllipticalCopulaHessian(getStandardDistribution(), subSquare_.getCholesky()));

  return transformation;
}

/* Tell if the distribution has independent copula */
Bool SubSquareCopula::hasIndependentCopula() const
{
  return subSquare_.hasIndependentCopula();
}

/* Parameters value and description accessor */
SubSquareCopula::NumericalPointWithDescriptionCollection SubSquareCopula::getParametersCollection() const
{
  const UnsignedInteger dimension(getDimension());
  NumericalPointWithDescriptionCollection parameters(0);
  if (dimension > 1)
  {
    // Put the dependence parameters
    const UnsignedInteger parametersDimension(dimension * (dimension - 1) / 2);
    NumericalPointWithDescription point(parametersDimension);
    Description description(parametersDimension);
    point.setName(getName());
    UnsignedInteger dependenceIndex(0);
    for (UnsignedInteger i = 0; i < dimension; ++i)
    {
      for (UnsignedInteger j = 0; j < i; ++j)
      {
        point[dependenceIndex] = correlation_(i, j);
        OSS oss;
        oss << "R_" << i + 1 << "_" << j + 1;
        description[dependenceIndex] = oss;
        ++dependenceIndex;
      }
    }
    point.setDescription(description);
    parameters.add(point);
  } // dimension > 1
  return parameters;
} // getParametersCollection

void SubSquareCopula::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  // Check if the given parameters are ok
  if (parametersCollection.getSize() != 1) throw InvalidArgumentException(HERE) << "Error: the given collection has a size=" << parametersCollection.getSize() << " but should be of size=1";
  const NumericalPoint parameters(parametersCollection[0]);
  const UnsignedInteger dimension(getDimension());
  if (parameters.getDimension() != dimension * (dimension - 1) / 2) throw InvalidArgumentException(HERE) << "Error: got " << parameters.getDimension() << " parameters instead of " << dimension * (dimension - 1) / 2;
  if (dimension == 1) return;
  UnsignedInteger dependenceIndex(0);
  for (UnsignedInteger i = 0; i < dimension; ++i)
  {
    for (UnsignedInteger j = 0; j < i; ++j)
    {
      correlation_(i, j) = parameters[dependenceIndex];
      ++dependenceIndex;
    }
  }
}

/* Compute the correlation matrix of a SubSquare Copula from its Spearman correlation matrix */
CorrelationMatrix SubSquareCopula::GetCorrelationFromSpearmanCorrelation(const CorrelationMatrix & matrix)
{
  const UnsignedInteger dimension(matrix.getNbRows());
  CorrelationMatrix result(dimension);
  for (UnsignedInteger i = 1; i < dimension; ++i)
    for (UnsignedInteger j = 0; j < i; ++j) result(i, j) = 2.0 * sin(M_PI * matrix(i, j) / 6.0);
  if (!result.isPositiveDefinite()) throw NotSymmetricDefinitePositiveException(HERE) << "Error: the subSquare copula correlation matrix built from the given Spearman correlation matrix is not definite positive";
  return result;
}

/* Compute the correlation matrix of a SubSquare Copula from its Kendal correlation matrix */
CorrelationMatrix SubSquareCopula::GetCorrelationFromKendallCorrelation(const CorrelationMatrix & matrix)
{
  const UnsignedInteger dimension(matrix.getNbRows());
  CorrelationMatrix result(dimension);
  for (UnsignedInteger i = 1; i < dimension; ++i)
    for (UnsignedInteger j = 0; j < i; ++j) result(i, j) = sin(M_PI_2 * matrix(i, j));
  if (!result.isPositiveDefinite()) throw NotSymmetricDefinitePositiveException(HERE) << "Error: the subSquare copula correlation matrix built from the given Kendall correlation matrix is not definite positive";
  return result;
}

/* Method save() stores the object through the StorageManager */
void SubSquareCopula::save(Advocate & adv) const
{
  CopulaImplementation::save(adv);
  adv.saveAttribute( "correlation_", correlation_ );
  adv.saveAttribute( "covariance_duplicate", covariance_ );
  adv.saveAttribute( "subSquare_", subSquare_ );
  adv.saveAttribute( "integrationNodesNumber_duplicate", integrationNodesNumber_ );
  adv.saveAttribute( "isAlreadyComputedCovariance_duplicate", isAlreadyComputedCovariance_ );
}

/* Method load() reloads the object from the StorageManager */
void SubSquareCopula::load(Advocate & adv)
{
  // The range is generic for all the copulas
  CopulaImplementation::load(adv);
  adv.loadAttribute( "correlation_", correlation_ );
  adv.loadAttribute( "covariance_duplicate", covariance_ );
  adv.loadAttribute( "subSquare_", subSquare_ );
  adv.loadAttribute( "integrationNodesNumber_duplicate", integrationNodesNumber_ );
  adv.loadAttribute( "isAlreadyComputedCovariance_duplicate", isAlreadyComputedCovariance_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS
