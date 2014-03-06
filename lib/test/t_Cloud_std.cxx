//                                               -*- C++ -*-
/**
 *  @file  t_Cloud_std.cxx
 *  @brief The test file of class Cloud for standard methods
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
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

    // Instanciate one distribution object
    UnsignedLong dim(2);
    NumericalPoint meanPoint(dim, 1.0);
    meanPoint[0] = 0.5;
    meanPoint[1] = -0.5;
    NumericalPoint sigma(dim, 1.0);
    sigma[0] = 2.0;
    sigma[1] = 3.0;
    CorrelationMatrix R(dim);
    for (UnsignedLong i = 1; i < dim; i++)
    {
      R(i, i - 1) = 0.5;
    }


    Normal distribution1(meanPoint, sigma, R);

    // Instanciate another distribution object
    meanPoint[0] = -1.5;
    meanPoint[1] = 0.5;
    sigma[0] = 4.0;
    sigma[1] = 1.0;
    for (UnsignedLong i = 1; i < dim; i++)
    {
      R(i, i - 1) = -0.25;
    }


    Normal distribution2(meanPoint, sigma, R);



    // Test for sampling
    UnsignedLong size = 200;
    NumericalSample sample1(distribution1.getSample( size ));
    NumericalSample sample2(distribution2.getSample( size ));

    // Create an empty graph
    Graph myGraph("Normal sample", "x1", "x2", true, "topright");

    // Create the first cloud
    Cloud myCloud1(sample1, "blue", "fsquare", "First Cloud");

    // Then, draw it
    myGraph.add(myCloud1);
    myGraph.draw("Graph_Cloud_a_OT", 640, 480);

    // Check that the correct files have been generated by computing their checksum
    fullprint << "bitmap=" << myGraph.getBitmap() << std::endl;
    fullprint << "postscript=" << myGraph.getPostscript() << std::endl;

    // Create the second cloud
    Cloud myCloud2(sample2, "red", "dot", "Second Cloud");

    // Add it to the graph and draw everything
    myGraph.add(myCloud2);
    myGraph.draw("Graph_Cloud_b_OT", 640, 480);
    fullprint << "bitmap=" << myGraph.getBitmap() << std::endl;
    fullprint << "postscript=" << myGraph.getPostscript() << std::endl;
  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }


  return ExitCode::Success;
}
