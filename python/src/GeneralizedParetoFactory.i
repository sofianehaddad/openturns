// SWIG file GeneralizedParetoFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "GeneralizedParetoFactory.hxx"
%}

%include GeneralizedParetoFactory.hxx
namespace OT { %extend GeneralizedParetoFactory { GeneralizedParetoFactory(const GeneralizedParetoFactory & other) { return new OT::GeneralizedParetoFactory(other); } } }
