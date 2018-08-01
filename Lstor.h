/* $Id: Lstor.h,v 1.1.1.1 2005-11-20 11:36:48 roma Exp $ */

/* lstor.h */

/*                     УПРАВЛЕНИЕ ЯЧЕИСТОЙ ПАМЯТЬЮ                   */
/*-------------------------------------------------------------------*/

#include <stdio.h>

typedef unsigned char byte; 

typedef struct LCELL  *           LADDRESS;
typedef const struct LCELL  *     LADDRESSC;
typedef byte  *                   LDATA;
typedef const byte  *             LDATAC;

#define LNULL           ((LADDRESS)NULL)
#define LCELL_SIZE      16
#define LCELL_DATA_SIZE (LCELL_SIZE - sizeof( LADDRESS ) )

#define LBLOCK_SIZE     256

struct LCELL{
        byte data[ LCELL_DATA_SIZE ];
        LADDRESS link;
};

/* Создание новой ячейки.
   Возвращает адрес ячейки. */
LADDRESS        new_cell( void );

/* Удаление ячейки.
   "а" - адрес удаляемой ячейки, полученный
   с помощью вызова new_cell.   */
#define del_cell( A )  del_chain( A, A );

/* Удаление цепочки.
   "b" и "e" - адреса первой и последней ячеек цепочки. */
void            del_chain( LADDRESS b, LADDRESS e );

/* Обращение цепочки a.
   В x помещается указатель на полученную цепочку.      */
void            invert_chain( LADDRESS * a );

/* Создание копии цепочки y.
   В x помещается адрес первой ячейки копии, возвращается адрес
   последней ячейки копии.      */
LADDRESS        copy_chain( LADDRESS *x, LADDRESSC y );

/* Вычисление длины цепочки x.
   В e помещается адрес последней ячейки цепочки. */
unsigned int	len_chain( LADDRESS x, LADDRESS *e );

/* Копирование блока данных размера LCELL_DATA_SIZE.
   y - источник, x - приемник. */
void            copy_cell_data( LDATA x, LDATAC y );
  
/* Объем ОП, зарезервированный под хранение
   ячеек (в байтах).    */
unsigned long   lleak( void );

/* Освобождение ОП, занятой под ячеистую память. Ссылки на
   не освобожденные явно цепочки становятся недействительными. */
void free_lstor( void );
