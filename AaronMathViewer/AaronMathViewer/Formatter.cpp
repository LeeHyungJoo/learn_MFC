
#include "pch.h"
#include "Formatter.h"

void Formatter::LineQuation(const CString & desc, const RationalNum & gradient, const RationalNum & constant, OUT CString * format)
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

void Formatter::Coord(const CString & desc, const RationalNum & x, const RationalNum & y, OUT CString * format)
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
