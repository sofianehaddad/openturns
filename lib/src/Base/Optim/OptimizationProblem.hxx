//                                               -*- C++ -*-
/**
 *  @file  OptimizationProblem.hxx
 *  @brief OptimizationProblem allows to describe an optimization problem
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
#ifndef OPENTURNS_OPTIMIZATIONPROBLEM_HXX
#define OPENTURNS_OPTIMIZATIONPROBLEM_HXX

#include "OTprivate.hxx"
#include "TypedInterfaceObject.hxx"
#include "NumericalMathFunction.hxx"
#include "OptimizationProblemImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class OptimizationProblem
 * OptimizationProblem allows to define an optimization problem, including:
 * + multiple objective functions
 * + multiple equality, inequality and bounds constraints
 * + continuous or discrete (lattice) search spaces
 */

class OptimizationProblem
  : public TypedInterfaceObject<OptimizationProblemImplementation>
{

  CLASSNAME;
public:

  typedef Pointer<OptimizationProblemImplementation>   Implementation;
  typedef Collection< NumericalMathFunction >          NumericalMathFunctionCollection;

  /** Default constructor */
  OptimizationProblem();

  /** Constructor from an implementation */
  OptimizationProblem(const OptimizationProblemImplementation & implementation);

  /** Constructor from a Pointer to an implementation */
  OptimizationProblem(const Implementation & p_implementation);

  /** Constructor with parameters */
  OptimizationProblem(const NumericalMathFunctionCollection & objectives,
		      const NumericalMathFunctionCollection & equalityConstraints,
		      const NumericalMathFunctionCollection & inequalityConstraints,
		      const Interval & bounds);
  
  /** Objective functions accessor */
  NumericalMathFunction getObjective() const;
  void setObjective(const NumericalMathFunction & objective);
  NumericalMathFunctionCollection getObjectives() const;
  void setObjective(const NumericalMathFunctionCollection & objectives);

  /** Equality constraint functions accessor */
  NumericalMathFunctionCollection getEqualityConstraints() const;
  void setEqualityConstraints(const NumericalMathFunctionCollection & equalityConstraints);

  /** Inequality constraint functions accessor */
  NumericalMathFunctionCollection getInequalityConstraints() const;
  void setInequalityConstraints(const NumericalMathFunctionCollection & inequalityConstraints);

  /** Bounds accessor */
  Interval getBounds() const;
  void setBounds(const Interval & bounds);

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

} ; /* class OptimizationProblem */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_OPTIMIZATIONPROBLEM_HXX */
