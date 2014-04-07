//                                               -*- C++ -*-
/**
 *  @file  ContinuousDistribution.cxx
 *  @brief Abstract top-level class for Continuous distributions
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#include <cmath>
#include <cstdlib>

#include "ContinuousDistribution.hxx"
#include "Collection.hxx"
#include "Distribution.hxx"
#include "ResourceMap.hxx"
#include "MethodBoundNumericalMathEvaluationImplementation.hxx"
#include "GaussKronrod.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ContinuousDistribution);

/* Default constructor */
ContinuousDistribution::ContinuousDistribution(const String & name)
  : DistributionImplementation(name)
{
  // Nothing to do
}

/* Virtual constructor */
ContinuousDistribution * ContinuousDistribution::clone() const
{
  return new ContinuousDistribution(*this);
}

/* Comparison operator */
Bool ContinuousDistribution::operator ==(const ContinuousDistribution & other) const
{
  return (this == &other);
}

/* String converter */
String ContinuousDistribution::__repr__() const
{
  OSS oss;
  oss << "class=" << ContinuousDistribution::GetClassName();
  return oss;
}

/* Get the DDF of the distributionImplementation */
NumericalPoint ContinuousDistribution::computeDDF(const NumericalPoint & point) const
{
  const UnsignedInteger dimension(getDimension());
  NumericalPoint ddf(dimension);
  const NumericalScalar h(pow(pdfEpsilon_, 1.0 / 3.0));
  const NumericalScalar idenom(1.0 / (2.0 * h));
  for (UnsignedInteger i = 0; i < dimension; ++i)
  {
    NumericalPoint left(point);
    left[i] += h;
    NumericalPoint right(point);
    right[i] -= h;
    ddf[i] = (computePDF(left) - computePDF(right)) * idenom;
  }
  return ddf;
}

/* Get the PDF of the distribution */
NumericalScalar ContinuousDistribution::computePDF(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}

/* Get the CDF of the distribution */
NumericalScalar ContinuousDistribution::computeCDF(const NumericalPoint & point) const
{
  const UnsignedInteger dimension(getDimension());
  const NumericalPoint lowerBounds(getRange().getLowerBound());
  const NumericalPoint upperBounds(getRange().getUpperBound());
  Bool allOutside(true);
  for (UnsignedInteger i = 0; i < dimension; ++i)
  {
    if (point[i] <= lowerBounds[i]) return 0.0;
    allOutside &= (point[i] >= upperBounds[i]);
  }
  if (allOutside) return 1.0;
  const NumericalScalar cdf(ContinuousDistribution::computeProbability(Interval(getRange().getLowerBound(), point)));
  return cdf;
}

/* Get the survival function of the distribution */
NumericalScalar ContinuousDistribution::computeSurvivalFunction(const NumericalPoint & point) const
{
  const UnsignedInteger dimension(getDimension());
  if (dimension == 1) return computeComplementaryCDF(point);
  const NumericalPoint lowerBounds(getRange().getLowerBound());
  const NumericalPoint upperBounds(getRange().getUpperBound());
  Bool allOutside(true);
  for (UnsignedInteger i = 0; i < dimension; ++i)
  {
    if (point[i] >= upperBounds[i]) return 0.0;
    allOutside &= (point[i] <= lowerBounds[i]);
  }
  if (allOutside) return 1.0;
  const NumericalScalar survival(ContinuousDistribution::computeProbability(Interval(point, getRange().getUpperBound())));
  return survival;
}

/* Get the probability content of an interval */
NumericalScalar ContinuousDistribution::computeProbability(const Interval & interval) const
{
  const Interval reducedInterval(interval.intersect(getRange()));
  if (reducedInterval.isNumericallyEmpty()) return 0.0;
  if (reducedInterval == getRange()) return 1.0;
  const UnsignedInteger dimension(getDimension());
  const NumericalPoint lowerBounds(reducedInterval.getLowerBound());
  const NumericalPoint upperBounds(reducedInterval.getUpperBound());
  NumericalSample nodesAndWeights(getGaussNodesAndWeights());
  // Perform the integration
  const UnsignedInteger marginalNodesNumber(getIntegrationNodesNumber());
  const UnsignedInteger size(static_cast< UnsignedInteger >(round(pow(static_cast<double>(marginalNodesNumber), static_cast<int>(dimension)))));
  NumericalScalar probability(0.0);
  Indices indices(dimension, 0);
  for (UnsignedInteger linearIndex = 0; linearIndex < size; ++linearIndex)
  {
    NumericalPoint node(dimension);
    NumericalScalar weight(1.0);
    for (UnsignedInteger j = 0; j < dimension; ++j)
    {
      const UnsignedInteger indiceJ(indices[j]);
      const NumericalScalar delta(0.5 * (upperBounds[j] - lowerBounds[j]));
      node[j] = lowerBounds[j] + delta * (1.0 + nodesAndWeights[0][indiceJ]);
      weight *= delta * nodesAndWeights[1][indiceJ];
    }
    probability += weight * computePDF(node);
    /* Update the indices */
    ++indices[0];
    /* Propagate the remainders */
    for (UnsignedInteger j = 0; j < dimension - 1; ++j) indices[j + 1] += (indices[j] == marginalNodesNumber);
    /* Correction of the indices. The last index cannot overflow. */
    for (UnsignedInteger j = 0; j < dimension - 1; ++j) indices[j] = indices[j] % marginalNodesNumber;
  } // Loop over the n-D nodes
  return probability;
}

/* Build a C1 interpolation of the CDF function for 1D continuous distributions */
Collection<PiecewiseHermiteEvaluationImplementation> ContinuousDistribution::interpolateCDF(const UnsignedInteger n)
{
  if (getDimension() != 1) throw NotYetImplementedException(HERE) << "Error: cannot interpolate CDF for multidimensional distributions.";
  const PDFWrapper pdfWrapper(this);
  const NumericalMathFunction fPDF(bindMethod<PDFWrapper, NumericalPoint, NumericalPoint>(pdfWrapper, &PDFWrapper::computePDF, 1, 1));
  const NumericalScalar xMin(getRange().getLowerBound()[0]);
  const NumericalScalar xMax(getRange().getUpperBound()[0]);
  const NumericalScalar mu(getMean()[0]);
  NumericalPoint locationsCDF(n);
  NumericalPoint locationsCCDF(n);
  NumericalPoint valuesCDF(n);
  NumericalPoint valuesCCDF(n);
  NumericalPoint derivativesCDF(n);
  NumericalPoint derivativesCCDF(n);
  NumericalScalar xCDFOld(xMin);
  NumericalScalar xCCDFOld(xMax);
  locationsCDF[0] = xMin;
  locationsCCDF[n - 1] = xMax;
  GaussKronrod algo;
  const NumericalScalar stepCDF((mu - xMin) / (n - 1.0));
  const NumericalScalar stepCCDF((xMax - mu) / (n - 1.0));
  for (UnsignedInteger i = 1; i < n; ++i)
  {
    const NumericalScalar xCDF(xMin + i * stepCDF);
    const NumericalScalar xCCDF(xMax - i * stepCCDF);
    locationsCDF[i] = xCDF;
    locationsCCDF[n - i - 1] = xCCDF;
    NumericalPoint ai;
    NumericalPoint bi;
    NumericalPoint fi;
    NumericalPoint ei;
    NumericalScalar error;
    valuesCDF[i] = valuesCDF[i - 1] + algo.integrate(fPDF, xCDFOld, xCDF, error, ai, bi, fi, ei);
    valuesCCDF[n - i - 1] = valuesCCDF[n - i] + algo.integrate(fPDF, xCCDF, xCCDFOld, error, ai, bi, fi, ei);
    derivativesCDF[i] = computePDF(xCDF);
    derivativesCCDF[n - i - 1] = -computePDF(xCCDF);
    xCDFOld = xCDF;
    xCCDFOld = xCCDF;
  }
  Collection<PiecewiseHermiteEvaluationImplementation> coll(2);
  coll[0] = PiecewiseHermiteEvaluationImplementation(locationsCDF, valuesCDF, derivativesCDF);
  coll[1] = PiecewiseHermiteEvaluationImplementation(locationsCCDF, valuesCCDF, derivativesCCDF);
  return coll;
}

/* Tell if the distribution is continuous */
Bool ContinuousDistribution::isContinuous() const
{
  return true;
}

END_NAMESPACE_OPENTURNS
