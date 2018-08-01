/* $Id: Lblock.c,v 1.2 2009-07-01 16:13:05 roma Exp $ */

/* lblock.c (�뤥����� ������ ��� �票���� ������) */

#include <stdlib.h> 
#include <malloc.h>
#include "lstor.h"

#ifdef _DEBUG 
	#if _MSC_VER >= 1400
		#include <crtdbg.h>
	#endif
#endif

void lstop( const char *, const char * );

static unsigned long int LMemLeak = 0L; /* ���稪 �뤥������ ����� */

/* 楯�窠 �뤥������ ������ */
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

/* �����頥� ���� �����.
   ��� ࠧ��饭�� �ᯮ������ "������" ���.
   � ��砥 ��墠⪨ ����� �ந�室�� ���਩��� �����襭��. */
LADDRESS get_lblock( void ){
        LADDRESS ret;
        struct LBlock  *lb;
        
        lb = (struct LBlock  *)ALLOCATE( sizeof( struct LBlock ) );
        if( lb == NULL )
        	lstop( "get_lblock", "�訡�� �� ࠧ��饭�� �����\n" );
        lb->next = LBlockChain;
        LBlockChain = lb;
        ret = (LADDRESS)lb->pool;
        LMemLeak += sizeof( struct LBlock );
        return ret;
}

/* LMemLeak - ���������㥬�� ����� ��६�����. */
unsigned long int lleak( void ){
        return LMemLeak;
}

/* �᢮�������� �����, ����⮩ ������� �票�⮩ �����.
   (����� �� ������ ᮤ����� �ᯮ��㥬�� �祥�).
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