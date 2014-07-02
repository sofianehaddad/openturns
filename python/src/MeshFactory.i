// SWIG file CovarianceModelFactory.i
// Author : $LastChangedBy$
// Date : $LastChangedDate$
// Id : $Id$

%{
#include "MeshFactory.hxx"
%}

OTTypedInterfaceObjectHelper(MeshFactory)

%include MeshFactory.hxx

namespace OT { 

  %extend MeshFactory {

     MeshFactory(const MeshFactory & other) { return new OT::MeshFactory(other); }

   }
 }
