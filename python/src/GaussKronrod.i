// SWIG file GaussKronrod.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "GaussKronrod.hxx"
%}

%include GaussKronrod.hxx
namespace OT { %extend GaussKronrod { GaussKronrod(const GaussKronrod & other) { return new OT::GaussKronrod(other); } } }
