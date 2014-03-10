//                                               -*- C++ -*-
/**
 *  @file  ProcessSample.hxx
 *  @brief ProcessSample class
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#ifndef OPENTURNS_PROCESSSAMPLE_HXX
#define OPENTURNS_PROCESSSAMPLE_HXX

#include "PersistentObject.hxx"
#include "NumericalPoint.hxx"
//#include "TimeGrid.hxx"
#include "Field.hxx"
#include "NumericalSample.hxx"
#include "Collection.hxx"
#include "PersistentCollection.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ProcessSample
 *
 * An interface for time series
 */
class ProcessSample
  : public PersistentObject
{
  CLASSNAME;

public:

  /** Some typedefs to ease reading */
  typedef Collection<NumericalSample>           NumericalSampleCollection;
  typedef PersistentCollection<NumericalSample> NumericalSamplePersistentCollection;

  /** Default constructor */
  ProcessSample();

  /** Constructors */
  ProcessSample(const UnsignedInteger size,
                const Field & field);

  ProcessSample(const Mesh & mesh,
                const UnsignedInteger size,
                const UnsignedInteger dimension);

  /** Partial copy constructor */
  void add(const Field & field);

#ifndef SWIG

  /** Operators accessors */
  void setField (const UnsignedInteger i, const Field & field);
  Field getField (const UnsignedInteger i) const;
  NumericalSample & operator[] (const UnsignedInteger i);
  const NumericalSample & operator[] (const UnsignedInteger i) const;

#endif

  /** Method __getitem__() for Python */
  Field __getitem__ (const UnsignedInteger i) const;

  /** Method __setitem__() is for Python */
  void __setitem__(const UnsignedInteger i,
                   const Field & field);

  /** Virtual constructor */
  virtual ProcessSample * clone() const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Time grid accessors */
  RegularGrid getTimeGrid() const;

  /** Mesh accessors */
  Mesh getMesh() const;

  /** Size accessor */
  UnsignedInteger getSize() const;

  /** Dimension accessor */
  UnsignedInteger getDimension() const;

  /** Mean accessor */
  Field computeMean() const;

  /** Temporal mean accessor */
  NumericalSample computeTemporalMean() const;

  /** Spatial mean accessor */
  NumericalSample computeSpatialMean() const;

  /**  Method computeQuantilePerComponent() gives the quantile per component of the sample */
  Field computeQuantilePerComponent(const NumericalScalar prob) const;

  /** Draw a marginal of the timeSerie */
  Graph drawMarginal(const UnsignedInteger index = 0) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  /** Mesh on which the ProcessSample focuses */
  Mesh mesh_;

  /** NumericalSample collection of all the fields */
  NumericalSamplePersistentCollection data_;

}; /* class ProcessSample */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PROCESSSAMPLE_HXX */
