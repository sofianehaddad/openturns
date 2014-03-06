//                                               -*- C++ -*-
/**
 *  @file  NatafEllipticalDistributionEvaluation.hxx
 *  @brief Class for the Nataf transformation evaluation for elliptical
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
#ifndef OPENTURNS_NATAFELLIPTICALDISTRIBUTIONEVALUATION_HXX
#define OPENTURNS_NATAFELLIPTICALDISTRIBUTIONEVALUATION_HXX

#include "OTprivate.hxx"
#include "LinearNumericalMathEvaluationImplementation.hxx"
#include "Matrix.hxx"
#include "NumericalPoint.hxx"
#include "StorageManager.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class NatafEllipticalDistributionEvaluation
 *
 * This class offers an interface for the Nataf function for elliptical distributions
 */
class NatafEllipticalDistributionEvaluation
  : public LinearNumericalMathEvaluationImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  NatafEllipticalDistributionEvaluation();

  /** Parameter constructor */
  NatafEllipticalDistributionEvaluation(const NumericalPoint & mean,
                                        const SquareMatrix & inverseCholesky);

  /** Virtual constructor */
  virtual NatafEllipticalDistributionEvaluation * clone() const;

  /** Gradient according to the marginal parameters */
  virtual Matrix parametersGradient(const NumericalPoint & inP) const;

  /** String converter */
  virtual String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:


private:

}; /* NatafEllipticalDistributionEvaluation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_NATAFELLIPTICALDISTRIBUTIONEVALUATION_HXX */
