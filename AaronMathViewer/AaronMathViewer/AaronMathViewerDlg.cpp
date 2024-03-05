
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
	m_vecCoord.push_back(CPoint(100, 200));
	UpdatePickCoords();

	m_vecCoord.push_back(CPoint(200, 400));
	UpdatePickCoords();

	m_vecCoord.push_back(CPoint(150, 300));
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
	m_vecCoord.clear();
	m_lbxExpr.ResetContent();
	m_bExprDecimal.clear();

	for (size_t i = 0; i < m_vecCoordEdits.size(); i++)
	{
		m_vecCoordEdits[i]->SetWindowText(L"");
		auto itr = m_mPickedCoordCount.find(m_lastMethodRadioID);
		m_vecCoordEdits[i]->EnableWindow(itr != m_mPickedCoordCount.end() && (size_t)itr->second > i);
	}
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

	switch (m_lastMethodRadioID)
	{
	case IDC_RADIO_TRIROT:
		m_edtDegree.EnableWindow(TRUE);
		m_btnRot.EnableWindow(TRUE);
		break;
	}
}

void CAaronMathViewerDlg::UpdatePickCoords()
{
	for (size_t i = 0; i < m_vecCoord.size(); i++)
	{
		CString strCoord;
		strCoord.Format(_T("(%d, %d)"), m_vecCoord[i].x, m_vecCoord[i].y);
		m_vecCoordEdits[i]->SetWindowText(strCoord);
	}

	switch (m_lastMethodRadioID)
	{
	case IDC_RADIO_PPC:
	{
		if (m_vecCoord.size() == 2)
		{
			auto m = RationalNum(m_vecCoord[1].y - m_vecCoord[0].y, m_vecCoord[1].x - m_vecCoord[0].x);
			auto c = RationalNum(-m_vecCoord[0].x) * m + m_vecCoord[0].y;

			CString expr;
			Formatter::LineQuation(L"직선 방정식", m, c, &expr);
			m_lbxExpr.AddString(expr);
			m_bExprDecimal.push_back(FALSE);
		}
		else if (m_vecCoord.size() == 3)
		{
			auto m = RationalNum(m_vecCoord[1].y - m_vecCoord[0].y, m_vecCoord[1].x - m_vecCoord[0].x);
			auto c = RationalNum(-m_vecCoord[0].x) * m + m_vecCoord[0].y;

			if ((m * m_vecCoord[2].x + c) == m_vecCoord[2].y)
			{
				m_vecCoord.pop_back();
				AfxMessageBox(_T("직선 위에 있는 점에서는 수선을 내릴 수 없습니다 !\r\n다시 시도해주세요 ! "), MB_ICONWARNING | MB_OK);
				break;
			}

			auto im = RationalNum(-m.GetDenomiator(), m.GetNumerator());
			auto ic = RationalNum(-m_vecCoord[2].x) * im + m_vecCoord[2].y;

			CString expr;
			Formatter::LineQuation(L"수선 방정식", im, ic, &expr);
			m_lbxExpr.AddString(expr);
			m_bExprDecimal.push_back(FALSE);

			CString strCoord;
			auto inter_x = RationalNum(ic - c, m - im);
			auto inter_y = m * inter_x + c;
			Formatter::Coord(L"교점", inter_x, inter_y, &strCoord);

			m_lbxExpr.AddString(strCoord);
			m_bExprDecimal.push_back(FALSE);

			POINT tar;
			tar.x = static_cast<LONG>(inter_x.GetValue());
			tar.y = static_cast<LONG>(inter_y.GetValue());
			m_vecCoord.push_back(CPoint(tar));
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
	ON_LBN_DBLCLK(IDC_LIST_VIEW, &CAaronMathViewerDlg::OnLbnDblclkListView)
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

	switch (m_lastMethodRadioID)
	{
	case IDC_RADIO_PPC:
	{
		if (m_vecCoord.size() >= 2)
		{
			CPen pen(PS_SOLID, 2, RGB(0, 255, 120));
			CPen* pOldPen = boardDC->SelectObject(&pen);

			boardDC->MoveTo(m_vecCoord[0]);
			boardDC->LineTo(m_vecCoord[1]);
			if (m_vecCoord.size() > 3)
			{
				CPen dotPen(PS_DOT, 1, RGB(255, 120, 0));
				boardDC->SelectObject(&dotPen);

				boardDC->MoveTo(m_vecCoord[2]);
				boardDC->LineTo(m_vecCoord[3]);
			}
			boardDC->SelectObject(*pOldPen);
		}

		if (m_vecCoord.size() >= 1)
		{
			CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
			CPen* pOldPen = boardDC->SelectObject(&pen);

			boardDC->Ellipse(m_vecCoord[0].x - 2, m_vecCoord[0].y - 2, m_vecCoord[0].x + 2, m_vecCoord[0].y + 2);

			if (m_vecCoord.size() >= 2)
			{
				boardDC->Ellipse(m_vecCoord[1].x - 2, m_vecCoord[1].y - 2, m_vecCoord[1].x + 2, m_vecCoord[1].y + 2);

				if (m_vecCoord.size() > 3)
				{
					boardDC->Ellipse(m_vecCoord[2].x - 2, m_vecCoord[2].y - 2, m_vecCoord[2].x + 2, m_vecCoord[2].y + 2);
					boardDC->Ellipse(m_vecCoord[3].x - 2, m_vecCoord[3].y - 2, m_vecCoord[3].x + 2, m_vecCoord[3].y + 2);
				}
			}
			boardDC->SelectObject(*pOldPen);
		}

		break;
	}
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
		screenPoint.Offset(-wRect.left, -wRect.top);

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
			if (m_lastMethodRadioID == 0)
			{
				AfxMessageBox(_T("먼저 함수 모드를 선택하십시오. !"), MB_ICONWARNING | MB_OK);
			}
			else
			{
				auto itr = m_mPickedCoordCount.find(m_lastMethodRadioID);
				if (itr != m_mPickedCoordCount.end() && m_vecCoord.size() < (size_t)itr->second)
				{
					m_vecCoord.push_back(screenPoint);
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
	Invalidate();
}

void CAaronMathViewerDlg::OnLbnDblclkListView()
{
	int selectedIdx = m_lbxExpr.GetCurSel();
	if (selectedIdx != LB_ERR)
	{
		CString expr;
		switch (m_lastMethodRadioID)
		{
		case IDC_RADIO_PPC:
		{
			switch (selectedIdx)
			{
			case 0:
			{
				auto m = RationalNum(m_vecCoord[1].y - m_vecCoord[0].y, m_vecCoord[1].x - m_vecCoord[0].x);
				auto c = RationalNum(-m_vecCoord[0].x) * m + m_vecCoord[0].y;

				Formatter::LineQuation(L"직선 방정식", m, c, &expr, !m_bExprDecimal[0]);
				break;
			}
			case 1:
			{
				auto m = RationalNum(m_vecCoord[1].y - m_vecCoord[0].y, m_vecCoord[1].x - m_vecCoord[0].x);
				auto c = RationalNum(-m_vecCoord[0].x) * m + m_vecCoord[0].y;

				auto im = RationalNum(-m.GetDenomiator(), m.GetNumerator());
				auto ic = RationalNum(-m_vecCoord[2].x) * im + m_vecCoord[2].y;

				Formatter::LineQuation(L"수선 방정식", im, ic, &expr, !m_bExprDecimal[selectedIdx]);
				break;
			}
			case 2:
			{
				auto m = RationalNum(m_vecCoord[1].y - m_vecCoord[0].y, m_vecCoord[1].x - m_vecCoord[0].x);
				auto c = RationalNum(-m_vecCoord[0].x) * m + m_vecCoord[0].y;

				auto im = RationalNum(-m.GetDenomiator(), m.GetNumerator());
				auto ic = RationalNum(-m_vecCoord[2].x) * im + m_vecCoord[2].y;

				auto inter_x = RationalNum(ic - c, m - im);
				auto inter_y = m * inter_x + c;

				Formatter::Coord(L"교점", inter_x, inter_y, &expr, !m_bExprDecimal[selectedIdx]);
				break;
			}
			default:
				return;
			}
			break;
		}
		default:
			return;
		}

		m_bExprDecimal[selectedIdx] = !m_bExprDecimal[selectedIdx];

		m_lbxExpr.DeleteString(selectedIdx);
		m_lbxExpr.InsertString(selectedIdx, expr);
		m_lbxExpr.SetCurSel(selectedIdx);
	}

}
