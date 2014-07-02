//                                               -*- C++ -*-
/**
 *  @file  SphericalModel.cxx
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
#include "SphericalModel.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class SphericalModel
 */

CLASSNAMEINIT(SphericalModel);

static Factory<SphericalModel> RegisteredFactory("SphericalModel");

/* Constructor with parameters */
SphericalModel::SphericalModel()
  : StationaryCovarianceModel()
  , amplitude_(1.0)
  , range_(1.0)
{
  dimension_ = 1;
}

SphericalModel::SphericalModel(const NumericalScalar amplitude,
                               const NumericalScalar range)
  : StationaryCovarianceModel()
  , amplitude_(0.0)
  , range_(0.0)
{
  dimension_ = 1;
  setAmplitude(amplitude);
  setRange(range);
}

/* Virtual constructor */
SphericalModel * SphericalModel::clone() const
{
  return new SphericalModel(*this);
}


/* Computation of the covariance function, stationary interface
 * C_{0,0}(tau) = amplitude_ * (1 - |tau|/(2a)(3 - |tau|^2/a^2)) for 0<=|tau<=a, 0 otherwise
 */
CovarianceMatrix SphericalModel::operator() (const NumericalPoint & tau) const
{
  CovarianceMatrix covarianceMatrix(dimension_);
  covarianceMatrix(0, 0) = computeAsScalar(tau);
  return covarianceMatrix;
}

NumericalScalar SphericalModel::computeAsScalar(const NumericalPoint & tau) const
{
  if (dimension_ != 1) throw NotDefinedException(HERE) << "Error: the covariance model is of dimension=" << dimension_ << ", expected dimension=1.";
  const NumericalScalar absTau(tau.norm());
  return (absTau >= range_ ? 0.0 : amplitude_ * (1.0 - 0.5 * absTau / range_ * (3.0 - absTau * absTau / (range_ * range_))));
}

/* Discretize the covariance function on a given TimeGrid */
CovarianceMatrix SphericalModel::discretize(const RegularGrid & timeGrid) const
{
  const UnsignedInteger size(timeGrid.getN());
  const UnsignedInteger fullSize(size);
  const NumericalScalar timeStep(timeGrid.getStep());

  CovarianceMatrix cov(fullSize);

  const CovarianceMatrix covTau0( operator()( 0.0 ) );

  for (UnsignedInteger diag = 0; diag < size; ++diag)
    {
      const NumericalScalar covTau( operator()( diag * timeStep )(0, 0) );
      for (UnsignedInteger i = 0; i < size - diag; ++i)
        cov( i, i + diag ) = covTau;
    }

  return cov;
}

/* Is it a stationary model ? */
Bool SphericalModel::isStationary() const
{
  return true;
}

/* String converter */
String SphericalModel::__repr__() const
{
  OSS oss(true);
  oss << "class=" << SphericalModel::GetClassName();
  oss << " amplitude=" << amplitude_
      << " range=" << range_;
  return oss;
}

/* String converter */
String SphericalModel::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "class=" << SphericalModel::GetClassName();
  oss << " amplitude=" << amplitude_
      << " range=" << range_;
  return oss;
}

/* Amplitude accessor */
NumericalScalar SphericalModel::getAmplitude() const
{
  return amplitude_;
}

void SphericalModel::setAmplitude(const NumericalScalar amplitude)
{
  if (amplitude <= 0.0) throw InvalidArgumentException(HERE) << "Error: the amplitude must be positive.";
  amplitude_ = amplitude;
}

/* Range accessor */
NumericalScalar SphericalModel::getRange() const
{
  return range_;
}

void SphericalModel::setRange(const NumericalScalar range)
{
  if (range <= 0.0) throw InvalidArgumentException(HERE) << "Error: the range must be positive.";
  range_ = range;
}

/* Method save() stores the object through the StorageManager */
void SphericalModel::save(Advocate & adv) const
{
  StationaryCovarianceModel::save(adv);
  adv.saveAttribute( "amplitude_", amplitude_);
  adv.saveAttribute( "range_", range_);
}

/* Method load() reloads the object from the StorageManager */
void SphericalModel::load(Advocate & adv)
{
  StationaryCovarianceModel::load(adv);
  adv.loadAttribute( "amplitude_", amplitude_);
  adv.loadAttribute( "range_", range_);
}

END_NAMESPACE_OPENTURNS
