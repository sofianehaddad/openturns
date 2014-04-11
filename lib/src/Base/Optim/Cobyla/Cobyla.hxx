//                                               -*- C++ -*-
/**
 *  @file  Cobyla.hxx
 *  @brief Cobyla is an actual implementation for
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
#ifndef OPENTURNS_COBYLA_HXX
#define OPENTURNS_COBYLA_HXX

#include "CobylaSpecificParameters.hxx"
#include "OTprivate.hxx"
#include "NearestPointAlgorithmImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class cobyla
 * cobyla is an actual implementation for
 * NearestPointAlgorithm using the cobyla library
 */
class OT_API Cobyla :
  public NearestPointAlgorithmImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  Cobyla();

  /** Constructor with parameters */
  explicit Cobyla(const NumericalMathFunction & levelFunction,
                  const Bool verbose = false);


  /** Constructor with parameters */
  Cobyla(const CobylaSpecificParameters & specificParameters,
         const NumericalMathFunction & levelFunction,
         const Bool verbose = false);


  /** Virtual constructor */
  virtual Cobyla * clone() const;

  /** Performs the actual computation. Must be overloaded by the actual optimisation algorithm */
  void run();

  /** Specific parameters accessor */
  CobylaSpecificParameters getSpecificParameters() const;

  /** Specific parameters accessor */
  void setSpecificParameters(const CobylaSpecificParameters & specificParameters);
  /** String converter */
  String __repr__() const;

private:

  /**
   * Function that computes half of the squared of a Vector. It will
   * become the objective function of the minimisation problem
   */
  static int ComputeObjectiveAndConstraint(int n,
      int m,
      double *x,
      double *f,
      double *con,
      void *state);
  CobylaSpecificParameters specificParameters_;

  /// temporary, used to track input/outputs
  NumericalSample evaluationInputHistory_;
  NumericalSample evaluationOutputHistory_;

}; /* class Cobyla */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COBYLA_HXX */
