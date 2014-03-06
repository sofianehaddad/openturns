//                                               -*- C++ -*-
/**
 *  @file  t_MeixnerDistributionFactory_std.cxx
 *  @brief The test file of class MeixnerDistribution for standard methods
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
    MeixnerDistribution distribution(1.5, 0.5, 2.5, -0.5);
    UnsignedLong size(10000);
    NumericalSample sample(distribution.getSample(size));
    MeixnerDistributionFactory factory;
    CovarianceMatrix covariance;
    // Distribution estimatedDistribution(factory.build(sample, covariance));
    Distribution estimatedDistribution(factory.build(sample));
    fullprint << "Distribution          =" << distribution << std::endl;
    fullprint << "Estimated distribution=" << estimatedDistribution << std::endl;
    // fullprint << "Covariance=" << covariance << std::endl;
    estimatedDistribution = factory.build();
    fullprint << "Default distribution=" << estimatedDistribution << std::endl;
    estimatedDistribution = factory.build(distribution.getParametersCollection());
    fullprint << "Distribution from parameters=" << estimatedDistribution << std::endl;
    MeixnerDistribution estimatedMeixnerDistribution(factory.buildAsMeixnerDistribution(sample));
    fullprint << "MeixnerDistribution          =" << distribution << std::endl;
    fullprint << "Estimated meixnerDistribution=" << estimatedMeixnerDistribution << std::endl;
    estimatedMeixnerDistribution = factory.buildAsMeixnerDistribution();
    fullprint << "Default meixnerDistribution=" << estimatedMeixnerDistribution << std::endl;
    estimatedMeixnerDistribution = factory.buildAsMeixnerDistribution(distribution.getParametersCollection());
    fullprint << "MeixnerDistribution from parameters=" << estimatedMeixnerDistribution << std::endl;
  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }


  return ExitCode::Success;
}
