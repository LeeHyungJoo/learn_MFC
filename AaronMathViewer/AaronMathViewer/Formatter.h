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

	static void CircleQuation(
		const CString& desc,
		const RationalNum& a,
		const RationalNum& b,
		const RationalNum& r,
		OUT CString* format,
		BOOL bDecimalFormat = TRUE
	);

	static void ParabolaQuationX(
		const CString& desc,
		const RationalNum& a,
		const RationalNum& b,
		const RationalNum& c,
		OUT CString* format,
		BOOL bDecimalFormat = TRUE
	);

	static void ParabolaQuationY(
		const CString& desc,
		const RationalNum& a,
		const RationalNum& b,
		const RationalNum& c,
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

	static void Coord(
		const CString& desc,
		const CPoint& point,
		OUT CString* format,
		BOOL bDecimalFormat = TRUE
	);

	static void Coord(
		const CString& desc,
		const DOUBLE& pointx,
		const DOUBLE& pointy,
		OUT CString* format,
		BOOL bDecimalFormat = TRUE
	);

	static void Coord(
		const CString& desc,
		const INT& label,
		const DOUBLE& pointx,
		const DOUBLE& pointy,
		OUT CString* format,
		BOOL bDecimalFormat = TRUE
	);
};