/* $Id: LSLCM.C,v 1.1.1.1 2005-11-20 11:36:52 roma Exp $ */

#include <assert.h>
#include "lsint.h"

/* [ДОПОЛНЕНИЯ, НОД] Вычисление наименьшего обшего кратного.
      x <- НОК(x, y)
      y <- НОД(x, y)
   Истичник:
      А.Акритас, основы компьютерной алгебры с приложениями.
      М.:Мир, 1994, определение 2.2.7. */
void lsintlcm( LSINTEGER *x, LSINTEGER *y ){
      LSINTEGER z;

      assert( lsintcmp( x, NULL ) != 0 &&
              lsintcmp( y, NULL ) != 0 );      
      lsintmul( &z, x, y );
      lsintabs( &z );
      lsintgcd( y, x );
      lsintdiv( &z, y, x );
      assert( lsintcmp( &z, NULL ) == 0 );
} 
      