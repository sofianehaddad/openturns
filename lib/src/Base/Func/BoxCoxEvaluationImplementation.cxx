//                                               -*- C++ -*-
/**
 * @file  BoxCoxEvaluationImplementation.cxx
 * @brief Class for a Box cox implementation
 *
 * Copyright (C) 2005-2014 Airbus-EDF-Phimeca
 *
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 *
 * @author schueller
 * @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */

#include "BoxCoxEvaluationImplementation.hxx"
#include "PersistentObjectFactory.hxx"
#include "TBB.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(BoxCoxEvaluationImplementation);

static Factory<BoxCoxEvaluationImplementation> RegisteredFactory("BoxCoxEvaluationImplementation");

/* Default constructor */
BoxCoxEvaluationImplementation::BoxCoxEvaluationImplementation()
  : NumericalMathEvaluationImplementation()
{
  // Nothing to do
}

/* Parameter constructor */
BoxCoxEvaluationImplementation::BoxCoxEvaluationImplementation(const NumericalPoint & lambda)
  : NumericalMathEvaluationImplementation()
  , lambda_(lambda)
  , shift_(lambda.getDimension())
{
  setInputDescription(Description::BuildDefault(lambda_.getSize(), "x"));
  setOutputDescription(Description::BuildDefault(lambda_.getSize(), "y"));
}

BoxCoxEvaluationImplementation::BoxCoxEvaluationImplementation(const NumericalPoint & lambda,
    const NumericalPoint & shift)
  : NumericalMathEvaluationImplementation()
  , lambda_(lambda)
  , shift_(shift)
{
  if (lambda.getDimension() != shift.getDimension()) throw InvalidArgumentException(HERE) << "Error: the given exponent vector has a dimension=" << lambda.getDimension() << " different from the shift dimension=" << shift.getDimension();
  setInputDescription(Description::BuildDefault(lambda_.getSize(), "x"));
  setOutputDescription(Description::BuildDefault(lambda_.getSize(), "y"));
}

/* Clone constructor */
BoxCoxEvaluationImplementation * BoxCoxEvaluationImplementation::clone() const
{
  return new BoxCoxEvaluationImplementation(*this);
}

/* Comparison operator */
Bool BoxCoxEvaluationImplementation::operator ==(const BoxCoxEvaluationImplementation & other) const
{
  if (this == &other) return true;
  return (lambda_ == other.lambda_) && (shift_ == other.shift_);
}

/* String converter */
String BoxCoxEvaluationImplementation::__repr__() const
{
  OSS oss(true);
  oss << "class=" << BoxCoxEvaluationImplementation::GetClassName()
      << " name=" << getName()
      << " dimension=" << getInputDimension()
      << " lambda=" << lambda_.__repr__()
      << " shift=" << shift_.__repr__();
  return oss;
}

/* String converter __str__ */
String BoxCoxEvaluationImplementation::__str__(const String & offset) const
{
  OSS oss (false);
  oss << "BoxCox(lambda=" << lambda_
      << ", shift=" << shift_
      << ")";
  return oss;
}

/* Accessor for the lambda */
NumericalPoint BoxCoxEvaluationImplementation::getLambda() const
{
  return lambda_;
}

/* Accessor for the shift */
NumericalPoint BoxCoxEvaluationImplementation::getShift() const
{
  return shift_;
}

/* Operator () */
NumericalSample BoxCoxEvaluationImplementation::operator() (const NumericalSample & inS) const
{
  if (inS.getDimension() != getInputDimension()) throw InvalidArgumentException(HERE) << "Error: the given sample has an invalid dimension. Expect a dimension " << getInputDimension() << ", got " << inS.getDimension();
  const UnsignedInteger size(inS.getSize());
  NumericalSample result(size, getInputDimension());
  const BoxCoxEvaluationImplementation::ComputeSamplePolicy policy( inS, result, *this );
  TBB::ParallelFor( 0, size, policy );
  callsNumber_ += size;
  if (isHistoryEnabled_)
  {
    inputStrategy_.store(inS);
    outputStrategy_.store(result);
  }
  result.setDescription(getOutputDescription());
  return result;
}

/* Operator () */
NumericalPoint BoxCoxEvaluationImplementation::operator() (const NumericalPoint & inP) const
{
  const UnsignedInteger dimension(getInputDimension());
  if (inP.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << dimension << ", got " << inP.getDimension();
  NumericalPoint result(lambda_.getDimension());

  // There is no check of positive variables
  // This last one must be done by user or, as the evaluation is used in a stochastic context, in the BoxCoxTransform class
  for (UnsignedInteger index = 0; index < dimension; ++index)
  {
    const NumericalScalar x(inP[index] + shift_[index]);
    if (x <= 0.0)
      throw InvalidArgumentException(HERE) << "Can not apply the Box Cox evaluation function to a negative shifted value x=" << x;

    // Applying the Box-Cox function
    const NumericalScalar lambda_i(lambda_[index]);
    if (lambda_i == 0) result[index] = log(x);
    else result[index] = ((pow(x, lambda_i) - 1.0) / lambda_i);
  }
  ++callsNumber_;
  if (isHistoryEnabled_)
  {
    inputStrategy_.store(inP);
    outputStrategy_.store(result);
  }
  return result;
}

/* Accessor for input point dimension */
UnsignedInteger BoxCoxEvaluationImplementation::getInputDimension() const
{
  return lambda_.getDimension();
}

/* Accessor for output point dimension */
UnsignedInteger BoxCoxEvaluationImplementation::getOutputDimension() const
{
  return lambda_.getDimension();
}

/* Method save() stores the object through the StorageManager */
void BoxCoxEvaluationImplementation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
  adv.saveAttribute( "lambda_", lambda_ );
  adv.saveAttribute( "shift_", shift_ );
}

/* Method load() reloads the object from the StorageManager */
void BoxCoxEvaluationImplementation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load(adv);
  adv.loadAttribute( "lambda_", lambda_ );
  adv.loadAttribute( "shift_", shift_ );
}

END_NAMESPACE_OPENTURNS
