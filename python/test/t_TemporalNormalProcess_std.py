#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator.SetSeed(0)

try:
    # Default dimension parameter to evaluate the model
    defaultDimension = 1

    # Amplitude values
    amplitude = NumericalPoint(defaultDimension, 1.0)
    # Scale values
    scale = NumericalPoint(defaultDimension, 1.0)

    # Second order model with parameters
    myModel = ExponentialCauchy(amplitude, scale)
    print "myModel = ", myModel

    # checking the copy-cast*/
    mySecondOrderModel = SecondOrderModel(myModel)
    print "mySecondOrderModel = ", mySecondOrderModel

    tmin = 0.0
    step = 0.1
    n = 11

    myTimeGrid = RegularGrid(tmin, step, n)
    size = 100

    myProcess = TemporalNormalProcess(myModel, myTimeGrid)
    print "myProcess = ", myProcess
    print "is stationary? ", myProcess.isStationary()
    ResourceMap.SetAsUnsignedInteger("TemporalNormalProcess-UseGibbsSampler", 0)
    print "mean over ", size, " realizations = ", myProcess.getSample(size).computeMean()
    ResourceMap.SetAsUnsignedInteger("TemporalNormalProcess-UseGibbsSampler", 1)
    print "mean over ", size, " realizations = ", myProcess.getSample(size).computeMean()

    # Second order model with parameters
    myCovModel = ExponentialModel(amplitude, scale)
    print "myCovModel = ", myCovModel

    myProcess1 = TemporalNormalProcess(myCovModel, myTimeGrid)
    print "myProcess1 = ", myProcess1
    print "is stationary? ", myProcess1.isStationary()
    ResourceMap.SetAsUnsignedInteger("TemporalNormalProcess-UseGibbsSampler", 0)
    print "mean over ", size, " realizations = ", myProcess.getSample(size).computeMean()
    ResourceMap.SetAsUnsignedInteger("TemporalNormalProcess-UseGibbsSampler", 1)
    print "mean over ", size, " realizations = ", myProcess.getSample(size).computeMean()

    # With constant trend
    trend = TrendTransform(NumericalMathFunction("t", "4.0"))
    myProcess2 = TemporalNormalProcess(trend, myCovModel, myTimeGrid)
    print "myProcess2 = ", myProcess2
    print "is stationary? ", myProcess2.isStationary()
    print "mean over ", size, " realizations= ", myProcess2.getSample(size).computeMean()

    # With varying trend
    trend3 = TrendTransform(NumericalMathFunction("t", "sin(t)"))
    myProcess3 = TemporalNormalProcess(trend3, myCovModel, myTimeGrid)
    print "myProcess3 = ", myProcess3
    print "is stationary? ", myProcess3.isStationary()
    ResourceMap.SetAsUnsignedInteger("TemporalNormalProcess-UseGibbsSampler", 0)
    print "mean over ", size, " realizations = ", myProcess.getSample(size).computeMean()
    ResourceMap.SetAsUnsignedInteger("TemporalNormalProcess-UseGibbsSampler", 1)
    print "mean over ", size, " realizations = ", myProcess.getSample(size).computeMean()
    
except:
    import sys
    print "t_TemporalNormalProcess_std.py", sys.exc_type, sys.exc_value
