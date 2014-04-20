// SWIG file Wishart.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Wishart.hxx"
%}

%include Wishart.hxx
namespace OT { %extend Wishart { Wishart(const Wishart & other) { return new OT::Wishart(other); } } }
