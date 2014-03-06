//                                               -*- C++ -*-
/**
 *  @file  OrthogonalBasis.hxx
 *  @brief This is the interface class for orthogonal basis
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
 *  @author dutka
 *  @date   2008-05-21 17:44:02 +0200 (Wed, 21 May 2008)
 */
#ifndef OPENTURNS_ORTHOGONALBASIS_HXX
#define OPENTURNS_ORTHOGONALBASIS_HXX

#include "TypedInterfaceObject.hxx"
#include "OrthogonalFunctionFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class OrthogonalBasis
 *
 * This is the interface class for orthogonal basis
 */

class OrthogonalBasis
  : public TypedInterfaceObject<OrthogonalFunctionFactory>
{
  CLASSNAME;
public:


  /** Default constructor */
  OrthogonalBasis();

  /** Constructor from implementation */
  OrthogonalBasis(const OrthogonalFunctionFactory & implementation);

  /** Build the NumericalMathFunction of the given index */
  NumericalMathFunction build(const UnsignedLong index) const;

  /** Return the measure upon which the basis is orthogonal */
  Distribution getMeasure() const;

  /** Return the enumerate function that translate unidimensional indices nto multidimensional indices */
  EnumerateFunction getEnumerateFunction() const;

  /** String converter */
  virtual String __repr__() const;

protected:

private:

} ; /* class OrthogonalBasis */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ORTHOGONALBASIS_HXX */
