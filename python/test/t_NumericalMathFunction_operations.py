#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:
    # Product
    # First, build a function from R^3->R
    inVar = ["x0", "x1", "x2"]
    outVar = ["y0"]
    formula = ["x0^2 + 2 * x1 * x2 + 3 * x2"]
    f1 = NumericalMathFunction(inVar, outVar, formula)
    # Second, build a function from R^3->R^2
    formula = ["x2 - x0 + x1"]
    formula.add("x0 + x1 * x0 + x2")
    outVar = ["y0", "y1"]
    f2 = NumericalMathFunction(inVar, outVar, formula)
    # Third, build the product function
    myFunction = f1 * f2
    inPoint = [1.2, 2.3, 3.4]
    print "myFunction=", myFunction
    const NumericalPoint value(myFunction(inPoint))
    print "Value at ", inPoint, "=\n", value
    const Matrix gradient(myFunction.gradient(inPoint))
    print "Gradient at ", inPoint, "=\n", gradient
    const SymmetricTensor hessian(myFunction.hessian(inPoint))
    print "Hessian at ", inPoint, "=\n", hessian

    # Sum/difference
    # First, build two functions from R^3->R^2
    inVar = ["x0", "x1", "x2"]
    outVar = ["y0", "y1"]
    formula = ["x0 + 2 * x1 * x2 + 3 * x2", "x2 - x0 + x1 * x0"]
    f1 = NumericalMathFunction(inVar, outVar, formula)
    formula = ["x0 + x1 + x2", "-2 * x0 + 3 * x2 * x1 - x1"]
    f2 = NumericalMathFunction(inVar, outVar, formula)
    # Second, build the function
    mySum = f1 + f2
    print "mySum=", mySum
    value = mySum(inPoint)
    print "Value at ", inPoint, "=\n", value
    gradient = mySum.gradient(inPoint)
    print "Gradient at ", inPoint, "=\n", gradient
    hessian = mySum.hessian(inPoint)
    print "Hessian at ", inPoint, "=\n", hessian
    myDiff = f1 - f2
    print "myDiff=", myDiff
    value = myDiff(inPoint)
    print "Value at ", inPoint, "=\n", value
    print "Value at ", inPoint, "=\n", value.__str__()
    gradient = myDiff.gradient(inPoint)
    print "Gradient at ", inPoint, "=\n", gradient
    hessian = myDiff.hessian(inPoint)
    print "Hessian at ", inPoint, "=\n", hessian

except:
    import sys
    print "t_NumericalMathFunction_operations.py", sys.exc_type, sys.exc_value
