
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

void Formatter::CircleQuation(const CString & desc, const RationalNum & a, const RationalNum & b, const RationalNum & r, OUT CString * format, BOOL bDecimalFormat)
{
	if (bDecimalFormat)
	{
		CString formatA;
		formatA.Format(a.IsInteger() ? L"%+.0f" : L"%+.3f", -a.GetValue());
		
		CString formatB;
		formatB.Format(b.IsInteger() ? L"%+.0f" : L"%+.3f", -b.GetValue());

		CString formatR;
		formatR.Format(r.IsInteger() ? L"%.0f" : L"%.3f", r.GetValue());


		format->Format(_T("%s [ (x%s)\u00b2+(y%s)\u00b2 = %s\u00b2 ]"),
			desc,
			formatA,
			formatB,
			formatR
		);
	}
	else
	{
		//NOT IMPLEMENT
	}
}

void Formatter::ParabolaQuationX(const CString & desc, const RationalNum & a, const RationalNum & b, const RationalNum & c, OUT CString * format, BOOL bDecimalFormat)
{
	if (bDecimalFormat)
	{
		RationalNum s = a;
		RationalNum x = b  / (a * 2);
		RationalNum y = -(std::pow(b.GetValue(), 2) / (a * 4).GetValue()) + c.GetValue();

		CString formatA;
		formatA.Format(s.IsInteger() ? L"%+.0f" : L"%+.3f", s.GetValue());

		CString formatB;
		formatB.Format(x.IsInteger() ? L"%+.0f" : L"%+.3f", x.GetValue());

		CString formatC;
		formatC.Format(y.IsInteger() ? L"%+.0f" : L"%+.3f", y.GetValue());

		format->Format(_T("%s [ y = %s(x%s)\u00b2%s ]"),
			desc,
			formatA,
			formatB,
			formatC
		);
	}
	else
	{
		//NOT IMPLEMENT
	}
}

void Formatter::ParabolaQuationY(const CString & desc, const RationalNum & a, const RationalNum & b, const RationalNum & c, OUT CString * format, BOOL bDecimalFormat)
{
	if (bDecimalFormat)
	{
		RationalNum s = a;
		RationalNum x = b / (a * 2);
		RationalNum y = -(std::pow(b.GetValue(), 2) / (a * 4).GetValue()) + c.GetValue();

		CString formatA;
		formatA.Format(s.IsInteger() ? L"%+.0f" : L"%+.3f", s.GetValue());

		CString formatB;
		formatB.Format(x.IsInteger() ? L"%+.0f" : L"%+.3f", x.GetValue());

		CString formatC;
		formatC.Format(y.IsInteger() ? L"%+.0f" : L"%+.3f", y.GetValue());

		format->Format(_T("%s [ x = %s(y%s)\u00b2%s ]"),
			desc,
			formatA,
			formatB,
			formatC
		);
	}
	else
	{
		//NOT IMPLEMENT
	}
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

void Formatter::Coord(const CString & desc, const DOUBLE & pointx, const DOUBLE & pointy, OUT CString * format, BOOL bDecimalFormat)
{
	format->Format(_T("%s ( %+.3f, %+.3f )"),
		desc,
		pointx,
		pointy
	);
}

void Formatter::Coord(const CString & desc, const INT & label, const DOUBLE & pointx, const DOUBLE & pointy, OUT CString * format, BOOL bDecimalFormat)
{
	format->Format(_T("%s %d ( %+.3f, %+.3f )"),
		desc,
		label,
		pointx,
		pointy
	);
}
