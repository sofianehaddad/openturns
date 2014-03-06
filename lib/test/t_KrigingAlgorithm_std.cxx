//                                               -*- C++ -*-
/**
 *  @file  t_KrigingAlgorithm_std.cxx
 *  @brief The test file of KrigingAlgorithm class
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
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

String printNumericalPoint(const NumericalPoint & point, const UnsignedLong digits)
{
  OSS oss;
  oss << "[";
  NumericalScalar eps(pow(0.1, digits));
  for (UnsignedLong i = 0; i < point.getDimension(); i++)
  {
    oss << std::scientific << std::setprecision(digits) << (i == 0 ? "" : ",") << Bulk<double>((fabs(point[i]) < eps) ? fabs(point[i]) : point[i]);
  }
  oss << "]";
  return oss;
}


int main(int argc, char *argv[])
{
  try
  {

    {
      UnsignedLong sampleSize = 6;
      UnsignedLong dimension = 1;

      // Create the function to estimate
      Description input(dimension);
      input[0] = "x0";
      Description foutput(1);
      foutput[0] = "f0";
      Description formulas(1);
      formulas[0] = "x0 * sin(x0)";
      NumericalMathFunction model(input, foutput, formulas);

      NumericalSample X(sampleSize, dimension);
      NumericalSample X2(sampleSize, dimension);
      for ( UnsignedLong i = 0; i < sampleSize; ++ i )
      {
        X[i][0] = 3.0 + i;
        X2[i][0] = 2.5 + i;
      }
      X[0][0] = 1.0;
      X[1][0] = 3.0;
      X2[0][0] = 2.0;
      X2[1][0] = 4.0;
      NumericalSample Y(model(X));
      NumericalSample Y2(model(X2));

      Basis basis(ConstantBasisFactory(dimension).build());
      SquaredExponential covarianceModel(dimension);
      covarianceModel.setParameters(NumericalPoint(1, 1e-2));
      KrigingAlgorithm algo(X, Y, basis, covarianceModel);

      algo.run();

      // perform an evaluation
      MetaModelResult result(algo.getResult());
      std::cout << "X=" << X << std::endl;
      std::cout << "f(X)=" << Y << std::endl;

      assert_almost_equal(result.getMetaModel()(X), Y, 1e-3);

      NumericalPoint residualRef(1, 5.57410e-06);
      assert_almost_equal(result.getResiduals(), residualRef, 1e-3, 1e-4);

      NumericalPoint relativeErrorRef(1, 9.17605e-12);
      assert_almost_equal(result.getRelativeErrors(), relativeErrorRef, 1e-3, 1e-5);

    }

    {
      UnsignedLong dimension = 2;
      UnsignedLong sampleSize = 8;
      // Create the function to estimate
      Description input(dimension);
      input[0] = "x0";
      input[1] = "x1";
      Description foutput(1);
      foutput[0] = "f0";
      Description formulas(1);
      formulas[0] = "5.-x1-0.5*(x0-0.1)^2";
      NumericalMathFunction model(input, foutput, formulas);
      NumericalSample X(sampleSize, dimension);
      X[0][0] = -4.61611719;
      X[0][1] = -6.00099547;
      X[1][0] = 4.10469096;
      X[1][1] = 5.32782448;
      X[2][0] = 0.;
      X[2][1] = -.5;
      X[3][0] = -6.17289014;
      X[3][1] = -4.6984743;
      X[4][0] = 1.3109306;
      X[4][1] = -6.93271427;
      X[5][0] = -5.03823144;
      X[5][1] = 3.10584743;
      X[6][0] = -2.87600388;
      X[6][1] = 6.74310541;
      X[7][0] = 5.21301203;
      X[7][1] = 4.26386883;
      NumericalSample Y(model(X));

      // create algorithm
      TNCSpecificParameters specific;
      Interval bounds(NumericalPoint(1, 1e-5), NumericalPoint(1, 1.));
      TNC optimizer(specific, NumericalMathFunction(Description(1, "x"), Description(1, "y"), Description(1, "x")), bounds, NumericalPoint(1, 1e-3), TNC::Result::MINIMIZATION);
      optimizer.setMaximumEvaluationsNumber(10000);
      optimizer.setMaximumAbsoluteError(1.0e-10);
      optimizer.setMaximumRelativeError(1.0e-10);
      optimizer.setMaximumObjectiveError(1.0e-10);
      optimizer.setMaximumConstraintError(1.0e-10);

      // create algorithm
      Basis basis(ConstantBasisFactory(dimension).build());
      SquaredExponential covarianceModel(dimension);
      covarianceModel.setParameters(NumericalPoint(1, 1e-2));
      KrigingAlgorithm algo(X, Y, basis, covarianceModel);
      algo.setOptimizer(optimizer);

      algo.run();

      // perform an evaluation
      MetaModelResult result(algo.getResult());
      std::cout << "X=" << X << std::endl;
      std::cout << "f(X)=" << Y << std::endl;

      assert_almost_equal(result.getMetaModel()(X), Y, 1e-3);

      std::cout << "cov=" << algo.getConditionalCovarianceModel() << std::endl;

      NumericalPoint residualRef(1, 2.45758e-15);
      assert_almost_equal(result.getResiduals(), residualRef, 1e-3, 1e-5);

      NumericalPoint relativeErrorRef(1, 6.52442e-31);
      assert_almost_equal(result.getRelativeErrors(), relativeErrorRef, 1e-3, 1e-5);

      std::cout << "df(X0)=" << model.gradient(X[1]) << std::endl;

      NumericalMathFunction metaModel(result.getMetaModel());
      std::cout << "d^f(X0) =" << metaModel.gradient(X[1]) << std::endl;

      metaModel.setGradientImplementation(new CenteredFiniteDifferenceGradient(ResourceMap::GetAsNumericalScalar( "CenteredFiniteDifferenceGradient-DefaultEpsilon" ), metaModel.getEvaluationImplementation()));
      std::cout << "d^f(X0) FD=" << metaModel.gradient(X[1]) << std::endl;

    }

    {
      UnsignedLong inputDimension = 3;
      UnsignedLong outputDimension = 2;

      // Reference analytical values
      NumericalPoint kappa(inputDimension);
      NumericalScalar a(7.0);
      NumericalScalar b(0.1);
      // Create the gSobol function
      Description inputVariables(inputDimension);
      Description outputVariables(outputDimension);
      outputVariables[0] = "y0";
      outputVariables[1] = "y1";
      Description formula(outputDimension);
      formula[0] = "1.0";
      for (UnsignedLong i = 0; i < inputDimension; ++i)
      {
        kappa[i] = 0.5 * i;
        inputVariables[i] = (OSS() << "xi" << i);
        formula[0] = (OSS() << formula[0] << " * ((abs(4.0 * xi" << i << " - 2.0) + " << kappa[i] << ") / (1.0 + " << kappa[i] << "))");
      }
      formula[1] = (OSS() << "sin(" << -M_PI << " + 2 * " << M_PI << " * xi0) + (" << a << ") * (sin(" << -M_PI << " + 2 * " << M_PI << " * xi1)) ^ 2 + (" << b << ") * (" << -M_PI << " + 2 * " << M_PI << " * xi2)^4 * sin(" << -M_PI << " + 2 * " << M_PI << " * xi0)");

      NumericalMathFunction model(inputVariables, outputVariables, formula);

      // Create the input distribution
      Collection<Distribution> marginals(inputDimension);
      for (UnsignedLong i = 0; i < inputDimension; ++i) marginals[i] = Uniform(0.0, 1.0);
      ComposedDistribution distribution(marginals);

      UnsignedLong samplingSize = 250;
      NumericalSample X(distribution.getSample(samplingSize));
      NumericalSample Y(model(X));

      // create algorithm
      Basis basis(ConstantBasisFactory(inputDimension).build());
      SquaredExponential covarianceModel(inputDimension);
      covarianceModel.setParameters(NumericalPoint(1, 0.25));
      KrigingAlgorithm algo(X, distribution, Y, basis, covarianceModel);

      algo.run();

      // perform an evaluation
      MetaModelResult result(algo.getResult());
      std::cout << "X=" << X << std::endl;
      std::cout << "f(X)=" << Y << std::endl;
      assert_almost_equal(result.getMetaModel()(X), Y, 1e-3);

      NumericalPoint residualRef(2);
      residualRef[0] = 3.77033e-08;
      residualRef[1] = 1.97183e-07;
      assert_almost_equal(result.getResiduals(), residualRef, 1e-3, 1e-5);

      NumericalPoint relativeErrorRef(2);
      relativeErrorRef[0] = 5.36712e-13;
      relativeErrorRef[1] = 6.91700e-13;
      assert_almost_equal(result.getRelativeErrors(), relativeErrorRef, 1e-3, 1e-5);
    }




  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }


  return ExitCode::Success;
}
