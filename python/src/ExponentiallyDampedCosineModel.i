// SWIG file ExponentiallyDampedCosineModel.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "ExponentiallyDampedCosineModel.hxx"
%}

%include ExponentiallyDampedCosineModel.hxx
namespace OT { %extend ExponentiallyDampedCosineModel { ExponentiallyDampedCosineModel(const ExponentiallyDampedCosineModel & other) { return new OT::ExponentiallyDampedCosineModel(other); } } }
