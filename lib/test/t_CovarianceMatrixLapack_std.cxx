//                                               -*- C++ -*-
/**
 *  @file  t_CovarianceMatrixLapack_std.cxx
 *  @brief The test file for the Lapack based methods of the CovarianceMatrix class
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  CovarianceMatrix matrix1(2);
  matrix1.setName("matrix1");
  fullprint << "matrix1 (default)=" << matrix1 << std::endl;
  matrix1(0, 0) = 1.0;
  matrix1(1, 0) = 0.5;
  matrix1(1, 1) = 1.0;
  fullprint << "matrix1 (initialized)=" << matrix1 << std::endl;

  NumericalPoint pt;
  pt.add(5.0);
  pt.add(0.0);
  fullprint << "pt=" << pt << std::endl;

  NumericalPoint result;
  result = matrix1.solveLinearSystem(pt);
  fullprint << "result=" << result << std::endl;

  Matrix b(2, 3);
  b(0, 0) =  5.0;
  b(1, 0) =  0.0;
  b(0, 1) = 10.0;
  b(1, 1) =  1.0;
  b(0, 2) = 15.0;
  b(1, 2) =  2.0;
  Matrix result2 ;
  result2 = matrix1.solveLinearSystem(b);
  fullprint << "result2=" << result2 << std::endl;

  NumericalScalar determinant ;
  determinant = matrix1.computeDeterminant();
  fullprint << "determinant=" << determinant << std::endl;

  Collection<NumericalScalar> ev(2);
  ev = matrix1.computeEigenValues().getCollection();
  fullprint << "ev=" << ev << std::endl;

  bool isSPD = matrix1.isPositiveDefinite();
  fullprint << "isSPD=" << isSPD << std::endl;

  SquareMatrix matrix2 = matrix1.computeCholesky();
  fullprint << "matrix2=" << matrix2 << std::endl;

  return ExitCode::Success;
}
