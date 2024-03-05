
#include "pch.h"
#include "Formatter.h"

void Formatter::LineQuation(
	const CString & desc,
	const RationalNum & gradient, 
	const RationalNum & constant, 
	OUT CString * format, 
	BOOL bDecimalFormat)
{
	if (bDecimalFormat)
	{
		if (gradient.IsInteger())
		{
			if (constant.IsInteger())
				format->Format(_T("%s [ y = %+.0fx %+.0f ]"),
					desc,
					gradient.GetNumerator(),
					constant.GetNumerator()
				);
			else
				format->Format(_T("%s [ y = %+.0fx %+.3f ]"),
					desc,
					gradient.GetNumerator(),
					constant.GetNumerator() / constant.GetDenomiator()
				);
		}
		else
		{
			if (constant.IsInteger())
				format->Format(_T("%s [ y = %+.3fx %+.0f ]"),
					desc,
					gradient.GetNumerator() / gradient.GetDenomiator(),
					constant.GetNumerator()
				);
			else
				format->Format(_T("%s [ y = %+.3fx %+.3f ]"),
					desc,
					gradient.GetNumerator() / gradient.GetDenomiator(),
					constant.GetNumerator() / constant.GetDenomiator()
				);
		}
	}
	else
	{
		if (gradient.IsInteger())
		{
			if (constant.IsInteger())
				format->Format(_T("%s [ y = %+.0fx %+.0f ]"),
					desc,
					gradient.GetNumerator(),
					constant.GetNumerator()
				);
			else
				format->Format(_T("%s [ y = %+.0fx %+.0f/%.0f ]"),
					desc,
					gradient.GetNumerator(),
					constant.GetNumerator(),
					constant.GetDenomiator()
				);
		}
		else
		{
			if (constant.IsInteger())
				format->Format(_T("%s [ y = %+.0f/%.0fx %+.0f ]"),
					desc,
					gradient.GetNumerator(),
					gradient.GetDenomiator(),
					constant.GetNumerator()
				);
			else
				format->Format(_T("%s [ y = %+.0f/%.0fx %+.0f/%.0f ]"),
					desc,
					gradient.GetNumerator(),
					gradient.GetDenomiator(),
					constant.GetNumerator(),
					constant.GetDenomiator()
				);
		}
	}
}

void Formatter::LineQuation(const CString & desc, const LONG dx, const LONG dy, const LONG x1, const LONG y1, OUT CString * format, BOOL bDecimalFormat)
{
	if (dx == 0)
	{
		VerticalLineQuation(desc, x1, format, bDecimalFormat);
	} 
	else if (dy == 0)
	{
		HorizontalLineQuation(desc, y1, format, bDecimalFormat);
	}
	else
	{
		auto m = RationalNum(dy, dx);
		LineQuation(desc, m, m * -x1 + y1, format, bDecimalFormat);
	}
}

void Formatter::VerticalLineQuation(const CString & desc, const LONG constant, OUT CString * format, BOOL bDecimalFormat)
{
	format->Format(_T("%s [ x = %ld ]"),
		desc,
		constant
	);
}

void Formatter::HorizontalLineQuation(const CString & desc, const LONG constant, OUT CString * format, BOOL bDecimalFormat)
{
	format->Format(_T("%s [ y = %ld ]"),
		desc,
		constant
	);
}


void Formatter::Coord(
	const CString & desc, 
	const RationalNum & x, 
	const RationalNum & y, 
	OUT CString * format,
	BOOL bDecimalFormat)
{
	if (bDecimalFormat)
	{
		if (x.IsInteger())
		{
			if (y.IsInteger())
				format->Format(_T("%s ( %+.0f, %+.0f )"),
					desc,
					x.GetNumerator(),
					y.GetNumerator()
				);
			else
				format->Format(_T("%s ( %+.0f, %+.3f )"),
					desc,
					x.GetNumerator(),
					y.GetNumerator() / y.GetDenomiator()
				);
		}
		else
		{
			if (y.IsInteger())
				format->Format(_T("%s ( %+.3f, %+.0f )"),
					desc,
					x.GetNumerator() / x.GetDenomiator(),
					y.GetNumerator()
				);
			else
				format->Format(_T("%s ( %+.3f, %+.3f )"),
					desc,
					x.GetNumerator() / x.GetDenomiator(),
					y.GetNumerator() / y.GetDenomiator()
				);
		}
	}
	else
	{
		if (x.IsInteger())
		{
			if (y.IsInteger())
				format->Format(_T("%s ( %+.0f, %+.0f )"),
					desc,
					x.GetNumerator(),
					y.GetNumerator()
				);
			else
				format->Format(_T("%s ( %+.0f, %+.0f/%.0f )"),
					desc,
					x.GetNumerator(),
					y.GetNumerator(),
					y.GetDenomiator()
				);
		}
		else
		{
			if (y.IsInteger())
				format->Format(_T("%s ( %+.0f/%.0f, %+.0f )"),
					desc,
					x.GetNumerator(),
					x.GetDenomiator(),
					y.GetNumerator()
				);
			else
				format->Format(_T("%s ( %+.0f/%.0f, %+.0f/%.0f )"),
					desc,
					x.GetNumerator(),
					x.GetDenomiator(),
					y.GetNumerator(),
					y.GetDenomiator()
				);
		}
	}
}

void Formatter::Coord(const CString & desc, const CPoint & point, OUT CString * format, BOOL bDecimalFormat)
{
	Formatter::Coord(desc, point.x, point.y, format, bDecimalFormat);
}
