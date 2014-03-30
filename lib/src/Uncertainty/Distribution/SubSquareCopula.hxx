//                                               -*- C++ -*-
/**
 *  @file  SubSquareCopula.hxx
 *  @brief A class that implements a subSquare copula
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#ifndef OPENTURNS_SUBSQUARECOPULA_HXX
#define OPENTURNS_SUBSQUARECOPULA_HXX

#include "CopulaImplementation.hxx"
#include "CorrelationMatrix.hxx"
#include "SubSquare.hxx"
#include "DistFunc.hxx"
#include "TBB.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class SubSquareCopula
 *
 * The class implements a subSquare copula
 */
class SubSquareCopula
  : public CopulaImplementation
{
  CLASSNAME;

public:

  /** Default constructor */
  explicit SubSquareCopula(const UnsignedInteger dim = 1);

  /** Default constructor */
  explicit SubSquareCopula(const CorrelationMatrix & correlation);


  /** Comparison operator */
  Bool operator ==(const SubSquareCopula & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /* Interface inherited from Distribution */

  /** Virtual constructor */
  virtual SubSquareCopula * clone() const;

  /** Get one realization of the SubSquareCopula distribution */
  NumericalPoint getRealization() const;
protected:
  NumericalSample getSampleParallel(const UnsignedInteger size) const;
public:
  NumericalSample getSample(const UnsignedInteger size) const;

  /** Get the DDF of the SubSquareCopula distribution */
  using CopulaImplementation::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the SubSquareCopula distribution */
  using CopulaImplementation::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the SubSquareCopula distribution */
  using CopulaImplementation::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;
  using CopulaImplementation::computeSurvivalFunction;
  NumericalScalar computeSurvivalFunction(const NumericalPoint & point) const;

  /** Get the probability content of an interval */
  NumericalScalar computeProbability(const Interval & interval) const;

  /** Get the shape matrix of the copula */
  CorrelationMatrix getShapeMatrix() const;

  /** Get the Spearman correlation of the distribution */
  CorrelationMatrix getSpearmanCorrelation() const;

  /** Get the Kendall concordance of the distribution */
  CorrelationMatrix getKendallTau() const;

  /** Get the PDF gradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDF gradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Compute the PDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
  NumericalScalar computeConditionalPDF(const NumericalScalar x,
                                        const NumericalPoint & y) const;

  /** Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
  NumericalScalar computeConditionalCDF(const NumericalScalar x,
                                        const NumericalPoint & y) const;

  /** Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
  NumericalScalar computeConditionalQuantile(const NumericalScalar q,
      const NumericalPoint & y) const;

  /** Get the distribution of the marginal distribution corresponding to indices dimensions */
  using CopulaImplementation::getMarginal;
  Implementation getMarginal(const Indices & indices) const;

  /** Get the isoprobabilist transformation */
  IsoProbabilisticTransformation getIsoProbabilisticTransformation() const;

  /** Get the inverse isoprobabilist transformation */
  InverseIsoProbabilisticTransformation getInverseIsoProbabilisticTransformation() const;

  /** Tell if the distribution has independent copula */
  Bool hasIndependentCopula() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using CopulaImplementation::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /** Compute the correlation matrix of a SubSquare Copula from its Spearman correlation matrix */
  static CorrelationMatrix GetCorrelationFromSpearmanCorrelation(const CorrelationMatrix & matrix);

  /** Compute the correlation matrix of a SubSquare Copula from its Kendall correlation matrix */
  static CorrelationMatrix GetCorrelationFromKendallCorrelation(const CorrelationMatrix & matrix);

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:


private:

  struct ComputeSamplePolicy
  {
    const NumericalSample input_;
    NumericalSample & output_;
    UnsignedInteger dimension_;

    ComputeSamplePolicy( const NumericalSample & input,
                         NumericalSample & output)
      : input_(input)
      , output_(output)
      , dimension_(input.getDimension())
    {}

    inline void operator()( const TBB::BlockedRange<UnsignedInteger> & r ) const
    {
      for (UnsignedInteger i = r.begin(); i != r.end(); ++i)
        for (UnsignedInteger j = 0; j < dimension_; ++j)
          output_[i][j] = DistFunc::pSubSquare(input_[i][j]);
    }

  }; /* end struct ComputeSamplePolicy */

  // SubSquare copula parameter
  CorrelationMatrix correlation_;
  // Underlying generic representative
  SubSquare subSquare_;

}; /* class SubSquareCopula */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SUBSQUARECOPULA_HXX */
