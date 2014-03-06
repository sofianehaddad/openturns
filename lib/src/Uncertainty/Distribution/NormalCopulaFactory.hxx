//                                               -*- C++ -*-
/**
 *  @file  NormalCopulaFactory.hxx
 *  @brief Factory for NormalCopula distribution
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
#ifndef OPENTURNS_NORMALCOPULAFACTORY_HXX
#define OPENTURNS_NORMALCOPULAFACTORY_HXX

#include "OTprivate.hxx"
#include "DistributionImplementationFactory.hxx"
#include "NormalCopula.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class NormalCopulaFactory
 */
class NormalCopulaFactory
  : public DistributionImplementationFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  NormalCopulaFactory();

  /** Virtual constructor */
  virtual NormalCopulaFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionImplementationFactory::build;

  Implementation build(const NumericalSample & sample) const;
  Implementation build(const NumericalPointCollection & parameters) const;
  Implementation build() const;
  NormalCopula buildAsNormalCopula(const NumericalSample & sample) const;
  NormalCopula buildAsNormalCopula(const NumericalPointCollection & parameters) const;
  NormalCopula buildAsNormalCopula(const NumericalPointWithDescriptionCollection & parameters) const;
  NormalCopula buildAsNormalCopula() const;

}; /* class NormalCopulaFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_NORMALCOPULAFACTORY_HXX */
