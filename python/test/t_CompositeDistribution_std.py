#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator.SetSeed(0)

try:
    # Instanciate one distribution object
    f = NumericalMathFunction("x", "x^2 + 2 * sin(x)")
    distribution = CompositeDistribution(f, Normal())
    print "Distribution ", repr(distribution)
    print "Distribution ", distribution

    # Is this distribution elliptical ?
    print "Elliptical = ", distribution.isElliptical()

    # Is this distribution continuous ?
    print "Continuous = ", distribution.isContinuous()

    # Test for realization of distribution
    oneRealization = distribution.getRealization()
    print "oneRealization=", oneRealization

    # Test for sampling
    size = 10000
    oneSample = distribution.getSample( size )
    print "oneSample first=", oneSample[0], " last=", oneSample[size - 1]
    print "mean=", oneSample.computeMean()
    print "covariance=", oneSample.computeCovariance()
    #RandomGenerator::SetSeed(0)
    size = 100
    for i in range(2):
        msg = ''
        if FittingTest.Kolmogorov(distribution.getSample(size), distribution).getBinaryQualityMeasure():
            msg = "accepted"
        else:
            msg = "rejected"
        print "Kolmogorov test for the generator, sample size=", size, " is", msg
        size *= 10


    # Define a point
    point = NumericalPoint( distribution.getDimension(), 1.0 )
    print "Point= ", point

    # Show PDF and CDF of point
    eps = 1e-5
    DDF = distribution.computeDDF( point )
    print "ddf     =", DDF
    LPDF = distribution.computeLogPDF( point )
    print "log pdf=", LPDF
    PDF = distribution.computePDF( point )
    print "pdf     =", PDF
    print "pdf (FD)=", (distribution.computeCDF( point + NumericalPoint(1, eps) ) - distribution.computeCDF( point  + NumericalPoint(1, -eps) )) / (2.0 * eps)
    CDF = distribution.computeCDF( point )
    print "cdf=", CDF
    CCDF = distribution.computeComplementaryCDF( point )
    print "ccdf=", CCDF
    Survival = distribution.computeSurvivalFunction( point )
    print "survival=", Survival
    quantile = distribution.computeQuantile( 0.95 )
    print "quantile=", quantile
    print "cdf(quantile)=", distribution.computeCDF(quantile)
    quantileTail = distribution.computeQuantile( 0.95, True )
    print "quantile (tail)=", quantileTail
    CDFTail = distribution.computeComplementaryCDF( quantileTail )
    print "cdf (tail)=", CDFTail
    CF = distribution.computeCharacteristicFunction( point[0] )
    print "characteristic function=", CF
    LCF = distribution.computeLogCharacteristicFunction( point[0] )
    print "log characteristic function=", LCF
    PDFgr = distribution.computePDFGradient( point )
    print "pdf gradient     =", PDFgr
    CDFgr = distribution.computeCDFGradient( point )
    print "cdf gradient     =", CDFgr
    mean = distribution.getMean()
    print "mean=", mean
    standardDeviation = distribution.getStandardDeviation()
    print "standard deviation=", standardDeviation
    skewness = distribution.getSkewness()
    print "skewness=", skewness
    kurtosis = distribution.getKurtosis()
    print "kurtosis=", kurtosis
    covariance = distribution.getCovariance()
    print "covariance=", covariance
    correlation = distribution.getCorrelation()
    print "correlation=", correlation
    spearman = distribution.getSpearmanCorrelation()
    print "spearman=", spearman
    kendall = distribution.getKendallTau()
    print "kendall=", kendall
    parameters = distribution.getParametersCollection()
    print "parameters=", parameters
    for i in range(6):
        print "standard moment n=", i, ", value=", distribution.getStandardMoment(i)
    print "Standard representative=", distribution.getStandardRepresentative()

    # Specific to this distribution
    antecedent = distribution.getAntecedent()
    print "antecedent=", antecedent
    function = distribution.getFunction()
    print "function=", function
    newDistribution = CompositeDistribution(function, antecedent)
    print "newDistribution=", newDistribution

except:
    import sys
    print "t_CompositeDistribution_std.py", sys.exc_type, sys.exc_value
