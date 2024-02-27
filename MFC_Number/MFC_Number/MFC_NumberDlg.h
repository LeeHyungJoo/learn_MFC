#pragma once

class CMFCNumberDlg : public CDialogEx
{
// Construction
public:
	CMFCNumberDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_NUMBER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	CListBox m_lsbNums;
	CEdit m_editCnt;

	std::vector<CString> m_data;
};
