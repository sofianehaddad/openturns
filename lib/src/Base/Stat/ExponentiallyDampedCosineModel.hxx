//                                               -*- C++ -*-
/**
 *  @file  ExponentiallyDampedCosineModel.hxx
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
#ifndef OPENTURNS_EXPONENTIALLYDAMPEDCOSINEMODEL_HXX
#define OPENTURNS_EXPONENTIALLYDAMPEDCOSINEMODEL_HXX

#include "PersistentObject.hxx"
#include "StationaryCovarianceModel.hxx"
#include "NumericalPoint.hxx"
#include "CorrelationMatrix.hxx"
#include "Mesh.hxx"

BEGIN_NAMESPACE_OPENTURNS

class RegularGrid;

/**
 * @class ExponentiallyDampedCosineModel
 */

class OT_API ExponentiallyDampedCosineModel
  : public StationaryCovarianceModel
{

  CLASSNAME;

public:


  /** Default constructor without parameters */
  ExponentiallyDampedCosineModel();

  /** Standard constructor with amplitude and scale parameters parameters */
  ExponentiallyDampedCosineModel(const NumericalScalar amplitude,
                                 const NumericalScalar scale,
                                 const NumericalScalar frequency);

  /** Virtual copy constructor */
  virtual ExponentiallyDampedCosineModel * clone() const;

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

protected:

  void setAmplitude(const NumericalScalar amplitude);

public:

  /** Scale accessor */
  NumericalScalar getScale() const;

protected:

  void setScale(const NumericalScalar scale);

public:

  /** Frequency accessor */
  NumericalScalar getFrequency() const;

protected:

  void setFrequency(const NumericalScalar frequency);

public:

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private :

  /** Amplitude  */
  NumericalScalar amplitude_;

  /** Scale */
  NumericalScalar scale_;

  /** Frequency */
  NumericalScalar frequency_;

} ; /* class ExponentiallyDampedCosineModel */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_EXPONENTIALLYDAMPEDCOSINEMODEL_HXX */
