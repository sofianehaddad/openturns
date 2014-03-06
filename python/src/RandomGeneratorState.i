// SWIG file RandomGeneratorState.i
//  @author ladier
//  @date   2013-11-27 15:35:43 +0100 (Wed, 27 Nov 2013)

%{
#include "RandomGeneratorState.hxx"
%}

%include RandomGeneratorState.hxx


namespace OT {
%extend RandomGeneratorState {
  RandomGeneratorState(const RandomGeneratorState & other) { return new OT::RandomGeneratorState(other); }
  Bool __eq__(const RandomGeneratorState & other) { return (*self) == other; }

} //RandomGeneratorState
} //OT