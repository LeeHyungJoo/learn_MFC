#pragma once

class Fraction 
{
public:
	Fraction(int num);
	Fraction(int num, int denom);

	bool IsInteger() const;
	int GetNumerator() const;
	int GetDenomiator() const;

public:
	Fraction operator+(const Fraction& other) const;
	Fraction operator-(const Fraction& other) const;
	Fraction operator*(const Fraction& other) const;
	Fraction operator/(const Fraction& other) const;

private:
	int gcd(int a, int b) {
		while (b != 0) {
			int temp = a % b;
			a = b;
			b = temp;
		}
		return a;
	}

private:
	int numerator;
	int denominator;
};
