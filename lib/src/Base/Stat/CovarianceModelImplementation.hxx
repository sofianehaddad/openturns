//                                               -*- C++ -*-
/**
 *  @file  CovarianceModelImplementation.hxx
 *  @brief This class enables to build a covariance model
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
 *  @author lebrun
 *  @date   2011-12-05 17:31:12 +0100 (lun. 05 déc. 2011)
 */
#ifndef OPENTURNS_COVARIANCEMODELIMPLEMENTATION_HXX
#define OPENTURNS_COVARIANCEMODELIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "CovarianceMatrix.hxx"
#include "Pointer.hxx"
#include "RegularGrid.hxx"
#include "Mesh.hxx"
#include "Basis.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class CovarianceModelImplementation
 */

class OT_API CovarianceModelImplementation
  : public PersistentObject
{

  CLASSNAME;

public:

  typedef Pointer<CovarianceModelImplementation> Implementation;

  CovarianceModelImplementation();

  /** Constructor with dimension parameter */
  explicit CovarianceModelImplementation(const UnsignedInteger dimension);

  /** Virtual copy constructor */
  virtual CovarianceModelImplementation * clone() const;

  /** Dimension accessor */
  virtual UnsignedInteger getDimension() const;

  /** Computation of the covariance matrix */
  /** @deprecated */
  virtual CovarianceMatrix computeCovariance(const NumericalScalar s,
      const NumericalScalar t) const;
  /** @deprecated */
  virtual CovarianceMatrix computeCovariance(const NumericalPoint & s,
      const NumericalPoint & t) const;

  virtual CovarianceMatrix operator() (const NumericalScalar s,
                                       const NumericalScalar t) const;
  virtual CovarianceMatrix operator() (const NumericalPoint & s,
                                       const NumericalPoint & t) const;

  /** Computation of the covariance matrix for stationnary models */
  /** @deprecated */
  virtual CovarianceMatrix computeCovariance(const NumericalScalar tau) const;

  /** @deprecated */
  virtual CovarianceMatrix computeCovariance(const NumericalPoint & tau) const;

  virtual CovarianceMatrix operator() (const NumericalScalar tau) const;
  virtual CovarianceMatrix operator() (const NumericalPoint & tau) const;

  /** Gradient */
  virtual SymmetricTensor partialGradient(const NumericalPoint & s,
                                          const NumericalPoint & t) const;

  /** Discretize the covariance function on a given TimeGrid/Mesh */
  /** @deprecated */
  virtual CovarianceMatrix discretizeCovariance(const RegularGrid & timeGrid) const;

  /** @deprecated */
  virtual CovarianceMatrix discretizeCovariance(const Mesh & mesh) const;

  virtual CovarianceMatrix discretize(const RegularGrid & timeGrid) const;

  virtual CovarianceMatrix discretize(const Mesh & mesh) const;

  /** Partial discretization with respect to the second argument */
  Basis getPartialDiscretization(const NumericalSample & secondLocation) const;

  /** Is it a stationary covariance model ? */
  virtual Bool isStationary() const;

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

  /** Parameters accessor */
  virtual void setParameters(const NumericalPointWithDescription & parameters);
  virtual NumericalPointWithDescription getParameters() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

  /** dimension parameter */
  UnsignedInteger dimension_;

} ; /* class CovarianceModelImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COVARIANCEMODELIMPLEMENTATION_HXX */
