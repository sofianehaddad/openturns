// SWIG file SphericalModel.i
// @author schueller
// @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)

%{
#include "SphericalModel.hxx"
%}

%include SphericalModel.hxx
namespace OT { %extend SphericalModel { SphericalModel(const SphericalModel & other) { return new OT::SphericalModel(other); } } }
