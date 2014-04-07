//                                               -*- C++ -*-
/**
 *  @file  Mvtdstpack.hxx
 *  @brief This file includes all of the mvtdstpack functions used in the platform
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
 *  @author schueller
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#ifndef OPENTURNS_MVTDSTPACK_HXX
#define OPENTURNS_MVTDSTPACK_HXX

#include "OTconfig.hxx"

BEGIN_C_DECLS

/** Function that evaluates n-dimensional normal CDF */
void mvtdst_(int *n, int *nu, double *lower, double *upper, int *infin, double *correl, double *delta, int *maxpts, double *abseps, double *releps, double *error, double *value, int *inform);

END_C_DECLS

#endif /* OPENTURNS_MVTDSTPACK_HXX */
