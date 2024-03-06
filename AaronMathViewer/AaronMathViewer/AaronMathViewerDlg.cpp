
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

void CAaronMathViewerDlg::TESTPICKCOORDS()
{
	m_vecPickedCoord.push_back(CPoint(100, 200));
	UpdatePickCoords();

	m_vecPickedCoord.push_back(CPoint(200, 400));
	UpdatePickCoords();

	m_vecPickedCoord.push_back(CPoint(150, 300));
	UpdatePickCoords();
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
	m_vecPickedCoord.clear();
	m_lbxExpr.ResetContent();

	for (size_t i = 0; i < m_vecCoordEdits.size(); i++)
	{
		m_vecCoordEdits[i]->SetWindowText(L"");
		auto itr = m_mPickedCoordCount.find(m_uMethodID);
		m_vecCoordEdits[i]->EnableWindow(itr != m_mPickedCoordCount.end() && (size_t)itr->second > i);
	}
}

void CAaronMathViewerDlg::ResetParamCoords()
{
	m_vecDoubleCoord.clear();
	m_vecParamCoord.clear();
}

BOOL CAaronMathViewerDlg::IsScreenPointInRect(const CPoint & screenPoint, const CRect & wRect) const
{
	BOOL bIn = true;
	bIn &= screenPoint.y <= wRect.bottom;
	bIn &= screenPoint.y >= wRect.top;
	bIn &= screenPoint.x <= wRect.right;
	bIn &= screenPoint.x >= wRect.left;
	return bIn;
}

void CAaronMathViewerDlg::UpdatePickCoords()
{
	for (size_t i = 0; i < m_vecPickedCoord.size(); i++)
	{
		CString strCoord;
		strCoord.Format(_T("(%d, %d)"), m_vecPickedCoord[i].x, m_vecPickedCoord[i].y);
		m_vecCoordEdits[i]->SetWindowText(strCoord);
	}

	switch (m_uMethodID)
	{
	case IDC_RADIO_PPC:
	{
		if (m_vecPickedCoord.size() == 2)
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
		else if (m_vecPickedCoord.size() == 3)
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

				m_vecParamCoord.push_back(CPoint(m_vecPickedCoord[0].x, m_vecPickedCoord[2].y));
			}
			else if (dy == 0)
			{
				CString expr;
				Formatter::VerticalLineQuation(L"수선 방정식", m_vecPickedCoord[2].x, &expr);
				m_lbxExpr.AddString(expr);

				CString strCoord;
				Formatter::Coord(L"교점", m_vecPickedCoord[2].x, m_vecPickedCoord[0].y, &strCoord);

				m_lbxExpr.AddString(strCoord);

				m_vecParamCoord.push_back(CPoint(m_vecPickedCoord[2].x, m_vecPickedCoord[0].y));
			}
			else
			{
				auto m = RationalNum(m_vecPickedCoord[1].y - m_vecPickedCoord[0].y, m_vecPickedCoord[1].x - m_vecPickedCoord[0].x);
				auto c = RationalNum(-m_vecPickedCoord[0].x) * m + m_vecPickedCoord[0].y;

				if ((m * m_vecPickedCoord[2].x + c) == m_vecPickedCoord[2].y)
				{
					m_vecPickedCoord.pop_back();
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
				m_vecParamCoord.push_back(CPoint(tar));
			}
		}
		break;
	}
	case IDC_RADIO_TRIROT:
	{

		if (m_vecPickedCoord.size() == 3)
		{
			CString strCoord;	

			Formatter::Coord(L"A :", m_vecPickedCoord[0], &strCoord);
			m_lbxExpr.AddString(strCoord);

			Formatter::Coord(L"B :", m_vecPickedCoord[1], &strCoord);
			m_lbxExpr.AddString(strCoord);

			auto m = RationalNum(m_vecPickedCoord[1].y - m_vecPickedCoord[0].y, m_vecPickedCoord[1].x - m_vecPickedCoord[0].x);
			auto c = RationalNum(-m_vecPickedCoord[0].x) * m + m_vecPickedCoord[0].y;

			if ((m * m_vecPickedCoord[2].x + c) == m_vecPickedCoord[2].y)
			{
				m_vecPickedCoord.pop_back();
				m_vecDoubleCoord.pop_back();
				AfxMessageBox(_T("앞의 두 점을 지나는 직선 위의 점은 선택할 수 없습니다!\r\n다시 시도해주세요 ! "), MB_ICONWARNING | MB_OK);
				break;
			}


			Formatter::Coord(L"C :", m_vecPickedCoord[2], &strCoord);
			m_lbxExpr.AddString(strCoord);

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
		m_vecCoordEdits.push_back((CEdit*)(GetDlgItem(editID)));

	return TRUE;
}

void CAaronMathViewerDlg::OnPaint()
{
	CDialogEx::OnPaint();

	ResetBoard();
	DrawMethod();
	DrawOthogonal();
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

void CAaronMathViewerDlg::ResetBoard()
{
	CDC* boardDC = m_pcBoard.GetDC();
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
		boardDC->FillSolidRect(cRect, RGB(255, 255, 255));
		break;
	}
	default:
		return;
	}
}

void CAaronMathViewerDlg::DrawMethod()
{
	std::vector<CPoint> oPickedCrd(m_vecPickedCoord);
	for (auto& oc : oPickedCrd)
		oc = ToClientFromOthogonal(oc);

	std::vector<CPoint> oParamCrd(m_vecParamCoord);
	for (auto& oc : oParamCrd)
		oc = ToClientFromOthogonal(oc);

	switch (m_uMethodID)
	{
	case IDC_RADIO_PPC:
	{
		if (oPickedCrd.size() > 1)
			DrawLine(oPickedCrd[0], oPickedCrd[1]);

		if (oPickedCrd.size() > 2 && oParamCrd.size() > 0)
			DrawDotLine(oPickedCrd[2], oParamCrd[0]);

		if (oPickedCrd.size() > 0)
			DrawDotCircle(oPickedCrd[0]);

		if (oPickedCrd.size() > 1)
			DrawDotCircle(oPickedCrd[1]);

		if (oPickedCrd.size() > 2)
			DrawDotCircle(oPickedCrd[2]);

		if (oParamCrd.size() > 0)
			DrawSpecificDotCircle(oParamCrd[0]);

		break;
	}
	case IDC_RADIO_TRIROT:
	{
		if (oPickedCrd.size() > 0)
			for (const auto& oc : oPickedCrd)
				DrawDotCircle(oc);

		if (oPickedCrd.size() == 3)
			DrawPolyLine(oPickedCrd, 0, 2);

		break;
	}
	}
}

void CAaronMathViewerDlg::DrawLine(const CPoint & start, const CPoint & end)
{
	CDC* boardDC = m_pcBoard.GetDC();

	CPen pen(PS_SOLID, 2, RGB(0, 255, 120));
	CPen* pOldPen = boardDC->SelectObject(&pen);

	boardDC->MoveTo(start);
	boardDC->LineTo(end);

	boardDC->SelectObject(*pOldPen);
}

void CAaronMathViewerDlg::DrawDotLine(const CPoint & start, const CPoint & end)
{
	CDC* boardDC = m_pcBoard.GetDC();

	CPen dotPen(PS_DOT, 1, RGB(255, 120, 0));
	CPen* pOldPen = boardDC->SelectObject(&dotPen);

	boardDC->MoveTo(start);
	boardDC->LineTo(end);

	boardDC->SelectObject(*pOldPen);
}

void CAaronMathViewerDlg::DrawDotCircle(const CPoint & point)
{
	CDC* boardDC = m_pcBoard.GetDC();

	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen* pOldPen = boardDC->SelectObject(&pen);

	boardDC->Ellipse(point.x - 2, point.y - 2, point.x + 2, point.y + 2);
	boardDC->SelectObject(*pOldPen);
}

void CAaronMathViewerDlg::DrawSpecificDotCircle(const CPoint & point)
{
	CDC* boardDC = m_pcBoard.GetDC();

	CPen interPen(PS_SOLID, 3, RGB(0, 50, 255));
	CPen* pOldPen = boardDC->SelectObject(&interPen);

	boardDC->SelectObject(&interPen);
	boardDC->Ellipse(point.x - 2, point.y - 2, point.x + 2, point.y + 2);

	boardDC->SelectObject(*pOldPen);
}

void CAaronMathViewerDlg::DrawPolyLine(const std::vector<CPoint>& points, INT startIdx, INT endIdx)
{
	if (endIdx - startIdx < 2)
		return;

	CDC* boardDC = m_pcBoard.GetDC();
	CPen dotPen(PS_SOLID, 1, RGB(120, 0, 255));
	CPen* pOldPen = boardDC->SelectObject(&dotPen);

	INT count = endIdx - startIdx + 2;
	POINT* pntArr = new POINT[count];
	for (INT i = startIdx; i <= endIdx; i++)
		pntArr[i] = points[i];
	pntArr[count - 1] = points[startIdx];

	boardDC->Polyline(pntArr, count);
	boardDC->SelectObject(*pOldPen);
}

void CAaronMathViewerDlg::DrawOthogonal()
{
	CDC* boardDC = m_pcBoard.GetDC();

	switch (m_uMethodID)
	{
	case IDC_RADIO_TRIROT:
	{
		CPen dotPen(PS_DOT, 1, RGB(0, 0, 0));
		CPen* pOldPen = boardDC->SelectObject(&dotPen);

		CRect cRect;
		m_pcBoard.GetClientRect(&cRect);

		boardDC->MoveTo(cRect.right / 2, cRect.top);
		boardDC->LineTo(cRect.right / 2, cRect.bottom);

		boardDC->MoveTo(cRect.left, cRect.right / 2);
		boardDC->LineTo(cRect.right, cRect.right / 2);

		boardDC->SelectObject(*pOldPen);
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

	CRect wRect;
	m_pcBoard.GetWindowRect(&wRect);

	if (IsScreenPointInRect(screenPoint, wRect))
	{
		auto clientPnt = CPoint(screenPoint.x - wRect.left, screenPoint.y - wRect.top);
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

	CRect wRect;
	m_pcBoard.GetWindowRect(&wRect);

	if (IsScreenPointInRect(screenPoint, wRect))
	{
		if (B_TEST)
		{
			TESTPICKCOORDS();
		}
		else
		{
			auto clientPnt = CPoint(screenPoint.x - wRect.left, screenPoint.y - wRect.top);
			auto othogonalPnt = ToOthogonalFromClient(clientPnt);

			if (m_uMethodID == 0)
			{
				AfxMessageBox(_T("먼저 함수 모드를 선택하십시오. !"), MB_ICONWARNING | MB_OK);
			}
			else
			{
				auto itr = m_mPickedCoordCount.find(m_uMethodID);
				if (itr != m_mPickedCoordCount.end() && m_vecPickedCoord.size() < (size_t)itr->second)
				{
					m_vecPickedCoord.push_back(othogonalPnt);
					m_vecDoubleCoord.push_back(othogonalPnt);
					UpdatePickCoords();
				}
				else
				{
					AfxMessageBox(_T("이미 필요한 좌표를 모두 입력하셨습니다. !"), MB_ICONWARNING | MB_OK);
				}
			}
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
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


void CAaronMathViewerDlg::OnBnClickedButtonRot()
{
	if (m_uMethodID != IDC_RADIO_TRIROT)
		return;

	CString degreeStr;
	m_edtDegree.GetWindowText(degreeStr);
	auto degree = _wtoi(degreeStr);
	auto radian = degree * (M_PI / 180.0);

	for (int i = 0 ; i < m_vecPickedCoord.size(); i++)
	{
		auto x = m_vecDoubleCoord[i].x;
		auto y = m_vecDoubleCoord[i].y;
		m_vecDoubleCoord[i].x = x * std::cos(radian) - y * std::sin(radian);
		m_vecDoubleCoord[i].y = x * std::sin(radian) + y * std::cos(radian);
		m_vecPickedCoord[i] = CPoint((LONG)m_vecDoubleCoord[i].x, (LONG)m_vecDoubleCoord[i].y);
	}

	UpdatePickCoords();
}
