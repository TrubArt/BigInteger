#include "Checker.h"

namespace
{
	void stopDebug(int numReal, int numCheck)
	{
		if (numReal == numCheck)
		{
			++numReal;
		}
	}
	template <typename T>
	T additionFunc(const T& arg1, const T& arg2)
	{
		return arg1 + arg2;
	}
	template <typename T>
	T subtractionFunc(const T& arg1, const T& arg2)
	{
		return arg1 - arg2;
	}
	template <typename T>
	T multiFunc(const T& arg1, const T& arg2)
	{
		return arg1 * arg2;
	}
	template <typename T>
	T divisionFunc(const T& arg1, const T& arg2)
	{
		return arg1 / arg2;
	}
	template <typename T>
	T remainderFunc(const T& arg1, const T& arg2)
	{
		return arg1 % arg2;
	}
}





OperationEnum::OperationEnum(operation op) : op(op)
{}

OperationEnum::operator operation() const
{
	return op;
}

std::string OperationEnum::name() const
{
	switch (op)
	{
	case operation::addition: return "+";
	case operation::subtraction: return "-";
	case operation::multi: return "*";
	case operation::division: return "/";
	case operation::remainder: return "%";
	default: return "undefOperation";
	}
}

int OperationEnum::id() const
{
	return static_cast<int>(op);
}

std::ostream& operator<<(std::ostream& out, const OperationEnum& x)
{
	return out << x.name();
}





OperationInFile::OperationInFile(const std::string& name) : file(name), op(OperationEnum::operation::addition)
{}

OperationInFile::OperationInFile(OperationEnum oper, const std::string& name) : OperationInFile(name)
{
	op = oper;
}

OperationInFile::~OperationInFile()
{
	file.close();
}

void OperationInFile::setOperation(OperationEnum oper)
{
	op = oper;
}

void OperationInFile::putInFile(int x1, int x2)
{
	file << op.name() << " " << x1 << " " << x2 << "\n";
}





StatusEnum::StatusEnum(status st) : st(st)
{}

StatusEnum::StatusEnum(bool flag) : st(flag ? status::good : status::bad)
{}

StatusEnum::operator status() const
{
	return st;
}

std::string StatusEnum::name() const
{
	switch (st)
	{
	case status::undef: return "undefined operation";
	case status::good: return "good";
	case status::bad: return "bad";
	}
}

int StatusEnum::id() const
{
	return static_cast<int>(st);
}

std::ostream& operator<<(std::ostream& out, const StatusEnum& x)
{
	return out << x.name();
}





Checker::Checker(int lb, int ub) : firstNum{ lb, ub }, secondNum{ lb, ub }
{}

Checker::Checker(int firstLB, int firstUB, int secondLB, int secondUB) 
	: firstNum{ firstLB, firstUB }
	, secondNum{ secondLB, secondUB }
{}

StatusEnum Checker::checkOperation(OperationEnum op) const
{
	using oper = OperationEnum::operation;
	OperationInFile file(op, "error" + op.name() + ".txt");

	switch (op)
	{
	case oper::addition: return !findError(file, additionFunc<BigInteger>, additionFunc<int>);
	case oper::subtraction: return !findError(file, subtractionFunc<BigInteger>, additionFunc<int>);
	case oper::multi: return !findError(file, multiFunc<BigInteger>, additionFunc<int>);
	case oper::division: return !findError(file, divisionFunc<BigInteger>, additionFunc<int>);
	case oper::remainder: return !findError(file, remainderFunc<BigInteger>, additionFunc<int>);
	default: return StatusEnum(StatusEnum::status::undef);
	}
}

bool Checker::findError(OperationInFile& file, std::function<BigInteger(const BigInteger&, const BigInteger&)> BIF, std::function<int(int, int)> IF) const
{
	BigInteger bx1{};
	BigInteger bx2{};
	bool findError = false;

	int intervalMessage = (firstNum.second - firstNum.first) / 100;
	if (intervalMessage == 0)
		intervalMessage = firstNum.second - firstNum.first;

	for (int x1 = firstNum.first; x1 <= firstNum.second; ++x1)
	{
		if (intervalMessage && x1 % intervalMessage == 0)
			std::cout << "x1: " << x1 << "\n";
		bx1 = x1;
		for (int x2 = secondNum.first; x2 <= secondNum.second; ++x2)
		{
			bx2 = x2;
			//std::cout << "x2: " << x2 << "\n";
			//stopDebug(x2, 100);
			//std::cout << "ВЫчисления: " << BIF(bx1, bx2) << " " << IF(x1, x2) << "\n";
			if (BIF(bx1, bx2) != IF(x1, x2))
			{
				findError = true;
				file.putInFile(x1, x2);
			}
		}
	}
	return findError;
}