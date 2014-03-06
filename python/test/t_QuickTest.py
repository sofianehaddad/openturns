#! /usr/bin/env python

import os

width = 32

# check that python can load OpenTURNS module
print '1: Python module load'.ljust(width),
try:
    import openturns as ot
    print 'OK'
except:
    print 'no'

# check that python can find the Viewer module
# If it fails, check that matplotlib package is installed
print '2: Viewer matplotlib'.ljust(width),
try:
    import openturns.viewer
    print 'OK'
except:
    print 'no'

# check that OpenTURNS can run R
# It should produce two files named testDraw.png and testDraw.eps
print '3: R'.ljust(width),
try:
    graph = ot.Normal().drawPDF()
    fname = 'testDraw'
    try:
        graph.draw(fname, 640, 480, ot.GraphImplementation.PNG)
        os.remove(fname + '.png')
    except:
        raise
    print 'OK'
except:
    print 'no'

# check that rot package is installed
print '4: R.rot'.ljust(width),
try:
    lm = ot.LinearModelFactory().build(
        ot.Normal(2).getSample(10), ot.Normal().getSample(10))
    print 'OK'
except:
    print 'no'

# check XML support
print '5: XML'.ljust(width),
try:
    storageManager = ot.XMLStorageManager('myFile.xml')
    print 'OK'
except:
    print 'no'

# check that the sample wrappers are accessible
print '6: compiled wrappers'.ljust(width),
try:
    f = ot.NumericalMathFunction('poutre')
    print 'OK'
except:
    print 'no'

# check that analytical function are available
print '7: analytical function'.ljust(width),
try:
    f = ot.NumericalMathFunction(['x1', 'x2'], ['y'], ['x1+x2'])
    print 'OK'
except:
    print 'no'
