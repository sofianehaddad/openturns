//                                               -*- C++ -*-
/**
 *  @file  GeneralizedExponential.cxx
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
 *  @author: schueller
 */
#include "GeneralizedExponential.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(GeneralizedExponential);

static Factory<GeneralizedExponential> RegisteredFactory("GeneralizedExponential");


/* Constructor with parameters */
GeneralizedExponential::GeneralizedExponential(const UnsignedLong & dimension)
  : CovarianceModelImplementation(dimension)
  , theta_(ResourceMap::GetAsNumericalScalar("GeneralizedExponential-DefaultTheta"))
  , p_(1.0)
{
}

GeneralizedExponential::GeneralizedExponential(const UnsignedLong & dimension,
    const NumericalScalar theta,
    const NumericalScalar p)
  : CovarianceModelImplementation(dimension)
  , theta_(theta)
  , p_(p)
{
}

/* Virtual constructor */
GeneralizedExponential * GeneralizedExponential::clone() const
{
  return new GeneralizedExponential(*this);
}

/* Computation of the covariance density function */
CovarianceMatrix GeneralizedExponential::operator() (const NumericalPoint & s,
    const NumericalPoint & t) const
{
  CovarianceMatrix covariance(1);
  const NumericalScalar d = (s - t).norm1();
  covariance(0, 0) = exp(-theta_ * pow(d, p_));
  return covariance;
}

/* Gradient */
SymmetricTensor GeneralizedExponential::partialGradient(const NumericalPoint & s,
    const NumericalPoint & t) const
{
  UnsignedLong dimension = getDimension();
  SymmetricTensor gradient(1, dimension);
  const NumericalScalar d = (s - t).norm1();
  for (UnsignedLong j = 0; j < dimension; ++ j)
  {
    const NumericalScalar sign = (s[j] - t[j] >= 0.) ? 1. : -1.;
    gradient(0, 0, j) = -theta_ * p_ * pow(d, p_ - 1.) * sign * exp(-theta_ * pow(d, p_));
  }
  return gradient;
}

/* String converter */
String GeneralizedExponential::__repr__() const
{
  OSS oss;
  oss << "class=" << GeneralizedExponential::GetClassName()
      << " theta=" << theta_
      << " p=" << p_;
  return oss;
}

/* String converter */
String GeneralizedExponential::__str__(const String & offset) const
{
  return __repr__();
}

void GeneralizedExponential::setParameters(const NumericalPointWithDescription& parameters)
{
  if (parameters.getDimension() != 1) throw InvalidArgumentException(HERE) << "GeneralizedExponential parameter dimension should be 1 (got " << parameters.getDimension() << ")";
  theta_ = parameters[0];
}

NumericalPointWithDescription GeneralizedExponential::getParameters() const
{
  NumericalPointWithDescription result(1, theta_);
  result.setDescription(Description(1, "theta"));
  return result;
}

/* Method save() stores the object through the StorageManager */
void GeneralizedExponential::save(Advocate & adv) const
{
  CovarianceModelImplementation::save(adv);
  adv.saveAttribute("theta_", theta_);
  adv.saveAttribute("p_", p_);
}

/* Method load() reloads the object from the StorageManager */
void GeneralizedExponential::load(Advocate & adv)
{
  CovarianceModelImplementation::load(adv);
  adv.loadAttribute("theta_", theta_);
  adv.loadAttribute("p_", p_);
}

END_NAMESPACE_OPENTURNS
