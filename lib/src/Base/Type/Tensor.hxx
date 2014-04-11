//                                               -*- C++ -*-
/**
 *  @file  Tensor.hxx
 *  @brief Tensor implements the classical mathematical tensor
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#ifndef OPENTURNS_TENSOR_HXX
#define OPENTURNS_TENSOR_HXX

#include "TensorImplementation.hxx"
#include "TypedInterfaceObject.hxx"


BEGIN_NAMESPACE_OPENTURNS



class TensorImplementation;

/**
 * @class Tensor
 *
 * Tensor implements the classical mathematical Tensor
 */

class OT_API Tensor :
  public TypedInterfaceObject<TensorImplementation>

{
  CLASSNAME;

public:

  typedef TypedInterfaceObject<TensorImplementation>::Implementation Implementation;
  typedef Collection<NumericalScalar>       NumericalScalarCollection;

  /** Default constructor */
  Tensor();

  /** Constructor with size (rowDim, colDim, sheetDim) */
  Tensor(const UnsignedInteger rowDim,
         const UnsignedInteger colDim,
         const UnsignedInteger sheetDim);

  /** Constructor from range of external collection */
  template <class InputIterator>
  Tensor(const UnsignedInteger rowDim,
         const UnsignedInteger colDim,
         const UnsignedInteger sheetDim,
         InputIterator first,
         InputIterator last);

  /** Constructor from external collection */
  Tensor(const UnsignedInteger rowDim,
         const UnsignedInteger colDim,
         const UnsignedInteger sheetDim,
         const NumericalScalarCollection & elementsValues);

  /** Constructor with implementation */
  Tensor(const Implementation & i);

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

#ifndef SWIG
  /** Operator () gives access to the elements of the tensor (to modify these elements) */
  /** The element of the tensor is designated by its row number i, its column number j and its sheet number k */
  NumericalScalar & operator () (const UnsignedInteger i,
                                 const UnsignedInteger j,
                                 const UnsignedInteger k);

  /** Operator () gives access to the elements of the tensor (read only) */
  /** The element of the tensor is designated by its row number i, its column number j and its sheet number k */
  const NumericalScalar & operator () (const UnsignedInteger i,
                                       const UnsignedInteger j,
                                       const UnsignedInteger k) const;
#endif

  /** getSheet returns the sheet specified by its sheet number k */
  Matrix getSheet(const UnsignedInteger k) const;

  /** setSheet sets matrix m as the sheet specified by its sheet number k  */
  void setSheet(const UnsignedInteger k,
                const Matrix & m);

  /** Get the dimensions of the tensor */
  /** Number of rows */
  const UnsignedInteger getNbRows() const;
  /** Number of columns */
  const UnsignedInteger getNbColumns() const;
  /** Number of sheets */
  const UnsignedInteger getNbSheets() const;

  /** Comparison operators */
  Bool operator == (const Tensor & rhs) const;

  /** Empty returns true if there is no element in the tensor */
  const Bool isEmpty() const;

  // These functions are only intended to be used by SWIG, DO NOT use them for your own purpose !
  // INTENTIONALY NOT DOCUMENTED
  const NumericalScalar * __baseaddress__ () const;
  UnsignedInteger __elementsize__ () const;
  UnsignedInteger __stride__ (UnsignedInteger dim) const;

protected:

}; /* class Tensor */



/** Constructor with size (rowDim, colDim, sheetDim) */
template <class InputIterator>
Tensor::Tensor(const UnsignedInteger rowDim,
               const UnsignedInteger colDim,
               const UnsignedInteger sheetDim,
               InputIterator first,
               InputIterator last)
  : TypedInterfaceObject<TensorImplementation>(new TensorImplementation(rowDim, colDim, sheetDim, first, last))
{
  // nothing to do
}

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_MATRIX_HXX */
