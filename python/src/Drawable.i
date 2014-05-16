// SWIG file Drawable.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "Drawable.hxx"
%}

OTDefaultCollectionConvertFunctions(Drawable)

OTTypedInterfaceObjectHelper(Drawable)
OTTypedCollectionInterfaceObjectHelper(Drawable)

%include Drawable.hxx


%pythoncode %{
def Drawable__repr_svg_(self):
    """ svg representation """
    from .viewer import ToSVGString
    return ToSVGString(self)

Drawable._repr_svg_ = Drawable__repr_svg_
%}

namespace OT{  

%extend Drawable {

Drawable(const Drawable & other) { return new OT::Drawable(other); }

Drawable(PyObject * pyObj)
{
 return new OT::Drawable( OT::convert<OT::_PyObject_,OT::Drawable>(pyObj) );
}

}
}
