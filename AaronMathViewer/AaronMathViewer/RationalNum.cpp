#include "pch.h"
#include "RationalNum.h"

RationalNum::RationalNum(DOUBLE num)
	: numerator(num), denominator(1)
{
}

RationalNum::RationalNum(DOUBLE num, DOUBLE denom)
	: numerator(num), denominator(denom)
{
	if (denominator < 0) 
	{
		numerator = -numerator;
		denominator = -denominator;
	}

	LONG64 factor = gcd(static_cast<LONG64>(abs(numerator)), static_cast<LONG64>(denominator));
	numerator /= factor;
	denominator /= factor;
}

RationalNum::RationalNum(const RationalNum & numFrac, const RationalNum & denomFrac)
{
	auto frac = RationalNum(
		numFrac.GetNumerator() * denomFrac.GetDenomiator(),
		numFrac.GetDenomiator() * denomFrac.GetNumerator()
	);

	numerator = frac.numerator;
	denominator = frac.denominator;
}

BOOL RationalNum::IsInteger() const
{
	return denominator == 1 && std::abs(numerator - (LONG)numerator) < DBL_EPSILON;
}

DOUBLE RationalNum::GetNumerator() const
{
	return numerator;
}

DOUBLE RationalNum::GetDenomiator() const
{
	return denominator;
}

DOUBLE RationalNum::GetValue() const
{
	return numerator / denominator;
}

RationalNum RationalNum::operator+(const RationalNum & other) const
{
	return RationalNum(
		numerator * other.denominator + other.numerator * denominator,
		denominator * other.denominator
	);
}

RationalNum RationalNum::operator-(const RationalNum & other) const
{
	return RationalNum(
		numerator * other.denominator - other.numerator * denominator,
		denominator * other.denominator
	);
}

RationalNum RationalNum::operator*(const RationalNum & other) const
{
	return RationalNum(
		numerator * other.numerator, 
		denominator * other.denominator
	);
}

RationalNum RationalNum::operator/(const RationalNum & other) const
{
	return RationalNum(
		numerator * other.denominator, 
		denominator * other.numerator
	);
}

BOOL RationalNum::operator==(const RationalNum & other) const
{
	return (abs(numerator - other.numerator) < DBL_EPSILON) && (abs(denominator - other.denominator) < DBL_EPSILON);
}
