//                                               -*- C++ -*-
/**
 *  @file  RandomGeneratorState.cxx
 *  @brief RandomGeneratorState implements methods to manage the random generator state
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
 *  @author ladier
 *  @date   2013-11-27 15:35:43 +0100 (Wed, 27 Nov 2013)
 */
#include "RandomGeneratorState.hxx"
#include "PersistentObjectFactory.hxx"
#include "Indices.hxx"
#include "OSS.hxx"

BEGIN_NAMESPACE_OPENTURNS


CLASSNAMEINIT(RandomGeneratorState);

static Factory<RandomGeneratorState> RegisteredFactory("RandomGeneratorState");


/* Default constructor */
RandomGeneratorState::RandomGeneratorState(): buffer_(0), index_(0)
{
  // Nothing to do
}

/* Standard constructor */
RandomGeneratorState::RandomGeneratorState(const Indices buffer, const UnsignedInteger index) : buffer_(buffer), index_(index)
{
  // Nothing to do
}

/* Virtual constructor */
RandomGeneratorState * RandomGeneratorState::clone() const
{
  return new RandomGeneratorState(*this);
}

/* String converter */
String RandomGeneratorState::__repr__() const
{
  return OSS(true) << "RandomGeneratorState("
         << "buffer=" << buffer_ << ", "
         << "index=" << index_ << ")";
}

String RandomGeneratorState::__str__(const String & offset) const
{
  return OSS(false) << "RandomGeneratorState("
         << "buffer=" << buffer_ << ", "
         << "index=" << index_ << ")";
}

/* Buffer Accessor */
Indices RandomGeneratorState::getBuffer() const
{
  return buffer_;
}

/* Index Accessor */
UnsignedInteger RandomGeneratorState::getIndex() const
{
  return index_;
}

/* Comparison operator */
Bool RandomGeneratorState::operator ==(const RandomGeneratorState & other) const
{
  Bool result = true;
  if (this != &other) result = (buffer_ == other.buffer_) && (index_ == other.index_);

  return result;
}

/* Method save() stores the object through the StorageManager */
void RandomGeneratorState::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  Indices indicesState(buffer_.getSize());
  for(UnsignedInteger i = 0; i < buffer_.getSize(); i++)
    indicesState[i] = buffer_[i];
  adv.saveAttribute( "buffer_", indicesState);
  adv.saveAttribute( "index_", index_);
}

/* Method load() reloads the object from the StorageManager */
void RandomGeneratorState::load(Advocate & adv)
{
  PersistentObject::load(adv);
  Indices indicesState(buffer_.getSize());
  adv.loadAttribute( "buffer_", indicesState);
  buffer_ = indicesState;
  adv.loadAttribute( "index_", index_);
}

END_NAMESPACE_OPENTURNS