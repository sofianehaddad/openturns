//                                               -*- C++ -*-
/**
 *  @brief Classification algorithm base type
 *
 *  Copyright 2005-2025 Airbus-EDF-IMACS-ONERA-Phimeca
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
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "openturns/PersistentObjectFactory.hxx"
#include "openturns/ClassifierImplementation.hxx"
#include "openturns/TBBImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ClassifierImplementation)

static const Factory<ClassifierImplementation> Factory_ClassifierImplementation;

/* Default constructor */
ClassifierImplementation::ClassifierImplementation()
  : PersistentObject()
  , inputSample_(0, 0)
  , classes_(0)
  , isParallel_(ResourceMap::GetAsBool("Classifier-Parallel"))
{
  // Nothing to do
}

/* Default constructor */
ClassifierImplementation::ClassifierImplementation(const Sample & inputSample,
    const Indices & classes)
  : PersistentObject()
  , inputSample_(inputSample)
  , classes_(classes)
  , isParallel_(ResourceMap::GetAsBool("Classifier-Parallel"))
{
  // Nothing to do
}

ClassifierImplementation * ClassifierImplementation::clone() const
{
  return new ClassifierImplementation(*this);
}


UnsignedInteger ClassifierImplementation::getNumberOfClasses() const
{
  throw NotYetImplementedException(HERE) << "In ClassifierImplementation::getNumberOfClasses() const";
}

/* Classify a sample */
UnsignedInteger ClassifierImplementation::classify(const Point & ) const
{
  throw NotYetImplementedException(HERE) << "In ClassifierImplementation::classify(const Point & inP) const";
}

/* Classify a sample */
struct ClassifyPolicy
{
  const Sample & input_;
  Indices & output_;
  const ClassifierImplementation * p_classifier_;

  ClassifyPolicy( const Sample & input,
                  Indices & output,
                  const ClassifierImplementation * p_classifier)
    : input_(input)
    , output_(output)
    , p_classifier_(p_classifier)
  {}

  inline void operator()( const TBBImplementation::BlockedRange<UnsignedInteger> & r ) const
  {
    for (UnsignedInteger i = r.begin(); i != r.end(); ++ i) output_[i] = p_classifier_->classify(input_[i]);
  }

}; /* end struct ClassifyPolicy */


Indices ClassifierImplementation::classify(const Sample & inS) const
{
  const UnsignedInteger size = inS.getSize();
  Indices result(size);
  const ClassifyPolicy policy(inS, result, this);
  TBBImplementation::ParallelForIf(isParallel(), 0, size, policy);
  return result;
}

/* Grade a point */
Scalar ClassifierImplementation::grade(const Point &, const UnsignedInteger ) const
{
  throw NotYetImplementedException(HERE) << "In ClassifierImplementation::grade(const Point & inP, const UnsignedInteger hClass) const";
}

/* Grade a sample */
struct GradePolicy
{
  const Sample & input_;
  const Indices & classes_;
  Point & output_;
  const ClassifierImplementation * p_classifier_;

  GradePolicy( const Sample & input,
               const Indices & classes,
               Point & output,
               const ClassifierImplementation * p_classifier)
    : input_(input)
    , classes_(classes)
    , output_(output)
    , p_classifier_(p_classifier)
  {}

  inline void operator()( const TBBImplementation::BlockedRange<UnsignedInteger> & r ) const
  {
    for (UnsignedInteger i = r.begin(); i != r.end(); ++ i) output_[i] = p_classifier_->grade(input_[i], classes_[i]);
  }

}; /* end struct GradePolicy */


/* Grade a sample */
Point ClassifierImplementation::grade(const Sample & inS, const Indices & hClass) const
{
  const UnsignedInteger size = inS.getSize();
  if (size != hClass.getSize())
    throw InvalidDimensionException(HERE) << "Input sample dimension (=" << size << ") and classes dimension (=" << hClass.getSize() << ") do not match.";
  Point result(size);
  const GradePolicy policy(inS, hClass, result, this);
  TBBImplementation::ParallelForIf(isParallel(), 0, size, policy);
  return result;
}

/** Parallelization flag accessor */
void ClassifierImplementation::setParallel(const Bool flag)
{
  isParallel_ = flag;
}

Bool ClassifierImplementation::isParallel() const
{
  return isParallel_;
}

/* String converter */
String ClassifierImplementation::__repr__() const
{
  return OSS(true) << "class=" << getClassName()
         << " x=" << inputSample_
         << " y=" << classes_;
}


String ClassifierImplementation::__str__(const String & ) const
{
  return __repr__();
}

/* Input space dimension accessor */
UnsignedInteger ClassifierImplementation::getDimension() const
{
  return inputSample_.getDimension();
}

/* Method save() stores the object through the StorageManager */
void ClassifierImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "inputSample_", inputSample_ );
  adv.saveAttribute( "classes_", classes_ );
}


/* Method load() reloads the object from the StorageManager */
void ClassifierImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "inputSample_", inputSample_ );
  adv.loadAttribute( "classes_", classes_ );
}


END_NAMESPACE_OPENTURNS

