// SWIG file IntegrationAlgorithmImplementation.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "IntegrationAlgorithmImplementation.hxx"
%}

%include IntegrationAlgorithmImplementation.hxx
namespace OT { %extend IntegrationAlgorithmImplementation { IntegrationAlgorithmImplementation(const IntegrationAlgorithmImplementation & other) { return new OT::IntegrationAlgorithmImplementation(other); } } }
