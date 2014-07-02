#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator.SetSeed(0)

try:
    size = 5
    dim = 6
    refSample = NumericalSample(size, dim)
    for i in range(size):
        p = NumericalPoint(dim)
        for j in range(dim):
            p[j] = i + j
        refSample[i] = p
    print "ref. sample=", refSample
    myPlane = FixedExperiment(refSample)
    print "myPlane = ", myPlane
    sample, weights = myPlane.generateWithWeights()
    print "sample = ", repr(sample)
    print "weights = ", repr(weights)

except:
    import sys
    print "t_FixedExperiment_std.py", sys.exc_type, sys.exc_value
