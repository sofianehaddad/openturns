//                                               -*- C++ -*-
/**
 *  @brief Regression from a data sample upon a particular basis
 *
 *  Copyright 2005-2025 Airbus-EDF-IMACS-ONERA-Phimeca
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
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "openturns/PersistentObjectFactory.hxx"
#include "openturns/ApproximationAlgorithmImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ApproximationAlgorithmImplementation)

static const Factory<ApproximationAlgorithmImplementation> Factory_ApproximationAlgorithmImplementation;

/* Default constructor */
ApproximationAlgorithmImplementation::ApproximationAlgorithmImplementation()
  : PersistentObject()
{
  // Nothing to do
}

/* Default constructor */
ApproximationAlgorithmImplementation::ApproximationAlgorithmImplementation(const Sample & x,
    const Sample & y,
    const FunctionCollection & psi,
    const Indices & indices)
  : PersistentObject()
  , x_(x)
  , y_(y)
  , weight_(Point( x.getSize(), 1.0 ))
  , hasUniformWeight_(true)
  , psi_(psi)
  , currentIndices_(indices)
{
  const UnsignedInteger dataSize = x.getSize();
  if (dataSize == 0) throw InvalidArgumentException(HERE) << "Error: cannot perform approximation based on an empty sample.";
  if (dataSize != y.getSize()) throw InvalidArgumentException(HERE) << "Error: cannot perform approximation based on samples of different size.";
  if (y.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot perform approximation based on output sample of dimension different from 1.";
}

/* Default constructor */
ApproximationAlgorithmImplementation::ApproximationAlgorithmImplementation(const Sample & x,
    const Sample & y,
    const Point & weight,
    const FunctionCollection & psi,
    const Indices & indices)
  : PersistentObject()
  , x_(x)
  , y_(y)
  , weight_()
  , psi_(psi)
  , currentIndices_(indices)
{
  UnsignedInteger dataSize = x.getSize();
  if (dataSize == 0) throw InvalidArgumentException(HERE) << "Error: cannot perform approximation based on an empty sample.";
  if (dataSize != y.getSize()) throw InvalidArgumentException(HERE) << "Error: cannot perform approximation based on samples of different size.";
  if (y.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot perform approximation based on output sample of dimension different from 1.";
  if ( dataSize != weight.getDimension() ) throw InvalidArgumentException(HERE) << "Error: cannot perform approximation with weight of improper dimension.";
  setWeight(weight);
}

/* Virtual constructor */
ApproximationAlgorithmImplementation * ApproximationAlgorithmImplementation::clone() const
{
  return new ApproximationAlgorithmImplementation(*this);
}

/* Accessors */
Sample ApproximationAlgorithmImplementation::getX() const
{
  return x_;
}

Sample ApproximationAlgorithmImplementation::getY() const
{
  return y_;
}

/* Weight accessors */
void ApproximationAlgorithmImplementation::setWeight(const Point & weight)
{
  const UnsignedInteger size = weight.getSize();
  if (size > 0)
  {
    const Scalar firstWeight = weight[0];
    hasUniformWeight_ = true;
    for (UnsignedInteger i = 0; i < size; ++ i)
    {
      const Scalar wI = weight[i];
      if (!(wI > 0.0)) throw InvalidArgumentException(HERE) << "Error: can only use positive weight.";
      hasUniformWeight_ = hasUniformWeight_ && (wI == firstWeight);
    }
  }
  weight_ = weight;
}

Point ApproximationAlgorithmImplementation::getWeight() const
{
  return weight_;
}

ApproximationAlgorithmImplementation::FunctionCollection ApproximationAlgorithmImplementation::getPsi() const
{
  return psi_;
}

/* Perform the selection */
void ApproximationAlgorithmImplementation::run()
{
  const DesignProxy proxy(x_, psi_);
  run(proxy);
}

void ApproximationAlgorithmImplementation::run(const DesignProxy & )
{
  throw NotYetImplementedException(HERE) << "In ApproximationAlgorithmImplementation::run(const DesignProxy & proxy)";
}

/* String converter */
String ApproximationAlgorithmImplementation::__repr__() const
{
  return OSS(true) << "class=" << getClassName()
         << " x=" << x_
         << " y=" << y_
         << " weight=" << weight_
         << " psi=" << psi_;
}

String ApproximationAlgorithmImplementation::__str__(const String & ) const
{
  return __repr__();
}

void ApproximationAlgorithmImplementation::setCoefficients(const Point & coefficients)
{
  coefficients_ = coefficients;
  isAlreadyComputedCoefficients_ = true;
}

Point ApproximationAlgorithmImplementation::getCoefficients()
{
  if (! isAlreadyComputedCoefficients_) run();

  return coefficients_;
}

void ApproximationAlgorithmImplementation::setResidual(const Scalar residual)
{
  residual_ = residual;
}

Scalar ApproximationAlgorithmImplementation::getResidual()
{
  if (! isAlreadyComputedCoefficients_) run();
  return residual_;
}

void ApproximationAlgorithmImplementation::setRelativeError(const Scalar relativeError)
{
  relativeError_ = relativeError;
}

Scalar ApproximationAlgorithmImplementation::getRelativeError()
{
  if (! isAlreadyComputedCoefficients_) run();
  return relativeError_;
}

Bool ApproximationAlgorithmImplementation::involvesModelSelection() const
{
  throw NotYetImplementedException(HERE) << "In ApproximationAlgorithmImplementation::involvesModelSelection()";
}

/* Method save() stores the object through the StorageManager */
void ApproximationAlgorithmImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute("x_", x_);
  adv.saveAttribute("y_", y_);
  adv.saveAttribute("weight_", weight_);
  adv.saveAttribute("psi_", psi_);
  adv.saveAttribute("currentIndices_", currentIndices_);
}


/* Method load() reloads the object from the StorageManager */
void ApproximationAlgorithmImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  Point weight;
  adv.loadAttribute("x_", x_);
  adv.loadAttribute("y_", y_);
  adv.loadAttribute("weight_", weight);
  adv.loadAttribute("psi_", psi_);
  adv.loadAttribute("currentIndices_", currentIndices_);
  setWeight(weight);
}

Collection<Indices> ApproximationAlgorithmImplementation::getSelectionHistory(Collection<Point> & /*coefficientsHistory*/) const
{
  throw NotYetImplementedException(HERE) << "in ApproximationAlgorithmImplementation::getSelectionHistory";
}

Point ApproximationAlgorithmImplementation::getErrorHistory() const
{
  throw NotYetImplementedException(HERE) << "in ApproximationAlgorithmImplementation::getErrorHistory";
}

END_NAMESPACE_OPENTURNS
