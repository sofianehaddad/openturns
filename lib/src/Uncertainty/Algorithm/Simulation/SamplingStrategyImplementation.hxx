//                                               -*- C++ -*-
/**
 *  @file  SamplingStrategyImplementation.hxx
 *  @brief
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
#ifndef OPENTURNS_SAMPLINGSTRATEGYIMPLEMENTATION_HXX
#define OPENTURNS_SAMPLINGSTRATEGYIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"
#include "PersistentObject.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class SamplingStrategyImplementation
 */

class OT_API SamplingStrategyImplementation
  : public PersistentObject
{

  CLASSNAME;

public:

  /** Constructor with parameters */
  explicit SamplingStrategyImplementation(const UnsignedInteger dimension);

  /** Virtual constructor */
  virtual SamplingStrategyImplementation * clone() const;

  /** Generate a set of directions */
  virtual NumericalSample generate() const;

  /** Generate a uniform random unit vector */
  NumericalPoint getUniformUnitVectorRealization(const UnsignedInteger dimension) const;

  /** Generate a uniform random unit vector */
  NumericalPoint getUniformUnitVectorRealization() const;

  /** Dimension accessor */
  void setDimension(const UnsignedInteger dimension);
  UnsignedInteger getDimension() const;

  /** String converter */
  virtual String __repr__() const;

protected:

  UnsignedInteger dimension_;

} ; /* class SamplingStrategyImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SAMPLINGSTRATEGYIMPLEMENTATION_HXX */
