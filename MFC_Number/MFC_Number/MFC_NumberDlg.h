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
	afx_msg void OnBnClickedButtonGen();

	afx_msg void OnBnClickedButtonAvg();
	afx_msg void OnBnClickedButtonSd();
	afx_msg void OnBnClickedButtonMnx();

public:
	CListBox m_lsbNums;
	DOUBLE* m_pVals;
	BOOL m_bSet;

	CEdit m_editCnt;
	UINT m_uCnt;

	CEdit m_editAvg;
	CEdit m_editSd;
	CEdit m_editMnx;
};
