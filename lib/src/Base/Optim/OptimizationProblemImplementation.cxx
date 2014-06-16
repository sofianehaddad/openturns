//                                               -*- C++ -*-
/**
 *  @file  OptimizationProblemImplementation.cxx
 *  @brief OptimizationProblemImplementation allows to describe an optimization problem
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
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(OptimizationProblemImplementation);

static Factory<OptimizationProblemImplementation> RegisteredFactory("OptimizationProblemImplementation");

/* Default constructor */
OptimizationProblemImplementation::OptimizationProblemImplementation()
  : PersistentObject()
  , objective_()
  , equalityConstraint_()
  , inequalityConstraint_()
  , bounds_()
  , dimension_(0)
{
  // Nothing to do
}

/*
 * @brief General multi-objective equality, inequality and bound constraints
 */
OptimizationProblemImplementation::OptimizationProblemImplementation(const NumericalMathFunction & objective,
                                                                     const NumericalMathFunction & equalityConstraint,
                                                                     const NumericalMathFunction & inequalityConstraint,
                                                                     const Interval & bounds)
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

/* Virtual constructor */
OptimizationProblemImplementation * OptimizationProblemImplementation::clone() const
{
  return new OptimizationProblemImplementation(*this);
}

/* Objective accessor */
NumericalMathFunction OptimizationProblemImplementation::getObjective() const
{
  return objective_;
}

void OptimizationProblemImplementation::setObjective(const NumericalMathFunction & objective)
{
  objective_ = objective;
}

Bool OptimizationProblemImplementation::hasMultipleObjective() const
{
  return objective_.getOutputDimension() > 1;
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

Bool OptimizationProblemImplementation::hasEqualityConstraint() const
{
  return equalityConstraint_.getEvaluationImplementation()->isActualImplementation();
}

/* Inequality constraint accessor */
NumericalMathFunction OptimizationProblemImplementation::getInequalityConstraint() const
{
  return inequalityConstraint_;
}

void OptimizationProblemImplementation::setInequalityConstraint(const NumericalMathFunction & inequalityConstraint)
{
  inequalityConstraint_ = inequalityConstraint;
}

Bool OptimizationProblemImplementation::hasInequalityConstraint() const
{
  return inequalityConstraint_.getEvaluationImplementation()->isActualImplementation();
}

/* Bounds accessor */
Interval OptimizationProblemImplementation::getBounds() const
{
  return bounds_;
}

void OptimizationProblemImplementation::setBounds(const Interval & bounds)
{
  bounds_ = bounds;
}

Bool OptimizationProblemImplementation::hasBounds() const
{
  return bounds_.getDimension() == dimension_;
}

/* Dimension accessor */
UnsignedInteger OptimizationProblemImplementation::getDimension() const
{
  return dimension_;
}

/* String converter */
String OptimizationProblemImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << OptimizationProblemImplementation::GetClassName()
      << " objective=" << objective_
      << " equality constraint=" << (hasEqualityConstraint() ? equalityConstraint_.__repr__() : "none")
      << " inequality constraint=" << (hasInequalityConstraint() ? inequalityConstraint_.__repr__() : "none")
      << " bounds=" << (hasBounds() ? bounds_.__repr__() : "none")
      << " dimension=" << dimension_;
  return oss;
}

/* Method save() stores the object through the StorageManager */
void OptimizationProblemImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "objective_", objective_ );
  adv.saveAttribute( "equalityConstraint_", equalityConstraint_ );
  adv.saveAttribute( "inequalityConstraint_", inequalityConstraint_ );
  adv.saveAttribute( "bounds_", bounds_ );
  adv.saveAttribute( "dimension_", dimension_ );
}

/* Method load() reloads the object from the StorageManager */
void OptimizationProblemImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "objective_", objective_ );
  adv.loadAttribute( "equalityConstraint_", equalityConstraint_ );
  adv.loadAttribute( "inequalityConstraint_", inequalityConstraint_ );
  adv.loadAttribute( "bounds_", bounds_ );
  adv.loadAttribute( "dimension_", dimension_ );
}

END_NAMESPACE_OPENTURNS
