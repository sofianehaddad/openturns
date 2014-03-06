//                                               -*- C++ -*-
/**
 *  @file  AbsoluteExponential.hxx
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
 *  @author: schueller
 */
#ifndef OPENTURNS_ABSOLUTEEXPONENTIAL_HXX
#define OPENTURNS_ABSOLUTEEXPONENTIAL_HXX

#include "CovarianceModel.hxx"
#include "PersistentCollection.hxx"
#include "Collection.hxx"
#include "RegularGrid.hxx"
#include "Mesh.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class AbsoluteExponential
 */

class AbsoluteExponential
  : public CovarianceModelImplementation
{

  CLASSNAME;

public:

  typedef PersistentCollection<CovarianceMatrix>          CovarianceMatrixPersistentCollection;
  typedef Collection<CovarianceMatrix>                    CovarianceMatrixCollection;

  /** Default constructor */
  explicit AbsoluteExponential(const UnsignedLong & dimension = 0);

  AbsoluteExponential(const UnsignedLong & dimension,
                      const NumericalScalar theta);

  /** Virtual copy constructor */
  AbsoluteExponential * clone() const;

  /** Computation of the covariance function */
  CovarianceMatrix operator() ( const NumericalPoint & s,
                                const NumericalPoint & t) const;
  /** Gradient */
  virtual SymmetricTensor partialGradient(const NumericalPoint & s,
                                          const NumericalPoint & t) const;

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

  /** Parameters accessor */
  virtual void setParameters(const NumericalPointWithDescription & parameters);
  virtual NumericalPointWithDescription getParameters() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  NumericalScalar theta_;

} ; /* class AbsoluteExponential */

END_NAMESPACE_OPENTURNS

#endif
