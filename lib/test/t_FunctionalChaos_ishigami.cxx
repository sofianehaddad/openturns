//                                               -*- C++ -*-
/**
 *  @file  t_FunctionalChaos_ishigami.cxx
 *  @brief The test file of FunctionalChaosAlgoritm class
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
 *  @date   2008-05-21 11:21:38 +0200 (mer. 21 mai 2008)
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

    // Problem parameters
    UnsignedInteger dimension(3);
    NumericalScalar a(7.0);
    NumericalScalar b(0.1);
    // Reference analytical values
    NumericalScalar meanTh(a / 2);
    NumericalScalar covTh((pow(b, 2) * pow(M_PI, 8)) / 18.0 + (b * pow(M_PI, 4)) / 5.0 + (pow(a, 2)) / 8.0 + 1.0 / 2.0);
    NumericalPoint sob_1(3);
    sob_1[0] = (b * pow(M_PI, 4) / 5.0 + pow(b, 2) * pow(M_PI, 8) / 50.0 + 1.0 / 2.0) / covTh;
    sob_1[1] = (pow(a, 2) / 8.0) / covTh;
    sob_1[2] = 0.0;
    NumericalPoint sob_2(3);
    sob_2[0] = 0.0;
    sob_2[1] = (pow(b, 2) * pow(M_PI, 8) / 18.0 - pow(b, 2) * pow(M_PI, 8) / 50.0) / covTh;
    sob_2[2] = 0.0;
    NumericalPoint sob_3(1, 0.0);
    NumericalPoint sob_T1(3);
    sob_T1[0] = sob_1[0] + sob_2[0] + sob_2[1] + sob_3[0];
    sob_T1[1] = sob_1[1] + sob_2[0] + sob_2[2] + sob_3[0];
    sob_T1[2] = sob_1[2] + sob_2[1] + sob_2[2] + sob_3[0];
    NumericalPoint sob_T2(3);
    sob_T2[0] = sob_2[0] + sob_3[0];
    sob_T2[1] = sob_2[1] + sob_3[0];
    sob_T2[2] = sob_2[2] + sob_3[0];
    NumericalPoint sob_T3(sob_3);
    // Create the Ishigami function
    Description inputVariables(dimension);
    inputVariables[0] = "xi1";
    inputVariables[1] = "xi2";
    inputVariables[2] = "xi3";
    Description outputVariables(1);
    outputVariables[0] = "y";
    Description formula(1);
    formula[0] = (OSS() << "sin(xi1) + (" << a << ") * (sin(xi2)) ^ 2 + (" << b << ") * xi3^4 * sin(xi1)");
    NumericalMathFunction model(inputVariables, outputVariables, formula);

    // Create the input distribution
    Collection<Distribution> marginals(dimension);
    marginals[0] = Uniform(-M_PI, M_PI);
    marginals[1] = Uniform(-M_PI, M_PI);
    marginals[2] = Uniform(-M_PI, M_PI);
    ComposedDistribution distribution(marginals);

    // Create the orthogonal basis
    Collection<OrthogonalUniVariatePolynomialFamily> polynomialCollection(dimension);
    polynomialCollection[0] = LegendreFactory();
    polynomialCollection[1] = LegendreFactory();
    polynomialCollection[2] = LegendreFactory();

    EnumerateFunction enumerateFunction(dimension);
    OrthogonalProductPolynomialFactory productBasis(polynomialCollection, enumerateFunction);

    // Create the adaptive strategy
    // We can choose amongst several strategies
    // First, the most efficient (but more complex!) strategy
    Collection<AdaptiveStrategy> listAdaptiveStrategy(0);
    UnsignedInteger degree(6);
    UnsignedInteger indexMax(enumerateFunction.getStrataCumulatedCardinal(degree));
    UnsignedInteger basisDimension(enumerateFunction.getStrataCumulatedCardinal(degree / 2));
    NumericalScalar threshold(1.0e-6);
    listAdaptiveStrategy.add(CleaningStrategy(productBasis, indexMax, basisDimension, threshold, false));
    // Second, the most used (and most basic!) strategy
    listAdaptiveStrategy.add(FixedStrategy(productBasis, enumerateFunction.getStrataCumulatedCardinal(degree)));
    // Third, a slight enhancement with respect to the basic strategy
    listAdaptiveStrategy.add(SequentialStrategy(productBasis, enumerateFunction.getStrataCumulatedCardinal(degree / 2), false));
    // Create the result object here in order to test the save/load mechanism outside of the double loop
    FunctionalChaosResult result;

    for(UnsignedInteger adaptiveStrategyIndex = 0; adaptiveStrategyIndex < listAdaptiveStrategy.getSize(); ++adaptiveStrategyIndex)
    {
      AdaptiveStrategy adaptiveStrategy(listAdaptiveStrategy[adaptiveStrategyIndex]);
      // Create the projection strategy
      UnsignedInteger samplingSize(250);
      Collection<ProjectionStrategy> listProjectionStrategy(0);
      // Monte Carlo sampling
      listProjectionStrategy.add(LeastSquaresStrategy(MonteCarloExperiment(samplingSize)));
      // LHS sampling
      listProjectionStrategy.add(LeastSquaresStrategy(LHSExperiment(samplingSize)));
      // Low Discrepancy sequence
      listProjectionStrategy.add(LeastSquaresStrategy(LowDiscrepancyExperiment(LowDiscrepancySequence(SobolSequence()), samplingSize)));
      // The integration strategy
      // Monte Carlo sampling
      listProjectionStrategy.add(IntegrationStrategy(MonteCarloExperiment(samplingSize)));
      // LHS sampling
      listProjectionStrategy.add(IntegrationStrategy(LHSExperiment(samplingSize)));
      // Low Discrepancy sequence
      listProjectionStrategy.add(IntegrationStrategy(LowDiscrepancyExperiment(LowDiscrepancySequence(SobolSequence()), samplingSize)));
      for(UnsignedInteger projectionStrategyIndex = 0; projectionStrategyIndex < listProjectionStrategy.getSize(); ++projectionStrategyIndex)
      {
        ProjectionStrategy projectionStrategy(listProjectionStrategy[projectionStrategyIndex]);
        // Create the polynomial chaos algorithm
        NumericalScalar maximumResidual(1.0e-10);
        FunctionalChaosAlgorithm algo(model, distribution, adaptiveStrategy, projectionStrategy);
        algo.setMaximumResidual(maximumResidual);
        // Reinitialize the RandomGenerator to see the effect of the sampling method only
        RandomGenerator::SetSeed(0);
        algo.run();

        // Examine the results
        result = algo.getResult();
        fullprint << "//////////////////////////////////////////////////////////////////////" << std::endl;
        fullprint << adaptiveStrategy << std::endl;
        fullprint << projectionStrategy << std::endl;
        NumericalPoint residuals(result.getResiduals());
        fullprint << "residuals=" << std::fixed << std::setprecision(5) << residuals << std::endl;
        NumericalPoint relativeErrors(result.getRelativeErrors());
        fullprint << "relative errors=" << std::fixed << std::setprecision(5) << relativeErrors << std::endl;

        // Post-process the results
        FunctionalChaosRandomVector vector(result);
        NumericalScalar mean(vector.getMean()[0]);
        fullprint << "mean=" << std::fixed << std::setprecision(5) << mean << " absolute error=" << std::scientific << std::setprecision(1) << fabs(mean - meanTh) << std::endl;
        NumericalScalar variance(vector.getCovariance()(0, 0));
        fullprint << "variance=" << std::fixed << std::setprecision(5) << variance << " absolute error=" << std::scientific << std::setprecision(1) << fabs(variance - covTh) << std::endl;
        for(UnsignedInteger i = 0; i < dimension; ++i)
        {
          NumericalScalar value(vector.getSobolIndex(i));
          fullprint << "Sobol index " << i << " = " << std::fixed << std::setprecision(5) << value << " absolute error=" << std::scientific << std::setprecision(1) << fabs(value - sob_1[i]) << std::endl;
        }
        Indices indices(2);
        UnsignedInteger k(0);
        for (UnsignedInteger i = 0; i < dimension; ++i)
        {
          indices[0] = i;
          for (UnsignedInteger j = i + 1; j < dimension; ++j)
          {
            indices[1] = j;
            NumericalScalar value(vector.getSobolIndex(indices));
            fullprint << "Sobol index " << indices << " =" << std::fixed << std::setprecision(5) << value << " absolute error=" << std::scientific << std::setprecision(1) << fabs(value - sob_2[k]) << std::endl;
            k = k + 1;
          }
        }
        indices = Indices(3);
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        NumericalScalar value(vector.getSobolIndex(indices));
        fullprint << "Sobol index " << indices << " =" << std::fixed << std::setprecision(5) << value << " absolute error=" << std::scientific << std::setprecision(1) << fabs(value - sob_3[0]) << std::endl;
        for (UnsignedInteger i = 0; i < dimension; ++i)
        {
          NumericalScalar value(vector.getSobolTotalIndex(i));
          fullprint << "Sobol total index " << i << " =" << std::fixed << std::setprecision(5) << value << " absolute error=" << std::scientific << std::setprecision(1) << fabs(value - sob_T1[i]) << std::endl;
        }
        indices = Indices(2);
        k = 0;
        for (UnsignedInteger i = 0; i < dimension; ++i)
        {
          indices[0] = i;
          for (UnsignedInteger j = i + 1; j < dimension; ++j)
          {
            indices[1] = j;
            NumericalScalar value(vector.getSobolTotalIndex(indices));
            fullprint << "Sobol total index " << indices << " =" << std::fixed << std::setprecision(5) << value << " absolute error=" << std::scientific << std::setprecision(1) << fabs(value - sob_T2[k]) << std::endl;
            k = k + 1;
          }
        }
        indices = Indices(3);
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        value = vector.getSobolTotalIndex(indices);
        fullprint << "Sobol total index " << indices << " =" << std::fixed << std::setprecision(5) << value << " absolute error=" << std::scientific << std::setprecision(1) << fabs(value - sob_T3[0]) << std::endl;

      }
    }
  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }


  return ExitCode::Success;
}
