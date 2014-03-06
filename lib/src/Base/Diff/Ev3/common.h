/**********************************************************************
 * Author:      Leo Liberti                                            *
 * Name:        common.h                                               *
 * Source:      GNU C++                                                *
 * Purpose:     common stuff                                           *
 * History:     050909 0.0 work started                                *
 * License:    (C) Leo Liberti, all rights reserved. Code published under the
               Common Public License.
***********************************************************************/

#ifndef __EV3COMMONH__
#define __EV3COMMONH__

#define NOVARIABLE -1
#define LARGE 1E10
#define NOTVARNAME "_var_not_found_"

namespace Ev3
{
typedef int Int;

// various operator types
enum OperatorType
{
  SUM, DIFFERENCE, PRODUCT, FRACTION, POWER,
  PLUS, MINUS, SIN, COS, TAN, ASIN, ACOS,
  ATAN, SINH, COSH, TANH, ASINH, ACOSH,
  ATANH, LOG2, LOG10, LOG, LN, LNGAMMA,
  GAMMA, EXP, ERF, ERFC, SQRT, CBRT,
  BESSELJ0, BESSELJ1, BESSELY0, BESSELY1,
  SIGN, RINT, ABS, COT, COTH,
  VAR, CONST,
  ERROR
};

// utility functions
extern double Ev3NearZero(void);
extern double Ev3Infinity(void);

} /* namespace Ev3 */
#endif /* __EV3COMMONH__ */
