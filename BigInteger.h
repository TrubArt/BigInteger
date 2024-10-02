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

	void shrinkToFit();
	size_t integerSize() const;
	std::string toString() const;

	int& operator[](size_t);
	const int& operator[](size_t) const;

private:
	// число хранится в data в перевёрнутом виде для простоты операций

	std::vector<int> data;
	size_t size;
	bool isNegative;
};

BigInteger operator""_bi(unsigned long long);
std::ostream& operator<<(std::ostream&, const BigInteger&);