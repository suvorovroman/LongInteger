/* $Id: Lint.c,v 1.1.1.1 2005-11-20 11:36:52 roma Exp $ */

/* lint.c (��䬥⨪� 楫�� ������⥫��� �ᥫ) */

#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> 
#include "lint.h"

/* �८�ࠧ�� ��ப� �� s �� t � �᫮ x.
   �����頥� ���� ��᫥���� �祩�� �᫠.     */
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

/* �८�ࠧ������ ��ப� s � 楫�� �᫮ x. 
   �㭪�� 㤠��� ����訥 �஡���, � ��⥬ �८�ࠧ�� � �᫮ ���� 
   ��ப� �� ��ࢮ�� ����஢��� ᨬ����. 
   �㭪��  ������ � x 㪠��⥫� �� ����� �祩�� �᫠ � �����頥� 
   㪠��⥫� �� ��᫥���� �祩�� �᫠.
   �᫨ ��ப� �� ����� ���� �८�ࠧ����� � �᫮, � x � �����頥-
   ��� ���祭�� ࠢ�� LNULL. */
LADDRESS lint( LINTEGER * x, const char * s ){
        const char * t;

        while( isspace( *s ) || *s == '0' )
                s++;
        t = s;
        while( isdigit( *t ) )
                t++;
        return lintscan( x, s, t );
}

/* �뢮� �᫠ x � 䠩� out.
   ��᫮ ���⠥��� ��� ������ �㫥�. */
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

/* �������� �᫠ x. */
void lintdel( LINTEGER x ){
        while( x != LNULL ){
                LADDRESS c;
        
                c = x;
                x = x->link;
                del_cell( c );
        }
}

/* ����஥ 㤠����� �᫠ x (����� �����⥭ e - 㪠��⥫� �� ��᫥����
   �祩�� �᫠ ). */
void lintdelex( LINTEGER x, LADDRESS e ){
        if( x != LNULL )
                del_chain( x, e );
}

/* x = x + y
   �᫨ ����� �᫠ 㢥��稫���, � � xe �������� 㪠��⥫� ��
   ��᫥���� �祩�� �᫠. � ��⨢��� ��砥 ���祭�� xe ��
   �������. 
   x � y ����� 㪠�뢠�� �� ���� � �� �� 楯���.
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
   � xe �������� 㪠��⥫� �� ��᫥���� �祩�� �᫠. 
   x �� ������ 㪠�뢠�� �� �� �� 楯���, �� y ��� z.
   y � z ����� 㪠�뢠�� �� ���� � �� �� 楯���.
   � 室� ���᫥��� ��஥ ���祭�� x ������.
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

/* ��������� �᫠ �� ����ࠧ�來� ����⥫� (�᫮ ������
   ���� ��� ������ 2-� ࠧ�來�). �।���������� �� �᫮ 
   �࠭����� ��稭�� � ����� ࠧ�冷�. ��⠥��� ⠪ ��, 
   �� �᫮ ᮤ�ন� �����筮� ������⢮ ࠧ�冷� ��� 
   �࠭���� ����祭���� �ந�������� (��७�� �� ���訩 
   ࠧ�� ���� �� ������). 
   ���� ��७�� ������ ���� ��襭, �� �����饭�� ��
   ��⠥��� ��襭. */
static void mulbyd( LINTEGER x, LDIGITC d ){
        LADDRESS xt, xp;
        byte w[ 2 ][ LDIGIT_SIZE ];

        lint12mul( x->data, d, (LDIGIT)w );
        lint12copy( x->data, w[ 0 ] );
        /* ���訩 ࠧ�� �ந�������� �� ���뢠���� */
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
                        /* �.�. �������� ��९�������, �
                           tp == xp ����� �ਭ��� 1 ��� ��९������� */
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
   � xe �������� 㪠��⥫� �� ��᫥���� �祩�� ���⪠, � � qe 
   㪠��⥫� �� ��᫥���� �祩�� ��⭮��. 
   ������� ��� �� x, y, q �� ������ ��뫠���� �� ���� � �� ��
   楯���.
   � 室� ���᫥��� ��஥ ���祭�� q ������.
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
                /* ��砩 ����ࠧ�來��� ����⥫� */
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
                /* xr == ���⮪ �� x/y */
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

        /* xbar 㪠�뢠�� �� ���� 㬥��蠥����, �� ���ன
           ���⠥��� ᠬ�� ������ ��� ���⠥����;
           xbar ��६�頥��� ��ࠢ� �� ��������; ���⨦����
           ���� �᫠ ������ � �����襭�� 横�� ������� */
        yc = y;
        xbar = *x;
        while( yc->link != LNULL && xbar->link != LNULL ){
                yc = yc->link;
                xbar = xbar->link;
        }
        if( yc->link != LNULL )
                /* ������� ���� ����⥫� */
                goto EXIT;
        if( xbar == LNULL && lint12cmp( (*x)->data, y->data ) < 0 )
                /* ����⥫� � ������� ���������� �����, ��
                   ����� ��� �������� ����� ���襩 ����
                   ����⥫� */
                goto EXIT;

        /* ��ଠ������ ����⥫�
           �� ��ଠ����樨 ����� ����⥫� ��⠥��� �०���, �
           ����� �������� 㢥��稢����� �� 1; ���⠭�� ��
           �������� ��稭����� � ��ன ���� */
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

        /* 横� ������� */
        do{
                /* �業�� ���� ��⭮�� */
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

                /* ���⠭�� �� �஬����筮�� ��������
                   ����⥫�, 㬭�������� �� ࠧ�� ��⭮�� */
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
                                        /* �㤥� �믮������
                                           ����������饥 ᫮����� */
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

                /* �믮������ ����������饣� ᫮����� */
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

                /* �������� ��।��� ���� � ��⭮�� */
ASSIGN:         xtmp = new_cell();
                lint12copy( xtmp->data, xq );
                xtmp->link = *q;
                *q = xtmp;
                if( *qe == LNULL )
                        *qe = xtmp;

                /* ᤢ�� ��ࠢ�;
                   ���訩 ࠧ�� �஬����筮�� ��������
                   㤠����� */
NEXT:           xtmp = *x;
                *x = (*x)->link;
                del_cell( xtmp );
                xbar = xbar->link;
        }
        while( xbar != LNULL );

        /* ����ଠ������ ����⥫� � �������� */
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

        /* 㤠����� ������ �㫥� ���⪠ */
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

/* �ࠢ����� x � y. �����頥��� ���祭��:
        x < y - -1;
        x > y - +1;
        x = y -  0.     
   x � y ����� ��뫠���� �� ���� � �� �� 楯���.
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
   �᫨ ����� x ����������, � � xe �������� 㪠��⥫� �� ��᫥����
   �祩�� �᫠. � ��⨢��� ��砥 ���祭�� xe �� ���������. 
   x � y ����� ��뫠���� �� ���� � �� �� 楯���. 
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
                /* ࠧ����� � �������⥫쭮� ���� -
                   ࠧ����� ���⠥��� �� 0 */
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

                /* 㤠����� ������ �㫥��� ࠧ�冷� */
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

/* �८�ࠧ������ �������� ������������ 楫���. ����祭��� �᫮
   �ᥣ�� ��⮨� �� ����� �祩�� (�� ���� ��ᢠ������� x �
   �����頥��� � ����⢥ १����). */
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
