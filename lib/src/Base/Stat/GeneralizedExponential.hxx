//                                               -*- C++ -*-
/**
 *  @file  GeneralizedExponential.hxx
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
#ifndef OPENTURNS_GENERALIZEDEXPONENTIAL_HXX
#define OPENTURNS_GENERALIZEDEXPONENTIAL_HXX

#include "CovarianceModel.hxx"
#include "PersistentCollection.hxx"
#include "Collection.hxx"
#include "RegularGrid.hxx"
#include "Mesh.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class GeneralizedExponential
 */

class GeneralizedExponential
  : public CovarianceModelImplementation
{

  CLASSNAME;

public:

  typedef PersistentCollection<CovarianceMatrix>          CovarianceMatrixPersistentCollection;
  typedef Collection<CovarianceMatrix>                    CovarianceMatrixCollection;

  /** Default constructor */
  explicit GeneralizedExponential(const UnsignedInteger & dimension = 0);

  GeneralizedExponential(const UnsignedInteger & dimension,
                         const NumericalScalar theta,
                         const NumericalScalar p);

  /** Virtual copy constructor */
  GeneralizedExponential * clone() const;

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
  NumericalScalar p_;

} ; /* class GeneralizedExponential */

END_NAMESPACE_OPENTURNS

#endif
