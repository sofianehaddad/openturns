//                                               -*- C++ -*-
/**
 *  @file  t_SpatialFunction_std.cxx
 *  @brief The test file of class SpatialFunction for standard methods
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
 *  @date   2011-05-24 19:30:41 +0200 (mar. 24 mai 2011)
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

    /* Create an intance */
    NumericalMathFunction myFunc("x", "x^2");
    SpatialFunction mySpatialFunc(myFunc);

    fullprint << "mySpatialFunc=" << mySpatialFunc << std::endl;
    /* Get the input and output description */
    fullprint << "mySpatialFunc input description=" << mySpatialFunc.getInputDescription() << std::endl;
    fullprint << "mySpatialFunc output description=" << mySpatialFunc.getOutputDescription() << std::endl;
    /* Get the input and output dimension, based on description */
    fullprint << "mySpatialFunc input dimension=" << mySpatialFunc.getInputDimension() << std::endl;
    fullprint << "mySpatialFunc output dimension=" << mySpatialFunc.getOutputDimension() << std::endl;
    /* Create a TimeSeries */
    RegularGrid tg(0.0, 0.2, 6);
    NumericalSample data(tg.getN(), myFunc.getInputDimension());
    for (UnsignedLong i = 0; i < data.getSize(); ++i)
      for (UnsignedLong j = 0; j < data.getDimension(); ++j)
        data(i, j) = i * data.getDimension() + j;
    TimeSeries ts(tg, data);
    fullprint << "input time series=" << ts << std::endl;
    fullprint << "output time series=" << mySpatialFunc(ts) << std::endl;
    /* Get the number of calls */
    fullprint << "called " << mySpatialFunc.getCallsNumber() << " times" << std::endl;
  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }


  return ExitCode::Success;
}
