#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()


def test_model(myModel):

    print "myModel = ",  myModel

    dimension = myModel.getDimension()

    x1 = NumericalPoint(dimension)
    x2 = NumericalPoint(dimension)
    for j in range(dimension):
        x1[j] = -1.0 - j
        x2[j] = 3.0 + 2.0 * j

    print "myModel(", x1, ", ", x2, ")=",  myModel(x1, x2)

    grad = SymmetricTensor(myModel.partialGradient(x1, x2))
    print "dCov =", grad

    eps = 1e-5
    gradfd = NumericalPoint(dimension)
    for j in range(dimension):
        x1_d = x1
        x1_d[j] = x1_d[j] + eps
        gradfd[j] = (myModel(x1_d, x2)[0, 0] - myModel(x1, x2)[0, 0]) / eps

    print "dCov (FD)=", gradfd


dimension = 2


myDefautModel = SquaredExponential()
print "myDefautModel = ",  myDefautModel


myModel = SquaredExponential(dimension)
test_model(myModel)


myDefautModel = GeneralizedExponential()
print "myDefautModel = ",  myDefautModel

myModel = GeneralizedExponential(dimension, 0.1, 1.5)
test_model(myModel)

myDefautModel = AbsoluteExponential()
print "myDefautModel = ",  myDefautModel

myModel = AbsoluteExponential(dimension)
test_model(myModel)
