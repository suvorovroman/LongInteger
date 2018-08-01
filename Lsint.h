/* $Id: Lsint.h,v 1.1.1.1 2005-11-20 11:36:48 roma Exp $ */

#include <utilib_extern.h>
#include "lint.h"

struct tagLSINTEGER{ 
        char sign;      /* знак минус (используется младший бит) */
        LADDRESS x;     /* указатель на число */
        LADDRESS xe;    /* указатель на хвост числа */
};
typedef struct tagLSINTEGER LSINTEGER;

/* Функции раздела ИНИЦИАЛИЗАЦИЯ предназначены для задания
   начальных значений переменных. Начальное значение должно
   быть присвоено переменной:
        - при первом появлении переменной в программе (до
          того как она будет участвовать в каких-либо
          иных операциях);
        - после деиницилизации переменной функцией
          lsintdel.
*/

/* [ИНИЦИАЛИЗАЦИЯ] Переменной v присваевается 0.
*/
_UTILIB_EXTERN_ void lsintzero( LSINTEGER *v );

/* [ИНИЦИАЛИЗАЦИЯ] Переменной v присваевается значение,
   записанное в строке s.
*/
_UTILIB_EXTERN_ void lsintstr( LSINTEGER *v, const char *s );

/* [ИНИЦИАЛИЗАЦИЯ] Переменной v присваевается значение x.
*/
_UTILIB_EXTERN_ void lsintulong( LSINTEGER *v, unsigned long x );

/* [ИНИЦИАЛИЗАЦИЯ] Переменной v присваевается значение x.
*/
_UTILIB_EXTERN_ void lsintlong( LSINTEGER *v, long x );

/* [ИНИЦИАЛИЗАЦИЯ] Переменной v присваевается значение переменной w.
*/
_UTILIB_EXTERN_ void lsintlsint( LSINTEGER *v, const LSINTEGER *w );

/* [ИНИЦИАЛИЗАЦИЯ] Переменной v присваевается значение, заданное
   указателями на целое неотрицательное число и на его хвост.
   Параметр sign задает знак числа.
*/
_UTILIB_EXTERN_ void lsintlint( LSINTEGER *v, LINTEGER x, LINTEGER xe, int sign );

/* [ИНИЦИАЛИЗАЦИЯ] Переменной v присваевается псевдослучайное
   число длины length десятичных цифр.
*/
_UTILIB_EXTERN_ void lsintrand( LSINTEGER *v, int length );

_UTILIB_EXTERN_ void lsintdouble( LSINTEGER *v, double x );

/* [РАЗМЕЩЕНИЕ] Копирует значение переменной w в переменную v.
   Перед копированием память, занимаемая предыдущим значением 
   переменной v освобождается.
*/
_UTILIB_EXTERN_ void lsintcopy( LSINTEGER *v, const LSINTEGER *w );

/* [РАЗМЕЩЕНИЕ] Переменные v и w обмениваются значениями.
   Обмен происходит без переразмещения памяти.
*/
_UTILIB_EXTERN_ void lsintexch( LSINTEGER *v, LSINTEGER *w );

/* [РАЗМЕЩЕНИЕ] Освобождает память занимаемую значениями переменных.
   За параметром n указывается список переменных. При этом
   n указывает их количество. Перед повторным использованием после
   освобождения памяти необходима инициализация переменной
   (см. разд. [ИНИЦИАЛИЗАЦИЯ]).
*/
_UTILIB_EXTERN_ void lsintdel( int n, ... );

struct tagLSINTDTLIST{
      LSINTEGER *v;
      LADDRESS  xc;
      byte       d[ LINT12_NDT ];
      int        i;
};
typedef struct tagLSINTDTLIST LSINTDTLIST;

/* Функции раздела [ПРЕДСТАВЛЕНИЕ] образуют итератор, который
   возвращает последовательность десятичных цифр числа. Цифры
   в последовательности упорядочены от старших к младшим.
*/

/* [ПРЕДСТАВЛЕНИЕ] Инициализация итератора l числом v.
*/
_UTILIB_EXTERN_ void lsintdtstart( LSINTDTLIST *l, LSINTEGER *v );

/* [ПРЕДСТАВЛЕНИЕ] Получение очередной десятичной цифры из
   итератора l.
*/
_UTILIB_EXTERN_ byte lsintdtget( LSINTDTLIST *l );

/* [ПРЕДСТАВЛЕНИЕ] Возвращает ненулевое значение если итератор l
   еще содержит цифры.
*/
_UTILIB_EXTERN_ int lsintdtcont( const LSINTDTLIST *l );

/* [ПРЕДСТАВЛЕНИЕ] Особождает итератор l после того, как тот
   вернул все цифры. После освобождения итератор может быть
   инициализирован снова.
*/
_UTILIB_EXTERN_ void lsintdtend( LSINTDTLIST *l );

/* [АРИФМЕТИКА] Сравнивает значения переменных v и w. Результат
   сравнения:
      -1 - v < w,
       1 - v > w,
       0 - v = w.
   Для случая когда w = 0, второй параметр может быть NULL (первый
   параметр никогда не должен быть равен NULL).
*/   
_UTILIB_EXTERN_ int lsintcmp( const LSINTEGER *v, const LSINTEGER *w );

/* [АРИФМЕТИКА] v <- v + w.
   Оба параметра могут указывать на одно и то же число.
*/
_UTILIB_EXTERN_ void lsintadd( LSINTEGER *v, const LSINTEGER *w );

/* [АРИФМЕТИКА] v <- v - w.
   Оба параметра могут указывать на одно и то же число.
*/
_UTILIB_EXTERN_ void lsintsub( LSINTEGER *v, const LSINTEGER *w );

/* [АРИФМЕТИКА] v <- -v.
*/
_UTILIB_EXTERN_ void lsintneg( LSINTEGER *v );

/* [ВЫВОД] Вывод числа v в файл fout.
*/
_UTILIB_EXTERN_ void lsintout( LSINTEGER *v, FILE *fout );

/* [АРИФМЕТИКА, ИНИЦИАЛИЗАЦИЯ] t <- v*w.
   Параметры v, w могут указывать на одно и то же значение, однако
   ни один из них не может указывать на значение t. После выполнения
   функции t указывает на произведение v и w, при этом предполагается,
   что до вызова значение t неопределено (не было инициализировано или
   деинициализировано) или равно 0. */
_UTILIB_EXTERN_ void lsintmul( LSINTEGER *t, const LSINTEGER *v, const LSINTEGER *w );

/* [АРИФМЕТИКА, ИНИЦИАЛИЗАЦИЯ] q <- v div w, v <- v mod w.
   Параметры v и w не должны указывать на одно и то же значение.
   Ни один из них не может указывать на значение q. Значение q
   должно быть неопределено (не инициализированно или 
   деинициализированно) или равно 0. */
_UTILIB_EXTERN_ void lsintdiv( LSINTEGER *v, const LSINTEGER *w, LSINTEGER *q );

/* [АРИФМЕТИКА] v <- abs(v). */
_UTILIB_EXTERN_ void lsintabs( LSINTEGER *v );

/* [ДОПОЛНЕНИЯ, НОД] Алгоритм Евклида (Euclidean Algorithm). 
      x <- НОД(x, y)
      y <- 0
   Источник:
      А.Акритас, Основы компьютерной алгебры с приложениями
      М.:Мир, 1994, алгоритм EA. */
_UTILIB_EXTERN_ void lsintgcd( LSINTEGER *x, LSINTEGER *y );

/* [ДОПОЛНЕНИЯ, НОД] Вычисление наименьшего обшего кратного.
      x <- НОК(x, y)
      y <- НОД(x, y)
   Истичник:
      А.Акритас, основы компьютерной алгебры с приложениями.
      М.:Мир, 1994, определение 2.2.7. */
_UTILIB_EXTERN_ void lsintlcm( LSINTEGER *x, LSINTEGER *y );