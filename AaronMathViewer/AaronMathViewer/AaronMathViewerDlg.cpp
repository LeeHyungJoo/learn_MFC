
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
{
}

void CAaronMathViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAaronMathViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

BOOL CAaronMathViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;
}

void CAaronMathViewerDlg::OnPaint()
{
	CDialogEx::OnPaint();
}


void CAaronMathViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}
