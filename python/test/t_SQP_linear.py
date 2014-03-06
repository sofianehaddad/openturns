#! /usr/bin/env python

from openturns import *
from math import *


def printNumericalPoint(point, digits):
    oss = "["
    eps = pow(0.1, digits)
    for i in range(point.getDimension()):
        if i == 0:
            sep = ""
        else:
            sep = ","
        if fabs(point[i]) < eps:
            oss += sep + "%.6f" % fabs(point[i])
        else:
            oss += sep + "%.6f" % point[i]
        sep = ","
    oss += "]"
    return oss

TESTPREAMBLE()

try:
    try:
        # Test function operator ()
        levelFunction = NumericalMathFunction(
            ["x1", "x2", "x3", "x4"], ["y1"], ["x1+2*x2-3*x3+4*x4"])
        # Add a finite difference gradient to the function
        myGradient = NonCenteredFiniteDifferenceGradient(
            1e-7, levelFunction.getEvaluationImplementation())
        print "myGradient = ", repr(myGradient)
        # Substitute the gradient
        levelFunction.setGradientImplementation(
            NonCenteredFiniteDifferenceGradient(myGradient))
        specific = SQPSpecificParameters()
        startingPoint = NumericalPoint(4, 0.0)
        mySQPAlgorithm = SQP(specific, levelFunction)
        mySQPAlgorithm.setStartingPoint(startingPoint)
        mySQPAlgorithm.setLevelValue(3.0)
        print "mySQPAlgorithm = ", repr(mySQPAlgorithm)
        mySQPAlgorithm.run()
        print "result = ", printNumericalPoint(mySQPAlgorithm.getResult().getMinimizer(), 4)
    except:
        raise
except:
    import sys
    print "t_SQP_linear.py", sys.exc_type, sys.exc_value
