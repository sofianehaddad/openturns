//                                               -*- C++ -*-
/**
 *  @file  t_DirichletFactory_std.cxx
 *  @brief The test file of class Dirichlet for standard methods
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
 *  @author lebrun
 *  @date   2011-01-12 22:41:37 +0100 (mer. 12 janv. 2011)
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
    NumericalPoint theta(0);
    theta.add(0.2);
    theta.add(0.3);
    theta.add(0.4);
    theta.add(0.5);
    Dirichlet distribution(theta);
    UnsignedInteger size(10000);
    NumericalSample sample(distribution.getSample(size));
    DirichletFactory factory;
    CovarianceMatrix covariance;
    // Distribution estimatedDistribution(factory.build(sample, covariance));
    Distribution estimatedDistribution(factory.build(sample));
    fullprint << "Distribution          =" << distribution << std::endl;
    fullprint << "Estimated distribution=" << estimatedDistribution << std::endl;
    estimatedDistribution = factory.build();
    fullprint << "Default distribution=" << estimatedDistribution << std::endl;
    Dirichlet estimatedDirichlet(factory.buildAsDirichlet(sample));
    fullprint << "Dirichlet          =" << distribution << std::endl;
    fullprint << "Estimated dirichlet=" << estimatedDirichlet << std::endl;
    estimatedDirichlet = factory.buildAsDirichlet();
    fullprint << "Default dirichlet=" << estimatedDirichlet << std::endl;
  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }


  return ExitCode::Success;
}
