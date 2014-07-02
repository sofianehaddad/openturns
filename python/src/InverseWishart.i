// SWIG file InverseWishart.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "InverseWishart.hxx"
%}

%include InverseWishart.hxx
namespace OT { %extend InverseWishart { InverseWishart(const InverseWishart & other) { return new OT::InverseWishart(other); } } }
