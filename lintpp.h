/* $Id: lintpp.h,v 1.4 2007-03-11 11:50:23 roma Exp $ */

#include <iostream>

extern "C"{
	#include "lsint.h" 
}

class LINTAPI linteger:private tagLSINTEGER{ 
public:
	linteger(){
		lsintzero(this);
	}

	linteger(const char *s){
		lsintstr(this, s);
	}

	linteger(unsigned long x){
		lsintulong(this, x);
	}

	linteger(long x){
		lsintlong(this, x);
	}

	linteger(unsigned int x){
		lsintulong(this, x);
	}

	linteger(int x){
		lsintlong(this, x);
	}

	linteger(const linteger &v){
		lsintlsint(this, &v);
	}

	linteger(double v){
		lsintdouble(this, v);
	}

	~linteger(){
		lsintdel(1, this);
	}

	operator double();

	linteger & operator =(const linteger &w){
		lsintcopy(this, &w);
		return *this;
	}

	friend int operator ==(const linteger &v, const linteger &w){
		return lsintcmp(&v, &w) == 0;
	}

	friend int operator !=(const linteger &v, const linteger &w){
		return lsintcmp(&v, &w) != 0;
	}

	friend int operator <(const linteger &v, const linteger &w){
		return lsintcmp(&v, &w) < 0;
	}

	friend int operator >(const linteger &v, const linteger &w){
		return lsintcmp(&v, &w) > 0;
	}

	friend int operator <=(const linteger &v, const linteger &w){
		return lsintcmp(&v, &w) <= 0;
	}

	friend int operator >=(const linteger &v, const linteger &w){
		return lsintcmp(&v, &w) >= 0;
	}

	friend linteger operator /(const linteger &v, const linteger &w){
		linteger q, r;

		r = v;
		lsintdiv(&r, &w, &q);
		return q;
	}

	friend linteger & operator /=(linteger &v, const linteger &w){
		linteger q;

		lsintdiv(&v, &w, &q);
		v = q;
		return v;
	}

	friend linteger operator %(const linteger &v, const linteger &w){
		linteger q, r;

		r = v;
		lsintdiv(&r, &w, &q);
		return r;
	}

	friend linteger & operator %=(linteger &v, const linteger &w){
		linteger q;

		lsintdiv(&v, &w, &q);
		return v;
	}

	friend void div(const linteger &n, const linteger &d, linteger &q, linteger &r){
		r = n;
		lsintdiv(&r, &d, &q);
	}

	friend linteger operator *(const linteger &v, const linteger &w){
		linteger t;

		lsintmul(&t, &v, &w);
		return t;
	}

	friend linteger & operator *=(linteger &v, const linteger &w){
		linteger t;

		lsintmul(&t, &v, &w);
		v = t;
		return v;
	}

	friend linteger operator +(const linteger &v, const linteger &w){
		linteger t = v;
		
		lsintadd(&t, &w);
		return t;
	}

	friend linteger & operator +=(linteger &v, const linteger &w){
		lsintadd(&v, &w);
		return v;
	}


	friend linteger operator -(const linteger &v, const linteger &w){
		linteger t = v;

		lsintsub(&t, &w);
		return t;
	}

	friend linteger & operator -=(linteger &v, const linteger &w){
		lsintsub(&v, &w);
		return v;
	}

	friend linteger operator -(const linteger &v){
		linteger t = v;

		lsintneg(&t);
		return t;
	}

	friend linteger abs(const linteger &v){
		linteger t = v;

		lsintabs(&t);
		return t;
	}

	friend linteger rand(int length){
		linteger t;
		lsintrand(&t, length);
		return t;
	}

	friend LINTAPI std::ostream & operator <<(std::ostream &out, const linteger &v);

	friend linteger lcm(linteger &v, linteger &w){
		linteger	t1 = v, t2 = w;
		lsintlcm(&t1, &t2);
		return t1;
	}

	friend linteger gcd(const linteger &v, const linteger &w){
		linteger	t1 = v, t2 = w;

		lsintgcd(&t1, &t2);
		return t1;
	}

	int length() const;

	static void finalize(){
		free_lstor();
	}
};
