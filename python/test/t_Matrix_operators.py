#! /usr/bin/env python

import openturns as ot

ref = ot.Matrix([[1.0, 0.0], [0.0, 0.5]])

# conversion
print ot.SquareMatrix(ref)
print ot.TriangularMatrix(ref)
print ot.SymmetricMatrix(ref)
print ot.CovarianceMatrix(ref)
print ot.CorrelationMatrix(ref)
print ot.HermitianMatrix(ref)
print ot.TriangularComplexMatrix(ref)
print ot.SquareComplexMatrix(ref)

# multiplication
A = ot.Matrix([[1., 2.], [3., 4.], [5., 6.]])
B = ot.SquareMatrix([[1, 0], [0, 1]])
print(A * B)
