/*
 *  @file  OTwindows.h
 *  @brief Include windows.h cleanly
 */
#ifdef WIN32

#include <windows.h>

// windows.h which defines a HUGE number of macros...

// wrappers
#undef GetClassName
#undef ERROR
#undef IN
#undef OUT

// ev3
#undef DIFFERENCE 
#undef CONST 
#undef interface 
#undef NONE 

#endif
