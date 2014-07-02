#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:
    distribution = Normal(4)
    size = 10
    myPlane = LHSExperiment(distribution, size)
    print "myPlane = ", myPlane
    sample, weights = myPlane.generateWithWeights()
    print "sample = ", repr(sample)
    print "weights = ", repr(weights)
except:
    import sys
    print "t_LHSExperiment_std.py", sys.exc_type, sys.exc_value
