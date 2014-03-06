// SWIG file AbsoluteExponential.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "AbsoluteExponential.hxx"
%}

%include AbsoluteExponential.hxx
namespace OT { %extend AbsoluteExponential { AbsoluteExponential(const AbsoluteExponential & other) { return new OT::AbsoluteExponential(other); } } }
