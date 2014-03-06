//                                               -*- C++ -*-
/**
 *  @file  PiecewiseHermiteEvaluationImplementation.cxx
 *  @brief The evaluation part of a hermite piecewise scalar function
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
 *  @author dutka
 *  @date   2008-05-21 11:21:38 +0200 (Wed, 21 May 2008)
 */
#include <algorithm>
#include "PiecewiseHermiteEvaluationImplementation.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "Description.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(PiecewiseHermiteEvaluationImplementation);

static Factory<PiecewiseHermiteEvaluationImplementation> RegisteredFactory("PiecewiseHermiteEvaluationImplementation");


/* Default constructor */
PiecewiseHermiteEvaluationImplementation::PiecewiseHermiteEvaluationImplementation()
  : NumericalMathEvaluationImplementation()
  , locations_(1)
  , values_(1, 1)
  , derivatives_(1, 1)
{
  // Nothing to do
}


/* Parameters constructor */
PiecewiseHermiteEvaluationImplementation::PiecewiseHermiteEvaluationImplementation(const NumericalPoint & locations,
    const NumericalPoint & values,
    const NumericalPoint & derivatives)
  : NumericalMathEvaluationImplementation()
  , locations_(0)
  , values_(0, 0)
  , derivatives_(0, 0)
{
  const UnsignedLong sizeValues(values.getSize());
  NumericalSample sampleValues(sizeValues, 1);
  for (UnsignedLong i = 0; i < sizeValues; ++i) sampleValues[i][0] = values[i];
  const UnsignedLong sizeDerivatives(derivatives.getSize());
  NumericalSample sampleDerivatives(sizeDerivatives, 1);
  for (UnsignedLong i = 0; i < sizeDerivatives; ++i) sampleDerivatives[i][0] = derivatives[i];
  // Check the input
  setLocationsValuesAndDerivatives(locations, sampleValues, sampleDerivatives);
}


/* Parameters constructor */
PiecewiseHermiteEvaluationImplementation::PiecewiseHermiteEvaluationImplementation(const NumericalPoint & locations,
    const NumericalSample & values,
    const NumericalSample & derivatives)
  : NumericalMathEvaluationImplementation()
  , locations_(0)
  , values_(0, 0)
  , derivatives_(0, 0)
{
  // Check the input
  setLocationsValuesAndDerivatives(locations, values, derivatives);
}


/* Virtual constructor */
PiecewiseHermiteEvaluationImplementation * PiecewiseHermiteEvaluationImplementation::clone() const
{
  return new PiecewiseHermiteEvaluationImplementation(*this);
}


/* String converter */
String PiecewiseHermiteEvaluationImplementation::__repr__() const
{
  return OSS(true) << "class=" << GetClassName()
         << " locations=" << locations_
         << " values=" << values_;
}

String PiecewiseHermiteEvaluationImplementation::__str__(const String & offset) const
{
  return OSS(false) << offset << __repr__();
}


/* Evaluation operator */
NumericalPoint PiecewiseHermiteEvaluationImplementation::operator () (const NumericalPoint & inP) const
{
  if (inP.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: expected an input point of dimension 1, got dimension=" << inP.getDimension();
  const NumericalScalar x(inP[0]);
  UnsignedLong i0(0);
  if (x <= locations_[i0]) return values_[i0];
  UnsignedLong i1(locations_.getSize() - 1);
  if (x >= locations_[i1]) return values_[i1];
  if (isRegular_)
  {
    i0 = static_cast<UnsignedLong>(trunc(x - locations_[0]) / (locations_[1] - locations_[0]));
    i1 = i0 + 1;
  }
  else
    // Find the segment containing x by bisection
    while (i1 - i0 > 1)
    {
      const UnsignedLong im((i1 + i0) / 2);
      if (x < locations_[im]) i1 = im;
      else i0 = im;
    }

  const NumericalScalar h(locations_[i1] - locations_[i0]);
  const NumericalScalar theta((x - locations_[i0]) / h);
  const NumericalPoint v0(values_[i0]);
  const NumericalPoint v1(values_[i1]);
  const NumericalPoint dv0(derivatives_[i0]);
  const NumericalPoint dv1(derivatives_[i1]);
  const UnsignedLong dimension(getOutputDimension());
  NumericalPoint value(dimension);
  const NumericalScalar alpha(1.0 - theta);
  const NumericalScalar beta(theta * alpha);
  const NumericalScalar gamma(2.0 * theta - 1.0);
  for (UnsignedLong i = 0; i < dimension; ++i) value[i] = alpha * v0[i] + theta * v1[i] + beta * (gamma * (v1[i] - v0[i]) + h * (alpha * dv0[i] - theta * dv1[i]));
  return value;
}

/* Compute the derivative */
NumericalPoint PiecewiseHermiteEvaluationImplementation::derivate(const NumericalPoint & inP) const
{
  if (inP.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: expected an input point of dimension 1, got dimension=" << inP.getDimension();
  const NumericalScalar x(inP[0]);
  UnsignedLong i0(0);
  if (x <= locations_[i0]) return values_[i0];
  UnsignedLong i1(locations_.getSize() - 1);
  if (x >= locations_[i1]) return values_[i1];
  if (isRegular_)
  {
    i0 = static_cast<UnsignedLong>(trunc(x - locations_[0]) / (locations_[1] - locations_[0]));
    i1 = i0 + 1;
  }
  else
    // Find the segment containing x by bisection
    while (i1 - i0 > 1)
    {
      const UnsignedLong im((i1 + i0) / 2);
      if (x < locations_[im]) i1 = im;
      else i0 = im;
    }

  const NumericalScalar h(locations_[i1] - locations_[i0]);
  const NumericalScalar theta((x - locations_[i0]) / h);
  const NumericalPoint v0(values_[i0]);
  const NumericalPoint v1(values_[i1]);
  const NumericalPoint dv0(derivatives_[i0]);
  const NumericalPoint dv1(derivatives_[i1]);
  const UnsignedLong dimension(getOutputDimension());
  NumericalPoint value(dimension);
  const NumericalScalar alpha(1.0 - theta);
  const NumericalScalar beta(theta * alpha);
  const NumericalScalar gamma(2.0 * theta - 1.0);
  for (UnsignedLong i = 0; i < dimension; ++i) value[i] = (-v0[i] + v1[i] + alpha * (gamma * (v1[i] - v0[i]) + h * (alpha * dv0[i] - theta * dv1[i])) + beta * (2.0 * (v1[i] - v0[i]) + h * (- dv0[i] - dv1[i]))) / h;
  return value;
}

/* Locations accessor */
NumericalPoint PiecewiseHermiteEvaluationImplementation::getLocations() const
{
  return locations_;
}

void PiecewiseHermiteEvaluationImplementation::setLocations(const NumericalPoint & locations)
{
  const UnsignedLong size(locations.getSize());
  if (size < 2) throw InvalidArgumentException(HERE) << "Error: there must be at least 2 points to build a piecewise Hermite interpolation function.";
  if (locations.getSize() != values_.getSize()) throw InvalidArgumentException(HERE) << "Error: the number of locations=" << size << " must match the number of previously set values=" << values_.getSize();
  const NumericalScalar step(locations_[0] - locations_[0]);
  const NumericalScalar epsilon(ResourceMap::GetAsNumericalScalar("PiecewiseHermiteEvaluationImplementation-EpsilonRegular") * fabs(step));
  isRegular_ = true;
  for (UnsignedLong i = 0; i < size; ++i) isRegular_ = isRegular_ && (fabs(locations[i] - locations[0] - i * step) < epsilon);
  locations_ = locations;
  std::stable_sort(locations_.begin(), locations_.end());
}

/* Values accessor */
NumericalSample PiecewiseHermiteEvaluationImplementation::getValues() const
{
  return values_;
}

void PiecewiseHermiteEvaluationImplementation::setValues(const NumericalSample & values)
{
  const UnsignedLong size(values.getSize());
  if (size < 2) throw InvalidArgumentException(HERE) << "Error: there must be at least 2 points to build a piecewise Hermite interpolation function.";
  if (size != locations_.getSize()) throw InvalidArgumentException(HERE) << "Error: the number of values=" << size << " must match the number of previously set locations=" << locations_.getSize();
  values_ = values;
}

/* Derivatives accessor */
NumericalSample PiecewiseHermiteEvaluationImplementation::getDerivatives() const
{
  return derivatives_;
}

void PiecewiseHermiteEvaluationImplementation::setDerivatives(const NumericalSample & derivatives)
{
  const UnsignedLong size(derivatives.getSize());
  if (size < 2) throw InvalidArgumentException(HERE) << "Error: there must be at least 2 points to build a piecewise Hermite interpolation function.";
  if (size != locations_.getSize()) throw InvalidArgumentException(HERE) << "Error: the number of derivatives=" << size << " must match the number of previously set locations=" << locations_.getSize();
  derivatives_ = derivatives;
}


void PiecewiseHermiteEvaluationImplementation::setLocationsValuesAndDerivatives(const NumericalPoint & locations,
    const NumericalSample & values,
    const NumericalSample & derivatives)
{
  const UnsignedLong size(locations.getSize());
  if (size < 2) throw InvalidArgumentException(HERE) << "Error: there must be at least 2 points to build a piecewise Hermite interpolation function.";
  if (size != values.getSize()) throw InvalidArgumentException(HERE) << "Error: the number of values=" << values.getSize() << " must match the number of locations=" << size;
  if (size != derivatives.getSize()) throw InvalidArgumentException(HERE) << "Error: the number of derivatives=" << derivatives.getSize() << " must match the number of locations=" << size;
  const UnsignedLong outputDimension(values.getDimension());
  if (outputDimension != derivatives.getDimension()) throw InvalidArgumentException(HERE) << "Error: the dimension of the derivatives=" << derivatives.getDimension() << " must match the dimension of the locations=" << outputDimension;
  // Sort the data in increasing order according to the locations
  NumericalSample data(size, 1 + 2 * outputDimension);
  for (UnsignedLong i = 0; i < size; ++i)
  {
    data[i][0] = locations[i];
    for (UnsignedLong j = 0; j < outputDimension; ++j)
      data[i][1 + j] = values[i][j];
    for (UnsignedLong j = 0; j < outputDimension; ++j)
      data[i][1 + outputDimension + j] = derivatives[i][j];
  }
  data = data.sortAccordingToAComponent(0);
  locations_ = NumericalPoint(size);
  values_ = NumericalSample(size, outputDimension);
  derivatives_ = NumericalSample(size, outputDimension);
  const NumericalScalar step(data[1][0] - data[0][0]);
  const NumericalScalar epsilon(ResourceMap::GetAsNumericalScalar("PiecewiseHermiteEvaluationImplementation-EpsilonRegular") * fabs(step));
  isRegular_ = true;
  for (UnsignedLong i = 0; i < size; ++i)
  {
    locations_[i] = data[i][0];
    isRegular_ = isRegular_ && (fabs(locations_[i] - locations_[0] - i * step) < epsilon);
    for (UnsignedLong j = 0; j < outputDimension; ++j)
      values_[i][j] = data[i][1 + j];
    for (UnsignedLong j = 0; j < outputDimension; ++j)
      derivatives_[i][j] = data[i][1 + outputDimension + j];
  }
}

/* Input dimension accessor */
UnsignedLong PiecewiseHermiteEvaluationImplementation::getInputDimension() const
{
  return 1;
}

/* Output dimension accessor */
UnsignedLong PiecewiseHermiteEvaluationImplementation::getOutputDimension() const
{
  return values_.getDimension();
}


/* Method save() stores the object through the StorageManager */
void PiecewiseHermiteEvaluationImplementation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
  adv.saveAttribute( "locations_", locations_ );
  adv.saveAttribute( "values_", values_ );
  adv.saveAttribute( "derivatives_", derivatives_ );
  adv.saveAttribute( "isRegular_", isRegular_ );
}


/* Method load() reloads the object from the StorageManager */
void PiecewiseHermiteEvaluationImplementation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load(adv);
  adv.loadAttribute( "locations_", locations_ );
  adv.loadAttribute( "values_", values_ );
  adv.loadAttribute( "derivatives_", derivatives_ );
  adv.loadAttribute( "isRegular_", isRegular_ );
}


END_NAMESPACE_OPENTURNS
