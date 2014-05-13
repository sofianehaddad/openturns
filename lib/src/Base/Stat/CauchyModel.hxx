//                                               -*- C++ -*-
/**
 *  @file  CauchyModel.hxx
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
#ifndef OPENTURNS_CAUCHYMODEL_HXX
#define OPENTURNS_CAUCHYMODEL_HXX

#include "PersistentObject.hxx"
#include "SpectralModel.hxx"
#include "NumericalPoint.hxx"
#include "CorrelationMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS

class TimeGrid;

/**
 * @class CauchyModel
 */

class OT_API CauchyModel
  : public SpectralModelImplementation
{

  CLASSNAME;

public:


  /** Default constructor without parameters */
  CauchyModel();

  /** Standard constructor with amplitude and scale parameters parameters */
  CauchyModel(const NumericalPoint & amplitude,
              const NumericalPoint & scale);

  /** Standard constructor with amplitude, scale and spatial correlation parameters parameters */
  CauchyModel(const NumericalPoint & amplitude,
              const NumericalPoint & scale,
              const CorrelationMatrix & spatialCorrelation);

  /** Standard constructor with scale and spatial covariance parameters parameters */
  CauchyModel(const NumericalPoint & scale,
              const CovarianceMatrix & spatialCovariance);

  /** Virtual copy constructor */
  virtual CauchyModel * clone() const;

  /** Computation of the spectral density function */
  HermitianMatrix operator()(const NumericalScalar frequency) const;

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

  /** Amplitude accessor */
  NumericalPoint getAmplitude() const;

protected:

  void setAmplitude(const NumericalPoint & amplitude);

public:

  /** Scale accessor */
  NumericalPoint getScale() const;

protected:

  void setScale(const NumericalPoint & scale);

public:

  /** Spatial correlation accessor */
  CorrelationMatrix getSpatialCorrelation() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private :

  /** Collection - Container for amplitude values  */
  NumericalPoint amplitude_;

  /** Collection - Container for scale values  */
  NumericalPoint scale_;

  /** Correlation matrix of the spatial dependence structure */
  CorrelationMatrix spatialCorrelation_;

  /** Flag to tell if the model is diagonal */
  Bool isDiagonal_;

} ; /* class CauchyModel */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_CAUCHYMODEL_HXX */
