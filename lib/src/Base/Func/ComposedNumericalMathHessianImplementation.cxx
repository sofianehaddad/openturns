//                                               -*- C++ -*-
/**
 *  @file  ComposedNumericalMathHessianImplementation.cxx
 *  @brief The class that implements the composition between numerical math hessians
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include "ComposedNumericalMathHessianImplementation.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(ComposedNumericalMathHessianImplementation);

static Factory<ComposedNumericalMathHessianImplementation> RegisteredFactory("ComposedNumericalMathHessianImplementation");

/* Default constructor */
ComposedNumericalMathHessianImplementation::ComposedNumericalMathHessianImplementation(const GradientImplementation & p_leftGradient,
    const HessianImplementation & p_leftHessian,
    const EvaluationImplementation & p_rightFunction,
    const GradientImplementation & p_rightGradient,
    const HessianImplementation & p_rightHessian)
  : NumericalMathHessianImplementation(),
    p_leftGradient_(p_leftGradient),
    p_leftHessian_(p_leftHessian),
    p_rightFunction_(p_rightFunction),
    p_rightGradient_(p_rightGradient),
    p_rightHessian_(p_rightHessian)
{
  // We check that the dimensions of the input parameters are compatible
  // First, check the compatibility between the right function, the right gradient and the right hessian
  if ((p_rightFunction->getInputDimension() != p_rightGradient->getInputDimension()) || (p_rightFunction->getInputDimension() != p_rightHessian->getInputDimension()) || (p_rightFunction->getOutputDimension() != p_rightGradient->getOutputDimension()) || (p_rightFunction->getOutputDimension() != p_rightHessian->getOutputDimension())) throw InvalidArgumentException(HERE) << "Error: the right function, the right gradient and the right hessian have incompatible input or output dimensions.";
  // Second, check the compatibility between the left gradient and the left hessian
  if ((p_leftGradient->getInputDimension() != p_leftHessian->getInputDimension()) || (p_leftGradient->getOutputDimension() != p_leftHessian->getOutputDimension())) throw InvalidArgumentException(HERE) << "Error: the left gradient and the left hessian have incompatible input or output dimensions.";
  // Third, check the compatibility between the right output dimension and the left input dimension
  if (p_leftGradient->getInputDimension() != p_rightFunction->getOutputDimension()) throw InvalidArgumentException(HERE) << "Error: the left gradient and the right function have incompatible input or output dimensions.";
}

/* Virtual constructor */
ComposedNumericalMathHessianImplementation * ComposedNumericalMathHessianImplementation::clone() const
{
  return new ComposedNumericalMathHessianImplementation(*this);
}

/* Comparison operator */
Bool ComposedNumericalMathHessianImplementation::operator ==(const ComposedNumericalMathHessianImplementation & other) const
{
  return true;
}

/* String converter */
String ComposedNumericalMathHessianImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << ComposedNumericalMathHessianImplementation::GetClassName()
      << " name=" << getName()
      << " leftGradient=" << p_leftGradient_->__repr__()
      << " leftHessian=" << p_leftHessian_->__repr__()
      << " rightFunction=" << p_rightFunction_->__repr__()
      << " rightGradient=" << p_rightGradient_->__repr__()
      << " rightHessian=" << p_rightHessian_->__repr__();
  return oss;
}

/* Method hessian() returns the symmetric tensor of the second gradient of the function at point */
SymmetricTensor ComposedNumericalMathHessianImplementation::hessian(const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inP.getDimension();
  ++callsNumber_;
  // Hessian of the composed function fog, g:R^n->R^p, f:R^p->R^q so fog:R^n->R^q
  const NumericalPoint p_rightIn(p_rightFunction_->operator()(inP));
  const Matrix p_rightGradientIn(p_rightGradient_->gradient(inP));
  const SymmetricTensor p_rightHessianIn(p_rightHessian_->hessian(inP));
  const Matrix p_leftGradientIn(p_leftGradient_->gradient(p_rightIn));
  const SymmetricTensor p_leftHessianIn(p_leftHessian_->hessian(p_rightIn));
  // Get the several dimensions
  const UnsignedLong outputDimension(getOutputDimension());
  const UnsignedLong innerDimension(p_rightIn.getDimension());
  SymmetricTensor result(inputDimension, outputDimension);
  /* We unroll the formula:
   *  D2(h)(x) = D2(f)(g(x))D(g)(x)D(g)(x)+D(f)(g(x))D2(g)(x)
   *  after transposition to convert jacobian and hessian into
   *  the associated gradients
   */
  for (UnsignedLong i = 0; i < inputDimension; ++i)
  {
    for (UnsignedLong j = 0; j <= i; ++j)
    {
      for (UnsignedLong k = 0; k < outputDimension; ++k)
      {
        result(i, j, k) = 0.0;
        for (UnsignedLong m = 0; m < innerDimension; ++m)
        {
          result(i, j, k) += p_leftGradientIn(m, k) * p_rightHessianIn(i, j, m);
          for (UnsignedLong n = 0; n < p_rightIn.getDimension(); ++n)
          {
            result(i, j, k) += p_leftHessianIn(m, n, k) * p_rightGradientIn(i, m) * p_rightGradientIn(j, n);
          } // n
        } // m
      } // k
    } // j
  } // i
  return result;
}

/* Accessor for input point dimension */
UnsignedLong ComposedNumericalMathHessianImplementation::getInputDimension() const
{
  return p_rightHessian_->getInputDimension();
}

/* Accessor for output point dimension */
UnsignedLong ComposedNumericalMathHessianImplementation::getOutputDimension() const
{
  return p_leftHessian_->getOutputDimension();
}

/* Method save() stores the object through the StorageManager */
void ComposedNumericalMathHessianImplementation::save(Advocate & adv) const
{
  NumericalMathHessianImplementation::save(adv);
  adv.saveAttribute( "leftGradient_", *p_leftGradient_ );
  adv.saveAttribute( "leftHessian_", *p_leftHessian_ );
  adv.saveAttribute( "rightFunction_", *p_rightFunction_ );
  adv.saveAttribute( "rightGradient_", *p_rightGradient_ );
  adv.saveAttribute( "rightHessian_", *p_rightHessian_ );
}

/* Method load() reloads the object from the StorageManager */
void ComposedNumericalMathHessianImplementation::load(Advocate & adv)
{
  TypedInterfaceObject<NumericalMathEvaluationImplementation> evaluationValue;
  TypedInterfaceObject<NumericalMathGradientImplementation> gradientValue;
  TypedInterfaceObject<NumericalMathHessianImplementation> hessianValue;
  NumericalMathHessianImplementation::load(adv);
  adv.loadAttribute( "leftGradient_", gradientValue );
  p_leftGradient_ = gradientValue.getImplementation();
  adv.loadAttribute( "leftHessian_", hessianValue );
  p_leftHessian_ = hessianValue.getImplementation();
  adv.loadAttribute( "rightFunction_", evaluationValue );
  p_rightFunction_ = evaluationValue.getImplementation();
  adv.loadAttribute( "rightGradient_", gradientValue );
  p_rightGradient_ = gradientValue.getImplementation();
  adv.loadAttribute( "rightHessian_", hessianValue );
  p_rightHessian_ = hessianValue.getImplementation();
}

END_NAMESPACE_OPENTURNS
