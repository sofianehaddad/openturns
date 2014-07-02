//                                               -*- C++ -*-
/**
 *  @file  TemporalNormalProcess.cxx
 *  @brief A class which implements the TemporalNormalProcess process
 *
 *  Copyright (C) 2005-2014 Airbus-EDF-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author schueller
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */

#include "TemporalNormalProcess.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"
#include "Log.hxx"
#include "CovarianceMatrix.hxx"
#include "DistFunc.hxx"
#include "TriangularMatrix.hxx"
#include "RandomGenerator.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(TemporalNormalProcess);

static Factory<TemporalNormalProcess> RegisteredFactory("TemporalNormalProcess");

TemporalNormalProcess::TemporalNormalProcess()
  : ProcessImplementation()
  , covarianceModel_()
  , choleskyFactorCovarianceMatrix_(0)
  , isInitialized_(false)
  , hasStationaryTrend_(true)
  , checkedStationaryTrend_(true)
  , trend_()
  , stationaryTrendValue_(0)
{
  // Nothing to do
}

/* Standard constructor  */
TemporalNormalProcess::TemporalNormalProcess(const TrendTransform & trend,
    const SecondOrderModel & model,
    const Mesh & mesh)
  : ProcessImplementation()
  , covarianceModel_(model.getCovarianceModel())
  , choleskyFactorCovarianceMatrix_(0)
  , isInitialized_(false)
  , hasStationaryTrend_(false)
  , checkedStationaryTrend_(false)
  , trend_(trend)
  , stationaryTrendValue_(trend.getOutputDimension())
{
  if (trend.getOutputDimension() != model.getDimension()) throw InvalidArgumentException(HERE) << "Error: the given trend has an output dimension=" << trend.getOutputDimension() << " different from the second order model dimension=" << model.getDimension();
  setMesh(mesh);
  setDimension(model.getDimension());
}

/* Standard constructor  */
TemporalNormalProcess::TemporalNormalProcess(const TrendTransform & trend,
    const CovarianceModel & covarianceModel,
    const Mesh & mesh)
  : ProcessImplementation()
  , covarianceModel_(covarianceModel)
  , choleskyFactorCovarianceMatrix_(0)
  , isInitialized_(false)
  , hasStationaryTrend_(false)
  , checkedStationaryTrend_(false)
  , trend_(trend)
  , stationaryTrendValue_(trend.getOutputDimension())
{
  if (trend.getOutputDimension() != covarianceModel.getDimension()) throw InvalidArgumentException(HERE) << "Error: the given trend has an output dimension=" << trend.getOutputDimension() << " different from the covariance model dimension=" << covarianceModel.getDimension();
  setMesh(mesh);
  setDimension(covarianceModel.getDimension());
}

/* Standard constructor  */
TemporalNormalProcess::TemporalNormalProcess(const SecondOrderModel & model,
    const Mesh & mesh)
  : ProcessImplementation()
  , covarianceModel_(model.getCovarianceModel())
  , choleskyFactorCovarianceMatrix_(0)
  , isInitialized_(false)
  , hasStationaryTrend_(true)
  , checkedStationaryTrend_(true)
  , trend_()
  , stationaryTrendValue_(model.getDimension())
{
  setMesh(mesh);
  setDimension(model.getDimension());
  trend_ = TrendTransform(NumericalMathFunction(Description::BuildDefault(mesh.getDimension(), "x"), Description(getDimension(), "0.0")));
}

/* Standard constructor  */
TemporalNormalProcess::TemporalNormalProcess(const CovarianceModel & covarianceModel,
    const Mesh & mesh)
  : ProcessImplementation()
  , covarianceModel_(covarianceModel)
  , choleskyFactorCovarianceMatrix_(0)
  , isInitialized_(false)
  , hasStationaryTrend_(true)
  , checkedStationaryTrend_(true)
  , trend_()
  , stationaryTrendValue_(covarianceModel.getDimension())
{
  setMesh(mesh);
  setDimension(covarianceModel.getDimension());
  trend_ = TrendTransform(NumericalMathFunction(Description::BuildDefault(mesh.getDimension(), "x"), Description(getDimension(), "0.0")));
}

/* Virtual constructor */
TemporalNormalProcess * TemporalNormalProcess::clone() const
{
  return new TemporalNormalProcess(*this);
}

void TemporalNormalProcess::initialize() const
{
  // Initialization of the process
  // Get the covariance matrix (its Cholesky factor)
  LOGINFO(OSS() << "Discretize the covariance model");
  CovarianceMatrix covarianceMatrix(covarianceModel_.discretize(mesh_));
  const UnsignedInteger fullSize(covarianceMatrix.getDimension());
  // Boolean flag to tell if the regularization is enough
  Bool continuationCondition(true);
  // Scaling factor of the matrix : M-> M + \lambda I with \lambda very small
  // The regularization is needed for fast decreasing covariance models
  const NumericalScalar startingScaling(ResourceMap::GetAsNumericalScalar("TemporalNormalProcess-StartingScaling"));
  const NumericalScalar maximalScaling(ResourceMap::GetAsNumericalScalar("TemporalNormalProcess-MaximalScaling"));
  NumericalScalar cumulatedScaling(0.0);

  NumericalScalar scaling(startingScaling);
  while (continuationCondition && (cumulatedScaling < maximalScaling))
  {
    try
    {
      cumulatedScaling += scaling ;
      // Unroll the regularization to optimize the computation
      for (UnsignedInteger i = 0; i < fullSize; ++i) covarianceMatrix(i, i) += scaling;
      LOGINFO(OSS() << "Factor the covariance matrix");
      choleskyFactorCovarianceMatrix_ = covarianceMatrix.computeCholesky();
      continuationCondition = false;
    }
    // If it has not yet been computed, compute it and store it
    catch (InternalException & ex)
    {
      scaling *= 2.0;
      LOGWARN(OSS() << "Must regularize the covariance matrix, factor=" << cumulatedScaling);
    }
  }

  if (scaling >= maximalScaling)
    throw InvalidArgumentException(HERE) << "Error; Could not compute the Cholesky factor"
                                         << " Scaling up to "  << cumulatedScaling << " was not enough";

  if (cumulatedScaling > 0.0)
    LOGWARN(OSS() <<  "Warning! Scaling up to "  << cumulatedScaling << " was needed in order to get an admissible covariance. ");

  // The process has been initialized
  isInitialized_ = true;
}

/* String converter */
String TemporalNormalProcess::__repr__() const
{
  OSS oss(true);
  oss << "class=" << TemporalNormalProcess::GetClassName();
  oss << " mesh=" << mesh_
      << " trend=" << trend_
      << " covarianceModel=" << covarianceModel_
      << " choleskyFactorCovarianceMatrix=" << choleskyFactorCovarianceMatrix_
      << " isInitialized=" << isInitialized_
      << " hasStationaryTrend=" << hasStationaryTrend_
      << " checkedStationaryTrend=" << checkedStationaryTrend_;
  return oss;
}

String TemporalNormalProcess::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "TemporalNormalProcess("
      << " dimension=" << dimension_
      << " mesh=\n" << mesh_.__str__(offset)
      << " trend=\n" << trend_.__str__(offset)
      << " covarianceModel=" << covarianceModel_.__str__(offset)
      << "\n)";
  return oss;
}

/* Mesh accessor */
void TemporalNormalProcess::setMesh(const Mesh & mesh)
{
  checkedStationaryTrend_ = false;
  ProcessImplementation::setMesh(mesh);
}

/* Realization generator */
Field TemporalNormalProcess::getRealization() const
{
  NumericalSample values;
  if ((getDimension() == 1) && ResourceMap::GetAsUnsignedInteger("TemporalNormalProcess-UseGibbsSampler") == 1) values = getRealizationGibbs();
  else values = getRealizationCholesky();
  // If constant trend
  if (isTrendStationary())
  {
    // If zero trend
    if (stationaryTrendValue_.norm() == 0.0) return Field(mesh_, values);
    // If nonzero trend
    return Field(mesh_, values + stationaryTrendValue_);
  }
  // else apply the trend
  return trend_(Field(mesh_, values));
}

NumericalSample TemporalNormalProcess::getRealizationGibbs() const
{
  const NumericalSample vertices(getMesh().getVertices());
  const UnsignedInteger size(vertices.getSize());
  const UnsignedInteger nMax(std::max(static_cast<UnsignedInteger>(1), ResourceMap::GetAsUnsignedInteger("TemporalNormalProcess-GibbsMaximumIteration")));

  NumericalSample values(size, 1);
  NumericalPoint diagonal(size);
  for (UnsignedInteger n = 0; n < nMax; ++n)
  {
    for (UnsignedInteger i = 0; i < size; ++i)
    {
      // Here we work on the normalized covariance, ie the correlation
      NumericalSample covarianceRow(covarianceModel_.discretizeRow(vertices, i));
      diagonal[i] = covarianceRow[i][0];
      const NumericalPoint delta(1, (DistFunc::rNormal() - values[i][0]) / diagonal[i]);
      values += covarianceRow * delta;
    }
    LOGINFO(OSS() << "Gibbs sampler - iteration " << n + 1 << " over " << nMax);
  }
  // We have to rescale the realization
  for (UnsignedInteger i = 0; i < size; ++i) values[i] *= diagonal[i];
  return values;
}

NumericalSample TemporalNormalProcess::getRealizationCholesky() const
{
  if (!isInitialized_) initialize();
  // Constantes values
  const UnsignedInteger size(getMesh().getVerticesNumber());
  const UnsignedInteger fullSize(choleskyFactorCovarianceMatrix_.getDimension());
  NumericalPoint gaussianPoint(fullSize);
  // N gaussian realizations
  for (UnsignedInteger index = 0; index < fullSize; ++index) gaussianPoint[index] = DistFunc::rNormal();

  gaussianPoint = choleskyFactorCovarianceMatrix_ * gaussianPoint;

  NumericalSample values(size, dimension_);
  values.getImplementation()->setData(gaussianPoint);
  return values;
}

/* Covariance model accessor */
CovarianceModel TemporalNormalProcess::getCovarianceModel() const
{
  return covarianceModel_;
}

/* Trend accessor */
TrendTransform TemporalNormalProcess::getTrend() const
{
  return trend_;
}

/* Check if the process is stationary */
Bool TemporalNormalProcess::isStationary() const
{
  return covarianceModel_.isStationary() && isTrendStationary();
}

/* Tell if the process is trend stationary */
Bool TemporalNormalProcess::isTrendStationary() const
{
  if (!checkedStationaryTrend_) checkStationaryTrend();
  return hasStationaryTrend_;
}

/* Check if the process is trend stationary */
void TemporalNormalProcess::checkStationaryTrend() const
{
  hasStationaryTrend_ = true;
  checkedStationaryTrend_ = true;
  const UnsignedInteger n(mesh_.getVerticesNumber());
  if (n == 0) return;
  stationaryTrendValue_ = (*trend_.getEvaluation())(mesh_.getVertices()[0]);
  for (UnsignedInteger i = 1; i < n; ++i)
  {
    if ((*trend_.getEvaluation())(mesh_.getVertices()[i]) != stationaryTrendValue_)
    {
      hasStationaryTrend_ = false;
      return;
    }
  }
  return;
}

/* Check if the process is Normal */
Bool TemporalNormalProcess::isNormal() const
{
  return true;
}


/* Method save() stores the object through the StorageManager */
void TemporalNormalProcess::save(Advocate & adv) const
{
  ProcessImplementation::save(adv);
  adv.saveAttribute("covarianceModel_", covarianceModel_);
  adv.saveAttribute("choleskyFactorCovarianceMatrix_", choleskyFactorCovarianceMatrix_);
  adv.saveAttribute("isInitialized_", isInitialized_);
  adv.saveAttribute("hasStationaryTrend_", hasStationaryTrend_);
  adv.saveAttribute("checkedStationaryTrend_", checkedStationaryTrend_);
  adv.saveAttribute("trend_", trend_);
  adv.saveAttribute("stationaryTrendValue_", stationaryTrendValue_);
}

/* Method load() reloads the object from the StorageManager */
void TemporalNormalProcess::load(Advocate & adv)
{
  ProcessImplementation::load(adv);
  adv.loadAttribute("covarianceModel_", covarianceModel_);
  adv.loadAttribute("choleskyFactorCovarianceMatrix_", choleskyFactorCovarianceMatrix_);
  adv.loadAttribute("isInitialized_", isInitialized_);
  adv.loadAttribute("hasStationaryTrend_", hasStationaryTrend_);
  adv.loadAttribute("checkedStationaryTrend_", checkedStationaryTrend_);
  adv.loadAttribute("trend_", trend_);
  adv.loadAttribute("stationaryTrendValue_", stationaryTrendValue_);
}

END_NAMESPACE_OPENTURNS
