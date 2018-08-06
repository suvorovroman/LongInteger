/* $Id: LSRND.C,v 1.1.1.1 2005-11-20 11:36:52 roma Exp $ */

#include <stdlib.h> 
#include "lsint.h"

/* [ИНИЦИАЛИЗАЦИЯ] Переменной v присваевается псевдослучайное
   число длины length десятичных цифр.
*/
void lsintrand( LSINTEGER *v, int length ){
      lsintzero( v );
      if( length > 0 ){
            char *buffer;

			length = rand()%(length + 1);
            buffer = (char *)malloc( length + 2 );
            if( buffer != NULL ){
                  int i;

                  buffer[ 0 ] = (rand()%2) ? '+':'-';
                  for( i = 1; i <= length; i++ )
                        buffer[ i ] = rand()%10 + '0';
                  buffer[ i ] = '\0';
                  lsintstr( v, buffer );
                  free( buffer );
            }
      }
}