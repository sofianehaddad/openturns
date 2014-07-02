//                                               -*- C++ -*-
/**
 *  @file  Description.cxx
 *  @brief Description is a collection of string for human usage
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
#include "Description.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Description);

TEMPLATE_CLASSNAMEINIT(PersistentCollection<String>);

static Factory<PersistentCollection<String> > RegisteredFactory2("PersistentCollection<String>");

static Factory<Description> RegisteredFactory("Description");

/* Default constructor */
Description::Description()
  : InternalType()
{
  // Nothing to do
}


/* Constructor with size */
Description::Description(const UnsignedInteger size)
  : InternalType(size)
{
  // Nothing to do
}


/* Constructor with size and default value */
Description::Description(const UnsignedInteger size,
                         const String & value)
  : InternalType(size, value)
{
  // Nothing to do
}

/* Constructor with size and default value */
Description::Description(const Collection<String> & coll)
  : InternalType(coll)
{
  // Nothing to do
}

/* Check if the content is blank */
Bool Description::isBlank() const
{
  for (UnsignedInteger i = 0; i < getSize(); ++i)
  {
    const UnsignedInteger length((*this)[i].size());
    for (UnsignedInteger j = 0; j < length; ++j)
      if (!isblank((*this)[i][j])) return false;
  }
  return true;
}

/* Destructor */
Description::~Description()
{
  // Nothing to do
}

void Description::sort()
{
  std::sort(begin(), end());
}

Description Description::BuildDefault(const UnsignedInteger dimension,
                                      const String & prefix)
{
  Description description(dimension);
  for (UnsignedInteger k = 0; k < dimension; ++k) description[k] =  String(OSS() << prefix << k);
  return description;
}

END_NAMESPACE_OPENTURNS



