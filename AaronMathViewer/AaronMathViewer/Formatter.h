#pragma once

class Fraction;
class Formatter
{
public:
	static void LineQuation(
		const CString& desc, 
		const Fraction& gradient, 
		const Fraction& constant, 
		OUT CString* format
	);

	static void Coord(
		const CString& desc,
		const Fraction& x,
		const Fraction& y,
		OUT CString* format
	);
};