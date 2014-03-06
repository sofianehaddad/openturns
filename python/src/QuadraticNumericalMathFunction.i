// SWIG file QuadraticNumericalMathFunction.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "QuadraticNumericalMathFunction.hxx"
%}

%include QuadraticNumericalMathFunction.hxx
namespace OT { %extend QuadraticNumericalMathFunction { QuadraticNumericalMathFunction(const QuadraticNumericalMathFunction & other) { return new OT::QuadraticNumericalMathFunction(other); } } }
