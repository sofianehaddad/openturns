//                                               -*- C++ -*-
/**
 *  @file  CovarianceModelImplementation.cxx
 *  @brief
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
#include "CovarianceModelImplementation.hxx"
#include "Exception.hxx"
#include "Log.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class CovarianceModelImplementation
 */

CLASSNAMEINIT(CovarianceModelImplementation);

static Factory<CovarianceModelImplementation> RegisteredFactory("CovarianceModelImplementation");

/* Constructor without parameters */
CovarianceModelImplementation::CovarianceModelImplementation(const String & name)
  : PersistentObject(name)
  , dimension_(0)
{
  // Nothing to do
}

/* Constructor with dimension parameter */
CovarianceModelImplementation::CovarianceModelImplementation(const UnsignedInteger dimension,
                                                             const String & name)
  : PersistentObject(name)
  , dimension_(dimension)
{
  // Nothing to do
}

/* Virtual constructor */
CovarianceModelImplementation * CovarianceModelImplementation::clone() const
{
  return new CovarianceModelImplementation(*this);
}

/* Dimension accessor */
UnsignedInteger CovarianceModelImplementation::getDimension() const
{
  return dimension_;
}

CovarianceMatrix CovarianceModelImplementation::operator() (const NumericalScalar s,
                                                            const NumericalScalar t) const
{
  return operator() (NumericalPoint(1, s), NumericalPoint(1, t));
}

CovarianceMatrix CovarianceModelImplementation::operator() (const NumericalPoint & s,
                                                            const NumericalPoint & t) const
{
  throw NotYetImplementedException(HERE);
}

NumericalScalar CovarianceModelImplementation::computeAsScalar (const NumericalPoint & s,
								const NumericalPoint & t) const
{
  if (dimension_ != 1) throw NotDefinedException(HERE) << "Error: the covariance model is of dimension=" << dimension_ << ", expected dimension=1.";
  return (*this)(s, t)(0, 0);
}

/* Computation of the covariance function */
CovarianceMatrix CovarianceModelImplementation::operator() (const NumericalScalar tau) const
{
  return operator() (NumericalPoint(1, tau));
}

CovarianceMatrix CovarianceModelImplementation::operator() (const NumericalPoint & tau) const
{
  return operator() (NumericalPoint(tau.getDimension()), tau);
}

NumericalScalar CovarianceModelImplementation::computeAsScalar (const NumericalPoint & tau) const
{
  if (dimension_ != 1) throw NotDefinedException(HERE) << "Error: the covariance model is of dimension=" << dimension_ << ", expected dimension=1.";
  return (*this)(tau)(0, 0);
}

/* Gradient */
SymmetricTensor CovarianceModelImplementation::partialGradient (const NumericalPoint & s,
                                                                const NumericalPoint & t) const
{
  throw NotYetImplementedException(HERE);
}

/* Discretize the covariance function on a given TimeGrid/Mesh */
CovarianceMatrix CovarianceModelImplementation::discretize(const RegularGrid & timeGrid) const
{
  return discretize(Mesh(timeGrid));
}

CovarianceMatrix CovarianceModelImplementation::discretize(const Mesh & mesh) const
{
  const NumericalSample vertices(mesh.getVertices());
  const UnsignedInteger size(vertices.getSize());
  const UnsignedInteger fullSize(size * dimension_);
  CovarianceMatrix covarianceMatrix(fullSize);

  // Fill-in the matrix by blocks
  for (UnsignedInteger rowIndex = 0; rowIndex < size; ++rowIndex)
    {
      // Only the lower part has to be filled-in
      for (UnsignedInteger columnIndex = 0; columnIndex <= rowIndex; ++columnIndex)
        {
          const CovarianceMatrix localCovarianceMatrix(operator()( vertices[rowIndex],  vertices[columnIndex] ));
          // We fill the covariance matrix using the previous local one
          // The full local covariance matrix has to be copied as it is
          // not copied on a symmetric position
          for (UnsignedInteger rowIndexLocal = 0; rowIndexLocal < dimension_; ++rowIndexLocal)
            {
              for (UnsignedInteger columnIndexLocal = 0; columnIndexLocal < dimension_; ++columnIndexLocal)
                {
                  covarianceMatrix(columnIndex + columnIndexLocal * size, rowIndex + rowIndexLocal * size ) = localCovarianceMatrix(rowIndexLocal, columnIndexLocal) ;
                } // column index within the block
            } // row index within the block
        } // column index of the block
    } // row index of the block
  return covarianceMatrix;
}

NumericalSample CovarianceModelImplementation::discretizeRow(const NumericalSample & vertices,
							     const UnsignedInteger p) const
{
  if (dimension_ != 1) throw InternalException(HERE) << "Error: the discretizeRow() method is not defined if dimension is not 1. Here, dimension=" << dimension_;
  const UnsignedInteger size(vertices.getSize());
  NumericalSample result(size, 1);
  const CovarianceModelImplementation::DiscretizePolicy policy( vertices, p, result, *this );
  TBB::ParallelFor( 0, size, policy );
  return result;
}

/* Partial discretization with respect to the second argument */
Basis CovarianceModelImplementation::getPartialDiscretization(const NumericalSample & secondLocation) const
{
  throw NotYetImplementedException(HERE);
}

/* Is it a stationary model ? */
Bool CovarianceModelImplementation::isStationary() const
{
  return false;
}

/* String converter */
String CovarianceModelImplementation::__repr__() const
{
  OSS oss(true);
  oss << "class= " << CovarianceModelImplementation::GetClassName();
  return oss;
}

/* String converter */
String CovarianceModelImplementation::__str__(const String & offset) const
{
  OSS oss(false);
  oss << offset << "class= " << CovarianceModelImplementation::GetClassName();
  return oss;
}

void CovarianceModelImplementation::setParameters(const NumericalPointWithDescription& parameters)
{
  throw NotYetImplementedException(HERE);
}

NumericalPointWithDescription CovarianceModelImplementation::getParameters() const
{
  throw NotYetImplementedException(HERE);
}

/* Method save() stores the object through the StorageManager */
void CovarianceModelImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);

  adv.saveAttribute("dimension_", dimension_);
}

/* Method load() reloads the object from the StorageManager */
void CovarianceModelImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);

  adv.loadAttribute("dimension_", dimension_);

}

END_NAMESPACE_OPENTURNS
