// SWIG file geom_module.i
// @author schueller
// @date   2012-01-04 12:26:21 +0100 (Wed, 04 Jan 2012)

%module(package="openturns", docstring="Geometrical classes.") geom
%feature("autodoc","1");

%{
#include "OTconfig.hxx"
#include "OTCommon.hxx"
#include "OTGeom.hxx"
#include "OTStat.hxx"
#include "OTType.hxx"
%}

%include typemaps.i
%include OTtypes.i
%include OTexceptions.i
%include std_vector.i

/* Base/Common */
%import common_module.i

/* Wrapper */
%import wrapper_module.i
%import BaseWrapperTemplateDefs.i

/* Base/Type */
%import typ_module.i
%import BaseTypTemplateDefs.i

/* Base/Graph */
%import graph_module.i
%import BaseGraphTemplateDefs.i

/* Base/Func */
%import func_module.i
%import BaseFuncTemplateDefs.i

/* Base/Geom */
%include DomainImplementation.i
%include Domain.i
%include LevelSet.i
%include Mesh.i
%include RegularGrid.i
%include MeshFactoryImplementation.i
%include MeshFactory.i
%include IntervalMesher.i
