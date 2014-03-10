//                                               -*- C++ -*-
/**
 *  @file  Indices.hxx
 *  @brief Set of indices defined as a Collection of UnsignedInteger class
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
#ifndef OPENTURNS_INDICES_HXX
#define OPENTURNS_INDICES_HXX

#include "OTprivate.hxx"
#include "PersistentCollection.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Indices
 *
 * A class that holds a collection of indices
 */
class Indices :
  public PersistentCollection<UnsignedInteger>
{
public:
  typedef PersistentCollection<UnsignedInteger>   InternalType;

  /** Default constructor */
  Indices()
    : InternalType()
  {
    // Nothing to do
  }

  /** Constructor that pre-allocate size elements */
  explicit Indices(const UnsignedInteger size) : InternalType(size)
  {
    // Nothing to do
  }

  /** Constructor that pre-allocate size elements with value */
  Indices(const UnsignedInteger size,
          const UnsignedInteger value) : InternalType(size, value)
  {
    // Nothing to do
  }

  /** Constructor from a range of elements */
  template <typename InputIterator>
  Indices(const InputIterator first,
          const InputIterator last)
    : InternalType(first, last)
  {
    // Nothing to do
  }

  /** Destructor */
  ~Indices() throw() {}

  /** Check that no value is repeated and no value exceed the given bound */
  Bool check(const UnsignedInteger bound) const;

  /** Fill the indices with a linear progression, starting from start value by step stepsize */
  void fill(const UnsignedInteger initialValue = 0,
            const UnsignedInteger stepSize = 1);
#ifdef SWIG
  /** @copydoc Object::__repr__() const */
  virtual String __repr__() const;

  /** @copydoc Object::__str__() const */
  virtual String __str__(const String & offset = "") const;
#endif

}; /* class Indices */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_INDICES_HXX */
