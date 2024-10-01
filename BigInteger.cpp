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

BigInteger::BigInteger(long long x) : data(20)
{
	x < 0 ? data[0] = -1 : data[0] = 1;

	size_t ind = 19;
	while (x != 0)
	{
		data[ind--] = x % 10 * data[0];
		x /= 10;
	}
}

BigInteger::BigInteger(const std::string& str) : data{}
{
	data.reserve(str.size());
	std::string goodStr = getStringWithoutSpace(str);
	auto it = goodStr.cbegin();

	if (goodStr[0] == '-')
	{
		data.push_back(-1);
		++it;
	}
	else
		data.push_back(1);

	while (it < goodStr.cend())
	{
		data.push_back(*it++ - 48);
		std::cout << this->toString() << "\n";
	}
}

size_t BigInteger::size() const
{
	return data.size() - 1; // тк храним ещё знак
}

std::string BigInteger::toString() const
{
	std::string answ{ data[0] < 0 ? "-" : "" };
	for (size_t ind = 1; ind < data.size(); ++ind)
		answ += std::to_string(data[ind]);
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