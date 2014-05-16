//                                               -*- C++ -*-
/**
 *  @file  KrigingAlgorithm.cxx
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

#include "KrigingAlgorithm.hxx"
#include "PersistentObjectFactory.hxx"
#include "LinearNumericalMathFunction.hxx"
#include "SpecFunc.hxx"
#include "KrigingEvaluation.hxx"
#include "KrigingGradient.hxx"
#include "CenteredFiniteDifferenceHessian.hxx"
#include "MethodBoundNumericalMathEvaluationImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(KrigingAlgorithm);

static Factory<KrigingAlgorithm> RegisteredFactory("KrigingAlgorithm");


/* Default constructor */
KrigingAlgorithm::KrigingAlgorithm()
  : MetaModelAlgorithm()
  , optimizerProvided_(false)
  , outputIndex_(0)
{
  // Nothing to do
}


/* Constructor */
KrigingAlgorithm::KrigingAlgorithm(const NumericalSample & inputSample,
                                   const NumericalSample & outputSample,
                                   const Basis & basis,
                                   const CovarianceModel & covarianceModel)
  : MetaModelAlgorithm()
  , inputSample_(inputSample)
  , outputSample_(outputSample)
  , basis_(basis)
  , covarianceModel_(covarianceModel)
  , optimizerProvided_(false)
  , outputIndex_(0)
{
  if (inputSample_.getSize() != outputSample_.getSize())
    throw InvalidArgumentException(HERE) << "Input sample size (" << inputSample_.getSize() << ") does not match output sample size (" << outputSample_.getSize() << ").";

  if (inputSample_.getDimension() != covarianceModel.getDimension())
    throw InvalidArgumentException(HERE) << "Input sample dimension (" << inputSample_.getDimension() << ") does not match covariance model dimension (" << covarianceModel.getDimension() << ").";
  
  // normalize the sample
  UnsignedInteger dimension(inputSample.getDimension());
  NumericalPoint mean(inputSample.computeMean());
  NumericalPoint stdev(inputSample.computeStandardDeviationPerComponent());
  SquareMatrix linear(dimension);
  for (UnsignedInteger j = 0; j < dimension; ++ j)
  {
    linear(j, j) = 1.0;
    if (fabs(stdev[j]) > SpecFunc::MinNumericalScalar) linear(j, j) /= stdev[j];
  }
  NumericalPoint zero(dimension);
  setInputTransformation(LinearNumericalMathFunction(mean, zero, linear));
}


/* Constructor */
KrigingAlgorithm::KrigingAlgorithm(const NumericalSample & inputSample,
                                   const Distribution & inputDistribution,
                                   const NumericalSample & outputSample,
                                   const Basis & basis,
                                   const CovarianceModel & covarianceModel)
  : MetaModelAlgorithm()
  , inputSample_(inputSample)
  , outputSample_(outputSample)
  , basis_(basis)
  , covarianceModel_(covarianceModel)
  , optimizerProvided_(false)
  , outputIndex_(0)
{
  if (inputSample_.getSize() != outputSample_.getSize())
    throw InvalidArgumentException(HERE) << "Input sample size (" << inputSample_.getSize() << ") does not match output sample size (" << outputSample_.getSize() << ").";

  if (inputSample_.getDimension() != covarianceModel.getDimension())
    throw InvalidArgumentException(HERE) << "Input sample dimension (" << inputSample_.getDimension() << ") does not match covariance model dimension (" << covarianceModel.getDimension() << ").";
  
  // use the isoprobabilistic transformation
  setInputTransformation(inputDistribution.getIsoProbabilisticTransformation());
}


/* Perform regression */
void KrigingAlgorithm::run()
{
  const UnsignedInteger size(inputSample_.getSize());
  const UnsignedInteger basisSize(basis_.getSize());
  normalizedInputSample_ = inputTransformation_(inputSample_);

  const UnsignedInteger outputDimension(outputSample_.getDimension());
  NumericalMathFunction::NumericalMathFunctionCollection processEvaluationCollection(outputDimension);
  //  NumericalSample allBeta(outputDimension, size);
  //  NumericalSample allGamma(outputDimension, basisSize);
  //  NumericalSample allParameters(outputDimension, covarianceModel_.getParameters());
  // Compute F
  F_ = Matrix(size, basisSize);
  for (UnsignedInteger j = 0; j < basisSize; ++ j )
  {
    const NumericalSample basisSample(basis_[j](normalizedInputSample_));
    for ( UnsignedInteger i = 0; i < size; ++ i ) F_(i, j) = basisSample[i][0];
  }
  // For each output component, build the associated kriging model
  for (outputIndex_ = 0; outputIndex_ < outputDimension; ++ outputIndex_)
  {
    NumericalPointWithDescription covarianceModelParameters;
    // Here we compute both the meta-parameters and the kriging coefficients
    if (optimizerProvided_) covarianceModelParameters = optimizeLikelihood();
    else
    {
      // Here we simply read the meta-parameters
      covarianceModelParameters = covarianceModel_.getParameters();
      //      allParameters[outputIndex_] = covarianceModel_.getParameters();
      // And we compute the kriging coefficients
      computeLogLikelihood(covarianceModelParameters);
    }
    conditionalCovarianceModel_ = covarianceModel_;
    conditionalCovarianceModel_.setParameters(covarianceModelParameters);
    //    allBeta[outputIndex_] = beta_;
    //    allGamma[outputIndex_] = gamma_;
    NumericalMathFunction processEvaluation;
    processEvaluation.setEvaluationImplementation(new KrigingEvaluation(basis_, normalizedInputSample_, conditionalCovarianceModel_, beta_, gamma_));
    processEvaluation.setGradientImplementation(new KrigingGradient(basis_, normalizedInputSample_, conditionalCovarianceModel_, beta_, gamma_));
    processEvaluation.setHessianImplementation(new CenteredFiniteDifferenceHessian(ResourceMap::GetAsNumericalScalar( "CenteredFiniteDifferenceGradient-DefaultEpsilon" ), processEvaluation.getEvaluationImplementation()));
    processEvaluationCollection[outputIndex_] = processEvaluation;
  }

  NumericalMathFunction metaModel;
  if (outputDimension > 1) metaModel = NumericalMathFunction(NumericalMathFunction(processEvaluationCollection), inputTransformation_);
  else metaModel = NumericalMathFunction(processEvaluationCollection[0], inputTransformation_);

  // compute residual, relative error
  NumericalPoint residuals(outputDimension);
  NumericalPoint relativeErrors(outputDimension);
  const NumericalSample mY(metaModel(inputSample_));
  const NumericalPoint outputVariance(outputSample_.computeVariancePerComponent());

  for ( UnsignedInteger outputIndex = 0; outputIndex < outputDimension; ++ outputIndex )
  {
    NumericalScalar quadraticResidual(0.0);
    for ( UnsignedInteger i = 0; i < size; ++ i )
    {
      const NumericalScalar slack(outputSample_[i][outputIndex] - mY[i][outputIndex]);
      quadraticResidual += slack * slack;
    }
    residuals[outputIndex] = sqrt( quadraticResidual ) / size;
    const NumericalScalar empiricalError(quadraticResidual / size);
    relativeErrors[outputIndex] = empiricalError / outputVariance[outputIndex];
  }

  result_ = MetaModelResult(NumericalMathFunction(), metaModel, residuals, relativeErrors);
  //  result_ = KrigingResult(basis_, normalizedInputSample_, conditionalCovarianceModel_, allBeta, allGamma, F_, metaModel, residuals, relativeErrors);
}


NumericalScalar KrigingAlgorithm::computeLogLikelihood(const NumericalPoint & theta) const
{
  if (theta.getSize() != covarianceModel_.getParameters().getSize())
    throw InvalidArgumentException(HERE) << "Error; Could not compute likelihood"
                                         << " the covariance model requires arguments of size" << covarianceModel_.getParameters().getSize()
                                         << " here we got " << theta.getSize();
  CovarianceModel cov(covarianceModel_);
  cov.setParameters(theta);

  const UnsignedInteger size(inputSample_.getSize());

  CovarianceMatrix R(size);
  for ( UnsignedInteger i = 0; i < size; ++ i )
    for ( UnsignedInteger j = 0; j < i; ++ j )
      R(i, j) = cov.operator()(normalizedInputSample_[i], normalizedInputSample_[j])(0, 0);

  Bool continuationCondition(true);
  const NumericalScalar startingScaling(ResourceMap::GetAsNumericalScalar("KrigingAlgorithm-StartingScaling"));
  const NumericalScalar maximalScaling(ResourceMap::GetAsNumericalScalar("KrigingAlgorithm-MaximalScaling"));
  NumericalScalar cumulatedScaling(0.0);
  NumericalScalar scaling(startingScaling);
  TriangularMatrix C;
  while (continuationCondition && (cumulatedScaling < maximalScaling))
  {
    try
    {
      // C_ = R.computeCholesky().getImplementation();
      C = R.computeCholesky().getImplementation();
      continuationCondition = false;
    }
    // If it has not yet been computed, compute it and store it
    catch (InternalException & ex)
    {
      cumulatedScaling += scaling ;
      // Unroll the regularization to optimize the computation
      for (UnsignedInteger i = 0; i < size; ++i) R(i, i) += scaling;
      scaling *= 2.0;
    }
  }
  if (scaling >= maximalScaling)
    throw InvalidArgumentException(HERE) << "Error; Could not compute the Cholesky factor"
                                         << " Scaling up to "  << cumulatedScaling << " was not enough";
  if (cumulatedScaling > 0.0)
    LOGWARN(OSS() <<  "Warning! Scaling up to "  << cumulatedScaling << " was needed in order to get an admissible covariance. ");

  Matrix Ft(C.solveLinearSystem(F_));
  NumericalPoint y(size);
  for ( UnsignedInteger i = 0; i < size; ++ i ) y[i] = outputSample_[i][outputIndex_];
  NumericalPoint Yt(C.solveLinearSystem(y));

  Matrix G;
  Matrix Q(Ft.computeQR(G));
  NumericalPoint b(Q.transpose() * Yt);
  beta_ = G.solveLinearSystem(b);

  NumericalPoint rho(Yt - Ft * beta_);

  gamma_ = C.transpose().solveLinearSystem(rho);

  NumericalScalar sigma2(rho.normSquare() / size);
  NumericalScalar logDetR(0.0);
  for ( UnsignedInteger i = 0; i < size; ++ i ) logDetR += 2.0 * log(C(i, i)) / size;
  const NumericalScalar result(-(log(sigma2) + logDetR));

  return result;
}


NumericalPoint KrigingAlgorithm::optimizeLikelihood()
{
  // initial guess
  NumericalPoint theta(covarianceModel_.getParameters());
  NumericalScalar bestLikelihood(computeLogLikelihood(theta));

  BoundConstrainedAlgorithm optimizer(optimizer_);
  optimizer.setObjectiveFunction(bindMethod <KrigingAlgorithm, NumericalScalar, NumericalPoint> ( *this, &KrigingAlgorithm::computeLogLikelihood, theta.getSize(), 1 ));
  optimizer.setOptimizationProblem(BoundConstrainedAlgorithmImplementationResult::MAXIMIZATION);
  optimizer.setStartingPoint(theta);
  optimizer.run();

  // check result
  const NumericalScalar likelihood(optimizer.getResult().getOptimalValue());

  if (likelihood > bestLikelihood) theta = optimizer.getResult().getOptimizer();

  // the last optimized point is not necessarily the last evaluated, so update intermediate results
  computeLogLikelihood(theta);

  return theta;
}


void KrigingAlgorithm::setOptimizer(const BoundConstrainedAlgorithm& optimizer)
{
  optimizer_ = optimizer;
  optimizerProvided_ = true;
}


BoundConstrainedAlgorithm KrigingAlgorithm::getOptimizer() const
{
  return optimizer_;
}


void KrigingAlgorithm::setInputTransformation(const NumericalMathFunction& inputTransformation)
{
  if (inputTransformation.getInputDimension() != inputSample_.getDimension()) throw InvalidDimensionException(HERE)
        << "Input dimension (" << inputTransformation.getInputDimension() << ") should match sample dimension (" << inputSample_.getDimension() << ")";
  if (inputTransformation.getOutputDimension() != inputSample_.getDimension()) throw InvalidDimensionException(HERE)
        << "Output dimension (" << inputTransformation.getOutputDimension() << ") should match sample dimension (" << inputSample_.getDimension() << ")";
  inputTransformation_ = inputTransformation;
}

NumericalMathFunction KrigingAlgorithm::getInputTransformation() const
{
  return inputTransformation_;
}


/* Virtual constructor */
KrigingAlgorithm * KrigingAlgorithm::clone() const
{
  return new KrigingAlgorithm(*this);
}


/* String converter */
String KrigingAlgorithm::__repr__() const
{
  return OSS() << "class=" << getClassName();
}


NumericalSample KrigingAlgorithm::getInputSample() const
{
  return inputSample_;
}


NumericalSample KrigingAlgorithm::getOutputSample() const
{
  return outputSample_;;
}

CovarianceModel KrigingAlgorithm::getConditionalCovarianceModel() const
{
  return conditionalCovarianceModel_;
}


MetaModelResult KrigingAlgorithm::getResult()
{
  return result_;
}


/* Method save() stores the object through the StorageManager */
void KrigingAlgorithm::save(Advocate & adv) const
{
  MetaModelAlgorithm::save(adv);
  adv.saveAttribute( "inputSample_", inputSample_ );
  adv.saveAttribute( "inputTransformation_", inputTransformation_ );
  adv.saveAttribute( "outputSample_", outputSample_ );
  adv.saveAttribute( "basis_", basis_ );
  adv.saveAttribute( "covarianceModel_", covarianceModel_ );
  adv.saveAttribute( "optimizer_", optimizer_ );
  adv.saveAttribute( "optimizerProvided_", optimizerProvided_ );
  adv.saveAttribute( "conditionalCovarianceModel_", conditionalCovarianceModel_ );
  adv.saveAttribute( "result_", result_ );
}


/* Method load() reloads the object from the StorageManager */
void KrigingAlgorithm::load(Advocate & adv)
{
  MetaModelAlgorithm::load(adv);
  adv.loadAttribute( "inputSample_", inputSample_ );
  adv.loadAttribute( "inputTransformation_", inputTransformation_ );
  adv.loadAttribute( "outputSample_", outputSample_ );
  adv.loadAttribute( "basis_", basis_ );
  adv.loadAttribute( "covarianceModel_", covarianceModel_ );
  adv.loadAttribute( "optimizer_", optimizer_ );
  adv.loadAttribute( "optimizerProvided_", optimizerProvided_ );
  adv.loadAttribute( "conditionalCovarianceModel_", conditionalCovarianceModel_ );
  adv.loadAttribute( "result_", result_ );
}

END_NAMESPACE_OPENTURNS
