//                                               -*- C++ -*-
/**
 *  @brief Karhunen-Loeve decomposition and by-products
 *
 *  Copyright 2005-2018 Airbus-EDF-IMACS-Phimeca
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
 */
#ifndef OPENTURNS_KARHUNENLOEVERESULTIMPLEMENTATION_HXX
#define OPENTURNS_KARHUNENLOEVERESULTIMPLEMENTATION_HXX

#include "openturns/PersistentObject.hxx"
#include "openturns/CovarianceModel.hxx"
#include "openturns/Point.hxx"
#include "openturns/Basis.hxx"
#include "openturns/Function.hxx"
#include "openturns/ProcessSample.hxx"
#include "openturns/Matrix.hxx"
#include "openturns/Field.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class KarhunenLoeveResultImplementation
 *
 * Compute the KL decomposition of a given covariance model
 */
class OT_API KarhunenLoeveResultImplementation
  : public PersistentObject
{
  CLASSNAME

public:

  typedef Collection<Function> FunctionCollection;

  /** Default constructor */
  KarhunenLoeveResultImplementation();

  /** Default constructor */
  explicit KarhunenLoeveResultImplementation(const CovarianceModel & covariance,
      const Scalar threshold,
      const Point & eigenvalues,
      const Basis & modes,
      const ProcessSample & modesAsProcessSample,
      const Matrix & projection);

  /** Virtual constructor */
  virtual KarhunenLoeveResultImplementation * clone() const;

  /** Threshold accessor */
  Scalar getThreshold() const;

  /** Covariance model accessor */
  CovarianceModel getCovarianceModel() const;

  /** Eigenvalues accessor */
  Point getEigenValues() const;

  /** Modes accessors */
  Basis getModes() const;
  ProcessSample getModesAsProcessSample() const;

  /** Scaled modes accessors */
  Basis getScaledModes() const;
  ProcessSample getScaledModesAsProcessSample() const;

  /** Projection matrix accessor */
  Matrix getProjectionMatrix() const;

  /** Projection method */
  Point project(const Function & function) const;
  Point project(const Field & field) const;
  Sample project(const FunctionCollection & basis) const;
  Sample project(const ProcessSample & sample) const;

  /** Lift method */
  Function lift(const Point & coefficients) const;
  Field liftAsField(const Point & coefficients) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:
  /** Covariance model */
  CovarianceModel covariance_;

  /** Threshold */
  Scalar threshold_;

  /** Eigenvalues */
  Point eigenvalues_;

  /** Modes */
  Basis modes_;

  /** Modes as process sample */
  ProcessSample modesAsProcessSample_;

  /** Projection matrix */
  Matrix projection_;

}; /* class KarhunenLoeveResultImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_KARHUNENLOEVERESULTIMPLEMENTATION_HXX */
