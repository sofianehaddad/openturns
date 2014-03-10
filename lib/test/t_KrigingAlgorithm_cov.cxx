//                                               -*- C++ -*-
/**
 *  @file  t_SquaredExponential_std.cxx
 *  @brief The test file of class SquaredExponential
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
 *  @author: $LastChangedBy$
 *  @date:   $LastChangedDate$
 *  Id:      $Id$
 */
#include <iostream>
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;


void test_model(const CovarianceModel & myModel)
{
  OStream fullprint(std::cout);

  fullprint << "myModel = " << myModel << std::endl;

  const UnsignedInteger dimension = myModel.getDimension();

  NumericalPoint x1(dimension);
  NumericalPoint x2(dimension);
  for (UnsignedInteger j = 0; j < dimension; ++ j)
  {
    x1[j] = 8.0 * (0.5 - j);
    x2[j] = -1.0 * (3.0 - 2.0 * j);
  }
  fullprint << "myModel(" << x1 << ", " << x2 << ")=" << myModel(x1, x2) << std::endl;


  SymmetricTensor grad(myModel.partialGradient(x1, x2));
  fullprint << "dCov =" << grad << std::endl;

  NumericalScalar eps = 1e-5;
  NumericalPoint gradfd(dimension);
  for (UnsignedInteger j = 0; j < dimension; ++ j)
  {
    NumericalPoint x1_d(x1);
    x1_d[j] += eps;
    gradfd[j] = (myModel(x1_d, x2)(0, 0) - myModel(x1, x2)(0, 0)) / eps;
  }
  fullprint << "dCov (FD)=" << gradfd << std::endl;
}


int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
  {
    // Default dimension parameter to evaluate the model
    const UnsignedInteger dimension = 2;


    {
      /* Default constructor */
      SquaredExponential myDefautModel;
      fullprint << "myDefautModel = " << myDefautModel << std::endl;


      SquaredExponential myModel(dimension);
      test_model(myModel);

    }
    {
      /* Default constructor */
      GeneralizedExponential myDefautModel;
      fullprint << "myDefautModel = " << myDefautModel << std::endl;

      GeneralizedExponential myModel(dimension, 0.1, 1.5);
      test_model(myModel);
    }
    {
      /* Default constructor */
      AbsoluteExponential myDefautModel;
      fullprint << "myDefautModel = " << myDefautModel << std::endl;

      AbsoluteExponential myModel(dimension);
      test_model(myModel);

    }






  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }


  return ExitCode::Success;
}
