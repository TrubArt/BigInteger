#include "BigInteger.h"
#include <iostream>
#define NOMINMAX
#include <Windows.h>

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	long long x = -12345;
	std::cout << x << "\n\n";
	BigInteger bx(x);
	std::cout << bx.integerSize() << " " << bx[0] << " " << bx[4] << "\n";
	std::cout << bx.toString();

	//std::string x = "-12345";
	//std::cout << x << "\n\n";
	//BigInteger bx(x);
	//std::cout << bx.integerSize() << " " << bx[0] << " " << bx[4] << "\n";
	//std::cout << bx.toString() << "\n";
	//bx.shrinkToFit();
	//std::cout << bx.integerSize() << " " << bx.toString();
}