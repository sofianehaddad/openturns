#! /usr/bin/env python

from openturns import *
from math import *

TESTPREAMBLE()

try:
    mesh1D = Mesh()
    print "Default 1D mesh=", mesh1D
    vertices = NumericalSample(0, 1)
    vertices.add([0.5])
    vertices.add([1.5])
    vertices.add([2.1])
    vertices.add([2.7])
    simplicies = IndicesCollection(3, Indices(2))
    simplicies[0] = [0, 1]
    simplicies[1] = [1, 2]
    simplicies[2] = [2, 3]
    mesh1D = Mesh(vertices, simplicies)
    print "1D mesh=", mesh1D
    print "Is empty? ", mesh1D.isEmpty()
    print "vertices=", mesh1D.getVertices()
    print "simplices=", mesh1D.getSimplices()
    print "volume=", mesh1D.getVolume()
    print "First simplex volume=", mesh1D.computeSimplexVolume(0)
    p = [1.3]
    print "is p=", p, " in mesh? ", mesh1D.contains(p)
    mesh1D.draw().draw("mesh1D")
    mesh1D.draw1D().draw("mesh1D")
    vertices = [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0],
                [1.5, 1.0], [2.0, 1.5], [0.5, 1.5]]
    simplicies = IndicesCollection(
        [[0, 1, 2], [1, 2, 3], [2, 3, 4], [2, 4, 5], [0, 2, 5]])

    mesh2D = Mesh(vertices, simplicies)
    print "2D mesh=", mesh2D
    mesh2D.draw().draw("mesh2D")
    mesh2D.draw2D().draw("mesh2D")

    vertices = NumericalSample(0, 3)

    vertices.add([0.0, 0.0, 0.0])
    vertices.add([0.0, 0.0, 1.0])
    vertices.add([0.0, 1.0, 0.0])
    vertices.add([0.0, 1.0, 1.0])
    vertices.add([1.0, 0.0, 0.0])
    vertices.add([1.0, 0.0, 1.0])
    vertices.add([1.0, 1.0, 0.0])
    vertices.add([1.0, 1.0, 1.0])

    simplicies = IndicesCollection(6, Indices(4))
    simplicies[0] = [0, 1, 2, 4]
    simplicies[1] = [3, 5, 6, 7]
    simplicies[2] = [1, 2, 3, 6]
    simplicies[3] = [1, 2, 4, 6]
    simplicies[4] = [1, 3, 5, 6]
    simplicies[5] = [1, 4, 5, 6]

    mesh3D = Mesh(vertices, simplicies)
    print "3D mesh=", mesh3D
    mesh3D.draw().draw("mesh3D")
    rotation = SquareMatrix(3)
    rotation[0, 0] = cos(pi / 3.0)
    rotation[0, 1] = sin(pi / 3.0)
    rotation[1, 0] = -sin(pi / 3.0)
    rotation[1, 1] = cos(pi / 3.0)
    rotation[2, 2] = 1.0
    mesh3D.draw3D(True, rotation, True).draw("mesh3D_2")

except:
    import sys
    print "t_Mesh_std.py", sys.exc_type, sys.exc_value
