//                                               -*- C++ -*-
/**
 *  @file  OptimizationProblem.cxx
 *  @brief OptimizationProblem implements an algorithm for finding the
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
#include "OptimizationProblem.hxx"
#include "Cobyla.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(OptimizationProblem);

/* Default constructor */
OptimizationProblem::OptimizationProblem() :
  TypedInterfaceObject<OptimizationProblemImplementation>(new OptimizationProblemImplementation())
{
  // Nothing to do
}

/* Constructor from an implementation */
OptimizationProblem::OptimizationProblem(const OptimizationProblemImplementation & implementation) :
  TypedInterfaceObject<OptimizationProblemImplementation>(implementation.clone())
{
  // Nothing to do
}

/* Constructor from a Pointer to an implementation */
OptimizationProblem::OptimizationProblem(const Implementation & p_implementation) :
  TypedInterfaceObject<OptimizationProblemImplementation>(p_implementation)
{
  // Nothing to do
}

/*
 * @brief  Standard constructor: the problem is defined by a scalar valued function  (in fact, a 1-D vector valued function)
 *         and a level value
 */
OptimizationProblem::OptimizationProblem(const NumericalMathFunction & levelFunction):
  TypedInterfaceObject<OptimizationProblemImplementation>(new Cobyla(levelFunction))
{
  // Nothing to do
}

/* Starting point accessor */
NumericalPoint OptimizationProblem::getStartingPoint() const
{
  return getImplementation()->getStartingPoint();
}

/* Starting point accessor */
void OptimizationProblem::setStartingPoint(const NumericalPoint & startingPoint)
{
  copyOnWrite();
  getImplementation()->setStartingPoint(startingPoint);
}

/* Level value accessor */
NumericalScalar OptimizationProblem::getLevelValue() const
{
  return getImplementation()->getLevelValue();
}

/* Level value accessor */
void OptimizationProblem::setLevelValue(const NumericalScalar levelValue)
{
  copyOnWrite();
  getImplementation()->setLevelValue(levelValue);
}

/* Result accessor */
OptimizationProblem::Result OptimizationProblem::getResult() const
{
  return getImplementation()->getResult();
}

/* Result accessor */
void OptimizationProblem::setResult(const Result & result)
{
  copyOnWrite();
  getImplementation()->setResult(result);
}

/* Maximum iterations number accessor */
UnsignedInteger OptimizationProblem::getMaximumIterationsNumber() const
{
  return getImplementation()->getMaximumIterationsNumber();
}

/* Maximum iterations number accessor */
void OptimizationProblem::setMaximumIterationsNumber(const UnsignedInteger maximumIterationsNumber)
{
  copyOnWrite();
  getImplementation()->setMaximumIterationsNumber(maximumIterationsNumber);
}

/* Maximum absolute error accessor */
NumericalScalar OptimizationProblem::getMaximumAbsoluteError() const
{
  return getImplementation()->getMaximumAbsoluteError();
}

/* Maximum absolute error accessor */
void OptimizationProblem::setMaximumAbsoluteError(const NumericalScalar maximumAbsoluteError)
{
  copyOnWrite();
  getImplementation()->setMaximumAbsoluteError(maximumAbsoluteError);
}

/* Maximum relative error accessor */
NumericalScalar OptimizationProblem::getMaximumRelativeError() const
{
  return getImplementation()->getMaximumRelativeError();
}

/* Maximum relative error accessor */
void OptimizationProblem::setMaximumRelativeError(const NumericalScalar maximumRelativeError)
{
  copyOnWrite();
  getImplementation()->setMaximumRelativeError(maximumRelativeError);
}

/* Maximum residual error accessor */
NumericalScalar OptimizationProblem::getMaximumResidualError() const
{
  return getImplementation()->getMaximumResidualError();
}

/* Maximum residual error accessor */
void OptimizationProblem::setMaximumResidualError(const NumericalScalar maximumResidualError)
{
  copyOnWrite();
  getImplementation()->setMaximumResidualError(maximumResidualError);
}

/* Maximum constraint error accessor */
NumericalScalar OptimizationProblem::getMaximumConstraintError() const
{
  return getImplementation()->getMaximumConstraintError();
}

/* Maximum constraint error accessor */
void OptimizationProblem::setMaximumConstraintError(const NumericalScalar maximumConstraintError)
{
  copyOnWrite();
  getImplementation()->setMaximumConstraintError(maximumConstraintError);
}

/* String converter */
String OptimizationProblem::__repr__() const
{
  OSS oss(true);
  oss << "class=" << OptimizationProblem::GetClassName()
      << " implementation=" << getImplementation()->__repr__();
  return oss;
}

/* String converter */
String OptimizationProblem::__str__(const String & offset) const
{
  return __repr__();
}

/* Level function accessor */
NumericalMathFunction OptimizationProblem::getLevelFunction() const
{
  return getImplementation()->getLevelFunction();
}

/* Level function accessor */
void OptimizationProblem::setLevelFunction(const NumericalMathFunction & levelFunction)
{
  copyOnWrite();
  getImplementation()->setLevelFunction(levelFunction);
}

/* Performs the actual computation. Must be overloaded by the actual optimisation algorithm */
void OptimizationProblem::run()
{
  getImplementation()->run();
}

/* Verbose accessor */
Bool OptimizationProblem::getVerbose() const
{
  return getImplementation()->getVerbose();
}

/* Verbose accessor */
void OptimizationProblem::setVerbose(const Bool verbose)
{
  copyOnWrite();
  getImplementation()->setVerbose(verbose);
}

END_NAMESPACE_OPENTURNS
