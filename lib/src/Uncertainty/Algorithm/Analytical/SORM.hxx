//                                               -*- C++ -*-
/**
 *  @file  SORM.hxx
 *  @brief SORM implements the First Order Reliability Method
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
#ifndef OPENTURNS_SORM_HXX
#define OPENTURNS_SORM_HXX

#include "Analytical.hxx"
#include "Event.hxx"
#include "SquareMatrix.hxx"
#include "Distribution.hxx"
#include "SORMResult.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class SORM
 * SORM implements the Second Order Reliability Method
 * and compute the results of such kind of analyses
 */

class OT_API SORM :
  public Analytical
{

  CLASSNAME;
public:


  /** Default constructor */
  SORM();

  /** Constructor with parameters */
  SORM(const NearestPointAlgorithm & nearestPointAlgorithm,
       const Event & event,
       const NumericalPoint & physicalStartingPoint);


  /** Virtual constructor */
  virtual SORM * clone() const;

  /** Result accessor */
  SORMResult getResult() const;

  /** Result accessor */
  void setResult(const SORMResult & sormResult);

  /** String converter */
  String __repr__() const;

  /** Function that computes the design point by re-using the Analytical::run() and creates a SORM::Result */
  void run();

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  SORMResult sormResult_;

} ; /* class SORM */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SORM_HXX */
