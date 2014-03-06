// SWIG file ResourceMap.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "ResourceMap.hxx"
%}

%template(StringStringMap) std::map<OT::String,OT::String>;

%nodefaultctor ResourceMap;

%include ResourceMap.hxx
