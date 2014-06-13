// SWIG file OrthogonalUniVariatePolynomialFamily.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

// do not pass argument by reference, return it as tuple item
%typemap(in, numinputs=0) OT::NumericalPoint & weights ($*ltype temp) %{ temp = OT::NumericalPoint(); $1 = &temp; %}
%typemap(argout) OT::NumericalPoint & weights %{ $result = SWIG_Python_AppendOutput($result, SWIG_NewPointerObj(new OT::NumericalPoint(*$1), SWIG_TypeQuery("OT::NumericalPoint *"), SWIG_POINTER_OWN |  0 )); %}
OT::NumericalPoint OT::OrthogonalUniVariatePolynomialFamily::getNodesAndWeights(OT::NumericalPoint & weights) const;

%{
#include "OrthogonalUniVariatePolynomialFamily.hxx"
%}

%include OrthogonalUniVariatePolynomialFamily_doc.i

OTDefaultCollectionConvertFunctionsMisnamed(OrthogonalUniVariatePolynomialFamily, OrthogonalUniVariatePolynomialFactory)

OTTypedInterfaceObjectImplementationHelper(OrthogonalUniVariatePolynomialFamily, OrthogonalUniVariatePolynomialFactory)
OTTypedCollectionInterfaceObjectMisnamedHelper(OrthogonalUniVariatePolynomialFamily, PolynomialFamilyCollection)

%include OrthogonalUniVariatePolynomialFamily.hxx

namespace OT{  

%extend OrthogonalUniVariatePolynomialFamily {

OrthogonalUniVariatePolynomialFamily(const OrthogonalUniVariatePolynomialFamily & other) { return new OT::OrthogonalUniVariatePolynomialFamily(other); }

}

}

%clear OT::NumericalPoint & weights;
