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
#include "SpecFunc.hxx"

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
    {
      // Nothing to do
    }

    inline void operator()( const TBB::BlockedRange<UnsignedInteger> & r ) const
    {
      for (UnsignedInteger i = r.begin(); i != r.end(); ++i)
        {
          for (UnsignedInteger j = 0; j < evaluation_.getInputDimension(); ++j)
            {
              const NumericalScalar lambda_j(evaluation_.getLambda()[j]);
              const NumericalScalar logX(log(input_[i][j] + evaluation_.getShift()[j]));
              if (std::abs(lambda_j * logX) < 1e-8) output_[i][j] = logX * (1.0 + 0.5 * lambda_j * logX);
              else output_[i][j] = expm1(lambda_j * logX) / lambda_j;
            } // j
        } // i
    } // operator ()
  };  // struct ComputeSamplePolicy

  /** Lambda vector of the box cox transform */
  NumericalPoint lambda_;

  /** Shift vector of the box cox transform */
  NumericalPoint shift_;


}; /* class BoxCoxEvaluationImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_BOXCOXEVALUATIONIMPLEMENTATION_HXX */
