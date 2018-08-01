/* $Id: lratpp.cpp,v 1.1.1.1 2005-11-20 11:36:52 roma Exp $ */

#include <math.h>
#include "lratpp.h" 

lrational::lrational(double v, double tol){
	linteger p1 = 0, p0 = 1, q1 = 1, q0 = 0;
	double c, x, qx, qx1 = 1.0, qx0 = 0.0;
		
	x = fabs(v);
	for(;;){
		c = floor(x);
		n = (linteger)c*p0 + p1;
		d = (linteger)c*q0 + q1;
		/* тест на случайное совпадение */
		if(x - c == 0.0) break;
		/* тест на близость к результату */
		qx = c*qx0 + qx1;
		if(1.0/(qx*qx) < tol) break;
		/* следующая подходящая дробь */
		x = 1.0/(x - c);
		p1 = p0;
		q1 = q0;
		p0 = n;
		q0 = d;
		qx1 = qx0;
		qx0 = qx;
	}
	if(v < 0.0)	n = -n;
}

lrational::operator double(){
	linteger zero;
	if(d == (linteger)1 || d == zero)
		return double(n);
	else{
		linteger q, r;
		div(n, d, q, r);
		if(r == (linteger)0)
			return double(q);
		else
			return double(q) + 1.0/double(lrational(d, r));
	}
}
