//                                               -*- C++ -*-
/**
 *  @brief The test file of class MarginalDistribution for standard methods
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

    // Test basic functionnalities
    //checkClassWithClassName<TestObject>();

    const UnsignedInteger dimension = 5;
    const Indices indices = {2, 0, 1};
    Collection<Distribution> coll;
    coll.add(Normal(dimension));
    // Here the probabilities don't sum to 1 on purpose
    coll.add(Multinomial(10, Point(dimension, 1.0 / (dimension + 2))));
    for (UnsignedInteger nDistribution = 0; nDistribution < coll.getSize(); ++nDistribution)
    {
      Distribution fullDistribution = coll[nDistribution];
      MarginalDistribution distribution(fullDistribution, indices);
      //distribution.setUsePDF(fullDistribution.isContinuous());
      fullprint << "Distribution " << distribution << std::endl;
      std::cout << "Distribution " << distribution << std::endl;

      // Is this distribution elliptical ?
      fullprint << "Elliptical = " << (distribution.isElliptical() ? "true" : "false") << std::endl;

      // Is this distribution continuous ?
      fullprint << "Continuous = " << (distribution.isContinuous() ? "true" : "false") << std::endl;

      // Is this distribution discrete ?
      fullprint << "Discrete = " << (distribution.isDiscrete() ? "true" : "false") << std::endl;

      // Is this distribution integral ?
      fullprint << "Integral = " << (distribution.isIntegral() ? "true" : "false") << std::endl;

      // Test for realization of distribution
      Point oneRealization = distribution.getRealization();
      fullprint << "oneRealization=" << oneRealization << std::endl;

      // Test for sampling
      UnsignedInteger size = 10000;
      Sample oneSample = distribution.getSample( size );
      fullprint << "oneSample first=" << oneSample[0] << " last=" << oneSample[size - 1] << std::endl;
      fullprint << "mean=" << oneSample.computeMean() << std::endl;
      fullprint << "covariance=" << oneSample.computeCovariance() << std::endl;

      // Define a point
      Point point( distribution.getDimension(), 1.0 );
      fullprint << "Point= " << point << std::endl;

      // Show PDF and CDF of point
      if (distribution.isContinuous())
      {
        Point DDF = distribution.computeDDF( point );
        fullprint << "ddf     =" << DDF << std::endl;
        Scalar LPDF = distribution.computeLogPDF( point );
        fullprint << "log pdf=" << LPDF << std::endl;
      }
      Scalar PDF = distribution.computePDF( point );
      fullprint << "pdf     =" << PDF << std::endl;
      Scalar CDF = distribution.computeCDF( point );
      fullprint << "cdf=" << CDF << std::endl;
      Scalar CCDF = distribution.computeComplementaryCDF( point );
      fullprint << "ccdf=" << CCDF << std::endl;
      Scalar Survival = distribution.computeSurvivalFunction( point );
      fullprint << "survival=" << Survival << std::endl;
      if (distribution.isContinuous())
      {
        Point InverseSurvival = distribution.computeInverseSurvivalFunction(0.95);
        fullprint << "Inverse survival=" << InverseSurvival << std::endl;
        fullprint << "Survival(inverse survival)=" << distribution.computeSurvivalFunction(InverseSurvival) << std::endl;
      }
      Point quantile = distribution.computeQuantile( 0.95 );
      fullprint << "quantile=" << quantile << std::endl;
      fullprint << "cdf(quantile)=" << distribution.computeCDF(quantile) << std::endl;
      Point quantileTail = distribution.computeQuantile( 0.95, true );
      fullprint << "quantile (tail)=" << quantileTail << std::endl;
      Scalar CDFTail = distribution.computeComplementaryCDF( quantileTail );
      fullprint << "cdf (tail)=" << CDFTail << std::endl;
      Point mean = distribution.getMean();
      fullprint << "mean=" << mean << std::endl;
      Point standardDeviation = distribution.getStandardDeviation();
      fullprint << "standard deviation=" << standardDeviation << std::endl;
      Point skewness = distribution.getSkewness();
      fullprint << "skewness=" << skewness << std::endl;
      Point kurtosis = distribution.getKurtosis();
      fullprint << "kurtosis=" << kurtosis << std::endl;
      CovarianceMatrix covariance = distribution.getCovariance();
      fullprint << "covariance=" << covariance << std::endl;
      CorrelationMatrix correlation = distribution.getCorrelation();
      fullprint << "correlation=" << correlation << std::endl;
      if (distribution.isContinuous())
      {
        CovarianceMatrix spearman = distribution.getSpearmanCorrelation();
        fullprint << "spearman=" << spearman << std::endl;
        CovarianceMatrix kendall = distribution.getKendallTau();
        fullprint << "kendall=" << kendall << std::endl;
      }
      fullprint << "Standard representative=" << distribution.getStandardRepresentative().__str__() << std::endl;
    }
  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }


  return ExitCode::Success;
}
