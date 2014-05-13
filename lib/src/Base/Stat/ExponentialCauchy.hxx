//                                               -*- C++ -*-
/**
 *  @file  ExponentialCauchy.hxx
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
#ifndef OPENTURNS_EXPONENTIALCAUCHYMODEL_HXX
#define OPENTURNS_EXPONENTIALCAUCHYMODEL_HXX

#include "PersistentObject.hxx"
#include "SecondOrderModelImplementation.hxx"
#include "NumericalPoint.hxx"
#include "CorrelationMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS


class RegularGrid;

/**
 * @class ExponentialCauchy
 */

class OT_API ExponentialCauchy
  : public SecondOrderModelImplementation
{

  CLASSNAME;

public:


  /** Default constructor without parameters */
  ExponentialCauchy();

  /** Standard constructor with amplitude and scale parameters parameters */
  ExponentialCauchy(const NumericalPoint & amplitude,
                    const NumericalPoint & scale);

  /** Standard constructor with amplitude, scale and spatial correlation parameters parameters */
  ExponentialCauchy(const NumericalPoint & amplitude,
                    const NumericalPoint & scale,
                    const CorrelationMatrix & spatialCorrelation);

  /** Standard constructor with scale and spatial covariance parameters parameters */
  ExponentialCauchy(const NumericalPoint & scale,
                    const CovarianceMatrix & spatialCovariance);

  /** Virtual copy constructor */
  virtual ExponentialCauchy * clone() const;

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

  /** Amplitude accessor */
  NumericalPoint getAmplitude() const;

  /** Scale accessor */
  NumericalPoint getScale() const;

  /** Spatial correlation accessor */
  CorrelationMatrix getSpatialCorrelation() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private :

} ; /* class SecondOrderModelImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_EXPONENTIALCAUCHYMODEL_HXX */
