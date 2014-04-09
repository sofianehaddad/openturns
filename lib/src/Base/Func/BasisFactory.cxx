//                                               -*- C++ -*-
/**
 *  @file  BasisFactory.cxx
 *  @brief Abstract top-level class for all BasisFactory
 *
 *  (C) Copyright 2005-2014 Airbus-EDF-Phimeca
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License.
 *
 *  This library is distributed in the hope that it will be useful
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 *  @author schueller
 */
#include "BasisFactory.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(BasisFactory);

static Factory<BasisFactory> RegisteredFactory("BasisFactory");


/* Default constructor */
BasisFactory::BasisFactory()
  : PersistentObject()
{
  // Nothing to do
}

/* String converter */
String BasisFactory::__repr__() const
{
  return OSS() << "class=" << BasisFactory::GetClassName()
         << " name=" << getName();
}


/* Here is the interface that all derived class must implement */

BasisFactory* BasisFactory::clone() const
{
  return new BasisFactory(*this);
}


Basis BasisFactory::build() const
{
  throw NotYetImplementedException(HERE);
}


END_NAMESPACE_OPENTURNS
