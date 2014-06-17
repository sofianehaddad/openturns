//                                               -*- C++ -*-
/**
 *  @file  Description.hxx
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
#ifndef OPENTURNS_DESCRIPTION_HXX
#define OPENTURNS_DESCRIPTION_HXX

#include "TypedCollectionInterfaceObject.hxx"
#include "DescriptionImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @typedef Description
 *
 * Description is a collection of string for human usage
 */

class OT_API Description :
  public TypedCollectionInterfaceObject<DescriptionImplementation>
{
  CLASSNAME;
public:

  /** Default constructor */
  Description();

  /** Constructor with size */
  explicit Description(const UnsignedInteger size);

  /** Constructor with value */
  explicit Description(const String & value);

  /** Constructor with size and default value */
  Description(const UnsignedInteger size,
              const String & value);

  /** Constructor from a collection */
  Description(const Collection<String> & coll);

  /** Contructor from implementation */
  Description(const DescriptionImplementation & implementation);

  /** Check if the content is blank */
  Bool isBlank() const;

  /** Destructor */
  virtual ~Description();

#ifdef SWIG
  /** @copydoc Object::__repr__() const */
  virtual String __repr__() const;

  /** @copydoc Object::__str__() const */
  virtual String __str__(const String & offset = "") const;
#endif

  void sort();

  /** Build a default description */
  static Description BuildDefault(const UnsignedInteger dimension,
      const String & prefix = "Component");


}; /* class Description */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DESCRIPTION_HXX */
