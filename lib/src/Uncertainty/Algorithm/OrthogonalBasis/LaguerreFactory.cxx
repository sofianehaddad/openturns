//                                               -*- C++ -*-
/**
 *  @file  LaguerreFactory.cxx
 *  @brief Laguerre polynomial factory
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
 *  @date   2008-05-21 11:21:38 +0200 (Wed, 21 May 2008)
 */
#include "LaguerreFactory.hxx"
#include "PersistentObjectFactory.hxx"
#include "Gamma.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(LaguerreFactory);

static Factory<LaguerreFactory> RegisteredFactory("LaguerreFactory");



/* Default constructor, associated with the default Gamma distribution which is equal to the Exponential distribution */
LaguerreFactory::LaguerreFactory()
  : OrthogonalUniVariatePolynomialFactory(Gamma()), k_(0.0)
{
  initializeCache();
}


/* Parameter constructor: k is the order of the generalized Laguerre polynomial, associated with the Gamma(k+1, 1, 0) distribution */
LaguerreFactory::LaguerreFactory(const NumericalScalar k,
                                 const ParameterSet parameterization)
  : OrthogonalUniVariatePolynomialFactory( (parameterization == ANALYSIS ? Gamma(k + 1.0, 1.0, 0.0) : Gamma(k, 1.0, 0.0) ) ),
    k_(k)
{
  initializeCache();
}

/* Virtual constructor */
LaguerreFactory * LaguerreFactory::clone() const
{
  return new LaguerreFactory(*this);
}


/* Calculate the coefficients of recurrence a0n, a1n, a2n such that
   Pn+1(x) = (a0n * x + a1n) * Pn(x) + a2n * Pn-1(x) */
LaguerreFactory::Coefficients LaguerreFactory::getRecurrenceCoefficients(const UnsignedLong n) const
{
  Coefficients recurrenceCoefficients(3, 0.0);
  if (n == 0)
  {
    const NumericalScalar factor(sqrt(k_ + 1.0));
    recurrenceCoefficients[0] = 1.0 / factor;
    recurrenceCoefficients[1] = -factor;
    // Conventional value of 0.0 for recurrenceCoefficients[2]
    return recurrenceCoefficients;
  }
  const NumericalScalar factor(1.0 / sqrt((n + 1.0) * (n + 1.0 + k_)));
  recurrenceCoefficients[0] = factor;
  recurrenceCoefficients[1] = -(2.0 * n + 1.0 + k_) * factor;
  recurrenceCoefficients[2] = -sqrt((n + k_) * n) * factor;
  return recurrenceCoefficients;
}

/* K accessor */
NumericalScalar LaguerreFactory::getK() const
{
  return k_;
}

/* String converter */
String LaguerreFactory::__repr__() const
{
  return OSS() << "class=" << getClassName()
         << " k=" << k_
         << " measure=" << measure_;
}


/* Method save() stores the object through the StorageManager */
void LaguerreFactory::save(Advocate & adv) const
{
  OrthogonalUniVariatePolynomialFactory::save(adv);
  adv.saveAttribute( "k_", k_ );
}


/* Method load() reloads the object from the StorageManager */
void LaguerreFactory::load(Advocate & adv)
{
  OrthogonalUniVariatePolynomialFactory::load(adv);
  adv.loadAttribute( "k_", k_ );
}

END_NAMESPACE_OPENTURNS
