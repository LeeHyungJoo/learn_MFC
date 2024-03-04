
#include "pch.h"
#include "Formatter.h"

static bool DECIMAL_FORMAT = TRUE;

void Formatter::LineQuation(const CString & desc, const RationalNum & gradient, const RationalNum & constant, OUT CString * format)
{
	if (DECIMAL_FORMAT)
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

void Formatter::Coord(const CString & desc, const RationalNum & x, const RationalNum & y, OUT CString * format)
{
	if (DECIMAL_FORMAT)
	{
		if (x.IsInteger())
		{
			if (y.IsInteger())
				format->Format(_T("%s ( %+.0f, %.0f )"),
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
				format->Format(_T("%s ( %+.3f, %.0f )"),
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
				format->Format(_T("%s ( %+.0f, %.0f )"),
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
				format->Format(_T("%s ( %+.0f/%.0f, %.0f )"),
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
