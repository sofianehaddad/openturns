//                                               -*- C++ -*-
/**
 *  @file  IntegrationAlgorithmImplementation.cxx
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
#include "IntegrationAlgorithmImplementation.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class IntegrationAlgorithmImplementation
 */

CLASSNAMEINIT(IntegrationAlgorithmImplementation);

static Factory<IntegrationAlgorithmImplementation> RegisteredFactory("IntegrationAlgorithmImplementation");

/* Constructor without parameters */
IntegrationAlgorithmImplementation::IntegrationAlgorithmImplementation()
  : PersistentObject()
{
  // Nothing to do
}

/* Virtual constructor */
IntegrationAlgorithmImplementation * IntegrationAlgorithmImplementation::clone() const
{
  return new IntegrationAlgorithmImplementation(*this);
}

/* Compute an approximation of \int_{[a,b]}f(x)dx, where [a,b]
 * is an n-D interval
 */
NumericalScalar IntegrationAlgorithmImplementation::integrate(const NumericalMathFunction & function,
    const Interval & interval) const
{
  NumericalScalar error(0.0);
  return integrate(function, interval, error);
}

NumericalScalar IntegrationAlgorithmImplementation::integrate(const NumericalMathFunction & function,
    const Interval & interval,
    NumericalPoint & error) const
{
  error = NumericalPoint(1);
  return integrate(function, interval, error[0]);
}

NumericalScalar IntegrationAlgorithmImplementation::integrate(const NumericalMathFunction & function,
    const Interval & interval,
    NumericalScalar & error) const
{
  throw NotYetImplementedException(HERE);
}

/* String converter */
String IntegrationAlgorithmImplementation::__repr__() const
{
  OSS oss(true);
  oss << "class=" << IntegrationAlgorithmImplementation::GetClassName();
  return oss;
}

/* String converter */
String IntegrationAlgorithmImplementation::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "class=" << IntegrationAlgorithmImplementation::GetClassName();
  return oss;
}

END_NAMESPACE_OPENTURNS
