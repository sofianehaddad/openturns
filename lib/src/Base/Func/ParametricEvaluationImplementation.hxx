//                                               -*- C++ -*-
/**
 *  @file  ParametricEvaluationImplementation.hxx
 *  @brief ParametricEvaluationImplementation
 *
 *  Copyright (C) 2005-2012 EDF-EADS-Phimeca
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License.
 *
 *  This library is distributed in the hope that it will be useful
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 *  @author: schueller
 */
#ifndef OPENTURNS_PARAMETRICEVALUATIONIMPLEMENTATION_HXX
#define OPENTURNS_PARAMETRICEVALUATIONIMPLEMENTATION_HXX

#include "NumericalMathEvaluationImplementation.hxx"
#include "StorageManager.hxx"
#include "Indices.hxx"
#include "NumericalMathFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ParametricEvaluationImplementation
 *
 * ParametricEvaluationImplementation implements the concept of parametric function for its evaluation part
 */
class ParametricEvaluationImplementation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;

public:
  /** Default constructor */
  ParametricEvaluationImplementation();

  /** Parameters constructor */
  ParametricEvaluationImplementation(const NumericalMathFunction & function,
                                     const Indices & set,
				     const NumericalPoint & referencePoint,
				     const Bool parametersSet = true);

  /** Virtual constructor method */
  ParametricEvaluationImplementation * clone() const;

  /** Evaluation operator */
  using NumericalMathEvaluationImplementation::operator();
  NumericalPoint operator() (const NumericalPoint & point) const;

  /** Parameters positions accessor */
  Indices getParametersPositions() const;

  /** Input positions accessor */
  Indices getInputPositions() const;

  /** Function accessor */
  NumericalMathFunction getFunction() const;

  /** Dimension accessor */
  UnsignedInteger getInputDimension() const;
  UnsignedInteger getParametersDimension() const;
  UnsignedInteger getOutputDimension() const;

  /** Parameters accessor */
  using NumericalMathEvaluationImplementation::setParameters;
  void setParameters(const NumericalPointWithDescription & parameters);
  void setParameters(const NumericalPoint & parameters);

  /** String converter */
  String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

private:

  friend class ParametricGradientImplementation;
  friend class ParametricHessianImplementation;

  /** The underlying function */
  NumericalMathFunction function_;

  /** The position of the parameters */
  Indices parametersPositions_;

  /** The position of the input */
  Indices inputPositions_;

}; /* class ParametricEvaluationImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PARAMETRICEVALUATIONIMPLEMENTATION_HXX */
