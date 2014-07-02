//                                               -*- C++ -*-
/**
 *  @file  t_IntervalMesher_std.cxx
 *  @brief The test file of class IntervalMesher for standard methods
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
    // The 1D mesher
    IntervalMesher mesher1D(Indices(1, 5));
    fullprint << "mesher1D=" << mesher1D << std::endl;
    Mesh mesh1D(mesher1D.build(Interval(-1.0, 2.0)));
    fullprint << "mesh1D=" << mesh1D << std::endl;
    IntervalMesher mesher2D(Indices(2, 5));
    fullprint << "mesher2D=" << mesher2D << std::endl;
    Mesh mesh2D(mesher2D.build(Interval(NumericalPoint(2, -1.0), NumericalPoint(2, 2.0))));
    fullprint << "mesh2D=" << mesh2D << std::endl;
  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }

  return ExitCode::Success;
}
