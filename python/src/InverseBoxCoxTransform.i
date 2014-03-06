// SWIG file InverseBoxCoxTransform.i
// @author schueller
// @date   2012-01-04 12:26:21 +0100 (Wed, 04 Jan 2012)

%{
#include "InverseBoxCoxTransform.hxx"
%}

%include InverseBoxCoxTransform.hxx
namespace OT { %extend InverseBoxCoxTransform { InverseBoxCoxTransform(const InverseBoxCoxTransform & other) { return new OT::InverseBoxCoxTransform(other); } } }
