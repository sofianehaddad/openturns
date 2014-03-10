//                                               -*- C++ -*-
/**
 *  @file  ProcessImplementation.cxx
 *  @brief An interface for all implementation class of process
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include "PersistentObjectFactory.hxx"
#include "ProcessImplementation.hxx"
#include "Exception.hxx"
#include "NumericalMathFunction.hxx"
#include "PiecewiseLinearEvaluationImplementation.hxx"
#include "DatabaseNumericalMathEvaluationImplementation.hxx"
#include "RegularGrid.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ProcessImplementation);

static Factory<ProcessImplementation> RegisteredFactory("ProcessImplementation");

/* Default constructor */
ProcessImplementation::ProcessImplementation(const String & name)
  : PersistentObject(name),
    description_(),
    dimension_(1),
    mesh_(RegularGrid(0.0, 1.0, 1))
{
  // Nothing to do
}

/* Virtual constructor */
ProcessImplementation * ProcessImplementation::clone() const
{
  return new ProcessImplementation(*this);
}

/* String converter */
String ProcessImplementation::__repr__() const
{
  OSS oss(true);
  oss << "class=" << ProcessImplementation::GetClassName()
      << " dimension_=" << dimension_
      << " description=" << description_
      << " mesh=" << mesh_;
  return oss;
}

/* String converter */
String ProcessImplementation::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "class=" << ProcessImplementation::GetClassName()
      << " dimension=" << dimension_
      << " description=" << description_.__str__(offset)
      << " mesh=" << mesh_.__str__(offset);
  return oss;
}

/* Dimension accessor */
UnsignedInteger ProcessImplementation::getMeshDimension() const
{
  return mesh_.getDimension();
}

UnsignedInteger ProcessImplementation::getDimension() const
{
  return dimension_;
}

void ProcessImplementation::setDimension(const UnsignedInteger dimension)
{
  dimension_ = dimension;
}

/* Description accessor */
void ProcessImplementation::setDescription(const Description & description)
{
  description_ = description;
}

Description ProcessImplementation::getDescription() const
{
  return description_;
}

/* TimeGrid accessor */
RegularGrid ProcessImplementation::getTimeGrid() const
{
  return getMesh();
}

void ProcessImplementation::setTimeGrid(const RegularGrid & timeGrid)
{
  setMesh(timeGrid);
}

/* Mesh accessor */
Mesh ProcessImplementation::getMesh() const
{
  return mesh_;
}

void ProcessImplementation::setMesh(const Mesh & mesh)
{
  mesh_ = mesh;
}


/* Here is the interface that all derived class must implement */


/* Is the underlying a gaussian process ? */
Bool ProcessImplementation::isNormal() const
{
  return false;
}

/* Is the underlying a stationary process ? */
Bool ProcessImplementation::isStationary() const
{
  return false;
}

/* Is the underlying a composite process ? */
Bool ProcessImplementation::isComposite() const
{
  return false;
}

/* Discrete realization accessor */
Field ProcessImplementation::getRealization() const
{
  throw NotYetImplementedException(HERE);
}

/* Continuous realization accessor */
NumericalMathFunction ProcessImplementation::getContinuousRealization() const
{
  const Field field(getRealization());
  const NumericalSample values(field.getValues());
  if (mesh_.getDimension() == 1)
  {
    // The continuous realization will be obtained by a piecewise linear interpolation
    NumericalPoint locations(mesh_.getVertices().getImplementation()->getData());
    return PiecewiseLinearEvaluationImplementation(locations, values);
  }
  return DatabaseNumericalMathEvaluationImplementation(mesh_.getVertices(), values);
}

ProcessSample ProcessImplementation::getSample(const UnsignedInteger size) const
{
  ProcessSample result(mesh_, size, dimension_);
  for (UnsignedInteger i = 0; i < size; ++i) result[i] = getRealization().getValues();
  return result;
}

/* Future accessor */
TimeSeries ProcessImplementation::getFuture(const UnsignedInteger stepNumber) const
{
  throw NotYetImplementedException(HERE);
}

ProcessSample ProcessImplementation::getFuture(const UnsignedInteger stepNumber,
    const UnsignedInteger size) const
{
  if (mesh_.getDimension() != 1) throw NotDefinedException(HERE) << "Error: can extend the realization of a process only if defined on a 1D mesh.";
  if (size == 0) return ProcessSample(mesh_, 0, dimension_);
  ProcessSample result(size, getFuture(stepNumber));
  for (UnsignedInteger i = 1; i < size; ++i) result[i] = getFuture(stepNumber).getValues();
  return result;
}

/* Get the random vector corresponding to the i-th marginal component */
ProcessImplementation::Implementation ProcessImplementation::getMarginalProcess(const UnsignedInteger i) const
{
  throw NotYetImplementedException(HERE);
}

/* Get the marginal random vector corresponding to indices components */
ProcessImplementation::Implementation ProcessImplementation::getMarginalProcess(const Indices & indices) const
{
  throw NotYetImplementedException(HERE);
}


/* Method save() stores the object through the StorageManager */
void ProcessImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "dimension_", dimension_ );
  adv.saveAttribute( "description_", description_ );
  adv.saveAttribute( "mesh_", mesh_ );
}

/* Method load() reloads the object from the StorageManager */
void ProcessImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "dimension_", dimension_ );
  adv.loadAttribute( "description_", description_ );
  adv.loadAttribute( "mesh_", mesh_ );
}

END_NAMESPACE_OPENTURNS
