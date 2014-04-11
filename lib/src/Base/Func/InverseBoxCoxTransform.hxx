//                                               -*- C++ -*-
/**
 *  @file  InverseBoxCoxTransform.hxx
 *  @brief Abstract top-level class for all numerical math function implementations
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
#ifndef OPENTURNS_INVERSEBOXCOXTRANSFORM_HXX
#define OPENTURNS_INVERSEBOXCOXTRANSFORM_HXX

#include "NumericalMathFunction.hxx"
#include "NumericalPoint.hxx"
#include "Collection.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class InverseBoxCoxTransform
 *
 * The class that simulates the classical Box Cox method which acts on the spatial part of a time series.
 */

class BoxCoxTransform;

class OT_API InverseBoxCoxTransform
  : public NumericalMathFunction

{
  CLASSNAME;
public:

  /** Default constructor */
  InverseBoxCoxTransform();

  /** Standard parameter constructor */
  InverseBoxCoxTransform(const NumericalPoint & lambda);

  InverseBoxCoxTransform(const NumericalPoint & lambda,
                         const NumericalPoint & shift);

  /** 1D NumericalScalar parameter constructor */
  InverseBoxCoxTransform(const NumericalScalar & lambda);

  InverseBoxCoxTransform(const NumericalScalar & lambda,
                         const NumericalScalar & shift);

  /** Virtual constructor */
  InverseBoxCoxTransform * clone() const;

  /** Lambda accessor */
  NumericalPoint getLambda () const;

  /** Shift accessor */
  NumericalPoint getShift () const;

  /** Inverse accessor */
  BoxCoxTransform getInverse() const;

private:

  /** Internal setDefaultDescription method */
  void setDefaultDescription();

}; /* class InverseBoxCoxTransform */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_INVERSEBOXCOXTRANSFORM_HXX */
