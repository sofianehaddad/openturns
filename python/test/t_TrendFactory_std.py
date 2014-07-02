#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator.SetSeed(0)

try:

    inVar = Description(1)
    inVar[0] = "t"
    outVar = Description(1)
    outVar[0] = "y"
    formula = Description(1)

    functions = []

    formula[0] = "1"
    functions.append(NumericalMathFunction(inVar, outVar, formula))

    formula[0] = "cos(2 * t)"
    functions.append(NumericalMathFunction(inVar, outVar, formula))

    formula[0] = "sin(2 * t)"
    functions.append(NumericalMathFunction(inVar, outVar, formula))

    # We build the weights
    coefficients = NumericalSample(0, 2)
    p = NumericalPoint(2)
    p[0] = 1.5
    p[1] = 2.5
    coefficients.add(p)
    p[0] = -0.5
    p[1] = 0.5
    coefficients.add(p)
    p[0] = 1.
    p[1] = 1.
    coefficients.add(p)

    # Third, build the function
    myFunction = NumericalMathFunction(functions, coefficients)

    # Fourth : we build a time series for estimation
    # it issued from a white noise
    dimension = 2

    # Fix the realization as a Normal
    noiseDistribution = Normal(dimension)

    # TimeGrid parameters
    N = 1000
    timeStart = 0.
    timeStep = 0.1
    timeGrid = RegularGrid(timeStart, timeStep, N)

    # White noise
    myWhiteNoise = WhiteNoise(noiseDistribution, timeGrid)

    realization = TimeSeries(myWhiteNoise.getRealization())
    print "White noise realization = ", realization

    # We make a trend transform to the time series
    # We get a time series which contains values of time
    myTransformFunction = TrendTransform(myFunction)
    myTimeSeries = TimeSeries(myTransformFunction(realization))
    print "myTimeSeries = ", myTimeSeries

    # We wants to get the coefficients using a factory
    # Build a factory using default constructor
    myDefaultFactory = TrendFactory()
    print "myDefaultFactory = ", myDefaultFactory

    myEstimateTrend = myDefaultFactory.build(myTimeSeries, Basis(functions))
    print "myEstimateTrend = ", myEstimateTrend

    # We fix a new fitting algorithm
    myDefaultFactory.setFittingAlgorithm(KFold())
    print "myDefaultFactory = ", myDefaultFactory
    myNewEstimateTrend = myDefaultFactory.build(myTimeSeries, Basis(functions))
    print "myNewEstimateTrend = ", myNewEstimateTrend

except:
    import sys
    print "t_TrendTransform_std.py", sys.exc_type, sys.exc_value
