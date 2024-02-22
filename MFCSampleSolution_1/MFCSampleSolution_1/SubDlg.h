#pragma once

class SubDlg : public CDialog
{
	DECLARE_DYNAMIC(SubDlg)

public:
	SubDlg(CWnd* pParent, DTO dto);
	virtual ~SubDlg();

protected:
	virtual void DoDataExchange
	(CDataExchange* pDX);

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	void RenderDTO();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SubDlg };
#endif

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedClose();

public:
	CWnd* m_pParent;
	DTO m_dto;
	CListBox m_lstbSpec;
};
