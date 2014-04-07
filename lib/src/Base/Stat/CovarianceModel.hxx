//                                               -*- C++ -*-
/**
 *  @file  CovarianceModel.hxx
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
 *  @author schueller
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#ifndef OPENTURNS_COVARIANCEMODEL_HXX
#define OPENTURNS_COVARIANCEMODEL_HXX

#include "TypedInterfaceObject.hxx"
#include "CovarianceModelImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class CovarianceModel
 */

class CovarianceModel
  : public TypedInterfaceObject<CovarianceModelImplementation>
{

  CLASSNAME;

public:

  typedef CovarianceModelImplementation::Implementation    Implementation;

  /** Default constructor without parameters */
  CovarianceModel();

  /** Copy constructors */
  CovarianceModel(const CovarianceModelImplementation & implementation);

  /** Constructor from implementation */
  CovarianceModel(const Implementation & p_implementation);

#ifndef SWIG
  /** Constructor from implementation pointer */
  CovarianceModel(CovarianceModelImplementation * p_implementation);
#endif

  /** Dimension accessor */
  virtual UnsignedInteger getDimension() const;

  /** Computation of the covariance matrix */
  virtual CovarianceMatrix computeCovariance(const NumericalScalar s,
      const NumericalScalar t) const;

  virtual CovarianceMatrix computeCovariance(const NumericalPoint & s,
      const NumericalPoint & t) const;

  virtual CovarianceMatrix operator() (const NumericalScalar s,
                                       const NumericalScalar t) const;

  virtual CovarianceMatrix operator() (const NumericalPoint & s,
                                       const NumericalPoint & t) const;

  /** Computation of the covariance matrix for stationnary models */
  virtual CovarianceMatrix computeCovariance(const NumericalScalar tau) const;
  virtual CovarianceMatrix computeCovariance(const NumericalPoint & tau) const;

  virtual CovarianceMatrix operator() (const NumericalScalar tau) const;
  virtual CovarianceMatrix operator() (const NumericalPoint & tau) const;

  /** Gradient */
  virtual SymmetricTensor partialGradient(const NumericalPoint & s,
                                          const NumericalPoint & t) const;

  /** Discretize the covariance function on a given TimeGrid/Mesh */
  virtual CovarianceMatrix discretizeCovariance(const RegularGrid & timeGrid) const;
  virtual CovarianceMatrix discretizeCovariance(const Mesh & mesh) const;

  virtual CovarianceMatrix discretize(const RegularGrid & timeGrid) const;
  virtual CovarianceMatrix discretize(const Mesh & mesh) const;

  /** Partial discretization with respect to the second argument */
  Basis getPartialDiscretization(const NumericalSample & secondLocation) const;

  /** Is it a stationary model ? */
  virtual Bool isStationary() const;

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

  /** Parameters accessor */
  void setParameters(const NumericalPointWithDescription & parameters);
  NumericalPointWithDescription getParameters() const;

} ; /* class CovarianceModel */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COVARIANCEMODEL_HXX */
