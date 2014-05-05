//                                               -*- C++ -*-
/**
 *  @file  UserDefinedStationaryCovarianceModel.hxx
 *  @brief This class build a stationary covariance model using a time grid and a collection of covariance matrices
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
 *  @author: $LastChangedBy: schueller $
 *  @date:   $LastChangedDate: 2012-02-17 19:35:43 +0100 (ven. 17 févr. 2012) $
 *  Id:      $Id: UserDefinedStationaryCovarianceModel.hxx 2392 2012-02-17 18:35:43Z schueller $
 */
#ifndef OPENTURNS_USERDEFINEDSTATIONARYCOVARIANCEMODEL_HXX
#define OPENTURNS_USERDEFINEDSTATIONARYCOVARIANCEMODEL_HXX

#include "StationaryCovarianceModel.hxx"
#include "PersistentCollection.hxx"
#include "Collection.hxx"
#include "RegularGrid.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class UserDefinedStationaryCovarianceModel
 */

class UserDefinedStationaryCovarianceModel
  : public StationaryCovarianceModel
{

  CLASSNAME;

public:

  typedef PersistentCollection<CovarianceMatrix>          CovarianceMatrixPersistentCollection;
  typedef Collection<CovarianceMatrix>                    CovarianceMatrixCollection;

  /** Default onstructor */
  UserDefinedStationaryCovarianceModel(const String & name = OT::DefaultName);

  /** Standard onstructor */
  UserDefinedStationaryCovarianceModel(const Mesh & mesh,
                                       const CovarianceMatrixCollection & covarianceCollection,
                                       const String & name = OT::DefaultName);

  /** Virtual copy constructor */
  virtual UserDefinedStationaryCovarianceModel * clone() const;

  /** Computation of the covariance function */
  using StationaryCovarianceModel::operator();
  CovarianceMatrix operator() (const NumericalPoint & tau) const;

  /** Time grid/mesh accessor */
  Mesh getMesh() const;
  RegularGrid getTimeGrid() const;

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  /** Collection of covariance functions */
  CovarianceMatrixPersistentCollection covarianceCollection_;

  /** Mesh of evaluation */
  Mesh mesh_;

} ; /* class UserDefinedStationaryCovarianceModel */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_USERDEFINEDSTATIONARYCOVARIANCEMODEL_HXX */
