/* $Id: Lsint.c,v 1.1.1.1 2005-11-20 11:36:52 roma Exp $ */

#include "lsint.h"
#include <stdarg.h>
#include <ctype.h> 
#include <stdlib.h>
#include <assert.h>

#define CLS_SIGN( v )           v->sign &= 0xFE
#define SET_SIGN( v )           v->sign |= 0x01
#define GET_SIGN( v )           (v->sign&0x01)
#define IS_ZERO( v )            (v->x == LNULL)
#define CHECK_ZERO_SIGN( v )    if( IS_ZERO( v ) )\
                                        CLS_SIGN( v )

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
void lsintzero( LSINTEGER *v ){
      CLS_SIGN( v );
      v->x = v->xe = LNULL;
}

/* [ИНИЦИАЛИЗАЦИЯ] Переменной v присваевается значение,
   записанное в строке s.
*/
void lsintstr( LSINTEGER *v, const char *s ){
      while( isspace( *s ) )
              s++;
      CLS_SIGN( v );
      v->sign = 0;
      if( *s == '-' || *s == '+' ){
              if( *s == '-' )
                      SET_SIGN( v );
              s++;
      }
      v->xe = lint( &(v->x), s );
      CHECK_ZERO_SIGN( v );
}

/* [ИНИЦИАЛИЗАЦИЯ] Переменной v присваевается значение x.
*/
void lsintulong( LSINTEGER *v, unsigned long x ){
      CLS_SIGN( v );
      v->xe = lintlong( &(v->x), x );
}

/* [ИНИЦИАЛИЗАЦИЯ] Переменной v присваевается значение x.
*/
void lsintlong( LSINTEGER *v, long x ){
      CLS_SIGN( v );
      if( x < 0 )
              SET_SIGN( v );
      v->xe = lintlong( &(v->x), labs( x ) );
}

/* [ИНИЦИАЛИЗАЦИЯ] Переменной v присваевается значение переменной w.
*/
void lsintlsint( LSINTEGER *v, const LSINTEGER *w ){
      v->sign = w->sign;
      v->xe = lintcopy( &(v->x), w->x );
}

/* [ИНИЦИАЛИЗАЦИЯ] Переменной v присваевается значение, заданное
   указателями на целое неотрицательное число и на его хвост.
   Параметр sign задает знак числа.
*/
void lsintlint( LSINTEGER *v, LINTEGER x, LINTEGER xe, int sign ){
      if( sign )
            SET_SIGN( v );
      else
            CLS_SIGN( v );
      v->x = x;
      v->xe = xe;
      CHECK_ZERO_SIGN( v );
}

/* [РАЗМЕЩЕНИЕ] Копирует значение переменной w в переменную v.
   Перед копированием память, занимаемая предыдущим значением 
   переменной v освобождается.
*/
void lsintcopy( LSINTEGER *v, const LSINTEGER *w ){
        if( v->x != LNULL )
                lintdelex( v->x, v->xe );
        lsintlsint( v, w );
}

/* [РАЗМЕЩЕНИЕ] Переменные v и w обмениваются значениями.
   Обмен происходит без переразмещения памяти.
*/
void lsintexch( LSINTEGER *v, LSINTEGER *w ){
        struct tagLSINTEGER t;

        t.sign = v->sign;
        t.x = v->x;
        t.xe = v->xe;
        v->sign = w->sign;
        v->x = w->x;
        v->xe = w->xe;
        w->sign = t.sign;
        w->x = t.x;
        w->xe = t.xe;
}

/* [РАЗМЕЩЕНИЕ] Освобождает память занимаемую значениями переменных.
   За параметром n указывается список переменных. При этом
   n указывает их количество. Перед повторным использованием после
   освобождения памяти необходима инициализация переменной
   (см. разд. [ИНИЦИАЛИЗАЦИЯ]).
*/
void lsintdel( int n, ... ){
        va_list p;
        int i;

        va_start( p, n );
        for( i = 0; i < n; i++ ){
                LSINTEGER *t;

                t = va_arg( p, LSINTEGER * );
                lintdelex( t->x, t->xe );
        }
        va_end( p );
}

/* Функции раздела [ПРЕДСТАВЛЕНИЕ] образуют итератор, который
   возвращает последовательность десятичных цифр числа. Цифры
   в последовательности упорядочены от старших к младшим.
*/

/* [ПРЕДСТАВЛЕНИЕ] Инициализация итератора l числом v.
*/
void lsintdtstart( LSINTDTLIST *l, LSINTEGER *v ){
      l->v = v;
      invert_chain( &(l->v->x) );
      l->xc = l->v->x;
      if( l->xc != LNULL ){
            lint12dt( l->xc->data, l->d );
            l->i = LINT12_NDT - 1;
            while( l->d[ l->i ] == 0 )
                        (l->i)--;
            assert( l->i >= 0 );
      }
}

/* [ПРЕДСТАВЛЕНИЕ] Получение очередной десятичной цифры из
   итератора l.
*/
LINT_DIGIT lsintdtget( LSINTDTLIST *l ){
      LINT_DIGIT ret;

      ret = l->d[ l->i ];
      (l->i)--;
      if( l->i < 0 ){
            l->xc = l->xc->link;
            if( l->xc != LNULL ){
                  l->i = LINT12_NDT - 1;
                  lint12dt( l->xc->data, l->d );
            }
      }
      return ret;
}

/* [ПРЕДСТАВЛЕНИЕ] Возвращает ненулевое значение если итератор l
   еще содержит цифры.
*/
int lsintdtcont( const LSINTDTLIST *l ){
      return l->xc != LNULL;
}

/* [ПРЕДСТАВЛЕНИЕ] Особождает итератор l после того, как тот
   вернул все цифры. После освобождения итератор может быть
   инициализирован снова.
*/
void lsintdtend( LSINTDTLIST *l ){
      invert_chain( &(l->v->x) );
}

/* [АРИФМЕТИКА] Сравнивает значения переменных v и w. Результат
   сравнения:
      -1 - v < w,
       1 - v > w,
       0 - v = w.
   Для случая когда w = 0, второй параметр может быть NULL (первый
   параметр никогда не должен быть равен NULL).
*/   
int lsintcmp( const LSINTEGER *v, const LSINTEGER *w ){
      if( w == NULL )
            return GET_SIGN( v ) ? -1 : lintcmp( v->x, LNULL );
      if( GET_SIGN( w ) )
            return GET_SIGN( v ) ? lintcmp( w->x, v->x ) : 1;
      else
            return GET_SIGN( v ) ? -1 : lintcmp( v->x, w->x );
}

/* [АРИФМЕТИКА] v <- v + w.
   Оба параметра могут указывать на одно и то же число.
*/
void lsintadd( LSINTEGER *v, const LSINTEGER *w ){
      int sv;

      sv = GET_SIGN( v );
      if( sv != GET_SIGN( w ) ){
            int sign;

            sign = lintsub( &(v->x), &(v->xe), w->x );
            if( sv ){
            	if( sign )
            		CLS_SIGN( v );
            	else
            		CHECK_ZERO_SIGN( v );
            }
            else{
            	if( sign )
                        /* v != 0 */
            		SET_SIGN( v );
            }
      }
      else
            lintadd( &(v->x), &(v->xe), w->x );
}

/* [АРИФМЕТИКА] v <- -v.
*/
void lsintneg( LSINTEGER *v ){
      if( GET_SIGN( v ) )
            CLS_SIGN( v );
      else
      if( v->x != LNULL )
            SET_SIGN( v );
}

/* [АРИФМЕТИКА] v <- v - w.
   Оба параметра могут указывать на одно и то же число.
*/
void lsintsub( LSINTEGER *v, const LSINTEGER *w ){
      int sv;

      sv = GET_SIGN( v );
      if( sv == GET_SIGN( w ) ){
            int sign;

            sign = lintsub( &(v->x), &(v->xe), w->x );
            if( sv ){
                  if( sign )
                        CLS_SIGN( v );
                  else
                        CHECK_ZERO_SIGN( v );
            }
            else
                  if( sign )
                        /* v != 0 */
                        SET_SIGN( v );
      }
      else
            lintadd( &(v->x), &(v->xe), w->x );
}

/* [АРИФМЕТИКА, ИНИЦИАЛИЗАЦИЯ] t <- v*w.
   Параметры v, w могут указывать на одно и то же значение, однако
   ни один из них не может указывать на число t. После выполнения
   функции t указывает на произведение v и w, при этом предполагается,
   что до вызова значение t неопределено (не было инициализированно или
   деинициализировано) или равно 0. */
void lsintmul( LSINTEGER *t, const LSINTEGER *v, const LSINTEGER *w ){
      int sv, sw;

      sv = GET_SIGN( v );
      sw = GET_SIGN( w );
      lintmul( v->x, w->x, &(t->x), &(t->xe) );
      if( IS_ZERO( t ) || sv&&sw || !sv&&!sw )
            CLS_SIGN( t );
      else
            SET_SIGN( t );
}

/* [АРИФМЕТИКА, ИНИЦИАЛИЗАЦИЯ] q <- v div w, v <- v mod w.
   Параметры v и w не должны указывать на одно и то же значение.
   Ни один из них не может указывать на значение q. Значение q
   должно быть неопределено (не инициализированно или 
   деинициализированно) или равно 0. */
void lsintdiv( LSINTEGER *v, const LSINTEGER *w, LSINTEGER *q ){
      int sv, sw;

      assert( !IS_ZERO( w ) );
      sv = GET_SIGN( v );
      sw = GET_SIGN( w );
      lintdiv( &(v->x), &(v->xe), w->x, &(q->x), &(q->xe) );
      if( sv ){
            if( !IS_ZERO( v ) ){
                  /* I. a < 0, b > 0
                        -abs(a) = -abs(b)*q - r = 
                        abs(b)*(-q) - r + abs(b) - abs(b) =
                        abs(b)*(-(q+1)) + (abs(b) - r) = 
                        b*(-(q+1)) + (b - r) = a
                    II. a < 0, b < 0
                        -abs(a) = -abs(b)*(q+1) + (abs(b) - r) =
                        b*(q+1) + (abs(b) - r) = a */
                  LINTEGER x, xe;

                  xe = lintlong( &x, 1 );
                  lintadd( &(q->x), &(q->xe), x );
                  lintdelex( x, xe );
                  lintsub( &(v->x), &(v->xe), w->x );
            }
            if( IS_ZERO( q ) || sw )
                  CLS_SIGN( q );
            else
                  SET_SIGN( q );
            CLS_SIGN( v );
      }
      else
            if( IS_ZERO( q ) || !sw )
                  CLS_SIGN( q );
            else
                  SET_SIGN( q );
}

/* [АРИФМЕТИКА] v <- abs(v). */
void lsintabs( LSINTEGER *v ){
      CLS_SIGN( v );
}

void lsintdouble( LSINTEGER *v, double x ){
	CLS_SIGN(v);
	if(x < 0.0)
		SET_SIGN(v);
	v->xe = lintdouble(&(v->x), x);
}
