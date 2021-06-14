/* $Id: Lint.c,v 1.1.1.1 2005-11-20 11:36:52 roma Exp $ */

/* lint.c (арифметика целых неотрицательных чисел) */

#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> 
#include "lint.h"

/* Преобразует строку от s до t в число x.
   Возвращает адрес последней ячейки числа.     */
static LADDRESS lintscan( LINTEGER * x,
                          const char * s, const char * t ){
        LADDRESS c;

        *x = c = LNULL;
        while( s != t ){
                if( c != LNULL ){
                        c->link = new_cell();
                        c = c->link;
                }
                else
                        *x = c = new_cell();
                lint12zero( c->data );
                t = lint12scan( c->data, s, t );
        }
        if( c != LNULL )
                c->link = LNULL;        
        return c;
}

/* Преобразование строки s в целое число x. 
   Функция удаляет ведушие пробелы, а затем преобразует в число часть 
   строки до первого нецифрового символа. 
   Функция  заносит в x указатель на первую ячейку числа и возвращает 
   указатель на последнюю ячейку числа.
   Если строка не может быть преобразована в число, то x и возвращае-
   мое значение равны LNULL. */
LADDRESS lint( LINTEGER * x, const char * s ){
        const char * t;

        while( isspace( *s ) || *s == '0' )
                s++;
        t = s;
        while( isdigit( *t ) )
                t++;
        return lintscan( x, s, t );
}

/* Вывод числа x в файл out.
   Число печатается без ведущих нулей. */
void lintout( FILE * out, LINTEGER x ){
        LADDRESS c;

        invert_chain( &x );
        c = x;
        if( c != LNULL ){
                lint12out( out, c->data, 1 );
                c = c->link;
                while( c != LNULL ){
                        lint12out( out, c->data, 0 );
                        c = c->link;
                }
        }
        else
                fputc( '0', out );
        invert_chain( &x );
}

/* Удаление числа x. */
void lintdel( LINTEGER x ){
        while( x != LNULL ){
                LADDRESS c;
        
                c = x;
                x = x->link;
                del_cell( c );
        }
}

/* Быстрое удаление числа x (когда известен e - указатель на последнюю
   ячейку числа ). */
void lintdelex( LINTEGER x, LADDRESS e ){
        if( x != LNULL )
                del_chain( x, e );
}

/* x = x + y
   Если длина числа увеличилась, то в xe заносится указатель на
   последнюю ячейку числа. В противном случае значение xe не
   меняется. 
   x и y могут указывать на одну и ту же цепочку.
*/
void lintadd( LINTEGER *x, LADDRESS *xe, LINTEGERC y ){
        LADDRESS xc, xt;

        LintCf = 0;
        
        if( y == LNULL )
                return;
        if( *x == LNULL ){
                *xe = lintcopy( x, y );
                return;
        }
        
        xc = *x;
        do{
                xt = xc;
                lint12add( xt->data, y->data, xt->data );
                xc = xc->link;
                y = y->link;
        }
        while( y != LNULL && xc != LNULL );
        
        if( y != LNULL ){
                do{ 
                        xt->link = new_cell();
                        xt = xt->link;
                        lint12add( Lint12Zero, y->data, xt->data );
                        y = y->link;
                }
                while( y != LNULL );
                xt->link = LNULL;
                *xe = xt;
        }
        
        if( LintCf ){
                while( xt->link != LNULL && LintCf ){
                        xt = xt->link;
                        lint12add( xt->data, Lint12Zero, xt->data );
                }
                if( LintCf ){
                        xt->link = new_cell();
                        *xe = xt = xt->link;
                        lint12copy( xt->data, Lint12One );
                        xt->link = LNULL;
                }
        }
}

/* x = y*z
   В xe заносится указатель на последнюю ячейку числа. 
   x не должно указывать на ту же цепочку, что y или z.
   y и z могут указывать на одну и ту же цепочку.
   В ходе вычислений старое значение x теряется.
*/
void lintmul( LINTEGERC y, LINTEGERC z, LINTEGER *x, LADDRESS *xe ){
        LADDRESS xc, xs, xt;
        LADDRESSC yc;
        byte t[ LDIGIT_SIZE ];

        *x = *xe = LNULL;
        LintCf = 0;

        if( z == LNULL || y == LNULL )
                return;

        *x = new_cell();
        yc = y->link;
        xc = *x;
        lint12zero( xc->data );
        while( yc != LNULL ){
                xc->link = new_cell();
                xc = xc->link;
                lint12zero( xc->data );
                yc = yc->link;
        }

        xs = *x;
        do{
                lint12zero( t );
                yc = y;
                xc = xs;
                do{
                        byte w[ 2 ][ LDIGIT_SIZE ];
        
                        lint12mul( yc->data, z->data, (LDIGIT)w );
                        /* LintCf == 0 */
                        lint12add( w[ 0 ], t, w[ 0 ] );
                        lint12add( w[ 1 ], Lint12Zero, w[ 1 ] );
                        /* LintCf == 0 */
                        xt = xc;
                        lint12add( xt->data, w[ 0 ], xt->data );
                        lint12add( w[ 1 ], Lint12Zero, t );
                        yc = yc->link;
                        xc = xc->link;
                }
                while( yc != LNULL );
                z = z->link;
                if( z != LNULL ){
                        xt->link = new_cell();
                        lint12copy( xt->link->data, t );
                }         
                xs = xs->link;
        }
        while( z != LNULL );

        if( lint12cmp( t, Lint12Zero ) ){
                xt->link = new_cell();
                xt = xt->link;
                lint12copy( xt->data, t );
        }
        xt->link = LNULL;
        *xe = xt;
}

/* Умножение числа на одноразрядный делитель (число должно
   быть как минимум 2-х разрядным). Предполагается что число 
   храниться начиная со старших разрядов. Считается так же, 
   что число содержит достаточное количество разрядов для 
   хранения полученного произведения (переноса за старший 
   разряд быть не должно). 
   Флаг переноса должен быть сброшен, по возвращении он
   остается сброшен. */
static void mulbyd( LINTEGER x, LDIGITC d ){
        LADDRESS xt, xp;
        byte w[ 2 ][ LDIGIT_SIZE ];

        lint12mul( x->data, d, (LDIGIT)w );
        lint12copy( x->data, w[ 0 ] );
        /* старший разряд произведения не учитывается */
        xt = LNULL;
        xp = x;
        x = x->link;
        do{
                lint12mul( x->data, d, (LDIGIT)w );
                lint12copy( x->data, w[ 0 ] );
                lint12add( xp->data, w[ 1 ], xp->data );
                if( LintCf ){
                        assert( xt != LNULL );
                        lint12add( xt->data, Lint12Zero, xt->data );
                        /* LintCf == 0 */
                        xt = xt->link;
                        while( xt != xp ){
                                lint12zero( xt->data );
                                xt = xt->link;
                        }
                        /* т.к. возникло переполнение, то
                           tp == xp может принять 1 без переполнения */
                }
                else
                if( lint12cmp( xp->data, Lint12Beta1 ) )
                        xt = xp;
                xp = x;
                x = x->link;
        }
        while( x != LNULL );
}

/* x = x%y, q = x/y
   В xe заносится указатель на последнюю ячейку остатка, а в qe 
   указатель на последнюю ячейку частного. 
   Никакая пара из x, y, q не должна ссылаться на одну и ту же
   цепочку.
   В ходе вычислений старое значение q теряется.
*/
void lintdiv( LINTEGER *x, LADDRESS *xe, LINTEGER y,
              LINTEGER *q, LADDRESS *qe ){
        LADDRESS xc, yc, xtmp, xbar, xt, xp;
        byte xq[ LDIGIT_SIZE ], xr[ LDIGIT_SIZE ], d[ LDIGIT_SIZE ];
        int comp;

        *q = *qe = LNULL;

        if( *x == LNULL || y == LNULL )
                return;

        invert_chain( x );
        invert_chain( &y );

        if( y->link == LNULL ){
                /* случай одноразрядного делителя */
                lint12zero( xr );
                xc = *x;
                do{
                        lint12div( xc->data, xr, y->data, xq, xr );
                        if( *q != LNULL || lint12cmp( xq, Lint12Zero )){
                                xtmp = new_cell();
                                lint12copy( xtmp->data, xq );
                                xtmp->link = *q;
                                *q = xtmp;
                                if( *qe == LNULL )
                                        *qe = xtmp;
                        }
                        xtmp = xc;
                        xc = xc->link;
                }
                while( xc != LNULL );
                /* xr == остаток от x/y */
                del_chain( *x, xtmp );
                if( lint12cmp( xr, Lint12Zero ) ){
                        *x = *xe = new_cell();
                        lint12copy( (*x)->data, xr );
                        (*x)->link = LNULL;
                }
                else
                        *x = *xe = LNULL;
                return;
        }

        /* xbar указывает на цифру уменьшаемого, из которой
           вычитается самая младшая цифра вычитаемого;
           xbar перемещается вправо по делимому; достижение
           конца числа говорит о завершение цикла деления */
        yc = y;
        xbar = *x;
        while( yc->link != LNULL && xbar->link != LNULL ){
                yc = yc->link;
                xbar = xbar->link;
        }
        if( yc->link != LNULL )
                /* делимое короче делителя */
                goto EXIT;
        if( xbar == LNULL && lint12cmp( (*x)->data, y->data ) < 0 )
                /* делитель и делимое одинаковой длины, но
                   старшая цифра делимого меньше старшей цифры
                   делителя */
                goto EXIT;

        /* нормализация делителя
           при нормализации длина делителя остается прежней, а
           длина делимого увеличивается на 1; вычитание из
           делимого начинается со второй цифры */
        LintCf = 0;
        xtmp = new_cell();
        lint12zero( xtmp->data );
        xtmp->link = *x;
        *x = xtmp;

        /* d = Lint12Beta/(y->data + 1) */
        lint12add( y->data, Lint12One, xq );
        if( LintCf ){
                LintCf = 0;
                lint12copy( d, Lint12One );
        }
        else
                lint12div( Lint12Zero, Lint12One, xq, d, xr );
        if( lint12cmp( d, Lint12One ) ){
                mulbyd( y, d );
                mulbyd( *x, d );
        }
        /* LintCf == 0 */

        /* цикл деления */
        do{
                /* оценка цифры частного */
                if( lint12cmp( (*x)->data, y->data ) < 0 ){
                        lint12div( (*x)->link->data,
                                   (*x)->data,
                                   y->data,
                                   xq, xr );
                        if( !lint12cmp( xq, Lint12Zero ) )
                                if( *q == LNULL )
                                        goto NEXT;
                                else
                                        goto ASSIGN;
                }
                else{
                        lint12copy( xq, Lint12Beta1 );
                        lint12add( (*x)->link->data, y->data, xr );
                }
                if( !LintCf ){
                        byte w[ 2 ][ LDIGIT_SIZE ];
                        int c;

                        lint12mul( y->link->data, xq, (LDIGIT)w );
                        c = lint12cmp( w[ 1 ], xr );
                        while( c > 0 || (c == 0 && lint12cmp( w[ 0 ], 
                              (*x)->link->link->data ) > 0 ) ){
                                lint12sub( xq, Lint12One, xq );
                                lint12add( xr, y->data, xr );
                                if( LintCf ){
                                        LintCf = 0;
                                        break;
                                }
                                lint12mul( y->link->data, xq,(LDIGIT)w);
                                c = lint12cmp( w[ 1 ], xr );
                        }
                        if( !lint12cmp( xq, Lint12Zero ) )
                                if( *q == LNULL )
                                        goto NEXT;
                                else
                                        goto ASSIGN;
                }
                else
                        LintCf = 0;

                /* вычитание из промежуточного делимого
                   делителя, умноженного на разряд частного */
                comp = 0;
                yc = y;
                xt = LNULL;
                xp = *x;
                xc = xp->link;
                do{
                        byte w[ 2 ][ LDIGIT_SIZE ];

                        lint12mul( yc->data, xq, (LDIGIT)w );
                        lint12sub( xc->data, w[ 0 ], xc->data );
                        lint12sub( xp->data, w[ 1 ], xp->data );
                        if( LintCf ){
                                if( xt != LNULL ){
                                        lint12sub( xt->data, 
                                        Lint12Zero, 
                                        xt->data );
                                        xt = xt->link;
                                        while( xt != xp ){
                                                lint12copy( xt->data, 
                                                Lint12Beta1 );
                                                xt = xt->link;
                                        }
                                }
                                else
                                        /* будет выполнятся
                                           компенсирующее сложение */
                                        comp = 1;
                        }
                        else
                        if( lint12cmp( xp->data, Lint12Zero ) )
                                xt = xp;
                        xp = xc;
                        xc = xc->link;
                        yc = yc->link;
                }
                while( yc != LNULL );
                if( !comp )
                        goto ASSIGN;

                /* выполнение компенсирующего сложения */
                yc = y;
                xt = *x;
                xc = xt->link;
                do{
                        lint12add( xc->data, yc->data, xc->data );
                        if( LintCf && xt != LNULL ){
                                lint12add( xt->data, Lint12Zero,
                                           xt->data );
                                xt = xt->link;
                                while( xt != xc ){
                                        lint12zero( xt->data );
                                        xt = xt->link;
                                }
                        }
                        else
                        if( lint12cmp( xc->data, Lint12Beta1 ) )
                                xt = xc;
                        xc = xc->link;
                        yc = yc->link;
                }
                while( yc != LNULL );

                /* добавить очередную цифру к частному */
ASSIGN:         xtmp = new_cell();
                lint12copy( xtmp->data, xq );
                xtmp->link = *q;
                *q = xtmp;
                if( *qe == LNULL )
                        *qe = xtmp;

                /* сдвиг вправо;
                   старший разряд промежуточного делимого
                   удаляется */
NEXT:           xtmp = *x;
                *x = (*x)->link;
                del_cell( xtmp );
                xbar = xbar->link;
        }
        while( xbar != LNULL );

        /* денормализация делителя и делимого */
        if( lint12cmp( d, Lint12One ) ){
                byte t[ LDIGIT_SIZE ];

                lint12zero( t );
                yc = y;
                do{
                        lint12div( yc->data, t, d, yc->data, t );
                        yc = yc->link;
                }
                while( yc != LNULL );
                /* t == 0 */

                xc = *x;
                do{
                        lint12div( xc->data, t, d, xc->data, t );
                        xc = xc->link;
                }
                while( xc != LNULL );
        }

        /* удаление ведущих нулей остатка */
        xtmp = LNULL;
        xc = *x;
        while( xc != LNULL && !lint12cmp( xc->data, Lint12Zero ) ){
                xtmp = xc;
                xc = xc->link;
        }
        if( xtmp != LNULL ){
                del_chain( *x, xtmp );
                *x = xc;
        }

EXIT:
        *xe = *x;
        invert_chain( x );
        invert_chain( &y );
}

/* Сравнение x и y. Возвращаемое значение:
        x < y - -1;
        x > y - +1;
        x = y -  0.     
   x и y могут ссылаться на одну и ту же цепочку.
*/
int lintcmp( LINTEGERC x, LINTEGERC y ){
        int c;

        c = 0;
        while( x != LNULL && y != LNULL ){
                int cm;

                cm = lint12cmp( x->data, y->data );
                if( cm )
                        c = cm;
                x = x->link;
                y = y->link;
        }
        if( x == LNULL && y != LNULL )
                return -1;
        else
        if( x != LNULL && y == LNULL )
                return 1;
        else
                return c;
}

/* x = x - y
   Если длина x изменилась, то в xe заносится указатель на последнюю
   ячейку числа. В противном случае значение xe не изменяется. 
   x и y могут ссылаться на одну и ту же цепочку. 
*/
int lintsub( LINTEGER *x, LADDRESS *xe, LINTEGERC y ){
        LADDRESS xc, xt;
        int sign;

        LintCf = 0;
        
        if( y == LNULL )
                return 0;
        if( *x == LNULL ){
                *xe = lintcopy( x, y );
                return -1;
        }

        xc = *x;
        do{
                xt = xc;
                lint12sub( xt->data, y->data, xt->data );
                xc = xc->link;
                y = y->link;
        }
        while( y != LNULL && xc != LNULL );
        
        if( y != LNULL ){
                do{ 
                        xt->link = new_cell();
                        xt = xt->link;
                        lint12sub( Lint12Zero, y->data, xt->data );
                        y = y->link;
                }
                while( y != LNULL );
                xt->link = LNULL;
                *xe = xt;
        }
        else 
                while( LintCf && xt->link != LNULL ){
                        xt = xt->link;
                        lint12sub( xt->data, Lint12Zero, xt->data );
                }

        if( LintCf ){
                /* разность в дополнительном коде -
                   разность вычитается из 0 */
                LintCf = 0;
                xc = *x;
                do{
                        lint12sub( Lint12Zero, xc->data, xc->data );
                        xc = xc->link;
                }
                while( xc != LNULL );
                sign = -1;
        }
        else
                sign = 0;

        if( xt->link == LNULL && !lint12cmp( xt->data, Lint12Zero ) ){
                LADDRESS xs;

                /* удаление ведущих нулевых разрядов */
                xs = LNULL;
                xc = *x;
                do{
                        if( lint12cmp( xc->data, Lint12Zero ) )
                                xs = xc;
                        xc = xc->link;
                }
                while( xc != LNULL );
                if( xs == LNULL ){
                        del_chain( *x, xt );
                        *x = xt = LNULL;
                }
                else{
                        del_chain( xs->link, xt );
                        xt = xs;
                        xt->link = LNULL;
                }
                *xe = xt;
        }

        return sign;
}

/* Преобразование длинного беззнакового целого. Полученное число
   всегда состоит из одной ячейки (ее адрес присваевается x и
   возвращается в качестве результата). */
LADDRESS lintlong( LINTEGER *x, unsigned long v ){
        if( v != 0 ){
                *x = new_cell();
                lint12long( (*x)->data, v );
                (*x)->link = LNULL;
        }
        else
                *x = LNULL;
        return *x;
}

LADDRESS lintdouble(LINTEGER *x, double v){
	LADDRESS xe;
	double r;
	int i;

	*x = xe = LNULL;
	i = LDIGIT_SIZE;
	v = fabs(v);
	while(v >= 1.0){
		r = fmod(v, LINT_BETA);
		if(i >= LDIGIT_SIZE){
			if(xe == LNULL)
				xe = *x = new_cell();
			else{
				xe->link = new_cell();
				xe = xe->link;
			}
			xe->link = LNULL;
			i = 0;
		}
		xe->data[i++] = (int)r;
		v = (v - r)/LINT_BETA;
	}
	while(i > 0 && i < LDIGIT_SIZE)
		xe->data[i++] = 0;
	return xe;
}
