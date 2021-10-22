#include <iostream>

using namespace std;

void coutp(unsigned int x)
{
	int n = sizeof(int) * 8;
	unsigned  maska = (1 << (n - 1));

	for (int i = 1; i <= n; i++)
	{
		cout << ((x & maska) >> (n - i));
		maska = maska >> 1;
	}
	cout << endl;
}

int setBitToOne(int x) 
{
	unsigned int mask = 0x55555555;
	return x | mask;
}

int setBitToZero(int x)
{
	unsigned int mask = 0x1E0;
	return x & (~mask);
}

int makeBitMultiply(int x)
{
	unsigned int mask = 5;
	return x << mask;
}

int makeBitDivision(int x)
{
	unsigned int mask = 5;
	return (x >> mask);
}

int setNBitToOne(int x, int n)
{
	unsigned int mask = 0x80000000;
	mask = mask >> (31 - n);
	return x | mask;
}

int main()
{
	setlocale(LC_ALL, "russian");
    unsigned int var = 0xABCDEF;
	cout << "Задание №1" << endl;
	cout << "Первоначальное число: " << var << endl;
	coutp(var);
	var = setBitToOne(var);
	coutp(var);
	cout << "Число после выполнения операции: " << var << endl;


	cout << "Задание №2" << endl;
	cout << "Певоначальное число: ";
	unsigned int var2;
	cin >> var2;
	cout << endl;
	coutp(var2);
	var2 = setBitToZero(var2);
	coutp(var2);
	cout << "Число после выполнения операции: " << var2 << endl;


	cout << "Задание №3" << endl;
	cout << "Певоначальное число: ";
	unsigned int var3;
	cin >> var3;
	cout << endl;
	coutp(var3);
	var3 = makeBitMultiply(var3);
	coutp(var3);
	cout << "Число после выполнения операции: " << var3 << endl;



	cout << "Задание №4" << endl;
	cout << "Певоначальное число: ";
	unsigned int var4;
	cin >> var4;
	cout << endl;
	coutp(var4);
	var4 = makeBitDivision(var4);
	coutp(var4);
	cout << "Число после выполнения операции: " << var4 << endl;


	cout << "Задание №5" << endl;
	cout << "Певоначальное число: ";
	unsigned int var5;
	cin >> var5;
	cout << endl;
	cout << "Порядковый номер бита: ";
	unsigned int var6;
	cin >> var6;
	cout << endl;
	coutp(var5);
	var5 = setNBitToOne(var5, var6);
	coutp(var5);
	cout << "Число после выполнения операции: " << var5 << endl;

}
