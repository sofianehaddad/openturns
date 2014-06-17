// SWIG file OrthogonalUniVariatePolynomialFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

// do not pass argument by reference, return it as tuple item
%typemap(in, numinputs=0) OT::NumericalPoint & weights ($*ltype temp) %{ temp = OT::NumericalPoint(); $1 = &temp; %}
%typemap(argout) OT::NumericalPoint & weights %{ $result = SWIG_Python_AppendOutput($result, SWIG_NewPointerObj(new OT::NumericalPoint(*$1), SWIG_TypeQuery("OT::NumericalPoint *"), SWIG_POINTER_OWN |  0 )); %}
OT::NumericalPoint OT::OrthogonalUniVariatePolynomialFactory::getNodesAndWeights(const OT::UnsignedInteger n, OT::NumericalPoint & weights) const;

%{
#include "OrthogonalUniVariatePolynomialFactory.hxx"
%}

%include OrthogonalUniVariatePolynomialFactory_doc.i

%include OrthogonalUniVariatePolynomialFactory.hxx
namespace OT{ %extend OrthogonalUniVariatePolynomialFactory { OrthogonalUniVariatePolynomialFactory(const OrthogonalUniVariatePolynomialFactory & other) { return new OT::OrthogonalUniVariatePolynomialFactory(other); } } }

%clear OT::NumericalPoint & weights;

