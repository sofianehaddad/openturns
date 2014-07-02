//                                               -*- C++ -*-
/**
 *  @file  CovarianceModel.cxx
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
#include "CovarianceModel.hxx"
#include "ExponentialModel.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class CovarianceModel
 */

CLASSNAMEINIT(CovarianceModel);

//   static Factory<CovarianceModel> RegisteredFactory("CovarianceModel");

/* Constructor without parameters */
CovarianceModel::CovarianceModel()
  : TypedInterfaceObject<CovarianceModelImplementation>(new ExponentialModel())
{
  // Nothing to do
}

/* Parameters constructor */
CovarianceModel::CovarianceModel(const CovarianceModelImplementation & implementation)
  : TypedInterfaceObject<CovarianceModelImplementation>(implementation.clone())
{
  // Nothing to do
}

/* Constructor from implementation */
CovarianceModel::CovarianceModel(const Implementation & p_implementation)
  : TypedInterfaceObject<CovarianceModelImplementation>(p_implementation)
{
  // Nothing to do
}

/* Constructor from implementation pointer */
CovarianceModel::CovarianceModel(CovarianceModelImplementation * p_implementation)
  : TypedInterfaceObject<CovarianceModelImplementation>(p_implementation)
{
  // Nothing to do
}

/* Dimension accessor */
UnsignedInteger CovarianceModel::getDimension() const
{
  return getImplementation()->getDimension();
}

/* Computation of the covariance function */
CovarianceMatrix CovarianceModel::operator() (const NumericalScalar s,
    const NumericalScalar t) const
{
  return getImplementation()->operator() (s, t);
}

CovarianceMatrix CovarianceModel::operator() (const NumericalPoint & s,
    const NumericalPoint & t) const
{
  return getImplementation()->operator() (s, t);
}

NumericalScalar CovarianceModel::computeAsScalar (const NumericalPoint & s,
    const NumericalPoint & t) const
{
  return getImplementation()->computeAsScalar(s, t);
}

CovarianceMatrix CovarianceModel::operator() (const NumericalScalar tau) const
{
  return getImplementation()->operator() (tau);
}

CovarianceMatrix CovarianceModel::operator() (const NumericalPoint & tau) const
{
  return getImplementation()->operator() (tau);
}

NumericalScalar CovarianceModel::computeAsScalar (const NumericalPoint & tau) const
{
  return getImplementation()->computeAsScalar(tau);
}

/* Gradient */
SymmetricTensor CovarianceModel::partialGradient(const NumericalPoint & s,
    const NumericalPoint & t) const
{
  return getImplementation()->partialGradient(s, t);
}

/* Discretize the covariance function on a given TimeGrid/Mesh */
CovarianceMatrix CovarianceModel::discretize(const RegularGrid & timeGrid) const
{
  return getImplementation()->discretize(timeGrid);
}

CovarianceMatrix CovarianceModel::discretize(const Mesh & mesh) const
{
  return getImplementation()->discretize(mesh);
}

NumericalSample CovarianceModel::discretizeRow(const NumericalSample & vertices,
    const UnsignedInteger p) const
{
  return getImplementation()->discretizeRow(vertices, p);
}

/* Partial discretization with respect to the second argument */
Basis CovarianceModel::getPartialDiscretization(const NumericalSample & secondLocation) const
{
  return getImplementation()->getPartialDiscretization(secondLocation);
}

/* Is it a stationary covariance model ? */
Bool CovarianceModel::isStationary() const
{
  return getImplementation()->isStationary();
}

/* String converter */
String CovarianceModel::__repr__() const
{
  return getImplementation()->__repr__();
}

/* String converter */
String CovarianceModel::__str__(const String & offset) const
{
  return getImplementation()->__str__(offset);
}

void CovarianceModel::setParameters(const NumericalPointWithDescription& parameters)
{
  copyOnWrite();
  getImplementation()->setParameters(parameters);
}

NumericalPointWithDescription CovarianceModel::getParameters() const
{
  return getImplementation()->getParameters();
}


END_NAMESPACE_OPENTURNS
