// SWIG file SquareMatrix.i
// @author schueller
// @date   2012-02-15 18:09:50 +0100 (Wed, 15 Feb 2012)

// do not pass argument by reference, return it as tuple item
%typemap(in, numinputs=0) OT::SquareComplexMatrix & v ($*ltype temp) %{ temp = OT::SquareComplexMatrix(); $1 = &temp; %}
%typemap(argout) OT::SquareComplexMatrix & v %{ $result = SWIG_Python_AppendOutput($result, SWIG_NewPointerObj(new OT::SquareComplexMatrix(*$1), SWIG_TypeQuery("OT::SquareComplexMatrix *"), SWIG_POINTER_OWN |  0 )); %}
OT::SquareMatrix OT::SquareMatrix::computeEVD(OT::SquareComplexMatrix & v, const Bool keepIntact = true);

%{
#include "SquareMatrix.hxx"
%}

%include SquareMatrix_doc.i

%apply const NumericalScalarCollection & { const OT::SquareMatrix::NumericalScalarCollection & };

%rename(__pow__) OT::SquareMatrix::power(const UnsignedInteger n) const;

%include SquareMatrix.hxx

namespace OT {  

%extend SquareMatrix {

  SquareMatrix(const SquareMatrix & other) { return new OT::SquareMatrix(other); }

  SquareMatrix(PyObject * pyObj) { return new OT::SquareMatrix( OT::convert<OT::_PySequence_,OT::SquareMatrix>(pyObj) ); }

  OTMatrixAccessors()
      
  SquareMatrix __truediv__(NumericalScalar s) { return (*self) / s; }

} // SquareMatrix
} // OT
