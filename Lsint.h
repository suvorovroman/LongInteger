/* $Id: Lsint.h,v 1.1.1.1 2005-11-20 11:36:48 roma Exp $ */

#include <utilib_extern.h>
#include "lint.h"

struct tagLSINTEGER{ 
        char sign;      /* ���� ����� (�ᯮ������ ����訩 ���) */
        LADDRESS x;     /* 㪠��⥫� �� �᫮ */
        LADDRESS xe;    /* 㪠��⥫� �� 墮�� �᫠ */
};
typedef struct tagLSINTEGER LSINTEGER;

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
_UTILIB_EXTERN_ void lsintzero( LSINTEGER *v );

/* [�������������] ��६����� v ��ᢠ������� ���祭��,
   ����ᠭ��� � ��ப� s.
*/
_UTILIB_EXTERN_ void lsintstr( LSINTEGER *v, const char *s );

/* [�������������] ��६����� v ��ᢠ������� ���祭�� x.
*/
_UTILIB_EXTERN_ void lsintulong( LSINTEGER *v, unsigned long x );

/* [�������������] ��६����� v ��ᢠ������� ���祭�� x.
*/
_UTILIB_EXTERN_ void lsintlong( LSINTEGER *v, long x );

/* [�������������] ��६����� v ��ᢠ������� ���祭�� ��६����� w.
*/
_UTILIB_EXTERN_ void lsintlsint( LSINTEGER *v, const LSINTEGER *w );

/* [�������������] ��६����� v ��ᢠ������� ���祭��, ��������
   㪠��⥫ﬨ �� 楫�� ������⥫쭮� �᫮ � �� ��� 墮��.
   ��ࠬ��� sign ������ ���� �᫠.
*/
_UTILIB_EXTERN_ void lsintlint( LSINTEGER *v, LINTEGER x, LINTEGER xe, int sign );

/* [�������������] ��६����� v ��ᢠ������� �ᥢ����砩���
   �᫮ ����� length �������� ���.
*/
_UTILIB_EXTERN_ void lsintrand( LSINTEGER *v, int length );

_UTILIB_EXTERN_ void lsintdouble( LSINTEGER *v, double x );

/* [����������] ������� ���祭�� ��६����� w � ��६����� v.
   ��। ����஢����� ������, ���������� �।��騬 ���祭��� 
   ��६����� v �᢮���������.
*/
_UTILIB_EXTERN_ void lsintcopy( LSINTEGER *v, const LSINTEGER *w );

/* [����������] ��६���� v � w ������������ ���祭�ﬨ.
   ����� �ந�室�� ��� ���ࠧ��饭�� �����.
*/
_UTILIB_EXTERN_ void lsintexch( LSINTEGER *v, LSINTEGER *w );

/* [����������] �᢮������� ������ ���������� ���祭�ﬨ ��६�����.
   �� ��ࠬ��஬ n 㪠�뢠���� ᯨ᮪ ��६�����. �� �⮬
   n 㪠�뢠�� �� ������⢮. ��। ������ �ᯮ�짮������ ��᫥
   �᢮�������� ����� ����室��� ���樠������ ��६�����
   (�. ࠧ�. [�������������]).
*/
_UTILIB_EXTERN_ void lsintdel( int n, ... );

struct tagLSINTDTLIST{
      LSINTEGER *v;
      LADDRESS  xc;
      byte       d[ LINT12_NDT ];
      int        i;
};
typedef struct tagLSINTDTLIST LSINTDTLIST;

/* �㭪樨 ࠧ���� [�������������] ��ࠧ��� �����, �����
   �����頥� ��᫥����⥫쭮��� �������� ��� �᫠. �����
   � ��᫥����⥫쭮�� 㯮�冷祭� �� ����� � ����訬.
*/

/* [�������������] ���樠������ ����� l �᫮� v.
*/
_UTILIB_EXTERN_ void lsintdtstart( LSINTDTLIST *l, LSINTEGER *v );

/* [�������������] ����祭�� ��।��� �����筮� ���� ��
   ����� l.
*/
_UTILIB_EXTERN_ byte lsintdtget( LSINTDTLIST *l );

/* [�������������] �����頥� ���㫥��� ���祭�� �᫨ ����� l
   �� ᮤ�ন� ����.
*/
_UTILIB_EXTERN_ int lsintdtcont( const LSINTDTLIST *l );

/* [�������������] �ᮡ������ ����� l ��᫥ ⮣�, ��� ��
   ���� �� ����. ��᫥ �᢮�������� ����� ����� ����
   ���樠����஢�� ᭮��.
*/
_UTILIB_EXTERN_ void lsintdtend( LSINTDTLIST *l );

/* [����������] �ࠢ������ ���祭�� ��६����� v � w. �������
   �ࠢ�����:
      -1 - v < w,
       1 - v > w,
       0 - v = w.
   ��� ���� ����� w = 0, ��ன ��ࠬ��� ����� ���� NULL (����
   ��ࠬ��� ������� �� ������ ���� ࠢ�� NULL).
*/   
_UTILIB_EXTERN_ int lsintcmp( const LSINTEGER *v, const LSINTEGER *w );

/* [����������] v <- v + w.
   ��� ��ࠬ��� ����� 㪠�뢠�� �� ���� � � �� �᫮.
*/
_UTILIB_EXTERN_ void lsintadd( LSINTEGER *v, const LSINTEGER *w );

/* [����������] v <- v - w.
   ��� ��ࠬ��� ����� 㪠�뢠�� �� ���� � � �� �᫮.
*/
_UTILIB_EXTERN_ void lsintsub( LSINTEGER *v, const LSINTEGER *w );

/* [����������] v <- -v.
*/
_UTILIB_EXTERN_ void lsintneg( LSINTEGER *v );

/* [�����] �뢮� �᫠ v � 䠩� fout.
*/
_UTILIB_EXTERN_ void lsintout( LSINTEGER *v, FILE *fout );

/* [����������, �������������] t <- v*w.
   ��ࠬ���� v, w ����� 㪠�뢠�� �� ���� � � �� ���祭��, ������
   �� ���� �� ��� �� ����� 㪠�뢠�� �� ���祭�� t. ��᫥ �믮������
   �㭪樨 t 㪠�뢠�� �� �ந�������� v � w, �� �⮬ �।����������,
   �� �� �맮�� ���祭�� t ����।����� (�� �뫮 ���樠����஢��� ���
   �����樠����஢���) ��� ࠢ�� 0. */
_UTILIB_EXTERN_ void lsintmul( LSINTEGER *t, const LSINTEGER *v, const LSINTEGER *w );

/* [����������, �������������] q <- v div w, v <- v mod w.
   ��ࠬ���� v � w �� ������ 㪠�뢠�� �� ���� � � �� ���祭��.
   �� ���� �� ��� �� ����� 㪠�뢠�� �� ���祭�� q. ���祭�� q
   ������ ���� ����।����� (�� ���樠����஢���� ��� 
   �����樠����஢����) ��� ࠢ�� 0. */
_UTILIB_EXTERN_ void lsintdiv( LSINTEGER *v, const LSINTEGER *w, LSINTEGER *q );

/* [����������] v <- abs(v). */
_UTILIB_EXTERN_ void lsintabs( LSINTEGER *v );

/* [����������, ���] ������ ������� (Euclidean Algorithm). 
      x <- ���(x, y)
      y <- 0
   ���筨�:
      �.�����, �᭮�� �������୮� ������� � �ਫ�����ﬨ
      �.:���, 1994, ������ EA. */
_UTILIB_EXTERN_ void lsintgcd( LSINTEGER *x, LSINTEGER *y );

/* [����������, ���] ���᫥��� �������襣� ��襣� ��⭮��.
      x <- ���(x, y)
      y <- ���(x, y)
   ���筨�:
      �.�����, �᭮�� �������୮� ������� � �ਫ�����ﬨ.
      �.:���, 1994, ��।������ 2.2.7. */
_UTILIB_EXTERN_ void lsintlcm( LSINTEGER *x, LSINTEGER *y );