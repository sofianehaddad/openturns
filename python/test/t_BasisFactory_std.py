#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:
    inputDimension = 2

    x = NumericalPoint(inputDimension)
    for i in range(inputDimension):
        x[i] = 2. + i

    print "x=", x

    factory = ConstantBasisFactory(inputDimension)
    print "factory=", factory
    basis = factory.build()
    print "basis=", basis

    f = NumericalMathFunction(basis)
    y = f(x)
    print "y=", y

    factory = LinearBasisFactory(inputDimension)
    print "factory=", factory
    basis = factory.build()
    print "basis=", basis

    f = NumericalMathFunction(basis)
    y = f(x)
    print "y=", y

    factory = QuadraticBasisFactory(inputDimension)
    print "factory=", factory
    basis = factory.build()
    print "basis=", basis

    f = NumericalMathFunction(basis)
    y = f(x)
    print "y=", y

except:
    import sys
    print "t_BasisFactory_std.py", sys.exc_type, sys.exc_value
