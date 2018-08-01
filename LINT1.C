/* $Id: LINT1.C,v 1.1.1.1 2005-11-20 11:36:52 roma Exp $ */

/* lint1.c (однобайтовая арифметика) */

#include "lint1.h" 

byte LintCf = 0;        /* флаг переноса/заема */

/* Сложение чисел x, y с учетом LintCf.
   Возвращает сумму по модулю LINT_BETA. При возникновении переноса
   LintCf устанавливается в 1. */
byte lint1add( byte x, byte y ){
        int t;

        t = x + y + LintCf;
        LintCf = (byte)(t/LINT_BETA);
        return (byte)(t%LINT_BETA);
}

/* Разность чисел x, y с учетом LintCf.
   Если результат лежит в диапазоне -LINT_BETA...-1, то он
   увеличивается на LINT_BETA и LintCf устанавливается в 1. */
byte lint1sub( byte x, byte y ){
        int t;

        t = x + LINT_BETA - y - LintCf;
        LintCf = (byte)((t >= LINT_BETA) ? 0:1);
        return (byte)(t%LINT_BETA);
}

/* Умножение чисел x, y.
   Возвращает в z[ 0 ] младший разряд произведения, а
   а старший разряд - в z[ 1 ].        */
void lint1mul( byte x, byte y, byte z[ 2 ] ){
        int t;

        t = x*y;
        z[ 0 ] = (byte)(t%LINT_BETA);
        z[ 1 ] = (byte)(t/LINT_BETA);
}

/* Деление числа x = xh*LINT_BETA + xl на y (x/y < LINT_BETA).
   Возвращает остаток, а в q помещается частное.     */
byte lint1div( byte xl, byte xh, byte y, LDATA q ){
        int t;

        t = xh*LINT_BETA + xl;
        *q = (byte)(t/y);
        return (byte)(t%y);
}

/* Считывание разряда из строки s.
   t - указатель на конец строки (t > s).
   Начиная с символа t - 1 и двигаясь к началу строки считывает
   разряд и помещает его в x. Возвращает указатель на то место s,
   откуда может начаться следующее считывание.  */
const char * lint1scan( LDATA x, const char * s, const char * t ){
        int i1, i2;
 
        t--;
        i1 = *t - '0';
        i2 = 0;
        if( s != t ){
                t--;
                i2 = *t - '0';
        }
        *x = (byte)(i2*10 + i1);
        return t;
}

/* Вывод однобайтового числа x в файл out.
   Параметр p включает (1) и выключает (0) вывод ведущего нуля. */
void lint1out( FILE * out, byte x, int p ){
      byte d[ LINT1_NDT ];

      lint1dt( x, d );
      d[ 0 ] += '0';
      d[ 1 ] += '0';
      if( d[ 1 ] != '0' || !p )
                fputc( d[ 1 ], out );
      fputc( d[ 0 ], out );
}

/* Вырабатывает представление числа x в виде последовательности
   десятичных цифр. d[ i ] содержит i-ю десятичную цифру числа
   x (начиная с младших цифр).
*/
void lint1dt( byte x, byte d[ LINT1_NDT ] ){
      d[ 0 ] = x%10;
      d[ 1 ] = x/10;
}
