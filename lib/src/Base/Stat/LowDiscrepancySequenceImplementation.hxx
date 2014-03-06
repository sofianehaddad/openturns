//                                               -*- C++ -*-
/**
 *  @file  LowDiscrepancySequenceImplementation.hxx
 *  @brief This class is a top-level class for low discrepancy sequences
 *  simulation algorithms. It delegates to its children the effective
 *  history strategy.
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
#ifndef OPENTURNS_LOWDISCREPANCYSEQUENCEIMPLEMENTATION_HXX
#define OPENTURNS_LOWDISCREPANCYSEQUENCEIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "Interval.hxx"
#include "Indices.hxx"
#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"
#include "Collection.hxx"

BEGIN_NAMESPACE_OPENTURNS
/**
 * @class LowDiscrepancySequenceImplementation
 */

class LowDiscrepancySequenceImplementation
  : public PersistentObject
{

  CLASSNAME;

public:


  /** Constructor with parameters */
  explicit LowDiscrepancySequenceImplementation(const UnsignedLong dimension = 1);

  /** Virtual constructor */
  virtual LowDiscrepancySequenceImplementation * clone() const;

  /** initialize the sequence */
  virtual void initialize(const UnsignedLong dimension);

  /** Dimension accessor */
  UnsignedLong getDimension() const;

  /** Generate a quasi-random vector of numbers uniformly distributed over [0, 1) */
  virtual NumericalPoint generate();

  /** Generate a sample of pseudo-random vectors of numbers uniformly distributed over [0, 1) */
  virtual NumericalSample generate(const UnsignedLong size);

  /** Compute the star discrepancy of a sample uniformly distributed over [0, 1) */
  static NumericalScalar ComputeStarDiscrepancy(const NumericalSample & sample);

  /** String converter */
  virtual String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

private:
  /** Compute the local discrepancy of a sample, given a multidimensionnal interval */
  static NumericalScalar ComputeLocalDiscrepancy(const NumericalSample & sample,
      const Interval & interval);

protected:
  typedef Collection<Unsigned64BitsInteger>                           Unsigned64BitsIntegerCollection;
  typedef PersistentCollection<Unsigned64BitsInteger>                 Unsigned64BitsIntegerPersistentCollection;

  /** Get the needed prime numbers */
  static Unsigned64BitsIntegerCollection GetPrimeNumbers(const Indices & indices);

  /** Compute the n first prime numbers */
  static Unsigned64BitsIntegerCollection ComputeFirstPrimeNumbers(const UnsignedLong n);

  /** Compute the least prime number greater or equal to n */
  static Unsigned64BitsInteger ComputeNextPrimeNumber(const UnsignedLong n);

  /** Dimension parameter */
  UnsignedLong dimension_;

} ; /* class LowDiscrepancySequenceImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LOWDISCREPANCYSEQUENCEIMPLEMENTATION_HXX */
