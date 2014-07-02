#! /usr/bin/env python

from openturns import *
import math

TESTPREAMBLE()
RandomGenerator.SetSeed(0)

try:
    # First, a smooth function
    f = NumericalMathFunction("x", "sin(x)")
    a = -2.5
    b = 4.5
    # Default parameters
    algo = GaussKronrod()
    rules = [GaussKronrodRule(GaussKronrodRule.G3K7), GaussKronrodRule(
        GaussKronrodRule.G7K15), GaussKronrodRule(GaussKronrodRule.G11K23), GaussKronrodRule(GaussKronrodRule.G15K31), GaussKronrodRule(GaussKronrodRule.G25K51)]
    for i in range(len(rules)):
        algo.setRule(rules[i])
        print "Algo=", algo
        # High-level interface
        error = NumericalPoint()
        value = algo.integrate(f, Interval(a, b), error)
        ref = math.cos(a) - math.cos(b)
        print "value=%.6f" % value, ", ref=%.6f" % ref, ", true error below bound? ", abs(ref - value) < algo.getMaximumError(), ", estimated error below bound? ", error[0] < algo.getMaximumError()
        # Low-level interface
        #ai = NumericalPoint()
        #bi = NumericalPoint()
        #fi = NumericalPoint()
        #ei = NumericalPoint()
        #value2 = algo.integrate(f, a, b, error, ai, bi, fi, ei)
        # ai.add(b)
        #g = f.draw(a, b, 512)
        #lower = Cloud(ai, NumericalPoint(ai.getDimension()))
        # lower.setColor("magenta")
        # g.add(lower)
        #g.draw("Smooth_function_adaptation_" + str(i))
    # Second, a piecewise smooth function
    f = NumericalMathFunction("x", "abs(sin(x))")
    a = -2.5
    b = 4.5
    algo = GaussKronrod()
    rules = [GaussKronrodRule(GaussKronrodRule.G3K7), GaussKronrodRule(
        GaussKronrodRule.G7K15), GaussKronrodRule(GaussKronrodRule.G11K23), GaussKronrodRule(GaussKronrodRule.G15K31), GaussKronrodRule(GaussKronrodRule.G25K51)]
    for i in range(len(rules)):
        algo.setRule(rules[i])
        print "Algo=", algo
        error = NumericalPoint()
        value = algo.integrate(f, Interval(a, b), error)
        ref = 4.0 + math.cos(b) - math.cos(a)
        print "value=%.6f" % value, ", ref=%.6f" % ref, ", true error below bound? ", abs(ref - value) < algo.getMaximumError(), ", estimated error below bound? ", error[0] < algo.getMaximumError()
        # Low-level interface
        ## ai = NumericalPoint()
        ## bi = NumericalPoint()
        ## fi = NumericalPoint()
        ## ei = NumericalPoint()
        ## value2 = algo.integrate(f, a, b, error, ai, bi, fi, ei)
        ## print "ai=", ai
        ## print "bi=", bi
        ## print "fi=", fi
        ## print "ei=", ei
        ## ai.add(b)
        ## g = f.draw(a, b, 512)
        ## lower = Cloud(ai, NumericalPoint(ai.getDimension()))
        ## lower.setColor("magenta")
        ## g.add(lower)
        ## g.draw("Nonsmooth_function_adaptation_" + str(i))
except:
    import sys
    print "t_GaussKronrod_std.py", sys.exc_type, sys.exc_value
