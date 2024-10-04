#include "BigInteger.h"
#include "Checker.h"
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
	BigInteger bstr(str);
	std::cout << bstr.intSize() << " " << bstr[0] << " " << bstr[4] << "\n";
	std::cout << bstr.toString() << "\n\n";

	BigInteger bistr = 12345_bi;
	std::cout << bistr.intSize() << " " << bistr[0] << " " << bistr[4] << "\n";
	std::cout << bistr.toString() << "\n\n";
}

void checkEqua()
{
	Checker ch(0, 100);
	std::cout << ch.checkOperation(Checker::operation::addition) << "\n";
}