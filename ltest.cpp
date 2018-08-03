/* $Id$ */
#include "lratpp.h"
#include <iostream>
#include <time.h>

using namespace std;

static void unit_01(int _l)
{
	std::cout << "Arithmetic test. Length " << _l << endl;
	linteger a = linteger::rand(_l);
	linteger b = linteger::rand(_l);
	cout << "a: " << a << '\t' << '[' << a.length() << ']' << endl;
	cout << "b: " << b << '\t' << '[' << b.length() << ']' << endl;
}

int main(int _argc, const char* _argv[])
{
	srand(time(0));
	unit_01(1);
	unit_01(2);
	unit_01(3);
	unit_01(5);
	unit_01(10);
	linteger::finalize();
	cin.get();
	return 0;
}