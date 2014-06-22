//                                               -*- C++ -*-
/**
 *  @file  Polygon.hxx
 *  @brief Polygon class for polygon plots
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
#ifndef OPENTURNS_POLYGON_HXX
#define OPENTURNS_POLYGON_HXX

#include "DrawableImplementation.hxx"
#include "NumericalSample.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Polygon
 *
 * The class describing a polygon plot
 * Instance of Drawable
 */


class OT_API Polygon : public DrawableImplementation
{

  CLASSNAME;

public:


  /** Default constructor */
  explicit Polygon(const String & legend = "");

  /** Default constructor */
  explicit Polygon(const NumericalSample & data,
                   const String & legend = "");

  /** Contructor from 2 data sets */
  Polygon(const NumericalSample & dataX,
          const NumericalSample & dataY,
          const String & legend = "");

  /** Contructor from 2 data sets */
  Polygon(const NumericalPoint & dataX,
          const NumericalPoint & dataY,
          const String & legend = "");

  /** Constructor with parameters */
  Polygon(const NumericalSample & data,
          const String & color,
          const String & edgeColor,
          const String & legend = "");

  /** Constructor with parameters */
  Polygon(const NumericalSample & data,
          const Description & colors,
          const String & legend = "");

  /** String converter */
  String __repr__() const;

  /** Draw method */
  String draw() const;

  /** Clone method */
  virtual Polygon * clone() const;


  /** Color of the edge
   */
  void setEdgeColor(const String & edgeColor);
  String getEdgeColor() const;


  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() stores the object through the StorageManager */
  void load(Advocate & adv);

  /** Remove all the temporary files */
  void clean() const;

protected:
  /** Check fo data validity */
  virtual void checkData(const NumericalSample & data) const;

private:
  /** Color of the edge of the polygon */
  String edgeColor_;

  /** List of colors for strip of polygons */
  Description colors_;

  /** Name of the file to store colors of a strip */
  mutable String colorsFileName_;
}; /* class Polygon */



END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_POLYGON_HXX */
