//                                               -*- C++ -*-
/**
 *  @file  RosenblattEvaluation.hxx
 *  @brief Class for the Nataf transformationEvaluation evaluation for elliptical
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
 *  @author lebrun
 *  @date   2008-07-03 08:31:34 +0200 (jeu, 03 jui 2008)
 */
#ifndef OPENTURNS_ROSENBLATTEVALUATION_HXX
#define OPENTURNS_ROSENBLATTEVALUATION_HXX

#include "OTprivate.hxx"
#include "NumericalMathEvaluationImplementation.hxx"
#include "Matrix.hxx"
#include "NumericalPoint.hxx"
#include "Distribution.hxx"
#include "StorageManager.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class RosenblattEvaluation
 *
 * This class offers an interface for the Nataf function for elliptical distributions
 */
class RosenblattEvaluation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  RosenblattEvaluation();

  /** Parameter constructor */
  explicit RosenblattEvaluation(const Distribution & distribution);

  /** Virtual constructor */
  virtual RosenblattEvaluation * clone() const;

  /** Evaluation */
  NumericalPoint operator () (const NumericalPoint & inP) const;

  /** Gradient according to the marginal parameters */
  virtual Matrix parametersGradient(const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  virtual UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  virtual UnsignedLong getOutputDimension() const;

  /** String converter */
  virtual String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:


private:
  Distribution distribution_;

}; /* RosenblattEvaluation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ROSENBLATTEVALUATION_HXX */
