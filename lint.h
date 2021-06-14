/* $Id: Lint.h,v 1.1.1.1 2005-11-20 11:36:48 roma Exp $ */

/* lint.h */

/*             АРИФМЕТИКА ЦЕЛЫХ НЕОТРИЦАТЕЛЬНЫХ ЧИСЕЛ                */
/*-------------------------------------------------------------------*/

#include <stdio.h> 
#include "lint12.h"

#define LINTEGER        LADDRESS
#define LINTEGERC       LADDRESSC

void            lintout( FILE * out, LINTEGER x );
LADDRESS        lint( LINTEGER *x, const char * s );
void            lintdel( LINTEGER x );
void            lintdelex( LINTEGER x, LADDRESS e );
void            lintadd( LINTEGER *x, LADDRESS *xe, LINTEGERC y );
int             lintsub( LINTEGER *x, LADDRESS *xe, LINTEGERC y );
int             lintcmp( LINTEGERC x, LINTEGERC y );
void            lintmul( LINTEGERC y, LINTEGERC z, LINTEGER *x,
                                         LADDRESS *xe );
void            lintdiv( LINTEGER *x, LINTEGER *xe, LINTEGER y,
                         LINTEGER *q, LINTEGER *qe );
LADDRESS        lintlong( LINTEGER *x, unsigned long v );
LADDRESS		lintdouble(LINTEGER *x, double v);

#define         lintcopy( x, y )        copy_chain( x, y )
