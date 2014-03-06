// SWIG file SquaredExponential.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SquaredExponential.hxx"
%}

%include SquaredExponential.hxx
namespace OT { %extend SquaredExponential { SquaredExponential(const SquaredExponential & other) { return new OT::SquaredExponential(other); } } }
