// SWIG file BasisFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "BasisFactory.hxx"
%}

%include BasisFactory.hxx
namespace OT { %extend BasisFactory { BasisFactory(const BasisFactory & other) { return new OT::BasisFactory(other); } } }
