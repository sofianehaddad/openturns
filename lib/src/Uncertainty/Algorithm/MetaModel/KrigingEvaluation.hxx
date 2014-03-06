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
#ifndef OPENTURNS_KRIGINGEVALUATION_HXX
#define OPENTURNS_KRIGINGEVALUATION_HXX

#include "NumericalMathEvaluationImplementation.hxx"
#include "CovarianceModel.hxx"
#include "TBB.hxx"


BEGIN_NAMESPACE_OPENTURNS

/**
 * @class KrigingEvaluation
 *
 * This class permits prediction on a gaussian process
 */
class KrigingEvaluation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME

public:

  /** Default constructor */
  KrigingEvaluation();

  /** Constructor with parameters */
  KrigingEvaluation(const Basis & basis,
                    const NumericalSample & inputSample,
                    const CovarianceModel & correlationModel,
                    const NumericalPoint & beta,
                    const NumericalPoint & gamma);

  /** Virtual constructor */
  virtual KrigingEvaluation * clone() const;

  /** Comparison operator */
  Bool operator ==(const KrigingEvaluation & other) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Test for actual implementation */
  virtual Bool isActualImplementation() const;

  /** Operator () */
  virtual NumericalPoint operator()(const NumericalPoint & inP) const;
  virtual NumericalSample operator()(const NumericalSample & inS) const;

  /** Accessor for input point dimension */
  virtual UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  virtual UnsignedLong getOutputDimension() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  // Helper for the parallel version of the point-based evaluation operator
  struct KrigingEvaluationPointFunctor
  {
    const NumericalPoint & input_;
    const KrigingEvaluation & evaluation_;
    NumericalScalar accumulator_;

    KrigingEvaluationPointFunctor(const NumericalPoint & input,
                                  const KrigingEvaluation & evaluation)
      : input_(input)
      , evaluation_(evaluation)
      , accumulator_(0.0)
    {}

    KrigingEvaluationPointFunctor(const KrigingEvaluationPointFunctor & other,
                                  TBB::Split)
      : input_(other.input_)
      , evaluation_(other.evaluation_)
      , accumulator_(0.0)
    {}

    inline void operator()( const TBB::BlockedRange<UnsignedLong> & r )
    {
      for (UnsignedLong i = r.begin(); i != r.end(); ++i) accumulator_ += evaluation_.covarianceModel_(input_, evaluation_.inputSample_[i])(0, 0) * evaluation_.gamma_[i];
    } // operator()

    inline void join(const KrigingEvaluationPointFunctor & other)
    {
      accumulator_ += other.accumulator_;
    }

  }; // struct KrigingEvaluationPointFunctor

  // Helper for the parallel version of the sample-based evaluation operator
  struct KrigingEvaluationSampleFunctor
  {
    const NumericalSample & input_;
    NumericalSample & output_;
    const KrigingEvaluation & evaluation_;
    UnsignedLong trainingSize_;
    UnsignedLong basisSize_;

    KrigingEvaluationSampleFunctor(const NumericalSample & input,
                                   NumericalSample & output,
                                   const KrigingEvaluation & evaluation)
      : input_(input)
      , output_(output)
      , evaluation_(evaluation)
      , trainingSize_(evaluation.inputSample_.getSize())
      , basisSize_(evaluation.basis_.getSize())
    {}

    inline void operator()( const TBB::BlockedRange<UnsignedLong> & r ) const
    {
      const UnsignedLong start(r.begin());
      const UnsignedLong size(r.end() - start);
      Matrix R(size, trainingSize_);
      Matrix F(size, basisSize_);
      for (UnsignedLong i = 0; i != size; ++i)
      {
        for (UnsignedLong j = 0; j < trainingSize_; ++j)
          R(i, j) = evaluation_.covarianceModel_(input_[start + i], evaluation_.inputSample_[j])(0, 0);
        for (UnsignedLong j = 0; j < basisSize_; ++j)
          F(i, j) = evaluation_.basis_[j](input_[start + i])[0];
      }
      const NumericalPoint pointResult(F * evaluation_.beta_ + R * evaluation_.gamma_);
      for (UnsignedLong i = 0; i != size; ++i) output_[start + i][0] = pointResult[i];
    } // operator()
  }; // struct KrigingEvaluationSampleFunctor

  /// Basis
  Basis basis_;

  /// Training (input) sample
  NumericalSample inputSample_;

  /// Correlation model
  CovarianceModel covarianceModel_;

  /// Regression weights
  NumericalPoint beta_;
  NumericalPoint gamma_;

}; /* class KrigingEvaluation */



END_NAMESPACE_OPENTURNS

#endif
