//                                               -*- C++ -*-
/**
 *  @file  t_LinearNumericalMathFunction_std.cxx
 *  @brief The test file of class LinearNumericalMathFunctionImplementation for standard methods
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
    UnsignedInteger inputDimension(3);
    UnsignedInteger outputDimension(2);
    // Center
    NumericalPoint center(inputDimension);
    center[0] = -1;
    center[1] = 0.5;
    center[2] = 1;
    // Constant term
    NumericalPoint constant(outputDimension);
    constant[0] = -1.0;
    constant[1] =  2.0;
    // Linear term
    Matrix linear(outputDimension, inputDimension);
    linear(0, 0) = 1.0;
    linear(1, 0) = 2.0;
    linear(0, 1) = 3.0;
    linear(1, 1) = 4.0;
    linear(0, 2) = 5.0;
    linear(1, 2) = 6.0;

    LinearNumericalMathFunction myFunction(center, constant, linear);
    myFunction.setName("linearFunction");
    NumericalPoint inPoint(inputDimension);
    inPoint[0] = 7.0;
    inPoint[1] = 8.0;
    inPoint[2] = 9.0;
    fullprint << "myFunction=" << myFunction << std::endl;
    fullprint << myFunction.getName() << "( " << inPoint << " ) = " <<  myFunction(inPoint) << std::endl;
    fullprint << myFunction.getName() << ".gradient( " << inPoint << " ) = " << myFunction.gradient(inPoint) << std::endl;
    fullprint << myFunction.getName() << ".hessian( " << inPoint << " ) = " << myFunction.hessian(inPoint) << std::endl;
  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }

  return ExitCode::Success;
}
