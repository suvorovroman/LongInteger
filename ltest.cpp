/* Copyright (C) 1996-2018 Suvorov Roman I. */
#include "lratpp.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

using namespace std;

/** 	\brief Тест основных арифметических операций.

	Проверяет условие a = q*b + r для двух произвольных целых чисел. Здесь
	q = a/b
	r = a mod b
	и b != 0.
	
	\param[in]	_l	Максимальная длина исходных данных.
*/
static void unit_01(int _l)
{
	std::cout << "Arithmetic test. Length " << _l << endl;
	linteger a = linteger::rand(_l);	
	linteger b, zero;
	do
		b = linteger::rand(_l);		
	while(b == zero);
	linteger q, r;
	div(a, b, q, r);
	if(q*b + r != a)
	{
		cout << "a:" << a << '[' << a.length() << ']' << endl;
		cout << "b:" << b << '[' << b.length() << ']' << endl;
		cout << "q:" << q << '[' << q.length() << ']' << endl;
		cout << "r:" << r << '[' << q.length() << ']' << endl;
		cout << "q*b+r:" << q*b + r << endl;
		exit(2018);
	}
}

#define	MAX_LENGTH	17711
#define MAX_ITER	10

int main(int _argc, const char* _argv[])
{
	int f1 = 1, f2 = 1;
	srand((int)time(0L));
	
	/* Арифметический тест. Размер чисел растет как последовательность Фиббоначи. */ 
	do
	{
		for(int i = 0; i < MAX_ITER; i++)
			unit_01(f2);
		int f = f1 + f2;
		f1 = f2;
		f2 = f;
	}
	while(f2 <= MAX_LENGTH);
	
	linteger::finalize();
	cout << "ok" << endl;
	cin.get();
	return 0;
}
