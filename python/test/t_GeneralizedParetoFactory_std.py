#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator.SetSeed(0)
PlatformInfo.SetNumericalPrecision(4)
try:
    xi = [-0.75, 0.0, 0.75]
    size = 10000
    covariance = CovarianceMatrix(2)
    factory = GeneralizedParetoFactory()
    for i in range(3):
        distribution = GeneralizedPareto(2.5, xi[i])
        sample = distribution.getSample(size)
        # Distribution estimatedDistribution(factory.build(sample, covariance))
        estimatedDistribution = factory.build(sample)
        print "Distribution          =", distribution
        print "Estimated distribution=", estimatedDistribution
        estimatedGeneralizedPareto = factory.buildAsGeneralizedPareto(sample)
        print "GeneralizedPareto          =", distribution
        print "Estimated generalizedPareto=", estimatedGeneralizedPareto
        # print "Covariance=", covariance
    estimatedDistribution = factory.build()
    print "Default distribution=", estimatedDistribution
    estimatedDistribution = factory.build(
        distribution.getParametersCollection())
    print "Distribution from parameters=", estimatedDistribution
    estimatedGeneralizedPareto = factory.buildAsGeneralizedPareto()
    print "Default generalizedPareto=", estimatedGeneralizedPareto
    estimatedGeneralizedPareto = factory.buildAsGeneralizedPareto(
        distribution.getParametersCollection())
    print "GeneralizedPareto from parameters=", estimatedGeneralizedPareto

except:
    import sys
    print "t_GenralizedParetoFactory_std.py", sys.exc_type, sys.exc_value
