// SWIG file ParametricGradientImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "ParametricGradientImplementation.hxx"
%}

%include ParametricGradientImplementation.hxx
namespace OT { %extend ParametricGradientImplementation { ParametricGradientImplementation(const ParametricGradientImplementation & other) { return new OT::ParametricGradientImplementation(other); } } }
