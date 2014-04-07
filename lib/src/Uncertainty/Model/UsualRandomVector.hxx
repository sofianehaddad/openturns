//                                               -*- C++ -*-
/**
 *  @file  UsualRandomVector.hxx
 *  @brief An implementation class for distribution-based random vectors
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#ifndef OPENTURNS_USUALRANDOMVECTOR_HXX
#define OPENTURNS_USUALRANDOMVECTOR_HXX

#include "OTprivate.hxx"
#include "RandomVectorImplementation.hxx"
#include "Distribution.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class UsualRandomVector
 *
 * An implementation class for distribution-based random vectors
 */
class UsualRandomVector
  : public RandomVectorImplementation
{
  CLASSNAME;

public:

  /** Some typedefs to ease reading */


  /** Default constructor */
  explicit UsualRandomVector(const Distribution & distribution);


  /** Virtual constructor */
  virtual UsualRandomVector * clone() const;

  /** String converter */
  String __repr__() const;


  /* Here is the interface that all derived class must implement */

  /** Dimension accessor */
  UnsignedInteger getDimension() const;

  /** Realization accessor */
  NumericalPoint getRealization() const;

  /** Numerical sample accessor */
  NumericalSample getSample(const UnsignedInteger size) const;

  /** Mean accessor */
  NumericalPoint getMean() const;

  /** Covariance accessor */
  CovarianceMatrix getCovariance() const;

  /** Get the random vector corresponding to the i-th marginal component */
  Implementation getMarginal(const UnsignedInteger i) const;

  /** Get the marginal random vector corresponding to indices components */
  Implementation getMarginal(const Indices & indices) const;

  /** Distribution accessor */
  Distribution getDistribution() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  friend class Factory<UsualRandomVector>;

  /** Default constructor */
  UsualRandomVector() : RandomVectorImplementation() {};

private:

  /** The distribution the vector follows */
  Distribution distribution_;

}; /* class UsualRandomVector */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_USUALRANDOMVECTOR_HXX */
