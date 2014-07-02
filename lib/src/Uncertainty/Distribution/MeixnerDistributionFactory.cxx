//                                               -*- C++ -*-
/**
 *  @file  MeixnerDistributionFactory.cxx
 *  @brief Factory for MeixnerDistribution distribution
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#include "MeixnerDistributionFactory.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(MeixnerDistributionFactory);

/* Default constructor */
MeixnerDistributionFactory::MeixnerDistributionFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
MeixnerDistributionFactory * MeixnerDistributionFactory::clone() const
{
  return new MeixnerDistributionFactory(*this);
}


/* Here is the interface that all derived class must implement */

MeixnerDistributionFactory::Implementation MeixnerDistributionFactory::build(const NumericalSample & sample) const
{
  return buildAsMeixnerDistribution(sample).clone();
}

MeixnerDistributionFactory::Implementation MeixnerDistributionFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsMeixnerDistribution(parameters).clone();
}

MeixnerDistributionFactory::Implementation MeixnerDistributionFactory::build() const
{
  return buildAsMeixnerDistribution().clone();
}

MeixnerDistribution MeixnerDistributionFactory::buildAsMeixnerDistribution(const NumericalSample & sample) const
{
  UnsignedInteger size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a MeixnerDistribution distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a MeixnerDistribution distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  const NumericalScalar gamma1(sample.computeSkewnessPerComponent()[0]);
  const NumericalScalar gamma2(sample.computeKurtosisPerComponent()[0]);
  const NumericalScalar upperBound(3.0 + 2.0 * gamma1 * gamma1);
  if (gamma2 <= upperBound) throw InvalidArgumentException(HERE) << "Error: cannot estimate a MeixnerDistribution distribution if the sample kurtosis=" << gamma2 << " is not greater than 2*skewness^2+3=" << upperBound;
  const NumericalScalar m(sample.computeMean()[0]);
  const NumericalScalar s2(sample.computeVariancePerComponent()[0]);
  const NumericalScalar delta(1.0 / (gamma2 - gamma1 * gamma1 - 3.0));
  const NumericalScalar beta(((0.0 < gamma1) - (gamma1 < 0.0)) * std::acos(2.0 - delta * (gamma2 - 3.0)));
  const NumericalScalar alpha(cbrt(s2 * (std::cos(beta) + 1.0)));
  const NumericalScalar mu(m - alpha * delta * std::tan(0.5 * beta));
  MeixnerDistribution result(alpha, beta, delta, mu);
  result.setDescription(sample.getDescription());
  return result;
}

MeixnerDistribution MeixnerDistributionFactory::buildAsMeixnerDistribution(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsMeixnerDistribution(RemoveDescriptionFromCollection(parameters));
}

MeixnerDistribution MeixnerDistributionFactory::buildAsMeixnerDistribution(const NumericalPointCollection & parameters) const
{
  try
  {
    MeixnerDistribution distribution;
    distribution.setParametersCollection(parameters);
    return distribution;
  }
  catch (InvalidArgumentException & ex)
  {
    throw InvalidArgumentException(HERE) << "Error: cannot build a MeixnerDistribution distribution from the given parameters";
  }
}

MeixnerDistribution MeixnerDistributionFactory::buildAsMeixnerDistribution() const
{
  return MeixnerDistribution();
}

END_NAMESPACE_OPENTURNS
