//                                               -*- C++ -*-
/**
 *  @file  Experiment.hxx
 *  @brief Abstract top-level view of an experiment plane
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
#ifndef OPENTURNS_EXPERIMENT_HXX
#define OPENTURNS_EXPERIMENT_HXX

#include "TypedInterfaceObject.hxx"
#include "ExperimentImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class Experiment
 *
 * The class describes the probabilistic concept of experiment plan
 */
class Experiment
  : public TypedInterfaceObject<ExperimentImplementation>
{
  CLASSNAME;
public:

  /** Default constructor */
  Experiment();

  /** Constructor from implementation */
  Experiment(const ExperimentImplementation & implementation);

#ifndef SWIG
  /** Constructor from implementation pointer */
  Experiment(const Implementation & p_implementation);
#endif

  /** String converter */
  virtual String __repr__() const;

  /** Sample generation */
  virtual NumericalSample generate();

  /** Implementation accessor */
  void setImplementation(const Implementation & p_implementation);

}; /* class Experiment */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_EXPERIMENT_HXX */
