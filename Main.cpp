#include "BigInteger.h"
#include "Checker.h"
#include <iostream>
#define NOMINMAX
#include <Windows.h>
#include <bitset>

void checkCreate();
void checkEqua();

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	//std::cout << tmp.count() << "\n";
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
	Checker ch(-100, 100);
	std::cout << "Сложение - " << ch.checkOperation(OperationEnum(OperationEnum::operation::addition)) << "\n";
	std::cout << "Вычитание - " << ch.checkOperation(OperationEnum(OperationEnum::operation::subtraction)) << "\n";
	std::cout << "Умножение - " << ch.checkOperation(OperationEnum(OperationEnum::operation::multi)) << "\n";

	Checker ch2(1, 1000, 1, 100 );
	std::cout << "Деление - " << ch2.checkOperation(OperationEnum(OperationEnum::operation::division)) << "\n";
	std::cout << "Остаток - " << ch2.checkOperation(OperationEnum(OperationEnum::operation::remainder)) << "\n";
}