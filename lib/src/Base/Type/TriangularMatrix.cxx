//                                               -*- C++ -*-
/**
 *  @file  TriangularMatrix.cxx
 *  @brief TriangularMatrix implements the classical mathematical triangluar matrix
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include "TriangularMatrix.hxx"
#include "Exception.hxx"
#include "Matrix.hxx"
#include "SquareMatrix.hxx"
#include "SymmetricMatrix.hxx"
#include "IdentityMatrix.hxx"
#include "HermitianMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(TriangularMatrix);

/* Default constructor */
TriangularMatrix::TriangularMatrix()
  : SquareMatrix(0),
    isTriangularLower_(true)
{
  // Nothing to do
}


/* Constructor with size (dim, which is the same for nbRows_ and nbColumns_ )*/
TriangularMatrix::TriangularMatrix(const UnsignedLong dimension,
                                   Bool isLower)
  : SquareMatrix(dimension),
    isTriangularLower_(isLower)
{

}


/* Constructor with implementation */
TriangularMatrix::TriangularMatrix(const Implementation & i,
                                   Bool isLower)

  : SquareMatrix(i),
    isTriangularLower_(isLower)
{
  // Nothing to do
}


/* String converter */
String TriangularMatrix::__repr__() const
{
  return OSS() << "class=" << getClassName()
         << " dimension=" << this->getDimension()
         << " implementation=" << getImplementation()->__repr__();
}

String TriangularMatrix::__str__(const String & offset) const
{
  return SquareMatrix::__str__();
}

/* Get the dimension of the matrix */
const UnsignedLong TriangularMatrix::getDimension() const
{
  return getImplementation()->getDimension();
}

/* TriangularMatrix transpose */
TriangularMatrix TriangularMatrix::transpose () const
{
  // Quick return for empty or scalar TriangularMatrix
  if (getDimension() <= 1) return (*this);
  return TriangularMatrix(Implementation(getImplementation()->transpose().clone()), !isTriangularLower_);
}


/* Check if the matrix is lower or upper */
Bool TriangularMatrix::isTriangularLower() const
{
  return isTriangularLower_;
}


/* Operator () gives access to the elements of the matrix (to modify these elements) */
/* The element of the matrix is designated by its row number i and its column number j */
/* the first element of the matrix is m(0,0) */
NumericalScalar & TriangularMatrix::operator() (const UnsignedLong i,
    const UnsignedLong j)
{
  if (isTriangularLower() && (i < j))
    throw InvalidArgumentException(HERE) << "Error; The triangular matrix is lower. "
                                         << "The indices are not valid" ;

  if (!isTriangularLower() && (i > j))
    throw InvalidArgumentException(HERE) << "Error; The triangular matrix is upper. "
                                         << "The indices are not valid" ;

  return  (*getImplementation())(i, j) ;
}

/* Operator () gives access to the elements of the matrix (read only) */
/* The element of the matrix is designated by its row number i and its column number j */
const NumericalScalar & TriangularMatrix::operator() (const UnsignedLong i,
    const UnsignedLong j)  const
{
  return (*getImplementation())(i, j) ;
}


/* TriangularMatrix additions */
SquareMatrix TriangularMatrix::operator+ (const TriangularMatrix & m) const
{
  return Implementation((*getImplementation() + * (m.getImplementation()) ).clone());
}

/* Matrix additions */
SquareMatrix TriangularMatrix::operator+ (const SquareMatrix & m) const
{
  return Implementation((*getImplementation() + * (m.getImplementation()) ).clone());
}

/* Matrix substractions */
SquareMatrix TriangularMatrix::operator- (const SquareMatrix & m) const
{
  return Implementation((*getImplementation() - * (m.getImplementation()) ).clone());
}

/* TriangularMatrix substractions */
SquareMatrix TriangularMatrix::operator- (const TriangularMatrix & m) const
{
  return Implementation((*getImplementation() - * (m.getImplementation()) ).clone());
}

/* Matrix multiplications */
SquareMatrix TriangularMatrix::operator * (const SquareMatrix & m) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return Implementation((getImplementation()->triangularProd(*(m.getImplementation()), 'L',  uplo ) ).clone());
}

/* Matrix multiplications */
Matrix TriangularMatrix::operator * (const Matrix & m) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return Implementation((getImplementation()->triangularProd(*(m.getImplementation()), 'L',  uplo ) ).clone());
}

/* TriangularMatrix multiplications */
SquareMatrix TriangularMatrix::operator * (const TriangularMatrix & m) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return Implementation((getImplementation()->triangularProd(*(m.getImplementation()), 'L', uplo ) ).clone());
}

/* SymmetricMatrix multiplications */
SquareMatrix TriangularMatrix::operator * (const SymmetricMatrix & m) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return Implementation((getImplementation()->triangularProd(*(m.getImplementation()), 'L', uplo ) ).clone());
}

/*  IdentityMatrix multiplications */
TriangularMatrix TriangularMatrix::operator * (const IdentityMatrix & m) const
{
  return (*this);
}

/* Multiplication with a NumericalScalarCollection (must have consistent dimensions) */
TriangularMatrix::NumericalScalarCollection TriangularMatrix::operator * (const NumericalScalarCollection & pt) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return getImplementation()->triangularVectProd(pt, uplo) ;
}

/* Multiplication with a NumericalPoint (must have consistent dimensions) */
TriangularMatrix::NumericalScalarCollection TriangularMatrix::operator * (const NumericalPoint & pt) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return getImplementation()->triangularVectProd(pt, uplo) ;
}

/* Multiplication with a Numerical */
TriangularMatrix TriangularMatrix::operator* (const NumericalScalar s) const
{
  return TriangularMatrix(Implementation((*getImplementation() * s ).clone()), isTriangularLower_);
}

/* Division by a Numerical*/
TriangularMatrix TriangularMatrix::operator / (const NumericalScalar s) const
{
  return TriangularMatrix(Implementation((*getImplementation() / s ).clone()), isTriangularLower_);
}


/* Resolution of a linear system */
NumericalPoint TriangularMatrix::solveLinearSystem (const NumericalPoint & b,
    const Bool keepIntact)
{
  return getImplementation()->solveLinearSystemTri(b, keepIntact, isTriangularLower_);
}

Matrix TriangularMatrix::solveLinearSystem (const Matrix & b,
    const Bool keepIntact)
{
  return Implementation(getImplementation()->solveLinearSystemTri(*b.getImplementation(), keepIntact, isTriangularLower_).clone());
}

END_NAMESPACE_OPENTURNS
