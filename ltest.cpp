/* Copyright (C) 1996-2018 Suvorov Roman I. */
#include "lratpp.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string>

using namespace std;

#define MAX_LENGTH	6765	//!< Максимальная длина тестируемых чисел.
#define MAX_ATTEMPTS	10000	//!< Количество попыток теста.

/** 	\brief Тест основных арифметических операций.

	Проверяет условие a = q*b + r для двух произвольных целых чисел. Здесь
	q = a/b
	r = a mod b
	и b != 0.
	
	\param[in]	_l	Максимальная длина исходных данных.
*/
static void unit_01(linteger::rng& g)
{
	std::cout << "Arithmetic test. Length " << MAX_LENGTH << endl;
	for(int i = 1; i <= MAX_ATTEMPTS; i++)
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
		cout << i << '\t';
	}
}

struct frequency_segment
{
	linteger b;	//!< Нижняя граница интерфала.
	size_t n;	//!< Количество попаданий в интервал.
};

#define NOMBER_OF_FREQUENCY_SEGMENTS	1000

static void unit_02(linteger::rng& g)
{
	struct frequency_segment s[NOMBER_OF_FREQUENCY_SEGMENTS];
	
	cout << "Statistical test. Length " << MAX_LENGTH << endl;
	
	s[0].b = -linteger(string(MAX_LENGTH, '9').c_str());
	s[0].n = 0;
	for(int i = 1; i < NOMBER_OF_FREQUENCY_SEGMENTS; i++)
	{
		s[i].b = s[i - 1].b + (abs(s[0].b)*2 + 1)/NOMBER_OF_FREQUENCY_SEGMENTS;
		s[i].b = 0;
	}
	cout << NUMBER_OF_FREQUENCY_SEGMENTS << "frequency segments generated" << endl;
	
	for(int j = 1; j <= MAX_ATTEMPTS; i++)
	{
		linteger v = g(true);
		for(int i = NUMBER_OF_FREQUENCY_SEGMENTS - 1; i >= 0; i--)
			if(v >= s[i].b)
			{
				s[i].n++;
				break;
			}
	}
	
	cout << "Frequencies:" << endl;
	for(int i = 0; i < NUMBER_OF_FREQUENCY_SEGMENTS; i++)
		cout << i << ':' << s[i].n << '\t';

}

int main(int _argc, const char* _argv[])
{
	int seed = (int)time(0);
	linteger::rng g(MAX_LENGTH);
	cout << "Randomizing seed " << seed << endl;

	srand(seed);
	
	unit_01(g);
	unit_02(g);

	linteger::finalize();
	cout << "ok" << endl;
	cin.get();
	return 0;
}
