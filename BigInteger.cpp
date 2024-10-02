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

BigInteger::BigInteger(long long x) : data(), isNegative(false), size(0)
{
	if (x < 0)
	{
		isNegative = true;
		x *= -1;
	}

	while (x != 0)
	{
		data.push_back(x % 10);
		++size;
		x /= 10;
	}
}

BigInteger::BigInteger(const std::string& str) : data(), isNegative(false), size(0)
{
	data.reserve(str.size());
	std::string goodStr = getStringWithoutSpace(str);

	if (goodStr[0] == '-')
	{
		isNegative = true;
		goodStr.erase(0, 1);
	}

	size = goodStr.size();
	auto it = goodStr.crbegin();
	while (it != goodStr.crend())
	{
		data.push_back(*it++ - 48);
	}
}

size_t BigInteger::integerSize() const
{
	return size;
}

std::string BigInteger::toString() const
{
	std::string answ{ isNegative ? "-" : "" };
	for (auto it = data.crbegin(); it != data.crend(); ++it)
		answ += std::to_string(*it);
	return answ; // RVO
}

int& BigInteger::operator[](size_t index)
{
	if (index >= data.size())
		throw "fatal index";

	return data[index];
}

const int& BigInteger::operator[](size_t index) const
{
	if (index >= data.size())
		throw "fatal index";
	return data[index];
}

BigInteger operator""_bi(unsigned long long x)
{
	return BigInteger(x);
}

std::ostream& operator<<(std::ostream& out, const BigInteger& x)
{
	return out << x.toString();
}