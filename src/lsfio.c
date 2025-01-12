/* $Id: LSFIO.C,v 1.1.1.1 2005-11-20 11:36:52 roma Exp $ */

#include <stdio.h>
#include "lsint.h" 

/* [ВЫВОД] Вывод числа v в файл fout.
*/
void lsintout( LSINTEGER *v, FILE *fout ){
      int cr;

      cr = lsintcmp( v, NULL );
      if( cr < 0 )
            fputc( '-', fout );
      if( cr != 0 ){
            LSINTDTLIST l;

            lsintdtstart( &l, v );
            while( lsintdtcont( &l ) )
                  fputc( lsintdtget( &l ) + '0', fout );
            lsintdtend( &l );
      }
      else
            fputc( '0', fout );
}