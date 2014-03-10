//                                               -*- C++ -*-
/**
 *  @file  KrigingAlgorithm.cxx
 *  @brief The class building gaussian process regression
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
 */

#include "KrigingGradient.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(KrigingGradient);


static Factory<KrigingGradient> RegisteredFactory("KrigingGradient");


/* Constructor with parameters */
KrigingGradient::KrigingGradient()
  : NumericalMathGradientImplementation()
{}


/* Constructor with parameters */
KrigingGradient::KrigingGradient (const Basis & basis,
                                  const NumericalSample & inputSample,
                                  const CovarianceModel & covarianceModel,
                                  const NumericalPoint & beta,
                                  const NumericalPoint & gamma)
  : NumericalMathGradientImplementation()
  , basis_(basis)
  , inputSample_(inputSample)
  , covarianceModel_(covarianceModel)
  , beta_(beta)
  , gamma_(gamma)
{
}


/* Virtual constructor */
KrigingGradient * KrigingGradient::clone() const
{
  return new KrigingGradient(*this);
}

/* Comparison operator */
Bool KrigingGradient::operator==(const KrigingGradient & other) const
{
  return true;
}

/* String converter */
String KrigingGradient::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName()
      << " correlationModel=" << covarianceModel_
      << " beta=" << beta_
      << " gamma=" << gamma_;
  return oss;
}

/* String converter */
String KrigingGradient::__str__(const String & offset) const
{
  return OSS(false) << offset << GetClassName();
}

/* Test for actual implementation */
Bool KrigingGradient::isActualImplementation() const
{
  return true;
}

Matrix KrigingGradient::gradient(const NumericalPoint & inP) const
{
  const UnsignedInteger size = inP.getSize();
  const UnsignedInteger trainingSize = inputSample_.getSize();

  const UnsignedInteger basisSize = basis_.getSize();

  Matrix R(size, trainingSize);
  for (UnsignedInteger i = 0; i < size; ++ i )
  {
    for (UnsignedInteger j = 0; j < trainingSize; ++ j )
    {
      R(i, j) = covarianceModel_.partialGradient(inP, inputSample_[j])(0, 0, i);
    }
  }

  Matrix F(size, basisSize);
  for ( UnsignedInteger i = 0; i < size; ++ i )
  {
    for (UnsignedInteger j = 0; j < basisSize; ++ j )
    {
      F(i, j) = basis_[j].gradient(inP)(i, 0);
    }
  }

  NumericalPoint pointResult(F * beta_ + R * gamma_);

  callsNumber_ += size;

  Matrix result(size, 1);
  for ( UnsignedInteger i = 0; i < size; ++ i )
  {
    result(i, 0) = pointResult[i];
  }

  return result;
}


/* Accessor for input point dimension */
UnsignedInteger KrigingGradient::getInputDimension() const
{
  return basis_.getDimension();
}

/* Accessor for output point dimension */
UnsignedInteger KrigingGradient::getOutputDimension() const
{
  return 1;
}

/* Method save() stores the object through the StorageManager */
void KrigingGradient::save(Advocate & adv) const
{
  NumericalMathGradientImplementation::save(adv);
  adv.saveAttribute("basis_", basis_);
  adv.saveAttribute("inputSample_", inputSample_);
  adv.saveAttribute("covarianceModel_", covarianceModel_);
  adv.saveAttribute("beta_", beta_);
  adv.saveAttribute("gamma_", gamma_);
}

/* Method load() reloads the object from the StorageManager */
void KrigingGradient::load(Advocate & adv)
{
  NumericalMathGradientImplementation::load(adv);
  adv.loadAttribute("basis_", basis_);
  adv.loadAttribute("inputSample_", inputSample_);
  adv.loadAttribute("covarianceModel_", covarianceModel_);
  adv.loadAttribute("beta_", beta_);
  adv.loadAttribute("gamma_", gamma_);
}


END_NAMESPACE_OPENTURNS
