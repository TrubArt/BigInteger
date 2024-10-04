#include "Checker.h"

namespace
{
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
		return arg1 + arg2;
	}
	template <typename T>
	T divisionFunc(const T& arg1, const T& arg2)
	{
		return arg1 + arg2;
	}
	template <typename T>
	T remainderFunc(const T& arg1, const T& arg2)
	{
		return arg1 + arg2;
	}
}






OperationInFile::OperationInFile(Checker::operation op, const std::string & name) : op(op), file(name)
{}

void OperationInFile::setOperation(Checker::operation oper)
{
	op = oper;
}

void OperationInFile::putInFile(int x1, int x2)
{
	file << static_cast<int>(op) << " " << x1 << " " << x2 << "\n";
}





CheckStatus::CheckStatus(status st) : st(st)
{}

CheckStatus::CheckStatus(bool flag) : st(flag ? status::good : status::bad)
{}

std::string CheckStatus::name() const
{
	switch (st)
	{
	case status::undef:
		return "undefined operation";
		break;
	case status::good:
		return "good";
		break;
	case status::bad:
		return "bad";
		break;
	}
}

int CheckStatus::id() const
{
	return static_cast<int>(st);
}

std::ostream& operator<<(std::ostream& out, const CheckStatus& x)
{
	return out << x.name();
}





Checker::Checker(int lb, int ub) : firstNum{ lb, ub }, secondNum{ lb, ub }
{}

Checker::Checker(int firstLB, int firstUB, int secondLB, int secondUB) 
	: firstNum{ firstLB, firstUB }
	, secondNum{ secondLB, secondUB }
{}

CheckStatus Checker::checkOperation(operation op) const
{
	OperationInFile file(operation::addition);
	switch (op)
	{
	case operation::addition:
		return !check(file, additionFunc<BigInteger>, additionFunc<int>);
		break;

	case operation::subtraction:
		file.setOperation(operation::subtraction);
		return !check(file, subtractionFunc<BigInteger>, additionFunc<int>);
		break;

	case operation::multi:
		file.setOperation(operation::multi);
		return !check(file, multiFunc<BigInteger>, additionFunc<int>);
		break;

	case operation::division:
		file.setOperation(operation::division);
		return !check(file, divisionFunc<BigInteger>, additionFunc<int>);
		break;

	case operation::remainder:
		file.setOperation(operation::remainder);
		return !check(file, remainderFunc<BigInteger>, additionFunc<int>);
		break;

	default:
		return CheckStatus(CheckStatus::status::undef);
		break;
	}
}

bool Checker::check(OperationInFile& file, std::function<BigInteger(const BigInteger&, const BigInteger&)> BIF, std::function<int(int, int)> IF) const
{
	BigInteger bx1{};
	BigInteger bx2{};
	bool findError = false;

	for (int x1 = firstNum.first; x1 <= firstNum.second; ++x1)
	{
		bx1 = x1;
		//std::cout << x1 << "\n";
		for (int x2 = secondNum.first; x2 <= secondNum.second; ++x2)
		{
			bx2 = x2;
			//std::cout << BIF(bx1, bx2) << " " << IF(x1, x2) << "\n";
			if (BIF(bx1, bx2) != IF(x1, x2))
			{
				findError = true;
				file.putInFile(x1, x2);
			}
		}
	}
	return findError;
}