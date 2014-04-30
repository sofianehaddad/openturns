// SWIG file KDTree.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "KDTree.hxx"
%}

%include KDTree.hxx
namespace OT { %extend KDTree { KDTree(const KDTree & other) { return new OT::KDTree(other); } } }
