//                                               -*- C++ -*-
/**
 *  @file  PersistentObject.hxx
 *  @brief Class PersistentObject saves and reloads the object's internal state
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
#ifndef OPENTURNS_PERSISTENTOBJECT_HXX
#define OPENTURNS_PERSISTENTOBJECT_HXX

#include "OSS.hxx"
#include "Object.hxx"
#include "StorageManager.hxx"
#include "IdFactory.hxx"
#include "Pointer.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * The default name of any object
 */
extern const String DefaultName;



class StorageManager;
class StorageManagerAdvocate;
template <class PERSISTENT> class Factory;

/**
 * @class PersistentObject
 *
 * @brief This class defines load and save mechanisms.
 *
 * This is the base class of all objects that can be stored into
 * and reloaded from the study.
 *
 * The class defines an unique Id for every object so they can be
 * equal but not identical. This Id is an essential part for the management
 * of objects in studies.
 * @sa Study
 */

class PersistentObject
  : public Object
{
  CLASSNAME;
public:

  /**
   * Default constructor
   *
   * The constructor sets a new Id to the object,
   * so it can be later referenced by a Study object.
   * It is also declared visible if member of a study.
   *
   * The object has the default name but it does not
   * use storage for it.
   */
  PersistentObject()
    : p_name_(),
      id_(IdFactory::BuildId()),
      shadowedId_(id_),
      studyVisible_(true)
  {}

  /**
   * Standard constructor
   *
   * The constructor sets a new Id to the object,
   * so it can be later referenced by a Study object.
   * It is also declared visible if member of a study.
   *
   * The \p name is used to set the name of the object.
   * Setting the name implies storage space allocation.
   * @param name The name of the object
   */
  explicit PersistentObject(const String & name)
    : p_name_(new String(name)),
      id_(IdFactory::BuildId()),
      shadowedId_(id_),
      studyVisible_(true)
  {}

  /** Copy constructor */
  PersistentObject(const PersistentObject & other)
    : p_name_(other.p_name_),
      id_(IdFactory::BuildId()),
      shadowedId_(other.shadowedId_),
      studyVisible_(other.studyVisible_)
  {}

  /**
   * Virtual constructor
   *
   * @warning This method MUST be overloaded in derived classes.
   * @return A pointer to a newly allocated object similar to this one
   */
  virtual PersistentObject * clone() const = 0;

  /** Destructor */
  virtual ~PersistentObject() {}

  /** Assignment */
  inline
  PersistentObject & operator =(const PersistentObject & other)
  {
    if (this != &other)   // Other is NOT me, so I can assign it to me
    {
      p_name_ = other.p_name_;
      studyVisible_ = other.studyVisible_;
    }
    return *this;
  }


  /**
   * Comparison operator
   *
   * This method compares objects based on their content.
   */
  inline virtual
  Bool operator ==(const PersistentObject & other) const
  {
    return true;
  }

  /**
   * Identity comparator
   *
   * This method compares objects based on their Id.
   * @return True if objects are the same
   */
  inline
  Bool is(const PersistentObject & other) const
  {
    return (getId() == other.getId());
  }

  /* String converter */
  inline virtual
  String __repr__() const
  {
    return OSS() << "class=" << getClassName() << " name=" << getName();
  }

  /* String converter */
  inline virtual
  String __str__(const String & offset = "") const
  {
    return __repr__();
  }


  /**
   * Id accessor
   * @return The id of this object
   */
  inline
  Id getId() const
  {
    return id_;
  }

  /**
   * Shadowed id accessor
   * @internal
   */
  inline
  void setShadowedId(Id id)
  {
    shadowedId_ = id;
  }
  inline
  Id getShadowedId() const
  {
    return shadowedId_;
  }

  /** Visibility accessor */
  inline
  void setVisibility(Bool visible)
  {
    studyVisible_ = visible;
  }
  inline
  Bool getVisibility() const
  {
    return studyVisible_;
  }

  /**
   * %Object name query
   *
   * This methos returns true if a name was given to the object
   * @return True if object has a name
   */
  inline
  Bool hasName() const
  {
    return p_name_;
  }

  /**
   * %Object name visibility query
   *
   * This methos returns true if a non-empty name was given to the object
   * @return True if object has a non-empty name
   */
  inline
  Bool hasVisibleName() const
  {
    return (p_name_ && (p_name_->size() > 0) && (*p_name_ != DefaultName)) ;
  }

  /**
   * %Object name accessor
   *
   * This method returns the name of the object if it has been previously set
   * or the default name. Accessing the default name does not cause storage
   * allocation as a side effect.
   * @return The name of this object
   */
  inline
  String getName() const
  {
    return ( hasName() ? *p_name_ : DefaultName );
  }

  /**
   * %Object name accessor
   *
   * This method sets \p name as the new name for the object. Setting the name
   * may cause storage allocation for the new name, especially of the name was
   * not defined so far.
   * @param name The new name for this object
   */
  inline
  void setName(const String & name)
  {
    p_name_.reset(new String(name));
  }


  /** Method save() stores the object through the StorageManager
   * @internal
   */
  void save(StorageManager & mgr, const String & label, bool fromStudy = false) const;

  /** Method save() stores the object through the StorageManager
   * @internal
   */
  void save(StorageManager & mgr, bool fromStudy = false) const;

  /** Method save() stores the object through the StorageManager
   *
   * @warning This method MUST be overloaded in derived classes.
   * @internal
   */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager
   *
   * @warning This method MUST be overloaded in derived classes.
   * @internal
   */
  virtual void load(Advocate & adv);


protected:

private:

  /** The name of the object */
  mutable Pointer<String> p_name_;

  /**
   * The unique identifier of the object
   *
   * This identifier is needed when saving and reloading the object
   * because it allows the chaining of objects even if they are
   * relocated.
   */
  const Id id_;

  /**
   * The shadowed id is used when object is reloaded. The object gets
   * a new id that is almost always different from that stored in the study.
   * So when we need to rebuild the objects dependency tree (ie, when
   * object A embed object B), we have to make the id translation: the
   * object holds the both ids, the new one (aka the Id as returned by getId)
   * and the former one stored in the study (aka the shadowed id). This latter
   * is never seen except by the object factory.
   * @internal
   */
  Id shadowedId_;

  /**
   * This flag is used by the Study to know if the object should be displayed
   * even if it had been added to the study (in particular, when the object
   * was rebuild from file)
   */
  Bool studyVisible_;

}; /* class PersistentObject */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PERSISTENTOBJECT_HXX */
