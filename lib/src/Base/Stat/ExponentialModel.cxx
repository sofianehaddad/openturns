//                                               -*- C++ -*-
/**
 *  @file  ExponentialModel.cxx
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
#include "ExponentialModel.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ExponentialModel
 */

CLASSNAMEINIT(ExponentialModel);

static Factory<ExponentialModel> RegisteredFactory("ExponentialModel");

/* Constructor with parameters */
ExponentialModel::ExponentialModel()
  : StationaryCovarianceModel()
  , amplitude_(NumericalPoint(1, 1.0))
  , scale_(NumericalPoint(1, 1.0))
  , spatialCorrelation_(0)
  , isDiagonal_(true)
{
  dimension_ = 1;
}

ExponentialModel::ExponentialModel(const NumericalPoint & amplitude,
                                   const NumericalPoint & scale)
  : StationaryCovarianceModel()
  , amplitude_(0)
  , scale_(0)
  , spatialCorrelation_(0)
  , isDiagonal_(true)
{
  if (amplitude.getDimension() != scale.getDimension() )
    throw InvalidArgumentException(HERE) << "Incompatible dimensions between amplitude and scale values";
  setAmplitude(amplitude);
  setScale(scale);
  dimension_ = amplitude.getDimension();
}

ExponentialModel::ExponentialModel(const NumericalPoint & amplitude,
                                   const NumericalPoint & scale,
                                   const CorrelationMatrix & spatialCorrelation)
  : StationaryCovarianceModel()
  , amplitude_(0)
  , scale_(0)
  , spatialCorrelation_(0)
  , isDiagonal_(false)
{
  dimension_ = amplitude.getDimension();
  if (scale.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Incompatible dimensions between amplitude and scale values";
  if (spatialCorrelation.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given spatial correlation has a dimension different from the scales and amplitudes.";
  setAmplitude(amplitude);
  setScale(scale);
  isDiagonal_ = spatialCorrelation.isDiagonal();
  if (!isDiagonal_) spatialCorrelation_ = spatialCorrelation;
}

ExponentialModel::ExponentialModel(const NumericalPoint & scale,
                                   const CovarianceMatrix & spatialCovariance)
  : StationaryCovarianceModel()
  , amplitude_(0)
  , scale_(scale)
  , spatialCorrelation_(0)
  , isDiagonal_(false)
{
  dimension_ = scale.getDimension();
  if (spatialCovariance.getDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the given spatial covariance has a dimension different from the scales and amplitudes.";
  setScale(scale);
  NumericalPoint amplitude(dimension_);
  for (UnsignedInteger i = 0; i < dimension_; ++i)
    amplitude[i] = sqrt(spatialCovariance(i, i));
  // Check that the amplitudes are valid
  setAmplitude(amplitude);
  // Convert the spatial covariance into a spatial correlation
  isDiagonal_ = spatialCovariance.isDiagonal();
  if (!isDiagonal_)
  {
    spatialCorrelation_ = CorrelationMatrix(dimension_);
    for (UnsignedInteger i = 0; i < dimension_; ++i)
      for (UnsignedInteger j = 0; j < i; ++j)
        spatialCorrelation_(i, j) = spatialCovariance(i, j) / (amplitude[i] * amplitude[j]);
  } // !isDiagonal
}

/* Virtual constructor */
ExponentialModel * ExponentialModel::clone() const
{
  return new ExponentialModel(*this);
}


/* Computation of the covariance function, stationary interface
 * C_{i,j}(tau) = amplitude_i * exp(-|tau| / scale_i) * R_{i,j} * amplitude_j * exp(-|tau| / scale_j)
 * C_{i,i}(tau) = amplitude_i^2 * exp(-|tau| / scale_i)
 */
CovarianceMatrix ExponentialModel::operator() (const NumericalPoint & tau) const
{
  CovarianceMatrix covarianceMatrix(dimension_);
  // Absolute value of tau
  const NumericalScalar absTau(tau.norm());
  NumericalPoint exponentialTerms(dimension_);
  for (UnsignedInteger i = 0; i < dimension_; ++i)
  {
    const NumericalScalar value(amplitude_[i] * exp(-0.5 * absTau / scale_[i]));
    exponentialTerms[i] = value;
    covarianceMatrix(i, i) = value * value;
  }
  if (!isDiagonal_)
    for (UnsignedInteger i = 0; i < dimension_ ; ++i)
      for (UnsignedInteger j = 0; j < i ; ++j)
        covarianceMatrix(i, j) = exponentialTerms[i] * spatialCorrelation_(i, j) * exponentialTerms[j];

  return covarianceMatrix;
}

NumericalScalar ExponentialModel::computeAsScalar(const NumericalPoint & tau) const
{
  if (dimension_ != 1) throw NotDefinedException(HERE) << "Error: the covariance model is of dimension=" << dimension_ << ", expected dimension=1.";
  return amplitude_[0] * exp(-tau.norm() / scale_[0]);
}

/* Discretize the covariance function on a given TimeGrid */
CovarianceMatrix ExponentialModel::discretize(const RegularGrid & timeGrid) const
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

  // Loop over the main diagonal block
  for (UnsignedInteger block = 0; block < size; ++block)
    // Copy of the lower triangle only
    for (UnsignedInteger i = 0; i < dimension_; ++i)
    {
      // The diagonal part
      cov( block * dimension_ + i,
           block * dimension_ + i ) = covTau0(i, i);
      // The lower off-diagonal part if needed
      if (!isDiagonal_)
        for (UnsignedInteger j = 0; j < i; ++j)
          cov( block * dimension_ + i,
               block * dimension_ + j ) = covTau0(i, j);
    } // Lower triangle
  // Loop over the remaining diagonal blocks
  for (UnsignedInteger diag = 1; diag < size; ++diag)
  {
    const CovarianceMatrix covTau( operator()( diag * timeStep ) );

    // Loop over the main block diagonal
    for (UnsignedInteger block = 0; block < size - diag; ++block)
      // Copy of the full block
      for (UnsignedInteger i = 0; i < dimension_; ++i)
      {
        // The diagonal part
        cov( block          * dimension_ + i,
             (block + diag) * dimension_ + i ) = covTau(i, i);
        // The off-diagonal part if needed
        if (!isDiagonal_)
        {
          for (UnsignedInteger j = 0; j < i; ++j)
            cov( block          * dimension_ + i,
                 (block + diag) * dimension_ + j ) = covTau(i, j);
          for (UnsignedInteger j = i + 1; j < dimension_; ++j)
            cov( block          * dimension_ + i,
                 (block + diag) * dimension_ + j ) = covTau(i, j);
        } // Off-diagonal
      } // Full block
  } // Off-diagonal blocks

  return cov;
}

/* Partial discretization with respect to the second argument */
/* Computation of the covariance function, stationary interface
 * C_{i,j}(tau) = amplitude_i * exp(-|tau| / scale_i) * R_{i,j} * amplitude_j * exp(-|tau| / scale_j)
 * C_{i,i}(tau) = amplitude_i^2 * exp(-|tau| / scale_i)
 */
Basis ExponentialModel::getPartialDiscretization(const NumericalSample & secondLocation) const
{
  Basis basis;
  throw NotYetImplementedException(HERE) << "ExponentialModel::getPartialDiscretization";
  return basis;
}

/* Is it a stationary model ? */
Bool ExponentialModel::isStationary() const
{
  return true;
}

/* String converter */
String ExponentialModel::__repr__() const
{
  OSS oss(true);
  oss << "class=" << ExponentialModel::GetClassName();
  oss << " amplitude=" << amplitude_
      << " scale=" << scale_
      << " spatial correlation=" << spatialCorrelation_
      << " isDiagonal=" << isDiagonal_;
  return oss;
}

/* String converter */
String ExponentialModel::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "class=" << ExponentialModel::GetClassName();
  oss << " amplitude=" << amplitude_
      << " scale=" << scale_;
  if (!isDiagonal_)
    oss << " spatial correlation=\n" << spatialCorrelation_.__str__(offset);
  else
    oss << " no spatial correlation";
  return oss;
}

/* Amplitude accessor */
NumericalPoint ExponentialModel::getAmplitude() const
{
  return amplitude_;
}

void ExponentialModel::setAmplitude(const NumericalPoint & amplitude)
{
  for (UnsignedInteger index = 0; index < dimension_; ++index)
    if (amplitude[index] <= 0)
      throw InvalidArgumentException(HERE) << "Error - The component " << index << " of amplitude is non positive" ;
  amplitude_ = amplitude;
}

/* Scale accessor */
NumericalPoint ExponentialModel::getScale() const
{
  return scale_;
}

void ExponentialModel::setScale(const NumericalPoint & scale)
{
  for (UnsignedInteger index = 0; index < dimension_; ++index)
    if (scale[index] <= 0)
      throw InvalidArgumentException(HERE) << "Error - The component " << index << " of scale is non positive" ;
  scale_ = scale;
}

/* Spatial correlation accessor */
CorrelationMatrix ExponentialModel::getSpatialCorrelation() const
{
  if (!isDiagonal_) return spatialCorrelation_;
  return CorrelationMatrix(dimension_);
}

/* Method save() stores the object through the StorageManager */
void ExponentialModel::save(Advocate & adv) const
{
  StationaryCovarianceModel::save(adv);
  adv.saveAttribute( "amplitude_", amplitude_);
  adv.saveAttribute( "scale_", scale_);
  adv.saveAttribute( "spatialCorrelation_", spatialCorrelation_);
  adv.saveAttribute( "isDiagonal_", isDiagonal_);
}

/* Method load() reloads the object from the StorageManager */
void ExponentialModel::load(Advocate & adv)
{
  StationaryCovarianceModel::load(adv);
  adv.loadAttribute( "amplitude_", amplitude_);
  adv.loadAttribute( "scale_", scale_);
  adv.loadAttribute( "spatialCorrelation_", spatialCorrelation_);
  adv.loadAttribute( "isDiagonal_", isDiagonal_);
}

END_NAMESPACE_OPENTURNS
