//                                               -*- C++ -*-
/**
 *  @brief This class is enables to build an exponential covariance
 *  model, a second order model's implementation
 *
 *  Copyright 2005-2025 Airbus-EDF-IMACS-ONERA-Phimeca
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
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef OPENTURNS_HAMMING_HXX
#define OPENTURNS_HAMMING_HXX

#include "openturns/FilteringWindowsImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Hamming
 */

class OT_API Hamming
  : public FilteringWindowsImplementation
{

  CLASSNAME

public:


  /** Default constructor without parameters */
  Hamming();

  /** Virtual copy constructor */
  Hamming * clone() const override;

  /** String converter */
  String __repr__() const override;

  /** String converter */
  String __str__(const String & offset = "") const override;

  /** Evaluation method  */
  Scalar operator()(const Scalar t) const override;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const override;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv) override;

} ; /* class Hamming */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_HAMMING_HXX */
