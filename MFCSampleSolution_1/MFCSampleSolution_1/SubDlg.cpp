#include "pch.h"
#include "MFCSampleSolution_1.h"
#include "SubDlg.h"
#include "MFCSampleSolution_1Dlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(SubDlg, CDialog)

SubDlg::SubDlg(CWnd* pParent, DTO dto)
	: CDialog(IDD_SubDlg, pParent)
	, m_pParent(pParent)
	, m_dto(dto)
{
}

SubDlg::~SubDlg()
{
	delete m_pParent;
}

void SubDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstbSpec);
}

BOOL SubDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	RenderDTO();
	return TRUE;
}

BEGIN_MESSAGE_MAP(SubDlg, CDialog)
	ON_BN_CLICKED(IDCLOSE, &SubDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_BUTTON1, &SubDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

void SubDlg::RenderDTO()
{
	m_lstbSpec.ResetContent();
	{
		CString s;
		s.Format(L"combo select [%s]", m_dto.sComboSelect);
		m_lstbSpec.AddString(s);
	}
	{
		CString s;
		s.Format(L"option type [%s]", m_dto.sOptionType);
		m_lstbSpec.AddString(s);
	}
	{
		CString s;
		s.Format(L"options [%s]", m_dto.sOption);
		m_lstbSpec.AddString(s);
	}
	{
		CString s;
		s.Format(L"counter [%d]", m_dto.uCounter);
		m_lstbSpec.AddString(s);
	}
	{
		CString s;
		s.Format(L"timer elapsed [%d]", m_dto.uElapsed);
		m_lstbSpec.AddString(s);
	}
	{
		CString s;
		s.Format(L"horizontal scroll [%d]", m_dto.uHScrollbarPos);
		m_lstbSpec.AddString(s);
	}
	{
		CString s;
		s.Format(L"vertical scroll [%d]", m_dto.uVScrollbarPos);
		m_lstbSpec.AddString(s);
	}
}

void SubDlg::OnBnClickedButton1()
{
	dynamic_cast<MainDlg*>(m_pParent)->MakeDTO(&m_dto);
	RenderDTO();
}

void SubDlg::OnBnClickedClose()
{
	CDialog::DestroyWindow();
}
