//                                               -*- C++ -*-
/**
 *  @file  TriangularMatrix.hxx
 *  @brief TriangularMatrix implements the classical mathematical triangular matrix with values
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
#ifndef OPENTURNS_TRIANGULARMATRIX_HXX
#define OPENTURNS_TRIANGULARMATRIX_HXX

#include "SquareMatrix.hxx"
#include "Collection.hxx"

BEGIN_NAMESPACE_OPENTURNS

class SymmetricMatrix;
class IdentityMatrix;
class SquareMatrix;
class Matrix;
class HermitianMatrix;

/**
 * @class TriangularMatrix
 *
 * TriangularMatrix implements the classical mathematical triangular matrix with scalar values
 * Default implementation is triangular lower
 */

class TriangularMatrix :
  public SquareMatrix
{
  CLASSNAME;

#ifndef SWIG
  friend TriangularMatrix operator * (const NumericalScalar s,
                                      const TriangularMatrix & m);
#endif

public:


  /** Default constructor */
  TriangularMatrix();

  /** Constructor with size (dim, which is the same for nbRows_ and nbColumns_) */
  explicit TriangularMatrix(const UnsignedLong dimension,
                            Bool isLower = true);

  /** Constructor with implementation */
  TriangularMatrix(const Implementation & i,
                   const Bool isLower = true);

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Get the dimension of the matrix */
  const UnsignedLong getDimension() const;

  /** TriangularMatrix transpose */
  TriangularMatrix transpose () const;

  /** Check if the matrix is lower or upper */
  Bool isTriangularLower() const;

#ifndef SWIG
  /** Operator () gives access to the elements of the matrix (to modify these elements) */
  /** The element of the matrix is designated by its row number i and its column number j */
  NumericalScalar & operator ()(const UnsignedLong i,
                                const UnsignedLong j) ;

  /** Operator () gives access to the elements of the matrix (read only) */
  /** The element of the matrix is designated by its row number i and its column number j */
  const NumericalScalar & operator ()(const UnsignedLong i,
                                      const UnsignedLong j) const;
#endif


  /** TriangularMatrix additions : result is a square matrix */
  SquareMatrix operator + (const TriangularMatrix & m) const;

  /** Addition operator */
  SquareMatrix operator + (const SquareMatrix & m) const;

  /** Substraction operator with TriangularMatrix */
  SquareMatrix operator - (const TriangularMatrix & m) const;

  /** Substraction operator */
  SquareMatrix operator - (const SquareMatrix & m) const;

  /** Multiplication with a scalar */
  TriangularMatrix operator * (const NumericalScalar s) const ;

  /** Matrix multiplications */
  Matrix operator * (const Matrix & m) const;

  /** SquareMatrix multiplications */
  SquareMatrix operator * (const SquareMatrix & m) const;

  /** TriangularMatrix multiplications  */
  SquareMatrix operator * (const TriangularMatrix & m) const;

  /** Real SymmetricMatrix multiplications  */
  SquareMatrix operator * (const SymmetricMatrix & m) const;

  /** IdentityMatrix multiplications  */
  TriangularMatrix operator * (const IdentityMatrix & m) const;

  /** Multiplication with a NumericalScalarCollection (must have consistent dimensions) */
  NumericalScalarCollection operator * (const NumericalScalarCollection & p) const;

  /** Multiplication with a NumericalPoint (must have consistent dimensions) */
  NumericalScalarCollection operator * (const NumericalPoint & p) const;

  /** Division by a NumericalScalar */
  TriangularMatrix operator / (const NumericalScalar s) const;

  /** Resolution of a linear system */
  NumericalPoint solveLinearSystem(const NumericalPoint & b,
                                   const Bool keepIntact = true);

  Matrix solveLinearSystem(const Matrix & b,
                           const Bool keepIntact = true);
private:

  /** Boolean information : is the matrix triangular lower or upper? */
  mutable Bool isTriangularLower_;

}; /* class TriangularMatrix */


inline TriangularMatrix operator * (const NumericalScalar s,
                                    const TriangularMatrix & m)
{
  return m.operator * (s);
}


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_TRIANGULARMATRIX_HXX */
