//                                               -*- C++ -*-
/**
 *  @file  StandardEvent.cxx
 *  @brief The class that implements an event based on a composite random vector whose
 *  antecedent has a spherical distribution
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
#include "StandardEvent.hxx"
#include "NumericalMathFunction.hxx"
#include "Distribution.hxx"
#include "CompositeRandomVector.hxx"
#include "UsualRandomVector.hxx"
#include "RandomVectorImplementation.hxx"
#include "Pointer.hxx"
#include "ComparisonOperatorImplementation.hxx"
#include "CovarianceMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS


typedef Distribution::InverseIsoProbabilisticTransformation InverseIsoProbabilisticTransformation;

CLASSNAMEINIT(StandardEvent);

/* Default constructor */
StandardEvent::StandardEvent()
  : Event()
{
  // Nothing to do
}

/* Constructor from a RandomVector */
StandardEvent::StandardEvent(const RandomVector & antecedent,
                             const ComparisonOperator & op,
                             const NumericalScalar threshold,
                             const String & name)
  : Event(antecedent, op, threshold, name)
{
  // StandardEvent can only be constructed from composite random vector whose antecedent has a spherical distribution. As we cannot check it, we just check that the distribution is elliptical
  if (!antecedent.getImplementation()->getAntecedent()->getDistribution().isElliptical()) throw InvalidArgumentException(HERE) << "Error: StandardEvent can only be constructed from composite random vectors whose antecedent is standard spherical, here the distribution is " << antecedent.getImplementation()->getAntecedent()->getDistribution().getImplementation()->__str__();
  CovarianceMatrix covariance(antecedent.getImplementation()->getAntecedent()->getDistribution().getCovariance());
  const UnsignedLong dimension(antecedent.getImplementation()->getAntecedent()->getDistribution().getDimension());
  for (UnsignedLong i = 0; i < dimension; ++i)
  {
    if (covariance(i, i) != 1.0) throw InvalidArgumentException(HERE) << "Error: StandardEvent can only be constructed from composite random vectors whose antecedent is standard spherical, here the distribution is " << antecedent.getImplementation()->getAntecedent()->getDistribution().getImplementation()->__str__();
    for (UnsignedLong j = 0; j < i; ++j) if (covariance(i, j) != 0.0) throw InvalidArgumentException(HERE) << "Error: StandardEvent can only be constructed from composite random vectors whose antecedent is standard spherical, here the distribution is " << antecedent.getImplementation()->getAntecedent()->getDistribution().getImplementation()->__str__();
  }
}

/* Constructor from an Event */
StandardEvent::StandardEvent(const Event & event,
                             const String & name)
  : Event(event)
{
  // Get the isoprobabilistic inverse transformation from the antecedent distribution
  const Distribution distribution(event.getImplementation()->getAntecedent()->getDistribution());
  const InverseIsoProbabilisticTransformation inverse(distribution.getInverseIsoProbabilisticTransformation());
  // Get the function
  const NumericalMathFunction function(event.getImplementation()->getFunction());
  // Get the standard distribution from the antecedent distribution
  const Distribution standard(distribution.getStandardDistribution());
  // Build the antecedent
  const RandomVector antecedent(standard);
  const NumericalMathFunction composed(function, inverse);
  const RandomVector vect(composed, antecedent);
  // Set the random vector implementation
  (*this) = StandardEvent(vect, event.getOperator(), event.getThreshold(), event.getName());
}

END_NAMESPACE_OPENTURNS
