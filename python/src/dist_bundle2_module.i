// SWIG file dist_module.i
// @author schueller
// @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)

%module(package="openturns", docstring="Probabilistic distributions.") dist_bundle2
#pragma SWIG nowarn=302,509
//%feature("autodoc","1");

%{
#include "OTconfig.hxx"
#include "OTBase.hxx"
#include "OTModel.hxx"
#include "OTDistribution.hxx"
%}

%include typemaps.i
%include OTtypes.i
%include OTexceptions.i
%include std_vector.i

%import base_module.i
%include UncertaintyModelCopulaCollection.i
%import model_copula_module.i

/* Uncertainty/Distribution (except copulas) */
%include HistogramPair.i
%include Histogram.i
%include HistogramFactory.i
%include InverseNormal.i
%include InverseNormalFactory.i
%include KPermutationsDistribution.i
%include KernelMixture.i
%include KernelSmoothing.i
%include Laplace.i
%include LaplaceFactory.i
%include Logistic.i
%include LogisticFactory.i
%include LogNormal.i
%include LogNormalFactory.i
%include LogUniform.i
%include LogUniformFactory.i
%include MeixnerDistribution.i
%include MeixnerDistributionFactory.i
%include Mixture.i
%include Multinomial.i
%include MultinomialFactory.i
%include NegativeBinomial.i
%include NegativeBinomialFactory.i
%include NonCentralChiSquare.i
%include NonCentralStudent.i
%include Normal.i
%include NormalFactory.i

%include ConditionalDistribution.i
%include PosteriorDistribution.i

/* At last we include template definitions */
%include UncertaintyDistTemplateDefs.i
