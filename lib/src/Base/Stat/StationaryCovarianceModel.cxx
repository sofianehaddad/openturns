//                                               -*- C++ -*-
/**
 *  @file  StationaryCovarianceModel.cxx
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
#include "StationaryCovarianceModel.hxx"
#include "Exception.hxx"
#include "Log.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class StationaryCovarianceModel
 */

CLASSNAMEINIT(StationaryCovarianceModel);

static Factory<StationaryCovarianceModel> RegisteredFactory("StationaryCovarianceModel");

/* Constructor without parameters */
StationaryCovarianceModel::StationaryCovarianceModel()
  : CovarianceModelImplementation(0)
{
  // Nothing to do
}

/* Virtual constructor */
StationaryCovarianceModel * StationaryCovarianceModel::clone() const
{
  return new StationaryCovarianceModel(*this);
}

/* Computation of the covariance function */
CovarianceMatrix StationaryCovarianceModel::operator() (const NumericalPoint & s,
    const NumericalPoint & t) const
{
  return (*this)(t - s);
}

CovarianceMatrix StationaryCovarianceModel::operator() (const NumericalPoint & tau) const
{
  throw NotYetImplementedException(HERE);
}

NumericalScalar StationaryCovarianceModel::computeAsScalar(const NumericalPoint & s,
    const NumericalPoint & t) const
{
  return computeAsScalar(t - s);
}

NumericalScalar StationaryCovarianceModel::computeAsScalar(const NumericalPoint & tau) const
{
  return (*this)(tau)(0, 0);
}

/* Discretize the covariance function on a given TimeGrid */
CovarianceMatrix StationaryCovarianceModel::discretize(const RegularGrid & timeGrid) const
{
  const UnsignedInteger size(timeGrid.getN());
  const NumericalScalar timeStep(timeGrid.getStep());
  const UnsignedInteger fullSize(size * dimension_);
  CovarianceMatrix covarianceMatrix(fullSize);

  // Fill-in the matrix by blocks
  for (UnsignedInteger rowIndex = 0; rowIndex < size; ++rowIndex)
  {
    // Only the lower part has to be filled-in
    for (UnsignedInteger columnIndex = 0; columnIndex < rowIndex; ++columnIndex)
    {
      const CovarianceMatrix localCovarianceMatrix(operator()( rowIndex * timeStep,  columnIndex * timeStep) );
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

/* Is the underlying a stationary covariance model ? */
Bool StationaryCovarianceModel::isStationary() const
{
  return true;
}

/* String converter */
String StationaryCovarianceModel::__repr__() const
{
  OSS oss(true);
  oss << "class= " << StationaryCovarianceModel::GetClassName();
  return oss;
}

/* String converter */
String StationaryCovarianceModel::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "class= " << StationaryCovarianceModel::GetClassName();
  return oss;
}

/* Method save() stores the object through the StorageManager */
void StationaryCovarianceModel::save(Advocate & adv) const
{
  CovarianceModelImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void StationaryCovarianceModel::load(Advocate & adv)
{
  CovarianceModelImplementation::load(adv);
}

END_NAMESPACE_OPENTURNS
