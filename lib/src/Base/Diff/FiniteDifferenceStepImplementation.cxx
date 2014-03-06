//                                             -*- C++ -*-
/**
 *  @file  FiniteDifferenceStepImplementation.cxx
 *  @brief Implementation of finite difference step
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
 *  @author slama
 *  @date   2008-11-19 16:10:42 +0100 (mer, 19 nov 2008)
 */

#include "FiniteDifferenceStepImplementation.hxx"
#include "PersistentObjectFactory.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(FiniteDifferenceStepImplementation);

static Factory<FiniteDifferenceStepImplementation> RegisteredFactory("FiniteDifferenceStepImplementation");

/* Default constructor */
FiniteDifferenceStepImplementation::FiniteDifferenceStepImplementation()
  : PersistentObject()
  , epsilon_(1.0)
{
  // Nothing to do
}

/* Parameters constructor */
FiniteDifferenceStepImplementation::FiniteDifferenceStepImplementation(const NumericalPoint & epsilon)
  : PersistentObject()
  , epsilon_(epsilon)
{
  // Nothing to do
}

/* Virtual constructor */
FiniteDifferenceStepImplementation * FiniteDifferenceStepImplementation::clone() const
{
  return new FiniteDifferenceStepImplementation(*this);
}

/* Epsilon accessor */
void FiniteDifferenceStepImplementation::setEpsilon(const NumericalPoint & epsilon)
{
  const UnsignedLong dimension( epsilon.getDimension() );
  for (UnsignedLong i = 0; i < dimension; ++ i)
  {
    if (epsilon[i] < SpecFunc::MinNumericalScalar ) throw InvalidArgumentException(HERE) << "Null epsilon for component " << i;
  }
  epsilon_ = epsilon;
}

NumericalPoint FiniteDifferenceStepImplementation::getEpsilon() const
{
  return epsilon_;
}

/* Evaluation operator */
NumericalPoint FiniteDifferenceStepImplementation::operator()(const NumericalPoint & inP) const
{
  throw NotYetImplementedException(HERE);
}

/* String converter */
String FiniteDifferenceStepImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << FiniteDifferenceStepImplementation::GetClassName()
      << " epsilon=" << epsilon_.__repr__();
  return oss;
}

/* Method save() stores the object through the StorageManager */
void FiniteDifferenceStepImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute("epsilon_", epsilon_);
}

/* Method load() reloads the object from the StorageManager */
void FiniteDifferenceStepImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute("epsilon_", epsilon_);
}


END_NAMESPACE_OPENTURNS

