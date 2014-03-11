//                                               -*- C++ -*-
/**
 *  @file  Mesh.hxx
 *  @brief Mesh is defined as a collection of n-D vertices and simplices
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#ifndef OPENTURNS_MESH_HXX
#define OPENTURNS_MESH_HXX

#include "PersistentObject.hxx"
#include "Collection.hxx"
#include "PersistentCollection.hxx"
#include "NumericalPoint.hxx"
#include "Indices.hxx"
#include "SquareMatrix.hxx"
#include "IdentityMatrix.hxx"
#include "DomainImplementation.hxx"
#include "Graph.hxx"
#include "TBB.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Mesh
 *
 * A class that holds a mesh
 */
class Mesh
  : public DomainImplementation
{
  CLASSNAME;

public:
  typedef Collection< Indices >           IndicesCollection;
  typedef PersistentCollection< Indices > IndicesPersistentCollection;

  /** Default constructor */
  explicit Mesh(const UnsignedInteger dimension = 1);

  /** Parameters constructor */
  Mesh(const NumericalSample & vertices,
       const IndicesCollection & simplices);

  /** Virtual constructor method */
  virtual Mesh * clone() const;

  /** Check if the given point is inside of the closed mesh */
  Bool contains(const NumericalPoint & point) const;

  /** Get the description of the vertices */
  Description getDescription() const;

  /** Get the number of vertices */
  UnsignedInteger getVerticesNumber() const;

  /** Get the number of simplices */
  UnsignedInteger getSimplicesNumber() const;

  /** Get the index of the nearest vertex */
  UnsignedInteger getNearestVertexIndex(const NumericalPoint & point) const;

  /** Comparison operator */
  Bool operator == (const Mesh & rhs) const;

  /** Check mesh validity, i.e:
      non-overlaping simplices,
      no unused vertex,
      no simplices with duplicate vertices,
      no coincident vertices */
  Bool isValid() const;

  /** Check if the given point is in the given simplex */
  Bool checkPointInSimplex(const NumericalPoint & point,
                           const UnsignedInteger index) const;

  /** Vertices accessor */
  NumericalSample getVertices() const;
  void setVertices(const NumericalSample & vertices);

  /** Vertex accessor */
  NumericalPoint getVertex(const UnsignedInteger index) const;
  void setVertex(const UnsignedInteger index,
                 const NumericalPoint & vertex);

  /** Simplices accessor */
  IndicesCollection getSimplices() const;
  void setSimplices(const IndicesCollection & simplices);

  /** Simplex accessor */
  Indices getSimplex(const UnsignedInteger index) const;

  /** Compute the volume of a given simplex */
  NumericalScalar computeSimplexVolume(const UnsignedInteger index) const;

  /** Tells if the mesh is regular */
  Bool isRegular() const;

  /** Drawing method */
  Graph draw() const;
  Graph draw1D() const;
  Graph draw2D() const;
  Graph draw3D(const Bool drawEdge = true,
               const SquareMatrix & rotation = IdentityMatrix(3),
               const Bool shading = false,
               const NumericalScalar rho = 1.0) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

  /** FreeFem mesh import */
  static Mesh ImportFromMSHFile(const String & fileName);

  /** VTK export */
  String streamToVTKFormat() const;
  void exportToVTKFile(const String & fileName) const;

protected:

  /* TBB functor to speed-up volume computation */
  struct VolumeFunctor
  {
    const Mesh & mesh_;
    NumericalScalar accumulator_;

    VolumeFunctor(const Mesh & mesh)
      : mesh_(mesh), accumulator_(0.0) {}

    VolumeFunctor(const VolumeFunctor & other, TBB::Split)
      : mesh_(other.mesh_), accumulator_(0.0) {}

    void operator() (const TBB::BlockedRange<UnsignedInteger> & r)
    {
      for (UnsignedInteger i = r.begin(); i != r.end(); ++i) accumulator_ += mesh_.computeSimplexVolume(i);
    }

    void join(const VolumeFunctor & other)
    {
      accumulator_ += other.accumulator_;
    }

  }; /* end struct VolumeFunctor */

  // Build the affine matrix associated with a given simplex
  SquareMatrix buildSimplexMatrix(const UnsignedInteger index) const;

  // Compute the total volume of the mesh
  void computeVolume() const;

  // An n-D mesh is a set of vertices with a topology described by a set of simplices
  // The vertices
  NumericalSample vertices_;

  // The simplices
  IndicesPersistentCollection simplices_;

}; /* class Mesh */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_MESH_HXX */
