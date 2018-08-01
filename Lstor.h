/* $Id: Lstor.h,v 1.1.1.1 2005-11-20 11:36:48 roma Exp $ */

/* lstor.h */

/*                     ���������� �������� �������                   */
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

/* �������� ����� �祩��.
   �����頥� ���� �祩��. */
LADDRESS        new_cell( void );

/* �������� �祩��.
   "�" - ���� 㤠�塞�� �祩��, ����祭��
   � ������� �맮�� new_cell.   */
#define del_cell( A )  del_chain( A, A );

/* �������� 楯�窨.
   "b" � "e" - ���� ��ࢮ� � ��᫥���� �祥� 楯�窨. */
void            del_chain( LADDRESS b, LADDRESS e );

/* ���饭�� 楯�窨 a.
   � x ����頥��� 㪠��⥫� �� ����祭��� 楯���.      */
void            invert_chain( LADDRESS * a );

/* �������� ����� 楯�窨 y.
   � x ����頥��� ���� ��ࢮ� �祩�� �����, �����頥��� ����
   ��᫥���� �祩�� �����.      */
LADDRESS        copy_chain( LADDRESS *x, LADDRESSC y );

/* ���᫥��� ����� 楯�窨 x.
   � e ����頥��� ���� ��᫥���� �祩�� 楯�窨. */
unsigned int	len_chain( LADDRESS x, LADDRESS *e );

/* ����஢���� ����� ������ ࠧ��� LCELL_DATA_SIZE.
   y - ���筨�, x - �ਥ����. */
void            copy_cell_data( LDATA x, LDATAC y );
  
/* ��ꥬ ��, ��१�ࢨ஢���� ��� �࠭����
   �祥� (� �����).    */
unsigned long   lleak( void );

/* �᢮�������� ��, ����⮩ ��� �票���� ������. ��뫪� ��
   �� �᢮�������� � 楯�窨 �⠭������ ������⢨⥫�묨. */
void free_lstor( void );
