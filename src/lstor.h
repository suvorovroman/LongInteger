/**\file	lstor.h
   \brief	Пул ячеек памяти
   \author	Суворов Роман
   \date	23.11.2024

   $Id: Lstor.h,v 1.1.1.1 2005-11-20 11:36:48 roma Exp $
 */

#include "lstor64.h"

/** Размер значения ячейки в байтах.

    Размер адреса - 4 байта (1 слово). Размер блока - 4096 (1024 слова).
    Размер пула для ячеек в блоке - 1023 (1 указатель на следующий блок).
    Разложение размера пула блока на простые множители - 3*11*31, т.е. 31 ячейка
    по 33 слова. Одно слово в ячейке - указатель на следующую ячейку поэтомы размер
    значения ячейки (33 - 1) = 32 слова или 128 байт.
*/

typedef struct lcell lcell;
typedef unsigned char lcell_byte;

#define LNULL ((lcell*)NULL)

/** Ячейка. */
struct lcell
{
	lcell *link;				/**< Указатель на следующую ячейку в цепочке. */
	lcell_byte data[ LSTOR_CELL_SIZE ];	/**< Данные ячейки. */
};

/* Создание новой ячейки.
   Возвращает адрес ячейки. */
lcell* new_cell( void );

/* Удаление ячейки.
   "а" - адрес удаляемой ячейки, полученный
   с помощью вызова new_cell.   */
#define del_cell( A )  del_chain( A, A );

/* Удаление цепочки.
   "b" и "e" - адреса первой и последней ячеек цепочки. */
void            del_chain( lcell* b, lcell* e );

/* Обращение цепочки a.
   В x помещается указатель на полученную цепочку.      */
void            invert_chain( lcell** a );

/* Создание копии цепочки y.
   В x помещается адрес первой ячейки копии, возвращается адрес
   последней ячейки копии.      */
lcell*        copy_chain( lcell** x, const lcell* y );

/* Вычисление длины цепочки x.
   В e помещается адрес последней ячейки цепочки. */
unsigned int	len_chain( lcell* x, lcell** e );

/* Копирование блока данных размера LCELL_DATA_SIZE.
   y - источник, x - приемник. */
void            copy_cell_data( lcell_byte *x, const lcell_byte *y );
  
/* Объем ОП, зарезервированный под хранение
   ячеек (в байтах).    */
unsigned long   lleak( void );

/* Освобождение ОП, занятой под ячеистую память. Ссылки на
   не освобожденные явно цепочки становятся недействительными. */
void free_lstor( void );
