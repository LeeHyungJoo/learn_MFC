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
	, m_uCnt(10U), m_pVals(new DOUBLE[MAX_LIST_CNT])
{
}

void CMFCNumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RANDLIST, m_lsbNums);
	DDX_Control(pDX, IDC_EDIT_CNT, m_editCnt);
	DDX_Control(pDX, IDC_EDIT_AVG, m_editAvg);
	DDX_Control(pDX, IDC_EDIT_SD, m_editSd);
	DDX_Control(pDX, IDC_EDIT_MNX, m_editMnx);
	DDX_Text(pDX, IDC_EDIT_CNT, m_uCnt);

	DDV_MinMaxUInt(pDX, m_uCnt, 10u, (std::numeric_limits<UINT>::max)());
}

BEGIN_MESSAGE_MAP(CMFCNumberDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_AVG, &CMFCNumberDlg::OnBnClickedButtonAvg)
	ON_BN_CLICKED(IDC_BUTTON_SD, &CMFCNumberDlg::OnBnClickedButtonSd)
	ON_BN_CLICKED(IDC_BUTTON_MNX, &CMFCNumberDlg::OnBnClickedButtonMnx)
	ON_BN_CLICKED(IDC_BUTTON_GEN, &CMFCNumberDlg::OnBnClickedButtonGen)
END_MESSAGE_MAP()


// CMFCNumberDlg message handlers

BOOL CMFCNumberDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

void CMFCNumberDlg::OnBnClickedButtonGen()
{
	//m_lsbNums.
}


void CMFCNumberDlg::OnBnClickedButtonAvg()
{

}


void CMFCNumberDlg::OnBnClickedButtonSd()
{

}


void CMFCNumberDlg::OnBnClickedButtonMnx()
{

}
