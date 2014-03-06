//                                               -*- C++ -*-
/**
 *  @file  Curve.cxx
 *  @brief Curve class for curve plots
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
#include "Curve.hxx"
#include "PersistentObjectFactory.hxx"
#include "Log.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Curve);

static Factory<Curve> RegisteredFactory("Curve");

/* Default constructor */
Curve::Curve(const String & legend)
  : DrawableImplementation(NumericalSample(0, 2), legend)
{
  // Nothing to do
  setPointStyle("none");
}

/* Default constructor */
Curve::Curve(const NumericalSample & data,
             const String & legend)
  : DrawableImplementation(NumericalSample(0, 2), legend)
{
  NumericalSample dataFull;
  // If data is unidimensional, assume that it means Y values with indices as X values
  if (data.getDimension() == 1)
  {
    const UnsignedLong size(data.getSize());
    dataFull = NumericalSample(size, 2);
    for (UnsignedLong i = 0; i < size; ++i)
    {
      dataFull[i][0] = i;
      dataFull[i][1] = data[i][0];
    }
  }
  else dataFull = data;
  // Check data validity
  setData(dataFull);
  setPointStyle("none");
}

/* Contructor from 2 data sets */
Curve::Curve(const NumericalSample & dataX,
             const NumericalSample & dataY,
             const String & legend)
  : DrawableImplementation(NumericalSample(0, 2), legend)
{
  const UnsignedLong size(dataX.getSize());
  if (dataY.getSize() != size) throw InvalidArgumentException(HERE) << "Error: cannot build a Curve based on two numerical samples with different size.";
  if ((dataX.getDimension() != 1) || (dataY.getDimension() != 1)) throw InvalidArgumentException(HERE) << "Error: cannot build a Curve based on two numerical samples of dimension greater than 1.";
  NumericalSample dataFull(size, 2);
  for (UnsignedLong i = 0; i < size; ++i)
  {
    dataFull[i][0] = dataX[i][0];
    dataFull[i][1] = dataY[i][0];
  }
  // Check data validity
  setData(dataFull);
  setPointStyle("none");
}

/* Contructor from 2 data sets */
Curve::Curve(const NumericalPoint & dataX,
             const NumericalPoint & dataY,
             const String & legend)
  : DrawableImplementation(NumericalSample(0, 2), legend)
{
  const UnsignedLong size(dataX.getDimension());
  if (dataY.getDimension() != size) throw InvalidArgumentException(HERE) << "Error: cannot build a Curve based on two numerical points with different dimension.";
  NumericalSample dataFull(size, 2);
  for (UnsignedLong i = 0; i < size; ++i)
  {
    dataFull[i][0] = dataX[i];
    dataFull[i][1] = dataY[i];
  }
  // Check data validity
  setData(dataFull);
  setPointStyle("none");
}

/* Constructor with parameters */
Curve::Curve(const NumericalSample & data,
             const String & color,
             const String & lineStyle,
             const UnsignedLong lineWidth,
             const String & legend)
  : DrawableImplementation(NumericalSample(0, 2), legend)
{
  NumericalSample dataFull;
  // If data is unidimensional, assume that it means Y values with indices as X values
  if (data.getDimension() == 1)
  {
    const UnsignedLong size(data.getSize());
    dataFull = NumericalSample(size, 2);
    for (UnsignedLong i = 0; i < size; ++i)
    {
      dataFull[i][0] = i;
      dataFull[i][1] = data[i][0];
    }
  }
  else dataFull = data;
  // Check data validity
  setData(dataFull);
  setLineStyle(lineStyle);
  setLineWidth(lineWidth);
  setColor(color);
  setPointStyle("none");
}

/* String converter */
String Curve::__repr__() const
{
  OSS oss;
  oss << "class=" << Curve::GetClassName()
      << " name=" << getName()
      << " derived from " << DrawableImplementation::__repr__();
  return oss;
}

/* Draw method */
String Curve::draw() const
{
  dataFileName_ = "";
  OSS oss;
  // Stores the data in a temporary file
  oss << DrawableImplementation::draw() << "\n";
  // The specific R command for drawing
  oss << "lines(dataOT[,1], dataOT[,2]"
      << ", lty=\"" << lineStyle_
      << "\", col=\"" << color_
      << "\", lwd=" << lineWidth_;
  if (pointStyle_ != "none")
  {
    const String code((OSS() << getPointCode(pointStyle_)));
    oss << ", type=\"b\""
        << ", pch=" << (pointStyle_ == "dot" ? "\".\"" : code);
  }
  else
  {
    oss << ", type=\"l\"";
  }
  oss << ")";

  return oss;
}

/* Clone method */
Curve * Curve::clone() const
{
  return new Curve(*this);
}
/* Check validity of data */
void Curve::checkData(const NumericalSample & data) const
{
  if (data.getDimension() != 2)
  {
    throw InvalidDimensionException(HERE) << "Expected sample of dimension 2: got " << data.getDimension();
  }
}

/* Method save() stores the object through the StorageManager */
void Curve::save(Advocate & adv) const
{
  DrawableImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void Curve::load(Advocate & adv)
{
  DrawableImplementation::load(adv);
}

END_NAMESPACE_OPENTURNS
