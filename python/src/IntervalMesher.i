// SWIG file IntervalMesher.i
// Author : $LastChangedBy$
// Date : $LastChangedDate$
// Id : $Id$

%{
#include "IntervalMesher.hxx"
%}

%include IntervalMesher.hxx
namespace OT {%extend IntervalMesher {IntervalMesher(const IntervalMesher & other){return new OT::IntervalMesher(other);}}}
