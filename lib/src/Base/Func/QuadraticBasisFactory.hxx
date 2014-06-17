//                                               -*- C++ -*-
/**
 *  @file  QuadraticBasisFactory.hxx
 *  @brief Abstract top-level class for all QuadraticBasisFactory
 *
 *  (C) Copyright 2005-2014 Airbus-EDF-Phimeca
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License.
 *
 *  This library is distributed in the hope that it will be useful
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 *  @author schueller
 */
#ifndef OPENTURNS_QUADRATICBASISFACTORY_HXX
#define OPENTURNS_QUADRATICBASISFACTORY_HXX

#include "BasisFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

class OT_API QuadraticBasisFactory
  : public BasisFactory
{
  CLASSNAME;
public:

  /** Default constructor */
  QuadraticBasisFactory (const UnsignedInteger inputDimension = 1);

  /* String converter */
  OT::String __repr__() const;

  /* Here is the interface that all derived class must implement */

  /* Virtual constructor */
  virtual QuadraticBasisFactory * clone() const;

  /** @copydoc BasisFactory::build() const */
  virtual Basis build() const;

  /* Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const;

  /* Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv);

private:
  UnsignedInteger inputDimension_;
};

END_NAMESPACE_OPENTURNS

#endif
