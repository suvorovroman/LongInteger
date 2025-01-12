/** \file
    \brief	Арифметика целых неотрицательных чисел
    \author	Suvorov Roman
    \data	20.11.2005
    \version	1.1
 */

/* $Id: Lint.h,v 1.1.1.1 2005-11-20 11:36:48 roma Exp $ */

/* lint.h */

/*             АРИФМЕТИКА ЦЕЛЫХ НЕОТРИЦАТЕЛЬНЫХ ЧИСЕЛ                */
/*-------------------------------------------------------------------*/

#include <stdio.h> 
#include "lint12.h"

typedef lcell* LINTEGER;
typedef const lcell* LINTEGERC;

void            lintout( FILE * out, LINTEGER x );
lcell*        lint( LINTEGER *x, const char * s );
void            lintdel( LINTEGER x );
void            lintdelex( LINTEGER x, lcell* e );
void            lintadd( LINTEGER *x, lcell** xe, LINTEGERC y );
int             lintsub( LINTEGER *x, lcell** xe, LINTEGERC y );
int             lintcmp( LINTEGERC x, LINTEGERC y );
void            lintmul( LINTEGERC y, LINTEGERC z, LINTEGER *x,
                                         lcell** xe );
void            lintdiv( LINTEGER *x, LINTEGER *xe, LINTEGER y,
                         LINTEGER *q, LINTEGER *qe );
lcell*        lintlong( LINTEGER *x, unsigned long v );
lcell*		lintdouble(LINTEGER *x, double v);
lcell*		lintrand(LINTEGER *x, int length);

#define         lintcopy( x, y )        copy_chain( x, y )
