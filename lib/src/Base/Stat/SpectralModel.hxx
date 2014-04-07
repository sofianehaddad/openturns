//                                               -*- C++ -*-
/**
 *  @file  SpectralModel.hxx
 *  @brief This class is enables to build a spectral model using its spectral density function
 *  This is the interface class
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
#ifndef OPENTURNS_SPECTRALMODEL_HXX
#define OPENTURNS_SPECTRALMODEL_HXX

#include "SpectralModelImplementation.hxx"
#include "TypedInterfaceObject.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class SpectralModel
 */

class SpectralModel
  : public TypedInterfaceObject<SpectralModelImplementation>
{

  CLASSNAME;

public:

  typedef SpectralModelImplementation::Implementation Implementation;

  /** Default constructor */
  SpectralModel();

  /** Copy constructors */
  SpectralModel(const SpectralModelImplementation & implementation);

  /** Constructor from implementation */
  SpectralModel(const Implementation & p_implementation);

#ifndef SWIG
  /** Constructor from implementation pointer */
  SpectralModel(SpectralModelImplementation * p_implementation);
#endif


  /** Dimension accessor */
  UnsignedInteger getDimension() const;
  void setDimension(const UnsignedInteger dimension);

  /** Computation of the spectral density function */
  /** @deprecated */
  virtual HermitianMatrix computeSpectralDensity(const NumericalScalar frequency) const;

  virtual HermitianMatrix operator() (const NumericalScalar frequency) const;

  /** Frequency grid accessors */
  virtual RegularGrid getFrequencyGrid() const;
  virtual void setFrequencyGrid(const RegularGrid & frequencyGrid);

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

} ; /* class SpectralModel */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SPECTRALMODEL_HXX */
