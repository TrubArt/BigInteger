#pragma once
#include <iostream>
#include <vector>
#include <string>

class BigInteger
{
public:
	BigInteger() = delete;
	BigInteger(long long);
	BigInteger(const std::string&);

	bool isNULL() const;
	bool isNegative() const;
	size_t intSize() const;
	std::string toString() const;

	int& operator[](size_t);
	const int& operator[](size_t) const;
	BigInteger operator+() const;
	BigInteger operator-() const;

private:
	void shrinkToFit();
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

BigInteger operator""_bi(unsigned long long);
std::ostream& operator<<(std::ostream&, const BigInteger&);