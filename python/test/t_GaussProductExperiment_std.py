#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:
    distribution = ComposedDistribution(
        DistributionCollection([Exponential(), Triangular(-1.0, -0.5, 1.0)]))
    marginalDegrees = Indices([3, 6])
    myPlane = GaussProductExperiment(
        Distribution(distribution), marginalDegrees)
    print "myPlane = ", myPlane
    sample, weights = myPlane.generateWithWeights()
    print "sample = ", repr(sample)
    print "weights = ", repr(weights)
except:
    import sys
    print "t_GaussProductExperiment_std.py", sys.exc_type, sys.exc_value
