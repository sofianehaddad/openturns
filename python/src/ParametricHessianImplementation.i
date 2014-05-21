// SWIG file ParametricHessianImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "ParametricHessianImplementation.hxx"
%}

%include ParametricHessianImplementation.hxx
namespace OT { %extend ParametricHessianImplementation { ParametricHessianImplementation(const ParametricHessianImplementation & other) { return new OT::ParametricHessianImplementation(other); } } }
