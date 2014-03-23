//                                               -*- C++ -*-
/**
 *  @file  CompositeProcess.cxx
 *  @brief A class which implements the CompositeProcess process
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */

#include "CompositeProcess.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(CompositeProcess);

static Factory<CompositeProcess> RegisteredFactory("CompositeProcess");

CompositeProcess::CompositeProcess(const String & name)
  : ProcessImplementation(name)
{
  // Nothing to do
}

/* Standard constructor */
CompositeProcess::CompositeProcess(const DynamicalFunction & function,
                                   const Antecedent & p_antecedent,
                                   const String & name)
  : ProcessImplementation(name)
  , function_(function)
  , p_antecedent_(p_antecedent)
{
  if (function.getInputDimension() != p_antecedent->getDimension())
    throw InvalidArgumentException(HERE) << "Error: trying to build a CompositeProcess from a Process and a DynamicalFunction with incompatible dimensions "
                                         << "here Process dimension=" << p_antecedent->getDimension()
                                         << " and DynamicalFunction input dimension=" << function.getInputDimension();
  setMesh(p_antecedent_->getMesh());
  setDimension(function.getOutputDimension());
}

/* Standard constructor */
CompositeProcess::CompositeProcess(const DynamicalFunction & function,
                                   const Process & antecedent,
                                   const String & name)
  : ProcessImplementation(name)
  , function_(function)
  , p_antecedent_(antecedent.getImplementation())
{
  std::cerr << "function=" << function << ", process=" << antecedent << std::endl;
  if (function.getInputDimension() != p_antecedent_->getDimension())
    throw InvalidArgumentException(HERE) << "Error: trying to build a CompositeProcess from a Process and a DynamicalFunction with incompatible dimensions "
                                         << "here Process dimension=" << p_antecedent_->getDimension()
                                         << " and DynamicalFunction input dimension=" << function.getInputDimension();
  setMesh(p_antecedent_->getMesh());
  setDimension(function.getOutputDimension());
}

/* Virtual constructor */
CompositeProcess * CompositeProcess::clone() const
{
  return new CompositeProcess(*this);
}

/* String converter */
String CompositeProcess::__repr__() const
{
  OSS oss(true);
  oss << "class=" << CompositeProcess::GetClassName()
      << " function=" << function_.__repr__()
      << " antecedent=" << p_antecedent_->__repr__();
  return oss;
}

String CompositeProcess::__str__(const String & offset) const
{
  OSS oss(false);
  oss << " CompositeProcess process dimension = " << dimension_ << "\n"
      << " Process = " << function_.__str__(offset + "  ")
      << "( " << p_antecedent_->__str__(offset + "  ")
      << ") " ;
  return oss;
}

/* Is the underlying a composite process ? */
Bool CompositeProcess::isComposite() const
{
  return true;
}

/* Antecedent accessor */
CompositeProcess::Antecedent CompositeProcess::getAntecedent() const
{
  return p_antecedent_;
}

/* Function accessor */
DynamicalFunction CompositeProcess::getFunction() const
{
  return function_;
}

/* Realization accessor */
Field CompositeProcess::getRealization() const
{
  return function_(p_antecedent_->getRealization());
}

/* Compute the next steps of a random walk */
TimeSeries CompositeProcess::getFuture(const UnsignedInteger stepNumber) const
{
  /* TimeGrid of the process */
  RegularGrid timeGrid;
  try
  {
    timeGrid = getTimeGrid();
  }
  catch (...)
  {
    throw InternalException(HERE) << "Error: can extend the realization of a process only if defined on a regular gris.";
  }
  if (stepNumber == 0) throw InvalidArgumentException(HERE) << "Error: the number of future steps must be positive.";
  /* TimeGrid associated with the possible future */
  const NumericalScalar timeStep(timeGrid.getStep());
  const RegularGrid futurTimeGrid(timeGrid.getEnd(), timeStep, stepNumber);
  return TimeSeries(futurTimeGrid, function_(p_antecedent_->getFuture(stepNumber)).getValues());
}

/* Get the random vector corresponding to the i-th marginal component */
CompositeProcess::Implementation CompositeProcess::getMarginalProcess(const UnsignedInteger i) const
{
  return new CompositeProcess(function_.getMarginal(i), Pointer<ProcessImplementation>(p_antecedent_->getMarginalProcess(i)));
}

/* Get the marginal random vector corresponding to indices components */
CompositeProcess::Implementation CompositeProcess::getMarginalProcess(const Indices & indices) const
{
  return new CompositeProcess(function_.getMarginal(indices), Pointer<ProcessImplementation>(p_antecedent_->getMarginalProcess(indices)));
}

/* Method save() stores the object through the StorageManager */
void CompositeProcess::save(Advocate & adv) const
{
  ProcessImplementation::save(adv);
  adv.saveAttribute( "function_", function_ );
  adv.saveAttribute( "antecedent_", *p_antecedent_ );
}

/* Method load() reloads the object from the StorageManager */
void CompositeProcess::load(Advocate & adv)
{
  ProcessImplementation::load(adv);
  adv.loadAttribute( "function_", function_ );
  TypedInterfaceObject<ProcessImplementation> antecedent;
  adv.loadAttribute( "antecedent_", antecedent );
  p_antecedent_ = antecedent.getImplementation();
}

END_NAMESPACE_OPENTURNS
