#pragma once

class RationalNum;
class Formatter
{
public:
	static void LineQuation(
		const CString& desc, 
		const RationalNum& gradient, 
		const RationalNum& constant, 
		OUT CString* format
	);

	static void Coord(
		const CString& desc,
		const RationalNum& x,
		const RationalNum& y,
		OUT CString* format
	);
};