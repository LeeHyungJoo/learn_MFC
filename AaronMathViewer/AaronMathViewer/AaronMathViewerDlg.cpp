
// AaronMathViewerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "AaronMathViewer.h"
#include "AaronMathViewerDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAaronMathViewerDlg::CAaronMathViewerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AARONMATHVIEWER_DIALOG, pParent)
	, m_lastMethodRadioID(0U)
{
	m_mPickedCoordCount[IDC_RADIO_PPC] = 3U;
	m_mPickedCoordCount[IDC_RADIO2] = -1;
	m_mPickedCoordCount[IDC_RADIO3] = -1;
	m_mPickedCoordCount[IDC_RADIO4] = -1;
	m_mPickedCoordCount[IDC_RADIO5] = -1;
}

void CAaronMathViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICCONTROL, m_pcBoard);
	DDX_Control(pDX, IDC_STATIC_PNTCOORD, m_stCoord);
	DDX_Control(pDX, IDC_LIST_VIEW, m_lbxExpr);
}

void CAaronMathViewerDlg::ResetContentBoard()
{
	CClientDC dc(&m_pcBoard);
	CRect cRect;
	m_pcBoard.GetClientRect(&cRect);
	dc.FillSolidRect(cRect, RGB(255, 255, 255));
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

	m_lastMethodRadioID = ID;

	m_vecCoord.clear();
	m_lbxExpr.ResetContent();
	ResetContentBoard();

	for (size_t i = 0; i < m_vecCoordEdits.size(); i++)
	{
		m_vecCoordEdits[i]->SetWindowText(L"");
		auto itr = m_mPickedCoordCount.find(m_lastMethodRadioID);
		m_vecCoordEdits[i]->EnableWindow(itr != m_mPickedCoordCount.end() && (size_t)itr->second > i);
	}
}

void CAaronMathViewerDlg::UpdatePickCoords()
{
	for (size_t i = 0 ; i < m_vecCoord.size() ; i++)
	{
		CString strCoord;
		strCoord.Format(_T("(%d, %d)"), m_vecCoord[i].x, m_vecCoord[i].y);
		m_vecCoordEdits[i]->SetWindowText(strCoord);
	}

	CClientDC dc(&m_pcBoard);
	switch (m_lastMethodRadioID)
	{
	case IDC_RADIO_PPC:
	{
		if (m_vecCoord.size() == 2)
		{
			dc.MoveTo(m_vecCoord[0]);
			dc.LineTo(m_vecCoord[1]);
			
			auto m = RationalNum(m_vecCoord[1].y - m_vecCoord[0].y, m_vecCoord[1].x - m_vecCoord[0].x);
			auto c = RationalNum(-m_vecCoord[0].x) * m + m_vecCoord[0].y;

			CString expr;
			Formatter::LineQuation(L"직선 방정식", m, c, &expr);
			m_lbxExpr.AddString(expr);
		}
		else if (m_vecCoord.size() == 3)
		{
			auto m = RationalNum(m_vecCoord[1].y - m_vecCoord[0].y, m_vecCoord[1].x - m_vecCoord[0].x);
			auto c = RationalNum(-m_vecCoord[0].x) * m + m_vecCoord[0].y;

			auto im = RationalNum(-m.GetDenomiator(), m.GetNumerator());
			auto ic = RationalNum(-m_vecCoord[2].x) * im + m_vecCoord[2].y;

			CString expr;
			Formatter::LineQuation(L"수선 방정식", im, ic, &expr);
			m_lbxExpr.AddString(expr);


			CString strCoord;
			auto inter_x = RationalNum(ic - c, m - im);
			auto inter_y = m * inter_x + c;
			Formatter::Coord(L"교점", inter_x, inter_y, &strCoord);

			dc.MoveTo(m_vecCoord[2]);
			POINT tar;
			tar.x = static_cast<LONG>(inter_x.GetValue());
			tar.y = static_cast<LONG>(inter_y.GetValue());
			dc.LineTo(CPoint(tar));

			m_lbxExpr.AddString(strCoord);
		}

		break;
	}
	}

}

BEGIN_MESSAGE_MAP(CAaronMathViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND_RANGE(IDC_RADIO_PPC, IDC_RADIO5, OnMethodRadioChanged)
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
		screenPoint.Offset(-wRect.left, -wRect.top);
		auto itr = m_mPickedCoordCount.find(m_lastMethodRadioID);
		if (itr != m_mPickedCoordCount.end() && m_vecCoord.size() < (size_t)itr->second)
		{
			m_vecCoord.push_back(screenPoint);
			UpdatePickCoords();
		}
		else
		{
			AfxMessageBox(_T("Already Picked MAX Coords"), MB_ICONWARNING | MB_OK);
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}
