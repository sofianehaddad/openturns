//                                               -*- C++ -*-
/**
 *  @file  AbsoluteExponential.cxx
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
#include "AbsoluteExponential.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(AbsoluteExponential);

static Factory<AbsoluteExponential> RegisteredFactory("AbsoluteExponential");


/* Constructor with parameters */
AbsoluteExponential::AbsoluteExponential(const UnsignedInteger & dimension)
  : CovarianceModelImplementation(dimension)
  , theta_(ResourceMap::GetAsNumericalScalar("AbsoluteExponential-DefaultTheta"))
{
}

AbsoluteExponential::AbsoluteExponential(const UnsignedInteger & dimension,
    const NumericalScalar theta)
  : CovarianceModelImplementation(dimension)
  , theta_(theta)
{
}

/* Virtual constructor */
AbsoluteExponential * AbsoluteExponential::clone() const
{
  return new AbsoluteExponential(*this);
}

/* Computation of the covariance density function */
CovarianceMatrix AbsoluteExponential::operator() (const NumericalPoint & s,
    const NumericalPoint & t) const
{
  CovarianceMatrix covariance(1);
  const NumericalScalar d = (s - t).norm1();
  covariance(0, 0) = exp(-theta_ * d);
  return covariance;
}

/* Gradient */
SymmetricTensor AbsoluteExponential::partialGradient(const NumericalPoint & s,
    const NumericalPoint & t) const
{
  UnsignedInteger dimension = getDimension();
  SymmetricTensor gradient(1, dimension);
  const NumericalScalar d = (s - t).norm1();
  for (UnsignedInteger j = 0; j < dimension; ++ j)
  {
    const NumericalScalar sign = (s[j] - t[j] >= 0.) ? 1. : -1.;
    gradient(0, 0, j) = -theta_ * sign * exp(-theta_ * d);
  }
  return gradient;
}

/* String converter */
String AbsoluteExponential::__repr__() const
{
  OSS oss;
  oss << "class=" << AbsoluteExponential::GetClassName()
      << " theta=" << theta_;
  return oss;
}

/* String converter */
String AbsoluteExponential::__str__(const String & offset) const
{
  return __repr__();
}

void AbsoluteExponential::setParameters(const NumericalPointWithDescription& parameters)
{
  if (parameters.getDimension() != 1) throw InvalidArgumentException(HERE) << "AbsoluteExponential parameter dimension should be 1 (got " << parameters.getDimension() << ")";
  theta_ = parameters[0];
}

NumericalPointWithDescription AbsoluteExponential::getParameters() const
{
  NumericalPointWithDescription result(1, theta_);
  result.setDescription(Description(1, "theta"));
  return result;
}

/* Method save() stores the object through the StorageManager */
void AbsoluteExponential::save(Advocate & adv) const
{
  CovarianceModelImplementation::save(adv);
  adv.saveAttribute("theta_", theta_);
}

/* Method load() reloads the object from the StorageManager */
void AbsoluteExponential::load(Advocate & adv)
{
  CovarianceModelImplementation::load(adv);
  adv.loadAttribute("theta_", theta_);
}

END_NAMESPACE_OPENTURNS
