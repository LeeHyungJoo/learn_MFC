#include "pch.h"
#include "Fraction.h"

Fraction::Fraction(int num, int denom) 
	: numerator(num), denominator(denom) 
{
	if (denominator < 0) {
		numerator = -numerator;
		denominator = -denominator;
	}

	int factor = gcd(abs(numerator), denominator);
	numerator /= factor;
	denominator /= factor;
}

Fraction Fraction::operator+(const Fraction & other) const
{
	return Fraction(
		numerator * other.denominator + other.numerator * denominator,
		denominator * other.denominator
	);
}

Fraction Fraction::operator-(const Fraction & other) const
{
	return Fraction(
		numerator * other.denominator - other.numerator * denominator,
		denominator * other.denominator
	);
}

Fraction Fraction::operator*(const Fraction & other) const
{
	return Fraction(
		numerator * other.numerator, 
		denominator * other.denominator
	);
}

Fraction Fraction::operator/(const Fraction & other) const
{
	return Fraction(
		numerator * other.denominator, 
		denominator * other.numerator
	);
}