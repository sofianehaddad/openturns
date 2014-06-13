// SWIG file DistributionImplementation.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

// do not pass argument by reference, return it as tuple item
%typemap(in, numinputs=0) OT::NumericalSample & grid ($*ltype temp) %{ temp = OT::NumericalSample(); $1 = &temp; %}
%typemap(argout) OT::NumericalSample & grid %{ $result = SWIG_Python_AppendOutput($result, SWIG_NewPointerObj(new OT::NumericalSample(*$1), SWIG_TypeQuery("OT::NumericalSample *"), SWIG_POINTER_OWN |  0 )); %}
OT::NumericalSample OT::DistributionImplementation::computePDF(const OT::NumericalScalar xMin,
                           const OT::NumericalScalar xMax,
                           const OT::UnsignedInteger pointNumber,
                           OT::NumericalSample & grid) const;
OT::NumericalSample OT::DistributionImplementation::computePDF(const OT::NumericalPoint & xMin,
                           const OT::NumericalPoint & xMax,
                           const OT::Indices & pointNumber,
                           OT::NumericalSample & grid) const;

%typemap(in, numinputs=0) OT::NumericalPoint & weights ($*ltype temp) %{ temp = OT::NumericalPoint(); $1 = &temp; %}
%typemap(argout) OT::NumericalPoint & weights %{ $result = SWIG_Python_AppendOutput($result, SWIG_NewPointerObj(new OT::NumericalPoint(*$1), SWIG_TypeQuery("OT::NumericalPoint *"), SWIG_POINTER_OWN |  0 )); %}
OT::NumericalPoint OT::DistributionImplementation::getGaussNodesAndWeights(OT::NumericalPoint & weights) const;

%{
#include "DistributionImplementation.hxx"
%}

%include DistributionImplementation_doc.i

%include DistributionImplementation.hxx
namespace OT { %extend DistributionImplementation { DistributionImplementation(const DistributionImplementation & other) { return new OT::DistributionImplementation(other); } } }

%clear OT::NumericalPoint & weights;

