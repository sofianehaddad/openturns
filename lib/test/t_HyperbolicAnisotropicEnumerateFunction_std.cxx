//                                               -*- C++ -*-
/**
 *  @brief The test file of class HyperbolicAnisotropicEnumerateFunction for standard methods
 *
 *  Copyright 2005-2025 Airbus-EDF-IMACS-ONERA-Phimeca
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
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "openturns/OT.hxx"
#include "openturns/OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

int main(int, char *[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
  {

    fullprint << "Default q : " << ResourceMap::GetAsScalar("HyperbolicAnisotropicEnumerateFunction-DefaultQ") << std::endl << std::endl;

    // first verify consistency with LinearEnumerateFunction
    UnsignedInteger size = 10;
    UnsignedInteger stratas = 5;
    for (UnsignedInteger dimension = 1; dimension < 4; ++ dimension)
    {
      HyperbolicAnisotropicEnumerateFunction f( dimension, 1.0 );
      LinearEnumerateFunction g(dimension);
      fullprint << "First " << size << " values for dimension " << dimension << std::endl;
      for (UnsignedInteger index = 0; index < size; ++index)
      {
        if (f(index) != g(index))
          throw InternalException(HERE) << "Results are different";
        fullprint << "index=" << index << " " << f(index) << std::endl;
      }
      Indices strataCardinal;
      for ( UnsignedInteger index = 0; index < stratas; ++ index )
      {
        strataCardinal.add( f.getStrataCardinal(index) );
      }
      fullprint << "And first " << stratas << " strata cardinals :" << strataCardinal << std::endl << std::endl;
    }

    // values with varying q
    size = 25;
    Point qValues;
    qValues.add(0.75);
    qValues.add(0.5);
    qValues.add(0.25);

    for (UnsignedInteger dimension = 2; dimension < 5; ++ dimension)
    {
      for (UnsignedInteger j = 0; j < qValues.getDimension(); ++ j)
      {
        Scalar q = qValues[j];
        fullprint << "First " << size << " values dimension=" << dimension << " q=" << q << std::endl;
        HyperbolicAnisotropicEnumerateFunction f( dimension,  q);
        for (UnsignedInteger index = 0; index < size; ++index)
        {
          fullprint << "index=" << index << " " << f(index) << std::endl;
        }
        Indices strataCardinal;
        for ( UnsignedInteger index = 0; index < stratas; ++ index )
        {
          strataCardinal.add( f.getStrataCardinal(index) );
        }
        fullprint << "And first " << stratas << " strata cardinals :" << strataCardinal << std::endl << std::endl;
      }
    }

    // Test getMarginal()
    fullprint << "Test getMarginal()" << std::endl;
    HyperbolicAnisotropicEnumerateFunction enumerateFunction(10, 0.5);
    Indices indices({0, 2, 4, 6, 9});
    EnumerateFunction marginalEnumerate(enumerateFunction.getMarginal(indices));
    assert_equal(marginalEnumerate.getDimension(), indices.getSize());
    for (UnsignedInteger index = 0; index < size; ++index)
    {
      Indices multiIndex(marginalEnumerate(index));
      fullprint << "index=" << index << ", multi-index=" << multiIndex << std::endl;
    }


  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }

  return ExitCode::Success;
}
