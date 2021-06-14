/* $Id: LINT1.H,v 1.1.1.1 2005-11-20 11:36:48 roma Exp $ */

/* lint1.h */

#include "lstor.h"
 
#define LINT_BETA       100
#define LINT_BETA1      (LINT_BETA - 1)

#define LINT1_NDT       2

/* ОПЕРАЦИИ С ОДНОРАЗРЯДНЫМИ ЧИСЛАМИ (по основанию LINT_BETA) */
/* ---------------------------------------------------------- */

byte            lint1add( byte x, byte y );
byte            lint1sub( byte x, byte y );
void            lint1mul( byte x, byte y, byte z[ 2 ] );
byte            lint1div( byte xl, byte xh, byte y, LDATA q );
const char *    lint1scan( LDATA x, const char * s, const char * t );
void            lint1out( FILE * out, byte x, int p );

/* Вырабатывает представление числа x в виде последовательности
   десятичных цифр. d[ i ] содержит i-ю десятичную цифру числа
   x (начиная с младших цифр).
*/
void            lint1dt( byte x, byte d[ LINT1_NDT ] );

extern byte LintCf;     /* флаг переноса/заема */
