//                                               -*- C++ -*-
/**
 *  @file  AggregatedNumericalMathGradientImplementation.cxx
 *  @brief The gradient part of linear combination of polynomials
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
 *  @author dutka
 *  @date   2008-05-21 11:21:38 +0200 (Wed, 21 May 2008)
 */
#include "AggregatedNumericalMathGradientImplementation.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(AggregatedNumericalMathGradientImplementation);

static Factory<AggregatedNumericalMathGradientImplementation> RegisteredFactory("AggregatedNumericalMathGradientImplementation");


/* Default constructor */
AggregatedNumericalMathGradientImplementation::AggregatedNumericalMathGradientImplementation()
  : NumericalMathGradientImplementation()
  , evaluation_()
{
  // Nothing to do
}


/* Parameters constructor */
AggregatedNumericalMathGradientImplementation::AggregatedNumericalMathGradientImplementation(const AggregatedNumericalMathEvaluationImplementation & evaluation)
  : NumericalMathGradientImplementation()
  , evaluation_(evaluation)
{
  // Nothing to do
}

/* Virtual constructor */
AggregatedNumericalMathGradientImplementation * AggregatedNumericalMathGradientImplementation::clone() const
{
  return new AggregatedNumericalMathGradientImplementation(*this);
}


/* Gradient method */
Matrix AggregatedNumericalMathGradientImplementation::gradient(const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inP.getDimension();
  const UnsignedLong size(evaluation_.functionsCollection_.getSize());
  Matrix result(evaluation_.getInputDimension(), evaluation_.getOutputDimension());
  UnsignedLong columnIndex(0);
  // Loop over the contributors
  for (UnsignedLong contributorIndex = 0; contributorIndex < size; ++contributorIndex)
  {
    const Matrix contributorGradient(evaluation_.functionsCollection_[contributorIndex].gradient(inP));
    // Copy the contributor gradient into the global matrix gradient
    // Must proceed in column order
    for (UnsignedLong j = 0; j < contributorGradient.getNbColumns(); ++j)
    {
      for (UnsignedLong i = 0; i < contributorGradient.getNbRows(); ++i)
        result(i, columnIndex) = contributorGradient(i, j);
      ++columnIndex;
    }
  } // contributorIndex
  return result;
}

/* Accessor for input point dimension */
UnsignedLong AggregatedNumericalMathGradientImplementation::getInputDimension() const
{
  return evaluation_.getInputDimension();
}

/* Accessor for output point dimension */
UnsignedLong AggregatedNumericalMathGradientImplementation::getOutputDimension() const
{
  return evaluation_.getOutputDimension();
}

/* String converter */
String AggregatedNumericalMathGradientImplementation::__repr__() const
{
  return OSS(true) << "class=" << GetClassName()
         << " evaluation=" << evaluation_;
}

String AggregatedNumericalMathGradientImplementation::__str__(const String & offset) const
{
  return __repr__();
}

/* Method save() stores the object through the StorageManager */
void AggregatedNumericalMathGradientImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "evaluation_", evaluation_ );
}


/* Method load() reloads the object from the StorageManager */
void AggregatedNumericalMathGradientImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "evaluation_", evaluation_ );
}



END_NAMESPACE_OPENTURNS
