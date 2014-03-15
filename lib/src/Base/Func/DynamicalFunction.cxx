//                                               -*- C++ -*-
/**
 *  @file  DynamicalFunction.cxx
 *  @brief Abstract top-level class for all distributions
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
 *  @date   2011-05-24 19:30:41 +0200 (mar. 24 mai 2011)
 */
#include "DynamicalFunction.hxx"
#include "SpatialFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(DynamicalFunction);

/* Default constructor */
DynamicalFunction::DynamicalFunction(const UnsignedInteger meshDimension)
  : TypedInterfaceObject<DynamicalFunctionImplementation>(new DynamicalFunctionImplementation(meshDimension))
{
  // Nothing to do
}

/* Constructor from NumericalMathFunction */
DynamicalFunction::DynamicalFunction(const NumericalMathFunction & function,
				     const UnsignedInteger meshDimension)
  : TypedInterfaceObject<DynamicalFunctionImplementation>(SpatialFunction(function, meshDimension).clone())
{
  // Nothing to do
}

/* Constructor from DynamicalFunctionImplementation */
DynamicalFunction::DynamicalFunction(const DynamicalFunctionImplementation & implementation)
  : TypedInterfaceObject<DynamicalFunctionImplementation>(implementation.clone())
{
  // Nothing to do
}

/* Constructor from implementation */
DynamicalFunction::DynamicalFunction(const Implementation & p_implementation)
  : TypedInterfaceObject<DynamicalFunctionImplementation>(p_implementation)
{
  // Nothing to do
}

/* Constructor from implementation pointer */
DynamicalFunction::DynamicalFunction(DynamicalFunctionImplementation * p_implementation)
  : TypedInterfaceObject<DynamicalFunctionImplementation>(p_implementation)
{
  // Nothing to do
}

/* Comparison operator */
Bool DynamicalFunction::operator ==(const DynamicalFunction & other) const
{
  return true;
}

/* String converter */
String DynamicalFunction::__repr__() const
{
  return OSS(true) << "class=" << DynamicalFunction::GetClassName()
         << " name=" << getName()
         << " implementation=" << getImplementation()->__repr__();
}

/* String converter */
String DynamicalFunction::__str__(const String & offset) const
{
  return OSS(false) << offset << getClassName() << " :\n" << getImplementation()->__str__( offset + "  ");
}

/* Get the i-th marginal function */
DynamicalFunction DynamicalFunction::getMarginal(const UnsignedInteger i) const
{
  return *(getImplementation()->getMarginal(i));
}

/* Get the function corresponding to indices components */
DynamicalFunction DynamicalFunction::getMarginal(const Indices & indices) const
{
  return *(getImplementation()->getMarginal(indices));
}

/* Input description Accessor */
Description DynamicalFunction::getInputDescription() const
{
  return getImplementation()->getInputDescription();
}

/* Output description Accessor */
Description DynamicalFunction::getOutputDescription() const
{
  return getImplementation()->getOutputDescription();
}

/* Operator () */
NumericalPoint DynamicalFunction::operator() (const NumericalScalar timeStamp,
    const NumericalPoint & inP) const
{
  return getImplementation()->operator()(timeStamp, inP);
}

NumericalPoint DynamicalFunction::operator() (const NumericalPoint & location,
    const NumericalPoint & inP) const
{
  return getImplementation()->operator()(location, inP);
}

Field DynamicalFunction::operator() (const Field & inFld) const
{
  return getImplementation()->operator()(inFld);
}

/* Operator () */
ProcessSample DynamicalFunction::operator() (const ProcessSample & inPS) const
{
  return getImplementation()->operator()(inPS);
}

/* Accessor for mesh dimension */
UnsignedInteger DynamicalFunction::getMeshDimension() const
{
  return getImplementation()->getMeshDimension();
}

/* Accessor for input point dimension */
UnsignedInteger DynamicalFunction::getInputDimension() const
{
  return getImplementation()->getInputDimension();
}

/* Accessor for output point dimension */
UnsignedInteger DynamicalFunction::getOutputDimension() const
{
  return getImplementation()->getOutputDimension();
}

/* Number of calls to the dynamical function */
UnsignedInteger DynamicalFunction::getCallsNumber() const
{
  return getImplementation()->getCallsNumber();
}

END_NAMESPACE_OPENTURNS
