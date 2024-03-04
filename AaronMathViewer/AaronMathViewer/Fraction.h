#pragma once

class Fraction 
{
public:
	Fraction(DOUBLE num);
	Fraction(DOUBLE num, DOUBLE denom);
	Fraction(const Fraction& numFrac, const Fraction& denomFrac);

	BOOL IsInteger() const;
	LONG GetNumerator() const;
	LONG GetDenomiator() const;

	DOUBLE GetValue() const;

public:
	Fraction operator+(const Fraction& other) const;
	Fraction operator-(const Fraction& other) const;
	Fraction operator*(const Fraction& other) const;
	Fraction operator/(const Fraction& other) const;

private:
	LONG gcd(LONG a, LONG b) {
		while (b != 0) {
			int temp = a % b;
			a = b;
			b = temp;
		}
		return a;
	}

private:
	DOUBLE numerator;
	DOUBLE denominator;
};
