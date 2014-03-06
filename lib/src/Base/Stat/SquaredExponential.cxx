//                                               -*- C++ -*-
/**
 *  @file  SquaredExponential.cxx
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
#include "SquaredExponential.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(SquaredExponential);

static Factory<SquaredExponential> RegisteredFactory("SquaredExponential");


/* Constructor with parameters */
SquaredExponential::SquaredExponential(const UnsignedLong & dimension)
  : CovarianceModelImplementation(dimension)
  , theta_(ResourceMap::GetAsNumericalScalar("SquaredExponential-DefaultTheta"))
{
}

SquaredExponential::SquaredExponential(const UnsignedLong & dimension,
                                       const NumericalScalar theta
                                      )
  : CovarianceModelImplementation(dimension)
  , theta_(theta)
{
}

/* Virtual constructor */
SquaredExponential * SquaredExponential::clone() const
{
  return new SquaredExponential(*this);
}

/* Computation of the covariance density function */
CovarianceMatrix SquaredExponential::operator() (const NumericalPoint & s,
    const NumericalPoint & t) const
{
  CovarianceMatrix covariance(1);
  const NumericalScalar d = (s - t).normSquare();
  covariance(0, 0) = exp(-theta_ * d);
  return covariance;
}

/* Gradient */
SymmetricTensor SquaredExponential::partialGradient(const NumericalPoint & s,
    const NumericalPoint & t) const
{
  UnsignedLong dimension = getDimension();
  SymmetricTensor gradient(1, dimension);
  const NumericalScalar d = (s - t).normSquare();
  for (UnsignedLong j = 0; j < dimension; ++ j)
  {
    gradient(0, 0, j) = -2.*theta_ * (s[j] - t[j]) * exp(-theta_ * d);
  }
  return gradient;
}

/* String converter */
String SquaredExponential::__repr__() const
{
  OSS oss;
  oss << "class=" << SquaredExponential::GetClassName()
      << " theta=" << theta_;
  return oss;
}

/* String converter */
String SquaredExponential::__str__(const String & offset) const
{
  return __repr__();
}

void SquaredExponential::setParameters(const NumericalPointWithDescription& parameters)
{
  if (parameters.getDimension() != 1) throw InvalidArgumentException(HERE) << "SquaredExponential parameter dimension should be 1 (got " << parameters.getDimension() << ")";
  theta_ = parameters[0];
}

NumericalPointWithDescription SquaredExponential::getParameters() const
{
  NumericalPointWithDescription result(1, theta_);
  result.setDescription(Description(1, "theta"));
  return result;
}

/* Method save() stores the object through the StorageManager */
void SquaredExponential::save(Advocate & adv) const
{
  CovarianceModelImplementation::save(adv);
  adv.saveAttribute("theta_", theta_);
}

/* Method load() reloads the object from the StorageManager */
void SquaredExponential::load(Advocate & adv)
{
  CovarianceModelImplementation::load(adv);
  adv.loadAttribute("theta_", theta_);
}

END_NAMESPACE_OPENTURNS
