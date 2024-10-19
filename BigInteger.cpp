#include "BigInteger.h"

// for bitPow
#include <bitset>

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
		throw std::exception("vec is NULL in subtraction\n");
		return 0;	// тк на вход идёт подходящий вектор, в эту строчку программа никогда не зайдёт
	}

	// функция конвертации BigInteger в std::vector
	// Знак не учитывается
	// Вспомогательная для оператора деления
	std::vector<int> toVec(const BigInteger& x)
	{
		std::vector<int> answer;
		for (size_t ind = 0; ind < x.intSize(); ++ind)
		{
			answer.push_back(x[ind]);
		}
		return answer;
	}

	// (A/B) Возвращает (x(наибольшую цифру, на которую надо домножить B, чтобы оно было меньше A), B * x)
	// Работает методом бинарного поиска. Максимум 4 итерации
	// Вспомогательная для оператора деления
	int findMultiplyable(const BigInteger& A, const BigInteger& B)
	{
		int start = 1;
		int end = 10;
		int midle = 0;
		BigInteger number;
		do
		{
			midle = (start + end) / 2;
			number = B * midle;

			if (number <= A)
				start = midle;
			else
				end = midle;
		} while (end - start != 1);

		return start;
	}
}

BigInteger::BigInteger() : BigInteger(0) 
{}

BigInteger::BigInteger(long long x) : data(), size(0), isNegat(false)
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

BigInteger::BigInteger(const std::string& str) : data(), size(0), isNegat(false)
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

BigInteger::BigInteger(const std::vector<int>& x, bool sign) : data(), size(x.size()), isNegat(sign)
{
	data.reserve(x.size());
	for (auto revIt = x.rbegin(); revIt != x.rend(); ++revIt)
	{
		data.push_back(*revIt);
	}
	shrinkToFit();
}

BigInteger::operator long long() const
{
	if (size > 20)
		throw std::exception("unreal cast to long long");
	return stoll(toString());
}

BigInteger BigInteger::stupidVectorCreate(const std::vector<int>& x, bool sign)
{
	BigInteger answer; 
	answer.data = x;
	answer.size = x.size();
	answer.isNegat = sign;
	return answer;
}

BigInteger& BigInteger::makeNULL()
{
	data.clear();
	size = 0;
	increaseData(0);
	return *this;
}

BigInteger& BigInteger::shrinkToFit()
{	
	if (isNULL())
	{
		return makeNULL();
	}

	auto lastSpace = data.end();
	auto firstSpace = lastSpace - 1;
	while (*firstSpace == 0)
	{
		--firstSpace;
		--size;
	}
	data.erase(firstSpace + 1, lastSpace);
	return *this;
}

void BigInteger::increaseData(int value)
{
	data.push_back(value);
	++size;
}

// возвращает остаток от деления
BigInteger BigInteger::findRemainder(const BigInteger& b)
{
	// можно оптимизировать и найти первый tmpNumber за 1 сравнение
	// предварительно необходимо tmpNumber сделать равным *this[0,b.size)
	// тогда if (tmpNumber < b) tmpNumber = *this[0,b.size]

	std::vector<int> tmpNumber;
	for (auto index = data.rbegin(); index != data.rend(); ++index)
	{
		tmpNumber.push_back(*index);

		// if необходим для оптимизации. Если убирать if, то необходимо сделать start = 0 в функции findMultiplyable
		if (static_cast<BigInteger>(tmpNumber) < b)
			*index = 0;
		else
		{
			// число - ans, BigInteger - b * ans
			int num = findMultiplyable(tmpNumber, b);

			//std::cout << numAndBi.first << " " << numAndBi.second << "\n";

			*index = num;
			tmpNumber = toVec(static_cast<BigInteger>(tmpNumber) - num * b);
		}
	}
	return tmpNumber;
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
	BigInteger tmp(*this);
	if (isNegat)
		tmp.isNegat = false;
	return tmp;
}

BigInteger BigInteger::powBadSlow(long long power) const
{
	if (power < 0)
		return BigInteger(0);

	BigInteger answer = 1;
	for (int count = 0; count < power; ++count)
	{
		answer *= (*this);
	}
	return answer;
}

BigInteger BigInteger::powRightLeft(long long power) const
{
	if (power < 0)
		return BigInteger(0);

	std::bitset<sizeof(power)> bitPower = power;
	BigInteger answer = bitPower[0] == 1 ? *this : 1;
	BigInteger tmpPowerValue = *this;

	for (size_t ind = 1; ind < bitPower.size(); ++ind)
	{
		tmpPowerValue *= tmpPowerValue;
		if (bitPower[ind] != 0)
			answer *= tmpPowerValue;
	}
	return answer;
}

int& BigInteger::operator[](size_t index)
{
	if (index >= data.size())
		throw std::out_of_range("fatal index");

	return data[size - 1 - index];
}

const int& BigInteger::operator[](size_t index) const
{
	if (index >= data.size())
		throw std::out_of_range("fatal index");
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
	return *this += 1_bi;
}

BigInteger BigInteger::operator++(int)
{
	BigInteger copy(*this);
	++(*this);
	return copy;
}

BigInteger& BigInteger::operator--()
{
	return *this -= 1_bi;
}

BigInteger BigInteger::operator--(int)
{
	BigInteger copy(*this);
	--(*this);
	return copy;
}


BigInteger& BigInteger::operator+=(const BigInteger& b)
{
	// определение знаков
	if (isNegat && b.isNegat)			// - -
	{
		isNegat = false;	// так быстрее,чем арифметические операции
		*this += (-b);
		isNegat = true;
		return *this;
	}
	if (isNegat)						// - +
		return *this = b - (-*this); // ?избавиться от копирования?
	if (!isNegat && b.isNegat)			// + -
		return *this -= (-b);


	size_t minSize = std::min(size, b.size);
	size_t maxSize = std::max(size, b.size);
	int transfer = 0;	// остаток

	// вычисление совпадающих разрядов
	for (size_t index = 0; index < minSize; ++index)
	{
		data[index] += b.data[index] + transfer;
		transfer = data[index] / 10;
		data[index] %= 10;
	}

	// вычисление оставшихся разрядов
	if (minSize == size)	// if вынесен за пределы цикла ради уменьшения числа итераций
	{
		int sum = 0;
		for (size_t count = minSize; count < maxSize; ++count)
		{
			sum = b.data[count] + transfer;
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

BigInteger& BigInteger::operator-=(const BigInteger& b)
{
	// определение знаков
	if (!isNegat && b.isNegat)		// + -
		return *this += (-b);
	if (isNegat && b.isNegat)		// - -
		return *this = (-b) - (-*this);
	if (isNegat)					// - +
	{
		isNegat = false;
		*this += b;
		isNegat = true;
		return *this;
	}

	// проверка корректности вычитания
	if (*this < b)
	{
		*this = b - *this;
		isNegat = true;
		return *this;
	}

	// вычисление совпадающих разрядов
	for (size_t index = 0; index < b.size; ++index)
	{
		if (data[index] < b.data[index])
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

		data[index] -= b.data[index];
	}

	return shrinkToFit();
}

BigInteger& BigInteger::operator*=(const BigInteger& b)
{
	BigInteger answer = stupidVectorCreate(std::vector<int>(size + b.size), true);

	// определение знака 
	answer.isNegat = isNegat ^ b.isNegat;

	for (size_t x2Ind = 0; x2Ind < b.size; ++x2Ind)
	{
		for (size_t x1Ind = 0; x1Ind < size; ++x1Ind)
		{
			answer.data[x2Ind + x1Ind] += b.data[x2Ind] * data[x1Ind];
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

BigInteger& BigInteger::operator/=(const BigInteger& b)
{
	// можно оптимизировать путём игры со знаками, без дополнительных копирований
	if (b.abs() > abs())
		return makeNULL();
	// определение знака
	isNegat ^= b.isNegat;

	findRemainder(b);

	return shrinkToFit();
}

BigInteger& BigInteger::operator%=(const BigInteger& b)
{
	if (b.abs() > abs())
		return *this;
	// в с++ почему-то сохраняется знак
	bool neg = isNegat;
	*this = findRemainder(b);
	isNegat = neg;

	return *this;
}

bool operator==(const BigInteger& a, const BigInteger& b)
{
	// hardcode 0, because (-0 = 0)
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
	// hardcode 0, because (-0 = 0)
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