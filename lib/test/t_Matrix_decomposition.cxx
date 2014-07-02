//                                               -*- C++ -*-
/**
 *  @file  t_MatrixSingular_decomposition.cxx
 *  @brief The test file for the singularValues method of the Matrix class
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
 *  @date   2011-05-09 15:48:17 +0200 (lun. 09 mai 2011)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;


Matrix quadM(UnsignedInteger m, UnsignedInteger n)
{
  Matrix res(m, n);
  for (UnsignedInteger i = 0; i < m; ++i)
    for (UnsignedInteger j = 0; j < n; ++j)
      res(i, j) = pow(i + 1.0, j + 1.0);
  return res;
}

void testSVD(UnsignedInteger m, UnsignedInteger n)
{
  OStream fullprint(std::cout);

  // Square case
  Matrix matrix1(quadM(m, n));
  matrix1.setName("matrix1");
  fullprint << "matrix1= " << matrix1.__str__() << std::endl;

  NumericalPoint result1 ;
  result1 = matrix1.computeSingularValues();
  fullprint << "svd (svd only)= " << result1 << std::endl;

  Matrix u;
  Matrix vT;
//   Matrix S;

  result1 = matrix1.computeSVD(u, vT, true);
  fullprint << "svd (svd + u, vT full)= " << result1 << std::endl;

//   S = Matrix(m, n);
//   for (UnsignedInteger i = 0; i < std::min(m,n); ++i)
//     S(i,i) = result1[i];
//   fullprint << "u*vT= " << ((u*S)*vT).__str__() << std::endl;

  result1 = matrix1.computeSVD(u, vT, false);
  fullprint << "svd (svd + u, vT small)= " << result1 << std::endl << "u= " << u.__str__() << std::endl << "vT= " << vT.__str__() << std::endl;

//   S = Matrix(std::min(m,n), std::min(m,n));
//   for (UnsignedInteger i = 0; i < std::min(m,n); ++i)
//     S(i,i) = result1[i];
//   fullprint << "u*vT= " << ((u*S)*vT).__str__() << std::endl;

  fullprint << std::endl;

}


void testQR(UnsignedInteger m, UnsignedInteger n)
{
  OStream fullprint(std::cout);

  Matrix matrix1(quadM(m, n));
  Matrix R;
  Matrix Q(matrix1.computeQR(R));
  fullprint << "Q= " << Q.__str__() << std::endl;
  fullprint << "R=" << R.__str__() << std::endl;
//   fullprint << "QR="<<(Q*R).__str__()<< std::endl;
}


int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  fullprint << "-- SVD square case ------------------------------------------" << std::endl;
  testSVD(3, 3);

  fullprint << "-- SVD rectangular case, m < n ------------------------------" << std::endl;
  testSVD(3, 5);

  fullprint << "-- SVD rectangular case, m > n ------------------------------" << std::endl;
  testSVD(5, 3);

  fullprint << "-- QR square case ------------------------------------------" << std::endl;
  testQR(3, 3);

  return ExitCode::Success;
}
