//                                               -*- C++ -*-
/**
 *  @file  FieldImplementation.hxx
 *  @brief The class FieldImplementation implements values indexed by
 *  the vertices of a Mesh
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
 *  MERCHANTABILITY or FITNESS FOR A PARTCULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author schueller
 *  @date   2011-05-24 19:30:41 +0200 (Tue, 24 May 2011)
 */
#ifndef OPENTURNS_FIELDIMPLEMENTATION_HXX
#define OPENTURNS_FIELDIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "NumericalSample.hxx"
#include "Mesh.hxx"
#include "RegularGrid.hxx"
#include "TBB.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class FieldImplementation
 */

class OT_API FieldImplementation
  : public PersistentObject
{
  CLASSNAME;

public:

  /**
   * Default constructor
   */
  FieldImplementation();

  /** Standard constructor */
  FieldImplementation(const Mesh & mesh,
                      const UnsignedInteger dim);

  /** Constructor from a Mesh and a sample */
  FieldImplementation(const Mesh & mesh,
                      const NumericalSample & values);

  /** Accessor to values */
  NumericalPoint getValueAtIndex(const UnsignedInteger index) const;
  void setValueAtIndex(const UnsignedInteger index,
                       const NumericalPoint & val);

  NumericalPoint getValueAtNearestPosition(const NumericalPoint & position) const;
  void setValueAtNearestPosition(const NumericalPoint & position,
                                 const NumericalPoint & val);

  /** Accessor to values */
  NumericalPoint getValueAtNearestTime(const NumericalScalar timestamp) const;
  void setValueAtNearestTime(const NumericalScalar timestamp,
                             const NumericalPoint & val);

  /** Virtual constructor */
  virtual FieldImplementation * clone() const;

  /** Description Accessor */
  void setDescription(const Description & description);
  Description getDescription() const;

  /** Mesh accessor */
  Mesh getMesh() const;
  RegularGrid getTimeGrid() const;

  /** Comparison operator */
  Bool operator ==(const FieldImplementation & other) const;

  /**
   * String converter
   * This method shows human readable information on the
   * internal state of an FieldImplementation. It is used when streaming
   * the FieldImplementation or for user information.
   */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Size accessor */
  UnsignedInteger getSize() const;

  /** Dimension accessor */
  UnsignedInteger getMeshDimension() const;
  UnsignedInteger getDimension() const;

  /** Return the values stored in the field as a sample */
  NumericalSample getSample() const;
  NumericalSample getValues() const;

#ifndef SWIG
  /** Individual value accessor */
  NSI_point operator[](const UnsignedInteger index);
  NSI_const_point operator[](const UnsignedInteger index) const;
  NSI_point at(const UnsignedInteger index);
  NSI_const_point at(const UnsignedInteger index) const;
  NumericalScalar & operator() (const UnsignedInteger i,
                                const UnsignedInteger j);
  const NumericalScalar & operator() (const UnsignedInteger i,
                                      const UnsignedInteger j) const;
  NumericalScalar & at(const UnsignedInteger i,
                       const UnsignedInteger j);
  const NumericalScalar & at(const UnsignedInteger i,
                             const UnsignedInteger j) const;
#endif

  /* Method __len__() is for Python */
  UnsignedInteger __len__() const;

  /* Method __contains__() is for Python */
  Bool __contains__(const NumericalPoint & val) const;


  const NumericalPoint __getitem__ (const UnsignedInteger index) const;
  void __setitem__ (const UnsignedInteger index,
                    const NumericalPoint & val);

  /** Return the field as a sample, ie its values and positions */
  NumericalSample asSample() const;

  /** Return the field as a defomed mesh, ie its values are added to the components of the vertices if the dimensions match */
  Mesh asDeformedMesh() const;

  /** Compute the spatial mean of the field */
  NumericalPoint getSpatialMean() const;

  /** Compute the temporal mean of the field, ie its spatial mean when the mesh
      is regular and of dimension 1 */
  NumericalPoint getTemporalMean() const;

  /** Draw a marginal of the timeSerie */
  Graph drawMarginal(const UnsignedInteger index = 0,
                     const Bool interpolate = true,
		     const Bool stream = false) const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

  /** VTK export */
  void exportToVTKFile(const String & fileName) const;

protected:

  /** Compute the spatial mean of the field */
  void computeSpatialMean() const;

  /* TBB functor to speed-up spatial mean computation */
  struct SpatialMeanFunctor
  {
    const FieldImplementation & field_;
    NumericalScalar volumeAccumulator_;
    NumericalPoint accumulator_;

    SpatialMeanFunctor(const FieldImplementation & field)
      : field_(field), volumeAccumulator_(0.0), accumulator_(field.getDimension(), 0.0) {}

    SpatialMeanFunctor(const SpatialMeanFunctor & other, TBB::Split)
      : field_(other.field_), volumeAccumulator_(0.0), accumulator_(other.field_.getDimension(), 0.0) {}

    void operator() (const TBB::BlockedRange<UnsignedInteger> & r)
    {
      const UnsignedInteger meshDimension(field_.getMeshDimension());
      const UnsignedInteger dimension(field_.getDimension());
      for (UnsignedInteger i = r.begin(); i != r.end(); ++i) 
	{
	  const NumericalScalar volume(field_.mesh_.computeSimplexVolume(i));
	  const Indices simplex(field_.mesh_.getSimplex(i));
	  NumericalPoint meanValue(dimension, 0.0);
	  for (UnsignedInteger j = 0; j <= meshDimension; ++j) meanValue += field_.values_[simplex[j]];
	  volumeAccumulator_ += volume;
	  accumulator_ += (volume / dimension) * meanValue;
	}
    }

    void join(const SpatialMeanFunctor & other)
    {
      volumeAccumulator_ += other.volumeAccumulator_;
      accumulator_ += other.accumulator_;
    }

  }; /* end struct SpatialMeanFunctor */

  /** The mesh associated to the field */
  Mesh mesh_;

  /** The values associated to the field */
  NumericalSample values_;

  /** The description of all components */
  Description description_;

  /** The spatial mean */
  mutable NumericalPoint spatialMean_;

  /** Flag to tell if the spatial mean has already been computed */
  mutable Bool isAlreadyComputedSpatialMean_;
}; /* class FieldImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_FIELDIMPLEMENTATION_HXX */
