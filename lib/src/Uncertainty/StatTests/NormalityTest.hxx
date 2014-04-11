//                                               -*- C++ -*-
/**
 *  @file  NormalityTest.hxx
 *  @brief StatTest implements statistical tests
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#ifndef OPENTURNS_NORMALITYTEST_HXX
#define OPENTURNS_NORMALITYTEST_HXX

#include "OTprivate.hxx"
#include "TestResult.hxx"
#include "NumericalSample.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class NormalityTest
 *
 */

class OT_API NormalityTest
{
public:

  /** Anderson Darling normality test for normal distributions. */
  static TestResult AndersonDarlingNormal(const NumericalSample & sample,
                                          const NumericalScalar level = 0.95);

  /** Cramer Von Mises normality test for normal distributions. */
  static TestResult CramerVonMisesNormal(const NumericalSample & sample,
                                         const NumericalScalar level = 0.95);

private:
  NormalityTest();
}; /* class NormalityTest */

END_NAMESPACE_OPENTURNS
#endif /* OPENTURNS_NORMALITYTEST_HXX */
