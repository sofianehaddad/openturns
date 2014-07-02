#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator.SetSeed(0)


def cleanScalar(inScalar):
    if (abs(inScalar) < 1.e-6):
        return 0.0
    return inScalar


try:

    # Default constructor
    myDefautModel = UserDefinedSpectralModel()
    print "myDefautModel = ", myDefautModel

    # Default dimension parameter to evaluate the model
    dimension = 2

    # Amplitude values
    amplitude = NumericalPoint(dimension)
    # Scale values
    scale = NumericalPoint(dimension)
    # Spatial correclation
    spatialCorrelation = CorrelationMatrix(dimension)
    for index in range(dimension):
            # constant amplitude
        amplitude[index] = 1.0
        scale[index] = (index + 1.0) / dimension
        if (index > 0):
            spatialCorrelation[index, index - 1] = 1.0 / index

    # Sample a CauchyModel
    referenceModel = CauchyModel(amplitude, scale, spatialCorrelation)

    size = 5
    frequencyGrid = RegularGrid(0.0, 2.0 / size, size)
    dspCollection = HermitianMatrixCollection(frequencyGrid.getN())
    for i in range(frequencyGrid.getN()):
        dspCollection[i] = referenceModel(frequencyGrid.getValue(i))

    # Create a UserDefinedSpectralModel
    myModel = UserDefinedSpectralModel(frequencyGrid, dspCollection)
    print "myModel=", myModel

    # Sample the UserDefinedSpectralModel
    samplingGrid = RegularGrid(-0.4, 1.0 / 16, 5 * size)

    for i in range(samplingGrid.getN()):
        frequency = samplingGrid.getValue(i)
        print "frequency= %g myModel=" % cleanScalar(frequency)
        print myModel(frequency).clean(1e-6), ", referenceModel="
        print referenceModel(frequency).clean(1e-6)

except:
    import sys
    print "t_UserDefinedSpectralModel_std.py", sys.exc_type, sys.exc_value
