// SWIG file TriangularMatrix.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "TriangularMatrix.hxx"
%}

%include TriangularMatrix.hxx

namespace OT { 

%extend TriangularMatrix {

  TriangularMatrix(const TriangularMatrix & other) { return new OT::TriangularMatrix(other); }

  TriangularMatrix(PyObject * pyObj) { return new OT::TriangularMatrix( OT::convert<OT::_PySequence_,OT::TriangularMatrix>(pyObj) ); }

  OTMatrixAccessors(TriangularMatrix, NumericalScalar)

  TriangularMatrix __rmul__(NumericalScalar s) { return s * (*self); }
  
  TriangularMatrix __truediv__(NumericalScalar s) { return (*self) / s; }
  
  } // TriangularMatrix
} // OT
