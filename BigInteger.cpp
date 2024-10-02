#include "BigInteger.h"

namespace
{
	std::string getStringWithoutSpace(const std::string& str)
	{
		std::string answ = str;
		for (size_t i = 0; i < answ.size(); ++i)
			if (answ[i] == ' ')
				answ.erase(i, 1);
		return answ;
	}
}

BigInteger::BigInteger(long long x) : data(), isNegat(false), size(0)
{
	if (x < 0)
	{
		isNegat = true;
		x *= -1;
	}

	while (x != 0)
	{
		data.push_back(x % 10);
		++size;
		x /= 10;
	}
}

BigInteger::BigInteger(const std::string& str) : data(), isNegat(false), size(0)
{
	data.reserve(str.size());
	std::string goodStr = getStringWithoutSpace(str);

	if (goodStr[0] == '-')
	{
		isNegat = true;
		goodStr.erase(0, 1);
	}

	size = goodStr.size();
	auto it = goodStr.crbegin();
	while (it != goodStr.crend())
	{
		data.push_back(*it++ - 48);
	}

	shrinkToFit();
}

void BigInteger::shrinkToFit()
{	
	if (isNULL())
		return;

	auto lastSpace = data.end();
	auto firstSpace = lastSpace - 1;
	while (*firstSpace == 0)
	{
		--firstSpace;
		--size;
	}
	data.erase(firstSpace + 1, lastSpace);
}

bool BigInteger::isNULL() const
{
	for (const auto& i : data)
		if (i != 0)
			return false;
	return true;
}

bool BigInteger::isNegative() const
{
	return isNegat;
}

size_t BigInteger::intSize() const
{
	return size;
}

std::string BigInteger::toString() const
{
	std::string answ{ isNegat ? "-" : "" };
	for (auto it = data.crbegin(); it != data.crend(); ++it)
		answ += std::to_string(*it);
	return answ; // RVO
}

int& BigInteger::operator[](size_t index)
{
	if (index >= data.size())
		throw "fatal index";

	return data[size - 1 - index];
}

const int& BigInteger::operator[](size_t index) const
{
	if (index >= data.size())
		throw "fatal index";
	return data[size - 1 - index];
}

BigInteger BigInteger::operator+() const
{
	return *this;
}

BigInteger BigInteger::operator-() const
{
	BigInteger negative(*this);
	negative.isNegat = !isNegat;
	return negative;
}


bool operator==(const BigInteger& a, const BigInteger& b)
{
	if (a.intSize() != b.intSize())
		return false;
	if (a.isNegative() != b.isNegative())
		return false;

	std::string astr = a.toString();
	std::string bstr = b.toString();

	for (size_t ind = 0, size = astr.size(); ind < size; ++ind)
	{
		if (astr[ind] != bstr[ind])
			return false;
	}
	return true;
}

bool operator!=(const BigInteger& a, const BigInteger& b)
{
	return !(a == b);
}

bool operator<(const BigInteger& a, const BigInteger& b)
{
	if (a.isNegative() ^ b.isNegative())	// разные знаки
	{
		if (a.isNegative())
			return true;
		return false;
	}

	std::string astr = a.toString();
	std::string bstr = b.toString();
	if (a.isNegative())						// если оба числа отрицательные
	{
		if (a.intSize() > b.intSize())		// сравнение размерностей
			return true;

		for (size_t ind = 0, size = astr.size(); ind < size; ++ind)
		{
			if (astr[ind] != bstr[ind])
			{
				if (astr[ind] > bstr[ind])
					return true;
				return false;
			}
		}
		return false; // случай равенства
	}

	if (a.intSize() > b.intSize())		// сравнение размерностей
		return false;
	for (size_t ind = 0, size = astr.size(); ind < size; ++ind)
	{
		if (astr[ind] != bstr[ind])
		{
			if (astr[ind] > bstr[ind])
				return false;
			return true;
		}
	}
	return false; // случай равенства
}

bool operator<=(const BigInteger& a, const BigInteger& b)
{
	return !(a > b);
}

bool operator>(const BigInteger& a, const BigInteger& b)
{
	return b < a;
}

bool operator>=(const BigInteger& a, const BigInteger& b)
{
	return !(a < b);
}


BigInteger operator""_bi(unsigned long long x)
{
	return BigInteger(x);
}

std::ostream& operator<<(std::ostream& out, const BigInteger& x)
{
	return out << x.toString();
}