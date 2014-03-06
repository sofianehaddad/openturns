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

#include "KrigingEvaluation.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(KrigingEvaluation);


static Factory<KrigingEvaluation> RegisteredFactory("KrigingEvaluation");


/* Constructor with parameters */
KrigingEvaluation::KrigingEvaluation()
  : NumericalMathEvaluationImplementation()
{
  // Nothing to do
}


/* Constructor with parameters */
KrigingEvaluation::KrigingEvaluation (const Basis & basis,
                                      const NumericalSample & inputSample,
                                      const CovarianceModel & covarianceModel,
                                      const NumericalPoint & beta,
                                      const NumericalPoint & gamma)
  : NumericalMathEvaluationImplementation()
  , basis_(basis)
  , inputSample_(inputSample)
  , covarianceModel_(covarianceModel)
  , beta_(beta)
  , gamma_(gamma)
{
  setInputDescription(BuildDefaultDescription(getInputDimension(), "x"));
  setOutputDescription(BuildDefaultDescription(getOutputDimension(), "y"));
  setParameters(NumericalPointWithDescription(getInputDimension()));
}


/* Virtual constructor */
KrigingEvaluation * KrigingEvaluation::clone() const
{
  return new KrigingEvaluation(*this);
}

/* Comparison operator */
Bool KrigingEvaluation::operator==(const KrigingEvaluation & other) const
{
  return true;
}

/* String converter */
String KrigingEvaluation::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName()
      << " correlationModel=" << covarianceModel_
      << " beta=" << beta_
      << " gamma=" << gamma_;
  return oss;
}

/* String converter */
String KrigingEvaluation::__str__(const String & offset) const
{
  return OSS(false) << offset << GetClassName();
}

/* Test for actual implementation */
Bool KrigingEvaluation::isActualImplementation() const
{
  return true;
}

/* Operator () */
NumericalPoint KrigingEvaluation::operator()(const NumericalPoint & inP) const
{
  //  return operator()(NumericalSample(1, inP))[0];
  const UnsignedLong trainingSize(inputSample_.getSize());
  // Evaluate the kernel part in parallel
  KrigingEvaluationPointFunctor functor( inP, *this );
  TBB::ParallelReduce( 0, trainingSize, functor );
  NumericalScalar value(functor.accumulator_);
  // Evaluate the basis part sequentially
  const UnsignedLong basisSize(basis_.getSize());
  for (UnsignedLong i = 0; i < basisSize; ++i) value += basis_[i](inP)[0] * beta_[i];
  ++callsNumber_;
  return NumericalPoint(1, value);
}

NumericalSample KrigingEvaluation::operator()(const NumericalSample & inS) const
{
  const UnsignedLong size(inS.getSize());
  NumericalSample result(size, getOutputDimension());
  const KrigingEvaluationSampleFunctor functor( inS, result, *this );
  TBB::ParallelFor( 0, size, functor );
  callsNumber_ += size;
  return result;
}


/* Accessor for input point dimension */
UnsignedLong KrigingEvaluation::getInputDimension() const
{
  return basis_.getDimension();
}

/* Accessor for output point dimension */
UnsignedLong KrigingEvaluation::getOutputDimension() const
{
  return 1;
}

/* Method save() stores the object through the StorageManager */
void KrigingEvaluation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
  adv.saveAttribute("basis_", basis_);
  adv.saveAttribute("inputSample_", inputSample_);
  adv.saveAttribute("covarianceModel_", covarianceModel_);
  adv.saveAttribute("beta_", beta_);
  adv.saveAttribute("gamma_", gamma_);
}

/* Method load() reloads the object from the StorageManager */
void KrigingEvaluation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load(adv);
  adv.loadAttribute("basis_", basis_);
  adv.loadAttribute("inputSample_", inputSample_);
  adv.loadAttribute("covarianceModel_", covarianceModel_);
  adv.loadAttribute("beta_", beta_);
  adv.loadAttribute("gamma_", gamma_);
}


END_NAMESPACE_OPENTURNS
