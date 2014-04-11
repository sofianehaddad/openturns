//                                               -*- C++ -*-
/**
 *  @file  ARMA.hxx
 *  @brief A class which implements the ARMA process
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#ifndef OPENTURNS_ARMA_HXX
#define OPENTURNS_ARMA_HXX

#include "ProcessImplementation.hxx"
#include "Process.hxx"
#include "Pointer.hxx"
#include "Distribution.hxx"
#include "ARMACoefficients.hxx"
#include "ARMAState.hxx"
#include "WhiteNoise.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ARMA
 *
 * An interface class for ARMA
 */
class OT_API ARMA
  : public ProcessImplementation
{
  CLASSNAME;

public:

  /** Some typedefs to ease reading */


  /** Default constructor */
  ARMA();

  /** Standard constructor with coefficients and a White Noise */
  ARMA(const ARMACoefficients & ARCoefficients,
       const ARMACoefficients & MACoefficients,
       const WhiteNoise & whiteNoise);

  /** Standard constructor with coefficients, a White Noise and a state */
  ARMA(const ARMACoefficients & ARCoefficients,
       const ARMACoefficients & MACoefficients,
       const WhiteNoise & whiteNoise,
       const ARMAState & state);

  /** Virtual constructor */
  virtual ARMA * clone() const;

  /** String converter */
  String __repr__() const;

  /** String converter  - pretty print */
  String __str__(const String & offset = "") const;

  /** Is the underlying a stationary process ? */
  Bool isStationary() const;

  /** Is the underlying a Normal process ? */
  Bool isNormal() const;

  /** Realization accessor */
  Field getRealization() const;

  /** Prediction of the N futur iterations of an ARMA process */
  using ProcessImplementation::getFuture;
  TimeSeries getFuture(const UnsignedInteger stepNumber) const;

  /** Coefficients accessor : AR & MA */
  ARMACoefficients getARCoefficients() const;
  ARMACoefficients getMACoefficients() const;

  /** State accessor of the ARMA process */
  ARMAState getState() const;
  void setState(const ARMAState & state) const;

  /** WhiteNoise accessor of the ARMA process */
  WhiteNoise getWhiteNoise() const;
  void setWhiteNoise(const WhiteNoise & whiteNoise);

  /** Computation of nThermalization */
  UnsignedInteger computeNThermalization(const NumericalScalar epsilon) const;

  /** Nthermalization accessor - Visibility is done */
  UnsignedInteger getNThermalization() const;

  /** Nthermalization accessor - Setting the value */
  void setNThermalization(const UnsignedInteger n);

  /** Get the random vector corresponding to the i-th marginal component */
  Implementation getMarginalProcess(const UnsignedInteger i) const;

  /** Get the marginal random vector corresponding to indices components */
  Implementation getMarginalProcess(const Indices & indices) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


private:


  /** Compute the steps next values of the process starting from the current state.
      The result is the current state extended stepNumber steps further */
  ARMAState computeReccurence(const UnsignedInteger stepNumber) const;

  /** thermalize method */
  void thermalize() const;

  /** AR coefficients of the ARMA process */
  ARMACoefficients ARCoefficients_;

  /** MA coefficients of the ARMA process */
  ARMACoefficients MACoefficients_;

  /** The distribution underlying the White Noise of the process */
  Distribution noiseDistribution_;

  /** Size of AR part */
  UnsignedInteger p_;

  /** Size of MA part */
  UnsignedInteger q_;

  /** Mutable  current state of the ARMA process */
  mutable ARMAState state_;

  /** Boolean flag - compute once the number of iterations of the thermalize */
  mutable Bool hasComputedNThermalization_;

  /** Number of iterations for the thermalize method */
  mutable UnsignedInteger nThermalization_;

}; /* class ARMA */
END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ARMA_HXX */
