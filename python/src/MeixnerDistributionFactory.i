// SWIG file MeixnerDistributionFactory.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "MeixnerDistributionFactory.hxx"
%}

%include MeixnerDistributionFactory.hxx
namespace OT { %extend MeixnerDistributionFactory { MeixnerDistributionFactory(const MeixnerDistributionFactory & other) { return new OT::MeixnerDistributionFactory(other); } } }
