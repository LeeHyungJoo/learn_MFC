
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
	, m_lastMethodRadioID(0U)
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
	m_bExprDecimal.clear();

	for (size_t i = 0; i < m_vecCoordEdits.size(); i++)
	{
		m_vecCoordEdits[i]->SetWindowText(L"");
		auto itr = m_mPickedCoordCount.find(m_lastMethodRadioID);
		m_vecCoordEdits[i]->EnableWindow(itr != m_mPickedCoordCount.end() && (size_t)itr->second > i);
	}
}

void CAaronMathViewerDlg::ResetParamCoords()
{
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

	switch (m_lastMethodRadioID)
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
			m_bExprDecimal.push_back(TRUE);
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
				m_bExprDecimal.push_back(TRUE);

				CString strCoord;
				Formatter::Coord(L"교점", m_vecPickedCoord[0].x, m_vecPickedCoord[2].y, &strCoord);

				m_lbxExpr.AddString(strCoord);
				m_bExprDecimal.push_back(TRUE);

				m_vecParamCoord.push_back(CPoint(m_vecPickedCoord[0].x, m_vecPickedCoord[2].y));
			}
			else if (dy == 0)
			{
				CString expr;
				Formatter::VerticalLineQuation(L"수선 방정식", m_vecPickedCoord[2].x, &expr);
				m_lbxExpr.AddString(expr);
				m_bExprDecimal.push_back(TRUE);

				CString strCoord;
				Formatter::Coord(L"교점", m_vecPickedCoord[2].x, m_vecPickedCoord[0].y, &strCoord);

				m_lbxExpr.AddString(strCoord);
				m_bExprDecimal.push_back(TRUE);

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
				m_bExprDecimal.push_back(TRUE);

				CString strCoord;
				auto inter_x = RationalNum(ic - c, m - im);
				auto inter_y = m * inter_x + c;
				Formatter::Coord(L"교점", inter_x, inter_y, &strCoord);

				m_lbxExpr.AddString(strCoord);
				m_bExprDecimal.push_back(TRUE);

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

			Formatter::Coord(L"C :", m_vecPickedCoord[2], &strCoord);
			m_lbxExpr.AddString(strCoord);
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

	CDC* boardDC = m_pcBoard.GetDC();
	switch (m_lastMethodRadioID)
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

	//TODO : Seperate Line, Dot Method.
	switch (m_lastMethodRadioID)
	{
	case IDC_RADIO_PPC:
	{
		if (m_vecPickedCoord.size() > 1)
			DrawLine(m_vecPickedCoord[0], m_vecPickedCoord[1]);

		if (m_vecPickedCoord.size() > 2 && m_vecParamCoord.size() > 0)
			DrawDotLine(m_vecPickedCoord[2], m_vecParamCoord[0]);

		if (m_vecPickedCoord.size() > 0)
			DrawDotCircle(m_vecPickedCoord[0]);

		if (m_vecPickedCoord.size() > 1)
			DrawDotCircle(m_vecPickedCoord[1]);

		if (m_vecPickedCoord.size() > 2)
			DrawDotCircle(m_vecPickedCoord[2]);

		if (m_vecParamCoord.size() > 0)
			DrawSpecificDotCircle(m_vecParamCoord[0]);

		break;
	}
	case IDC_RADIO_TRIROT:
	{
		if (m_vecPickedCoord.size() > 0)
			for(const auto& c : m_vecPickedCoord)
				DrawDotCircle(c);

		if (m_vecPickedCoord.size() == 3)
			DrawPolyLine(m_vecPickedCoord, 0, 2);

		break;
	}
	}
}

void CAaronMathViewerDlg::OnMethodRadioChanged(UINT ID)
{
	if (m_lastMethodRadioID == ID)
		return;

	switch (m_lastMethodRadioID)
	{
	case IDC_RADIO_TRIROT:
		m_edtDegree.EnableWindow(FALSE);
		m_btnRot.EnableWindow(FALSE);
		break;
	}

	m_lastMethodRadioID = ID;

	ResetPicking();
	ResetParamCoords();

	switch (m_lastMethodRadioID)
	{
	case IDC_RADIO_TRIROT:
		m_edtDegree.EnableWindow(TRUE);
		m_btnRot.EnableWindow(TRUE);
		break;
	}
}

void CAaronMathViewerDlg::ChangeCoordinateSystem(OUT CPoint & point)
{
	switch (m_lastMethodRadioID)
	{
	case IDC_RADIO_TRIROT:
	{
		CRect cRect;
		m_pcBoard.GetClientRect(&cRect);
		point.x -= cRect.right / 2;
		point.y -= cRect.bottom / 2;
		point.y *= -1;
		break;
	}
	default:
		break;
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

void CAaronMathViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint screenPoint = point;
	ClientToScreen(&screenPoint);

	CRect wRect;
	m_pcBoard.GetWindowRect(&wRect);

	if (IsScreenPointInRect(screenPoint, wRect))
	{
		screenPoint.Offset(-wRect.left, -wRect.top);

		ChangeCoordinateSystem(screenPoint);

		CString strCoord;
		strCoord.Format(_T("(%d, %d)"), screenPoint.x, screenPoint.y);
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
			screenPoint.Offset(-wRect.left, -wRect.top);
			ChangeCoordinateSystem(screenPoint);

			if (m_lastMethodRadioID == 0)
			{
				AfxMessageBox(_T("먼저 함수 모드를 선택하십시오. !"), MB_ICONWARNING | MB_OK);
			}
			else
			{
				auto itr = m_mPickedCoordCount.find(m_lastMethodRadioID);
				if (itr != m_mPickedCoordCount.end() && m_vecPickedCoord.size() < (size_t)itr->second)
				{
					m_vecPickedCoord.push_back(screenPoint);
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
