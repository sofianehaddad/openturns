//                                               -*- C++ -*-
/**
 *  @file  GaussKronrod.hxx
 *  @brief This class allows to compute integrals of a function over an interval
 *         using GaussKronrod method for 1D scalar function
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
#ifndef OPENTURNS_GAUSSKRONROD_HXX
#define OPENTURNS_GAUSSKRONROD_HXX

#include "IntegrationAlgorithmImplementation.hxx"
#include "GaussKronrodRule.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class GaussKronrod
 */

class GaussKronrod
  : public IntegrationAlgorithmImplementation
{

  CLASSNAME;

public:

  /** Default constructor without parameters */
  explicit GaussKronrod(const String & name = DefaultName);

  /** Parameter constructor */
  GaussKronrod(const UnsignedLong maximumSubIntervals,
               const NumericalScalar maximumError,
               const GaussKronrodRule & rule);

  /** Virtual copy constructor */
  virtual GaussKronrod * clone() const;

  /** Compute an approximation of \int_{[a,b]}f(x)dx, where [a,b]
   * is an 1D interval
   */
  using IntegrationAlgorithmImplementation::integrate;
#ifndef SWIG
  virtual NumericalScalar integrate(const NumericalMathFunction & function,
                                    const Interval & interval,
                                    NumericalScalar & error) const;
  virtual NumericalScalar integrate(const NumericalMathFunction & function,
                                    const NumericalScalar a,
                                    const NumericalScalar b,
                                    NumericalScalar & error,
                                    NumericalPoint & ai,
                                    NumericalPoint & bi,
                                    NumericalPoint & fi,
                                    NumericalPoint & ei) const;
#endif
  virtual NumericalScalar integrate(const NumericalMathFunction & function,
                                    const NumericalScalar a,
                                    const NumericalScalar b,
                                    NumericalPoint & error,
                                    NumericalPoint & ai,
                                    NumericalPoint & bi,
                                    NumericalPoint & fi,
                                    NumericalPoint & ei) const;

  /** Maximum sub-intervals accessor */
  UnsignedLong getMaximumSubIntervals() const;
  void setMaximumSubIntervals(const UnsignedLong maximumSubIntervals);

  /** Maximum error accessor */
  NumericalScalar getMaximumError() const;
  void setMaximumError(const NumericalScalar maximumError);

  /** Rule accessor */
  GaussKronrodRule getRule() const;
  void setRule(const GaussKronrodRule & rule);

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

private:

  /** Compute the local GaussKronrod rule over [a, b] */
  NumericalScalar computeRule(const NumericalMathFunction & function,
                              const NumericalScalar a,
                              const NumericalScalar b,
                              NumericalScalar & localError) const;

  /* Maximum number of sub-intervals */
  UnsignedLong maximumSubIntervals_;

  /* Maximum estimated error */
  NumericalScalar maximumError_;

  /* Local integration rule */
  GaussKronrodRule rule_;

} ; /* class GaussKronrod */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_GAUSSKRONROD_HXX */
