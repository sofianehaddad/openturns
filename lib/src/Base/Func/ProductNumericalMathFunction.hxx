//                                               -*- C++ -*-
/**
 *  @file  ProductNumericalMathFunction.hxx
 *  @brief The class that implements product numerical math functions
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
 *  @author: $LastChangedBy: lebrun $
 *  @date:   $LastChangedDate: 2008-07-03 08:31:34 +0200 (jeu, 03 jui 2008) $
 *  Id:      $Id: ProductNumericalMathFunction.hxx 867 2008-07-03 06:31:34Z lebrun $
 */
#ifndef OPENTURNS_PRODUCTNUMERICALMATHFUNCTION_HXX
#define OPENTURNS_PRODUCTNUMERICALMATHFUNCTION_HXX

#include "NumericalMathFunctionImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class ProductNumericalMathFunction
 */

class ProductNumericalMathFunction
  : public NumericalMathFunctionImplementation
{
  CLASSNAME;
public:

  /* Some typedefs for easy reading */

  /** Composition constructor */
  ProductNumericalMathFunction(const Implementation & p_left,
                               const Implementation & p_right);

  /** Virtual constructor */
  virtual ProductNumericalMathFunction * clone() const;

  /** Comparison operator */
  Bool operator ==(const ProductNumericalMathFunction & other) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;



  /** Gradient according to the marginal parameters */
  virtual Matrix parametersGradient(const NumericalPoint & inP) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  ProductNumericalMathFunction() {};
  friend class Factory<ProductNumericalMathFunction>;

private:

  /** The f function in fog */
  Implementation p_leftFunction_;

  /** The g function in fog */
  Implementation p_rightFunction_;

}; /* class ProductNumericalMathFunction */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PRODUCTNUMERICALMATHFUNCTION_HXX */
