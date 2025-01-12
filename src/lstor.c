/* $Id: Lstor.c,v 1.1.1.1 2005-11-20 11:36:52 roma Exp $ */

/* lstor.c (управление ячеистой памятью) */
 
#include <stdlib.h>
#include <malloc.h>
#include "lstor.h"

/* Адрес начала списка свободных ячеек. */
static lcell* Avail = LNULL;

lcell*        get_lblock( void );
void            free_lbchain( void );

/* Если возможно выделяет ячейку из списка Avail.
   Если Avail пуст,то пытается выделить ячейку из текущего блока памяти.
   Если текущий блок исчерпан, то выделяется новый.     */
lcell* new_cell( void ){
        lcell *ret;

        if( Avail == LNULL ){
                static lcell *b;
                static unsigned int r = 0;

                if( r == 0 ){
                        b = get_lblock();
                        r = LSTOR_POOL_SIZE;
                }
                ret = b + (--r);
        }
        else{
                ret = Avail;    
                Avail = Avail->link;
        }
        return ret;
}

/* Возвращение цепочки в список Avail.
   Предполагается, что b и e являются началом и концом
   одной и той же цепочки (b != LNULL && e != LNULL). */
void del_chain( lcell* b, lcell* e ){
        e->link = Avail;
        Avail = b;
}

/* Обращение цепочки (x может быть LNULL). */
void invert_chain( lcell **x ){
        lcell *c;

        c = LNULL;
        while( *x != LNULL ){
                lcell *f;

                f = *x;
                *x = f->link;
                f->link = c;
                c = f;
        }
        *x = c;
}

/* Копирование цепочки (y может быть LNULL).
   В x помешается адрес начала цепочки, а возвращается адрес
   конца. */
lcell* copy_chain( lcell **x, const lcell* y ){
        lcell* xe;

        *x = xe = LNULL;
        while( y != LNULL ){
                if( xe != LNULL ){
                        xe->link = new_cell();
                        xe = xe->link;
                }
                else
                        *x = xe = new_cell();
                copy_cell_data( xe->data, y->data );
                y = y->link;
        }
        if( xe != LNULL )
                xe->link = LNULL;
        return xe;
}

/* Копирование поля данных ячейки (чтобы не использовать
   функции _fmemcpy, _fmemset и т.д.). */
void copy_cell_data( lcell_byte *x, const lcell_byte *y ){
        struct wrap{
                lcell_byte data[ LSTOR_CELL_SIZE ];
        };
        *((struct wrap  *)x) = *((struct wrap  *)y);
}

/* Вычисление длины цепочки x. В e заносится адрес последней
   ячейки. */
unsigned int len_chain( lcell *x, lcell **e ){
	unsigned int count;
	
	count = 0;
	*e = x;
	if( x ){
		count++;
		while( (*e)->link != LNULL ){
			*e = (*e)->link;
			count++;
		}
	}
	return count;
}

/* Освобождение ОП, занятой под ячеистую память. Ссылки на
   не освобожденные явно цепочки становятся недействительными.
*/
void free_lstor( void ){
      Avail = NULL;
      free_lbchain();
}
