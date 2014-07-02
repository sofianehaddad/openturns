//                                               -*- C++ -*-
/**
 *  @file  KFold.cxx
 *  @brief K-Fold cross validation
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
 *  @date   2010-11-09 13:44:00 +0100 (Tue, 09 Nov 2010)
 */

#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"
#include "ResourceMap.hxx"
#include "KFold.hxx"
#include "PenalizedLeastSquaresAlgorithm.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(KFold);


static Factory<KFold> RegisteredFactory("KFold");


/* Default constructor */
KFold::KFold( const UnsignedInteger k )
  : FittingAlgorithmImplementation()
{
  setK( k );
}

/* Virtual constructor */
KFold * KFold::clone() const
{
  return new KFold( *this );
}

/* String converter */
String KFold::__repr__() const
{
  return OSS() << "class=" << GetClassName();
}

/* Perform cross-validation */
NumericalScalar KFold::run(const NumericalSample & x,
                           const NumericalSample & y,
                           const Basis & basis) const
{
  const UnsignedInteger dimension( x.getDimension() );
  const UnsignedInteger sampleSize( x.getSize() );
  const NumericalScalar variance( y.computeVariancePerComponent()[0] );

  if ( y.getDimension() != 1 ) throw InvalidArgumentException( HERE ) << "Output sample should be unidimensional (dim=" << y.getDimension() << ").";
  if ( y.getSize() != sampleSize ) throw InvalidArgumentException( HERE ) << "Samples should be equally sized (in=" << sampleSize << " out=" << y.getSize() << ").";
  if ( k_ >= sampleSize ) throw InvalidArgumentException( HERE ) << "K (" << k_ << ") should be < size (" << sampleSize << ").";
  if ( variance <= 0.0 ) throw InvalidArgumentException( HERE ) << "Null output sample variance.";

  NumericalScalar quadraticResidual(0.0);

  // the size of a subsample
  const UnsignedInteger testSize( sampleSize / k_ );

  // We build the test sample by selecting one over k points of the given samples up to the test size, with a varying initial index
  // i is the initial index
  for ( UnsignedInteger i = 0; i < k_; ++ i )
  {
    LOGINFO(OSS() << "Sub-sample " << i << " over " << k_ - 1);
    // build training/test samples
    NumericalSample xTrain( 0, dimension );
    NumericalSample yTrain( 0, 1 );
    NumericalSample xTest( 0, dimension );
    NumericalSample yTest( 0, 1 );

    for ( UnsignedInteger j = 0; j < ( k_ * testSize ); ++ j )
    {
      if ( (j % k_) != i )
      {
        xTrain.add( x[j] );
        yTrain.add( y[j] );
      }
      else
      {
        xTest.add( x[j] );
        yTest.add( y[j] );
      }
    } // Partitioning loop

    // perform LS regression on the current basis
    LOGINFO("Solve current least-squares problem");
    PenalizedLeastSquaresAlgorithm penalizedLeastSquaresAlgorithm( xTrain, yTrain, basis );
    penalizedLeastSquaresAlgorithm.run();
    NumericalPoint coefficientsTrain( penalizedLeastSquaresAlgorithm.getCoefficients() );
    LOGINFO("Compute the residual");
    // evaluate on the test sample
    NumericalMathFunction metamodelTrain( basis, coefficientsTrain );
    NumericalSample yHatTest(metamodelTrain(xTest));
    // The empirical error is the normalized L2 error
    for ( UnsignedInteger j = 0; j < testSize; ++ j ) quadraticResidual += pow( yTest[j][0] - yHatTest[j][0], 2.0 );
    LOGINFO(OSS() << "Cumulated residual=" << quadraticResidual);
  }

  const NumericalScalar empiricalError( quadraticResidual / ( testSize * k_ ) );

  const NumericalScalar relativeError(empiricalError / variance);
  LOGINFO(OSS() << "Relative error=" << relativeError);
  return relativeError;
}


/* Method save() stores the object through the StorageManager */
void KFold::save(Advocate & adv) const
{
  FittingAlgorithmImplementation::save(adv);
  adv.saveAttribute( "k_", k_ );
}


/* Method load() reloads the object from the StorageManager */
void KFold::load(Advocate & adv)
{
  FittingAlgorithmImplementation::load(adv);
  adv.loadAttribute( "k_", k_ );
}


/* P accessor */
void KFold::setK(const UnsignedInteger k)
{
  if ( k < 1 )
    throw InvalidArgumentException( HERE ) << "KFold k parameter should be > 0";
  k_ = k;
}


UnsignedInteger KFold::getK() const
{
  return k_;
}


END_NAMESPACE_OPENTURNS
