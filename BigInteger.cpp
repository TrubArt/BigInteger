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

	size_t findFirstNotNullIndex(size_t indStart, const std::vector<int>& vec)
	{
		for (size_t ind = indStart + 1; ind < vec.size(); ++ind)
		{
			if (vec[ind] != 0)
				return ind;
		}
		throw std::exception("unreal situtaion in subtraction\n");
		return 0;	// тк на вход идёт подходящий вектор, в эту строчку программа никогда не зайдёт
	}
}

BigInteger::BigInteger() : BigInteger(0) 
{}

BigInteger::BigInteger(long long x) : data(), isNegat(false), size(0)
{
	if (x < 0)
	{
		isNegat = true;
		x *= -1;
	}
	if (x == 0)
	{
		makeNULL();
	}
	while (x != 0)
	{
		increaseData(x % 10);
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

	auto it = goodStr.crbegin();
	while (it != goodStr.crend())
	{
		increaseData(*it++ - 48);
	}

	shrinkToFit();
}

BigInteger::BigInteger(const std::vector<int>& x, bool sign) : data(x), isNegat(sign), size(data.size())
{}

void BigInteger::makeNULL()
{
	data.clear();
	size = 0;
	increaseData(0);
}

void BigInteger::shrinkToFit()
{	
	if (isNULL())
	{
		makeNULL();
		return;
	}

	auto lastSpace = data.end();
	auto firstSpace = lastSpace - 1;
	while (*firstSpace == 0)
	{
		--firstSpace;
		--size;
	}
	data.erase(firstSpace + 1, lastSpace);
}

void BigInteger::increaseData(int value)
{
	data.push_back(value);
	++size;
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

BigInteger BigInteger::abs() const
{
	BigInteger tmp = *this;
	if (isNegat)
		tmp.isNegat = false;
	return tmp;
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

BigInteger& BigInteger::operator++()
{
	*this += 1_bi;
	return *this;
}

BigInteger BigInteger::operator++(int)
{
	BigInteger copy = *this;
	*this += 1_bi;
	return copy;
}

BigInteger& BigInteger::operator--()
{
	*this -= 1_bi;
	return *this;
}

BigInteger BigInteger::operator--(int)
{
	BigInteger copy = *this;
	*this -= 1_bi;
	return copy;
}


BigInteger& BigInteger::operator+=(const BigInteger& x)
{
	//std::cout << "Сумма: " << *this << " " << x << "\n";
	// определение знаков
	if (isNegat && x.isNegat)			// - -
	{
		isNegat = false;	// так быстрее,чем арифметические операции
		*this += (-x);
		isNegat = true;
		return *this;
	}
	if (isNegat)						// - +
		return *this = x - (-*this); // ?избавиться от копирования?
	if (!isNegat && x.isNegat)			// + -
		return *this -= (-x);


	size_t minSize = std::min(size, x.size);
	size_t maxSize = std::max(size, x.size);
	int transfer = 0;	// остаток

	// вычисление совпадающих разрядов
	for (size_t index = 0; index < minSize; ++index)
	{
		data[index] += x.data[index] + transfer;
		transfer = data[index] / 10;
		data[index] %= 10;
	}

	// вычисление оставшихся разрядов
	if (minSize == size)	// if вынесен за пределы цикла ради уменьшения числа итераций
	{
		int sum = 0;
		for (size_t count = minSize; count < maxSize; ++count)
		{
			sum = x.data[count] + transfer;
			transfer = sum / 10;
			increaseData(sum % 10);
		}
	}
	else
	{
		for (size_t count = minSize; count < maxSize; ++count)
		{
			data[count] += transfer;
			transfer = data[count] / 10;
			data[count] %= 10;
		}
	}

	if (transfer == 1)
		increaseData(1);

	return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& x)
{
	//std::cout << "Разность: " << *this << " " << x << "\n";
	// определение знаков
	if (!isNegat && x.isNegat)		// + -
		return *this += (-x);
	if (isNegat && x.isNegat)		// - -
		return *this = (-x) - (-*this);
	if (isNegat)					// - +
	{
		isNegat = false;
		*this += x;
		isNegat = true;
		return *this;
	}

	// проверка корректности вычитания
	if (*this < x)
	{
		*this = x - *this;
		isNegat = true;
		return *this;
	}

	// вычисление совпадающих разрядов
	for (size_t index = 0; index < x.size; ++index)
	{
		if (data[index] < x.data[index])
		{
			size_t creditor = findFirstNotNullIndex(index, data);
			data[creditor] -= 1;		// отдаём займ
			// обработка лучая займа у нулей
			for (creditor -= 1; creditor != index; --creditor)
			{
				data[creditor] = 9;
			}

			data[index] += 10;			// получение займа
		}

		data[index] -= x.data[index];
	}

	shrinkToFit();
	return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& x)
{
	BigInteger answer(std::vector<int>(size + x.size), true);

	// определение знака
	if (isNegat ^ x.isNegat)	// разные знаки
		answer.isNegat = true;
	else
		answer.isNegat = false;

	for (size_t x2Ind = 0; x2Ind < x.size; ++x2Ind)
	{
		for (size_t x1Ind = 0; x1Ind < size; ++x1Ind)
		{
			answer.data[x2Ind + x1Ind] += x.data[x2Ind] * data[x1Ind];
		}
	}

	// перенос разрядов
	int shift = 0;
	for (auto& i : answer.data)
	{
		i += shift;
		shift = i / 10;
		i %= 10;
	}

	answer.shrinkToFit();
	return *this = answer;
}

BigInteger& BigInteger::operator/=(const BigInteger& x)
{
	return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& x)
{
	return *this;
}

bool operator==(const BigInteger& a, const BigInteger& b)
{
	// hardcode 0, becouse (-0 = 0)
	if (a.isNULL() && b.isNULL())
		return true;

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
	// hardcode 0, becouse (-0 = 0)
	if (a.isNULL() && b.isNULL())
		return false;

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
		if (a.intSize() < b.intSize())
			return false;

		// случай равенства размерностей
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
	if (a.intSize() < b.intSize())
		return true;

	// случай равенства размерностей
	for (size_t ind = 0, size = astr.size(); ind < size; ++ind)
	{
		if (astr[ind] != bstr[ind])
		{
			if (astr[ind] > bstr[ind])
				return false;
			return true;
		}
	}
	return false;
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

BigInteger operator+(const BigInteger& a, const BigInteger& b)
{
	BigInteger tmp = a;
	tmp += b;
	return tmp;
}

BigInteger operator-(const BigInteger& a, const BigInteger& b)
{
	BigInteger tmp = a;
	tmp -= b;
	return tmp;
}

BigInteger operator*(const BigInteger& a, const BigInteger& b)
{
	BigInteger tmp = a;
	tmp *= b;
	return tmp;
}

BigInteger operator/(const BigInteger& a, const BigInteger& b)
{
	BigInteger tmp = a;
	tmp /= b;
	return tmp;
}

BigInteger operator%(const BigInteger& a, const BigInteger& b)
{
	BigInteger tmp = a;
	tmp %= b;
	return tmp;
}


BigInteger operator""_bi(unsigned long long x)
{
	return BigInteger(x);
}

std::ostream& operator<<(std::ostream& out, const BigInteger& x)
{
	return out << x.toString();
}