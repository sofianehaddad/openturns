//                                               -*- C++ -*-
/**
 *  @brief The evaluation part of an aggregation of functions from R^n to R^p_1,...,R^n to R^p_k
 *
 *  Copyright 2005-2025 Airbus-EDF-IMACS-ONERA-Phimeca
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
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef OPENTURNS_AGGREGATEDEVALUATION_HXX
#define OPENTURNS_AGGREGATEDEVALUATION_HXX

#include "openturns/EvaluationImplementation.hxx"
#include "openturns/Collection.hxx"
#include "openturns/PersistentCollection.hxx"
#include "openturns/Function.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class AggregatedEvaluation
 *
 * The evaluation part of an aggregation of functions from R^n to R^p_1,...,R^n to R^p_k
 */

class OT_API AggregatedEvaluation
  : public EvaluationImplementation
{
  CLASSNAME
public:

  typedef Collection<Function>                              FunctionCollection;
  typedef PersistentCollection<Function>                    FunctionPersistentCollection;

  /** Default constructor */
  AggregatedEvaluation();

  /** Parameter constructor */
  explicit AggregatedEvaluation(const FunctionCollection & functionsCollection);

  /** Virtual constructor */
  AggregatedEvaluation * clone() const override;

  /** String converter */
  String __repr__() const override;
  String __str__(const String & offset = "") const override;

  /** Evaluation operator */
  Point operator () (const Point & inP) const override;
  Sample operator () (const Sample & inS) const override;

  /** Functions accessor */
  FunctionCollection getFunctionsCollection() const;
  void setFunctionsCollection(const FunctionCollection & functionsCollection);

  /** Get the i-th marginal function */
  Evaluation getMarginal(const UnsignedInteger i) const override;

  /** Get the function corresponding to indices components */
  Evaluation getMarginal(const Indices & indices) const override;

  /** Input dimension accessor */
  UnsignedInteger getInputDimension() const override;

  /** Output dimension accessor */
  UnsignedInteger getOutputDimension() const override;

  /** Gradient according to the marginal parameters */
  Matrix parameterGradient(const Point & inP) const override;

  /** Parameters value accessor */
  void setParameter(const Point & parameter) override;
  Point getParameter() const override;

  /** Parameters description accessor */
  Description getParameterDescription() const override;

  /** Linearity accessors */
  Bool isLinear() const override;
  Bool isLinearlyDependent(const UnsignedInteger index) const override;

  /** Is it safe to call in parallel? */
  Bool isParallel() const override;

  /** Stop callback */
  typedef Bool (*StopCallback)(void * state);
  void setStopCallback(StopCallback callBack, void * state = nullptr) override;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const override;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv) override;

private:

  // Make the gradient and the hessian friend classes of the evaluation in order to share the functions and the coefficients
  friend class AggregatedGradient;
  friend class AggregatedHessian;

  // The functions to be aggregated
  FunctionPersistentCollection functionsCollection_;

  // The output dimension
  UnsignedInteger outputDimension_ = 0;

} ; /* class AggregatedEvaluation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_AGGREGATEDEVALUATION_HXX */
