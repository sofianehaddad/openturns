//                                               -*- C++ -*-
/**
 *  @file  ParametricHessianImplementation.cxx
 *  @brief ParametricHessianImplementation
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
 *  Id:      $Id: ParametricHessianImplementation.cxx 995 2008-10-31 10:52:04Z dutka $
 */
#include "ParametricHessianImplementation.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ParametricHessianImplementation);

static Factory<ParametricHessianImplementation> RegisteredFactory("ParametricHessianImplementation");


/* Default constructor */
ParametricHessianImplementation::ParametricHessianImplementation()
  : NumericalMathHessianImplementation()
  , evaluation_()
{
  // Nothing to do
}

/* Parameter constructor */
ParametricHessianImplementation::ParametricHessianImplementation(const ParametricEvaluationImplementation & evaluation)
  : NumericalMathHessianImplementation()
  , evaluation_(evaluation)
{
  // Nothing to do
}

/* Virtual constructor method */
ParametricHessianImplementation * ParametricHessianImplementation::clone() const
{
  return new ParametricHessianImplementation(*this);
}

/* Hessian operator */
SymmetricTensor ParametricHessianImplementation::hessian(const NumericalPoint & point,
    const NumericalPoint & parameters) const
{
  const UnsignedInteger parametersDimension(parameters.getDimension());
  if (parametersDimension != evaluation_.getParametersPositions().getSize()) throw InvalidArgumentException(HERE) << "Error: expected a parameters of dimension=" << evaluation_.getParametersPositions().getSize() << ", got dimension=" << parametersDimension;
  const UnsignedInteger inputDimension(evaluation_.function_.getInputDimension());
  const UnsignedInteger pointDimension(point.getDimension());
  if (pointDimension + parametersDimension != inputDimension) throw InvalidArgumentException(HERE) << "Error: expected a point of dimension=" << inputDimension - parametersDimension << ", got dimension=" << pointDimension;
  NumericalPoint x(inputDimension);
  for (UnsignedInteger i = 0; i < parametersDimension; ++i) x[evaluation_.parametersPositions_[i]] = parameters[i];
  for (UnsignedInteger i = 0; i < pointDimension; ++i) x[evaluation_.inputPositions_[i]] = point[i];
  const UnsignedInteger outputDimension(getOutputDimension());
  const SymmetricTensor fullHessian(evaluation_.function_.hessian(x));
  // The gradient wrt x corresponds to the inputPositions rows of the full gradient
  SymmetricTensor result(pointDimension, outputDimension);
  for (UnsignedInteger i = 0; i < pointDimension; ++i)
  {
    const UnsignedInteger i0(evaluation_.inputPositions_[i]);
    for (UnsignedInteger j = 0; j < pointDimension; ++j)
    {
      const UnsignedInteger j0(evaluation_.inputPositions_[j]);
      {
        for (UnsignedInteger k = 0; k < outputDimension; ++k)
          result(i, j, k) = fullHessian(i0, j0, k);
      }
    }
  }
  return result;
}

/* Hessian operator */
SymmetricTensor ParametricHessianImplementation::hessian(const OT::NumericalPoint & point) const
{
  // Use the current parameters value
  return hessian(point, evaluation_.getParameters());
}

/* Evaluation accessors */
ParametricEvaluationImplementation ParametricHessianImplementation::getEvaluation() const
{
  return evaluation_;
}

/* Dimension accessors */
UnsignedInteger ParametricHessianImplementation::getInputDimension() const
{
  return evaluation_.getInputDimension();
}

UnsignedInteger ParametricHessianImplementation::getParametersDimension() const
{
  return evaluation_.getParametersDimension();
}

UnsignedInteger ParametricHessianImplementation::getOutputDimension() const
{
  return evaluation_.getOutputDimension();
}

/* String converter */
String ParametricHessianImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << ParametricHessianImplementation::GetClassName()
      << " evaluation=" << evaluation_;
  return oss;
}

/* Method save() stores the object through the StorageManager */
void ParametricHessianImplementation::save(Advocate & adv) const
{
  NumericalMathHessianImplementation::save( adv );
  adv.saveAttribute( "evaluation_", evaluation_ );
}

/* Method load() reloads the object from the StorageManager */
void ParametricHessianImplementation::load(Advocate & adv)
{
  NumericalMathHessianImplementation::load( adv );
  adv.loadAttribute( "evaluation_", evaluation_ );
}


END_NAMESPACE_OPENTURNS
