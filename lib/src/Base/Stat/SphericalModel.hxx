//                                               -*- C++ -*-
/**
 *  @file  SphericalModel.hxx
 *  @brief This class is enables to build an exponential covariance
 *  model, a second order model's implementation
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
#ifndef OPENTURNS_SPHERICALMODEL_HXX
#define OPENTURNS_SPHERICALMODEL_HXX

#include "PersistentObject.hxx"
#include "StationaryCovarianceModel.hxx"
#include "NumericalPoint.hxx"
#include "CorrelationMatrix.hxx"
#include "Mesh.hxx"

BEGIN_NAMESPACE_OPENTURNS

class RegularGrid;

/**
 * @class SphericalModel
 */

class SphericalModel
  : public StationaryCovarianceModel
{

  CLASSNAME;

public:


  /** Default constructor without parameters */
  SphericalModel();

  /** Standard constructor with amplitude and range parameters parameters */
  SphericalModel(const NumericalScalar amplitude,
                 const NumericalScalar range);

  /** Virtual copy constructor */
  virtual SphericalModel * clone() const;

  /** Computation of the covariance function, stationary interface */
  using StationaryCovarianceModel::operator();
  CovarianceMatrix operator() (const NumericalPoint & tau) const;
  NumericalScalar computeAsScalar(const NumericalPoint & tau) const;

  /** Discretize the covariance function on a given TimeGrid */
  using StationaryCovarianceModel::discretize;
  CovarianceMatrix discretize(const RegularGrid & regularGrid) const;

  /** Is it a stationary model ? */
  Bool isStationary() const;

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

  /** Amplitude accessor */
  NumericalScalar getAmplitude() const;

  void setAmplitude(const NumericalScalar amplitude);

  /** Range accessor */
  NumericalScalar getRange() const;

  void setRange(const NumericalScalar range);

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private :

  /** Amplitude  */
  NumericalScalar amplitude_;

  /** Range */
  NumericalScalar range_;

} ; /* class SphericalModel */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SPHERICALMODEL_HXX */
