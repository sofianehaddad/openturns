//                                               -*- C++ -*-
/**
 *  @file  ParametricEvaluationImplementation.cxx
 *  @brief ParametricEvaluationImplementation
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
 *  Id:      $Id: ParametricEvaluationImplementation.cxx 995 2008-10-31 10:52:04Z dutka $
 */
#include "ParametricEvaluationImplementation.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ParametricEvaluationImplementation);

static Factory<ParametricEvaluationImplementation> RegisteredFactory("ParametricEvaluationImplementation");


/* Default constructor */
ParametricEvaluationImplementation::ParametricEvaluationImplementation()
  : NumericalMathEvaluationImplementation()
  , function_()
  , parametersPositions_(0)
  , inputPositions_(0)
{
  // Nothing to do
}

/* Parameter constructor */
ParametricEvaluationImplementation::ParametricEvaluationImplementation(const NumericalMathFunction & function,
    const Indices & set,
    const NumericalPoint & referencePoint,
    const Bool parametersSet)
  : NumericalMathEvaluationImplementation()
  , function_(function)
  , parametersPositions_(0)
  , inputPositions_(0)
{
  const UnsignedInteger inputDimension(function.getInputDimension());
  // Check if the given reference point has a dimension compatible with the function
  if (referencePoint.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given reference point dimension=" << referencePoint.getDimension() << " does not match the function input dimension=" << inputDimension;
  // Check if the given parameters positions are compatible with the input dimension of the function
  const UnsignedInteger setDimension(set.getSize());
  if (inputDimension < setDimension) throw InvalidArgumentException(HERE) << "Error: the size of the " << (parametersSet ? "parameters" : "input") << " positions=" << setDimension << " is greater than the input dimension=" << inputDimension << " of the function.";
  // Check if the given indices are valid
  if (!set.check(inputDimension)) throw InvalidArgumentException(HERE) << "Error: the given set of positions contain either duplicate positions or positions greater than the input dimension of the function.";
  // Deduce the input position from the input dimension of the function and the parameters positions
  // Warning! the parameters positions can be in any order
  Indices fullIndices(inputDimension);
  Indices otherSet(0);
  fullIndices.fill();
  // Flag by inputDimension all the positions already used by the parameters
  for (UnsignedInteger i = 0; i < setDimension; ++i) fullIndices[set[i]] = inputDimension;
  // Then gather all the available indices
  for (UnsignedInteger i = 0; i < inputDimension; ++i)
    if (fullIndices[i] != inputDimension) otherSet.add(i);
  if (parametersSet)
  {
    parametersPositions_ = set;
    inputPositions_ = otherSet;
  }
  else
  {
    parametersPositions_ = otherSet;
    inputPositions_ = set;
  }
  // Set the relevant part of the reference point in the parameters
  const UnsignedInteger parametersSize(parametersPositions_.getSize());
  NumericalPointWithDescription parameters(parametersSize);
  Description description(parametersSize);
  for (UnsignedInteger i = 0; i < parametersSize; ++i)
  {
    parameters[i] = referencePoint[parametersPositions_[i]];
    description[i] = function.getInputDescription()[i];
  }
  parameters.setDescription(description);
  // Here we cannot use the accessor as the dimension of the parameters is set at the first allocation
  parameters_ = parameters;
  // And finally the input/output descriptions
  const Description functionInputDescription(function.getInputDescription());
  Description inputDescription(0);
  for (UnsignedInteger i = 0; i < inputPositions_.getSize(); ++i) inputDescription.add(functionInputDescription[inputPositions_[i]]);
  setInputDescription(inputDescription);
  setOutputDescription(function_.getOutputDescription());
}

/* Virtual constructor method */
ParametricEvaluationImplementation * ParametricEvaluationImplementation::clone() const
{
  return new ParametricEvaluationImplementation(*this);
}

/* Evaluation operator */
NumericalPoint ParametricEvaluationImplementation::operator() (const NumericalPoint & point) const
{
  const UnsignedInteger inputDimension(function_.getInputDimension());
  const UnsignedInteger pointDimension(point.getDimension());
  const UnsignedInteger parametersDimension(getParametersDimension());
  if (pointDimension + parametersDimension != inputDimension) throw InvalidArgumentException(HERE) << "Error: expected a point of dimension=" << inputDimension - parametersDimension << ", got dimension=" << pointDimension;
  NumericalPoint x(inputDimension);
  for (UnsignedInteger i = 0; i < parametersDimension; ++i) x[parametersPositions_[i]] = parameters_[i];
  for (UnsignedInteger i = 0; i < pointDimension; ++i) x[inputPositions_[i]] = point[i];
  return function_(x);
}

/* Parameters accessor */
void ParametricEvaluationImplementation::setParameters(const NumericalPointWithDescription & parameters)
{
  const UnsignedInteger parametersDimension(parameters.getDimension());
  if (parametersDimension != parametersPositions_.getSize()) throw InvalidArgumentException(HERE) << "Error: expected a parameters of dimension=" << parametersPositions_.getSize() << ", got dimension=" << parametersDimension;
  parameters_ = parameters;
}

/* Parameters accessor */
void ParametricEvaluationImplementation::setParameters(const NumericalPoint & parameters)
{
  const UnsignedInteger parametersDimension(parameters.getDimension());
  if (parametersDimension != parametersPositions_.getSize()) throw InvalidArgumentException(HERE) << "Error: expected a parameters of dimension=" << parametersPositions_.getSize() << ", got dimension=" << parametersDimension;
  for (UnsignedInteger i = 0; i < parametersDimension; ++i) parameters_[i] = parameters[i];
}

/* Parameters positions accessor */
Indices ParametricEvaluationImplementation::getParametersPositions() const
{
  return parametersPositions_;
}

/* Input positions accessor */
Indices ParametricEvaluationImplementation::getInputPositions() const
{
  return inputPositions_;
}

/* Function accessor */
NumericalMathFunction ParametricEvaluationImplementation::getFunction() const
{
  return function_;
}

/* Dimension accessor */
UnsignedInteger ParametricEvaluationImplementation::getInputDimension() const
{
  return inputPositions_.getSize();
}

UnsignedInteger ParametricEvaluationImplementation::getParametersDimension() const
{
  return parametersPositions_.getSize();
}

UnsignedInteger ParametricEvaluationImplementation::getOutputDimension() const
{
  return function_.getOutputDimension();
}

/* String converter */
String ParametricEvaluationImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << ParametricEvaluationImplementation::GetClassName()
      << " function=" << function_
      << " parameters positions=" << parametersPositions_
      << " parameters=" << parameters_
      << " input positions=" << inputPositions_;
  return oss;
}

/* Method save() stores the object through the StorageManager */
void ParametricEvaluationImplementation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save( adv );
  adv.saveAttribute( "function_", function_ );
  adv.saveAttribute( "parametersPositions_", parametersPositions_ );
  adv.saveAttribute( "parameters_", parameters_ );
  adv.saveAttribute( "inputPositions_", inputPositions_ );
}

/* Method load() reloads the object from the StorageManager */
void ParametricEvaluationImplementation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load( adv );
  adv.loadAttribute( "function_", function_ );
  adv.loadAttribute( "parametersPositions_", parametersPositions_ );
  adv.loadAttribute( "parameters_", parameters_ );
  adv.loadAttribute( "inputPositions_", inputPositions_ );
}


END_NAMESPACE_OPENTURNS
