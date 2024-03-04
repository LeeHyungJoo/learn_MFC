#pragma once

class Fraction 
{
public:
	Fraction(DOUBLE num);
	Fraction(DOUBLE num, DOUBLE denom);
	Fraction(const Fraction& numFrac, const Fraction& denomFrac);

	BOOL IsInteger() const;
	DOUBLE GetNumerator() const;
	DOUBLE GetDenomiator() const;

	DOUBLE GetValue() const;

public:
	Fraction operator+(const Fraction& other) const;
	Fraction operator-(const Fraction& other) const;
	Fraction operator*(const Fraction& other) const;
	Fraction operator/(const Fraction& other) const;

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
