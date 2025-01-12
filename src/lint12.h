/** \file
    \brief	Операции над сегментами (основание #LINT_BETA**#LDIGIT_SIZE)
    \author	Суворов Роман
    \date	20.11.2005
    \version	1.1
 */
/* $Id: LINT12.H,v 1.1.1.1 2005-11-20 11:36:48 roma Exp $ */

/* lint12.h */

#include "lint1.h"

typedef lcell_byte* LDIGIT;
typedef const lcell_byte* LDIGITC;

/** Количество разрядов в сегменте. */
#define LDIGIT_SIZE     LSTOR_CELL_SIZE

/** Количество десятичных цифр в серменте числа. */
#define LINT12_NDT      (LINT1_NDT*LDIGIT_SIZE)

/* ОПЕРАЦИИ НАД 12-БАЙТОВЫМИ ЧИСЛАМИ (основание LINT_BETA**12) */
/* ----------------------------------------------------------- */

void            lint12div( LDIGITC xl, LDIGITC xh, LDIGIT y, LDIGIT q,
                           LDIGIT r );
void            lint12mul( LDIGITC x, LDIGITC y, LDIGIT z );
void            lint12add( LDIGITC x, LDIGITC y, LDIGIT z );
int             lint12cmp( LDIGITC x, LDIGITC y );
void            lint12sub( LDIGITC x, LDIGITC y, LDIGIT z );
const char *    lint12scan( LDIGIT x, const char *s, const char *t );
void            lint12out( FILE *out, LDIGITC x, int p );
void		lint12long( LDIGIT x, unsigned long v );

/* Вырабатывает представление числа x в виде последовательности
   десятичных цифр. d[ i ] содержит i-ю десятичную цифру числа
   x (начиная с младших цифр).
*/
void lint12dt( LDIGITC x, lcell_byte d[ LINT12_NDT ] );

#define lint12copy( x, y )      copy_cell_data( x, y )
#define lint12zero( x )         lint12copy( x, Lint12Zero )
#define lint12beta( x )         lint12copy( x, Lint12Beta1 )

/* Некоторые 12-байтовые константы: */
extern const LINT_DIGIT Lint12Zero [ LDIGIT_SIZE ];   /* 0 */
extern const LINT_DIGIT Lint12One  [ LDIGIT_SIZE ];   /* 1 */
extern const LINT_DIGIT Lint12Beta1[ LDIGIT_SIZE ];   /* base - 1 */
