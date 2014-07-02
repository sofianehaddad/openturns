// SWIG file Description.i
// @author schueller
// @date   2012-01-02 16:48:50 +0100 (Mon, 02 Jan 2012)

%{
#include "Description.hxx"
%}

%template(StringCollection) OT::Collection<OT::String>;
%template(StringPersistentCollection) OT::PersistentCollection<OT::String>;

%typemap(in) const OT::Description & ($1_basetype temp) {
  if (! SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, 0))) {
    temp = OT::convert<OT::_PySequence_,OT::Description>( $input );
    $1 = &temp;
  }
}

%typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) const OT::Description & {
  $1 = SWIG_IsOK(SWIG_ConvertPtr($input, NULL, $1_descriptor, 0)) ||
       OT::isAPythonSequenceOf<OT::_PyString_>( $input );
}

%include Description.hxx

namespace OT {  

%extend Description {

Description(const Description & other) { return new OT::Description(other); }

Description(PyObject * pyObj)
{
 OT::Pointer<OT::Collection<OT::String> > p_coll =  OT::buildCollectionFromPySequence<OT::String>( pyObj );
 return new OT::Description( *p_coll );
}

/* Method __len__() is for Python */
OT::UnsignedInteger __len__() const
{
  return self->getSize();
}

/* Method __contains__() is for Python */
OT::Bool __contains__(const OT::String & val) const
{
  for (OT::UnsignedInteger i=0; i<self->getSize(); ++i) if ( (*self)[i] == val ) return true;
  return false;
}

OT_COLLECTION_GETITEM(OT::Description, OT::String)
OT_COLLECTION_SETITEM(OT::Description, OT::String)

}

}
