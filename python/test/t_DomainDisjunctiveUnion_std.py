#! /usr/bin/env python

from __future__ import print_function
import openturns as ot

ot.TESTPREAMBLE()

try:
    dim = 2

    # First domain: [0,2]x[0,2]
    cube = ot.Interval([0.0]*dim, [2.0]*dim)

    # Second domain: sphere center=(0,0) r=1
    function = ot.SymbolicFunction(["x","y"], ["x^2 + y^2"])
    sphere = ot.LevelSet(function, 1.0)

    # Inside sphere but not cube
    p0 = [-0.2, 0.2]
    # Inside cube and sphere
    p1 = [0.2, 0.2]
    # Inside cube but not sphere
    p2 = [0.8, 0.8]
    # Outside
    p3 = [4.0, 4.0]

    domain = ot.DomainDisjunctiveUnion(cube, sphere)
    print("cube=", cube)
    print("sphere=", sphere)
    print("disjunctive union=", domain)
    # Accessors
    print("Dimension=", domain.getDimension())
    # Contains
    print("is point ", p0, " inside ? ",  domain.contains(p0))
    print("is point ", p1, " inside ? ",  domain.contains(p1))
    print("is point ", p2, " inside ? ",  domain.contains(p2))
    print("is point ", p3, " inside ? ",  domain.contains(p3))
    sample = [p0, p1, p2, p3]
    print("is sample ", sample, " inside ? ",  domain.contains(sample))
except:
    import sys
    print("t_DomainDisjunctiveUnion_std.py", sys.exc_info()[0], sys.exc_info()[1])
