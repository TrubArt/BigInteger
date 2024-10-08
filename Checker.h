#pragma once
#include "BigInteger.h"
#include <functional>
#include <fstream>
#include <string>

class StatusEnum;
class OperationInFile;


// класс-оболочка для enum operation
class OperationEnum
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

	OperationEnum(operation);
	operator operation() const;
	std::string name() const;
	int id() const;

private:
	operation op;
};
std::ostream& operator<<(std::ostream&, const OperationEnum&);




// проверяет правильность написания класса BigInteger
// сравнивает полученные после арифмитических операций значения BigInteger с правильными int-овыми вычислениями
class Checker
{
public:
	Checker(int, int);
	Checker(int, int, int, int);
	StatusEnum checkOperation(OperationEnum) const;

private:
	bool findError(OperationInFile& file, std::function<BigInteger(const BigInteger&, const BigInteger&)>, std::function<int(int, int)>) const;

	std::pair<int, int> firstNum;	// диапазон первого числа
	std::pair<int, int> secondNum;	// диапазон второго числа
};




// записывает данные в файл. Нужен для того, чтобы посмотреть
// с какими числами неправильно работают вычисления
class OperationInFile
{
public:
	OperationInFile(const std::string & = "errors.txt");
	OperationInFile(OperationEnum, const std::string& = "errors.txt");
	~OperationInFile();
	void setOperation(OperationEnum);
	void putInFile(int, int);

private:
	std::ofstream file;
	OperationEnum op;
};




// класс-оболочка для enum status
class StatusEnum
{
public:
	enum class status
	{
		undef = -1,
		bad,
		good
	};

	StatusEnum(status);
	StatusEnum(bool);
	operator status() const;
	std::string name() const;
	int id() const;

private:
	status st;
};
std::ostream& operator<<(std::ostream&, const StatusEnum&);