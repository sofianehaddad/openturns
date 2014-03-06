// SWIG file KrigingAlgorithm.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "KrigingAlgorithm.hxx"
%}

%include KrigingAlgorithm.hxx

namespace OT{ %extend KrigingAlgorithm { KrigingAlgorithm(const KrigingAlgorithm & other) { return new OT::KrigingAlgorithm(other); } } }
