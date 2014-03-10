//                                               -*- C++ -*-
/**
 *  @file  GaussKronrod.cxx
 *  @brief
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
#include "GaussKronrod.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class GaussKronrod
 */

CLASSNAMEINIT(GaussKronrod);

static Factory<GaussKronrod> RegisteredFactory("GaussKronrod");

/* Constructor without parameters */
GaussKronrod::GaussKronrod(const String & name)
  : IntegrationAlgorithmImplementation(name)
  , maximumSubIntervals_(ResourceMap::GetAsUnsignedInteger("GaussKronrod-MaximumSubIntervals"))
  , maximumError_(ResourceMap::GetAsNumericalScalar("GaussKronrod-MaximumError"))
  , rule_()
{
  // Nothing to do
}

/* Parameters constructor */
GaussKronrod::GaussKronrod(const UnsignedInteger maximumSubIntervals,
                           const NumericalScalar maximumError,
                           const GaussKronrodRule & rule)
  : IntegrationAlgorithmImplementation()
  , maximumSubIntervals_(maximumSubIntervals)
  , maximumError_(maximumError)
  , rule_(rule)
{
  // Nothing to do
}

/* Virtual constructor */
GaussKronrod * GaussKronrod::clone() const
{
  return new GaussKronrod(*this);
}

/* Compute an approximation of \int_{[a,b]}f(x)dx, where [a,b]
 * is an 1D interval and f a scalar function
 */
NumericalScalar GaussKronrod::integrate(const NumericalMathFunction & function,
                                        const Interval & interval,
                                        NumericalScalar & error) const
{
  if (interval.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given interval should be 1D, here dimension=" << interval.getDimension();
  NumericalPoint ai(0);
  NumericalPoint bi(0);
  NumericalPoint fi(0);
  NumericalPoint ei(0);
  return integrate(function, interval.getLowerBound()[0], interval.getUpperBound()[0], error, ai, bi, fi, ei);
}

NumericalScalar GaussKronrod::integrate(const NumericalMathFunction & function,
                                        const NumericalScalar a,
                                        const NumericalScalar b,
                                        NumericalPoint & error,
                                        NumericalPoint & ai,
                                        NumericalPoint & bi,
                                        NumericalPoint & fi,
                                        NumericalPoint & ei) const
{
  error = NumericalPoint(1);
  return integrate(function, a, b, error[0], ai, bi, fi, ei);
}

NumericalScalar GaussKronrod::integrate(const NumericalMathFunction & function,
                                        const NumericalScalar a,
                                        const NumericalScalar b,
                                        NumericalScalar & error,
                                        NumericalPoint & ai,
                                        NumericalPoint & bi,
                                        NumericalPoint & fi,
                                        NumericalPoint & ei) const
{
  if (function.getInputDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can integrate only 1D function, here input dimension=" << function.getInputDimension();
  if (function.getOutputDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can integrate only scalar function, here output dimension=" << function.getOutputDimension();
  NumericalScalar result(0.0);
  ai = NumericalPoint(maximumSubIntervals_);
  ai[0] = a;
  bi = NumericalPoint(maximumSubIntervals_);
  bi[0] = b;
  fi = NumericalPoint(maximumSubIntervals_);
  ei = NumericalPoint(maximumSubIntervals_);
  UnsignedInteger ip(0);
  UnsignedInteger im(0);
  error = maximumError_;
  while ((error > 0.25 * maximumError_) && (im < maximumSubIntervals_ - 1))
  {
    ++im;
    bi[im] = bi[ip];
    ai[im] = 0.5 * (ai[ip] + bi[ip]);
    bi[ip] = ai[im];
    fi[ip] = computeRule(function, ai[ip], bi[ip], ei[ip]);
    fi[im] = computeRule(function, ai[im], bi[im], ei[im]);
    UnsignedInteger iErrorMax(0);
    NumericalScalar errorMax(0.0);
    error = 0.0;
    result = 0.0;
    for (UnsignedInteger i = 0; i <= im; ++i)
    {
      const NumericalScalar localError(ei[i]);
      result += fi[i];
      error += localError * localError;
      if (localError > errorMax)
      {
        errorMax = localError;
        iErrorMax = i;
      }
    }
    ip = iErrorMax;
    error = sqrt(error);
  } // while (error >...)
  ai.resize(im + 1);
  bi.resize(im + 1);
  ei.resize(im + 1);
  fi.resize(im + 1);
  if (error > maximumError_) LOGINFO(OSS() << "The GaussKronrod algorithm was not able to reach the requested error=" << maximumError_ << ", the achieved error is " << error);
  return result;
}

/* Compute the local GaussKronrod rule over [a, b]. */
NumericalScalar GaussKronrod::computeRule(const NumericalMathFunction & function,
    const NumericalScalar a,
    const NumericalScalar b,
    NumericalScalar & localError) const
{
  const NumericalScalar width(0.5 * (b - a));
  const NumericalScalar center(0.5 * (a + b));
  // Generate the set of points
  NumericalSample x(2 * rule_.order_ + 1, 1);
  x[0][0] = center;
  for (UnsignedInteger i = 0; i < rule_.order_; ++i)
  {
    const NumericalScalar t(width * rule_.otherKronrodNodes_[i]);
    x[2 * i + 1][0] = center - t;
    x[2 * i + 2][0] = center + t;
  }
  // Use possible parallelization of the evaluation
  const NumericalSample y(function(x));
  NumericalScalar value(y[0][0]);
  NumericalScalar resultGauss(value * rule_.zeroGaussWeight_);
  NumericalScalar resultGaussKronrod(value * rule_.zeroKronrodWeight_);
  for (UnsignedInteger j = 0; j < (rule_.order_ - 1) / 2; ++j)
  {
    value = y[4 * j + 1][0] + y[4 * j + 2][0];
    resultGaussKronrod += rule_.otherKronrodWeights_[2 * j] * value;
    value = y[4 * j + 3][0] + y[4 * j + 4][0];
    resultGaussKronrod += rule_.otherKronrodWeights_[2 * j + 1] * value;
    resultGauss += rule_.otherGaussWeights_[j] * value;
  }
  value = y[2 * rule_.order_ - 1][0] + y[2 * rule_.order_][0];
  resultGaussKronrod = width * (resultGaussKronrod + rule_.otherKronrodWeights_[rule_.order_ - 1] * value);
  localError = fabs(resultGaussKronrod - width * resultGauss);
  return resultGaussKronrod;
}

/* Maximum sub-intervals accessor */
UnsignedInteger GaussKronrod::getMaximumSubIntervals() const
{
  return maximumSubIntervals_;
}

void GaussKronrod::setMaximumSubIntervals(const UnsignedInteger maximumSubIntervals)
{
  if (maximumSubIntervals < 1) throw InvalidArgumentException(HERE) << "Error: the number of intervals must be at least 1.";
  maximumSubIntervals_ = maximumSubIntervals;
}

/* Maximum error accessor */
NumericalScalar GaussKronrod::getMaximumError() const
{
  return maximumError_;
}

void GaussKronrod::setMaximumError(const NumericalScalar maximumError)
{
  if (maximumError < 0.0) throw InvalidArgumentException(HERE) << "Error: the maximum error must be nonnegative, here maximum error=" << maximumError;
  maximumError_ = maximumError;
}

/* Rule accessor */
GaussKronrodRule GaussKronrod::getRule() const
{
  return rule_;
}

void GaussKronrod::setRule(const GaussKronrodRule & rule)
{
  rule_ = rule;
}

/* String converter */
String GaussKronrod::__repr__() const
{
  OSS oss(true);
  oss << "class=" << GaussKronrod::GetClassName()
      << ", maximum sub intervals=" << maximumSubIntervals_
      << ", maximum error=" << maximumError_
      << ", rule=" << rule_;
  return oss;
}

/* String converter */
String GaussKronrod::__str__(const String & offset) const
{
  OSS oss(false);
  oss << offset << GaussKronrod::GetClassName()
      << "(maximum sub intervals=" << maximumSubIntervals_
      << ", maximum error=" << maximumError_
      << ", rule=" << rule_.__str__(offset)
      << ")";
  return oss;
}

END_NAMESPACE_OPENTURNS
