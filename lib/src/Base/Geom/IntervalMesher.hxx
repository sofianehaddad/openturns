//                                               -*- C++ -*-
/**
 *  @file  IntervalMesher.hxx
 *  @brief Meshing algorithm for intervals
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
 *  @author: $LastChangedBy: schueller $
 *  @date:   $LastChangedDate: 2012-02-17 19:35:43 +0100 (ven. 17 févr. 2012) $
 *  Id:      $Id: IntervalMesher.hxx 2392 2012-02-17 18:35:43Z schueller $
 */
#ifndef OPENTURNS_INTERVALMESHER_HXX
#define OPENTURNS_INTERVALMESHER_HXX

#include "Interval.hxx"
#include "Mesh.hxx"
#include "MeshFactoryImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class IntervalMesher
 */
class OT_API IntervalMesher
  : public MeshFactoryImplementation
{
  CLASSNAME;
public:

  /** Default constructor */
  IntervalMesher();

  /** Default constructor */
  explicit IntervalMesher(const Indices & discretization);

  /** Virtual constructor */
  virtual IntervalMesher * clone() const;

  /** Discretization accessors */
  void setDiscretization(const Indices & discretization);
  Indices getDiscretization() const;

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

  /* Here is the interface that all derived class must implement */
  /** Build a mesh based on a domain */
  virtual Mesh build(const Interval & interval,
		     const Bool diamond = false) const;

protected:

private:

  /* Discretization in each dimension */
  Indices discretization_;

}; /* class IntervalMesher */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_INTERVALMESHER_HXX */
