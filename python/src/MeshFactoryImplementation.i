// SWIG file MeshFactoryImplementation.i
// Author : $LastChangedBy$
// Date : $LastChangedDate$
// Id : $Id$

%{
#include "MeshFactoryImplementation.hxx"
%}

%include MeshFactoryImplementation.hxx
namespace OT {%extend MeshFactoryImplementation {MeshFactoryImplementation(const MeshFactoryImplementation & other){return new OT::MeshFactoryImplementation(other);}}}
