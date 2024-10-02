#include "BigInteger.h"
#include <iostream>
#define NOMINMAX
#include <Windows.h>

void checkCreate();
void checkEqua();

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	//checkCreate();
	checkEqua();
}




void checkCreate()
{
	long long x = -12345;
	std::cout << x << "\n\n";
	BigInteger blong(x);
	std::cout << blong.intSize() << " " << blong[0] << " " << blong[4] << "\n";
	std::cout << blong.toString() << "\n\n";

	std::string str = "-12345";
	std::cout << str << "\n\n";
	BigInteger bstr(x);
	std::cout << bstr.intSize() << " " << bstr[0] << " " << bstr[4] << "\n";
	std::cout << bstr.toString() << "\n\n";
}

void checkEqua()
{
	std::cout << (1321_bi != 1321_bi) << "\n";
}