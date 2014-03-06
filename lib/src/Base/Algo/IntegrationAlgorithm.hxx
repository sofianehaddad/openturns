//                                               -*- C++ -*-
/**
 *  @file  IntegrationAlgorithm.hxx
 *  @brief  This class is enables to integrate a function over an interval
 *  This is the interface class
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#ifndef OPENTURNS_INTEGRATIONALGORITHM_HXX
#define OPENTURNS_INTEGRATIONALGORITHM_HXX

#include "PersistentObject.hxx"
#include "IntegrationAlgorithmImplementation.hxx"
#include "TypedInterfaceObject.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class IntegrationAlgorithm
 */

class IntegrationAlgorithm
  : public TypedInterfaceObject<IntegrationAlgorithmImplementation>
{

  CLASSNAME;

public:

  typedef Pointer<IntegrationAlgorithmImplementation>    Implementation;

  /** Default onstructor */
  IntegrationAlgorithm();

  /** Copy constructors */
  IntegrationAlgorithm(const IntegrationAlgorithmImplementation & implementation,
                       const String & name = DefaultName);

  /** Constructor from implementation */
  IntegrationAlgorithm(const Implementation & p_implementation,
                       const String & name = DefaultName);

#ifndef SWIG
  /** Constructor from implementation pointer */
  IntegrationAlgorithm(IntegrationAlgorithmImplementation * p_implementation,
                       const String & name = DefaultName);
#endif

  /** Compute an approximation of \int_{[a,b]}f(x)dx, where [a,b]
   * is an n-D interval
   */
  NumericalScalar integrate(const NumericalMathFunction & function,
                            const Interval & interval) const;

#ifndef SWIG
  NumericalScalar integrate(const NumericalMathFunction & function,
                            const Interval & interval,
                            NumericalScalar & error) const;
#endif

  NumericalScalar integrate(const NumericalMathFunction & function,
                            const Interval & interval,
                            NumericalPoint & error) const;

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;


} ; /* class IntegrationAlgorithm */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_INTEGRATIONALGORITHM_HXX */
