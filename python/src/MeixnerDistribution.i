// SWIG file MeixnerDistribution.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "MeixnerDistribution.hxx"
%}

%include MeixnerDistribution.hxx
namespace OT { %extend MeixnerDistribution { MeixnerDistribution(const MeixnerDistribution & other) { return new OT::MeixnerDistribution(other); } } }
