//                                               -*- C++ -*-
/**
 *  @file  DescriptionImplementation.cxx
 *  @brief DescriptionImplementation is the active part of Description
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include <ctype.h> // for isblank

#include "DescriptionImplementation.hxx"
#include "Log.hxx"
#include "StorageManager.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(DescriptionImplementation);
TEMPLATE_CLASSNAMEINIT(PersistentCollection<String>);

static Factory<DescriptionImplementation> RegisteredFactory1("DescriptionImplementation");
static Factory<PersistentCollection<String> > RegisteredFactory2("PersistentCollection<String>");

/* Default constructor */
DescriptionImplementation::DescriptionImplementation()
  : PersistentCollection<String>()
{
  // Nothing to do
}

/* Constructor with size */
DescriptionImplementation::DescriptionImplementation(const UnsignedInteger size)
  : PersistentCollection<String>(size)
{
  // Nothing to do
}

/* Constructor with size */
DescriptionImplementation::DescriptionImplementation(const String & value)
  : PersistentCollection<String>(1, value)
{
  // Nothing to do
}


/* Constructor with size and default value */
DescriptionImplementation::DescriptionImplementation(const UnsignedInteger size,
    const String & value)
  : PersistentCollection<String>(size, value)
{
  // Nothing to do
}


/* Constructor from a collection */
DescriptionImplementation::DescriptionImplementation(const Collection<String> & coll)
  : PersistentCollection<String>(coll)
{
  // Nothing to do
}


/* Virtual constructor */
DescriptionImplementation * DescriptionImplementation::clone() const
{
  return new DescriptionImplementation(*this);
}

/* Check if the content is blank */
Bool DescriptionImplementation::isBlank() const
{
  for (UnsignedInteger i = 0; i < getSize(); ++i)
  {
    const UnsignedInteger length((*this)[i].size());
    for (UnsignedInteger j = 0; j < length; ++j)
      if (!isblank((*this)[i][j])) return false;
  }
  return true;
}

/* String converter */
String DescriptionImplementation::__repr__() const
{
  OSS oss(true);
  oss << "[";

  if (!isEmpty())
  {
    String separator = "";
    for(UnsignedInteger i = 0; i < getSize(); ++i, separator = ",")
      oss << separator << (*this)[i];
  }

  oss << "]";

  return oss;
}

String DescriptionImplementation::__str__(const String & offset) const
{
  return PersistentCollection<String>::__str__();
}


/* Method save() stores the object through the StorageManager */
void DescriptionImplementation::save(Advocate & adv) const
{
  PersistentCollection<String>::save(adv);
}


/* Method load() reloads the object from the StorageManager */
void DescriptionImplementation::load(Advocate & adv)
{
  PersistentCollection<String>::load(adv);
}

/* Build a default description */
DescriptionImplementation DescriptionImplementation::BuildDefault(const UnsignedInteger dimension,
    const String & prefix)
{
  DescriptionImplementation description(dimension);
  for (UnsignedInteger k = 0; k < dimension; ++k) description[k] =  String(OSS() << prefix << k);
  return description;
}


END_NAMESPACE_OPENTURNS
