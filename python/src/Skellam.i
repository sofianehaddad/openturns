// SWIG file Skellam.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Skellam.hxx"
%}

%include Skellam.hxx
namespace OT { %extend Skellam { Skellam(const Skellam & other) { return new OT::Skellam(other); } } }
