// SWIG file Path.i
// @author souchaud
// @date   2009-07-10 16:45:58 +0200 (ven. 10 juil. 2009)

%{
#include "Path.hxx"
%}

%template(DirectoryList) std::vector<OT::FileName>;

%nodefaultctor Path;

%include Path.hxx
