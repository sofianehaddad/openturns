//                                               -*- C++ -*-
/**
 *  @file  QuasiMonteCarloResult.hxx
 *  @brief Implementation of SimulationResult
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
 *  @date   2008-05-23 13:46:12 +0200 (Fri, 23 May 2008)
 */
#ifndef OPENTURNS_QUASIMONTECARLORESULT_HXX
#define OPENTURNS_QUASIMONTECARLORESULT_HXX

#include "SimulationResultImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class QuasiMonteCarloResult
 * Implementation of SimulationResult
 */
class QuasiMonteCarloResult
  : public SimulationResultImplementation
{

  CLASSNAME;
public:


  /** Default constructor */
  QuasiMonteCarloResult();

  /** Standard constructor */
  QuasiMonteCarloResult(const Event & event,
                        const NumericalScalar probabilityEstimate,
                        const NumericalScalar varianceEstimate,
                        const UnsignedInteger outerSampling,
                        const UnsignedInteger blockSize);

  /** Virtual constructor */
  virtual QuasiMonteCarloResult * clone() const;

  /** Coefficient of variation estimate accessor */
  virtual NumericalScalar getCoefficientOfVariation() const;

  /** Standard deviation estimate accessor */
  virtual NumericalScalar getStandardDeviation() const;

  /** Confidence length */
  virtual NumericalScalar getConfidenceLength(const NumericalScalar level = ResourceMap::GetAsNumericalScalar( "SimulationResultImplementation-DefaultConfidenceLevel" )) const;

  /** String converter */
  virtual String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

private:

}; // class QuasiMonteCarloResult

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_QUASIMONTECARLORESULT_HXX */
