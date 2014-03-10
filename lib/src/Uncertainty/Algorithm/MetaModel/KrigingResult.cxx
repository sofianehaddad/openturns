//                                               -*- C++ -*-
/**
 *  @file  KrigingResult.cxx
 *  @brief The result of a chaos expansion
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
#include "KrigingResult.hxx"
#include "Os.hxx"
#include "OSS.hxx"
#include "NumericalSample.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

static Factory<PersistentCollection<UnsignedInteger> > RegisteredFactory2("PersistentCollection<UnsignedInteger>");

CLASSNAMEINIT(KrigingResult);
static Factory<KrigingResult> RegisteredFactory("KrigingResult");

/* Default constructor */
KrigingResult::KrigingResult()
  : MetaModelResult()
{
  // Nothing to do
}


/* Default constructor */
KrigingResult::KrigingResult(const NumericalSample & inputSample,
                             const

                             const NumericalPoint & residuals,
                             const NumericalPoint & relativeErrors)
  : MetaModelResult(model, NumericalMathFunction(), residuals, relativeErrors),
{
  // The composed meta model will be a dual linear combination
  composedMetaModel_ = NumericalMathFunction(Psi_k, alpha_k);
  metaModel_ = NumericalMathFunction(composedMetaModel_, transformation);
}


/* Virtual constructor */
KrigingResult * KrigingResult::clone() const
{
  return new KrigingResult(*this);
}


/* String converter */
String KrigingResult::__repr__() const
{
  return OSS(true) << "class=" << getClassName()
         << " derived from " << MetaModelResult::__repr__()
         << " distribution=" << distribution_
         << " transformation=" << transformation_
         << " inverseTransformation=" << inverseTransformation_
         << " composedModel=" << composedModel_
         << " orthogonalBasis=" << orthogonalBasis_
         << " indices=" << I_
         << " coefficients=" << alpha_k_
         << " reduced basis=" << Psi_k_
         << " residuals=" << residuals_
         << " relativeErrors=" << relativeErrors_
         << " composedMetaModel=" << composedMetaModel_
         << " metaModel=" << metaModel_;
}

String KrigingResult::__str__(const String & offset) const
{
  OSS oss(false);
  oss << offset << "meta model=" << metaModel_ << Os::GetEndOfLine();
  oss << offset << "orthogonal basis=" << orthogonalBasis_;
  return oss;
}

/* Distribution accessor */
Distribution KrigingResult::getDistribution() const
{
  return distribution_;
}

/* IsoProbabilisticTransformation accessor */
NumericalMathFunction KrigingResult::getTransformation() const
{
  return transformation_;
}

/* InverseIsoProbabilisticTransformation accessor */
NumericalMathFunction KrigingResult::getInverseTransformation() const
{
  return inverseTransformation_;
}

/* Composed model accessor */
NumericalMathFunction KrigingResult::getComposedModel() const
{
  return composedModel_;
}

/* Orthogonal basis accessor */
OrthogonalBasis KrigingResult::getOrthogonalBasis() const
{
  return orthogonalBasis_;
}

/* Indices accessor */
Indices KrigingResult::getIndices() const
{
  return I_;
}

/* Coefficients accessor */
NumericalSample KrigingResult::getCoefficients() const
{
  return alpha_k_;
}

/* Reduced basis accessor */
KrigingResult::NumericalMathFunctionCollection KrigingResult::getReducedBasis() const
{
  return Psi_k_;
}

/* Composed meta model accessor */
NumericalMathFunction KrigingResult::getComposedMetaModel() const
{
  return composedMetaModel_;
}

/* Method save() stores the object through the StorageManager */
void KrigingResult::save(Advocate & adv) const
{
  MetaModelResult::save(adv);
  adv.saveAttribute( "distribution_", distribution_ );
  adv.saveAttribute( "transformation_", transformation_ );
  adv.saveAttribute( "inverseTransformation_", inverseTransformation_ );
  adv.saveAttribute( "composedModel_", composedModel_ );
  adv.saveAttribute( "orthogonalBasis_", orthogonalBasis_ );
  adv.saveAttribute( "I_", I_ );
  adv.saveAttribute( "alpha_k_", alpha_k_ );
  adv.saveAttribute( "Psi_k_", Psi_k_ );
  adv.saveAttribute( "composedMetaModel_", composedMetaModel_ );
}


/* Method load() reloads the object from the StorageManager */
void KrigingResult::load(Advocate & adv)
{
  MetaModelResult::load(adv);
  adv.loadAttribute( "distribution_", distribution_ );
  adv.loadAttribute( "transformation_", transformation_ );
  adv.loadAttribute( "inverseTransformation_", inverseTransformation_ );
  adv.loadAttribute( "composedModel_", composedModel_ );
  adv.loadAttribute( "orthogonalBasis_", orthogonalBasis_ );
  adv.loadAttribute( "I_", I_ );
  adv.loadAttribute( "alpha_k_", alpha_k_ );
  adv.loadAttribute( "Psi_k_", Psi_k_ );
  adv.loadAttribute( "composedMetaModel_", composedMetaModel_ );
}



END_NAMESPACE_OPENTURNS
