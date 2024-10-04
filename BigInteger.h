#pragma once
#include <iostream>
#include <vector>
#include <string>

class BigInteger
{
public:
	BigInteger();
	BigInteger(long long);
	BigInteger(const std::string&);

	bool isNULL() const;
	bool isNegative() const;
	size_t intSize() const;
	std::string toString() const;

	BigInteger abs() const;

	int& operator[](size_t);
	const int& operator[](size_t) const;
	BigInteger operator+() const;
	BigInteger operator-() const;
	BigInteger& operator++();
	BigInteger operator++(int);
	BigInteger& operator--();
	BigInteger operator--(int);
	BigInteger& operator+=(const BigInteger&);
	BigInteger& operator-=(const BigInteger&);
	BigInteger& operator*=(const BigInteger&);
	BigInteger& operator/=(const BigInteger&);
	BigInteger& operator%=(const BigInteger&);

private:
	void shrinkToFit();
	void increaseData(int);

	// число хранится в data в перевёрнутом виде для простоты операций

	std::vector<int> data;
	size_t size;
	bool isNegat;
};

bool operator==(const BigInteger&, const BigInteger&);
bool operator!=(const BigInteger&, const BigInteger&);
bool operator<(const BigInteger&, const BigInteger&);
bool operator<=(const BigInteger&, const BigInteger&);
bool operator>(const BigInteger&, const BigInteger&);
bool operator>=(const BigInteger&, const BigInteger&);
BigInteger operator+(const BigInteger&, const BigInteger&);
BigInteger operator-(const BigInteger&, const BigInteger&);
BigInteger operator*(const BigInteger&, const BigInteger&);
BigInteger operator/(const BigInteger&, const BigInteger&);
BigInteger operator%(const BigInteger&, const BigInteger&);

BigInteger operator""_bi(unsigned long long);
std::ostream& operator<<(std::ostream&, const BigInteger&);