//                                               -*- C++ -*-
/**
 *  @file  BetaFunctions.hxx
 *  @brief Efficient implementation of the computation of the incomplete
 *         regularized beta function and related functions
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
#ifndef OPENTURNS_BETAFUNCTIONS_HXX
#define OPENTURNS_BETAFUNCTIONS_HXX

#include "OTprivate.hxx"

BEGIN_NAMESPACE_OPENTURNS

namespace BetaFunctions
{
NumericalScalar IncompleteBeta(const NumericalScalar a,
                               const NumericalScalar b,
                               const NumericalScalar x,
                               const Bool tail = false);

NumericalScalar IncompleteBetaInverse(const NumericalScalar a,
                                      const NumericalScalar b,
                                      const NumericalScalar x,
                                      const Bool tail = false);

NumericalScalar IncompleteBetaP(const NumericalScalar a,
                                const NumericalScalar b,
                                const NumericalScalar x);

NumericalScalar IncompleteBetaQ(const NumericalScalar a,
                                const NumericalScalar b,
                                const NumericalScalar x);

NumericalScalar RegularizedIncompleteBeta(const NumericalScalar a,
    const NumericalScalar b,
    const NumericalScalar x,
    const Bool tail = false);

NumericalScalar RegularizedIncompleteBetaInverse(const NumericalScalar a,
    const NumericalScalar b,
    const NumericalScalar x,
    const Bool tail = false);

NumericalScalar RegularizedIncompleteBetaP(const NumericalScalar a,
    const NumericalScalar b,
    const NumericalScalar x);

NumericalScalar RegularizedIncompleteBetaQ(const NumericalScalar a,
    const NumericalScalar b,
    const NumericalScalar x);

NumericalScalar RegularizedIncompleteBetaContinuedFraction(const NumericalScalar a,
    const NumericalScalar b,
    const NumericalScalar x);

NumericalScalar BRATIO(const NumericalScalar a,
                       const NumericalScalar b,
                       const NumericalScalar x,
                       const Bool tail);

NumericalScalar BPSER(const NumericalScalar a,
                      const NumericalScalar b,
                      const NumericalScalar x);

NumericalScalar BUP(const NumericalScalar a,
                    const NumericalScalar b,
                    const NumericalScalar x,
                    const NumericalScalar y,
                    const UnsignedInteger n);

NumericalScalar ALGDIV(const NumericalScalar a,
                       const NumericalScalar b);

NumericalScalar BGRAT(const NumericalScalar a,
                      const NumericalScalar b,
                      const NumericalScalar x,
                      const NumericalScalar y,
                      const NumericalScalar w);

} // BetaFunctions

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_BETAFUNCTIONS_HXX */
