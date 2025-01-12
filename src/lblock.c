/** \file
    $Id: Lblock.c,v 1.2 2009-07-01 16:13:05 roma Exp $
*/

/* lblock.c (выделение блоков под ячеистую память) */

#include <stdlib.h> 
#include <malloc.h>
#include "lstor.h"

#if _MSC_VER >= 1400 && _DEBUG 
	#include <crtdbg.h>
#endif

void lstop( const char *, const char * );

static unsigned long int LMemLeak = 0L; /* счетчик выделенной памяти */

/* цепочка выделенных блоков */
struct LBlock{
        struct lcell	pool[ LSTOR_POOL_SIZE ];
        struct LBlock	*next;
};
static struct LBlock  *LBlockChain = NULL;

#ifdef _MSC_VER
	#ifdef _DEBUG
		#if _MSC_VER >= 1400
			#define ALLOCATE( SIZE )	_malloc_dbg( SIZE, _CLIENT_BLOCK, __FILE__, __LINE__ )
			#define DEALLOCATE( PTR )	_free_dbg( PTR, _CLIENT_BLOCK )
		#else
			#define ALLOCATE( SIZE )	malloc( SIZE )
			#define DEALLOCATE( PTR )	free( PTR )
		#endif
	#else
		#define ALLOCATE( SIZE )	malloc( SIZE )
		#define DEALLOCATE( PTR )	free( PTR )
	#endif
#else
	#define ALLOCATE( SIZE )	malloc( SIZE )
	#define DEALLOCATE( PTR )	free( PTR )
#endif

size_t lblock_size( void )
{
  return sizeof(struct LBlock);
}

/* Размещает блок памяти.
   Для размещения используется "дальняя" куча.
   В случае нехватки памяти происходит аварийное завершение. */
lcell* get_lblock( void ){
        lcell* ret;
        struct LBlock  *lb;
        
        lb = (struct LBlock  *)ALLOCATE( lblock_size() );
        if( lb == NULL )
        	lstop( "get_lblock", "Ошибка при размещении блока\n" );
        lb->next = LBlockChain;
        LBlockChain = lb;
        ret = (lcell*)lb->pool;
        LMemLeak += lblock_size();
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


