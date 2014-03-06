// SWIG file GeneralizedPareto.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "GeneralizedPareto.hxx"
%}

%include GeneralizedPareto.hxx
namespace OT { %extend GeneralizedPareto { GeneralizedPareto(const GeneralizedPareto & other) { return new OT::GeneralizedPareto(other); } } }
