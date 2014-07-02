//                                               -*- C++ -*-
/**
 * @file  InverseBoxCoxEvaluationImplementation.hxx
 * @brief Class for Inverse Box Cox function
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

#ifndef OPENTURNS_INVERSEBOXCOXEVALUATIONIMPLEMENTATION_HXX
#define OPENTURNS_INVERSEBOXCOXEVALUATIONIMPLEMENTATION_HXX

#include "NumericalMathEvaluationImplementation.hxx"
#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class InverseBoxCoxEvaluationImplementation
 *
 * This class offers an easy evaluation of the Box Cox function defined by :
 *  h(x) = \frac{x^\lambda - 1}{\lambda} for \lambda non zero, log(x) oherwise
 *  Care that x should be positive
 */

class OT_API InverseBoxCoxEvaluationImplementation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  InverseBoxCoxEvaluationImplementation();

  /** Parameter constructor */
  InverseBoxCoxEvaluationImplementation(const NumericalPoint & lamda);

  InverseBoxCoxEvaluationImplementation(const NumericalPoint & lamda,
                                        const NumericalPoint & shift);

  /** Virtual constructor */
  virtual InverseBoxCoxEvaluationImplementation * clone() const;

  /** Comparison operator */
  Bool operator ==(const InverseBoxCoxEvaluationImplementation & other) const;

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
    const InverseBoxCoxEvaluationImplementation & evaluation_;

    ComputeSamplePolicy(const NumericalSample & input,
                        NumericalSample & output,
                        const InverseBoxCoxEvaluationImplementation & evaluation)
      : input_(input)
      , output_(output)
      , evaluation_(evaluation)
    {}

    inline void operator()( const TBB::BlockedRange<UnsignedInteger> & r ) const
    {
      for (UnsignedInteger i = r.begin(); i != r.end(); ++i)
        {
          for (UnsignedInteger j = 0; j < evaluation_.getInputDimension(); ++j)
            {
              const NumericalScalar lambda_j(evaluation_.getLambda()[j]);
              const NumericalScalar x(input_[i][j] - evaluation_.getShift()[j]);
              if (std::abs(lambda_j * x * x) < 1e-8) output_[i][j] = exp(x) * (1.0 - 0.5 * lambda_j * x * x);
              else output_[i][j] = pow(lambda_j * x + 1.0, 1.0 / lambda_j);
            } // j
        } // i
    } // operator ()
  }; /* end struct ComputeSamplePolicy */

  /** lambda vector of the box cox transform */
  NumericalPoint lambda_;

  /** Shift vector of the box cox transform */
  NumericalPoint shift_;


}; /* class InverseBoxCoxEvaluationImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_INVERSEBOXCOXEVALUATIONIMPLEMENTATION_HXX */
