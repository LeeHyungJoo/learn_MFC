#pragma once

class RationalNum 
{
public:
	RationalNum(DOUBLE num);
	RationalNum(DOUBLE num, DOUBLE denom);
	RationalNum(const RationalNum& numFrac, const RationalNum& denomFrac);

	BOOL IsInteger() const;
	DOUBLE GetNumerator() const;
	DOUBLE GetDenomiator() const;

	DOUBLE GetValue() const;

public:
	RationalNum operator+(const RationalNum& other) const;
	RationalNum operator-(const RationalNum& other) const;
	RationalNum operator*(const RationalNum& other) const;
	RationalNum operator/(const RationalNum& other) const;
	void operator+=(const RationalNum& other);
	void operator-=(const RationalNum& other);
	void operator*=(const RationalNum& other);
	void operator/=(const RationalNum& other);
	BOOL operator==(const RationalNum& other) const;

private:
	LONG64 gcd(LONG64 a, LONG64 b) {
		while (b != 0) {
			LONG64 temp = a % b;
			a = b;
			b = temp;
		}
		return a;
	}

private:
	DOUBLE numerator;
	DOUBLE denominator;
};
