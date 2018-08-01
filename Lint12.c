/* $Id: Lint12.c,v 1.1.1.1 2005-11-20 11:36:52 roma Exp $ */

/* lint12.c (12-���⮢�� ��䬥⨪�) */

#include <stdlib.h> 
#include <assert.h>
#include "lint12.h"

/* ���᫥��� ������⢠ ������ ࠧ�冷� �᫠ x. ��᫮ ����� 
   ���� 12 ��� 24-���⮢� (l == 1 ��� 2 ᮮ⢥��⢥���). */
static int lendt( LDIGITC x, int l ){
        int m;

        for( m = l*LDIGIT_SIZE; m > 0 && x[ m - 1 ] == 0; m-- ) ;
        return m;
}

/* ������� 24-���⮢��� �᫠ (������ ���� - xl, ����� - xh) 
   �� �᫮ y. ������⮬ ���� ��⭮� q � ���⮪ r. ����� 
   �஢�ન �ᮡ�� ��砥� ������� �� 0 ��� ��९������� (��⭮� 
   �������� ����� 12 ����) �� �ந�������� (�. �맮�� assert). */
void lint12div( LDIGITC xl, LDIGITC xh, LDIGIT y, LDIGIT q, LDIGIT r ){
        int m, n, i, j, k;
        byte x[ 2*LDIGIT_SIZE + 1 ], d, t;

        m = lendt( y, 1 ) - 1;
        lint12copy( x, xl );
        lint12copy( x + LDIGIT_SIZE, xh );
        n = lendt( x, 2 );

        assert( m >= 0 );
        assert( n - m <= (int)(LDIGIT_SIZE + 1) );

        lint12zero( q );

        if( n < m + 1 ){
                lint12copy( r, x );
                return;
        }

        lint12zero( r );

        /* ��砩 ����ࠧ�來��� ����⥫� */
        if( m == 0 ){
                byte xq;

                for( i = n - 1; i >= 0; i-- ){
                        r[ 0 ] = lint1div( x[ i ], r[ 0 ], y[ 0 ], &xq);
                        if( i < LDIGIT_SIZE )
                                q[ i ] = xq;
                }
                return;
        }

        /* ��ଠ������ */
        LintCf = 0;
        d = lint1add( y[ m ], 1 );
        if( LintCf ){
                /* y[ m ] == LINT_BETA - 1 */
                d = 1;
                LintCf = 0;
        }
        else
                lint1div( 0, 1, d, &d );
        if( d != 1 ){
                byte w[ 2 ];

                t = 0;
                for( i = 0; i < n; i++ ){
                        lint1mul( x[ i ], d, w );
                        /* LintCf == 0 */
                        x[ i ] = lint1add( w[ 0 ], t );
                        t = lint1add( w[ 1 ], 0 );
                        /* LintCf == 0 */
                }
                x[ n ] = t;
                t = 0;
                for( i = 0; i <= m; i++ ){
                        lint1mul( y[ i ], d, w );
                        /* LintCf == 0 */
                        y[ i ] = lint1add( w[ 0 ], t );
                        t = lint1add( w[ 1 ], 0 );
                        /* LintCf == 0 */
                }
                /* t == 0 */
        }
        else
                x[ n ] = 0;

        /* 横� ������� */
        for( i = n; i > m; i-- ){
                byte xq, xr;

                /* �業�� ���� xq ��⭮�� */
                if( x[ i ] < y[ m ] ){
                        /* xq = (x[ i ]*LINT_BETA + x[ i - 1 ])/y[ m ]*/
                        xr = lint1div( x[ i - 1 ], x[ i ], y[ m ], &xq);
                        if( xq == 0 )
                                goto ASSIGN;
                }
                else {
                        /* xq = LINT_BETA - 1 */
                        xq = LINT_BETA - 1;
                        /* LintCf == 0 */
                        xr = lint1add( x[ i - 1 ], y[ m ] );
                }
                if( !LintCf ){
                        byte w[ 2 ];

                        /* xq == q || xq == q + 1 || xq == q + 2 */
                        lint1mul( y[ m - 1 ], xq, w );
                        while( w[ 1 ] > xr ||
                              (w[ 1 ] == xr && w[ 0 ] > x[ i - 2 ]) ){
                                xq = lint1sub( xq, 1 );
                                xr = lint1add( xr, y[ m ] );
                                if( LintCf ){
                                        /* ������ �����蠥��� 2-�
                                           ����� */
                                        LintCf = 0;
                                        break;
                                }
                                lint1mul( y[ m - 1 ], xq, w );
                                /* �᫨ xq == 0, � xr > 0. ����.
                                   w[ 1 ] < xr. */
                                /* xq == q || xq == q + 1 */
                        }
                        if( xq == 0 )
                                goto ASSIGN;
                }
                LintCf = 0;

                /* ���᫥��� �஬����筮�� ���⪠ */
                t = 0;
                for( j = 0, k = i - m - 1; j <= m; j++ ){
                        byte w[ 2 ];

                        lint1mul( y[ j ], xq, w );
                        /* LintCf == 0 */
                        w[ 0 ] = lint1add( w[ 0 ], t );
                        w[ 1 ] = lint1add( w[ 1 ], 0 );
                        /* LintCf == 0 �.�. w[ 1 ] < LINT_BETA - 1 */
                        x[ k + j ] = lint1sub( x[ k + j ], w[ 0 ] );
                        t = lint1add( w[ 1 ], 0 );
                        /* LintCf == 0 */
                }
                /* t >= x[ n ] */
                lint1sub( x[ i ], t );

                if( LintCf ){
                        /* ����������饥 ᫮����� */
                        LintCf = 0;
                        xq = lint1sub( xq, 1 );
                        /* LintCf == 0 �.�. xq > 0 �� ���⠭�� */
                        for( j = 0, k = i - m - 1; j <= m; j++ )
                                x[ k + j ] = lint1add( x[ k + j ],
                                                       y[ j ] );
                        LintCf = 0;
                }

ASSIGN:         k = i - m - 1;
                if( k < LDIGIT_SIZE )
                        q[ k ] = xq;
        }

        /* ������� �� ��ଠ�����騩 �����⥫� */
        if( d != 1 ){
                t = 0;
                for( i = m; i >= 0; i-- )
                        t = lint1div( y[ i ], t, d, y + i );
                /* t == 0 */
                for( i = m; i >= 0; i-- )
                        t = lint1div( x[ i ], t, d, r + i );
        }
        else
                for( i = m; i >= 0; i-- )
                        r[ i ] = x[ i ];
}

/* z (24 ����) = x*y. */
void lint12mul( LDIGITC x, LDIGITC y, LDIGIT z ){
        int i, m;

        LintCf = 0;
        lint12zero( z );
        lint12zero( z + LDIGIT_SIZE );
        m = lendt( y, 1 ) - 1;
        for( i = 0; i < LDIGIT_SIZE; i++ ){
                int j;
                byte t;

                if( x[ i ] == 0 )
                        continue;
                t = 0;
                for( j = 0; j <= m; j++ ){
                        byte w[ 2 ];

                        lint1mul( x[ i ], y[ j ], w );
                        /* LintCf == 0 */
                        w[ 0 ] = lint1add( w[ 0 ], t );
                        w[ 1 ] = lint1add( w[ 1 ], 0 );
                        /* LintCf == 0 */
                        z[ i + j ] = lint1add( z[ i + j ], w[ 0 ] );
                        t = lint1add( w[ 1 ], 0 );
                        /* LintCf == 0 */
                }
                z[ i + j ] = t;
        }
}

/* �ࠢ����� x � y. �����頥⬮� ���祭��:
        x < y - -1;
        x > y - +1;
        x = y -  0.     */
int lint12cmp( LDIGITC x, LDIGITC y ){
        int j, c;

        c = 0;
        for( j = LDIGIT_SIZE - 1; j >= 0 && !c; j-- )
                if( x[ j ] < y[ j ] )
                        c = -1;
                else
                if( x[ j ] > y[ j ] )
                        c = +1;
        return c;
}

/* z = x - y. 
   � ��砥 ����� ࠧ����� ����砥��� � �������⥫쭮� ���� 
   � 䫠� LintCf ��⠭���������� � 1. */
void lint12sub( LDIGITC x, LDIGITC y, LDIGIT z ){
        int j;

        for( j = 0; j < LDIGIT_SIZE; j++ )
                z[ j ] = lint1sub( x[ j ], y[ j ] );
}

/* z = x + y.
   �� ������������� ��९������� 䫠� LintCf ��⠭���������� � 1. */
void lint12add( LDIGITC x, LDIGITC y, LDIGIT z ){
        int j;

        for( j = 0; j < LDIGIT_SIZE; j++ )
                z[ j ] = lint1add( x[ j ], y[ j ] );
}

/* ���뢠��� �᫠ �� ��ப� �� s.
   t - 㪠��⥫� �� ����� ��ப� (t > s).
   ��稭�� � ᨬ���� t - 1 � �������� � ��砫� ��ப� ���뢠��
   ���ᨬ� LDIGIT_SIZE ࠧ�冷� � �᫮ x. �����頥� 㪠��⥫�
   �� � ���� s, ��㤠 ����� ������� ᫥���饥 ���뢠���.
   ����砫쭮 x ������ ���� ࠢ�� 0.    */
const char * lint12scan( LDIGIT x, const char * s, const char * t ){
        int j;

        for( j = 0; j < LDIGIT_SIZE && s != t; j++ )
                t = lint1scan( x + j, s, t );
        return t;
}

/* �뢮� �᫠ x � 䠩� out.
   ��ࠬ��� p ����砥� (0) � �몫�砥� (1) �뢮� ������ �㫥�. */
void lint12out( FILE * out, LDIGITC x, int p ){
        int j;

        j = LDIGIT_SIZE - 1;
        if( p )
                while( x[ j ] == 0 )
                        j--;
        assert( j >= 0 );
        lint1out( out, x[ j-- ], p );
        while( j >= 0 )
                lint1out( out, x[ j-- ], 0 );
}

/* ��ॢ�� �᫠ ⨯� unsigned long � �᫮ ⨯� LDIGIT. */
void lint12long( LDIGIT x, unsigned long v ){
	int i;
	
	lint12zero( x );
	for( i = 0; i < LDIGIT_SIZE; i++ ){
		x[ i ] = (char)v%LINT_BETA;
		v/=LINT_BETA;
	}
}

/* ��ࠡ��뢠�� �।�⠢����� �᫠ x � ���� ��᫥����⥫쭮��
   �������� ���. d[ i ] ᮤ�ন� i-� �������� ���� �᫠
   x (��稭�� � ������ ���).
*/
void lint12dt( LDIGITC x, byte d[ LINT12_NDT ] ){
      int i, j;

      for( i = j = 0; i < LDIGIT_SIZE; i++, j += LINT1_NDT )
            lint1dt( x[ i ], d + j );
}

/*                      12-�������� ���������                         */
/*--------------------------------------------------------------------*/

const byte Lint12Zero [ LDIGIT_SIZE ]
                               = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const byte Lint12One  [ LDIGIT_SIZE ]
                               = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const byte Lint12Beta1[ LDIGIT_SIZE ] = { LINT_BETA1,
                                          LINT_BETA1,
                                          LINT_BETA1,
                                          LINT_BETA1,
                                          LINT_BETA1,
                                          LINT_BETA1,
                                          LINT_BETA1,
                                          LINT_BETA1,
                                          LINT_BETA1,
                                          LINT_BETA1,
                                          LINT_BETA1,
                                          LINT_BETA1 };
