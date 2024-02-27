#include "pch.h"
#include "framework.h"
#include "MFC_Number.h"
#include "MFC_NumberDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMFCNumberDlg::CMFCNumberDlg(CWnd* pParent)
	: CDialogEx(IDD_MFC_NUMBER_DIALOG, pParent)
{

}

void CMFCNumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RANDLIST, m_lsbNums);
	DDX_Control(pDX, IDC_EDIT_CNT, m_editCnt);
}

BEGIN_MESSAGE_MAP(CMFCNumberDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CMFCNumberDlg message handlers

BOOL CMFCNumberDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;
}
