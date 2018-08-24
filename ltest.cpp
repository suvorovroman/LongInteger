/* Copyright (C) 1996-2018 Suvorov Roman I. */
#include "lratpp.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

using namespace std;

#define MAX_LENGTH	6765	//!< Максимальная длина тестируемых чисел.

/** 	\brief Тест основных арифметических операций.

	Проверяет условие a = q*b + r для двух произвольных целых чисел. Здесь
	q = a/b
	r = a mod b
	и b != 0.
	
	\param[in]	_l	Максимальная длина исходных данных.
*/
static void unit_01(linteger::rng& g)
{
	linteger a = g(true);	
	linteger b, zero;
	do
		b = g(true);		
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

int main(int _argc, const char* _argv[])
{
	int seed = (int)time(0);
	linteger::rng g(MAX_LENGTH);
	cout << "Randomizing seed " << seed << endl;

	srand(seed);
	
	std::cout << "Arithmetic test. Length " << MAX_LENGTH << endl;
	for(int i = 1; i <= 10000; i++)
	{
		cout << i << '\t';
		unit_01(g);
	}

	linteger::finalize();
	cout << "ok" << endl;
	cin.get();
	return 0;
}
