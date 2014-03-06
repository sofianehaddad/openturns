// SWIG file ProcessSamples.i
// @author dutka
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "ProcessSample.hxx"
%}

%include ProcessSample.hxx


namespace OT {  
%extend ProcessSample {

ProcessSample(const ProcessSample & other)
{
  return new OT::ProcessSample(other);
}

}

}

