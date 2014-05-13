//                                               -*- C++ -*-
/**
 *  @file  NumericalSample.cxx
 *  @brief The class NumericalSample implements blank free samples
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
#include <iomanip>
#include <fstream>
#include "NumericalSample.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(NumericalSample);

/* Factory of NumericalSample from CSV file */
NumericalSample NumericalSample::ImportFromCSVFile(const FileName & fileName,
    const String & csvSeparator)
{
  NumericalSample sample(NumericalSampleImplementation::BuildFromCSVFile(fileName, csvSeparator));
  return sample;
}

/* Factory of NumericalSample from Text file */
NumericalSample NumericalSample::ImportFromTextFile(const FileName & fileName,
    const String & separator)
{
  NumericalSample sample(NumericalSampleImplementation::BuildFromTextFile(fileName, separator));
  return sample;
}

/* Save to CSV file */
void NumericalSample::exportToCSVFile(const FileName & filename,
                                      const String & csvSeparator) const
{
  getImplementation()->exportToCSVFile(filename, csvSeparator);
}


/* Store a sample in a temporary text file, one realization by line. Returns the file name. */
String NumericalSample::storeToTemporaryFile() const
{
  return getImplementation()->storeToTemporaryFile();
}

/* Export a sample as a matrix, one row by realization, in a format suitable to exchange with R. */
String NumericalSample::streamToRFormat() const
{
  return getImplementation()->streamToRFormat();
}

/* Default constructor */
NumericalSample::NumericalSample()
  : TypedInterfaceObject<NumericalSampleImplementation>(new NumericalSampleImplementation(0, 1))
{
  // Nothing to do
}

/* Constructor with size and dimension */
NumericalSample::NumericalSample(const UnsignedInteger size,
                                 const UnsignedInteger dim)
  : TypedInterfaceObject<NumericalSampleImplementation>(new NumericalSampleImplementation(size, dim))
{
  // Nothing to do
}

/* Constructor from implementation */
NumericalSample::NumericalSample(const NumericalSampleImplementation & implementation)
  : TypedInterfaceObject<NumericalSampleImplementation>(implementation.clone())
{
  // Nothing to do
}

/* Constructor from implementation */
NumericalSample::NumericalSample(const Implementation & implementation)
  : TypedInterfaceObject<NumericalSampleImplementation>(implementation)
{
  // Nothing to do
}

/* Constructor from a NumericalPoint (all elements are equal to the NumericalPoint) */
NumericalSample::NumericalSample(const UnsignedInteger size,
                                 const NumericalPoint & point)
  : TypedInterfaceObject<NumericalSampleImplementation>(new NumericalSampleImplementation(size, point))
{
  // Nothing to do
}

/* Constructor from a collection of NumericalPoint */
NumericalSample::NumericalSample(const Collection<NumericalPoint> & coll)
  : TypedInterfaceObject<NumericalSampleImplementation>(new NumericalSampleImplementation(coll))
{
  // Nothing to do
}

/* Constructor from a collection of Indices */
NumericalSample::NumericalSample(const Collection<Indices> & coll)
  : TypedInterfaceObject<NumericalSampleImplementation>(new NumericalSampleImplementation(coll))
{
  // Nothing to do
}

/* Comparison operator */
Bool NumericalSample::operator ==(const NumericalSample & other) const
{
  return *getImplementation() == *(other.getImplementation());
}

/* Comparison operator */
Bool NumericalSample::operator !=(const NumericalSample & other) const
{
  return !operator==(other);
}


NSI_point NumericalSample::operator [] (const UnsignedInteger index)
{
#ifdef DEBUG_BOUNDCHECKING
  copyOnWrite();
  return this->at(index);
#else
  copyOnWrite();
  return (*getImplementation())[index];
#endif /* DEBUG_BOUNDCHECKING */
}

NSI_const_point NumericalSample::operator [] (const UnsignedInteger index) const
{
#ifdef DEBUG_BOUNDCHECKING
  return this->at(index);
#else
  return (*getImplementation())[index];
#endif /* DEBUG_BOUNDCHECKING */
}


NumericalScalar & NumericalSample::operator () (const UnsignedInteger i,
    const UnsignedInteger j)
{
#ifdef DEBUG_BOUNDCHECKING
  // No copyOnWrite() as the at() method already do it
  return this->at(i, j);
#else
  copyOnWrite();
  return (*getImplementation())[i][j];
#endif /* DEBUG_BOUNDCHECKING */
}

const NumericalScalar & NumericalSample::operator () (const UnsignedInteger i,
    const UnsignedInteger j) const
{
#ifdef DEBUG_BOUNDCHECKING
  return this->at(i, j);
#else
  return (*getImplementation())[i][j];
#endif /* DEBUG_BOUNDCHECKING */
}


NSI_point NumericalSample::at (const UnsignedInteger index)
{
  if (index >= getSize()) throw OutOfBoundException(HERE) << "Index (" << index << ") is not less than size (" << getSize() << ")";
  copyOnWrite();
  return (*getImplementation())[index];
}

NSI_const_point NumericalSample::at (const UnsignedInteger index) const
{
  if (index >= getSize()) throw OutOfBoundException(HERE) << "Index (" << index << ") is not less than size (" << getSize() << ")";
  return (*getImplementation())[index];
}

NumericalScalar & NumericalSample::at (const UnsignedInteger i,
                                       const UnsignedInteger j)
{
  if (i >= getSize()) throw OutOfBoundException(HERE) << "i (" << i << ") must be less than size (" << getSize() << ")";
  if (j >= getDimension()) throw OutOfBoundException(HERE) << "j (" << j << ") must be less than dimension (" << getDimension() << ")";
  copyOnWrite();
  return (*getImplementation())[i][j];
}

const NumericalScalar & NumericalSample::at (const UnsignedInteger i,
    const UnsignedInteger j) const
{
  if (i >= getSize()) throw OutOfBoundException(HERE) << "i (" << i << ") must be less than size (" << getSize() << ")";
  if (j >= getDimension()) throw OutOfBoundException(HERE) << "j (" << j << ") must be less than dimension (" << getDimension() << ")";
  return (*getImplementation())[i][j];
}

const NumericalPoint NumericalSample::__getitem__ (const UnsignedInteger index) const
{
  return this->at(index);
}

void NumericalSample::__setitem__ (const UnsignedInteger index,
                                   const NumericalPoint & val)
{
  copyOnWrite();
  this->at(index) = val;
}

const NumericalScalar * NumericalSample::__baseaddress__ () const
{
  return getImplementation()->__baseaddress__();
}

UnsignedInteger NumericalSample::__elementsize__ () const
{
  return getImplementation()->__elementsize__();
}



/* Method __len__() is for Python */
UnsignedInteger NumericalSample::__len__() const
{
  return getSize();
}

/* Method __contains__() is for Python */
Bool NumericalSample::__contains__(const NumericalPoint & val) const
{
  return getImplementation()->contains(val);
}



/* String converter */
String NumericalSample::__repr__() const
{
  return OSS(true) << "class=" << getClassName()
         << " name=" << getName()
         << " description=" << getDescription()
         << " implementation=" << getImplementation()->__repr__();
}

String NumericalSample::__str__(const String & offset) const
{
  return getImplementation()->__str__(offset);
}


/* Description accessor */
void NumericalSample::setDescription(const Description & description)
{
  copyOnWrite();
  getImplementation()->setDescription(description);
}



/* Description accessor */
Description NumericalSample::getDescription() const
{
  return getImplementation()->getDescription();
}


/* Dimension accessor */
UnsignedInteger NumericalSample::getDimension() const
{
  return getImplementation()->getDimension();
}


/* Size accessor */
UnsignedInteger NumericalSample::getSize() const
{
  return getImplementation()->getSize();
}

/* Maximum accessor */
NumericalPoint NumericalSample::getMax() const
{
  return getImplementation()->getMax();
}

/* Minimum accessor */
NumericalPoint NumericalSample::getMin() const
{
  return getImplementation()->getMin();
}


/* Method add() appends an element to the collection */
void NumericalSample::add(const NumericalPoint & point)
{
  if ( (getSize() > 0) && (getDimension() != point.getDimension()) )
    throw InvalidArgumentException(HERE)
        << "Point has invalid dimension (dim=" << point.getDimension()
        << ") for sample (dim=" << getDimension() << ")";

  copyOnWrite();
  getImplementation()->add(point);
}

/* Method add() appends another sample to the collection */
void NumericalSample::add(const NumericalSample & sample)
{
  if ( (getSize() > 0) && (getDimension() != sample.getDimension()) )
    throw InvalidArgumentException(HERE)
        << "Sample has invalid dimension (dim=" << sample.getDimension()
        << ") for sample (dim=" << getDimension() << ")";

  copyOnWrite();
  getImplementation()->add(*sample.getImplementation());
}

/* Stack another sample to the current sample */
void NumericalSample::stack(const NumericalSample & sample)
{
  copyOnWrite();
  getImplementation()->stack(*sample.getImplementation());
}


void NumericalSample::erase(const UnsignedInteger first,
                            const UnsignedInteger last)
{
  copyOnWrite();
  getImplementation()->erase(first, last);
}


void NumericalSample::erase(const UnsignedInteger index)
{
  copyOnWrite();
  getImplementation()->erase(index, index + 1);
}

void NumericalSample::erase(NumericalSampleImplementation::iterator first, NumericalSampleImplementation::iterator last)
{
  copyOnWrite();
  getImplementation()->erase(first, last);
}

void NumericalSample::clear()
{
  copyOnWrite();
  getImplementation()->clear();
}

/*
 * Method split() trunk the sample before the index passed as argument
 * and returns the remainder as new sample. This method tries its best not for doubling
 * memory usage.
 */
NumericalSample NumericalSample::split(const UnsignedInteger index)
{
  copyOnWrite();
  // We first save the size of the original sample
  UnsignedInteger theSize = getSize();

  // We first check that the index is in the sample's range
  if (index > theSize) throw OutOfBoundException(HERE) << "Index over size. Index=" << index << " size=" << theSize;
  // Quick check for easy cases
  if (index == theSize) return NumericalSample(0, getDimension());
  if (index >= theSize / 2)   // Strategy 1.
  {
    // We take two iterators :
    //  * one on the first element of the returned sample (at the i-th element)
    //  * one at the end of the returned sample
    NumericalSampleImplementation::iterator first = getImplementation()->begin() + index;
    NumericalSampleImplementation::iterator last  = getImplementation()->end();

    // We create a new sample by copy-constructing it from the two iterators
    NumericalSample newSample(new NumericalSampleImplementation(*getImplementation(), first, last));

    // We clear the copied elements of the original sample (*this)
    erase(first, last);

    // We return the new sample
    return newSample;

  }
  else     // Strategy 2.
  {
    // We take two iterators :
    //  * one at the beginning of the original sample
    //  * one before the beginning of the returned sample
    NumericalSampleImplementation::iterator first = getImplementation()->begin();
    NumericalSampleImplementation::iterator last  = getImplementation()->begin() + index;

    // We create a new sample by copy-constructing it from the two iterators
    NumericalSample newSample(new NumericalSampleImplementation(*getImplementation(), first, last));

    // We clear the copied elements of the original sample (*this)
    erase(first, last);

    // We swap the two samples
    this->swap(newSample);

    // We return the new sample
    return newSample;
  }
}

/*
 * Method computeMean() gives the mean of the sample, based on the formula
 * mean = sum of the elements in the sample / size of the sample
 */
NumericalPoint NumericalSample::computeMean() const
{
  return getImplementation()->computeMean();
}

/*
 * Method computeCovariance() gives the covariance of the sample
 */
CovarianceMatrix NumericalSample::computeCovariance() const
{
  return getImplementation()->computeCovariance();
}

/*
 * Method computeStandardDeviation() gives the standard deviation of the sample
 */
SquareMatrix NumericalSample::computeStandardDeviation() const
{
  return getImplementation()->computeStandardDeviation();
}

/*
 * Method computeStandardDeviationPerComponent() gives the standard deviation of each component of the sample
 */
NumericalPoint NumericalSample::computeStandardDeviationPerComponent() const
{
  return getImplementation()->computeStandardDeviationPerComponent();
}

/*
 * Method computePearsonCorrelation() gives the Pearson correlation matrix of the sample
 */
CorrelationMatrix NumericalSample::computeLinearCorrelation() const
{
  return getImplementation()->computeLinearCorrelation();
}

CorrelationMatrix NumericalSample::computePearsonCorrelation() const
{
  return getImplementation()->computePearsonCorrelation();
}

/*
 * Method computeSpearmanCorrelation() gives the Spearman correlation matrix of the sample
 */
CorrelationMatrix NumericalSample::computeSpearmanCorrelation() const
{
  return getImplementation()->computeSpearmanCorrelation();
}

/*
 * Method computeKendallTau() gives the Kendall tau matrix of the sample
 */
CorrelationMatrix NumericalSample::computeKendallTau() const
{
  return getImplementation()->computeKendallTau();
}

/*
 * Method computeRangePerComponent gives the range of the sample (by component)
 */
NumericalPoint NumericalSample::computeRangePerComponent() const
{
  return getImplementation()->computeRangePerComponent();
}

/*
 * Method computeMedian() gives the median of the sample (by component)
 */
NumericalPoint NumericalSample::computeMedianPerComponent() const
{
  return getImplementation()->computeMedianPerComponent();
}


/*
 * Method computeVariance() gives the variance of the sample (by component)
 */
NumericalPoint NumericalSample::computeVariancePerComponent() const
{
  return getImplementation()->computeVariancePerComponent();
}

/*
 * Method computeSkewness() gives the skewness of the sample (by component)
 */
NumericalPoint NumericalSample::computeSkewnessPerComponent() const
{
  return getImplementation()->computeSkewnessPerComponent();
}

/*
 * Method computeKurtosis() gives the kurtosis of the sample (by component)
 */
NumericalPoint NumericalSample::computeKurtosisPerComponent() const
{
  return getImplementation()->computeKurtosisPerComponent();
}

/*
 * Gives the centered moment of order k of the sample (by component)
 */
NumericalPoint NumericalSample::computeCenteredMomentPerComponent(const UnsignedInteger k) const
{
  return getImplementation()->computeCenteredMomentPerComponent(k);
}

/*
 * Gives the raw moment of order k of the sample (by component)
 */
NumericalPoint NumericalSample::computeRawMomentPerComponent(const UnsignedInteger k) const
{
  return getImplementation()->computeRawMomentPerComponent(k);
}

/*
 * Method computeQuantilePerComponent() gives the quantile per component of the sample
 */
NumericalPoint NumericalSample::computeQuantilePerComponent(const NumericalScalar prob) const
{
  return getImplementation()->computeQuantilePerComponent(prob);
}

/*
 * Method computeQuantile() gives the N-dimension quantile of the sample
 */
NumericalPoint NumericalSample::computeQuantile(const NumericalScalar prob) const
{
  return getImplementation()->computeQuantile(prob);
}

/*
 * Get the empirical CDF of the sample
 */
NumericalScalar NumericalSample::computeEmpiricalCDF(const NumericalPoint & point,
    const Bool tail) const
{
  return getImplementation()->computeEmpiricalCDF(point, tail);
}

/*
 * Get the position of a point in the sample.
 * Returns size+1 if the point does not belong to the sample.
 */
UnsignedInteger NumericalSample::find(const NumericalPoint & point) const
{
  return std::find(getImplementation()->begin(),  getImplementation()->end(), point) - getImplementation()->begin();
}

/*
 * Translate realizations in-place
 */
void NumericalSample::translate(const NumericalPoint & translation)
{
  copyOnWrite();
  getImplementation()->translate(translation);
}

NumericalSample & NumericalSample::operator += (const NumericalPoint & translation)
{
  copyOnWrite();
  getImplementation()->operator +=(translation);
  return *this;
}

NumericalSample & NumericalSample::operator += (const NumericalSample & translation)
{
  copyOnWrite();
  getImplementation()->operator +=(*translation.getImplementation());
  return *this;
}

NumericalSample & NumericalSample::operator -= (const NumericalPoint & translation)
{
  copyOnWrite();
  getImplementation()->operator -=(translation);
  return *this;
}

NumericalSample & NumericalSample::operator -= (const NumericalSample & translation)
{
  copyOnWrite();
  getImplementation()->operator -=(*translation.getImplementation());
  return *this;
}

NumericalSample NumericalSample::operator + (const NumericalPoint & translation) const
{
  NumericalSample sample(getImplementation()->operator + (translation));
  return sample;
}

NumericalSample NumericalSample::operator + (const NumericalSample & translation) const
{
  NumericalSample sample(getImplementation()->operator + (*translation.getImplementation()));
  return sample;
}

NumericalSample NumericalSample::operator - (const NumericalPoint & translation) const
{
  NumericalSample sample(getImplementation()->operator - (translation));
  return sample;
}

NumericalSample NumericalSample::operator - (const NumericalSample & translation) const
{
  NumericalSample sample(getImplementation()->operator - (*translation.getImplementation()));
  return sample;
}

/*
 * Scale realizations componentwise in-place
 */
void NumericalSample::scale(const NumericalPoint & scaling)
{
  copyOnWrite();
  getImplementation()->scale(scaling);
}

void NumericalSample::scale(const SquareMatrix & scaling)
{
  copyOnWrite();
  getImplementation()->scale(scaling);
}

NumericalSample & NumericalSample::operator *= (const NumericalPoint & scaling)
{
  copyOnWrite();
  getImplementation()->operator *=(scaling);
  return *this;
}

NumericalSample & NumericalSample::operator /= (const NumericalPoint & scaling)
{
  copyOnWrite();
  getImplementation()->operator /=(scaling);
  return *this;
}

NumericalSample & NumericalSample::operator *= (const SquareMatrix & scaling)
{
  copyOnWrite();
  getImplementation()->operator *=(scaling);
  return *this;
}

NumericalSample & NumericalSample::operator /= (const SquareMatrix & scaling)
{
  copyOnWrite();
  getImplementation()->operator /=(scaling);
  return *this;
}

NumericalSample NumericalSample::operator * (const NumericalPoint & scaling)
{
  NumericalSample sample(getImplementation()->operator * (scaling));
  return sample;
}

NumericalSample NumericalSample::operator / (const NumericalPoint & scaling)
{
  NumericalSample sample(getImplementation()->operator / (scaling));
  return sample;
}

NumericalSample NumericalSample::operator * (const SquareMatrix & scaling)
{
  NumericalSample sample(getImplementation()->operator * (scaling));
  return sample;
}

NumericalSample NumericalSample::operator / (const SquareMatrix & scaling)
{
  NumericalSample sample(getImplementation()->operator / (scaling));
  return sample;
}

/* Ranked sample */
NumericalSample NumericalSample::rank() const
{
  return getImplementation()->rank();
}

/* Ranked component */
NumericalSample NumericalSample::rank(const UnsignedInteger index) const
{
  return getImplementation()->rank(index);
}

/* Sorted sample */
NumericalSample NumericalSample::sort() const
{
  return getImplementation()->sort();
}

/* Sorted component */
NumericalSample NumericalSample::sort(const UnsignedInteger index) const
{
  return getImplementation()->sort(index);
}

/* Sorted component */
NumericalSample NumericalSample::sortAccordingToAComponent(const UnsignedInteger index) const
{
  return getImplementation()->sortAccordingToAComponent(index);
}

/* Get the i-th marginal sample */
NumericalSample NumericalSample::getMarginal(const UnsignedInteger index) const
{
  return getImplementation()->getMarginal(index);
}

/* Get the marginal sample corresponding to indices dimensions */
NumericalSample NumericalSample::getMarginal(const Indices & indices) const
{
  return getImplementation()->getMarginal(indices);
}

/* Returns a pointer to the underlying implementation object */
NumericalSample::ImplementationAsPersistentObject NumericalSample::getImplementationAsPersistentObject() const
{
  return getImplementation();
}

/* Sets the pointer to the underlying implementation object */
void NumericalSample::setImplementationAsPersistentObject(const ImplementationAsPersistentObject & obj)
{
  getImplementation().assign(obj);
}


END_NAMESPACE_OPENTURNS
