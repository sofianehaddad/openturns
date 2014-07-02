//                                               -*- C++ -*-
/**
 *  @file  ExponentiallyDampedCosineModel.cxx
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
#include "ExponentiallyDampedCosineModel.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ExponentiallyDampedCosineModel
 */

CLASSNAMEINIT(ExponentiallyDampedCosineModel);

static Factory<ExponentiallyDampedCosineModel> RegisteredFactory("ExponentiallyDampedCosineModel");

/* Constructor with parameters */
ExponentiallyDampedCosineModel::ExponentiallyDampedCosineModel()
  : StationaryCovarianceModel()
  , amplitude_(1.0)
  , scale_(1.0)
  , frequency_(1.0)
{
  dimension_ = 1;
}

ExponentiallyDampedCosineModel::ExponentiallyDampedCosineModel(const NumericalScalar amplitude,
    const NumericalScalar frequency,
    const NumericalScalar scale)
  : StationaryCovarianceModel()
  , amplitude_(0.0)
  , scale_(0.0)
  , frequency_(0.0)
{
  dimension_ = 1;
  setAmplitude(amplitude);
  setScale(scale);
  setFrequency(frequency);
}

/* Virtual constructor */
ExponentiallyDampedCosineModel * ExponentiallyDampedCosineModel::clone() const
{
  return new ExponentiallyDampedCosineModel(*this);
}


/* Computation of the covariance function, stationary interface
 * C_{0,0}(tau) = amplitude_ * exp(-|tau| / scale_) * cos(2 * pi * frequency_ * |tau|)
 */
CovarianceMatrix ExponentiallyDampedCosineModel::operator() (const NumericalPoint & tau) const
{
  CovarianceMatrix covarianceMatrix(dimension_);
  covarianceMatrix(0, 0) = computeAsScalar(tau);
  return covarianceMatrix;
}

NumericalScalar ExponentiallyDampedCosineModel::computeAsScalar(const NumericalPoint & tau) const
{
  if (dimension_ != 1) throw NotDefinedException(HERE) << "Error: the covariance model is of dimension=" << dimension_ << ", expected dimension=1.";
  const NumericalScalar absTau(tau.norm());
  return amplitude_ * exp(-absTau / scale_) * cos(2.0 * M_PI * absTau);
}

/* Discretize the covariance function on a given TimeGrid */
CovarianceMatrix ExponentiallyDampedCosineModel::discretize(const RegularGrid & timeGrid) const
{
  const UnsignedInteger size(timeGrid.getN());
  const UnsignedInteger fullSize(size * dimension_);
  const NumericalScalar timeStep(timeGrid.getStep());

  CovarianceMatrix cov(fullSize);

  // The stationary property of this model allows to optimize the discretization
  // over a regular time grid: the large covariance matrix is block-diagonal
  // Fill the matrix by block-diagonal
  // The main diagonal has a specific treatment as only its lower triangular part
  // has to be copied
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
Bool ExponentiallyDampedCosineModel::isStationary() const
{
  return true;
}

/* String converter */
String ExponentiallyDampedCosineModel::__repr__() const
{
  OSS oss(true);
  oss << "class=" << ExponentiallyDampedCosineModel::GetClassName();
  oss << " amplitude=" << amplitude_
      << " scale=" << scale_
      << " frequency=" << frequency_;
  return oss;
}

/* String converter */
String ExponentiallyDampedCosineModel::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "class=" << ExponentiallyDampedCosineModel::GetClassName();
  oss << " amplitude=" << amplitude_
      << " scale=" << scale_
      << " frequency=" << frequency_;
  return oss;
}

/* Amplitude accessor */
NumericalScalar ExponentiallyDampedCosineModel::getAmplitude() const
{
  return amplitude_;
}

void ExponentiallyDampedCosineModel::setAmplitude(const NumericalScalar amplitude)
{
  if (amplitude <= 0.0) throw InvalidArgumentException(HERE) << "Error: the amplitude must be positive.";
  amplitude_ = amplitude;
}

/* Scale accessor */
NumericalScalar ExponentiallyDampedCosineModel::getScale() const
{
  return scale_;
}

void ExponentiallyDampedCosineModel::setScale(const NumericalScalar scale)
{
  if (scale <= 0.0) throw InvalidArgumentException(HERE) << "Error: the scale must be positive.";
  scale_ = scale;
}

/* Frequency accessor */
NumericalScalar ExponentiallyDampedCosineModel::getFrequency() const
{
  return frequency_;
}

void ExponentiallyDampedCosineModel::setFrequency(const NumericalScalar frequency)
{
  if (frequency <= 0.0) throw InvalidArgumentException(HERE) << "Error: the frequency must be positive.";
  frequency_ = frequency;
}

/* Method save() stores the object through the StorageManager */
void ExponentiallyDampedCosineModel::save(Advocate & adv) const
{
  StationaryCovarianceModel::save(adv);
  adv.saveAttribute( "amplitude_", amplitude_);
  adv.saveAttribute( "scale_", scale_);
  adv.saveAttribute( "frequency_", frequency_);
}

/* Method load() reloads the object from the StorageManager */
void ExponentiallyDampedCosineModel::load(Advocate & adv)
{
  StationaryCovarianceModel::load(adv);
  adv.loadAttribute( "amplitude_", amplitude_);
  adv.loadAttribute( "scale_", scale_);
  adv.loadAttribute( "frequency_", frequency_);
}

END_NAMESPACE_OPENTURNS
