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

	size_t size() const;
	std::string toString() const;

	int& operator[](size_t index);
	const int& operator[](size_t index) const;

private:
	// перва€ €чейка знак, остальные - число
	std::vector<int> data;
};

BigInteger operator""_bi(unsigned long long);
std::ostream& operator<<(std::ostream&, const BigInteger&);