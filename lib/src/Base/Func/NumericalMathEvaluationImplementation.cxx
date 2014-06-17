//                                               -*- C++ -*-
/**
 * @file  NumericalMathEvaluationImplementation.cxx
 * @brief Abstract top-level class for all distributions
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
 * @author schueller
 * @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)
 */
#include <algorithm>
#include <iterator>

#include "NumericalMathEvaluationImplementation.hxx"
#include "ComposedNumericalMathEvaluationImplementation.hxx"
#include "OTconfig.hxx"
#ifdef OPENTURNS_HAVE_MUPARSER
#include "AnalyticalNumericalMathEvaluationImplementation.hxx"
#endif
#include "LinearNumericalMathEvaluationImplementation.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"
#include "Full.hxx"
#include "Contour.hxx"
#include "Curve.hxx"
#include "Indices.hxx"
#include "Box.hxx"

BEGIN_NAMESPACE_OPENTURNS

typedef NumericalMathEvaluationImplementation::CacheType NumericalMathEvaluationImplementationCache;

static Factory<NumericalMathEvaluationImplementationCache> RegisteredFactory_Cache("Cache<PersistentCollection<NumericalScalar>, PersistentCollection<NumericalScalar>>");

/* These methods are implemented here for the needs of Cache */
/* We should be careful because they may interfere with other definitions placed elsewhere */
TEMPLATE_CLASSNAMEINIT(PersistentCollection<UnsignedInteger>);
static Factory<PersistentCollection<UnsignedInteger> > RegisteredFactory_alt1("PersistentCollection<UnsignedInteger>");
#ifndef OPENTURNS_UNSIGNEDLONG_SAME_AS_UINT64
TEMPLATE_CLASSNAMEINIT(PersistentCollection<Unsigned64BitsInteger>);
static Factory<PersistentCollection<Unsigned64BitsInteger> > RegisteredFactory_alt1b("PersistentCollection<Unsigned64BitsInteger>");
#endif

TEMPLATE_CLASSNAMEINIT(PersistentCollection<PersistentCollection<NumericalScalar> >);
static Factory<PersistentCollection<PersistentCollection<NumericalScalar> > > RegisteredFactory_alt2("PersistentCollection<PersistentCollection<NumericalScalar> >");


CLASSNAMEINIT(NumericalMathEvaluationImplementation);

static Factory<NumericalMathEvaluationImplementation> RegisteredFactory("NumericalMathEvaluationImplementation");


/* Default constructor */
NumericalMathEvaluationImplementation::NumericalMathEvaluationImplementation()
  : PersistentObject()
  , callsNumber_(0)
  , p_cache_(new CacheType)
  , inputStrategy_(Full())
  , outputStrategy_(Full())
  , isHistoryEnabled_(false)
  , inputDescription_(0)
  , outputDescription_(0)
  , parameters_(0)
{
  // We disable the cache by default
  p_cache_->disable();
}

/* Virtual constructor */
NumericalMathEvaluationImplementation * NumericalMathEvaluationImplementation::clone() const
{
  return new NumericalMathEvaluationImplementation(*this);
}


/* Comparison operator */
Bool NumericalMathEvaluationImplementation::operator ==(const NumericalMathEvaluationImplementation & other) const
{
  return true;
}

/* String converter */
String NumericalMathEvaluationImplementation::__repr__() const
{
  OSS oss(true);
  oss << "class=" << NumericalMathEvaluationImplementation::GetClassName()
      << " name=" << getName()
      << " input description=" << inputDescription_
      << " output description=" << outputDescription_
      << " parameters=" << parameters_;
  return oss;
}

/* String converter */
String NumericalMathEvaluationImplementation::__str__(const String & offset) const
{
  return OSS(false) << offset << "NumericalMathEvaluationImplementation";
}

/* Description Accessor */
void NumericalMathEvaluationImplementation::setDescription(const Description & description)
{
  if (description.getSize() != getInputDimension() + getOutputDimension()) throw InvalidArgumentException(HERE) << "Error: the description must have a size of input dimension + output dimension, here size=" << description.getSize() << ", input dimension=" << getInputDimension() << ", output dimension=" << getOutputDimension();
  inputDescription_ = Description(getInputDimension());
  std::copy(description.begin(), description.begin() + getInputDimension(), inputDescription_.begin());
  outputDescription_ = Description(getOutputDimension());
  std::copy(description.begin() + getInputDimension(), description.begin() + description.getSize(), outputDescription_.begin());
}


/* Description Accessor */
Description NumericalMathEvaluationImplementation::getDescription() const
{
  Description description(getInputDescription());
  Description outputDescription(getOutputDescription());
  for (UnsignedInteger i = 0; i < getOutputDimension(); ++i) description.add(outputDescription[i]);
  return description;
}

/* Input description Accessor */
void NumericalMathEvaluationImplementation::setInputDescription(const Description & inputDescription)
{
  if (inputDescription.getSize() != getInputDimension()) throw InvalidArgumentException(HERE) << "Error: the input description must have a size=" << inputDescription.getSize() << " equal to the input dimension=" << getInputDimension();
  inputDescription_ = inputDescription;
}

Description NumericalMathEvaluationImplementation::getInputDescription() const
{
  if (inputDescription_.getSize() == 0) return Description::BuildDefault(getInputDimension(), "x");
  return inputDescription_;
}

/* Output description Accessor */
void NumericalMathEvaluationImplementation::setOutputDescription(const Description & outputDescription)
{
  if (outputDescription.getSize() != getOutputDimension()) throw InvalidArgumentException(HERE) << "Error: the output description must have a size=" << outputDescription.getSize() << " equal to the output dimension=" << getOutputDimension();
  outputDescription_ = outputDescription;
}

Description NumericalMathEvaluationImplementation::getOutputDescription() const
{
  if (outputDescription_.getSize() == 0) return Description::BuildDefault(getOutputDimension(), "y");
  return outputDescription_;
}

/* Test for actual implementation */
Bool NumericalMathEvaluationImplementation::isActualImplementation() const
{
  return true;
}

/* Here is the interface that all derived class must implement */

/* Operator () */
NumericalSample NumericalMathEvaluationImplementation::operator() (const NumericalSample & inSample) const
{
  const UnsignedInteger inputDimension(getInputDimension());
  if (inSample.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given sample has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inSample.getDimension();

  const UnsignedInteger size(inSample.getSize());
  NumericalSample outSample(size, getOutputDimension());
  // Simple loop over the evaluation operator based on point
  // The calls number is updated by these calls
  for (UnsignedInteger i = 0; i < size; ++i) outSample[i] = operator()(inSample[i]);
  outSample.setDescription(getOutputDescription());
  return outSample;
}


/* Operator () */
Field NumericalMathEvaluationImplementation::operator() (const Field & inField) const
{
  const UnsignedInteger inputDimension(getInputDimension());
  if (inField.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given time series has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inField.getDimension();
  return Field(inField.getMesh(), operator()(inField.getValues()));
}


/* Enable or disable the internal cache */
void NumericalMathEvaluationImplementation::enableCache() const
{
  p_cache_->enable();
}

void NumericalMathEvaluationImplementation::disableCache() const
{
  p_cache_->disable();
}

Bool NumericalMathEvaluationImplementation::isCacheEnabled() const
{
  return p_cache_->isEnabled();
}

UnsignedInteger NumericalMathEvaluationImplementation::getCacheHits() const
{
  return p_cache_->getHits();
}

void NumericalMathEvaluationImplementation::addCacheContent(const NumericalSample& inSample, const NumericalSample& outSample)
{
  p_cache_->enable();
  const UnsignedInteger size = inSample.getSize();
  for ( UnsignedInteger i = 0; i < size; ++ i )
  {
    p_cache_->add( inSample[i], outSample[i] );
  }
}

NumericalSample NumericalMathEvaluationImplementation::getCacheInput() const
{
  Bool cacheEnabled = isCacheEnabled();
  enableCache();
  PersistentCollection<CacheKeyType> keyColl( p_cache_->getKeys() );
  if ( ! cacheEnabled )
    disableCache();
  NumericalSample inSample(0, getInputDimension());
  for ( UnsignedInteger i = 0; i < keyColl.getSize(); ++ i ) inSample.add( keyColl[i] );
  return inSample;
}

NumericalSample NumericalMathEvaluationImplementation::getCacheOutput() const
{
  Bool cacheEnabled = isCacheEnabled();
  enableCache();
  PersistentCollection<CacheValueType> valuesColl( p_cache_->getValues() );
  if ( ! cacheEnabled )
    disableCache();
  NumericalSample outSample(0, getOutputDimension());
  for ( UnsignedInteger i = 0; i < valuesColl.getSize(); ++ i )
  {
    outSample.add( valuesColl[i] );
  }
  return outSample;
}

void NumericalMathEvaluationImplementation::clearCache() const
{
  p_cache_->clear();
}

/* Enable or disable the input/output history */
void NumericalMathEvaluationImplementation::enableHistory() const
{
  isHistoryEnabled_ = true;
}

void NumericalMathEvaluationImplementation::disableHistory() const
{
  isHistoryEnabled_ = false;
}

Bool NumericalMathEvaluationImplementation::isHistoryEnabled() const
{
  return isHistoryEnabled_;
}

void NumericalMathEvaluationImplementation::clearHistory() const
{
  inputStrategy_ = Full();
  outputStrategy_ = Full();
}

HistoryStrategy NumericalMathEvaluationImplementation::getInputHistory() const
{
  return inputStrategy_;
}

HistoryStrategy NumericalMathEvaluationImplementation::getOutputHistory() const
{
  return outputStrategy_;
}

/* Gradient according to the marginal parameters */
Matrix NumericalMathEvaluationImplementation::parametersGradient(const NumericalPoint & inP) const
{
  return Matrix(parameters_.getDimension(), getOutputDimension());
}

/* Parameters value and description accessor */
NumericalPointWithDescription NumericalMathEvaluationImplementation::getParameters() const
{
  return parameters_;
}

void NumericalMathEvaluationImplementation::setParameters(const NumericalPointWithDescription & parameters)
{
  parameters_ = parameters;
}

void NumericalMathEvaluationImplementation::setParameters(const NumericalPoint & parameters)
{
  parameters_ = parameters;
}

/* Operator () */
NumericalPoint NumericalMathEvaluationImplementation::operator() (const NumericalPoint & inP) const
{
  throw NotYetImplementedException(HERE);
}

NumericalPoint NumericalMathEvaluationImplementation::operator() (const NumericalPoint & inP,
								  const NumericalPoint & parameters)
{
  setParameters(parameters);
  return (*this)(inP);
}

/* Accessor for input point dimension */
UnsignedInteger NumericalMathEvaluationImplementation::getInputDimension() const
{
  throw NotYetImplementedException(HERE);
}

/* Accessor for output point dimension */
UnsignedInteger NumericalMathEvaluationImplementation::getOutputDimension() const
{
  throw NotYetImplementedException(HERE);
}

/* Accessor for input point dimension */
UnsignedInteger NumericalMathEvaluationImplementation::getParametersDimension() const
{
  return parameters_.getDimension();
}

/* Get the i-th marginal function */
NumericalMathEvaluationImplementation::Implementation NumericalMathEvaluationImplementation::getMarginal(const UnsignedInteger i) const
{
  if (i >= getOutputDimension()) throw InvalidArgumentException(HERE) << "Error: the index of a marginal function must be in the range [0, outputDimension-1]";
  return getMarginal(Indices(1, i));
}

/* Get the function corresponding to indices components */
NumericalMathEvaluationImplementation::Implementation NumericalMathEvaluationImplementation::getMarginal(const Indices & indices) const
{
  if (!indices.check(getOutputDimension() - 1)) throw InvalidArgumentException(HERE) << "Error: the indices of a marginal function must be in the range [0, outputDimension-1] and  must be different";
  // We build an analytical function that extract the needed component
  // If X1,...,XN are the descriptions of the input of this function, it is a function from R^n to R^p
  // with formula Yk = Xindices[k] for k=1,...,p
  // Build non-ambigous names for the inputs. We cannot simply use the output description, as it must be valid muParser identifiers
  const UnsignedInteger inputDimension(getOutputDimension());
  const UnsignedInteger outputDimension(indices.getSize());
#ifdef OPENTURNS_HAVE_MUPARSER
  Description input(inputDimension);
  for (UnsignedInteger index = 0; index < inputDimension; ++index)
    input[index] = OSS() << "x" << index;
  // Extract the components
  Description output(outputDimension);
  Description formulas(outputDimension);
  Description currentOutputDescription(getOutputDescription());
  for (UnsignedInteger index = 0; index < outputDimension; ++index)
  {
    output[index] = currentOutputDescription[indices[index]];
    formulas[index] = input[indices[index]];
  }
  const AnalyticalNumericalMathEvaluationImplementation left(input, output, formulas);
#else
  NumericalPoint center(inputDimension);
  Matrix linear(inputDimension, outputDimension);
  for ( UnsignedInteger index = 0; index < outputDimension; ++ index )
    linear(indices[index], index) = 1.0;
  NumericalPoint constant(outputDimension);
  const LinearNumericalMathEvaluationImplementation left(center, constant, linear);
#endif
  ComposedNumericalMathEvaluationImplementation marginal(left.clone(), clone());
  if (isHistoryEnabled())
  {
    marginal.enableHistory();
  }
  return marginal.clone();
}

/* Get the number of calls to operator() */
UnsignedInteger NumericalMathEvaluationImplementation::getCallsNumber() const
{
  return callsNumber_;
}


/* Draw the given 1D marginal output as a function of the given 1D marginal input around the given central point */
Graph NumericalMathEvaluationImplementation::draw(const UnsignedInteger inputMarginal,
    const UnsignedInteger outputMarginal,
    const NumericalPoint & centralPoint,
    const NumericalScalar xMin,
    const NumericalScalar xMax,
    const UnsignedInteger pointNumber) const
{
  if (getInputDimension() < 1) throw InvalidArgumentException(HERE) << "Error: cannot use this version of the draw() method with a function of input dimension less than 1";
  if (inputMarginal >= getInputDimension()) throw InvalidArgumentException(HERE) << "Error: the given input marginal index=" << inputMarginal << " must be less than the input dimension=" << getInputDimension();
  if (outputMarginal >= getOutputDimension()) throw InvalidArgumentException(HERE) << "Error: the given output marginal index=" << outputMarginal << " must be less than the output dimension=" << getOutputDimension();
  NumericalSample inputData(pointNumber, centralPoint);
  const NumericalScalar dx((xMax - xMin) / (pointNumber - 1.0));
  for (UnsignedInteger i = 0; i < pointNumber; ++i)
    inputData[i][inputMarginal] = xMin + i * dx;
  // Evaluate the function over all its input in one call in order to benefit from potential parallelism
  const NumericalSample outputData((*this)(inputData));
  const Description inputDescription(getInputDescription());
  const Description outputDescription(getOutputDescription());
  const String xName(inputDescription[inputMarginal]);
  const String yName(outputDescription[outputMarginal]);
  String title(OSS() << yName << " as a function of " << xName);
  if (centralPoint.getDimension() > 1) title = String(OSS(false) << title << " around " << centralPoint);
  Graph graph(title, xName, yName, true, "");
  graph.add(Curve(inputData.getMarginal(inputMarginal), outputData.getMarginal(outputMarginal)));
  // Add a slight vertical margin
  GraphImplementation::BoundingBox bb(graph.getBoundingBox());
  const NumericalScalar height(bb[3] - bb[2]);
  bb[2] -= 0.05 * height;
  bb[3] += 0.05 * height;
  graph.setBoundingBox(bb);
  return graph;
}

/* Draw the given 1D marginal output as a function of the given 2D marginal input around the given central point */
Graph NumericalMathEvaluationImplementation::draw(const UnsignedInteger firstInputMarginal,
    const UnsignedInteger secondInputMarginal,
    const UnsignedInteger outputMarginal,
    const NumericalPoint & centralPoint,
    const NumericalPoint & xMin,
    const NumericalPoint & xMax,
    const Indices & pointNumber) const
{
  if (getInputDimension() < 2) throw InvalidArgumentException(HERE) << "Error: cannot use this version of the draw() method with a function of input dimension less than 2";
  if ((xMin.getDimension() != 2) || (xMax.getDimension() != 2) || (pointNumber.getSize() != 2)) throw InvalidArgumentException(HERE) << "Error: xMin, xMax and PointNumber must be bidimensional";
  if ((pointNumber[0] <= 2) || (pointNumber[1] <= 2)) throw InvalidArgumentException(HERE) << "Error: the discretization must have at least 2 points per component";
  NumericalPoint discretization(2);
  NumericalPoint scaling(2);
  NumericalPoint origin(2);
  const NumericalScalar nX(pointNumber[0] - 2);
  discretization[0] = nX;
  // Discretization of the first component
  NumericalSample x(Box(NumericalPoint(1, nX)).generate());
  origin[0] = xMin[0];
  scaling[0] = xMax[0] - xMin[0];
  x.scale(NumericalPoint(1, scaling[0]));
  x.translate(NumericalPoint(1, origin[0]));
  const NumericalScalar nY(pointNumber[1] - 2);
  discretization[1] = nY;
  // Discretization of the second component
  NumericalSample y(Box(NumericalPoint(1, nY)).generate());
  origin[1] = xMin[1];
  scaling[1] = xMax[1] - xMin[1];
  y.scale(NumericalPoint(1, scaling[1]));
  y.translate(NumericalPoint(1, origin[1]));
  // Discretization of the XY plane
  NumericalSample xy(Box(discretization).generate());
  xy.scale(scaling);
  xy.translate(origin);
  // Prepare the input sample
  const UnsignedInteger sizeXY(xy.getSize());
  NumericalSample inputSample(sizeXY, centralPoint);
  for (UnsignedInteger i = 0; i < sizeXY; ++i)
  {
    inputSample[i][firstInputMarginal]  = xy[i][0];
    inputSample[i][secondInputMarginal] = xy[i][1];
  }
  // Compute the output sample, using possible parallelism
  const NumericalSample z((*this)(inputSample).getMarginal(outputMarginal));
  const String xName(getInputDescription()[firstInputMarginal]);
  const String yName(getInputDescription()[secondInputMarginal]);
  String title(OSS() << getOutputDescription()[outputMarginal] << " as a function of (" << xName << "," << yName << ")");
  if (centralPoint.getDimension() > 2) title = String(OSS(false) << title << " around " << centralPoint);
  Graph graph(title, xName, yName, true, "topright");
  Contour isoValues(Contour(x, y, z, NumericalPoint(0), Description(0), true, title));
  isoValues.buildDefaultLevels();
  isoValues.buildDefaultLabels();
  NumericalPoint levels(isoValues.getLevels());
  Description labels(isoValues.getLabels());
  for (UnsignedInteger i = 0; i < levels.getDimension(); ++i)
    {
      Contour current(isoValues);
      current.setLevels(NumericalPoint(1, levels[i]));
      current.setLabels(Description(1, labels[i]));
      current.setDrawLabels(false);
      current.setLegend(labels[i]);
      current.setColor(Contour::ConvertFromHSV((360.0 * i / levels.getDimension()), 1.0, 1.0));
      graph.add(current);
    }
  return graph;
}

/* Draw the output of the function with respect to its input when the input and output dimensions are 1 */
Graph NumericalMathEvaluationImplementation::draw(const NumericalScalar xMin,
    const NumericalScalar xMax,
    const UnsignedInteger pointNumber) const
{
  if (getInputDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot draw a function with input dimension=" << getInputDimension() << " different from 1 using this method. See the other draw() methods.";
  if (getOutputDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot draw a function with output dimension=" << getInputDimension() << " different from 1 using this method. See the other draw() methods.";
  return draw(0, 0, NumericalPoint(1), xMin, xMax, pointNumber);
}

/* Draw the output of the function with respect to its input when the input dimension is 2 and the output dimension is 1 */
Graph NumericalMathEvaluationImplementation::draw(const NumericalPoint & xMin,
    const NumericalPoint & xMax,
    const Indices & pointNumber) const
{
  if (getInputDimension() != 2) throw InvalidArgumentException(HERE) << "Error: cannot draw a function with input dimension=" << getInputDimension() << " different from 2 using this method. See the other draw() methods.";
  if (getOutputDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot draw a function with output dimension=" << getInputDimension() << " different from 1 using this method. See the other draw() methods.";
  return draw(0, 1, 0, NumericalPoint(2), xMin, xMax, pointNumber);
}


/* Method save() stores the object through the StorageManager */
void NumericalMathEvaluationImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "callsNumber_", callsNumber_ );
  adv.saveAttribute( "cache_", *p_cache_ );
  adv.saveAttribute( "inputDescription_", inputDescription_ );
  adv.saveAttribute( "outputDescription_", outputDescription_ );
  adv.saveAttribute( "parameters_", parameters_ );
}

/* Method load() reloads the object from the StorageManager */
void NumericalMathEvaluationImplementation::load(Advocate & adv)
{
  TypedInterfaceObject<CacheType> cache;
  PersistentObject::load(adv);
  adv.loadAttribute( "callsNumber_", callsNumber_ );
  adv.loadAttribute( "cache_", cache );
  p_cache_ = cache.getImplementation();
  adv.loadAttribute( "inputDescription_", inputDescription_ );
  adv.loadAttribute( "outputDescription_", outputDescription_ );
  adv.loadAttribute( "parameters_", parameters_ );
}

END_NAMESPACE_OPENTURNS
