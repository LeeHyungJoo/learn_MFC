// SubDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCSampleSolution_1.h"
#include "SubDlg.h"
#include "MFCSampleSolution_1Dlg.h"
#include "afxdialogex.h"


// SubDlg 대화 상자

IMPLEMENT_DYNAMIC(SubDlg, CDialog)

SubDlg::SubDlg(CWnd* pParent, DTO dto)
	: CDialog(IDD_SubDlg, pParent), m_pParent(pParent), m_dto(dto)
{
}

SubDlg::~SubDlg()
{
}

void SubDlg::RenderDTO()
{
	m_listbox.ResetContent();

	m_listbox.AddString(m_dto.combo);
}

BOOL SubDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	RenderDTO();

	return TRUE;
}

void SubDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listbox);
}


BEGIN_MESSAGE_MAP(SubDlg, CDialog)
	ON_BN_CLICKED(IDCLOSE, &SubDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_BUTTON1, &SubDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// SubDlg 메시지 처리기


void SubDlg::OnBnClickedClose()
{
	CDialog::DestroyWindow();
}


void SubDlg::OnBnClickedButton1()
{
	auto maindlg = (MainDlg*)(m_pParent);
	maindlg->MakeDTO(&m_dto);
	RenderDTO();
}

