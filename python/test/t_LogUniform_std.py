#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()
RandomGenerator.SetSeed(0)

try:
    # Instanciate one distribution object
    distribution = LogUniform(-0.5, 1.5)
    print "Distribution ", repr(distribution)
    print "Distribution ", distribution

    # Get mean and covariance
    print "Mean= ", repr(distribution.getMean())
    print "Covariance= ", repr(distribution.getCovariance())

    size = 100
    for i in range(2):
        msg = ''
        if FittingTest.Kolmogorov(distribution.getSample(size), distribution).getBinaryQualityMeasure():
            msg = "accepted"
        else:
            msg = "rejected"
        print "Kolmogorov test for the generator, sample size=", size, " is", msg
        size *= 10

    # Is this distribution elliptical ?
    print "Elliptical = ", distribution.isElliptical()

    # Test for realization of distribution
    oneRealization = distribution.getRealization()
    print "oneRealization=", repr(oneRealization)

    # Test for sampling
    size = 10000
    oneSample = distribution.getSample(size)
    print "oneSample first=", repr(oneSample[0]), " last=", repr(oneSample[size - 1])
    print "mean=", repr(oneSample.computeMean())
    print "covariance=", repr(oneSample.computeCovariance())

    # Define a point
    point = NumericalPoint(distribution.getDimension(), 1.0)
    print "Point= ", repr(point)

    # Show PDF and CDF of point
    eps = 1e-5

    # derivative of PDF with regards its arguments
    DDF = distribution.computeDDF(point)
    print "ddf     =", repr(DDF)
    # by the finite difference technique
    print "ddf (FD)=", repr(NumericalPoint(1, (distribution.computePDF(point + NumericalPoint(1, eps)) - distribution.computePDF(point + NumericalPoint(1, -eps))) / (2.0 * eps)))

    # PDF value
    LPDF = distribution.computeLogPDF(point)
    print "log pdf=%.6f" % LPDF
    PDF = distribution.computePDF(point)
    print "pdf     =%.6f" % PDF
    # by the finite difference technique from CDF
    print "pdf (FD)=%.6f" % ((distribution.computeCDF(point + NumericalPoint(1, eps)) - distribution.computeCDF(point + NumericalPoint(1, -eps))) / (2.0 * eps))

    # derivative of the PDF with regards the parameters of the distribution
    CDF = distribution.computeCDF(point)
    print "cdf=%.6f" % CDF
    CCDF = distribution.computeComplementaryCDF(point)
    print "ccdf=%.6f" % CCDF
    # CF = distribution.computeCharacteristicFunction(point[0])
    # print "characteristic function=", CF
    PDFgr = distribution.computePDFGradient(point)
    print "pdf gradient     =", repr(PDFgr)
    # by the finite difference technique
    PDFgrFD = NumericalPoint(2)
    PDFgrFD[0] = (LogUniform(distribution.getALog() + eps, distribution.getBLog()).computePDF(point) -
                  LogUniform(distribution.getALog() - eps, distribution.getBLog()).computePDF(point)) / (2.0 * eps)
    PDFgrFD[1] = (LogUniform(distribution.getALog(), distribution.getBLog() + eps).computePDF(point) -
                  LogUniform(distribution.getALog(), distribution.getBLog() - eps).computePDF(point)) / (2.0 * eps)
    print "pdf gradient (FD)=", repr(PDFgrFD)

    # derivative of the PDF with regards the parameters of the distribution
    CDFgr = distribution.computeCDFGradient(point)
    print "cdf gradient     =", repr(CDFgr)
    CDFgrFD = NumericalPoint(2)
    CDFgrFD[0] = (LogUniform(distribution.getALog() + eps, distribution.getBLog()).computeCDF(point) -
                  LogUniform(distribution.getALog() - eps, distribution.getBLog()).computeCDF(point)) / (2.0 * eps)
    CDFgrFD[1] = (LogUniform(distribution.getALog(), distribution.getBLog() + eps).computeCDF(point) -
                  LogUniform(distribution.getALog(), distribution.getBLog() - eps).computeCDF(point)) / (2.0 * eps)
    print "cdf gradient (FD)=",  repr(CDFgrFD)

    # quantile
    quantile = distribution.computeQuantile(0.95)
    print "quantile=", repr(quantile)
    print "cdf(quantile)=%.6f" % distribution.computeCDF(quantile)
    mean = distribution.getMean()
    print "mean=", repr(mean)
    standardDeviation = distribution.getStandardDeviation()
    print "standard deviation=", repr(standardDeviation)
    skewness = distribution.getSkewness()
    print "skewness=", repr(skewness)
    kurtosis = distribution.getKurtosis()
    print "kurtosis=", repr(kurtosis)
    covariance = distribution.getCovariance()
    print "covariance=", repr(covariance)
    parameters = distribution.getParametersCollection()
    print "parameters=", repr(parameters)
    for i in range(6):
        print "standard moment n=", i, " value=", distribution.getStandardMoment(i)
    print "Standard representative=", distribution.getStandardRepresentative()

except:
    import sys
    print "t_LogUniform_std.py", sys.exc_type, sys.exc_value
