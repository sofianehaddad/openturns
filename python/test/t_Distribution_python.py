#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()


class UniformNdPy(PythonDistribution):

    def __init__(self, a=[0.0], b=[1.0]):
        super(UniformNdPy, self).__init__(len(a))
        self.a = a
        self.b = b

    def getRange(self):
        return [self.a, self.b, [True] * len(self.a), [True] * len(self.a)]

    def getRealization(self):
        X = []
        for i in range(len(self.a)):
            X.append(
                self.a[i] + (self.b[i] - self.a[i]) * RandomGenerator.Generate())
        return X

    def getSample(self, size):
        X = []
        for i in range(size):
            X.append(self.getRealization())
        return X

    def computeCDF(self, X):
        return (X[0] - self.a[0]) / (self.b[0] - self.a[0])

    def computePDF(self, X):
        return 1. / (self.b[0] - self.a[0])

    def getMean(self):
        mu = []
        for i in range(len(self.a)):
            mu.append(0.5 * (self.a[i] + self.b[i]))
        return mu

    def getStandardDeviation(self):
        stdev = []
        for i in range(len(self.a)):
            stdev.append((self.b[i] - self.a[i]) / sqrt(12.))
        return stdev

    def getSkewness(self):
        return [0.] * len(self.a)

    def getKurtosis(self):
        return [1.8] * len(self.a)

    def getStandardMoment(self, n):
        if n % 2 == 1:
            return [0.] * len(self.a)
        return [1. / (n + 1.)] * len(self.a)

    def getMoment(self, n):
        return [-0.1 * n] * len(self.a)

    def getCenteredMoment(self, n):
        return [0.] * len(self.a)

    def computeCharacteristicFunction(self, x):
        if len(self.a) > 1:
            raise ValueError('dim>1')
        ax = self.a[0] * x
        bx = self.b[0] * x
        return (sin(bx) - sin(ax) + 1j * (cos(ax) - cos(bx))) / (bx - ax)

    def isElliptical(self):
        return (len(self.a) == 1) and (self.a[0] == -self.b[0])

    def isCopula(self):
        for i in range(len(self.a)):
            if self.a[i] != 0.0:
                return False
            if self.b[i] != 1.0:
                return False
        return True

for pyDist in [UniformNdPy(), UniformNdPy([0.] * 2, [1.] * 2)]:

    print "pyDist=", pyDist

    # Instance creation
    myDist = Distribution(pyDist)
    print "myDist=", repr(myDist)

    # Copy constructor
    newRV = Distribution(myDist)

    # Dimension
    dim = myDist.getDimension()
    print 'dimension=', dim

    # Realization
    X = myDist.getRealization()
    print 'realization=', X

    # Sample
    X = myDist.getSample(5)
    print 'sample=', X

    # PDF
    point = [0.2] * dim
    pdf = myDist.computePDF(point)
    print 'pdf=', pdf

    # CDF
    cdf = myDist.computeCDF(point)
    print 'cdf=', cdf

    # Mean
    mean = myDist.getMean()
    print 'mean=', mean

    # Standard deviation
    standardDeviation = myDist.getStandardDeviation()
    print 'standard deviation=', standardDeviation

    # Skewness
    skewness = myDist.getSkewness()
    print 'skewness=', skewness

    # Kurtosis
    kurtosis = myDist.getKurtosis()
    print 'kurtosis=', kurtosis

    # standard moment
    standardMoment = myDist.getStandardMoment(2)
    print 'standard moment=', standardMoment

    # Moment
    moment = myDist.getMoment(3)
    print 'moment=', moment

    # Centered moment
    centeredMoment = myDist.getCenteredMoment(3)
    print 'centered moment=', centeredMoment

    try:
        CF = myDist.computeCharacteristicFunction(point[0])
        print "characteristic function= (%.12g%+.12gj)" % (CF.real, CF.imag)
    except:
        pass

    isElliptical = myDist.isElliptical()
    print 'isElliptical=', isElliptical

    isCopula = myDist.isCopula()
    print 'isCopula=', isCopula

    # Range
    range_ = myDist.getRange()
    print 'range=', range_

# Use the distribution as a copula
myDist = Distribution(UniformNdPy([0.0] * 2, [1.0] * 2))
print ComposedDistribution([Normal(), Normal()], myDist)
try:
    print "try with another Python distribution"
    myDist = Distribution(UniformNdPy([0.0] * 2, [2.0] * 2))
    print ComposedDistribution([Normal(), Normal()], myDist)
except:
    print "The construction failed on purpose as", myDist, "is not a copula"
