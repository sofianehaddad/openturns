//                                               -*- C++ -*-
/**
 *  @file  KDTree.cxx
 *  @brief KDTree structure to speed-up queries on large samples
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
 *  @date   2010-11-09 13:44:00 +0100 (Tue, 09 Nov 2010)
 */

#include "Exception.hxx"
#include "KDTree.hxx"
#include "SpecFunc.hxx"
#include "Indices.hxx"
#include "SobolSequence.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(KDTree);

/**
 * @class KDTree
 *
 * Organize d-dimensional points into a hierarchical tree-like structure
 */

/* Default constructor */
KDTree::KDTree()
  : Object()
  , points_(0, 0)
  , p_root_(0)
{
  // Nothing to do
}

/* Parameters constructor */
KDTree::KDTree(const NumericalSample & points)
  : Object()
  , points_(points)
  , p_root_(0)
{
  // Build the tree
  // Scramble the order in which the points are inserted in the tree in order to improve its balancing
  const UnsignedInteger size(points.getSize());
  Indices buffer(size);
  buffer.fill();
  SobolSequence sequence(1);
  for (UnsignedInteger i = 0; i < points.getSize(); ++i)
    {
      const UnsignedInteger index(i + static_cast< UnsignedInteger >((size - i) * sequence.generate()[0]));
      insert(p_root_, buffer[index], 0);
      buffer[index] = buffer[i];
    }
}

/* Virtual constructor */
KDTree * KDTree::clone() const
{
  return new KDTree( *this );
}

/* String converter */
String KDTree::__repr__() const
{
  return OSS() << "class=" << GetClassName()
               << " root=" << p_root_->__repr__();
}

/* Check if the tree is empty */
Bool KDTree::isEmpty() const
{
  return points_.getSize() == 0;
}

/* Insert the given point into the tree */
void KDTree::insert(const NumericalPoint & point)
{
  if (point.getDimension() != points_.getDimension()) throw InvalidArgumentException(HERE) << "Error: expected a point of dimension=" << points_.getDimension() << ", got dimension=" << point.getDimension();
  points_.add(point);
  insert(p_root_, points_.getSize(), 0);
}

/* Insert the point at given index into the tree */
void KDTree::insert(KDNode::KDNodePointer & p_node,
                    const UnsignedInteger index,
                    const UnsignedInteger activeDimension)
{
  if (index >= points_.getSize()) throw InvalidArgumentException(HERE) << "Error: expected an index less than " << points_.getSize() << ", got " << index;
  // We are on a leaf
  if (p_node == 0) p_node = new KDNode(index);
  else if (points_[index][activeDimension] < points_[p_node->index_][activeDimension]) insert(p_node->p_left_, index, (activeDimension + 1) % points_.getDimension());
  else insert(p_node->p_right_, index, (activeDimension + 1) % points_.getDimension());
}

// /* Get the indices of the k nearest neighbours of the given point */
// Indices KDTree::getNearestNeighboursIndices(const NumericalPoint & x,
//                                             const UnsignedInteger k) const
// {
//   if (k > points_.getSize()) throw InvalidArgumentException(HERE) << "Error: cannot return more neighbours than points in the database!";
//   Indices result(0);
//   // If we need as many neighbours as points in the sample, just return all the possible indices
//   if (k == points_.getSize())
//     {
//       result = Indices(k);
//       result.fill();
//       return result;
//     }
//   NumericalPoint bestSquaredDistance(0);
//   return getNearestNeighboursIndices(p_root_, x, k, bestSquaredDistance, 0);
// }

// /* Recursive method to find the indices of the k nearest neighbours
//    Strategy:
//    + for a new candidate, if there is still room just add it to the list of neighbours
//    + else replace the worst candidate from the current list by the new candidate
//    Complexity: O(k) at each insertion, O(log(n)) expected insertions
//    A clever structure for the neighbours could probably improve the complexity of the insertion.

//    More details:
//    + We come from an upper level, so the best squared distances correspond to nodes seen so far either on top or on the left of the current node
//    + On the descending phase, the distances are not updated
//    + On the ascending phase, the distances are first aggregated, then they are compressed
// */
// Indices KDTree::getNearestNeighboursIndices(const KDNode::KDNodePointer & p_node,
// 					    const NumericalPoint & x,
//                                             const UnsignedInteger k,
// 					    NumericalPoint & bestSquaredDistances,
// 					    const UnsignedInteger activeDimension) const
// {
//   const NumericalScalar delta(x[activeDimension] - points_[p_node->index_][activeDimension]);
//   const KDNode::KDNodePointer & sameSide(delta < 0.0 ? p_node->p_left_ : p_node->p_right_);
//   const KDNode::KDNodePointer & oppositeSide(delta < 0.0 ? p_node->p_right_ : p_node->p_left_);
//   Indices currentBestIndices(k, points_.getSize());
//   NumericalPoint currentBestSquaredDistance(bestSquaredDistances);
//   // 1)
//   if (sameSide != 0)
//     {
//       // 1.1)
//       Indices candidateBestIndices(getNearestNeighbourIndices(sameSide, x, bestSquaredDistances, (activeDimension + 1) % points_.getDimension()));
//       if (bestSquaredDistance < currentBestSquaredDistance)
// 	{
// 	  currentBestSquaredDistance = bestSquaredDistance;
// 	  currentBestIndex = candidateBestIndex;
// 	}
//     } // sameSide != 0
//   // 2)
//   if (currentBestSquaredDistance < delta * delta)
//     {
//       // 2.1)
//       bestSquaredDistance = currentBestSquaredDistance;
//       return currentBestIndex;
//     }
//   // 2.2)
//   const UnsignedInteger localIndex(p_node->index_);
//   const NumericalScalar localSquaredDistance((x - points_[localIndex]).normSquare());
//   if (localSquaredDistance < currentBestSquaredDistance)
//     {
//       currentBestSquaredDistance = localSquaredDistance;
//       // To send the current best squared distance to the lower levels
//       bestSquaredDistance = localSquaredDistance;
//       currentBestIndex = localIndex;
//     }
//   // 2.3)
//   if (oppositeSide != 0)
//     {
//       // 2.4)
//       UnsignedInteger candidateBestIndex(getNearestNeighbourIndex(oppositeSide, x, bestSquaredDistance, (activeDimension + 1) % points_.getDimension()));
//       if (bestSquaredDistance < currentBestSquaredDistance)
// 	{
// 	  currentBestSquaredDistance = bestSquaredDistance;
// 	  currentBestIndex = candidateBestIndex;
// 	}
//     } // oppositeSide != 0
//   // 3)
//   bestSquaredDistance = currentBestSquaredDistance;
//   throw NotYetImplementedException(HERE);
// }

// /* Get the k nearest neighbours of the given point */
// NumericalSample KDTree::getNearestNeighbours(const NumericalPoint & x,
// 					     const UnsignedInteger k) const
// {
//   if (k > points_.getSize()) throw InvalidArgumentException(HERE) << "Error: cannot return more neighbours than points in the database!";
//   const Indices indices(getNearestNeighboursIndices(x, k));
//   NumericalSample result(k, points_.getDimension());
//   for (UnsignedInteger i = 0; i < k; ++i) result[i] = points_[indices[i]];
//   return result;
// }

UnsignedInteger KDTree::getNearestNeighbourIndex(const NumericalPoint & x) const
{
  if (points_.getSize() == 1) return 0;
  NumericalScalar smallestDistance(SpecFunc::MaxNumericalScalar);
  return getNearestNeighbourIndex(p_root_, x, smallestDistance, 0);
}

NumericalPoint KDTree::getNearestNeighbour(const NumericalPoint & x) const
{
  return points_[getNearestNeighbourIndex(x)];
}

UnsignedInteger KDTree::getNearestNeighbourIndex(const KDNode::KDNodePointer & p_node,
                                                 const NumericalPoint & x,
                                                 NumericalScalar & bestSquaredDistance,
                                                 const UnsignedInteger activeDimension) const
{
  if (p_node == 0) throw NotDefinedException(HERE) << "Error: cannot find a nearest neighbour in an emty tree";
  // Set delta = x[activeDimension] - points_[p_node->index_]
  // sameSide = p_node->p_left_ if delta < 0, p_node->p_right_ else
  // oppositeSide = p_node->p_right_ if delta < 0, p_node->p_left_ else
  // Possibilities:
  // 1) sameSide != 0
  // 1.1) Go on the same side. On return, the index is the best candidate index. If the associated distance is less than the current best index, update the current best index and the associated squared distance.
  // 2) Check if the current best squared distance is less than delta^2
  // 2.1*) If yes, the points associated to p_node or to its opposite side can't be better than the current best candidate so return it and the associated squared distance to the upper level
  // 2.2) If no, check the point associated to the current node and update the current best index and the associated squared distance
  // 2.3) oppositeSide != 0
  // 2.4) Go on the opposite side. On return, check if the returned squared distance is less than the current best squared distance and update the current best index and the associated squared distance.
  // 3*) Return the current best index and the associated squared distance to the upper level

  const NumericalScalar delta(x[activeDimension] - points_[p_node->index_][activeDimension]);
  const KDNode::KDNodePointer & sameSide(delta < 0.0 ? p_node->p_left_ : p_node->p_right_);
  const KDNode::KDNodePointer & oppositeSide(delta < 0.0 ? p_node->p_right_ : p_node->p_left_);
  UnsignedInteger currentBestIndex(points_.getSize());
  NumericalScalar currentBestSquaredDistance(bestSquaredDistance);
  // 1)
  if (sameSide != 0)
    {
      // 1.1)
      UnsignedInteger candidateBestIndex(getNearestNeighbourIndex(sameSide, x, bestSquaredDistance, (activeDimension + 1) % points_.getDimension()));
      if (bestSquaredDistance < currentBestSquaredDistance)
	{
	  currentBestSquaredDistance = bestSquaredDistance;
	  currentBestIndex = candidateBestIndex;
	}
    } // sameSide != 0
  // 2)
  if (currentBestSquaredDistance < delta * delta)
    {
      // 2.1)
      bestSquaredDistance = currentBestSquaredDistance;
      return currentBestIndex;
    }
  // 2.2)
  const UnsignedInteger localIndex(p_node->index_);
  const NumericalScalar localSquaredDistance((x - points_[localIndex]).normSquare());
  if (localSquaredDistance < currentBestSquaredDistance)
    {
      currentBestSquaredDistance = localSquaredDistance;
      // To send the current best squared distance to the lower levels
      bestSquaredDistance = localSquaredDistance;
      currentBestIndex = localIndex;
    }
  // 2.3)
  if (oppositeSide != 0)
    {
      // 2.4)
      UnsignedInteger candidateBestIndex(getNearestNeighbourIndex(oppositeSide, x, bestSquaredDistance, (activeDimension + 1) % points_.getDimension()));
      if (bestSquaredDistance < currentBestSquaredDistance)
	{
	  currentBestSquaredDistance = bestSquaredDistance;
	  currentBestIndex = candidateBestIndex;
	}
    } // oppositeSide != 0
  // 3)
  bestSquaredDistance = currentBestSquaredDistance;
  return currentBestIndex;
}

/* Points accessor */
NumericalSample KDTree::getPoints() const
{
  return points_;
}

END_NAMESPACE_OPENTURNS
