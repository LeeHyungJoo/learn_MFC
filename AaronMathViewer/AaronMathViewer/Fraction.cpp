#include "pch.h"
#include "Fraction.h"

Fraction::Fraction(DOUBLE num)
	: numerator(num), denominator(1)
{
}

Fraction::Fraction(DOUBLE num, DOUBLE denom)
	: numerator(static_cast<LONG>(num)), denominator(static_cast<LONG>(denom))
{
	if (denominator < 0) 
	{
		numerator = -numerator;
		denominator = -denominator;
	}

	LONG factor = gcd(static_cast<LONG>(abs(numerator)), static_cast<LONG>(denominator));
	numerator /= factor;
	denominator /= factor;
}

Fraction::Fraction(const Fraction & numFrac, const Fraction & denomFrac)
{
	auto frac = Fraction(
		numFrac.GetNumerator() * denomFrac.GetDenomiator(),
		numFrac.GetDenomiator() * denomFrac.GetNumerator()
	);

	numerator = frac.numerator;
	denominator = frac.denominator;
}

BOOL Fraction::IsInteger() const
{
	return denominator == 1;
}

LONG Fraction::GetNumerator() const
{
	return static_cast<LONG>(numerator);
}

LONG Fraction::GetDenomiator() const
{
	return static_cast<LONG>(denominator);
}

DOUBLE Fraction::GetValue() const
{
	return numerator / denominator;
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