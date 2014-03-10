//                                               -*- C++ -*-
/**
 *  @file  FFTImplementation.cxx
 *  @brief
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
#include "FFTImplementation.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class FFTImplementation
 */

CLASSNAMEINIT(FFTImplementation);

static Factory<FFTImplementation> RegisteredFactory("FFTImplementation");

/* Constructor without parameters */
FFTImplementation::FFTImplementation(const String & name)
  : PersistentObject(name)
{
  // Nothing to do
}

/* Virtual constructor */
FFTImplementation * FFTImplementation::clone() const
{
  return new FFTImplementation(*this);
}

/* FFT transformation on real */
FFTImplementation::NumericalComplexCollection FFTImplementation::transform(const NumericalScalarCollection & collection) const
{
  const UnsignedInteger size(collection.getSize());
  NumericalComplexCollection coll(size);
  for (UnsignedInteger i = 0; i < size; ++i) coll[i] = collection[i];
  return transform(coll);
}


/* FFT transformation on complex */
FFTImplementation::NumericalComplexCollection FFTImplementation::transform(const NumericalComplexCollection & collection) const
{
  return transform(collection, 0, collection.getSize());
}

/* FFT transformation on complex - For some FFT implementation, the transformation is applied on a part of the collection */
FFTImplementation::NumericalComplexCollection FFTImplementation::transform(const NumericalComplexCollection & collection,
    const UnsignedInteger first,
    const UnsignedInteger size) const
{
  throw NotYetImplementedException(HERE);
}

/* FFT transformation on complex - For some FFT implementation, the need is to transform a regular sequence of the collection (between first and last, by step = step)*/
FFTImplementation::NumericalComplexCollection FFTImplementation::transform(const NumericalComplexCollection & collection,
    const UnsignedInteger first,
    const UnsignedInteger step,
    const UnsignedInteger last) const
{
  throw NotYetImplementedException(HERE);
}

/* FFT inverse transformation */
FFTImplementation::NumericalComplexCollection FFTImplementation::inverseTransform(const NumericalComplexCollection & collection) const
{
  throw NotYetImplementedException(HERE);
}

/* FFT inverse transformation - The transformation is applied on a part of the collection */
FFTImplementation::NumericalComplexCollection FFTImplementation::inverseTransform(const NumericalComplexCollection & collection,
    const UnsignedInteger first,
    const UnsignedInteger size) const
{
  throw NotYetImplementedException(HERE);
}

/* FFT transformation on a regular sequence of the collection (between first and last, spearated  by step)*/
FFTImplementation::NumericalComplexCollection FFTImplementation::inverseTransform(const NumericalComplexCollection & collection,
    const UnsignedInteger first,
    const UnsignedInteger step,
    const UnsignedInteger last) const
{
  throw NotYetImplementedException(HERE);
}

/* String converter */
String FFTImplementation::__repr__() const
{
  OSS oss(true);
  oss << "class=" << FFTImplementation::GetClassName();
  return oss;
}

/* String converter */
String FFTImplementation::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "class=" << FFTImplementation::GetClassName();
  return oss;
}

END_NAMESPACE_OPENTURNS
