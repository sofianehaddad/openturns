// SWIG file Basis.i
// @author schueller
// @date   2009-11-09 17:38:15 +0100 (Mon, 09 Nov 2009)

%{
#include "PythonWrappingFunctions.hxx"
#include "Basis.hxx"
%}

%template(NumericalMathFunctionCollection) OT::Collection<OT::NumericalMathFunction>;
%template(NumericalMathFunctionPersistentCollection) OT::PersistentCollection<OT::NumericalMathFunction>;

%include Basis.hxx

%extend OT::Collection<OT::NumericalMathFunction>
{

OT::Collection<OT::NumericalMathFunction> (PyObject * pyObj)
{
  return OT::buildCollectionFromPySequence<OT::NumericalMathFunction>( pyObj );
}

}




namespace OT {  

%extend Basis {

Basis(const Basis & other)
{
  return new OT::Basis(other);
}

Basis(PyObject * pyObj)
{
  OT::Pointer<OT::Collection<OT::NumericalMathFunction> > p_coll =  OT::buildCollectionFromPySequence<OT::NumericalMathFunction>( pyObj );
  return new OT::Basis( *p_coll );
}

} // Basis
} // OT
