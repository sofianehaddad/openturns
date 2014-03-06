// SWIG file PiecewiseLinearEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "PiecewiseLinearEvaluationImplementation.hxx"
%}

%include PiecewiseLinearEvaluationImplementation.hxx

namespace OT { %extend PiecewiseLinearEvaluationImplementation { PiecewiseLinearEvaluationImplementation(const PiecewiseLinearEvaluationImplementation & other) { return new OT::PiecewiseLinearEvaluationImplementation(other); } } }
