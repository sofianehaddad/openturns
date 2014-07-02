//                                               -*- C++ -*-
/**
 *  @file  ParametricGradientImplementation.cxx
 *  @brief ParametricGradientImplementation
 *
 *  (C) Copyright 2005-2014 Airbus-EDF-Phimeca
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License.
 *
 *  This library is distributed in the hope that it will be useful
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 *  @author: $LastChangedBy: schueller $
 *  @date:   $LastChangedDate: 2008-10-31 11:52:04 +0100 (Fri, 31 Oct 2008) $
 *  Id:      $Id: ParametricGradientImplementation.cxx 995 2008-10-31 10:52:04Z dutka $
 */
#include "ParametricGradientImplementation.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ParametricGradientImplementation);

static Factory<ParametricGradientImplementation> RegisteredFactory("ParametricGradientImplementation");


/* Default constructor */
ParametricGradientImplementation::ParametricGradientImplementation()
  : NumericalMathGradientImplementation()
  , evaluation_()
{
  // Nothing to do
}

/* Parameter constructor */
ParametricGradientImplementation::ParametricGradientImplementation(const ParametricEvaluationImplementation & evaluation)
  : NumericalMathGradientImplementation()
  , evaluation_(evaluation)
{
  // Nothing to do
}

/* Virtual constructor method */
ParametricGradientImplementation * ParametricGradientImplementation::clone() const
{
  return new ParametricGradientImplementation(*this);
}

/* Gradient operator */
Matrix ParametricGradientImplementation::gradient(const NumericalPoint & point,
    const NumericalPoint & parameters) const
{
  const UnsignedInteger parametersDimension(parameters.getDimension());
  if (parametersDimension != evaluation_.getParametersPositions().getSize()) throw InvalidArgumentException(HERE) << "Error: expected a parameters of dimension=" << evaluation_.getParametersPositions().getSize() << ", got dimension=" << parametersDimension;
  const UnsignedInteger inputDimension(evaluation_.getFunction().getInputDimension());
  const UnsignedInteger pointDimension(point.getDimension());
  if (pointDimension + parametersDimension != inputDimension) throw InvalidArgumentException(HERE) << "Error: expected a point of dimension=" << inputDimension - parametersDimension << ", got dimension=" << pointDimension;
  NumericalPoint x(inputDimension);
  for (UnsignedInteger i = 0; i < parametersDimension; ++i) x[evaluation_.parametersPositions_[i]] = parameters[i];
  for (UnsignedInteger i = 0; i < pointDimension; ++i) x[evaluation_.inputPositions_[i]] = point[i];
  const UnsignedInteger outputDimension(getOutputDimension());
  const Matrix fullGradient(evaluation_.getFunction().gradient(x));
  // The gradient wrt x corresponds to the inputPositions rows of the full gradient
  Matrix result(pointDimension, outputDimension);
  for (UnsignedInteger i = 0; i < pointDimension; ++i)
  {
    const UnsignedInteger i0(evaluation_.inputPositions_[i]);
    for (UnsignedInteger j = 0; j < outputDimension; ++j)
      result(i, j) = fullGradient(i0, j);
  }
  return result;
}

/* Gradient operator */
Matrix ParametricGradientImplementation::gradient(const OT::NumericalPoint & point) const
{
  // Use the current parameters value
  return gradient(point, evaluation_.getParameters());
}

/* Evaluation accessor */
ParametricEvaluationImplementation ParametricGradientImplementation::getEvaluation() const
{
  return evaluation_;
}

/* Dimension accessors */
UnsignedInteger ParametricGradientImplementation::getInputDimension() const
{
  return evaluation_.getInputDimension();
}

UnsignedInteger ParametricGradientImplementation::getParametersDimension() const
{
  return evaluation_.getParametersDimension();
}

UnsignedInteger ParametricGradientImplementation::getOutputDimension() const
{
  return evaluation_.getOutputDimension();
}

/* String converter */
String ParametricGradientImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << ParametricGradientImplementation::GetClassName()
      << " evaluation=" << evaluation_;
  return oss;
}

/* Method save() stores the object through the StorageManager */
void ParametricGradientImplementation::save(Advocate & adv) const
{
  NumericalMathGradientImplementation::save( adv );
  adv.saveAttribute( "evaluation_", evaluation_ );
}

/* Method load() reloads the object from the StorageManager */
void ParametricGradientImplementation::load(Advocate & adv)
{
  NumericalMathGradientImplementation::load( adv );
  adv.loadAttribute( "evaluation_", evaluation_ );
}


END_NAMESPACE_OPENTURNS
