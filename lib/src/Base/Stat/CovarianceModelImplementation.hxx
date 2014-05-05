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

class CovarianceModelImplementation
  : public PersistentObject
{

  CLASSNAME;

public:

  typedef Pointer<CovarianceModelImplementation> Implementation;

  /** Default constructor without parameters */
  explicit CovarianceModelImplementation(const String & name = DefaultName);

  /** Constructor with dimension parameter */
  explicit CovarianceModelImplementation(const UnsignedInteger dimension,
                                         const String & name = DefaultName);

  /** Virtual copy constructor */
  virtual CovarianceModelImplementation * clone() const;

  /** Dimension accessor */
  virtual UnsignedInteger getDimension() const;

  /** Compute the covariance function */
  virtual CovarianceMatrix operator() (const NumericalScalar s,
                                       const NumericalScalar t) const;
  virtual CovarianceMatrix operator() (const NumericalPoint & s,
                                       const NumericalPoint & t) const;
  // Special case for 1D model
  virtual NumericalScalar computeAsScalar (const NumericalPoint & s,
					   const NumericalPoint & t) const;

  virtual CovarianceMatrix operator() (const NumericalScalar tau) const;
  virtual CovarianceMatrix operator() (const NumericalPoint & tau) const;
  // Special case for 1D model
  virtual NumericalScalar computeAsScalar (const NumericalPoint & tau) const;

  /** Gradient */
  virtual SymmetricTensor partialGradient(const NumericalPoint & s,
                                          const NumericalPoint & t) const;

  /** Discretize the covariance function on a given TimeGrid/Mesh */
  virtual CovarianceMatrix discretize(const RegularGrid & timeGrid) const;

  virtual CovarianceMatrix discretize(const Mesh & mesh) const;
  virtual NumericalSample discretizeRow(const NumericalSample & vertices,
					const UnsignedInteger p) const;

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

  struct DiscretizePolicy
  {
    const NumericalSample & input_;
    const NumericalPoint p_;
    NumericalSample & output_;
    const CovarianceModelImplementation & model_;

    DiscretizePolicy(const NumericalSample & input,
		     const UnsignedInteger p,
		     NumericalSample & output,
		     const CovarianceModelImplementation & model)
      : input_(input)
      , p_(input[p])
      , output_(output)
      , model_(model)
    {}

    inline void operator()( const TBB::BlockedRange<UnsignedInteger> & r ) const
    {
      for (UnsignedInteger i = r.begin(); i != r.end(); ++i) output_[i][0] = model_.computeAsScalar(p_, input_[i]);
    }

  }; /* end struct DiscretizePolicy */

  /** dimension parameter */
  UnsignedInteger dimension_;

} ; /* class CovarianceModelImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COVARIANCEMODELIMPLEMENTATION_HXX */
