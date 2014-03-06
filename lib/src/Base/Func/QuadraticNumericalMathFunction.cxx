//                                               -*- C++ -*-
/**
 *  @file  QuadraticNumericalMathFunction.cxx
 *  @brief The class that implements quadratic numerical math functions.
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
#include "QuadraticNumericalMathFunction.hxx"
#include "QuadraticNumericalMathEvaluationImplementation.hxx"
#include "LinearNumericalMathGradientImplementation.hxx"
#include "ConstantNumericalMathHessianImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(QuadraticNumericalMathFunction);

/* Default constructor */
QuadraticNumericalMathFunction::QuadraticNumericalMathFunction (const NumericalPoint & center,
    const NumericalPoint & constant,
    const Matrix & linear,
    const SymmetricTensor & quadratic)
  : NumericalMathFunction(new QuadraticNumericalMathEvaluationImplementation(center, constant, linear, quadratic),
                          new LinearNumericalMathGradientImplementation(center, linear, quadratic),
                          new ConstantNumericalMathHessianImplementation(quadratic))
{
  // Nothing to do
}

/* Comparison operator */
Bool QuadraticNumericalMathFunction::operator ==(const QuadraticNumericalMathFunction & other) const
{
  return true;
}

/* String converter */
String QuadraticNumericalMathFunction::__repr__() const
{
  OSS oss;
  oss << "class=" << QuadraticNumericalMathFunction::GetClassName()
      << " name=" << getName()
      << " implementation=" << getImplementation()->__repr__();
  return oss;
}

String QuadraticNumericalMathFunction::__str__(const String & offset) const
{
  OSS oss;
  oss << "class=" << QuadraticNumericalMathFunction::GetClassName()
      << " name=" << getName()
      << " implementation=" << getImplementation()->__str__();
  return oss;
}

END_NAMESPACE_OPENTURNS
