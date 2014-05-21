// SWIG file ParametricEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "ParametricEvaluationImplementation.hxx"
%}

%include ParametricEvaluationImplementation.hxx
namespace OT { %extend ParametricEvaluationImplementation { ParametricEvaluationImplementation(const ParametricEvaluationImplementation & other) { return new OT::ParametricEvaluationImplementation(other); } } }
