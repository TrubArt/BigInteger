#pragma once
#include "BigInteger.h"
#include <functional>
#include <fstream>
#include <string>

class OperationInFile;
class CheckStatus;



// проверяет правильность написания класса BigInteger
// сравнивает полученные после арифмитических операций значения BigInteger с правильными int-овыми вычислениями
class Checker
{
public:
	enum class operation 
	{
		addition,
		subtraction,
		multi,
		division,
		remainder
	};

	Checker(int, int);
	Checker(int, int, int, int);
	CheckStatus checkOperation(operation) const;

private:
	bool check(OperationInFile&, std::function<BigInteger(const BigInteger&, const BigInteger&)>, std::function<int(int, int)>) const;

	std::pair<int, int> firstNum;
	std::pair<int, int> secondNum;
};




// записывает данные в файл. Нужен для того, чтобы посмотреть
// с какими числами неправльно работают вычисления
class OperationInFile
{
	using oper = Checker::operation;

public:
	OperationInFile(Checker::operation op, const std::string& = "errors.txt");
	void setOperation(Checker::operation op);
	void putInFile(int, int);

private:
	std::ofstream file;
	oper op;
};



// класс-оболочка для enum status
class CheckStatus
{
public:
	enum class status
	{
		undef = -1,
		bad,
		good
	};

	CheckStatus(status);
	CheckStatus(bool);
	std::string name() const;
	int id() const;

private:
	status st;
};
std::ostream& operator<<(std::ostream&, const CheckStatus&);
