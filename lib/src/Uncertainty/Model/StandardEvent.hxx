//                                               -*- C++ -*-
/**
 *  @file  StandardEvent.hxx
 *  @brief The class that implements an event based on a composite random vector whose
 *   antecedent has a spherical distribution
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
#ifndef OPENTURNS_STANDARDEVENT_HXX
#define OPENTURNS_STANDARDEVENT_HXX

#include "OTprivate.hxx"
#include "RandomVector.hxx"
#include "Event.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class StandardEvent
 *
 * The class that implements an event based on a composite random vector whose
 * antecedent has a spherical distribution
 */
class OT_API StandardEvent
  : public Event
{
  CLASSNAME;

public:


  /** Default constructor */
  StandardEvent();

  /** Constructor from RandomVector */
  StandardEvent(const RandomVector & antecedent,
                const ComparisonOperator & op,
                const NumericalScalar threshold);

  /** Constructor from Event */
  StandardEvent(const Event & event);


protected:


private:

}; /* class StandardEvent */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_STANDARDEVENT_HXX */
