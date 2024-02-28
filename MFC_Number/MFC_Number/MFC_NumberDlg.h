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
	void MakeValArr(OUT DOUBLE* pVals, UINT cnt);
	void CalculateAverage(DOUBLE* pVals, UINT cnt, OUT DOUBLE* average);
	void CalculateStandardDeviation(DOUBLE* pVals, UINT cnt, OUT DOUBLE* SD);
	void GetMinMax(DOUBLE* pVals, UINT cnt, OUT DOUBLE* minv, OUT DOUBLE* maxv);

public:
	afx_msg void OnBnClickedButtonGen();
	afx_msg void OnBnClickedButtonAvg();
	afx_msg void OnBnClickedButtonSd();
	afx_msg void OnBnClickedButtonMnx();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonReset();

public:
	CListBox m_lsbNums;
	BOOL m_bSet;
	UINT m_uCnt;
	CEdit m_editAvg;
	CEdit m_editSd;
	CEdit m_editMnx;

};
