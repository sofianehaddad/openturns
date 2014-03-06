//                                               -*- C++ -*-
/**
 *  @file  PersistentObject.cxx
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
#include "PersistentObject.hxx"
#include "OTprivate.hxx"
#include "OTconfig.hxx"

BEGIN_NAMESPACE_OPENTURNS

/*
 * The default name of any object
 */
const String DefaultName = String(
#ifdef OBJECT_DEFAULT_NAME
                             OBJECT_DEFAULT_NAME
#else
                             "Unnamed"
#endif
                           );



CLASSNAMEINIT(PersistentObject);

/* Method save() stores the object through the StorageManager */
void PersistentObject::save(StorageManager & mgr, const String & label, bool fromStudy) const
{
  if (! mgr.isSavedObject(id_))
  {
    Pointer<Advocate> p_adv ( mgr.registerObject(*this, fromStudy) );
    p_adv->setLabel( label );
    save(*p_adv);
    p_adv->saveObject();
    mgr.markObjectAsSaved(id_);
  }
}

/* Method save() stores the object through the StorageManager */
void PersistentObject::save(StorageManager & mgr, bool fromStudy) const
{
  if (! mgr.isSavedObject(id_))
  {
    Pointer<Advocate> p_adv ( mgr.registerObject(*this, fromStudy) );
    save(*p_adv);
    p_adv->saveObject();
    mgr.markObjectAsSaved(id_);
  }
}

/* Method save() stores the object through the StorageManager */
void PersistentObject::save(Advocate & adv) const
{
  adv.saveAttribute( "class", getClassName() );
  adv.saveAttribute( "id", id_ );
  if ( hasName() )
  {
    adv.saveAttribute( "name", getName() );
  }
}

/* Method load() reloads the object from the StorageManager */
void PersistentObject::load(Advocate & adv)
{
  adv.loadAttribute( "id", shadowedId_);
  if ( adv.hasAttribute("name") )
  {
    String name;
    adv.loadAttribute( "name", name );
    setName( name );
  }
}


END_NAMESPACE_OPENTURNS
