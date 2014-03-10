//                                               -*- C++ -*-
/**
 *  @file  t_MeixnerFactory_std.cxx
 *  @brief The test file of class MeixnerFactory for standard methods
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
 *  @date   2008-05-21 17:44:02 +0200 (mer, 21 mai 2008)
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
    MeixnerFactory meixner(10, 0.3);
    fullprint << "meixner=" << meixner << std::endl;
    for (UnsignedInteger i = 0; i < 10; ++i)
    {
      fullprint << "meixner(" << i << ")=" << meixner.build(i).__str__() << std::endl;
    }
    NumericalPoint roots(meixner.getRoots(10));
    fullprint << "meixner(10) roots=" << roots << std::endl;
    NumericalPoint weights;
    roots = meixner.getNodesAndWeights(10, weights);
    fullprint << "meixner(10) roots=" << roots << " and weights=" << weights << std::endl;
  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }

  return ExitCode::Success;
}
