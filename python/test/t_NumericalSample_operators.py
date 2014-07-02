#! /usr/bin/env python

from openturns import *

TESTPREAMBLE()

try:
    # We create an empty NumericalSample
    sample1 = NumericalSample(0, 2)
    sample1.setName("Sample1")
    sample1.setDescription(('a0', 'a1'))

    # We populate the empty sample
    point1 = NumericalPoint(2)
    point1[0] = 10.
    point1[1] = 20.
    point2 = NumericalPoint(2)
    point2[0] = 11.
    point2[1] = 21.
    point3 = NumericalPoint(2)
    point3[0] = 12.
    point3[1] = 22.
    sample1.add(point1)
    sample1.add(point2)
    sample1.add(point3)
    print "sample1=", repr(sample1)

    # We create a new empty NumericalSample
    sample2 = NumericalSample(0, 2)
    sample2.setName("Sample2")
    sample2.setDescription(('a0', 'a1'))

    # We populate the empty sample
    point4 = NumericalPoint(2)
    point4[0] = 30.
    point4[1] = 40.
    point5 = NumericalPoint(2)
    point5[0] = 31.
    point5[1] = 41.
    point6 = NumericalPoint(2)
    point6[0] = 32.
    point6[1] = 42.
    sample2.add(point4)
    sample2.add(point5)
    sample2.add(point6)
    print "sample2=", repr(sample2)

    # Operator +|-
    summation = NumericalSample(sample1 + sample2)
    substraction = NumericalSample(sample2 - sample1)
    print "sample1 + sample2=", repr(summation)
    print "sample2 - sample1=", repr(substraction)

    # Operator +=|-=
    sample3 = NumericalSample(sample2)
    sample4 = NumericalSample(sample2)
    sample3 += sample1
    sample4 -= sample1
    print "sample3=", repr(sample3)
    print "sample4=", repr(sample4)

    # translation ==> += operator
    translation = NumericalPoint(2, 5.0)
    sample1 += translation
    print "after a translation of vector=", repr(translation), " sample1=", repr(sample1)

    # scaling ==> *= operator
    scaling = NumericalPoint(2, 2.0)
    sample1 *= scaling
    print "after a scaling of vector=", repr(scaling), " sample1=", repr(sample1)

    # operator /
    sample5 = NumericalSample(sample2)
    m = SquareMatrix([[1, 2], [3, 5]])
    mDivision = sample5 / m
    print "sample / m =", mDivision

    # operator /=
    sample5 /= m
    print "sample /= m =", mDivision

except:
    import sys
    print "t_NumericalSample_operators.py", sys.exc_type, sys.exc_value
