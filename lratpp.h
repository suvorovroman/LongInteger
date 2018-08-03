/* $Id: lratpp.h,v 1.2 2006-04-22 21:49:29 roma Exp $ */

#include "lintpp.h"

class LINTAPI lrational{
	linteger n, d;

	void correct(){
		if(d < (linteger)0){ 
			n = -n;
			d = -d;
		}
	}

public:

	lrational(){}

	lrational(double v, double tol = 1e-5);

	lrational(const linteger &n, const linteger &d){
		linteger zero;
		if(n != zero && d != zero){
			this->n = n;
			this->d = d;
			correct();
		}
	}

	operator double();

	friend std::ostream & operator <<(std::ostream &out, const lrational &v){
		return std::cout << '[' << v.n << '/' << v.d << ']';
	}

	friend lrational operator +(const lrational &a, const lrational &b){
		return lrational(a.n*b.d + b.n*a.d, a.d*b.d);
	}

	lrational & operator +=(const lrational &v){
		n *= v.d;
		n += v.n*d;
		d *= v.d;
		return *this;
	}

	friend lrational operator -(const lrational &a, const lrational &b){
		return lrational(a.n*b.d - b.n*a.d, a.d*b.d);
	}

	lrational & operator -=(const lrational &v){
		n *= v.d;
		n -= v.n*d;
		d *= v.d;
		return *this;
	}

	friend lrational operator *(const lrational &a, const lrational &b){
		return lrational(a.n*b.n, a.d*b.d);
	}

	lrational & operator *=(const lrational &v){
		n *= v.n;
		d *= v.d;
		return *this;
	}

	friend lrational operator /(const lrational &a, const lrational &b){
		return lrational(a.n*b.d, a.d*b.n);
	}

	lrational & operator /=(const lrational &v){
		n *= v.d;
		d *= v.n;
		correct();
		return *this;
	}

	lrational & canonicalize(){
		linteger g = gcd(n, d);
		n /= g;
		d /= g;
		return *this;
	}

	friend int compare(const lrational &a, const lrational &b){
		linteger v = a.n*b.d - a.d*b.n;
		linteger zero;
		return (v < zero) ? -1: (v == zero) ? 0: 1;
	}

	friend int operator ==(const lrational &a, const lrational &b){
		return compare(a, b) == 0;
	}

	friend int operator <(const lrational &a, const lrational &b){
		return compare(a, b) < 0;
	}

	friend int operator <=(const lrational &a, const lrational &b){
		return compare(a, b) <= 0;
	}

	friend int operator >(const lrational &a, const lrational &b){
		return compare(a, b) > 0;
	}

	friend int operator >=(const lrational &a, const lrational &b){
		return compare(a, b) >= 0;
	}
};
