#pragma once

class RationalNum;
class Formatter
{
public:
	static void LineQuation(
		const CString& desc, 
		const RationalNum& gradient, 
		const RationalNum& constant, 
		OUT CString* format,
		BOOL bDecimalFormat = TRUE
	);

	static void LineQuation(
		const CString& desc,
		const LONG dx,
		const LONG dy,
		const LONG x1,
		const LONG y1,
		OUT CString* format,
		BOOL bDecimalFormat = TRUE
	);

	static void VerticalLineQuation(
		const CString& desc,
		const LONG constant,
		OUT CString* format,
		BOOL bDecimalFormat = TRUE
	);

	static void HorizontalLineQuation(
		const CString& desc,
		const LONG constant,
		OUT CString* format,
		BOOL bDecimalFormat = TRUE
	);

	static void Coord(
		const CString& desc,
		const RationalNum& x,
		const RationalNum& y,
		OUT CString* format,
		BOOL bDecimalFormat = TRUE
	);
};