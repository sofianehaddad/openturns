//                                               -*- C++ -*-
/**
 *  @file  UsualRandomVector.cxx
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
#include "PersistentObjectFactory.hxx"
#include "UsualRandomVector.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(UsualRandomVector);

static Factory<UsualRandomVector> RegisteredFactory("UsualRandomVector");

/* Default constructor */
UsualRandomVector::UsualRandomVector(const Distribution & distribution,
                                     const String & name)
  : RandomVectorImplementation(name)
  , distribution_(distribution)
{
  // Get the description from the underlying distribution
  setDescription(distribution.getDescription());
}

/* Virtual constructor */
UsualRandomVector * UsualRandomVector::clone() const
{
  return new UsualRandomVector(*this);
}

/* String converter */
String UsualRandomVector::__repr__() const
{
  OSS oss;
  oss << "class=" << UsualRandomVector::GetClassName()
      << " distribution=" << distribution_;
  return oss;
}

/* Here is the interface that all derived class must implement */


/* Dimension accessor */
UnsignedInteger UsualRandomVector::getDimension() const
{
  return distribution_.getDimension();
}

/* Realization accessor */
NumericalPoint UsualRandomVector::getRealization() const
{
  return distribution_.getRealization();
}

/* Numerical sample accessor */
NumericalSample UsualRandomVector::getSample(const UnsignedInteger size) const
{
  return distribution_.getSample(size);
}

/* Mean accessor */
NumericalPoint UsualRandomVector::getMean() const
{
  return distribution_.getMean();
}

/* Covariance accessor */
CovarianceMatrix UsualRandomVector::getCovariance() const
{
  return distribution_.getCovariance();
}

/* Get the random vector corresponding to the i-th marginal component */
UsualRandomVector::Implementation UsualRandomVector::getMarginal(const UnsignedInteger i) const
{
  if (i >= getDimension()) throw InvalidArgumentException(HERE) << "The index of a marginal random vector must be in the range [0, dim-1]";
  return new UsualRandomVector(distribution_.getMarginal(i));
}

/* Get the marginal random vector corresponding to indices components */
UsualRandomVector::Implementation UsualRandomVector::getMarginal(const Indices & indices) const
{
  if (!indices.check(getDimension() - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal random vector must be in the range [0, dim-1] and  must be different";
  return new UsualRandomVector(distribution_.getMarginal(indices));
}

/* Distribution accessor */
Distribution UsualRandomVector::getDistribution() const
{
  return distribution_;
}

/* Method save() stores the object through the StorageManager */
void UsualRandomVector::save(Advocate & adv) const
{
  RandomVectorImplementation::save(adv);
  adv.saveAttribute( "distribution_", distribution_ );
}

/* Method load() reloads the object from the StorageManager */
void UsualRandomVector::load(Advocate & adv)
{
  RandomVectorImplementation::load(adv);
  adv.loadAttribute( "distribution_", distribution_ );
}

END_NAMESPACE_OPENTURNS
