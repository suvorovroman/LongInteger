/* $Id: Lstor.c,v 1.1.1.1 2005-11-20 11:36:52 roma Exp $ */

/* lstor.c (�ࠢ����� �票�⮩ �������) */
 
#include <stdlib.h>
#include <malloc.h>
#include "lstor.h"

/* ���� ��砫� ᯨ᪠ ᢮������ �祥�. */
static LADDRESS Avail = LNULL;

LADDRESS        get_lblock( void );
void            free_lbchain( void );

/* �᫨ �������� �뤥��� �祩�� �� ᯨ᪠ Avail.
   �᫨ Avail ����,� ��⠥��� �뤥���� �祩�� �� ⥪�饣� ����� �����.
   �᫨ ⥪�騩 ���� ���௠�, � �뤥����� ����.     */
LADDRESS new_cell( void ){
        LADDRESS ret;

        if( Avail == LNULL ){
                static LADDRESS b;
                static unsigned int r = 0;

                if( r == 0 ){
                        b = get_lblock();
                        r = LBLOCK_SIZE;
                }
                ret = b + (--r);
        }
        else{
                ret = Avail;    
                Avail = Avail->link;
        }
        return ret;
}

/* �����饭�� 楯�窨 � ᯨ᮪ Avail.
   �।����������, �� b � e ����� ��砫�� � ���殬
   ����� � ⮩ �� 楯�窨 (b != LNULL && e != LNULL). */
void del_chain( LADDRESS b, LADDRESS e ){
        e->link = Avail;
        Avail = b;
}

/* ���饭�� 楯�窨 (x ����� ���� LNULL). */
void invert_chain( LADDRESS * x ){
        LADDRESS c;

        c = LNULL;
        while( *x != LNULL ){
                LADDRESS f;

                f = *x;
                *x = f->link;
                f->link = c;
                c = f;
        }
        *x = c;
}

/* ����஢���� 楯�窨 (y ����� ���� LNULL).
   � x ����蠥��� ���� ��砫� 楯�窨, � �����頥��� ����
   ����. */
LADDRESS copy_chain( LADDRESS * x, LADDRESSC y ){
        LADDRESS xe;

        *x = xe = LNULL;
        while( y != LNULL ){
                if( xe != LNULL ){
                        xe->link = new_cell();
                        xe = xe->link;
                }
                else
                        *x = xe = new_cell();
                copy_cell_data( xe->data, y->data );
                y = y->link;
        }
        if( xe != LNULL )
                xe->link = LNULL;
        return xe;
}

/* ����஢���� ���� ������ �祩�� (�⮡� �� �ᯮ�짮����
   �㭪樨 _fmemcpy, _fmemset � �.�.). */
void copy_cell_data( LDATA x, LDATAC y ){
        struct wrap{
                byte data[ LCELL_DATA_SIZE ];
        };
        *((struct wrap  *)x) = *((struct wrap  *)y);
}

/* ���᫥��� ����� 楯�窨 x. � e �������� ���� ��᫥����
   �祩��. */
unsigned int len_chain( LADDRESS x, LADDRESS *e ){
	unsigned int count;
	
	count = 0;
	*e = x;
	if( x ){
		count++;
		while( (*e)->link != LNULL ){
			*e = (*e)->link;
			count++;
		}
	}
	return count;
}

/* �᢮�������� ��, ����⮩ ��� �票���� ������. ��뫪� ��
   �� �᢮�������� � 楯�窨 �⠭������ ������⢨⥫�묨.
*/
void free_lstor( void ){
      Avail = NULL;
      free_lbchain();
}