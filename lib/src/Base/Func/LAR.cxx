//                                               -*- C++ -*-
/**
 *  @file  LAR.cxx
 *  @brief Least Angle Regression Algorithm
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

#include "PersistentObjectFactory.hxx"
#include "BasisSequenceFactoryImplementation.hxx"
#include "LAR.hxx"
#include "PenalizedLeastSquaresAlgorithm.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(LAR);

typedef Collection<NumericalScalar>    NumericalScalarCollection;

static Factory<LAR> RegisteredFactory("LAR");

/* Default constructor */
LAR::LAR(const Bool verbose)
  : BasisSequenceFactoryImplementation(verbose)
{
  // Nothing to do
}

/* Virtual constructor */
LAR * LAR::clone() const
{
  return new LAR( *this );
}

/* Method to create new BasisSequence objects */
BasisSequence LAR::build(const NumericalSample & x,
                         const NumericalSample & y,
                         const Basis & psi) const
{
  const UnsignedInteger sampleSize( x.getSize() );
  const UnsignedInteger basisSize( psi.getSize() );

  if ( sampleSize == 0 ) throw InvalidArgumentException( HERE ) << "Output sample cannot be empty.";
  if ( y.getDimension() != 1 ) throw InvalidArgumentException( HERE ) << "Output sample should be unidimensional (dim=" << y.getDimension() << ").";
  if ( y.getSize() != sampleSize ) throw InvalidArgumentException( HERE ) << "Samples should be equally sized (in=" << sampleSize << " out=" << y.getSize() << ").";
  if ( x.getDimension() != psi.getDimension() ) throw InvalidArgumentException( HERE ) << "Sample dimension (" << x.getDimension() << ") does not match basis dimension (" << psi.getDimension() << ").";

  BasisSequence result( psi );

  // get y as as point
  NumericalPoint mY( sampleSize );
  for ( UnsignedInteger j = 0; j < sampleSize; ++ j ) mY[j] = y[j][0];

  // precompute the Gram matrix
  Matrix mPsiX( sampleSize, basisSize );
  for ( UnsignedInteger i = 0; i < sampleSize; ++ i )
    for ( UnsignedInteger j = 0; j < basisSize; ++ j )
      mPsiX(i, j) = psi[j](x[i])[0];

  // regression coefficients
  NumericalPoint coefficients( basisSize );

  // current least-square state
  NumericalPoint mu( sampleSize );

  // list of indices of the active set
  Indices predictors(0);


  // main loop
  NumericalScalar oldCoefficientsL1Norm( 0.0 );
  NumericalScalar coefficientsL1Norm( 0.0 );
  NumericalScalar relativeConvergence( 1.0);
  Matrix squareRootGramMatrix;

  const UnsignedInteger maximumNumberOfIterations(std::min( basisSize, sampleSize - 1 ));
  UnsignedInteger iterations(0);

  do
  {
    // find the predictor most correlated with the current residual
    const NumericalPoint c( mPsiX.transpose() * ( mY - mu ));
    UnsignedInteger candidatePredictor(0);
    NumericalScalar cMax(-1.0);
    for ( UnsignedInteger j = 0; j < basisSize; ++ j )
      if ( ! predictors.__contains__(j) )
      {
        NumericalScalar cAbs(fabs( c[j] ));
        if ( cAbs > cMax )
        {
          cMax = cAbs;
          candidatePredictor = j;
        }
      } // if

    if ( getVerbose() ) LOGINFO( OSS() << "predictor=" << candidatePredictor << " residual=" << cMax );

    // add the predictor index
    predictors.add( candidatePredictor );
    // Starting from here, predictors has a size at least equal to 1
    // store the sign of the correlation
    UnsignedInteger predictorsSize(predictors.getSize());
    NumericalPoint s( predictorsSize );
    for ( UnsignedInteger j = 0; j < predictorsSize; ++ j ) s[j] = (c[predictors[j]] < 0.0 ? -1.0 : 1.0);

    // store correlations of the inactive set
    NumericalPoint cI;
    for ( UnsignedInteger j = 0; j < basisSize; ++ j )
      if ( ! predictors.__contains__(j) )
        cI.add( c[j] );

    // matrix of elements of the inactive set
    // Here, basisSize >= predictorsSize as we perform a maximum
    // of min(basisSize, sampleSize-1) iterations and predictorsSize <= sampleSize
    Matrix mPsiAc ( sampleSize, basisSize - predictorsSize );
    UnsignedInteger acIndex(0);
    for ( UnsignedInteger j = 0; j < basisSize; ++ j )
      if ( ! predictors.__contains__(j) )
      {
        for ( UnsignedInteger i = 0; i < sampleSize; ++ i )
        {
          mPsiAc(i, acIndex) = mPsiX(i, j);
        }
        ++ acIndex;
      } // if

    if ( getVerbose() ) LOGINFO( OSS() << "matrix of elements of the inactive set built.");

    Matrix mPsiAk( sampleSize, predictorsSize - 1 );
    for ( UnsignedInteger i = 0; i < sampleSize; ++ i )
      for ( UnsignedInteger j = 0; j < predictorsSize - 1; ++ j )
        mPsiAk(i, j) = mPsiX(i, predictors[j]);

    if ( getVerbose() ) LOGINFO( OSS() << "matrix of elements of the active set built.");

    // update the cholesky decomposition of the Gram matrix
    Matrix xk( sampleSize, 1 );
    NumericalScalar diagk( 0.0 );
    for ( UnsignedInteger i = 0; i < sampleSize; ++ i )
    {
      xk(i, 0) = mPsiX(i, candidatePredictor);
      diagk += pow( xk(i, 0), 2.0 );
    }
    if ( squareRootGramMatrix.getNbRows() > 0 )
    {
      // solve upper triangular system R*rk=xk'*A to get the extra column
      NumericalPoint colk( ( (xk.transpose() * mPsiAk).transpose()) * NumericalPoint( 1, 1.0 ) ) ;
      NumericalPoint rk( predictorsSize - 1, 0.0 );
      for (UnsignedInteger i = 0; i < predictorsSize - 1; ++ i)
      {
        NumericalScalar sum( colk[i] );
        for ( UnsignedInteger j = 0; j < i; ++ j )
          sum -= squareRootGramMatrix( j, i ) * rk[j];
        rk[i] = sum / squareRootGramMatrix( i, i );
      }

      // the extra diagonal term
      const NumericalScalar rkk( sqrt( diagk - dot( rk, rk ) ) );

      // reconstitute the whole decomposition matrix
      SquareMatrix newSquareRootGramMatrix( predictorsSize );
      for ( UnsignedInteger i = 0; i < predictorsSize - 1; ++ i )
        for ( UnsignedInteger j = 0; j < predictorsSize - 1; ++ j )
          newSquareRootGramMatrix(i, j) = squareRootGramMatrix( i, j );
      for ( UnsignedInteger i = 0; i < predictorsSize - 1; ++ i )
        newSquareRootGramMatrix(i, predictorsSize - 1 ) = rk[i];
      newSquareRootGramMatrix( predictorsSize - 1, predictorsSize - 1 ) = rkk;
      squareRootGramMatrix = newSquareRootGramMatrix;
    } // if ( squareRootGramMatrix.getNbRows() > 0 )
    else squareRootGramMatrix = SquareMatrix( 1, NumericalScalarCollection( 1, sqrt( diagk ) ) );

    if ( getVerbose() ) LOGINFO( OSS() << "Cholesky factor updated.");

    // compute ga1 = AA'^-1*s using the cholesky decomposition
    NumericalPoint ga1( predictorsSize );
    for ( UnsignedInteger i = 0; i < predictorsSize; ++ i )
    {
      NumericalScalar sum( s[i] );
      for ( UnsignedInteger j = 0; j < i; ++ j ) sum -= squareRootGramMatrix( j, i ) * ga1[j];
      ga1[i] = sum / squareRootGramMatrix( i, i );
    }
    for ( SignedInteger i = predictorsSize - 1; i >= 0; -- i )
    {
      NumericalScalar sum( ga1[i] );
      for ( UnsignedInteger j = i + 1; j < predictorsSize; ++ j )
        sum -= squareRootGramMatrix( i, j ) * ga1[j];
      ga1[i] = sum / squareRootGramMatrix( i, i );
    }

    if ( getVerbose() ) LOGINFO( OSS() << "Solved normal equation.");

    // normalization coefficient
    NumericalScalar cNorm( 1.0 / sqrt( dot( s, ga1 ) ) );

    // update the Matrix of elements of the active set
    mPsiAk = Matrix ( sampleSize, predictorsSize );
    for ( UnsignedInteger i = 0; i < sampleSize; ++ i )
      for ( UnsignedInteger j = 0; j < predictorsSize; ++ j )
        mPsiAk( i, j ) = mPsiX( i, predictors[j] );

    // descent direction
    const NumericalPoint descentDirectionAk( cNorm * ga1 );
    const NumericalPoint u( mPsiAk * descentDirectionAk );
    const NumericalPoint d( mPsiAc.transpose() * u );

    // compute step
    NumericalScalar step( cMax / cNorm );
    for ( UnsignedInteger j = 0; j < basisSize - predictorsSize; ++ j )
    {
      NumericalScalar lhs( (cMax - cI[j]) / (cNorm - d[j]) );
      NumericalScalar rhs( (cMax + cI[j]) / (cNorm + d[j]) );
      if (lhs > 0.0)
        step = std::min(step, lhs);
      if (rhs > 0.0)
        step = std::min(step, rhs);
    }

    // update mu
    mu += step * u;

    // update coefficients
    oldCoefficientsL1Norm = coefficientsL1Norm;
    coefficientsL1Norm = 0.0;
    for ( UnsignedInteger j = 0; j < predictorsSize; ++ j )
    {
      coefficients[predictors[j]] += step * descentDirectionAk[j];
      coefficientsL1Norm += fabs( coefficients[predictors[j]] );
    }

    if (coefficientsL1Norm > 0.0) relativeConvergence = fabs(1.0 - oldCoefficientsL1Norm / coefficientsL1Norm);
    else relativeConvergence = -1.0;

    result.add(predictors);

    if ( getVerbose() ) LOGINFO( OSS() << "End of iteration " << iterations << " over " << maximumNumberOfIterations - 1 << " iteration(s)" << ", relative convergence=" << relativeConvergence << " for a target=" << maximumRelativeConvergence_);

    ++ iterations;

  }
  while ( ( iterations < maximumNumberOfIterations ) && ( relativeConvergence > maximumRelativeConvergence_ ) );

  return result;
}

/* String converter */
String LAR::__repr__() const
{
  return OSS(true) << "class=" << getClassName();
}

String LAR::__str__(const String & offset) const
{
  return OSS(false) << offset << __repr__();
}

/* Method save() stores the object through the StorageManager */
void LAR::save(Advocate & adv) const
{
  BasisSequenceFactoryImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void LAR::load(Advocate & adv)
{
  BasisSequenceFactoryImplementation::load(adv);
}

END_NAMESPACE_OPENTURNS
