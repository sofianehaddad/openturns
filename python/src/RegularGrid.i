// SWIG file RegularGrid.i
// @author schueller
// @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)

%{
#include "PythonWrappingFunctions.hxx"
#include "RegularGrid.hxx"
%}

%include RegularGrid.hxx

namespace OT { %extend RegularGrid { RegularGrid(const RegularGrid & other) { return new OT::RegularGrid(other); } } }
