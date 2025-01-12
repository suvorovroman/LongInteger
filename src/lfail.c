/* $Id: LFAIL.C,v 1.1.1.1 2005-11-20 11:36:51 roma Exp $ */

#include <stdlib.h>
#include <stdio.h>

void lstop( const char *fn, const char *msg ){
	fprintf( stderr, "%s: %s\n", fn, msg );
	exit( 1996 );
}
