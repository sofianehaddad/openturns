// SWIG file Polygon.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Polygon.hxx"
%}

%include Polygon.hxx
namespace OT { %extend Polygon { Polygon(const Polygon & other) { return new OT::Polygon(other); } } }
