// SWIG file TimeSeries.i
// @author schueller
// @date   2012-01-02 11:44:01 +0100 (Mon, 02 Jan 2012)

%{
#include "TimeSeries.hxx"
%}

%include TimeSeries.hxx
namespace OT { %extend TimeSeries { TimeSeries(const TimeSeries & other) { return new OT::TimeSeries(other); } } }
