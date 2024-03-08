
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
	m_mPickedCoordCount[IDC_RADIO3] = -1;
	m_mPickedCoordCount[IDC_RADIO4] = -1;
	m_mPickedCoordCount[IDC_RADIO5] = -1;
}

CAaronMathViewerDlg::~CAaronMathViewerDlg()
{

}

void CAaronMathViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICCONTROL, m_pcBoard);
	DDX_Control(pDX, IDC_STATIC_PNTCOORD, m_stCoord);
	DDX_Control(pDX, IDC_LIST_VIEW, m_lbxExpr);
	DDX_Control(pDX, IDC_EDIT_DEGREE, m_edtDegree);
	DDX_Control(pDX, IDC_BUTTON_ROT, m_btnRot);
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
	m_iRotDegree = 0;
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
		CString strCoord;
		strCoord.Format(_T("(%d, %d)"), m_vecPickedCoord[i].x, m_vecPickedCoord[i].y);
		static_cast<CEdit*>(m_vecCoordEdits[i])->SetWindowText(strCoord);
	}

	switch (m_uMethodID)
	{
	case IDC_RADIO_PPC:
	{
		if (m_vecPickedCoord.GetSize() == 2)
		{
			CString expr;
			Formatter::LineQuation(
				L"직선 방정식", 
				m_vecPickedCoord[1].x - m_vecPickedCoord[0].x,
				m_vecPickedCoord[1].y - m_vecPickedCoord[0].y, 
				m_vecPickedCoord[0].x,
				m_vecPickedCoord[0].y,
				&expr);

			m_lbxExpr.AddString(expr);
		}
		else if (m_vecPickedCoord.GetSize() == 3)
		{
			auto dy = m_vecPickedCoord[1].y - m_vecPickedCoord[0].y;
			auto dx = m_vecPickedCoord[1].x - m_vecPickedCoord[0].x;

			if (dx == 0)
			{
				CString expr;
				Formatter::HorizontalLineQuation(L"수선 방정식", m_vecPickedCoord[2].y, &expr);
				m_lbxExpr.AddString(expr);

				CString strCoord;
				Formatter::Coord(L"교점", m_vecPickedCoord[0].x, m_vecPickedCoord[2].y, &strCoord);

				m_lbxExpr.AddString(strCoord);

				m_vecParamCoord.Add(CPoint(m_vecPickedCoord[0].x, m_vecPickedCoord[2].y));
			}
			else if (dy == 0)
			{
				CString expr;
				Formatter::VerticalLineQuation(L"수선 방정식", m_vecPickedCoord[2].x, &expr);
				m_lbxExpr.AddString(expr);

				CString strCoord;
				Formatter::Coord(L"교점", m_vecPickedCoord[2].x, m_vecPickedCoord[0].y, &strCoord);

				m_lbxExpr.AddString(strCoord);

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

				CString expr;
				Formatter::LineQuation(L"수선 방정식", im, ic, &expr);
				m_lbxExpr.AddString(expr);

				CString strCoord;
				auto inter_x = RationalNum(ic - c, m - im);
				auto inter_y = m * inter_x + c;
				Formatter::Coord(L"교점", inter_x, inter_y, &strCoord);

				m_lbxExpr.AddString(strCoord);

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

				INT count = m_vecDoubleCoord.GetSize() + 1;
				POINT* pntArr = new POINT[count];

				for (int i = 0; i < m_vecDoubleCoord.GetSize(); i++)
					pntArr[i] = ToClientFromOthogonal(CPoint((LONG)m_vecDoubleCoord[i].x, (LONG)m_vecDoubleCoord[i].y));

				pntArr[count - 1] = pntArr[0];
				m_points.Add(std::make_pair(pntArr, count));
			}

			CString strCoord;
			auto radian = (DOUBLE)m_iRotDegree * (M_PI / 180.0);

			for (int i = 0; i < m_vecDoubleCoord.GetSize(); i++)
			{
				auto x = m_vecDoubleCoord[i].x;
				auto y = m_vecDoubleCoord[i].y;

				double dcx = x * std::cos(radian) - y * std::sin(radian);
				double dcy = x * std::sin(radian) + y * std::cos(radian);

				Formatter::Coord(L"", i ,dcx, dcy, &strCoord);
				m_lbxExpr.AddString(strCoord);
			}

			m_lbxExpr.SetCurSel(m_lbxExpr.GetCount() - 1);
		}
		break;
	}
	}

	Invalidate();
}

BEGIN_MESSAGE_MAP(CAaronMathViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND_RANGE(IDC_RADIO_PPC, IDC_RADIO5, OnMethodRadioChanged)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CAaronMathViewerDlg::OnBnClickedButtonReset)
	//ON_LBN_DBLCLK(IDC_LIST_VIEW, &CAaronMathViewerDlg::OnLbnDblclkListView)
	ON_BN_CLICKED(IDC_BUTTON_ROT, &CAaronMathViewerDlg::OnBnClickedButtonRot)
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
	case IDC_RADIO3:
	case IDC_RADIO4:
	case IDC_RADIO5:
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

void CAaronMathViewerDlg::DrawSpecificDotCircle(CPaintDC* dc, const CPoint & point)
{
	CPen interPen(PS_SOLID, 3, RGB(0, 50, 255));
	CPen* pOldPen = dc->SelectObject(&interPen);

	dc->SelectObject(&interPen);
	dc->Ellipse(point.x - 2, point.y - 2, point.x + 2, point.y + 2);

	dc->SelectObject(*pOldPen);
}

void CAaronMathViewerDlg::DrawPolyLine(CPaintDC* dc, const CArray<CPoint>& points, INT startIdx, INT endIdx)
{
	if (endIdx - startIdx < 2)
		return;

	CPen dotPen(PS_SOLID, 1, RGB(120, 0, 255));
	CPen* pOldPen = dc->SelectObject(&dotPen);

	INT count = endIdx - startIdx + 2;
	POINT* pntArr = new POINT[count];
	for (INT i = startIdx; i <= endIdx; i++)
		pntArr[i] = points[i];
	pntArr[count - 1] = points[startIdx];

	dc->Polyline(pntArr, count);
	dc->SelectObject(*pOldPen);

	delete[] pntArr;
}

void CAaronMathViewerDlg::DrawPolyLines(CPaintDC* dc, const CArray<std::pair<POINT*, INT>>& points, INT startIdx, INT endIdx)
{
	CPen pen(PS_DOT, 1, RGB(200, 200, 200));

	CPen* pOldPen = dc->SelectObject(&pen);

	for (INT i = startIdx; i <= endIdx; i++)
		dc->Polyline(points[i].first, points[i].second);

	dc->SelectObject(*pOldPen);
}

void CAaronMathViewerDlg::DrawOthogonal(CPaintDC* dc)
{
	switch (m_uMethodID)
	{
	case IDC_RADIO_TRIROT:
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

		CString strCoord;
		strCoord.Format(_T("(%d, %d)"), othogonalPnt.x, othogonalPnt.y);
		m_stCoord.SetWindowText(strCoord);
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
		auto othogonalPnt = ToOthogonalFromClient(clientPnt);

		if (m_uMethodID != 0)
		{
			INT cnt = 0;
			m_mPickedCoordCount.Lookup(m_uMethodID, cnt);
			if (m_vecPickedCoord.GetSize() < cnt)
			{
				m_vecPickedCoord.Add(othogonalPnt);
				m_vecDoubleCoord.Add(CPointDouble(othogonalPnt));
				UpdatePickCoords();
			}
			else
			{
				AfxMessageBox(_T("이미 필요한 좌표를 모두 입력하셨습니다. !"), MB_ICONWARNING | MB_OK);
			}
		}
		else
		{
			AfxMessageBox(_T("먼저 함수 모드를 선택하십시오. !"), MB_ICONWARNING | MB_OK);
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
	INT count = m_vecDoubleCoord.GetSize() + 1;
	POINT* pntArr = new POINT[count];

	for (int i = 0; i < m_vecDoubleCoord.GetSize(); i++)
	{
		auto x = m_vecDoubleCoord[i].x;
		auto y = m_vecDoubleCoord[i].y;

		double dcx = x * std::cos(radian) - y * std::sin(radian);
		double dcy = x * std::sin(radian) + y * std::cos(radian);

		auto cp = CPoint(dcx, dcy);

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


