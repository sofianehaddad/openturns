#! /usr/bin/env python

from openturns import *
from openturns.testing import *

TESTPREAMBLE()

sampleSize = 6
dimension = 1

f = NumericalMathFunction(['x0'], ['f0'], ['x0 * sin(x0)'])

X = NumericalSample(sampleSize, dimension)
X2 = NumericalSample(sampleSize, dimension)
for i in range(sampleSize):
    X[i, 0] = 3.0 + i
    X2[i, 0] = 2.5 + i
X[0, 0] = 1.0
X[1, 0] = 3.0
X2[0, 0] = 2.0
X2[1, 0] = 4.0
Y = NumericalSample(f(X))
Y2 = NumericalSample(f(X2))

# create algorithm
basis = ConstantBasisFactory(dimension).build()
covarianceModel = SquaredExponential(dimension)

algo = KrigingAlgorithm(X, Y, basis, covarianceModel)

algo.run()

# perform an evaluation
result = algo.getResult()
print "X=", X
print "f(X)=", Y

assert_almost_equal(result.getMetaModel()(X), Y)
assert_almost_equal(result.getResiduals(), [1.32804e-10])
assert_almost_equal(result.getRelativeErrors(), [5.20873e-21])
