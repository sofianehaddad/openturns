// SWIG file GaussKronrodRule.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "GaussKronrodRule.hxx"
%}

%include GaussKronrodRule.hxx
namespace OT { %extend GaussKronrodRule { GaussKronrodRule(const GaussKronrodRule & other) { return new OT::GaussKronrodRule(other); } } }
