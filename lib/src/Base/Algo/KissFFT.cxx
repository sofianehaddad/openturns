//                                               -*- C++ -*-
/**
 *  @file  KissFFT.cxx
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
#include "KissFFT.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"
#include "kissfft.hh"

BEGIN_NAMESPACE_OPENTURNS

typedef kissfft<NumericalScalar> KISSFFTScalar;

/**
 * @class KissFFT
 */

CLASSNAMEINIT(KissFFT);

static Factory<KissFFT> RegisteredFactory("KissFFT");

/* Constructor with parameters */
KissFFT::KissFFT()
  : FFTImplementation()
{
  // Nothing to do
}

/* Virtual constructor */
KissFFT * KissFFT::clone() const
{
  return new KissFFT(*this);
}

/* String converter */
String KissFFT::__repr__() const
{
  OSS oss(true);
  oss << "class=" << KissFFT::GetClassName();
  return oss;
}

/* String converter */
String KissFFT::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "class=" << KissFFT::GetClassName();
  return oss;
}


/* FFT transformation on complex */
KissFFT::NumericalComplexCollection KissFFT::transform(const NumericalComplexCollection & collection) const
{
  // Get the size of the collection
  const UnsignedInteger fftSize(collection.getSize());

  // Init of kissfft  - Boolean argument is to tell that we perform a direct transformation
  KISSFFTScalar fft(fftSize, false);

  // Allocation of memory for final result
  NumericalComplexCollection transformationResult(fftSize);
  fft.transform( &collection[0] , &transformationResult[0] );
  return transformationResult;
}

/* FFT transformation on a regular complex sequence of the collection (between first and last, with elements separated by step)*/
KissFFT::NumericalComplexCollection KissFFT::transform(const NumericalComplexCollection & collection,
    const UnsignedInteger first,
    const UnsignedInteger size) const
{
  // Init of kissfft  - Boolean argument is to tell that we perform a direct transformation
  KISSFFTScalar fft(size, false);

  // Allocation of memory for final result
  NumericalComplexCollection transformationResult(size);
  fft.transform( &collection[first] , &transformationResult[0] );
  return transformationResult;
}

/* FFT T transformation */
KissFFT::NumericalComplexCollection KissFFT::inverseTransform(const NumericalComplexCollection & collection) const
{
  // Get the size of the collection
  const UnsignedInteger size(collection.getSize());

  // Init of kissfft  - Boolean argument is to tell that we perform an inverse transformation
  KISSFFTScalar fft(size, true);

  // Allocation of memory for final result
  NumericalComplexCollection transformationResult(size);

  // Inverse transform = transform with the opposite sign in the exponent and a 1/N factor
  // Call inverse transformation
  fft.transform( &collection[0] , &transformationResult[0] );
  const NumericalScalar alpha(1.0 / size);
  for (UnsignedInteger index = 0; index < size; ++index) transformationResult[index] *= alpha;
  return transformationResult;
}

/* FFT transformation on a regular complex sequence of the collection (between first and last, with elements separated by step)*/
KissFFT::NumericalComplexCollection KissFFT::inverseTransform(const NumericalComplexCollection & collection,
    const UnsignedInteger first,
    const UnsignedInteger size) const
{
  // Init of kissfft  - Boolean argument is true
  KISSFFTScalar fft(size, true);

  // Allocation of memory for final result
  NumericalComplexCollection transformationResult(size);
  fft.transform( &collection[first] , &transformationResult[0] );
  const NumericalScalar alpha(1.0 / size);
  for (UnsignedInteger index = 0; index < size; ++index) transformationResult[index] *= alpha;
  return transformationResult;
}

END_NAMESPACE_OPENTURNS
