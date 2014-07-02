//                                               -*- C++ -*-
/**
 *  @file  KrigingAlgorithm.hxx
 *  @brief The class building gaussian process regression
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
 */
#ifndef OPENTURNS_KRIGINGALGORITHM_HXX
#define OPENTURNS_KRIGINGALGORITHM_HXX

#include "MetaModelAlgorithm.hxx"
#include "Basis.hxx"
#include "CovarianceModel.hxx"
#include "BoundConstrainedAlgorithm.hxx"
#include "TriangularMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class KrigingAlgorithm
 *
 * The class building chaos expansions
 */

class OT_API KrigingAlgorithm
  : public MetaModelAlgorithm
{
  CLASSNAME;

public:

  /** Default constructor */
  KrigingAlgorithm();

  /** Constructor */
  KrigingAlgorithm (const NumericalSample & inputSample,
                    const NumericalSample & outputSample,
                    const Basis & basis,
                    const CovarianceModel & covarianceModel,
		    const Bool normalize = true);

  /** Constructor */
  KrigingAlgorithm (const NumericalSample & inputSample,
                    const Distribution & inputDistribution,
                    const NumericalSample & outputSample,
                    const Basis & basis,
                    const CovarianceModel & covarianceModel);

  /** Virtual constructor */
  virtual KrigingAlgorithm * clone() const;

  /** String converter */
  virtual String __repr__() const;

  /** likelihood optimization algorithm */
  void setOptimizer(const BoundConstrainedAlgorithm & optimizer);
  BoundConstrainedAlgorithm getOptimizer() const;

  /** Perform regression */
  void run();

  /** input transformation accessor */
  void setInputTransformation(const NumericalMathFunction & inputTransformation);
  NumericalMathFunction getInputTransformation() const;

  /** conditional covariance model accessor */
  CovarianceModel getConditionalCovarianceModel() const;

  /** Sample accessors */
  virtual NumericalSample getInputSample() const;
  virtual NumericalSample getOutputSample() const;

  /** result accessor */
  virtual MetaModelResult getResult();

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);


protected:
  NumericalPoint optimizeLikelihood();
  NumericalScalar computeLogLikelihood(const NumericalPoint & theta) const;

  friend class Factory<KrigingAlgorithm>;

private:

  // The input data
  NumericalSample inputSample_;
  // Standardized version of the input data
  NumericalSample normalizedInputSample_;
  // Standardization funtion
  NumericalMathFunction inputTransformation_;
  // The associated output data
  NumericalSample outputSample_;
  // The basis used for the trend modeling
  Basis basis_;
  // The covariance model parametric family
  CovarianceModel covarianceModel_;
  // The member of the covariance model fitted to the data
  CovarianceModel conditionalCovarianceModel_;
  // The optimization algorithm used for the meta-parameters estimation
  BoundConstrainedAlgorithm optimizer_;
  Bool optimizerProvided_;

  // Attributes used to exchange data between the different methods
  // The current output index
  mutable UnsignedInteger outputIndex_;
  // The coefficients of the current output conditional expectation part
  mutable NumericalPoint beta_;
  // The coefficients of the current output deterministic trend
  mutable NumericalPoint gamma_;
  // The current output Cholesky factor of the covariance matrix
  //  mutable TriangularMatrix C_;
  // The current output Gram matrix
  mutable Matrix F_;

  /** Result */
  MetaModelResult result_;

}; // class KrigingAlgorithm


END_NAMESPACE_OPENTURNS

#endif
