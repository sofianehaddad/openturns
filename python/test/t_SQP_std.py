#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:

    try:
        # Test function operator ()
        levelFunction = NumericalMathFunction(
            ["x1", "x2", "x3", "x4"], ["y1"], ["x1+2*x2-3*x3+4*x4"])
        specific = SQPSpecificParameters()
        startingPoint = NumericalPoint(4, 1.0)
        mySQPAlgorithm = SQP(specific, levelFunction)
        mySQPAlgorithm.setStartingPoint(startingPoint)
        mySQPAlgorithm.setLevelValue(3.0)
        mySQPAlgorithm.setMaximumIterationsNumber(100)
        mySQPAlgorithm.setMaximumAbsoluteError(1.0e-10)
        mySQPAlgorithm.setMaximumRelativeError(1.0e-10)
        mySQPAlgorithm.setMaximumResidualError(1.0e-10)
        mySQPAlgorithm.setMaximumConstraintError(1.0e-10)
        print "mySQPAlgorithm = ", mySQPAlgorithm
        # except NoWrapperFileFoundException, ex :
    except:
        raise

except:
    import sys
    print "t_SQP_std.py", sys.exc_type, sys.exc_value
