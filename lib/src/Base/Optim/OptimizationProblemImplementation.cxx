//                                               -*- C++ -*-
/**
 *  @file  OptimizationProblemImplementation.cxx
 *  @brief OptimizationProblemImplementation implements an algorithm for finding the
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
#include <cstdlib>

#include "OptimizationProblemImplementation.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(OptimizationProblemImplementation);

/* Default constructor */
OptimizationProblemImplementation::OptimizationProblemImplementation()
 : PersistentObject()
 , objective_()
 , equalityContraint_()
 , inequalityConstraint_()
 , bounds_()
 , dimension_(0)
{
  // Nothing to do
}

/*
 * @brief General multi-objective equality, inequality and bound constraints
 */
OptimizationProblemImplementation::OptimizationProblemImplementation(NumericalMathFunction & objective,
								     NumericalMathFunction & equalityConstraint,
								     NumericalMathFunction & inequalityConstraint,
								     Interval & bounds)
  : PersistentObject()
  , objective_(objective)
  , equalityConstraint_(equalityConstraint)
  , inequalityConstraint_(inequalityConstraint)
  , bounds_(bounds)
  , dimension_(objective.getInputDimension())
{
  // Check if the input dimension of the objective, the constraints and the bounds are compatible
  if (hasEqualityConstraint() && (equalityConstraint.getInputDimension() != dimension_)) throw InvalidArgumentException(HERE) << "Error: the given equality constraints have an input dimension=" << equalityConstraint.getInputDimension() << " different from the input dimension=" << dimension_ << " of the objective.";
  if (hasInequalityConstraint() && (inequalityConstraint.getInputDimension() != dimension_)) throw InvalidArgumentException(HERE) << "Error: the given inequality constraints have an input dimension=" << inequalityConstraint.getInputDimension() << " different from the input dimension=" << dimension_ << " of the objective.";
  if (hasInequalityConstraint() && (inequalityConstraint.getInputDimension() != dimension_)) throw InvalidArgumentException(HERE) << "Error: the given inequality constraints have an input dimension=" << inequalityConstraint.getInputDimension() << " different from the input dimension=" << dimension_ << " of the objective.";
}

/* Equality constraint accessor */
NumericalMathFunction OptimizationProblemImplementation::getEqualityConstraint() const
{
  return equalityConstraint_;
}

void OptimizationProblemImplementation::setEqualityConstraint(const NumericalMathFunction & equalityConstraint)
{
  equalityConstraint_ = equalityConstraint;
}

/* Level value accessor */
NumericalScalar OptimizationProblemImplementation::getLevelValue() const
{
  return levelValue_;
}

/* Level value accessor */
void OptimizationProblemImplementation::setLevelValue(const NumericalScalar levelValue)
{
  levelValue_ = levelValue;
}

/* Result accessor */
OptimizationProblemImplementation::Result OptimizationProblemImplementation::getResult() const
{
  return result_;
}

/* Result accessor */
void OptimizationProblemImplementation::setResult(const Result & result)
{
  result_ = result;
}

/* Maximum iterations number accessor */
UnsignedInteger OptimizationProblemImplementation::getMaximumIterationsNumber() const
{
  return maximumIterationsNumber_;
}

/* Maximum iterations number accessor */
void OptimizationProblemImplementation::setMaximumIterationsNumber(const UnsignedInteger maximumIterationsNumber)
{
  maximumIterationsNumber_ = maximumIterationsNumber;
}

/* Maximum absolute error accessor */
NumericalScalar OptimizationProblemImplementation::getMaximumAbsoluteError() const
{
  return maximumAbsoluteError_;
}

/* Maximum absolute error accessor */
void OptimizationProblemImplementation::setMaximumAbsoluteError(const NumericalScalar maximumAbsoluteError)
{
  maximumAbsoluteError_ = maximumAbsoluteError;
}

/* Maximum relative error accessor */
NumericalScalar OptimizationProblemImplementation::getMaximumRelativeError() const
{
  return maximumRelativeError_;
}

/* Maximum relative error accessor */
void OptimizationProblemImplementation::setMaximumRelativeError(const NumericalScalar maximumRelativeError)
{
  maximumRelativeError_ = maximumRelativeError;
}

/* Maximum residual error accessor */
NumericalScalar OptimizationProblemImplementation::getMaximumResidualError() const
{
  return maximumResidualError_;
}

/* Maximum residual error accessor */
void OptimizationProblemImplementation::setMaximumResidualError(const NumericalScalar maximumResidualError)
{
  maximumResidualError_ = maximumResidualError;
}

/* Maximum constraint error accessor */
NumericalScalar OptimizationProblemImplementation::getMaximumConstraintError() const
{
  return maximumConstraintError_;
}

/* Maximum constraint error accessor */
void OptimizationProblemImplementation::setMaximumConstraintError(const NumericalScalar maximumConstraintError)
{
  maximumConstraintError_ = maximumConstraintError;
}

/* String converter */
String OptimizationProblemImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << OptimizationProblemImplementation::GetClassName()
      << " startingPoint=" << startingPoint_
      << " levelFunction=" << levelFunction_
      << " levelValue=" << levelValue_
      << " maximumIterationsNumber=" << maximumIterationsNumber_
      << " maximumAbsoluteError=" << maximumAbsoluteError_
      << " maximumRelativeError=" << maximumRelativeError_
      << " maximumResidualError=" << maximumResidualError_
      << " maximumConstraintError=" << maximumConstraintError_
      << " verbose=" << (verbose_ ? "true" : "false");
  return oss;
}

/* Level function accessor */
NumericalMathFunction OptimizationProblemImplementation::getLevelFunction() const
{
  return levelFunction_;
}

/* Level function accessor */
void OptimizationProblemImplementation::setLevelFunction(const NumericalMathFunction & levelFunction)
{
  levelFunction_ = levelFunction;
}

/* Performs the actual computation. Must be overloaded by the actual optimisation algorithm */
void OptimizationProblemImplementation::run()
{
  // To be overloaded
}

/* Virtual constructor */
OptimizationProblemImplementation * OptimizationProblemImplementation::clone() const
{
  return new OptimizationProblemImplementation(*this);
}

/* Verbose accessor */
Bool OptimizationProblemImplementation::getVerbose() const
{
  return verbose_;
}

/* Verbose accessor */
void OptimizationProblemImplementation::setVerbose(const Bool verbose)
{
  verbose_ = verbose;
}


END_NAMESPACE_OPENTURNS
