//                                               -*- C++ -*-
/**
 *  @file  UserDefinedStationaryCovarianceModel.cxx
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
 *  @author: $LastChangedBy$
 *  @date:   $LastChangedDate$
 *  Id:      $Id$
 */
#include "UserDefinedStationaryCovarianceModel.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

// TEMPLATE_CLASSNAMEINIT(PersistentCollection< CovarianceMatrix >);
// static Factory< PersistentCollection< CovarianceMatrix > > RegisteredFactory1("PersistentCollection< CovarianceMatrix >");

CLASSNAMEINIT(UserDefinedStationaryCovarianceModel);

static Factory<UserDefinedStationaryCovarianceModel> RegisteredFactory("UserDefinedStationaryCovarianceModel");

/* Constructor with parameters */
UserDefinedStationaryCovarianceModel::UserDefinedStationaryCovarianceModel()
  : StationaryCovarianceModel()
  , covarianceCollection_(0)
{
  dimension_ = 0;
}

// Classical constructor
// For a stationary model, we need N covariance matrices with N the number of time stamps in the time grid
UserDefinedStationaryCovarianceModel::UserDefinedStationaryCovarianceModel(const Mesh & mesh,
    const CovarianceMatrixCollection & covarianceFunction)
  : StationaryCovarianceModel()
  , covarianceCollection_(0)
{
  const UnsignedInteger size(mesh.getVerticesNumber());
  if (size != covarianceFunction.getSize())
    throw InvalidArgumentException(HERE) << "Error: for a non stationary covariance model, sizes are incoherents"
                                         << " mesh size = " << size << "covariance function size = " << covarianceFunction.getSize();
  mesh_ = mesh;

  covarianceCollection_ = CovarianceMatrixCollection(size);
  // put the first element
  covarianceCollection_[0] = covarianceFunction[0];
  dimension_ = covarianceCollection_[0].getDimension();
  // put the next elements if dimension is ok
  for (UnsignedInteger k = 1; k < size; ++k)
  {
    if (covarianceFunction[k].getDimension() != dimension_)
      throw InvalidArgumentException(HERE) << " Error with dimension; the covariance matrices should be of same dimension";
    covarianceCollection_[k] = covarianceFunction[k];
  }
}

/* Virtual constructor */
UserDefinedStationaryCovarianceModel * UserDefinedStationaryCovarianceModel::clone() const
{
  return new UserDefinedStationaryCovarianceModel(*this);
}

/* Computation of the covariance function */
CovarianceMatrix UserDefinedStationaryCovarianceModel::operator()(const NumericalPoint & t) const
{
  if (t.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: expect a shift of dimension=" << dimension_ << ", here dimension=" << t.getDimension();
  // If the grid size is one, return the covariance function
  // else find in the grid the nearest instant values
  if (mesh_.getVerticesNumber() == 1) return covarianceCollection_[0];

  return covarianceCollection_[mesh_.getNearestVertexIndex(t)];
}

/* Time grid/mesh accessor */
Mesh UserDefinedStationaryCovarianceModel::getMesh() const
{
  return mesh_;
}

/* Mesh accessor */
RegularGrid UserDefinedStationaryCovarianceModel::getTimeGrid() const
{
  return RegularGrid(mesh_);
}

/* String converter */
String UserDefinedStationaryCovarianceModel::__repr__() const
{
  OSS oss(true);
  oss << "class=" << UserDefinedStationaryCovarianceModel::GetClassName()
      << " mesh=" << mesh_
      << " dimension=" << dimension_
      << " covarianceCollection=" << covarianceCollection_;
  return oss;

}

/* String converter */
String UserDefinedStationaryCovarianceModel::__str__(const String & offset) const
{
  return __repr__();
}

/* Method save() stores the object through the StorageManager */
void UserDefinedStationaryCovarianceModel::save(Advocate & adv) const
{
  StationaryCovarianceModel::save(adv);
  adv.saveAttribute( "mesh_", mesh_);
  adv.saveAttribute( "covarianceCollection_", covarianceCollection_);
}

/* Method load() reloads the object from the StorageManager */
void UserDefinedStationaryCovarianceModel::load(Advocate & adv)
{
  StationaryCovarianceModel::load(adv);
  adv.loadAttribute( "mesh_", mesh_);
  adv.loadAttribute( "covarianceCollection_", covarianceCollection_);
}

END_NAMESPACE_OPENTURNS
