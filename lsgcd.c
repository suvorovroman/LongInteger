/* $Id: LSGCD.C,v 1.1.1.1 2005-11-20 11:36:52 roma Exp $ */

#include <assert.h>
#include "lsint.h"
 
/* [ДОПОЛНЕНИЯ, НОД] Алгоритм Евклида (Euclidean Algorithm). 
      x <- НОД(x, y)
      y <- 0
   Источник:
      А.Акритас, Основы компьютерной алгебры с приложениями
      М.:Мир, 1994, алгоритм EA. */
void lsintgcd( LSINTEGER *x, LSINTEGER *y ){
      assert( lsintcmp( x, NULL ) != 0 ||
              lsintcmp( y, NULL ) != 0 );
      if( lsintcmp( y, NULL ) == 0 )
            return;
      if( lsintcmp( x, NULL ) == 0 ){
            lsintlsint( x, y );
            lsintdel( 1, y );
            lsintzero( y );
            return;
      }
      while( lsintcmp( y, NULL ) != 0 ){
            LSINTEGER q;

            lsintdiv( x, y, &q );
            lsintexch( x, y );
            lsintdel( 1, &q );
      }
}