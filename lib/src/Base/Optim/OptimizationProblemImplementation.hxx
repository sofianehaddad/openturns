//                                               -*- C++ -*-
/**
 *  @file  OptimizationProblemImplementation.hxx
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
#ifndef OPENTURNS_OPTIMIZATIONPROBLEMIMPLEMENTATION_HXX
#define OPENTURNS_OPTIMIZATIONPROBLEMIMPLEMENTATION_HXX

#include "OTprivate.hxx"
#include "PersistentObject.hxx"
#include "NumericalMathFunction.hxx"
#include "Interval.hxx"
#include "OptimizationProblemImplementationResult.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class OptimizationProblemImplementation
 * OptimizationProblemImplementation implements an algorithm for finding the
 * point the nearest to the origin of an implicitely defined manifold
 */

class OptimizationProblemImplementation
  : public PersistentObject
{

  CLASSNAME;

public:

  /** Default constructor */
  OptimizationProblemImplementation();

  /** Constructor with parameters */
  OptimizationProblemImplementation(const NumericalMathFunction & objective,
                                    const NumericalMathFunction & equalityConstraint,
                                    const NumericalMathFunction & inequalityConstraint,
                                    const Interval & bounds);

  /** Objective functions accessor */
  NumericalMathFunction getObjective() const;
  void setObjective(const NumericalMathFunction & objective);

  /** Equality constraint function accessor */
  NumericalMathFunction getEqualityConstraint() const;
  void setEqualityConstraint(const NumericalMathFunction & equalityConstraint);

  /** Inequality constraint function accessor */
  NumericalMathFunction getInequalityConstraint() const;
  void setInequalityConstraint(const NumericalMathFunction & inequalityConstraint);

  /** Bounds accessor */
  Interval getBounds() const;
  void setBounds(const Interval & bounds);

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

private:
  // The objective function
  NumericalMathFunction objective_;

  // The equality constraint function
  NumericalMathFunction equalityConstraint_;

  // The inequality constraint function
  NumericalMathFunction inequalityConstraint_;

  // The bounds
  Interval bounds_;

  // The dimension of the search space
  UnsignedInteger dimension_;
} ; /* class OptimizationProblemImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_OPTIMIZATIONPROBLEMIMPLEMENTATION_HXX */
