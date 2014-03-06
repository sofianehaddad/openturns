// SWIG file LinearBasisFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "LinearBasisFactory.hxx"
%}

%include LinearBasisFactory.hxx
namespace OT { %extend LinearBasisFactory { LinearBasisFactory(const LinearBasisFactory & other) { return new OT::LinearBasisFactory(other); } } }
