//                                               -*- C++ -*-
/**
 * @file  BoxCoxEvaluationImplementation.hxx
 * @brief Class for Box Cox function implementation
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

#ifndef OPENTURNS_BOXCOXEVALUATIONIMPLEMENTATION_HXX
#define OPENTURNS_BOXCOXEVALUATIONIMPLEMENTATION_HXX

#include "NumericalMathEvaluationImplementation.hxx"
#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class BoxCoxEvaluationImplementation
 *
 * This class offers an easy evaluation of the Box Cox function defined by :
 *  h(x) = \frac{x^\lambda - 1}{\lambda} for \lambda non zero, log(x) oherwise
 *  Care that x should be positive
 */

class OT_API BoxCoxEvaluationImplementation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  BoxCoxEvaluationImplementation();

  /** Parameter constructor */
  BoxCoxEvaluationImplementation(const NumericalPoint & lamda);

  BoxCoxEvaluationImplementation(const NumericalPoint & lamda,
                                 const NumericalPoint & shift);

  /** Virtual constructor */
  virtual BoxCoxEvaluationImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const BoxCoxEvaluationImplementation & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Operator () */
  using NumericalMathEvaluationImplementation::operator();
  NumericalPoint operator() (const NumericalPoint & inP) const;
  NumericalSample operator() (const NumericalSample & inS) const;

  /** Accessor for input point dimension */
  UnsignedInteger getInputDimension() const;

  /** Accessor for output point dimension */
  UnsignedInteger getOutputDimension() const;

  /** Accessor for the lambda point */
  NumericalPoint getLambda() const;

  /** Accessor for the shift */
  NumericalPoint getShift() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  struct ComputeSamplePolicy
  {
    const NumericalSample & input_;
    NumericalSample & output_;
    const BoxCoxEvaluationImplementation & evaluation_;

    ComputeSamplePolicy(const NumericalSample & input,
                        NumericalSample & output,
                        const BoxCoxEvaluationImplementation & evaluation)
      : input_(input)
      , output_(output)
      , evaluation_(evaluation)
    {}

    inline void operator()( const TBB::BlockedRange<UnsignedInteger> & r ) const
    {
      for (UnsignedInteger i = 0; i < evaluation_.getInputDimension(); ++i)
	{
	  const NumericalScalar lambdaI(evaluation_.getLambda()[i]);
	  const NumericalScalar shiftI(evaluation_.getShift()[i]);
	  if (lambdaI != 0.0) evaluateNonZeroLambda(r, i, shiftI, lambdaI);
	  else evaluateZeroLambda(r, i, shiftI);
	}
    }

    inline void evaluateZeroLambda( const TBB::BlockedRange<UnsignedInteger> & r, const UnsignedInteger dimension, const NumericalScalar shiftI ) const
    {
      for (UnsignedInteger i = r.begin(); i != r.end(); ++i) output_[i][dimension] = log(input_[i][dimension] + shiftI);
    }

    inline void evaluateNonZeroLambda( const TBB::BlockedRange<UnsignedInteger> & r, const UnsignedInteger dimension, const NumericalScalar shiftI, const NumericalScalar lambdaI ) const
    {
      for (UnsignedInteger i = r.begin(); i != r.end(); ++i) output_[i][dimension] = (pow(input_[i][dimension] + shiftI, lambdaI) - 1.0) / lambdaI;
    }
  }; /* end struct ComputeSamplePolicy */

  /** Lambda vector of the box cox transform */
  NumericalPoint lambda_;

  /** Shift vector of the box cox transform */
  NumericalPoint shift_;


}; /* class BoxCoxEvaluationImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_BOXCOXEVALUATIONIMPLEMENTATION_HXX */
