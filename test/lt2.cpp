/* Copyright (C) 1996-2018 Suvorov Roman I. */
#include "lratpp.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string>

using namespace std;

#define MAX_LENGTH	6765	//!< Максимальная длина тестируемых чисел.
#define MAX_ATTEMPTS	10000	//!< Количество попыток теста.
#define MAX_SEGMENTS	100

extern "C" size_t lblock_size(void);

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
	}

	cout << "Memory leak: " << lleak() << endl;
}

static void unit_02(linteger::rng& g)
{
	cout << "Statistical test. Length " << MAX_LENGTH << ". Segments " << MAX_SEGMENTS << endl;
	size_t n[MAX_SEGMENTS];

	for(int i = 0; i < MAX_SEGMENTS; i++)
		n[i] = 0;

	linteger s = (linteger(string(MAX_LENGTH, '9').c_str()) + linteger(1))/linteger(MAX_SEGMENTS);

	for(int j = 0; j < MAX_ATTEMPTS; j++)
	{
		linteger v = g();
		for(int i = MAX_SEGMENTS - 1; i >= 0; i--)
			if(v >= s*linteger(i))
			{
				n[i]++;
				break;
			}
	}

	cout << endl;

	size_t c = 0;
	for(int i = 0; i < MAX_SEGMENTS; i++)
	{
		cout << i << ':' << n[i] << '\t';
		c += n[i];
	}
	cout << endl;

	if(c != MAX_ATTEMPTS)
	{
		cout << "Statistic calculation failure:" << MAX_ATTEMPTS << "!=" << c << endl;
		exit(2018);
	}

	cout << "Memory leak: " << lleak() << endl;
}

static void unit_03()
{
	cout << "RAND function test. Random number length determination test." << endl;
	size_t n[MAX_SEGMENTS];

	for(int i = 0; i < MAX_SEGMENTS; i++)
		n[i] = 0;

	int s = MAX_LENGTH/MAX_SEGMENTS;

	for(int j = 0; j < MAX_ATTEMPTS; j++)
	{
		size_t v = rand()%(MAX_LENGTH + 1);
		for(int i = MAX_SEGMENTS - 1; i >= 0; i--)
			if(v >= (size_t)s*i)
			{
				n[i]++;
				break;
			}
	}
	cout << endl;

	size_t c = 0;
	for(int i = 0; i < MAX_SEGMENTS; i++)
	{
		cout << i << ':' << n[i] << '\t';
		c += n[i];
	}
	cout << endl;

	if(c != MAX_ATTEMPTS)
	{
		cout << "Statistic calculation failure:" << MAX_ATTEMPTS << "!=" << c << endl;
		exit(2018);
	}
}

int main(int _argc, const char* _argv[])
{
  cout << "LSTOR_POOL_SIZE:\t" << LSTOR_POOL_SIZE << endl;
  cout << "LSTOR_CELL_SIZE:\t" << LSTOR_CELL_SIZE << endl;
  cout << "sizeof(lcell):\t" << sizeof(lcell) << endl;
  cout << "lblock_size:\t" << lblock_size() << endl;

	int seed = (int)time(0);

	linteger::rng g(MAX_LENGTH);
	cout << "Randomizing seed\t" << seed << endl;
	srand(seed);
	
      	unit_01(g);
	unit_02(g);
	unit_03();

	linteger::finalize();

	cout << "ok" << endl;
	cin.get();
	return 0;
}
