//                                               -*- C++ -*-
/**
 *  @file  GeneralizedParetoFactory.hxx
 *  @brief Factory for GeneralizedPareto distribution
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
 *  @author dutka
 *  @date   2007-05-10 16:43:31 +0200 (jeu. 10 mai 2007)
 */
#ifndef OPENTURNS_GENERALIZEDPARETOFACTORY_HXX
#define OPENTURNS_GENERALIZEDPARETOFACTORY_HXX

#include "OTprivate.hxx"
#include "DistributionImplementationFactory.hxx"
#include "GeneralizedPareto.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class GeneralizedParetoFactory
 */
class GeneralizedParetoFactory
  : public DistributionImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  GeneralizedParetoFactory();

  /** Virtual constructor */
  virtual GeneralizedParetoFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionImplementationFactory::build;

  Implementation build(const NumericalSample & sample) const;
  Implementation build(const NumericalPointCollection & parameters) const;
  Implementation build() const;
  GeneralizedPareto buildAsGeneralizedPareto(const NumericalSample & sample) const;
  GeneralizedPareto buildAsGeneralizedPareto(const NumericalPointCollection & parameters) const;
  GeneralizedPareto buildAsGeneralizedPareto(const NumericalPointWithDescriptionCollection & parameters) const;
  GeneralizedPareto buildAsGeneralizedPareto() const;

  /** Algorithm associated with the method of moments */
  GeneralizedPareto buildMethodOfMoments(const NumericalSample & sample) const;

  /** Algoritm associated with the method of exponential regression model */
  GeneralizedPareto buildMethodOfExponentialRegression(const NumericalSample & sample) const;

  /** Algorithm associated with the method of modified moments */
  GeneralizedPareto buildMethodOfProbabilityWeightedMoments(const NumericalSample & sample) const;

}; /* class GeneralizedParetoFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_GENERALIZEDPARETOFACTORY_HXX */
