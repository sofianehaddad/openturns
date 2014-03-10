//                                               -*- C++ -*-
/**
 *  @file  t_InverseNatafIndependentCopulaHessian_std.cxx
 *  @brief The test file of class InverseNatafIndependentCopulaHessian for standard methods
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
 *  @author dutka
 *  @date   2009-05-28 14:47:53 +0200 (jeu. 28 mai 2009)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);
  setRandomGenerator();

  try
  {
    UnsignedInteger dim(2);
    InverseNatafIndependentCopulaHessian transformation(dim);
    fullprint << "transformation=" << transformation << std::endl;
    NumericalPoint point(dim, 0.75);
    fullprint << "transformation(" << point << ")=" << transformation.hessian(point) << std::endl;
    fullprint << "input dimension=" << transformation.getInputDimension() << std::endl;
    fullprint << "output dimension=" << transformation.getOutputDimension() << std::endl;
  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }


  return ExitCode::Success;
}
