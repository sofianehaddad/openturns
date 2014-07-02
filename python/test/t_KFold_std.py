#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator.SetSeed(0)

try:
    size = 100
    xuniform = Uniform(0.9, 1.1)
    x = xuniform.getSample(size)
    yuniform = Uniform(1.9, 2.1)
    y = yuniform.getSample(size)
    f = NumericalMathFunction(['x'], ['y'], ['2.0*x'])
    basis = Basis()
    basis.add(f)

    fittingAlgo = KFold()

    print "algo =", fittingAlgo

    result = fittingAlgo.run(x, y, basis)

    print "result = %g" % result


except:
    import sys
    print "t_KFold_std.py", sys.exc_type, sys.exc_value
