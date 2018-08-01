/* $Id: lintpp.cpp,v 1.2 2006-04-22 21:49:29 roma Exp $ */

#include "lintpp.h" 

linteger::operator double(){
	LSINTDTLIST l;
	double x = 0.0;

	lsintdtstart(&l, (LSINTEGER *)this);
	while(lsintdtcont(&l)){
		x *= 10.0;
		x += lsintdtget(&l);
	}
	lsintdtend(&l);
	if(lsintcmp((LSINTEGER *)this, NULL) < 0) x = -x;
	return x;
}

std::ostream & operator <<(std::ostream &out, const linteger &v){
	LSINTDTLIST	l;

	if(v == 0) out << '0';
	if(v < 0) out << '-';
	lsintdtstart(&l, (LSINTEGER *)(&v));
	while(lsintdtcont(&l))
		out << (char)('0' + lsintdtget(&l));
	lsintdtend(&l);
	return out;
}

int linteger::length() const{
	LSINTDTLIST l;
	int count = 0;

	lsintdtstart(&l, (LSINTEGER *)this);
	while(lsintdtcont(&l)){
		count++;
		lsintdtget(&l);
	}
	lsintdtend(&l);
	return count;
}