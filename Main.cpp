#include "BigInteger.h"
#include <iostream>
#define NOMINMAX
#include <Windows.h>

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	//long long x = -3000020060005000041; //19
	//std::cout << x << "\n\n";
	//BigInteger bx(x);
	//std::cout << bx.size() << " " << bx[0] << " " << bx[19] << "\n";
	//std::cout << bx.toString();

	std::string x = "-3000020060005000041"; //19
	std::cout << x << "\n\n";
	BigInteger bx(x);
	std::cout << bx.size() << " " << bx[0] << " " << bx[19] << "\n";
	std::cout << bx.toString();
}