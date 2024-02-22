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
	DDX_Control(pDX, IDC_LIST1, m_listbox);
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
	m_listbox.ResetContent();
	{
		CString s;
		s.Format(L"combo [%s]", m_dto.combo);
		m_listbox.AddString(s);
	}
	{
		CString s;
		s.Format(L"radio [%s]", m_dto.opttype);
		m_listbox.AddString(s);
	}
	{
		CString s;
		s.Format(L"checkbox [%s]", m_dto.opt);
		m_listbox.AddString(s);
	}
	{
		CString s;
		s.Format(L"timer value [%d]", m_dto.val);
		m_listbox.AddString(s);
	}
	{
		CString s;
		s.Format(L"timer elapsed [%d]", m_dto.elapse);
		m_listbox.AddString(s);
	}
	{
		CString s;
		s.Format(L"horizontal scroll [%d]", m_dto.hscroll_val);
		m_listbox.AddString(s);
	}
	{
		CString s;
		s.Format(L"vertical scroll [%d]", m_dto.vscroll_val);
		m_listbox.AddString(s);
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
