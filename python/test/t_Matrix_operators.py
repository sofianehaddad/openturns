#! /usr/bin/env python

from openturns import *

ref = Matrix([[1.0, 0.0], [0.0, 0.5]])

print SquareMatrix(ref)
print TriangularMatrix(ref)
print SymmetricMatrix(ref)
print CovarianceMatrix(ref)
print CorrelationMatrix(ref)
print HermitianMatrix(ref)
print TriangularComplexMatrix(ref)
print SquareComplexMatrix(ref)

#A = ot.Matrix([[1., 2.], [3., 4.], [5., 6.]])
#B = ot.SquareMatrix(np.eye(2))
#print(A * B)
