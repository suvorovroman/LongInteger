/* Copyright (C) 1996-2018 Suvorov Roman I. */

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

linteger::rng::rng(int length):Length(length)
{
	Buffer = new char[Length + 1];
}

linteger::rng::~rng()
{
	delete[] Buffer;
}

linteger linteger::rng::get()
{
	/* Определение длины нового случайного числа. Длина всегда должны быть больше нуля, чтобы не
	   увеличивать количество нулей за счет двух возможных способов получения: строки нулевой длины и
	   строки, содержащей нули. Оставляем только выриант со строкой соделжащей нули.*/
	int l;
	do
		l = ::rand()%Length;
	while(l == 0);
	/* Разряды заполняются с конца. Младший байт не используется, чтобы повысить равномерность
	   распределения разряда. */
	Buffer[l + 1] = '\0';
	while(l >= 0)
		Buffer[l--] = ::rand()%10 + '0';
	return linteger(Buffer);
}
