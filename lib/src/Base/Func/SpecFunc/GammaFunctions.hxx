//                                               -*- C++ -*-
/**
 *  @file  GammaFunctions.hxx
 *  @brief Efficient implementation of the computation of the incomplete
 *         regularized gamma function and related functions
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
 *  @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)
 */
#ifndef OPENTURNS_GAMMAFUNCTIONS_HXX
#define OPENTURNS_GAMMAFUNCTIONS_HXX

#include "OTprivate.hxx"

BEGIN_NAMESPACE_OPENTURNS

namespace GammaFunctions
{
NumericalScalar IncompleteGamma(const NumericalScalar a,
                                const NumericalScalar x,
                                const Bool tail = false);

NumericalScalar IncompleteGammaInverse(const NumericalScalar a,
                                       const NumericalScalar x,
                                       const Bool tail = false);

NumericalScalar RegularizedIncompleteGamma(const NumericalScalar a,
    const NumericalScalar x,
    const Bool tail = false);

NumericalScalar RegularizedIncompleteGammaInverse(const NumericalScalar a,
    const NumericalScalar x,
    const Bool tail = false);

} // GammaFunctions

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_GAMMAFUNCTIONS_HXX */
