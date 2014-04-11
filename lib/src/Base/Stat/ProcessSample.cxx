//                                               -*- C++ -*-
/**
 *  @file  ProcessSample.cxx
 *  @brief ProcessSample Class
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

#include "ProcessSample.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"
#include "ResourceMap.hxx"
#include "Drawable.hxx"
#include "Description.hxx"

BEGIN_NAMESPACE_OPENTURNS

TEMPLATE_CLASSNAMEINIT(PersistentCollection<ProcessSample>);

TEMPLATE_CLASSNAMEINIT(PersistentCollection<Field>);

static Factory<PersistentCollection<Field> > RegisteredFactory1("PersistentCollection<Field>");

CLASSNAMEINIT(ProcessSample);

static Factory<ProcessSample> RegisteredFactory("ProcessSample");

ProcessSample::ProcessSample()
  : PersistentObject()
  , mesh_()
  , data_(0)
{
  // Nothing to do
}

/* Default constructor */
ProcessSample::ProcessSample(const UnsignedInteger size,
                             const Field & field)
  : PersistentObject()
  , mesh_(field.getMesh())
  , data_(NumericalSampleCollection(size, field.getValues()))
{
  // Nothing to do
}

ProcessSample::ProcessSample(const Mesh & mesh,
                             const UnsignedInteger size,
                             const UnsignedInteger dimension)
  : PersistentObject()
  , mesh_(mesh)
  , data_(NumericalSampleCollection(size, NumericalSample(mesh.getVerticesNumber(), dimension)))
{
  // Nothing to do
}

/* Virtual constructor */
ProcessSample * ProcessSample::clone() const
{
  return new ProcessSample(*this);
}

/* String converter */
String ProcessSample::__repr__() const
{
  OSS oss(true);
  oss << "class=" << ProcessSample::GetClassName()
      << " mesh=" << mesh_
      << " values=" << data_;
  return oss;
}

String ProcessSample::__str__(const String & offset) const
{
  OSS oss(false);
  oss << offset << "[";
  String separator("");
  for (UnsignedInteger i = 0; i < data_.getSize(); ++i, separator = "\n") oss << separator << offset << "field " << i << ":\n" << getField(i).__str__(offset);
  oss << "]";
  return oss;
}

void ProcessSample::add(const Field & field)
{
  if (getSize() == 0)
  {
    data_.add(field.getValues());
    mesh_ = field.getMesh();
  }
  else if ((data_[0].getDimension() == field.getDimension()) && (mesh_ == field.getMesh())) data_.add(field.getValues());
  else throw InvalidArgumentException(HERE) << "Error: could not add the fiels. Either its dimenson or its mesh are incompatible.";
}


/* Operators accessors */
Field ProcessSample::getField(const UnsignedInteger index) const
{
  if (index >= data_.getSize()) throw InvalidArgumentException(HERE)  << " Error - index should be between 0 and " << data_.getSize() - 1;
  return Field(mesh_, data_[index]);
}

void ProcessSample::setField(const UnsignedInteger index,
                             const Field & field)
{
  if (index >= data_.getSize()) throw InvalidArgumentException(HERE)  << " Error - index should be between 0 and " << data_.getSize() - 1;
  if (field.getDimension() != data_[0].getDimension()) throw InvalidArgumentException(HERE) << "Error: expected a field of dimension=" << data_[0].getDimension() << ", got a field of dimension=" << field.getDimension();
  data_[index] = field.getValues();
}

NumericalSample & ProcessSample::operator[] (const UnsignedInteger index)
{
  if (index >= data_.getSize()) throw InvalidArgumentException(HERE)  << " Error - index should be between 0 and " << data_.getSize() - 1;
  return data_[index];
}

const NumericalSample & ProcessSample::operator[] (const UnsignedInteger index) const
{
  if (index >= data_.getSize()) throw InvalidArgumentException(HERE)  << " Error - index should be between 0 and " << data_.getSize() - 1;
  return data_[index];
}

/* Method __getitem__() - Python use */
Field ProcessSample::__getitem__ (const UnsignedInteger i) const
{
  return getField(i);
}

/* Method __setitem__() is for Python */
void ProcessSample::__setitem__(const UnsignedInteger i, const Field & field)
{
  setField(i, field);
}

/* Time grid accessors */
RegularGrid ProcessSample::getTimeGrid() const
{
  return RegularGrid(mesh_);
}

/* Mesh accessors */
Mesh ProcessSample::getMesh() const
{
  return mesh_;
}

/* Dimension accessors */
UnsignedInteger ProcessSample::getDimension() const
{
  if (data_.getSize() == 0) return 0;
  return data_[0].getDimension();
}

/* Dimension accessors */
UnsignedInteger ProcessSample::getSize() const
{
  return data_.getSize();
}

Field ProcessSample::computeMean() const
{
  const UnsignedInteger size(getSize());
  if (size == 0) return Field();
  if (size == 1) return Field(mesh_, data_[0]);
  NumericalSample meanValues(data_[0]);
  for (UnsignedInteger i = 1; i < size; ++i) meanValues += data_[i];
  meanValues *= NumericalPoint(getDimension(), 1.0 / size);
  return Field(mesh_, meanValues);
}

/* Compute the sample of spatial means of each field */
NumericalSample ProcessSample::computeTemporalMean() const
{
  if (!mesh_.isRegular() || (mesh_.getDimension() != 1)) throw InvalidArgumentException(HERE) << "Error: the temporal mean is defined only when the mesh is regular and of dimension 1.";
  return computeSpatialMean();
}

/* Compute the sample of spatial means of each field */
NumericalSample ProcessSample::computeSpatialMean() const
{
  const UnsignedInteger size(getSize());
  const UnsignedInteger dimension(getDimension());
  NumericalSample result(size, dimension);
  for (UnsignedInteger i = 0; i < size; ++i) result[i] = data_[i].computeMean();
  return result;
}

/*
 * Method computeQuantilePerComponent() gives the quantile per component of the sample
 */
Field ProcessSample::computeQuantilePerComponent(const NumericalScalar prob) const
{
  const UnsignedInteger size(getSize());
  if (size == 0) return Field();
  if (size == 1) return Field(mesh_, data_[0]);
  // This initialization set the correct time grid into result
  const UnsignedInteger dimension(data_[0].getDimension());
  const UnsignedInteger length(data_[0].getSize());
  NumericalSample result(length, dimension);
  // Loop over the location indices
  for (UnsignedInteger i = 0; i < length; ++i)
  {
    NumericalSample dataI(size, dimension);
    for (UnsignedInteger j = 0; j < size; ++j)
      dataI[j] = data_[j][i];
    result[i] = dataI.computeQuantilePerComponent(prob);
  }
  return Field(mesh_, result);
}

/* Draw a marginal of the ProcessSample, ie the collection of all the Field marginals */
Graph ProcessSample::drawMarginal(const UnsignedInteger index) const
{
  if (index > getDimension() - 1 ) throw InvalidArgumentException(HERE) << "Error : indice should be between [0, " << getDimension() - 1 << "]";

  // Discretization of the x axis
  const String title(OSS() << getName() << " - " << index << " marginal" );
  Graph graph(title, "Time", "Values", true, "topright");
  const UnsignedInteger size(data_.getSize());
  const Description colors(Drawable::BuildDefaultPalette(size));
  for (UnsignedInteger i = 0; i < size; ++i)
  {
    Drawable drawable(Field(mesh_, data_[i]).drawMarginal(index).getDrawable(0));
    drawable.setColor(colors[i]);
    graph.add(drawable);
  }
  return graph;
}

/* Method save() stores the object through the StorageManager */
void ProcessSample::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "mesh_", mesh_);
  adv.saveAttribute( "data_", data_ );
}

/* Method load() reloads the object from the StorageManager */
void ProcessSample::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "mesh_", mesh_);
  adv.loadAttribute( "data_", data_ );
}

END_NAMESPACE_OPENTURNS
