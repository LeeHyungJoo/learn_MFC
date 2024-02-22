#pragma once


// SubDlg 대화 상자

class SubDlg : public CDialog
{
	DECLARE_DYNAMIC(SubDlg)

public:
	SubDlg(CWnd* pParent, DTO dto);   // 표준 생성자입니다.
	virtual ~SubDlg();

public:
	void RenderDTO();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SubDlg };
#endif
protected:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedButton1();

public:
	CWnd* m_pParent;
	DTO m_dto;
	CListBox m_listbox;
};
