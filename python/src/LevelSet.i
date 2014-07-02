// SWIG file LevelSet.i
// @author schueller
// @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)

%{
#include "LevelSet.hxx"
%}

%include LevelSet.hxx

namespace OT { %extend LevelSet { LevelSet(const LevelSet & other) { return new OT::LevelSet(other); } } }
