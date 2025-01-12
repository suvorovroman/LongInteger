/** \file
 */

/* $Id: LINT1.H,v 1.1.1.1 2005-11-20 11:36:48 roma Exp $ */

/* lint1.h */

#include "lstor.h"
#include <stdio.h>

/** Одноразрядное число (две десятичных цифры). */
typedef lcell_byte	LINT_DIGIT;

/** Основание разряда числа. */
#define LINT_BETA       100
#define LINT_BETA1      (LINT_BETA - 1)

/** Количество десятичных цифр в разряде числа. */
#define LINT1_NDT       2

/* ОПЕРАЦИИ С ОДНОРАЗРЯДНЫМИ ЧИСЛАМИ (по основанию LINT_BETA) */
/* ---------------------------------------------------------- */

LINT_DIGIT	lint1add( LINT_DIGIT x, LINT_DIGIT y );
LINT_DIGIT	lint1sub( LINT_DIGIT x, LINT_DIGIT y );
void		lint1mul( LINT_DIGIT x, LINT_DIGIT y, LINT_DIGIT z[ 2 ] );
LINT_DIGIT	lint1div( LINT_DIGIT xl, LINT_DIGIT xh, LINT_DIGIT y, lcell_byte *q );
const char *	lint1scan( lcell_byte *x, const char * s, const char * t );
void            lint1out( FILE * out, LINT_DIGIT x, int p );

/* Вырабатывает представление числа x в виде последовательности
   десятичных цифр. d[ i ] содержит i-ю десятичную цифру числа
   x (начиная с младших цифр).
*/
void		lint1dt( LINT_DIGIT x, LINT_DIGIT d[ LINT1_NDT ] );

extern LINT_DIGIT LintCf;     /* флаг переноса/заема */
