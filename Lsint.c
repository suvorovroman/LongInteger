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

/* �㭪樨 ࠧ���� ������������� �।�����祭� ��� �������
   ��砫��� ���祭�� ��६�����. ��砫쭮� ���祭�� ������
   ���� ��᢮��� ��६�����:
        - �� ��ࢮ� ������ ��६����� � �ணࠬ�� (��
          ⮣� ��� ��� �㤥� ���⢮���� � �����-����
          ���� �������);
        - ��᫥ �����樫���樨 ��६����� �㭪樥�
          lsintdel.
*/

/* [�������������] ��६����� v ��ᢠ������� 0.
*/
void lsintzero( LSINTEGER *v ){
      CLS_SIGN( v );
      v->x = v->xe = LNULL;
}

/* [�������������] ��६����� v ��ᢠ������� ���祭��,
   ����ᠭ��� � ��ப� s.
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

/* [�������������] ��६����� v ��ᢠ������� ���祭�� x.
*/
void lsintulong( LSINTEGER *v, unsigned long x ){
      CLS_SIGN( v );
      v->xe = lintlong( &(v->x), x );
}

/* [�������������] ��६����� v ��ᢠ������� ���祭�� x.
*/
void lsintlong( LSINTEGER *v, long x ){
      CLS_SIGN( v );
      if( x < 0 )
              SET_SIGN( v );
      v->xe = lintlong( &(v->x), labs( x ) );
}

/* [�������������] ��६����� v ��ᢠ������� ���祭�� ��६����� w.
*/
void lsintlsint( LSINTEGER *v, const LSINTEGER *w ){
      v->sign = w->sign;
      v->xe = lintcopy( &(v->x), w->x );
}

/* [�������������] ��६����� v ��ᢠ������� ���祭��, ��������
   㪠��⥫ﬨ �� 楫�� ������⥫쭮� �᫮ � �� ��� 墮��.
   ��ࠬ��� sign ������ ���� �᫠.
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

/* [����������] ������� ���祭�� ��६����� w � ��६����� v.
   ��। ����஢����� ������, ���������� �।��騬 ���祭��� 
   ��६����� v �᢮���������.
*/
void lsintcopy( LSINTEGER *v, const LSINTEGER *w ){
        if( v->x != LNULL )
                lintdelex( v->x, v->xe );
        lsintlsint( v, w );
}

/* [����������] ��६���� v � w ������������ ���祭�ﬨ.
   ����� �ந�室�� ��� ���ࠧ��饭�� �����.
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

/* [����������] �᢮������� ������ ���������� ���祭�ﬨ ��६�����.
   �� ��ࠬ��஬ n 㪠�뢠���� ᯨ᮪ ��६�����. �� �⮬
   n 㪠�뢠�� �� ������⢮. ��। ������ �ᯮ�짮������ ��᫥
   �᢮�������� ����� ����室��� ���樠������ ��६�����
   (�. ࠧ�. [�������������]).
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

/* �㭪樨 ࠧ���� [�������������] ��ࠧ��� �����, �����
   �����頥� ��᫥����⥫쭮��� �������� ��� �᫠. �����
   � ��᫥����⥫쭮�� 㯮�冷祭� �� ����� � ����訬.
*/

/* [�������������] ���樠������ ����� l �᫮� v.
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

/* [�������������] ����祭�� ��।��� �����筮� ���� ��
   ����� l.
*/
byte lsintdtget( LSINTDTLIST *l ){
      byte ret;

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

/* [�������������] �����頥� ���㫥��� ���祭�� �᫨ ����� l
   �� ᮤ�ন� ����.
*/
int lsintdtcont( const LSINTDTLIST *l ){
      return l->xc != LNULL;
}

/* [�������������] �ᮡ������ ����� l ��᫥ ⮣�, ��� ��
   ���� �� ����. ��᫥ �᢮�������� ����� ����� ����
   ���樠����஢�� ᭮��.
*/
void lsintdtend( LSINTDTLIST *l ){
      invert_chain( &(l->v->x) );
}

/* [����������] �ࠢ������ ���祭�� ��६����� v � w. �������
   �ࠢ�����:
      -1 - v < w,
       1 - v > w,
       0 - v = w.
   ��� ���� ����� w = 0, ��ன ��ࠬ��� ����� ���� NULL (����
   ��ࠬ��� ������� �� ������ ���� ࠢ�� NULL).
*/   
int lsintcmp( const LSINTEGER *v, const LSINTEGER *w ){
      if( w == NULL )
            return GET_SIGN( v ) ? -1 : lintcmp( v->x, LNULL );
      if( GET_SIGN( w ) )
            return GET_SIGN( v ) ? lintcmp( w->x, v->x ) : 1;
      else
            return GET_SIGN( v ) ? -1 : lintcmp( v->x, w->x );
}

/* [����������] v <- v + w.
   ��� ��ࠬ��� ����� 㪠�뢠�� �� ���� � � �� �᫮.
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

/* [����������] v <- -v.
*/
void lsintneg( LSINTEGER *v ){
      if( GET_SIGN( v ) )
            CLS_SIGN( v );
      else
      if( v->x != LNULL )
            SET_SIGN( v );
}

/* [����������] v <- v - w.
   ��� ��ࠬ��� ����� 㪠�뢠�� �� ���� � � �� �᫮.
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

/* [����������, �������������] t <- v*w.
   ��ࠬ���� v, w ����� 㪠�뢠�� �� ���� � � �� ���祭��, ������
   �� ���� �� ��� �� ����� 㪠�뢠�� �� �᫮ t. ��᫥ �믮������
   �㭪樨 t 㪠�뢠�� �� �ந�������� v � w, �� �⮬ �।����������,
   �� �� �맮�� ���祭�� t ����।����� (�� �뫮 ���樠����஢���� ���
   �����樠����஢���) ��� ࠢ�� 0. */
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

/* [����������, �������������] q <- v div w, v <- v mod w.
   ��ࠬ���� v � w �� ������ 㪠�뢠�� �� ���� � � �� ���祭��.
   �� ���� �� ��� �� ����� 㪠�뢠�� �� ���祭�� q. ���祭�� q
   ������ ���� ����।����� (�� ���樠����஢���� ��� 
   �����樠����஢����) ��� ࠢ�� 0. */
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

/* [����������] v <- abs(v). */
void lsintabs( LSINTEGER *v ){
      CLS_SIGN( v );
}

void lsintdouble( LSINTEGER *v, double x ){
	CLS_SIGN(v);
	if(x < 0.0)
		SET_SIGN(v);
	v->xe = lintdouble(&(v->x), x);
}