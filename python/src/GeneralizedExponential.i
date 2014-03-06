// SWIG file GeneralizedExponential.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "GeneralizedExponential.hxx"
%}

%include GeneralizedExponential.hxx
namespace OT { %extend GeneralizedExponential { GeneralizedExponential(const GeneralizedExponential & other) { return new OT::GeneralizedExponential(other); } } }
