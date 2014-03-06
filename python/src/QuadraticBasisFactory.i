// SWIG file QuadraticBasisFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "QuadraticBasisFactory.hxx"
%}

%include QuadraticBasisFactory.hxx
namespace OT { %extend QuadraticBasisFactory { QuadraticBasisFactory(const QuadraticBasisFactory & other) { return new OT::QuadraticBasisFactory(other); } } }
