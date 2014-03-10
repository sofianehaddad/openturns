//                                               -*- C++ -*-
/**
 *  @file  KrigingAlgorithm.hxx
 *  @brief The class building gaussian process regression
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
 */
#ifndef OPENTURNS_KRIGINGGRADIENT_HXX
#define OPENTURNS_KRIGINGGRADIENT_HXX

#include "NumericalMathGradientImplementation.hxx"
#include "CovarianceModel.hxx"


BEGIN_NAMESPACE_OPENTURNS

/**
  * @class KrigingGradient
  *
  * This class permits prediction on a gaussian process
  */
class KrigingGradient
  : public NumericalMathGradientImplementation
{
  CLASSNAME

public:

  /** Default constructor */
  KrigingGradient();

  /** Constructor with parameters */
  KrigingGradient(const Basis & basis,
                  const NumericalSample & inputSample,
                  const CovarianceModel & correlationModel,
                  const NumericalPoint & beta,
                  const NumericalPoint & gamma);

  /** Virtual constructor */
  virtual KrigingGradient * clone() const;

  /** Comparison operator */
  Bool operator ==(const KrigingGradient & other) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Test for actual implementation */
  virtual Bool isActualImplementation() const;

  /** Operator () */
//   using NumericalMathGradientImplementation::gradient;
  Matrix gradient(const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  virtual UnsignedInteger getInputDimension() const;

  /** Accessor for output point dimension */
  virtual UnsignedInteger getOutputDimension() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:
  /// Basis
  Basis basis_;

  /// Training (input) sample
  NumericalSample inputSample_;

  /// Correlation model
  CovarianceModel covarianceModel_;

  /// Regression weights
  NumericalPoint beta_;
  NumericalPoint gamma_;

};

END_NAMESPACE_OPENTURNS

#endif