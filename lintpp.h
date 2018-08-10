/* Copyright (C) 1996-2018 Suvorov Roman I. */

#include <iostream>

extern "C"{
	#include "lsint.h" 
}

/** \brief Класс целых чисел многократной точности.
		
	Класс обеспечивает объектно-ориентированный интерфейс для использования целых чисел произвольной точности в
	программе на C++:
		- конструкторы и деструктор для управления панятью, используемой для хранения целых чисел;
		- стандартную нотацию для записи арифметических операций;
		- потоковый вывод числа.
			
*/
class LINTAPI linteger:private tagLSINTEGER{ 
public:
	/** \brief Создать число 0. */
	linteger(){
		lsintzero(this);
	}

	/** \brief Создать число на основе строки символов.
	
		Преобразование строки s в целое число сознаком. 
   		Функция удаляет ведущие пробелы, проверяет наличие знака, а затем преобразует в число часть 
   		строки до первого нецифрового (десятичного) символа. Если строка не содержит цифровых символов 
		совсем, то создается 0.
			
		\param[in]	s	Число.
	*/
	linteger(const char *s){
		lsintstr(this, s);
	}

	/** \brief Создать число на основе беззнакового длинного целого.
		\param[in]	x	Число.
	*/
	linteger(unsigned long x){
		lsintulong(this, x);
	}

	/** \brief Создать число на основе длинного целого со знаком.
		\param[in]	x	Число.
	*/
	linteger(long x){
		lsintlong(this, x);
	}

	/** \brief Создать число на основе беззнакового целого.
		\param[in]	x	Число.
	*/
	linteger(unsigned int x){
		lsintulong(this, x);
	}

	/** \brief Создать число на основе знакового целого.
		\param[in]	x	Число.
	*/
	linteger(int x){
		lsintlong(this, x);
	}

	/** \brief Создать копию числа.
		param[in]	v	Число.
	*/
	linteger(const linteger &v){
		lsintlsint(this, &v);
	}

	/** \brief Создать число на основе числа с плавающей точкой двойной точности.
		\param[in]	v	Число.	
	*/
	linteger(double v){
		lsintdouble(this, v);
	}

	/** \brief Освобождает память, занимаемую числом. */
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

	/** \brief Создать случайное число.
	
		Создает случайное число размера, не превышающего length. Действительный размер числа вычисляется
		как случайное число в диапазоне от 1 до length. Само число формируется поразрядно: начиная со старшего каждый разряд
		получает случайное значение от 0 до 9. Знак так же определяется случайно.
		
		\param[in]	length	Максимальная длина случайного числа.
		\return	Случайное число.
	*/
	static linteger rand(int length){
		linteger t;
		lsintrand(&t, length);
		return t;
	}

	/** \brief Вывод числа в поток.
	
		\param[in]	out	Поток вывода.
		\param[in]	v	Число.
		\return Поток вывода.
	*/
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

	/** \brief Количество разрядов числа.
	
		Подсчитывает количество разрядов в числе (знак не учитывается).
		
		\return Количество разрядов.
	*/
	int length() const;

	/** \brief Освободить память, занимаемую числами.
	
		Освобождает память, занимаемую всеми числами. Все объекты становятся недействительными.
	*/
	static void finalize(){
		free_lstor();
	}
	
	/** \brief Генератор случайных чисел.
	
		Генератор случайных чисел используется для статистического тестирования алгоритмов
		с длинными целыми числами. Используется для генерации последовательности чисел длины,
		не превышающей заданную. 
	*/
	class rng
	{
		
		char* Buffer;	//!< Буфер числа.
		int Length;	//!< Максимальное количество разрядов числа.
		
	public:
		
		/** \brief Создать генератор с заданной максимальной длиной чисел.
		
			Генератор вычисляет числа из диапазона [0, 10^length - 1] - неотрицательные числа,
			количество разрядов в которых не превышает length или числа со знаком из диапазона
			[-10^length + 1, 10^length - 1].
			При вычислении очередного числа сначала случайным образом из диапазона [0, length - 1]
			определяется длина числа, а затем каждый разряд заполнятеся случайной десятичной цифрой.
			
			\param[in]	length	Максимальная длина числа.
		*/
		rng(int length);
		
		~rng();
		
		/** \brief Выдать очередное случайное число.
		
			Метод вычисляет очередное случайное число. Если указан параметр signed, то число может быть отрицательным.
			Знак числа выбирается случайным образом. Если signed == false, то вычисляется положительное число.
			\see rng
			
			\param[in]	signed	Число может иметь знак.
			
			\return Cлучайное число.
		*/
		linteger get(bool signed = false);
		
		/** \brief Выдать очередное случайное число.
			\see get
			\return Случайное число.
		*/
		linteger operator(bool signed = true)()
		{
			return get();
		}
		
	};
};
