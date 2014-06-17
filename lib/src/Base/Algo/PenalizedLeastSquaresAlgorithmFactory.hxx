//                                               -*- C++ -*-
/**
 *  @file  PenalizedLeastSquaresAlgorithmFactory.hxx
 *  @brief A factory for building PenalizedLeastSquaresAlgorithm objects
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
 *  @date   2010-11-09 13:44:00 +0100 (Tue, 09 Nov 2010)
 */
#ifndef OPENTURNS_PENALIZEDLEASTSQUARESALGORITHMFACTORY_HXX
#define OPENTURNS_PENALIZEDLEASTSQUARESALGORITHMFACTORY_HXX

#include "NumericalPoint.hxx"
#include "PenalizedLeastSquaresAlgorithm.hxx"
#include "ApproximationAlgorithmImplementationFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class PenalizedLeastSquaresAlgorithmFactory
 *
 * A factory for building PenalizedLeastSquaresAlgorithm objects
 */
class OT_API PenalizedLeastSquaresAlgorithmFactory
  : public ApproximationAlgorithmImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  PenalizedLeastSquaresAlgorithmFactory();

  /** Virtual constructor */
  virtual PenalizedLeastSquaresAlgorithmFactory * clone() const;

  /** Method to create new ApproximationAlgorithmImplementation objects */
  virtual PenalizedLeastSquaresAlgorithm * build(const NumericalSample & x,
      const NumericalSample & y,
      const NumericalPoint & weight,
      const Basis & psi) const;

  /** String converter */
  virtual String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

}; /* class PenalizedLeastSquaresAlgorithmFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PENALIZEDLEASTSQUARESALGORITHMFACTORY_HXX */
