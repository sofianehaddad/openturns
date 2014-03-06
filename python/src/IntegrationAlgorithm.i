// SWIG file IntegrationAlgorithm.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "IntegrationAlgorithm.hxx"
%}

OTTypedInterfaceObjectHelper(IntegrationAlgorithm)

%include IntegrationAlgorithm.hxx
namespace OT{ %extend IntegrationAlgorithm { IntegrationAlgorithm(const IntegrationAlgorithm & other) { return new OT::IntegrationAlgorithm(other); } } }
