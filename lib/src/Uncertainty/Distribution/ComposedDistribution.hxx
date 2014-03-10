//                                               -*- C++ -*-
/**
 *  @file  ComposedDistribution.hxx
 *  @brief The class that implements assembly distributions
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
#ifndef OPENTURNS_COMPOSEDDISTRIBUTION_HXX
#define OPENTURNS_COMPOSEDDISTRIBUTION_HXX

#include "Distribution.hxx"
#include "DistributionImplementation.hxx"
#include "PersistentCollection.hxx"
#include "Copula.hxx"
#include "TBB.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ComposedDistribution
 *
 * The class describes the probabilistic concept of distributions
 * made from marginal distributions and from a copula.
 */
class ComposedDistribution
  : public DistributionImplementation
{
  CLASSNAME;
public:

  /** A type for distribution collection */
  typedef Collection<Distribution>                 DistributionCollection;
  typedef PersistentCollection<Distribution>       DistributionPersistentCollection;

  /** Default constructor for save/load methods : 1D distribution with default Uniform marginal and IndependentCopula */
  ComposedDistribution();

  /** Default constructor, independent copula is supposed */
  explicit ComposedDistribution(const DistributionCollection & coll);

  /** Default constructor */
  explicit ComposedDistribution(const DistributionCollection & coll,
                                const Distribution & copula);


  /** Comparison operator */
  Bool operator ==(const ComposedDistribution & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;


  /** Distribution collection accessor */
  void setDistributionCollection(const DistributionCollection & coll);
  const DistributionCollection & getDistributionCollection() const;

  /** Copula accessor */
  void setCopula(const Distribution & copula);
  Implementation getCopula() const;



  /* Here is the interface that all derived class must implement */

  /** Virtual constructor */
  virtual ComposedDistribution * clone() const;

  /** Get one realization of the ComposedDistribution */
  NumericalPoint getRealization() const;
protected:
  NumericalSample getSampleParallel(const UnsignedInteger size) const;
public:
  NumericalSample getSample(const UnsignedInteger size) const;

  /** Get the DDF of the ComposedDistribution */
  using DistributionImplementation::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the ComposedDistribution */
  using DistributionImplementation::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the ComposedDistribution */
  using DistributionImplementation::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the survival function of the ComposedDistribution */
  using DistributionImplementation::computeSurvivalFunction;
  NumericalScalar computeSurvivalFunction(const NumericalPoint & point) const;

  /** Get the probability content of an interval */
  NumericalScalar computeProbability(const Interval & interval) const;

  /** Get the PDF gradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDF gradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the quantile of the distributionImplementation */
  NumericalPoint computeQuantile(const NumericalScalar prob,
                                 const Bool tail = false) const;

  /** Compute the PDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
  NumericalScalar computeConditionalPDF(const NumericalScalar x, const NumericalPoint & y) const;

  /** Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
  NumericalScalar computeConditionalCDF(const NumericalScalar x, const NumericalPoint & y) const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Get the i-th marginal distribution */
  Implementation getMarginal(const UnsignedInteger i) const;

  /** Get the distribution of the marginal distribution corresponding to indices dimensions */
  Implementation getMarginal(const Indices & indices) const;

  /** Get the isoprobabilist transformation */
  IsoProbabilisticTransformation getIsoProbabilisticTransformation() const;

  /** Get the inverse isoprobabilist transformation */
  InverseIsoProbabilisticTransformation getInverseIsoProbabilisticTransformation() const;

  /** Get the standard distribution */
  Implementation getStandardDistribution() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using DistributionImplementation::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /** Tell if the distribution has independent copula */
  Bool hasIndependentCopula() const;

  /** Tell if the distribution has elliptical copula */
  Bool hasEllipticalCopula() const;

  /** Check if the distribution is elliptical */
  Bool isElliptical() const;

  /** Check if the distribution is continuous */
  Bool isContinuous() const;

  /** Check if the distribution is discrete */
  Bool isDiscrete() const;

  /** Tell if the distribution is integer valued */
  Bool isIntegral() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


private:

  struct ComputeSamplePolicy
  {
    const NumericalSample input_;
    NumericalSample & output_;
    const Collection<Distribution> distributionCollection_;
    UnsignedInteger dimension_;

    ComputeSamplePolicy( const NumericalSample & input,
                         NumericalSample & output,
                         const Collection<Distribution> & distributionCollection)
      : input_(input)
      , output_(output)
      , distributionCollection_(distributionCollection)
      , dimension_(distributionCollection.getSize())
    {}

    inline void operator()( const TBB::BlockedRange<UnsignedInteger> & r ) const
    {
      for (UnsignedInteger i = r.begin(); i != r.end(); ++i)
        for (UnsignedInteger j = 0; j < dimension_; ++j)
          output_[i][j] = distributionCollection_[j].computeQuantile(input_[i][j])[0];
    }

  }; /* end struct ComputeSamplePolicy */

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** Compute the mean of the distribution */
  void computeMean() const;

  /** Compute the covariance of the distribution */
  void computeCovariance() const;

  /** The collection of distribution of the ComposedDistribution */
  DistributionPersistentCollection distributionCollection_;

  /** The copula of the ComposedDistribution */
  Distribution copula_;

}; /* class ComposedDistribution */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COMPOSEDDISTRIBUTION_HXX */
