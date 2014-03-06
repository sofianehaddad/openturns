#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try:
    elementaryFunctions = Description(0)
    elementaryFunctions.add("sin")
    elementaryFunctions.add("cos")
    elementaryFunctions.add("tan")
    elementaryFunctions.add("asin")
    elementaryFunctions.add("acos")
    elementaryFunctions.add("atan")
    elementaryFunctions.add("sinh")
    elementaryFunctions.add("cosh")
    elementaryFunctions.add("tanh")
    elementaryFunctions.add("asinh")
    elementaryFunctions.add("acosh")
    elementaryFunctions.add("atanh")
    elementaryFunctions.add("log2")
    elementaryFunctions.add("log10")
    elementaryFunctions.add("log")
    elementaryFunctions.add("ln")
    elementaryFunctions.add("lngamma")
    elementaryFunctions.add("gamma")
    elementaryFunctions.add("exp")
    elementaryFunctions.add("erf")
    elementaryFunctions.add("erfc")
    elementaryFunctions.add("sqrt")
    elementaryFunctions.add("cbrt")
    elementaryFunctions.add("besselJ0")
    elementaryFunctions.add("besselJ1")
    elementaryFunctions.add("besselY0")
    elementaryFunctions.add("besselY1")
    elementaryFunctions.add("sign")
    elementaryFunctions.add("rint")
    elementaryFunctions.add("abs")
    # Check the creation of the elementary functions
    for i in range(elementaryFunctions.getSize()):
        x = NumericalPoint(1, 0.4 / 3)
        # acosh only defined for 1 <= x <= pi
        if elementaryFunctions[i] == "acosh":
            x[0] = 1.4 / 3

        f = NumericalMathFunction(
            "x", "2.0*" + elementaryFunctions[i] + "(3*x)", "y")
        print "f=", f
        print "f(", x[0], ")=%.4e" % f(x)[0]
        analytical_grad = True
        try:
            df = f.gradient(x)[0, 0]
        except:
            analytical_grad = False
        if analytical_grad:
            f.setGradientImplementation(CenteredFiniteDifferenceGradient(ResourceMap.GetAsNumericalScalar(
                "CenteredFiniteDifferenceGradient-DefaultEpsilon"), f.getEvaluationImplementation()))
            df2 = f.gradient(x)[0, 0]
            print "df(", x[0], ")=%.4e" % df, "df (FD)=%.4e" % df2
            if abs(df) > 1e-5:
                err_g = abs(df2 / df - 1.)
            else:
                err_g = abs(df - df2)
            if err_g > 1e-5:
                print "GRADIENT ERROR! check " + elementaryFunctions[i] + " gradient, err=", err_g
        analytical_hess = True
        try:
            d2f = f.hessian(x)[0, 0, 0]
        except:
            analytical_hess = False
        if analytical_hess:
            f.setHessianImplementation(CenteredFiniteDifferenceHessian(ResourceMap.GetAsNumericalScalar(
                "CenteredFiniteDifferenceHessian-DefaultEpsilon"), f.getEvaluationImplementation()))
            d2f2 = f.hessian(x)[0, 0, 0]
            print "d2f(", x[0], ")=%.4e" % d2f, "d2f (FD)=%.4e" % d2f2
            if abs(d2f) > 1e-5:
                err_h = abs(d2f2 / d2f - 1.)
            else:
                err_h = abs(d2f - d2f2)
            if err_h > 1e-4:
                print "HESSIAN ERROR! check " + elementaryFunctions[i] + " hessian, err=", err_h

    nmf = NumericalMathFunction(['x0', 'x1'], ['y0', 'y1'], ['x0+x1', 'x0-x1'])
    marginal0 = nmf.getMarginal(0)
    marginal1 = nmf.getMarginal(1)
    print "marginal 0=", marginal0
    print "marginal 1=", marginal1

    # test sample as input of a function
    formula = ["sin(xi1) + 7. * (sin(xi2)) ^ 2 + 0.1 * xi3^4 * sin(xi1)"]
    model = NumericalMathFunction(["xi1", "xi2", "xi3"], ["y"], formula)

    # Create an input distribution to calculate reference values
    distribution = ComposedDistribution([Uniform(-pi, pi)] * 3)
    inSample = NumericalSample(distribution.getSample(100))
    resultSample = NumericalSample(model(inSample))
    refResultValues = [NumericalPoint(model([inSample[i][0], inSample[i][1], inSample[i][2]]))
                       for i in range(100)]

    print "First reference value : %.4e" % refResultValues[0][0]
    print "First result calculated : %.4e" % resultSample[0][0]

except:
    import sys
    print "t_NumericalMathFunction_analytical.py", sys.exc_type, sys.exc_value
