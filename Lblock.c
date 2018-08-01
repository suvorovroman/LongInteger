/* $Id: Lblock.c,v 1.2 2009-07-01 16:13:05 roma Exp $ */

/* lblock.c (выделение блоков под ячеистую память) */

#include <stdlib.h> 
#include <malloc.h>
#include "lstor.h"

#ifdef _DEBUG 
	#if _MSC_VER >= 1400
		#include <crtdbg.h>
	#endif
#endif

void lstop( const char *, const char * );

static unsigned long int LMemLeak = 0L; /* счетчик выделенной памяти */

/* цепочка выделенных блоков */
struct LBlock{
        struct LCELL    pool[ LBLOCK_SIZE ];
        struct LBlock    *next;
};
static struct LBlock  *LBlockChain = NULL;

#ifdef _DEBUG
	#if _MSC_VER >= 1400
		#define ALLOCATE( SIZE )	_malloc_dbg( SIZE, _CLIENT_BLOCK, __FILE__, __LINE__ )
		#define DEALLOCATE( PTR )   _free_dbg( PTR, _CLIENT_BLOCK )
	#else
		#define ALLOCATE( SIZE )	malloc( SIZE )
		#define DEALLOCATE( PTR )   free( PTR )
	#endif
#else
	#define ALLOCATE( SIZE )	malloc( SIZE )
	#define DEALLOCATE( PTR )   free( PTR )
#endif

/* Размещает блок памяти.
   Для размещения используется "дальняя" куча.
   В случае нехватки памяти происходит аварийное завершение. */
LADDRESS get_lblock( void ){
        LADDRESS ret;
        struct LBlock  *lb;
        
        lb = (struct LBlock  *)ALLOCATE( sizeof( struct LBlock ) );
        if( lb == NULL )
        	lstop( "get_lblock", "Ошибка при размещении блока\n" );
        lb->next = LBlockChain;
        LBlockChain = lb;
        ret = (LADDRESS)lb->pool;
        LMemLeak += sizeof( struct LBlock );
        return ret;
}

/* LMemLeak - немодифицируемая извне переменная. */
unsigned long int lleak( void ){
        return LMemLeak;
}

/* Освобождение памяти, занятой блоками ячеистой памяти.
   (блоки не должны содежать используемых ячеек).
*/
void free_lbchain( void ){
      while( LBlockChain != NULL ){
            struct LBlock  *lb;
		
            lb = LBlockChain;
            LBlockChain = LBlockChain->next;
            DEALLOCATE( lb );
      }
      LMemLeak = 0L;
}