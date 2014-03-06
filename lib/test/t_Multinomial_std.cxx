//                                               -*- C++ -*-
/**
 *  @file  t_Multinomial_std.cxx
 *  @brief The test file of class Multinomial for standard methods
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

class TestObject : public Multinomial
{
public:
  explicit TestObject() : Multinomial(5, NumericalPoint(3, 0.25)) {}
  explicit TestObject(const String & name) : Multinomial(5, NumericalPoint(3, 0.25))
  {
    setName(name);
  }
  virtual ~TestObject() {}
};


int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);
  setRandomGenerator();
  try
  {
    // Test basic functionnalities
    checkClassWithClassName<TestObject>();

    // Test some extra functionnalities
    checkNameFeature<TestObject>();

    // Instanciate one distribution object
    Multinomial distribution(5, NumericalPoint(3, 0.25));
    fullprint << "Distribution " << distribution << std::endl;
    std::cout << "Distribution " << distribution << std::endl;

    // Is this distribution elliptical ?
    fullprint << "Elliptical = " << (distribution.isElliptical() ? "true" : "false") << std::endl;

    // Is this distribution continuous ?
    fullprint << "Continuous = " << (distribution.isContinuous() ? "true" : "false") << std::endl;

    // Test for realization of distribution
    NumericalPoint oneRealization = distribution.getRealization();
    fullprint << "oneRealization=" << oneRealization << std::endl;

    // Test for sampling
    UnsignedLong size = 10000;
    NumericalSample oneSample = distribution.getSample( size );
    fullprint << "oneSample first=" << oneSample[0] << " last=" << oneSample[size - 1] << std::endl;
    fullprint << "mean=" << oneSample.computeMean() << std::endl;
    fullprint << "covariance=" << oneSample.computeCovariance() << std::endl;
    // Support
    NumericalSample support(distribution.getSupport());
    fullprint << "support=" << support << std::endl;
    Interval interval(NumericalPoint(distribution.getDimension(), 1.0), NumericalPoint(distribution.getDimension(), 3.0));
    NumericalSample restrictedSupport(distribution.getSupport(interval));
    fullprint << "support restricted to the interval=" << interval << " gives=" << restrictedSupport << std::endl;
    // Define a point
    NumericalPoint point( distribution.getDimension(), 1.0 );
    fullprint << "Point= " << point << std::endl;

    // Show PDF and CDF of point
    NumericalScalar LPDF = distribution.computeLogPDF( point );
    fullprint << "log pdf=" << LPDF << std::endl;
    NumericalScalar PDF = distribution.computePDF( point );
    fullprint << "pdf     =" << PDF << std::endl;
    NumericalScalar CDF = distribution.computeCDF( point );
    fullprint << "cdf=" << std::setprecision(5) << CDF << std::setprecision(6) << std::endl;
    NumericalScalar CCDF = distribution.computeComplementaryCDF( point );
    fullprint << "ccdf=" << std::setprecision(5) << CCDF << std::setprecision(6) << std::endl;
    //    NumericalScalar Survival = distribution.computeSurvivalFunction( point );
    //    fullprint << "survival=" << Survival << std::endl;
    NumericalPoint quantile = distribution.computeQuantile( 0.95 );
    fullprint << "quantile=" << quantile << std::endl;
    fullprint << "cdf(quantile)=" << distribution.computeCDF(quantile) << std::endl;
    NumericalPoint mean = distribution.getMean();
    fullprint << "mean=" << mean << std::endl;
    CovarianceMatrix covariance = distribution.getCovariance();
    fullprint << "covariance=" << covariance << std::endl;
    CovarianceMatrix correlation = distribution.getCorrelation();
    fullprint << "correlation=" << correlation << std::endl;
    //    CovarianceMatrix spearman = distribution.getSpearmanCorrelation();
    //    fullprint << "spearman=" << spearman << std::endl;
    //    CovarianceMatrix kendall = distribution.getKendallTau();
    //    fullprint << "kendall=" << kendall << std::endl;
    Multinomial::NumericalPointWithDescriptionCollection parameters = distribution.getParametersCollection();
    fullprint << "parameters=" << parameters << std::endl;
  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }


  return ExitCode::Success;
}
