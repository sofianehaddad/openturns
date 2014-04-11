//                                               -*- C++ -*-
/**
 *  @file  IntervalMesher.cxx
 *  @brief Meshing algorithm for intervals
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
 *  @author: $LastChangedBy$
 *  @author: $LastChangedBy$
 *  @date:   $LastChangedDate$
 *  Id:      $Id$
 */
#include "PersistentObjectFactory.hxx"
#include "IntervalMesher.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(IntervalMesher);
static Factory<IntervalMesher> RegisteredFactory("IntervalMesher");


/* Default constructor */
IntervalMesher::IntervalMesher()
  : MeshFactoryImplementation()
  , discretization_(0)
{
  // Nothing to do
}

/* Parameter constructor */
IntervalMesher::IntervalMesher(const Indices & discretization)
  : MeshFactoryImplementation()
  , discretization_(discretization)
{
  // Nothing to do
}

/* Virtual constructor */
IntervalMesher * IntervalMesher::clone() const
{
  return new IntervalMesher(*this);
}

/* String converter */
String IntervalMesher::__repr__() const
{
  OSS oss(true);
  oss << "class=" << IntervalMesher::GetClassName()
      << " discretization=" << discretization_;
  return oss;
}

/* String converter */
String IntervalMesher::__str__(const String & offset) const
{
  return __repr__();
}

/* Discretization accessors */
void IntervalMesher::setDiscretization(const Indices & discretization)
{
  // At least one slice per dimension
  for (UnsignedInteger i = 0; i < discretization.getSize(); ++i)
    if (discretization_[i] == 0) throw InvalidArgumentException(HERE) << "Error: expected positive values for the discretization, here discretization[" << i << "]=" << discretization[i];
  discretization_ = discretization;
}

Indices IntervalMesher::getDiscretization() const
{
  return discretization_;
}

/* Here is the interface that all derived class must implement */

Mesh IntervalMesher::build(const Interval & interval) const
{
  const UnsignedInteger dimension(interval.getDimension());
  if (discretization_.getSize() != dimension) throw InvalidArgumentException(HERE) << "Error: the mesh factory is for intervals of dimension=" << discretization_.getSize() << ", here dimension=" << dimension;
  if (dimension > 2) throw NotYetImplementedException(HERE);
  // Waiting for a generic implementation in higher dimension
  if (dimension == 1)
    {
      // We must insure that the interval bounds will be within the vertices
      const UnsignedInteger n(discretization_[0]);
      NumericalSample vertices(n + 1, 1);
      // First the vertices
      vertices[0] = interval.getLowerBound();
      vertices[n] = interval.getUpperBound();
      for (UnsignedInteger i = 0; i <= n; ++i) vertices[i][0] = (i * vertices[0][0] + (n - i) * vertices[0][n]) / n;
      // Second the simplices
      Mesh::IndicesCollection simplices(n);
      Indices simplex(2);
      for (UnsignedInteger i = 0; i < n; ++i)
        {
          simplex[0] = i;
          simplex[1] = i + 1;
          simplices[i] = simplex;
        } // i
      return Mesh(vertices, simplices);
    } // dimension == 1
  if (dimension == 2)
    {
      const UnsignedInteger m(discretization_[0]);
      const UnsignedInteger n(discretization_[1]);
      // First the vertices
      NumericalSample vertices(0, 2);
      NumericalPoint point(2);
      for (UnsignedInteger i = 0; i <= m; ++i)
        {
          point[0] = (i * interval.getLowerBound()[0] + (m - i) * interval.getUpperBound()[0]) / m;
          for (UnsignedInteger j = 0; j <= n; ++j)
            {
              point[1] = (j * interval.getLowerBound()[1] + (n - j) * interval.getUpperBound()[1]) / n;
              vertices.add(point);
            } // j
        } // i
      // Second the simplices
      Mesh::IndicesCollection simplices(0, Indices(3));
      UnsignedInteger vertexIndex(0);
      Indices index(3);
      for (UnsignedInteger i = 0; i < m; ++i)
        {
          for (UnsignedInteger j = 0; j < n; ++j)
            {
              index[0] = vertexIndex;
              index[1] = vertexIndex + 1;
              index[2] = vertexIndex + 1 + n;
              simplices.add(index);
              index[0] = vertexIndex + 2 + n;
              simplices.add(index);
              ++vertexIndex;
            } // j
          ++vertexIndex;
        } // i
      return Mesh(vertices, simplices);
    } // dimension == 2
}

END_NAMESPACE_OPENTURNS
