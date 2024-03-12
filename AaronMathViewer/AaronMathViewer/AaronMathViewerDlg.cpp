
#include "pch.h"
#include "framework.h"
#include "AaronMathViewer.h"
#include "AaronMathViewerDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const static BOOL B_TEST = FALSE;

CAaronMathViewerDlg::CAaronMathViewerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AARONMATHVIEWER_DIALOG, pParent)
	, m_uMethodID(0U)
{
	m_mPickedCoordCount[IDC_RADIO_PPC] = 3U;
	m_mPickedCoordCount[IDC_RADIO_TRIROT] = 3U;
	m_mPickedCoordCount[IDC_RADIO_LSMLINE] = 500U;
	m_mPickedCoordCount[IDC_RADIO_LSMCIRCLE] = 500U;
	m_mPickedCoordCount[IDC_RADIO_LSMPARABO] = 500U;
}

CAaronMathViewerDlg::~CAaronMathViewerDlg()
{
	m_vecCoordEdits.RemoveAll();
	m_points.RemoveAll();
}

void CAaronMathViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICCONTROL, m_pcBoard);
	DDX_Control(pDX, IDC_STATIC_PNTCOORD, m_stCoord);
	DDX_Control(pDX, IDC_LIST_VIEW, m_lbxExpr);
	DDX_Control(pDX, IDC_EDIT_DEGREE, m_edtDegree);
	DDX_Control(pDX, IDC_BUTTON_ROT, m_btnRot);
	DDX_Control(pDX, IDC_EDIT_TESTCOORD, m_edtTestCoord);
}

void CAaronMathViewerDlg::ResetPicking()
{
	m_vecPickedCoord.RemoveAll();
	m_lbxExpr.ResetContent();
	m_bFirstEnter = true;

	INT cnt = 0;
	for (INT i = 0; i < m_vecCoordEdits.GetSize(); i++)
	{
		static_cast<CEdit*>(m_vecCoordEdits[i])->SetWindowText(L"");
		m_mPickedCoordCount.Lookup(m_uMethodID, cnt);
		static_cast<CEdit*>(m_vecCoordEdits[i])->EnableWindow(cnt > i);
	}
}

void CAaronMathViewerDlg::ResetParamCoords()
{
	m_vecDoubleCoord.RemoveAll();
	m_vecParamCoord.RemoveAll();
	m_points.RemoveAll();
	m_vecParamArray.RemoveAll();
	m_iRotDegree = 0;
}

BOOL CAaronMathViewerDlg::PickCoord(const CPoint& clientPnt)
{
	if (m_uMethodID == 0)
	{
		AfxMessageBox(_T("먼저 함수 모드를 선택하십시오. !"), MB_ICONWARNING | MB_OK);
		return FALSE;
	}
	INT cnt = 0;
	m_mPickedCoordCount.Lookup(m_uMethodID, cnt);
	if (m_vecPickedCoord.GetSize() >= cnt)
	{
		AfxMessageBox(_T("이미 필요한 좌표를 모두 입력하셨습니다. !"), MB_ICONWARNING | MB_OK);
		return FALSE;
	}

	auto othogonalPnt = ToOthogonalFromClient(clientPnt);
	m_vecPickedCoord.Add(othogonalPnt);
	m_vecDoubleCoord.Add(CPointDouble(othogonalPnt));

	return TRUE;
}

BOOL CAaronMathViewerDlg::IsPointInBoard(const CPoint & clientPnt) const
{
	CRect cRect;
	m_pcBoard.GetClientRect(&cRect);

	BOOL bIn = true;
	bIn &= clientPnt.y <= cRect.bottom;
	bIn &= clientPnt.y >= cRect.top;
	bIn &= clientPnt.x <= cRect.right;
	bIn &= clientPnt.x >= cRect.left;
	return bIn;
}

void CAaronMathViewerDlg::UpdatePickCoords()
{
	for (INT i = 0; i < m_vecPickedCoord.GetSize(); i++)
	{
		CString show;
		show.Format(_T("(%d, %d)"), m_vecPickedCoord[i].x, m_vecPickedCoord[i].y);
		static_cast<CEdit*>(m_vecCoordEdits[i % 6])->SetWindowText(show);
	}

	switch (m_uMethodID)
	{
	case IDC_RADIO_PPC:
	{
		if (m_vecPickedCoord.GetSize() == 2)
		{
			CString show;
			Formatter::LineQuation(
				L"직선 방정식", 
				m_vecPickedCoord[1].x - m_vecPickedCoord[0].x,
				m_vecPickedCoord[1].y - m_vecPickedCoord[0].y, 
				m_vecPickedCoord[0].x,
				m_vecPickedCoord[0].y,
				&show);

			m_lbxExpr.AddString(show);
		}
		else if (m_vecPickedCoord.GetSize() == 3)
		{
			auto dy = m_vecPickedCoord[1].y - m_vecPickedCoord[0].y;
			auto dx = m_vecPickedCoord[1].x - m_vecPickedCoord[0].x;

			if (dx == 0)
			{
				CString show;
				Formatter::HorizontalLineQuation(L"수선 방정식", m_vecPickedCoord[2].y, &show);
				m_lbxExpr.AddString(show);

				Formatter::Coord(L"교점", m_vecPickedCoord[0].x, m_vecPickedCoord[2].y, &show);
				m_lbxExpr.AddString(show);

				m_vecParamCoord.Add(CPoint(m_vecPickedCoord[0].x, m_vecPickedCoord[2].y));
			}
			else if (dy == 0)
			{
				CString show;
				Formatter::VerticalLineQuation(L"수선 방정식", m_vecPickedCoord[2].x, &show);
				m_lbxExpr.AddString(show);

				Formatter::Coord(L"교점", m_vecPickedCoord[2].x, m_vecPickedCoord[0].y, &show);
				m_lbxExpr.AddString(show);

				m_vecParamCoord.Add(CPoint(m_vecPickedCoord[2].x, m_vecPickedCoord[0].y));
			}
			else
			{
				auto m = RationalNum(m_vecPickedCoord[1].y - m_vecPickedCoord[0].y, m_vecPickedCoord[1].x - m_vecPickedCoord[0].x);
				auto c = RationalNum(-m_vecPickedCoord[0].x) * m + m_vecPickedCoord[0].y;

				if ((m * m_vecPickedCoord[2].x + c) == m_vecPickedCoord[2].y)
				{
					m_vecPickedCoord.RemoveAt(m_vecPickedCoord.GetUpperBound());
					AfxMessageBox(_T("직선 위에 있는 점에서는 수선을 내릴 수 없습니다 !\r\n다시 시도해주세요 ! "), MB_ICONWARNING | MB_OK);
					break;
				}

				auto im = RationalNum(-m.GetDenomiator(), m.GetNumerator());
				auto ic = RationalNum(-m_vecPickedCoord[2].x) * im + m_vecPickedCoord[2].y;

				CString show;
				Formatter::LineQuation(L"수선 방정식", im, ic, &show);
				m_lbxExpr.AddString(show);

				auto inter_x = RationalNum(ic - c, m - im);
				auto inter_y = m * inter_x + c;

				Formatter::Coord(L"교점", inter_x, inter_y, &show);
				m_lbxExpr.AddString(show);

				POINT tar;
				tar.x = static_cast<LONG>(inter_x.GetValue());
				tar.y = static_cast<LONG>(inter_y.GetValue());
				m_vecParamCoord.Add(CPoint(tar));
			}
		}
		break;
	}
	case IDC_RADIO_TRIROT:
	{
		if (m_vecDoubleCoord.GetSize() == 3)
		{
			if (m_bFirstEnter)
			{
				m_bFirstEnter = false;
				m_iRotDegree = 0;

				INT64 count = m_vecDoubleCoord.GetSize() + 1;
				POINT* pntArr = new POINT[count];

				for (int i = 0; i < m_vecDoubleCoord.GetSize(); i++)
					pntArr[i] = ToClientFromOthogonal(CPoint((LONG)m_vecDoubleCoord[i].x, (LONG)m_vecDoubleCoord[i].y));

				pntArr[count - 1] = pntArr[0];
				m_points.Add(std::make_pair(pntArr, count));
			}

			CString show;
			auto radian = (DOUBLE)m_iRotDegree * (M_PI / 180.0);

			for (int i = 0; i < m_vecDoubleCoord.GetSize(); i++)
			{
				auto x = m_vecDoubleCoord[i].x;
				auto y = m_vecDoubleCoord[i].y;

				DOUBLE dcx = x * std::cos(radian) - y * std::sin(radian);
				DOUBLE dcy = x * std::sin(radian) + y * std::cos(radian);

				
				Formatter::Coord(L"", i ,dcx, dcy, &show);
				m_lbxExpr.AddString(show);
			}

			m_lbxExpr.SetCurSel(m_lbxExpr.GetCount() - 1);
		}
		break;
	}
	case IDC_RADIO_LSMLINE:
	{
		if (m_vecDoubleCoord.GetSize() > 2)
		{
			INT64 n = m_vecDoubleCoord.GetSize();
			DOUBLE A11 = 0, A12 = 0, A21 = 0, A22 = 0;
			DOUBLE B11 = 0, B21 = 0;

			for (INT64 i = 0; i < n; i++)
			{
				A11 += m_vecDoubleCoord[i].x * m_vecDoubleCoord[i].x;
				A12 += m_vecDoubleCoord[i].x;
				A21 += m_vecDoubleCoord[i].x;
				A22 += 1;

				B11 += m_vecDoubleCoord[i].x * m_vecDoubleCoord[i].y;
				B21 += m_vecDoubleCoord[i].y;
			}

			DOUBLE detA = 1.0 / (A11 * A22 - A12 * A21);
			DOUBLE inv_A11 = 0, inv_A12 = 0, inv_A21 = 0, inv_A22 = 0;

			inv_A11 = detA * +A22;
			inv_A12 = detA * -A21;
			inv_A21 = detA * -A12;
			inv_A22 = detA * +A11;

			DOUBLE m = inv_A11 * B11 + inv_A12 * B21;
			DOUBLE c = inv_A21 * B11 + inv_A22 * B21;

			CString show;
			Formatter::LineQuation(
				L"최소 자승식",
				m,
				c,
				&show);

			m_lbxExpr.AddString(show);

			CRect cRect;
			m_pcBoard.GetClientRect(&cRect);

			m_vecParamCoord.RemoveAll();
			
			//y = mx + c => x = (y - c) / m
			CPoint top = CPoint((LONG)((cRect.bottom / 2 - c) / m), cRect.bottom / 2);
			CPoint bottom = CPoint((LONG)((-cRect.bottom / 2 - c) / m), -cRect.bottom / 2);
			m_vecParamCoord.Add(top);
			m_vecParamCoord.Add(bottom);
		}

		break;
	}
	case IDC_RADIO_LSMCIRCLE:
	{
		if (m_vecDoubleCoord.GetSize() > 2)
		{
			INT64 n = m_vecDoubleCoord.GetSize();

			/*
				arg1 : x^2 
				arg2 : y
				arg3 : x
				arg4 : xy
				arg5 : y^2

				h = ((arg1 * arg2) - (arg3 * arg4)) / ((n * arg5) - (arg2 * arg2));
				k = ((arg5 * arg3) - (arg2 * arg4)) / ((n* arg5) - (arg2 * arg2));
				r = sqrt((1 / n) * (arg1 + arg5 - 2 * h * arg3 - 2 * k * arg2 + n * k * k + n * k * k));
			*/

			DOUBLE arg1 = 0, arg2 = 0, arg3 = 0, arg4 = 0, arg5 = 0 , arg6 = 0;

			for (INT64 i = 0; i < n; i++)
			{
				arg1 += (m_vecDoubleCoord[i].x * m_vecDoubleCoord[i].x);
				arg2 += m_vecDoubleCoord[i].y;
				arg3 += m_vecDoubleCoord[i].x;
				arg4 += (m_vecDoubleCoord[i].x * m_vecDoubleCoord[i].y);
				arg5 += (m_vecDoubleCoord[i].y * m_vecDoubleCoord[i].y);
			}

			DOUBLE h = 0, k = 0, r = 0;

			h = (arg3 / n);
			k = (arg2 / n);


			for (INT64 i = 0; i < n; i++)
			{
				arg6 += (m_vecDoubleCoord[i].x - h)* (m_vecDoubleCoord[i].x - h) /n + (m_vecDoubleCoord[i].y - k) * (m_vecDoubleCoord[i].y - k) /n;
			}

			r = sqrt(arg6);

			CString show;
			Formatter::CircleQuation(
				L"최소 자승식",
				h,
				k,
				r,
				&show
			);
			m_lbxExpr.AddString(show);

			m_vecParamCoord.RemoveAll();

			CPoint top = CPoint((LONG)(h - r), (LONG)(k - r) );
			CPoint bottom = CPoint((LONG)(h + r), (LONG)(k + r) );
			m_vecParamCoord.Add(top);
			m_vecParamCoord.Add(bottom);
		}

		break;
	}
	case IDC_RADIO_LSMPARABO :
	{
		if (m_vecDoubleCoord.GetSize() > 2)
		{
			INT64 n = m_vecDoubleCoord.GetSize();
			/*
				arg1 = x
				arg2 = x^2
				arg3 = x^3
				arg4 = y
				arg5 = xy
				arg6 = x^2 y
				arg7 = x^4

			*/

			DOUBLE arg1 = 0, arg2 = 0, arg3 = 0, arg4 = 0, arg5 = 0, arg6 = 0, arg7 = 0;

			for (INT64 i = 0; i < n; i++)
			{
				arg1 += m_vecDoubleCoord[i].x;
				arg2 += std::pow(m_vecDoubleCoord[i].x, 2);
				arg3 += std::pow(m_vecDoubleCoord[i].x, 3);
				arg4 += m_vecDoubleCoord[i].y;
				arg5 += m_vecDoubleCoord[i].x * m_vecDoubleCoord[i].y;
				arg6 += std::pow(m_vecDoubleCoord[i].x, 2) * m_vecDoubleCoord[i].y;
				arg7 += std::pow(m_vecDoubleCoord[i].x, 4);
			}

			DOUBLE ret1 = 0;
			ret1 = arg7 * (n * arg2 - std::pow(arg1, 2)) - arg3 * (n * arg3 - arg2 * arg1) + arg2 * (arg3 * arg1 - pow(arg2, 2));

			DOUBLE a = 0, b = 0, c = 0;

			a = (arg6 * (n * arg2 - std::pow(arg1, 2)) - arg3 * (n * arg5 - arg1 * arg4) + arg2 * (arg1 *arg5 - arg2 * arg4)) / ret1;
			b = (arg7 *(n * arg5 - arg1 * arg4) - arg6 * (n * arg3 - arg2 * arg1) + arg2 * (arg3 * arg4 - arg2 * arg5)) / ret1;
			c = (arg7 * (arg2 * arg4 - arg5 * arg1) - arg3 * (arg3 * arg4 - arg5 * arg2) + arg6 * (arg3 * arg1 - std::pow(arg2, 2))) / ret1;

			CString show;
			Formatter::ParabolaQuation(
				L"최소 자승식",
				a,
				b,
				c,
				&show
			);

			m_vecParamArray.RemoveAll();
			m_vecParamArray.Add(a);
			m_vecParamArray.Add(b);
			m_vecParamArray.Add(c);

			m_lbxExpr.AddString(show);
		}
		break;
	}
	}

	m_lbxExpr.SetCurSel(m_lbxExpr.GetCount() - 1);
	Invalidate();
}

BEGIN_MESSAGE_MAP(CAaronMathViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND_RANGE(IDC_RADIO_PPC, IDC_RADIO_LSMPARABO, OnMethodRadioChanged)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CAaronMathViewerDlg::OnBnClickedButtonReset)
	//ON_LBN_DBLCLK(IDC_LIST_VIEW, &CAaronMathViewerDlg::OnLbnDblclkListView)
	ON_BN_CLICKED(IDC_BUTTON_ROT, &CAaronMathViewerDlg::OnBnClickedButtonRot)
	ON_BN_CLICKED(IDC_BUTTON_TESTPICK, &CAaronMathViewerDlg::OnBnClickedButtonTestpick)
END_MESSAGE_MAP()

BOOL CAaronMathViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (auto editID = IDC_EDIT_PICK1; editID <= IDC_EDIT_PICK6; editID++)
		m_vecCoordEdits.Add((CEdit*)(GetDlgItem(editID)));

	return TRUE;
}

void CAaronMathViewerDlg::OnPaint()
{
	CDialogEx::OnPaint();

	CPaintDC dc(&m_pcBoard);
	CRect cRect;
	m_pcBoard.GetClientRect(&cRect);
	dc.IntersectClipRect(&cRect);

	ResetBoard(&dc);
	DrawMethod(&dc);
	DrawOthogonal(&dc);
}

void CAaronMathViewerDlg::OnMethodRadioChanged(UINT ID)
{
	if (m_uMethodID == ID)
		return;

	switch (m_uMethodID)
	{
	case IDC_RADIO_TRIROT:
		m_edtDegree.EnableWindow(FALSE);
		m_btnRot.EnableWindow(FALSE);
		break;
	}

	m_uMethodID = ID;

	ResetPicking();
	ResetParamCoords();

	switch (m_uMethodID)
	{
	case IDC_RADIO_TRIROT:
		m_edtDegree.EnableWindow(TRUE);
		m_btnRot.EnableWindow(TRUE);
		break;
	}

	Invalidate();
}

const CPoint CAaronMathViewerDlg::ToOthogonalFromClient(const CPoint & client)
{
	CPoint pt = client;

	switch (m_uMethodID)
	{
	case IDC_RADIO_TRIROT:
	case IDC_RADIO_LSMLINE:
	case IDC_RADIO_LSMCIRCLE:
	case IDC_RADIO_LSMPARABO:
		CRect cRect;
		m_pcBoard.GetClientRect(&cRect);
		pt.x -= cRect.right / 2;
		pt.y -= cRect.bottom / 2;
		pt.y *= -1;
		break;
	}

	return pt;
}

const CPoint CAaronMathViewerDlg::ToClientFromOthogonal(const CPoint & othogonal)
{
	CPoint pt = othogonal;

	switch (m_uMethodID)
	{
	case IDC_RADIO_TRIROT:
	case IDC_RADIO_LSMLINE:
	case IDC_RADIO_LSMCIRCLE:
	case IDC_RADIO_LSMPARABO:
		CRect cRect;
		m_pcBoard.GetClientRect(&cRect);
		pt.y *= -1;
		pt.x += cRect.right / 2;
		pt.y += cRect.bottom / 2;
		break;
	}

	return pt;
}

void CAaronMathViewerDlg::ResetBoard(CPaintDC* dc)
{
	switch (m_uMethodID)
	{
	case IDC_RADIO_PPC:
	case IDC_RADIO_TRIROT:
	case IDC_RADIO_LSMLINE:
	case IDC_RADIO_LSMCIRCLE:
	case IDC_RADIO_LSMPARABO:
	{
		CRect cRect;
		m_pcBoard.GetClientRect(&cRect);
		dc->FillSolidRect(cRect, RGB(255, 255, 255));
		break;
	}
	default:
		return;
	}
}

void CAaronMathViewerDlg::DrawMethod(CPaintDC* dc)
{
	CArray<CPoint> oPickedCrd;
	oPickedCrd.SetSize(m_vecPickedCoord.GetSize());
	for (int i = 0 ; i < oPickedCrd.GetSize(); i++)
		oPickedCrd[i] = ToClientFromOthogonal(m_vecPickedCoord[i]);

	CArray<CPoint> oParamCrd;
	oParamCrd.SetSize(m_vecParamCoord.GetSize());
	for (int i = 0; i < oParamCrd.GetSize(); i++)
		oParamCrd[i] = ToClientFromOthogonal(m_vecParamCoord[i]);

	switch (m_uMethodID)
	{
	case IDC_RADIO_PPC:
	{
		if (oPickedCrd.GetSize() > 1)
			DrawLine(dc, oPickedCrd[0], oPickedCrd[1]);

		if (oPickedCrd.GetSize() > 2 && oParamCrd.GetSize() > 0)
			DrawDotLine(dc, oPickedCrd[2], oParamCrd[0]);

		if (oPickedCrd.GetSize() > 0)
			DrawDotCircle(dc, oPickedCrd[0]);

		if (oPickedCrd.GetSize() > 1)
			DrawDotCircle(dc, oPickedCrd[1]);

		if (oPickedCrd.GetSize() > 2)
			DrawDotCircle(dc, oPickedCrd[2]);

		if (oParamCrd.GetSize() > 0)
			DrawSpecificDotCircle(dc, oParamCrd[0]);

		break;
	}
	case IDC_RADIO_TRIROT:
	{
		for (int i = 0; i < oPickedCrd.GetSize(); i++)
			DrawDotCircle(dc, oPickedCrd[i]);

		if (oPickedCrd.GetSize() == 3)
		{
			DrawPolyLines(dc, m_points, 0, m_points.GetUpperBound());
			DrawPolyLine(dc, oPickedCrd, 0, 2);
		}

		break;
	}
	case IDC_RADIO_LSMLINE:
	{
		for (int i = 0; i < oPickedCrd.GetSize(); i++)
			DrawDotCircle(dc, oPickedCrd[i]);

		if (oParamCrd.GetSize() > 1)
			DrawLine(dc, oParamCrd[0], oParamCrd[1]);

		break;
	}
	case IDC_RADIO_LSMCIRCLE:
	{
		if (oParamCrd.GetSize() > 1)
			DrawCircle(dc, oParamCrd[0], oParamCrd[1]);

		for (int i = 0; i < oPickedCrd.GetSize(); i++)
			DrawDotCircle(dc, oPickedCrd[i]);

		break;
	}
	case IDC_RADIO_LSMPARABO:
	{
		if (m_vecParamArray.GetSize() > 2)
			DrawParabola(dc, m_vecParamArray[0], m_vecParamArray[1], m_vecParamArray[2]);

		for (int i = 0; i < oPickedCrd.GetSize(); i++)
			DrawDotCircle(dc, oPickedCrd[i]);
		break;
	}
	}
}

void CAaronMathViewerDlg::DrawLine(CPaintDC* dc, const CPoint & start, const CPoint & end)
{
	CPen pen(PS_SOLID, 2, RGB(0, 255, 120));
	CPen* pOldPen = dc->SelectObject(&pen);

	dc->MoveTo(start);
	dc->LineTo(end);

	dc->SelectObject(*pOldPen);
}

void CAaronMathViewerDlg::DrawDotLine(CPaintDC* dc, const CPoint & start, const CPoint & end)
{
	CPen dotPen(PS_DOT, 1, RGB(255, 120, 0));
	CPen* pOldPen = dc->SelectObject(&dotPen);

	dc->MoveTo(start);
	dc->LineTo(end);

	dc->SelectObject(*pOldPen);
}

void CAaronMathViewerDlg::DrawDotCircle(CPaintDC* dc, const CPoint & point)
{
	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen* pOldPen = dc->SelectObject(&pen);

	dc->Ellipse(point.x - 2, point.y - 2, point.x + 2, point.y + 2);
	dc->SelectObject(*pOldPen);
}

void CAaronMathViewerDlg::DrawCircle(CPaintDC * dc, const CPoint & LT, const CPoint & RB)
{
	CPen pen(PS_SOLID, 2, RGB(0, 255, 120));
	CPen* pOldPen = dc->SelectObject(&pen);

	dc->Ellipse(LT.x,LT.y, RB.x, RB.y);
	dc->SelectObject(*pOldPen);
}

void CAaronMathViewerDlg::DrawSpecificDotCircle(CPaintDC* dc, const CPoint & point)
{
	CPen interPen(PS_SOLID, 3, RGB(0, 50, 255));
	CPen* pOldPen = dc->SelectObject(&interPen);

	dc->SelectObject(&interPen);
	dc->Ellipse(point.x - 2, point.y - 2, point.x + 2, point.y + 2);

	dc->SelectObject(*pOldPen);
}

void CAaronMathViewerDlg::DrawPolyLine(CPaintDC* dc, const CArray<CPoint>& points, INT64 startIdx, INT64 endIdx)
{
	if (endIdx - startIdx < 2)
		return;

	CPen dotPen(PS_SOLID, 1, RGB(0, 255, 120));
	CPen* pOldPen = dc->SelectObject(&dotPen);

	INT64 count = endIdx - startIdx + 2;
	POINT* pntArr = new POINT[count];
	for (INT64 i = startIdx; i <= endIdx; i++)
		pntArr[i] = points[i];
	pntArr[count - 1] = points[startIdx];

	dc->Polyline(pntArr, (INT)count);
	dc->SelectObject(*pOldPen);

	delete pntArr;
}

void CAaronMathViewerDlg::DrawPolyLines(CPaintDC* dc, const CArray<std::pair<POINT*, INT64>>& points, INT64 startIdx, INT64 endIdx)
{
	CPen pen(PS_DOT, 1, RGB(200, 200, 200));

	CPen* pOldPen = dc->SelectObject(&pen);

	for (INT64 i = startIdx; i <= endIdx; i++)
		dc->Polyline(points[i].first, (INT)points[i].second);

	dc->SelectObject(*pOldPen);
}

void CAaronMathViewerDlg::DrawParabola(CPaintDC * dc, DOUBLE a, DOUBLE b, DOUBLE c)
{
	CRect cRect;
	m_pcBoard.GetClientRect(&cRect);

	CPen pen(PS_SOLID, 2, RGB(0, 255, 120));
	CPen* pOldPen = dc->SelectObject(&pen);

	INT64 pntIdx = 0;
	POINT* pntArr = new POINT[cRect.right / 2];

	for (INT64 x = -cRect.right / 2; x < cRect.right / 2; x += 2)
	{
		pntArr[pntIdx++] = ToClientFromOthogonal(CPoint((LONG)x, (LONG)(x*x *a + b * x + c)));
	}

	dc->Polyline(pntArr, (INT)cRect.right / 2);
	dc->SelectObject(*pOldPen);

	delete pntArr;
}

void CAaronMathViewerDlg::DrawOthogonal(CPaintDC* dc)
{
	switch (m_uMethodID)
	{
	case IDC_RADIO_TRIROT:
	case IDC_RADIO_LSMLINE:
	case IDC_RADIO_LSMCIRCLE:
	case IDC_RADIO_LSMPARABO:
	{
		CPen dotPen(PS_DOT, 1, RGB(0, 0, 0));
		CPen* pOldPen = dc->SelectObject(&dotPen);

		CRect cRect;
		m_pcBoard.GetClientRect(&cRect);

		dc->MoveTo(cRect.right / 2, cRect.top);
		dc->LineTo(cRect.right / 2, cRect.bottom);

		dc->MoveTo(cRect.left, cRect.right / 2);
		dc->LineTo(cRect.right, cRect.right / 2);

		dc->SelectObject(*pOldPen);
		break;
	}
	default:
		break;
	}
}

void CAaronMathViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint screenPoint = point;
	ClientToScreen(&screenPoint);

	CPoint clientPnt = screenPoint;
	m_pcBoard.ScreenToClient(&clientPnt);

	if (IsPointInBoard(clientPnt))
	{
		auto othogonalPnt = ToOthogonalFromClient(clientPnt);

		CString show;
		show.Format(_T("(%d, %d)"), othogonalPnt.x, othogonalPnt.y);
		m_stCoord.SetWindowText(show);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CAaronMathViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint screenPoint = point;
	ClientToScreen(&screenPoint);
	
	CPoint clientPnt = screenPoint;
	m_pcBoard.ScreenToClient(&clientPnt);

	if (IsPointInBoard(clientPnt))
	{
		if (PickCoord(clientPnt))
		{
			UpdatePickCoords();
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CAaronMathViewerDlg::OnBnClickedButtonRot()
{
	if (m_uMethodID != IDC_RADIO_TRIROT)
		return;

	CString degreeStr;
	m_edtDegree.GetWindowText(degreeStr);
	auto degree = _wtoi(degreeStr);
	m_iRotDegree += degree;
	m_iRotDegree %= 360;

	auto radian = (DOUBLE)m_iRotDegree * (M_PI / 180.0);
	INT64 count = m_vecDoubleCoord.GetSize() + 1;
	POINT* pntArr = new POINT[count];

	for (int i = 0; i < m_vecDoubleCoord.GetSize(); i++)
	{
		auto x = m_vecDoubleCoord[i].x;
		auto y = m_vecDoubleCoord[i].y;

		double dcx = x * std::cos(radian) - y * std::sin(radian);
		double dcy = x * std::sin(radian) + y * std::cos(radian);

		auto cp = CPoint((LONG)dcx, (LONG)dcy);

		m_vecPickedCoord[i] = cp;
		pntArr[i] = ToClientFromOthogonal(cp);
	}

	pntArr[count - 1] = pntArr[0];
	m_points.Add(std::make_pair(pntArr, count));

	UpdatePickCoords();
}

void CAaronMathViewerDlg::OnBnClickedButtonReset()
{
	ResetPicking();
	ResetParamCoords();
	Invalidate();
}

//void CAaronMathViewerDlg::OnLbnDblclkListView()
//{
//	int selectedIdx = m_lbxExpr.GetCurSel();
//	if (selectedIdx != LB_ERR)
//	{
//		CString expr;
//		switch (m_lastMethodRadioID)
//		{
//		case IDC_RADIO_PPC:
//		{
//			switch (selectedIdx)
//			{
//			case 0:
//			{
//				auto m = RationalNum(m_vecPickedCoord[1].y - m_vecPickedCoord[0].y, m_vecPickedCoord[1].x - m_vecPickedCoord[0].x);
//				auto c = RationalNum(-m_vecPickedCoord[0].x) * m + m_vecPickedCoord[0].y;
//
//				Formatter::LineQuation(L"직선 방정식", m, c, &expr, !m_bExprDecimal[0]);
//				break;
//			}
//			case 1:
//			{
//				auto m = RationalNum(m_vecPickedCoord[1].y - m_vecPickedCoord[0].y, m_vecPickedCoord[1].x - m_vecPickedCoord[0].x);
//				auto c = RationalNum(-m_vecPickedCoord[0].x) * m + m_vecPickedCoord[0].y;
//
//				auto im = RationalNum(-m.GetDenomiator(), m.GetNumerator());
//				auto ic = RationalNum(-m_vecPickedCoord[2].x) * im + m_vecPickedCoord[2].y;
//
//				Formatter::LineQuation(L"수선 방정식", im, ic, &expr, !m_bExprDecimal[selectedIdx]);
//				break;
//			}
//			case 2:
//			{
//				auto m = RationalNum(m_vecPickedCoord[1].y - m_vecPickedCoord[0].y, m_vecPickedCoord[1].x - m_vecPickedCoord[0].x);
//				auto c = RationalNum(-m_vecPickedCoord[0].x) * m + m_vecPickedCoord[0].y;
//
//				auto im = RationalNum(-m.GetDenomiator(), m.GetNumerator());
//				auto ic = RationalNum(-m_vecPickedCoord[2].x) * im + m_vecPickedCoord[2].y;
//
//				auto inter_x = RationalNum(ic - c, m - im);
//				auto inter_y = m * inter_x + c;
//
//				Formatter::Coord(L"교점", inter_x, inter_y, &expr, !m_bExprDecimal[selectedIdx]);
//				break;
//			}
//			default:
//				return;
//			}
//			break;
//		}
//		default:
//			return;
//		}
//
//		m_bExprDecimal[selectedIdx] = !m_bExprDecimal[selectedIdx];
//
//		m_lbxExpr.DeleteString(selectedIdx);
//		m_lbxExpr.InsertString(selectedIdx, expr);
//		m_lbxExpr.SetCurSel(selectedIdx);
//	}
//
//}




void CAaronMathViewerDlg::OnBnClickedButtonTestpick()
{
	CString show;
	m_edtTestCoord.GetWindowText(show);

	INT idx = 0;
	auto x = show.Tokenize(L", ", idx);
	auto y = show.Tokenize(L", ", idx);
	auto pnt = ToClientFromOthogonal(CPoint(_wtoi(x), _wtoi(y)));

	if (PickCoord(pnt))
	{
		UpdatePickCoords();
	}
}
