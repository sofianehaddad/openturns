// SWIG file OrthogonalProductPolynomialFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

// do not pass argument by reference, return it as tuple item
%typemap(in, numinputs=0) OT::NumericalPoint & weights ($*ltype temp) %{ temp = OT::NumericalPoint(); $1 = &temp; %}
%typemap(argout) OT::NumericalPoint & weights %{ $result = SWIG_Python_AppendOutput($result, SWIG_NewPointerObj(new OT::NumericalPoint(*$1), SWIG_TypeQuery("OT::NumericalPoint *"), SWIG_POINTER_OWN |  0 )); %}
OT::NumericalSample OT::OrthogonalProductPolynomialFactory::getNodesAndWeights(const Indices & degrees, OT::NumericalPoint & weights) const;

%{
#include "OrthogonalProductPolynomialFactory.hxx"
%}

%include OrthogonalProductPolynomialFactory.hxx
namespace OT{ %extend OrthogonalProductPolynomialFactory { OrthogonalProductPolynomialFactory(const OrthogonalProductPolynomialFactory & other) { return new OT::OrthogonalProductPolynomialFactory(other); } } }

%clear OT::NumericalPoint & weights;

