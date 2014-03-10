//                                               -*- C++ -*-
/**
 *  @file  Indices.cxx
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
#include <algorithm>
#include "Indices.hxx"

BEGIN_NAMESPACE_OPENTURNS

/* Check that no value is repeated and no value exceed the given bound */
Bool Indices::check(const UnsignedInteger bound) const
{
  // Check if the values are all different
  Indices copy(*this);
  std::sort(copy.begin(), copy.end());
  iterator iter = std::unique(copy.begin(), copy.end());
  if (iter < copy.end()) return false;
  // Check if the values are in the given bound
  if (*max_element(begin(), end()) > bound) return false;
  return true;
}

/* Fill the indices with a linear progression, starting from start value by step stepsize */
void Indices::fill(const UnsignedInteger initialValue,
                   const UnsignedInteger stepSize)
{
  const UnsignedInteger size(getSize());
  UnsignedInteger value(initialValue);
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    operator[](i) = value;
    value += stepSize;
  }
}

END_NAMESPACE_OPENTURNS
