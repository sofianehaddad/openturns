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
#include "IntegrationAlgorithm.hxx"
#include "PersistentObjectFactory.hxx"
#include "GaussKronrod.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(IntegrationAlgorithm);

/* Constructor with parameters */
IntegrationAlgorithm::IntegrationAlgorithm()
  : TypedInterfaceObject<IntegrationAlgorithmImplementation>(new GaussKronrod())
{
  // Nothing to do
}

/* Parameters constructor */
IntegrationAlgorithm::IntegrationAlgorithm(const IntegrationAlgorithmImplementation & implementation,
    const String & name)
  : TypedInterfaceObject<IntegrationAlgorithmImplementation>(implementation.clone())
{
  getImplementation()->setName(name);
}

/* Constructor from implementation */
IntegrationAlgorithm::IntegrationAlgorithm(const Implementation & p_implementation,
    const String & name)
  : TypedInterfaceObject<IntegrationAlgorithmImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* Constructor from implementation pointer */
IntegrationAlgorithm::IntegrationAlgorithm(IntegrationAlgorithmImplementation * p_implementation,
    const String & name)
  : TypedInterfaceObject<IntegrationAlgorithmImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}


/* Compute an approximation of \int_{[a,b]}f(x)dx, where [a,b]
 * is an n-D interval
 */
NumericalScalar IntegrationAlgorithm::integrate(const NumericalMathFunction & function,
    const Interval & interval) const
{
  return getImplementation()->integrate(function, interval);
}

NumericalScalar IntegrationAlgorithm::integrate(const NumericalMathFunction & function,
    const Interval & interval,
    NumericalScalar & error) const
{
  return getImplementation()->integrate(function, interval, error);
}

NumericalScalar IntegrationAlgorithm::integrate(const NumericalMathFunction & function,
    const Interval & interval,
    NumericalPoint & error) const
{
  return getImplementation()->integrate(function, interval, error);
}

/* String converter */
String IntegrationAlgorithm::__repr__() const
{
  OSS oss(true);
  oss << "class=" << IntegrationAlgorithm::GetClassName()
      << " implementation=" << getImplementation()->__repr__();
  return oss;
}

/* String converter */
String IntegrationAlgorithm::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "class=" << IntegrationAlgorithm::GetClassName()
      << " implementation=" << getImplementation()->__str__(offset);
  return oss;
}

END_NAMESPACE_OPENTURNS
