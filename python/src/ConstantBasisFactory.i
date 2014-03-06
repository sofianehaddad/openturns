// SWIG file ConstantBasisFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ConstantBasisFactory.hxx"
%}

%include ConstantBasisFactory.hxx
namespace OT { %extend ConstantBasisFactory { ConstantBasisFactory(const ConstantBasisFactory & other) { return new OT::ConstantBasisFactory(other); } } }
