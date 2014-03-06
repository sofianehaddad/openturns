//                                               -*- C++ -*-
/**
 * @file  t_PiecewiseHermiteEvaluationImplementation_std.cxx
 * @brief The test file of class PiecewiseHermiteEvaluationImplementation for standard methods
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
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
  {
    NumericalMathFunction ref("x", "sin(x)");
    UnsignedLong size(12);
    NumericalPoint locations(size);
    NumericalPoint values(size);
    NumericalPoint derivatives(size);
    // Build locations/values/derivatives with non-increasing locations
    for (UnsignedLong i = 0; i < size; ++i)
    {
      locations[i] = 10.0 * i * i / (size - 1.0) / (size - 1.0);
      values[i] = ref(NumericalPoint(1, locations[i]))[0];
      derivatives[i] = ref.gradient(NumericalPoint(1, locations[i]))(0, 0);
    }
    PiecewiseHermiteEvaluationImplementation evaluation(locations, values, derivatives);
    fullprint << "evaluation=" << evaluation << std::endl;
    // Check the values
    for (UnsignedLong i = 0; i < 2 * size; ++i)
    {
      NumericalPoint x(1, -1.0 + 12.0 * i / (2.0 * size - 1.0));
      fullprint << "f(" << x[0] << ")=" << evaluation(x) << ", ref=" << ref(x) << std::endl;
    }
  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }

  return ExitCode::Success;
}
