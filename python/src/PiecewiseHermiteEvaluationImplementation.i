// SWIG file PiecewiseHermiteEvaluationImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (lun. 02 janv. 2012)

%{
#include "PiecewiseHermiteEvaluationImplementation.hxx"
%}

%include PiecewiseHermiteEvaluationImplementation.hxx

namespace OT { %extend PiecewiseHermiteEvaluationImplementation { PiecewiseHermiteEvaluationImplementation(const PiecewiseHermiteEvaluationImplementation & other) { return new OT::PiecewiseHermiteEvaluationImplementation(other); } } }
