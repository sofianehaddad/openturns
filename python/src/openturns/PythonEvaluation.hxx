//                                               -*- C++ -*-
/**
 * @brief PythonEvaluation declaration
 *
 *  Copyright 2005-2025 Airbus-EDF-IMACS-ONERA-Phimeca
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
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef OPENTURNS_PYTHONEVALUATION_HXX
#define OPENTURNS_PYTHONEVALUATION_HXX

#include <Python.h>
#include "openturns/EvaluationImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class PythonEvaluation
 *
 * This class binds a Python function to an OpenTURNS' Function
 */
class PythonEvaluation
  : public EvaluationImplementation
{
  CLASSNAME
public:


  /** Constructor from Python object*/
  PythonEvaluation(PyObject * pyCallable);

  /** Copy constructor */
  PythonEvaluation(const PythonEvaluation & other);

  /** Virtual constructor */
  PythonEvaluation * clone() const override;

  /** Copy assignment operator */
  PythonEvaluation & operator=(const PythonEvaluation & rhs);

  /** Destructor */
  virtual ~PythonEvaluation();

  /** Comparison operator */
  using EvaluationImplementation::operator ==;
  Bool operator ==(const PythonEvaluation & other) const;

  /** String converter */
  String __repr__() const override;
  String __str__(const String & offset = "") const override;

  /* Here is the interface that all derived class must implement */

  /** Operator () */
  Point operator() (const Point & inP) const override;

  Sample operator() (const Sample & inS) const override;

  /** Accessor for input point dimension */
  UnsignedInteger getInputDimension() const override;

  /** Accessor for output point dimension */
  UnsignedInteger getOutputDimension() const override;

  /** Linearity accessors */
  Bool isLinear() const override;
  Bool isLinearlyDependent(const UnsignedInteger index) const override;

  /** Is it safe to call in parallel? */
  Bool isParallel() const override;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const override;

  /** Method save() reloads the object from the StorageManager */
  void load(Advocate & adv) override;

protected:


private:

  friend class Factory<PythonEvaluation>;

  /** Default constructor */
  PythonEvaluation();

  /** Set pyBufferClass_ and pyObj_*_ members */
  void initializePythonState();

  /** The underlying Python callable object */
  PyObject * pyObj_ = nullptr;

  /** Tell whether Python callable object define these members */
  Bool pyObj_has_exec_ = false;
  Bool pyObj_has_exec_sample_ = false;
  Bool pyObj_discard_openturns_memoryview_ = true;

  /** Python openturns.memoryview.Buffer class */
  PyObject * pyBufferClass_ = nullptr;

}; /* class PythonEvaluation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PYTHONEVALUATION_HXX */

