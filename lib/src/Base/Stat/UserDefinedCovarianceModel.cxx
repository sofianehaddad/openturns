//                                               -*- C++ -*-
/**
 *  @file  UserDefinedCovarianceModel.cxx
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
#include "UserDefinedCovarianceModel.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

TEMPLATE_CLASSNAMEINIT(PersistentCollection< CovarianceMatrix >);
static Factory< PersistentCollection< CovarianceMatrix > > RegisteredFactory1("PersistentCollection< CovarianceMatrix >");

CLASSNAMEINIT(UserDefinedCovarianceModel);

static Factory<UserDefinedCovarianceModel> RegisteredFactory("UserDefinedCovarianceModel");

/* Constructor with parameters */
UserDefinedCovarianceModel::UserDefinedCovarianceModel(const String & name)
  : CovarianceModelImplementation(name)
  , covarianceCollection_(0)
  , p_mesh_(RegularGrid().clone())
{
  dimension_ = 0;
}

// For a non stationary model, we need N x N covariance functions with N the number of vertices in the mesh
UserDefinedCovarianceModel::UserDefinedCovarianceModel(const Mesh & mesh,
    const CovarianceMatrixCollection & covarianceFunction,
    const String & name)
  : CovarianceModelImplementation(name)
  , covarianceCollection_(0)
  , p_mesh_(0)
{
  const UnsignedInteger N(mesh.getVerticesNumber());
  const UnsignedInteger size((N * (N + 1)) / 2);
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: the mesh is empty.";
  if (size != covarianceFunction.getSize())
    throw InvalidArgumentException(HERE) << "Error: for a non stationary covariance model, sizes are incoherent:"
                                         << " mesh size=" << N << " and covariance function size=" << covarianceFunction.getSize() << " instead of " << size;
  p_mesh_ = mesh.clone();

  covarianceCollection_ = CovarianceMatrixCollection(size);
  // put the first element
  covarianceCollection_[0] = covarianceFunction[0];
  dimension_ = covarianceCollection_[0].getDimension();
  // put the next elements if dimension is ok
  for (UnsignedInteger k = 1; k < size; ++k)
  {
    if (covarianceFunction[k].getDimension() != dimension_)
      throw InvalidArgumentException(HERE) << " Error with dimension; all the covariance matrices must have the same dimension";
    covarianceCollection_[k] = covarianceFunction[k];
  }
}

/* Virtual constructor */
UserDefinedCovarianceModel * UserDefinedCovarianceModel::clone() const
{
  return new UserDefinedCovarianceModel(*this);
}


/* Computation of the covariance density function */
CovarianceMatrix UserDefinedCovarianceModel::operator() (const NumericalPoint & s,
    const NumericalPoint & t) const
{
  // If the grid size is one, return the covariance function
  // else find in the grid the nearest instant values
  const UnsignedInteger N(p_mesh_->getVerticesNumber());
  if (N == 1) return covarianceCollection_[0];

  // We look for the two vertices of the mesh the nearest to s and t resp.
  UnsignedInteger sIndex(p_mesh_->getNearestVertexIndex(s));
  UnsignedInteger tIndex(p_mesh_->getNearestVertexIndex(t));
  // The covariance matrices correspond to sIndex >= tIndex.
  // As C(s, t) = C(t, s), we swap sIndex and tIndex if sIndex < tIndex
  if (sIndex < tIndex) std::swap(sIndex, tIndex);
  // The covariances are stored the following way:
  // sIndex=0, tIndex=0 -> index=0
  // sIndex=1, tIndex=0 -> index=1
  // sIndex=1, tIndex=1 -> index=2
  // sIndex=2, tIndex=0 -> index=3
  // sIndex=2, tIndex=1 -> index=4
  // sIndex=2, tIndex=2 -> index=5
  // ie index = tIndex + sIndex * (sIndex + 1) / 2
  const SignedInteger index(tIndex + (sIndex * (sIndex + 1)) / 2);
  return covarianceCollection_[index];
}

/* Mesh accessor */
Mesh UserDefinedCovarianceModel::getMesh() const
{
  return *p_mesh_;
}

/* Time grid accessor */
RegularGrid UserDefinedCovarianceModel::getTimeGrid() const
{
  return RegularGrid(*p_mesh_);
}

/* String converter */
String UserDefinedCovarianceModel::__repr__() const
{
  OSS oss(true);
  oss << "class=" << UserDefinedCovarianceModel::GetClassName()
      << " mesh=" << p_mesh_->__repr__()
      << " dimension=" << dimension_
      << " covarianceCollection=" << covarianceCollection_;
  return oss;

}

/* String converter */
String UserDefinedCovarianceModel::__str__(const String & offset) const
{
  return __repr__();
}

/* Method save() stores the object through the StorageManager */
void UserDefinedCovarianceModel::save(Advocate & adv) const
{
  CovarianceModelImplementation::save(adv);
  adv.saveAttribute( "mesh_", *p_mesh_);
  adv.saveAttribute( "covarianceCollection_", covarianceCollection_);
}

/* Method load() reloads the object from the StorageManager */
void UserDefinedCovarianceModel::load(Advocate & adv)
{
  TypedInterfaceObject<Mesh> mesh;
  CovarianceModelImplementation::load(adv);
  adv.loadAttribute( "mesh_", mesh);
  p_mesh_ = mesh.getImplementation();
  adv.loadAttribute( "covarianceCollection_", covarianceCollection_);
}

END_NAMESPACE_OPENTURNS
