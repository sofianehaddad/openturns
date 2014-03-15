//                                               -*- C++ -*-
/**
 *  @file  FieldImplementation.cxx
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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author lebrun
 *  @date   2011-08-01 17:35:56 +0200 (Mon, 01 Aug 2011)
 */

#include <algorithm>
#include <string>
#include "OTconfig.hxx"
#include "FieldImplementation.hxx"
#include "StorageManager.hxx"
#include "PersistentObjectFactory.hxx"
#include "Log.hxx"
#include "Exception.hxx"
#include "ResourceMap.hxx"
#include "Path.hxx"
#include "Cloud.hxx"
#include "Curve.hxx"
#include "Os.hxx"
#include "PlatformInfo.hxx"

BEGIN_NAMESPACE_OPENTURNS

TEMPLATE_CLASSNAMEINIT(PersistentCollection<NumericalSample>);

static Factory<PersistentCollection<NumericalSample> > RegisteredFactory1("PersistentCollection<NumericalSample>");

CLASSNAMEINIT(FieldImplementation);

static Factory<FieldImplementation> RegisteredFactoryFI("FieldImplementation");

/* Default constructor is private */
FieldImplementation::FieldImplementation()
  : PersistentObject()
  , mesh_()
  , values_(0, 0)
  , description_(mesh_.getDescription())
  , spatialMean_(0)
  , isAlreadyComputedSpatialMean_(false)
{
  // Nothing to do
}

/* Standard constructor */
FieldImplementation::FieldImplementation(const Mesh & mesh,
    const UnsignedInteger dim)
  : PersistentObject()
  , mesh_(mesh)
  , values_(mesh.getVerticesNumber(), dim)
  , description_(0)
  , spatialMean_(dim)
  , isAlreadyComputedSpatialMean_(false)
{
  // Build the default description
  Description description(getMeshDimension() + getDimension());
  for (UnsignedInteger i = 0; i < getMeshDimension(); ++i) description[i] = mesh_.getVertices().getDescription()[i];
  for (UnsignedInteger i = 0; i < getDimension(); ++i) description[getMeshDimension() + i] = values_.getDescription()[i];
  setDescription(description);
}

/* Constructor from a Mesh and a sample */
FieldImplementation::FieldImplementation(const Mesh & mesh,
    const NumericalSample & values)
  : PersistentObject()
  , mesh_(mesh)
  , values_(values)
  , description_()
  , spatialMean_(values.getDimension())
  , isAlreadyComputedSpatialMean_(false)
{
  if (mesh.getVerticesNumber() != values.getSize()) throw InvalidArgumentException(HERE) << "Error: cannot build a Field with a number of values=" << values.getSize() << " different from the number of vertices=" << mesh.getVerticesNumber();
  Description description(getMeshDimension() + getDimension());
  for (UnsignedInteger i = 0; i < getMeshDimension(); ++i) description[i] = mesh_.getVertices().getDescription()[i];
  for (UnsignedInteger i = 0; i < getDimension(); ++i) description[getMeshDimension() + i] = values_.getDescription()[i];
  setDescription(description);
}

/* Virtual constructor */
FieldImplementation * FieldImplementation::clone() const
{
  return new FieldImplementation(*this);
}

/* Size accessor */
UnsignedInteger FieldImplementation::getSize() const
{
  return values_.getSize();
}

/* Dimension accessor */
UnsignedInteger FieldImplementation::getMeshDimension() const
{
  return mesh_.getDimension();
}

UnsignedInteger FieldImplementation::getDimension() const
{
  return values_.getDimension();
}

/* Mesh accessor */
Mesh FieldImplementation::getMesh() const
{
  return mesh_;
}

RegularGrid FieldImplementation::getTimeGrid() const
{
  return mesh_;
}

/* Individual value accessor */
NSI_point FieldImplementation::operator[](const UnsignedInteger index)
{
  isAlreadyComputedSpatialMean_ = false;
  return values_[index];
}

NSI_const_point FieldImplementation::operator[](const UnsignedInteger index) const
{
  return values_[index];
}

NumericalScalar & FieldImplementation::operator () (const UnsignedInteger i,
    const UnsignedInteger j)
{
  isAlreadyComputedSpatialMean_ = false;
#ifdef DEBUG_BOUNDCHECKING
  // No copyOnWrite() as the at() method already do it
  return at(i, j);
#else
  return (*this)[i][j];
#endif /* DEBUG_BOUNDCHECKING */
}

const NumericalScalar & FieldImplementation::operator () (const UnsignedInteger i,
    const UnsignedInteger j) const
{
#ifdef DEBUG_BOUNDCHECKING
  return at(i, j);
#else
  return (*this)[i][j];
#endif /* DEBUG_BOUNDCHECKING */
}


NSI_point FieldImplementation::at (const UnsignedInteger index)
{
  if (index >= getSize()) throw OutOfBoundException(HERE) << "Index (" << index << ") is not less than size (" << getSize() << ")";
  isAlreadyComputedSpatialMean_ = false;
  return (*this)[index];
}

NSI_const_point FieldImplementation::at (const UnsignedInteger index) const
{
  if (index >= getSize()) throw OutOfBoundException(HERE) << "Index (" << index << ") is not less than size (" << getSize() << ")";
  return (*this)[index];
}

NumericalScalar & FieldImplementation::at (const UnsignedInteger i,
    const UnsignedInteger j)
{
  if (i >= getSize()) throw OutOfBoundException(HERE) << "i (" << i << ") is not less than size (" << getSize() << ")";
  if (j >= getDimension()) throw OutOfBoundException(HERE) << "j (" << j << ") is not less than dimension (" << getDimension() << ")";
  isAlreadyComputedSpatialMean_ = false;
  return (*this)[i][j];
}

const NumericalScalar & FieldImplementation::at (const UnsignedInteger i,
    const UnsignedInteger j) const
{
  if (i >= getSize()) throw OutOfBoundException(HERE) << "i (" << i << ") is not less than size (" << getSize() << ")";
  if (j >= getDimension()) throw OutOfBoundException(HERE) << "j (" << j << ") is not less than dimension (" << getDimension() << ")";
  return (*this)[i][j];
}

/* Data accessors */
NumericalPoint FieldImplementation::getValueAtIndex(const UnsignedInteger index) const
{
  return values_[index];
}

void FieldImplementation::setValueAtIndex(const UnsignedInteger index,
    const NumericalPoint & val)
{
  isAlreadyComputedSpatialMean_ = false;
  values_[index] = val;
}

NumericalPoint FieldImplementation::getValueAtNearestPosition(const NumericalPoint & position) const
{
  return values_[mesh_.getNearestVertexIndex(position)];
}

void FieldImplementation::setValueAtNearestPosition(const NumericalPoint & position,
    const NumericalPoint & val)
{
  isAlreadyComputedSpatialMean_ = false;
  values_[mesh_.getNearestVertexIndex(position)] = val;
}


NumericalPoint FieldImplementation::getValueAtNearestTime(const NumericalScalar timestamp) const
{
  return getValueAtNearestPosition(NumericalPoint(1, timestamp));
}

void FieldImplementation::setValueAtNearestTime(const NumericalScalar timestamp, const NumericalPoint & val)
{
  isAlreadyComputedSpatialMean_ = false;
  setValueAtNearestPosition(NumericalPoint(1, timestamp), val);
}

/* Method __len__() is for Python */
UnsignedInteger FieldImplementation::__len__() const
{
  return getSize();
}

/* Method __contains__() is for Python */
Bool FieldImplementation::__contains__(const NumericalPoint & val) const
{
  for (UnsignedInteger i = 0; i < getSize(); ++i) if ( getValueAtIndex(i) == val ) return true;
  return false;
}

const NumericalPoint FieldImplementation::__getitem__ (const UnsignedInteger index) const
{
  return at(index);
}

void FieldImplementation::__setitem__ (const UnsignedInteger index,
                                       const NumericalPoint & val)
{
  at(index) = val;
}

/* Description Accessor */
void FieldImplementation::setDescription(const Description & description)
{
  if (description.getSize() != (getMeshDimension() + getDimension())) throw InvalidArgumentException(HERE) << "Error: the given description does not match the field input+output dimension.";
  description_ = description;
}

/* Description Accessor */
Description FieldImplementation::getDescription() const
{
  return description_;
}

/* Comparison function */
Bool FieldImplementation::operator ==(const FieldImplementation & other) const
{
  if (this == &other) return true;
  return (mesh_ == other.mesh_) && (values_ == other.values_);
}

/* String converter */
String FieldImplementation::__repr__() const
{
  OSS oss(true);
  oss << "class=" << FieldImplementation::GetClassName()
      << " name=" << getName()
      << " mesh=" << mesh_
      << " values=" << values_;
  return oss;
}

String FieldImplementation::__str__(const String & offset) const
{
  NumericalSample data(mesh_.getVertices());
  data.stack(values_);
  return data.__str__(offset);
}

/* Compute the spatial mean of the field */
void FieldImplementation::computeSpatialMean() const
{
  SpatialMeanFunctor functor( *this );
  TBB::ParallelReduce( 0, mesh_.getSimplicesNumber(), functor );
  if (functor.volumeAccumulator_ == 0.0) throw InternalException(HERE) << "Error: cannot compute the spatial mean of a field supported by a mesh of zero volume.";
  spatialMean_ = functor.accumulator_ / functor.volumeAccumulator_;
  isAlreadyComputedSpatialMean_ = true;
}


/* Compute the spatial mean of the field */
NumericalPoint FieldImplementation::getSpatialMean() const
{
  if (!isAlreadyComputedSpatialMean_) computeSpatialMean();
  return spatialMean_;
}

/* Compute the spatial mean of the field */
NumericalPoint FieldImplementation::getTemporalMean() const
{
  if (!mesh_.isRegular() || (mesh_.getDimension() != 1)) throw InvalidArgumentException(HERE) << "Error: the temporal mean is defined only when the mesh is regular and of dimension 1.";
  return values_.computeMean();
}

/* Return the values stored in the field as a sample */
NumericalSample FieldImplementation::getSample() const
{
  return values_;
}

NumericalSample FieldImplementation::getValues() const
{
  return values_;
}

/* Return the field as a sample, ie its values and positions */
NumericalSample FieldImplementation::asSample() const
{
  NumericalSample data(mesh_.getVertices());
  data.stack(values_);
  return data;
}

/* Return the field as a defomed mesh, ie its values are added to the components of the vertices if the dimensions match */
Mesh FieldImplementation::asDeformedMesh() const
{
  if (getDimension() != getMeshDimension()) throw InternalException(HERE) << "Error: cannot deform the mesh if the dimension of the values=" << values_.getDimension() << " does not match the mesh dimension=" << getMeshDimension();
  NumericalSample data(mesh_.getVertices());
  data += values_;
  return Mesh(data, mesh_.getSimplices());
}

/* Draw a marginal of the Field */
Graph FieldImplementation::drawMarginal(const UnsignedInteger index,
                                        const Bool interpolate) const
{
  if (index >= getDimension() ) throw InvalidArgumentException(HERE) << "Error : indice should be between [0, " << getDimension() - 1 << "]";
  const UnsignedInteger meshDimension(getMeshDimension());
  if (meshDimension > 2) throw NotYetImplementedException(HERE) << "Error: cannot draw a Field of mesh dimension greater than 2. Try the export to VTK for higher dimension.";
  const NumericalSample marginalValues(values_.getMarginal(index));
  const String title(OSS() << getName() << " - " << index << " marginal" );
  Graph graph(title, description_[0], "Values", true, "topright");
  if (meshDimension == 1)
  {
    // Discretization of the x axis
    if (interpolate)
    {
      const Curve curveSerie(mesh_.getVertices(), marginalValues);
      graph.add(curveSerie);
    }
    else
    {
      Cloud cloudSerie(mesh_.getVertices(), marginalValues);
      cloudSerie.setPointStyle("bullet");
      graph.add(cloudSerie);
    }
  }
  else if (meshDimension == 2)
  {
    graph.setYTitle(description_[1]);
    if (interpolate)
    {
      // Compute the iso-values
      const UnsignedInteger levelsNumber(ResourceMap::GetAsUnsignedInteger("FieldImplementation-LevelNumber"));
      NumericalPoint levels(levelsNumber);
      Description palette(levelsNumber);
      for (UnsignedInteger i = 0; i < levelsNumber; ++i)
      {
        const NumericalScalar q((i + 1.0) / (levelsNumber + 1.0));
        levels[i] = marginalValues.computeQuantile(q)[0];
	palette[i] = Curve::ConvertFromHSV((360.0 * i) / levelsNumber, 1.0, 1.0);
      }
      // Loop over the simplices to draw the segments (if any) associated with the different levels
      const UnsignedInteger simplicesNumber(mesh_.getSimplicesNumber());
      for (UnsignedInteger i = 0; i < simplicesNumber; ++i)
      {
        const Indices currentSimplex(mesh_.getSimplex(i));
        UnsignedInteger i0(currentSimplex[0]);
        UnsignedInteger i1(currentSimplex[1]);
        UnsignedInteger i2(currentSimplex[2]);
        NumericalScalar v0(marginalValues[i0][0]);
        NumericalScalar v1(marginalValues[i1][0]);
        NumericalScalar v2(marginalValues[i2][0]);
        // Sort the vertices such that v0 <= v1 <= v2
        if (v0 > v1)
        {
          std::swap(v0, v1);
          std::swap(i0, i1);
        }
        if (v1 > v2)
        {
          std::swap(v1, v2);
          std::swap(i1, i2);
        }
        // If the current simplex is constant, nothing to draw
        if (v0 == v2) continue;
        // For the current simplex, check all levels
        for (UnsignedInteger j = 0; j < levelsNumber; ++j)
        {
          const NumericalScalar level(levels[j]);
          if ((level >= v0) && (level <= v2))
          {
            const NumericalPoint x0(mesh_.getVertex(i0));
            const NumericalPoint x1(mesh_.getVertex(i1));
            const NumericalPoint x2(mesh_.getVertex(i2));
            NumericalSample data(2, 2);
            // The first point is on the [x0, x2] segment as v0 <= level <= v2 and v0 < v2
            data[0] = x0 + ((level - v0) / (v2 - v0)) * (x2 - x0);
            // if level < v1, the second point is on the [x0, x1] segment
            if (level < v1) data[1] = x0 + ((level - v0) / (v1 - v0)) * (x1 - x0);
            else if (v1 == v2) data[1] = 0.5 * (x1 + x2);
            else data[1] = x2 + ((level - v2) / (v1 - v2)) * (x1 - x2);
            graph.add(Curve(data, palette[j], "solid"));
          } // (level >= v0) && (level <= v2)
        } // j
      } // i
    } // interpolate
    else
    {
      const UnsignedInteger size(marginalValues.getSize());
      const Description palette(Cloud::BuildDefaultPalette(size));
      const NumericalSample ranks(marginalValues.rank());
      for (UnsignedInteger i = 0; i < size; ++i)
      {
        Cloud point(NumericalSample(1, mesh_.getVertex(i)));
        point.setColor(palette[static_cast<UnsignedInteger>(round(ranks[i][0]))]);
        point.setPointStyle("bullet");
        graph.add(point);
      }
    } // !interpolate
  } // meshDimension == 2
  return graph;
}


/* Method save() stores the object through the StorageManager */
void FieldImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "mesh_", mesh_);
  adv.saveAttribute( "values_", values_);
  adv.saveAttribute( "description_", description_);
  adv.saveAttribute( "spatialMean_", spatialMean_);
  adv.saveAttribute( "isAlreadyComputedSpatialMean_", isAlreadyComputedSpatialMean_);
}


/* Method load() reloads the object from the StorageManager */
void FieldImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "mesh_", mesh_);
  adv.loadAttribute( "values_", values_);
  adv.loadAttribute( "description_", description_);
  adv.loadAttribute( "spatialMean_", spatialMean_);
  adv.loadAttribute( "isAlreadyComputedSpatialMean_", isAlreadyComputedSpatialMean_);
}

/* Export to VTK file */
void FieldImplementation::exportToVTKFile(const String & fileName) const
{
  std::ofstream file(fileName.c_str(), std::ios::out);
  if (!file) throw FileNotFoundException(HERE) << "Error: can't open file " << fileName;
  const String content(mesh_.streamToVTKFormat());
  const UnsignedInteger oldPrecision(PlatformInfo::GetNumericalPrecision());
  PlatformInfo::SetNumericalPrecision(16);
  file << content << "\nPOINT_DATA " << getSize() << "\n";

  for (UnsignedInteger i = 0; i < getDimension(); ++i)
  {
    String fieldName(getDescription()[getMeshDimension() + i]);
    replace(fieldName.begin(), fieldName.end(), ' ', '~');
    if (fieldName.size() == 0) fieldName = String(OSS() << "v_" << i);
    file << "SCALARS " << fieldName << " float\nLOOKUP_TABLE default\n";
    for (UnsignedInteger j = 0; j < getSize(); ++j) file << values_[j][i] << "\n";
  }
  PlatformInfo::SetNumericalPrecision(oldPrecision);
  file.close();
}

END_NAMESPACE_OPENTURNS
