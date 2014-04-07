//                                               -*- C++ -*-
/**
 *  @file  Tvpack.hxx
 *  @brief This file includes all of the tvpack functions used in the platform
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
#ifndef OPENTURNS_TVPACK_HXX
#define OPENTURNS_TVPACK_HXX

#include "OTprivate.hxx"
#include "OTconfig.hxx"

BEGIN_C_DECLS

/** Function that evaluates 2-dimensional normal and student CDF. Nu=0 for normal CDF */
double bvtl_(int *nu, double *dh, double *dk, double *r);

/** Function that evaluates 3-dimensional normal and student CDF. Nu=0 for normal CDF */
double tvtl_(int *nu, double *h, double *r, double *epsi);

END_C_DECLS

#endif /* OPENTURNS_TVPACK_HXX */
