// SWIG file SkellamFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "SkellamFactory.hxx"
%}

%include SkellamFactory.hxx
namespace OT { %extend SkellamFactory { SkellamFactory(const SkellamFactory & other) { return new OT::SkellamFactory(other); } } }
