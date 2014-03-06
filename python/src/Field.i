// SWIG file Field.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Field.hxx"
%}

OTTypedInterfaceObjectHelper(Field)

%include Field.hxx
namespace OT { %extend Field { Field(const Field & other) { return new OT::Field(other); } } }
