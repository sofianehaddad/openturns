//                                               -*- C++ -*-
/**
 *  @file  HistoryStrategy.hxx
 *  @brief This class is a top-level class for the history mechanism.
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
#ifndef OPENTURNS_HISTORYSTRATEGY_HXX
#define OPENTURNS_HISTORYSTRATEGY_HXX

#include "HistoryStrategyImplementation.hxx"
#include "Pointer.hxx"
#include "TypedInterfaceObject.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class HistoryStrategy
 */

class OT_API HistoryStrategy
  : public TypedInterfaceObject<HistoryStrategyImplementation>
{

  CLASSNAME;

public:

  typedef Pointer<HistoryStrategyImplementation> Implementation;

  /** Default constructor */
  HistoryStrategy();

  /** Constructor with parameters */
  HistoryStrategy(const HistoryStrategyImplementation & implementation);

  /** Constructor with parameters */
  HistoryStrategy(Implementation & p_implementation);

  /** Store the point according to the strategy */
  void store(const NumericalPoint & point);
  void store(const NumericalSample & sample);

  /** Clear the history storage */
  void clear();

  /** History sample accessor */
  NumericalSample getSample() const;

  /** String converter */
  String __repr__() const;

} ; /* class HistoryStrategy */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_HISTORYSTRATEGY_HXX */
