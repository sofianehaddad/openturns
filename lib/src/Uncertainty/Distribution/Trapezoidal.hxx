//                                               -*- C++ -*-
/**
 *  @file  Trapezoidal.hxx
 *  @brief The Trapezoidal distribution
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
 *  @date   2009-05-28 14:47:53 +0200 (Thu, 28 May 2009)
 */
#ifndef OPENTURNS_TRAPEZOIDAL_HXX
#define OPENTURNS_TRAPEZOIDAL_HXX

#include "OTprivate.hxx"
#include "ContinuousDistribution.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Trapezoidal
 *
 * The Trapezoidal distribution.
 */
class Trapezoidal
  : public ContinuousDistribution
{
  CLASSNAME;
public:

  typedef Pointer<DistributionImplementation> Implementation;

  /** Default constructor */
  Trapezoidal();

  /** Parameters constructor */
  Trapezoidal(const NumericalScalar a,
              const NumericalScalar b,
              const NumericalScalar c,
              const NumericalScalar d);

  /** Comparison operator */
  Bool operator ==(const Trapezoidal & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /* Interface inherited from Distribution */

  /** Virtual constructor */
  virtual Trapezoidal * clone() const;

  /** Get one realization of the distribution */
  NumericalPoint getRealization() const;

  /** Get the DDF of the distribution, i.e. the gradient of its PDF w.r.t. point */
  using ContinuousDistribution::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the distribution */
  using ContinuousDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the distribution, i.e. P(X <= point) = CDF(point) */
  using ContinuousDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
  NumericalComplex computeCharacteristicFunction(const NumericalScalar x) const;

  /** Get the gradient of the PDF w.r.t the parameters of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the gradient of the CDF w.r.t the parameters of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the roughness, i.e. the L2-norm of the PDF */
  NumericalScalar getRoughness() const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Get the raw moments of the standardized distribution */
  NumericalPoint getStandardMoment(const UnsignedLong n) const;

  /** Get the standard representative in the parametric family, associated with the standard moments */
  Implementation getStandardRepresentative() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using ContinuousDistribution::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /* Interface specific to Trapezoidal */

  /** A accessor */
  void setA(const NumericalScalar a);
  NumericalScalar getA() const;

  /** B accessor */
  void setB(const NumericalScalar b);
  NumericalScalar getB() const;

  /** C accessor */
  void setC(const NumericalScalar c);
  NumericalScalar getC() const;

  /** D accessor */
  void setD(const NumericalScalar d);
  NumericalScalar getD() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


protected:


private:

  /** Get the quantile of the distribution, i.e the value Xp such that P(X <= Xp) = prob */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false) const;

  /** Compute the mean of the distribution */
  void computeMean() const;

  /** Compute the covariance of the distribution */
  void computeCovariance() const;

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** Update attributes */
  void update();

  /** Sigma accessor */
  NumericalScalar getSigma() const;

  /** The main parameter set of the distribution */
  NumericalScalar a_;
  NumericalScalar b_;
  NumericalScalar c_;
  NumericalScalar d_;
  NumericalScalar h_;

}; /* class Trapezoidal */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_TRAPEZOIDAL_HXX */
