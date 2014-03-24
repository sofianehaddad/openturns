// SWIG file CompositeDistribution.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "CompositeDistribution.hxx"
%}

%include CompositeDistribution.hxx
namespace OT { %extend CompositeDistribution { CompositeDistribution(const CompositeDistribution & other) { return new OT::CompositeDistribution(other); } } }
