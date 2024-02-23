#pragma once

class SubDlg;
class MainDlg : public CDialogEx
{
public:
	MainDlg(CWnd* pParent = nullptr);

protected:
	virtual void DoDataExchange
	(CDataExchange* pDX);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	void UpdateOptCheckBoxStr();
	void UpdateTimerVal();
	void UpdateTimerElapsedVal();
	void UpdateHScrollBarVal();
	void UpdateVScrollBarVal();

private:
	void ScrollControl(UINT nSBCode, UINT nPos, CScrollBar& pScrollBar);
	void LogInternal(const char* functionName, const char* format, ...);

public:
	void MakeDTO(OUT DTO* dto);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCSAMPLESOLUTION_1_DIALOG };
#endif

public:
	afx_msg void OnSelchangeCombo();
	afx_msg void OnRdBnClicked(UINT idx);
	afx_msg void OnCbChanged(UINT idx);

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStartTimer();
	afx_msg void OnBnClickedStopTimer();
	afx_msg void OnBnClickedResetTimer();

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSub();
	afx_msg void OnDestroy();

public:
	CComboBox m_cmbx;
	CButton   m_btSave;
	CButton   m_btDelete;

	CButton m_rbtActive;
	CButton m_rbtHide;
	CButton m_rbtDeactive;
	UINT	m_urbtActiveIdx;

	CButton m_cbxOpt1;
	CButton m_cbxOpt2;
	CButton m_cbxOpt3;
	std::vector<CButton*> m_vcbxOpt;
	CEdit	m_edtOpt;

	CListBox m_lstbLog;

	UINT	m_uCnt;
	UINT	m_uElapsed;
	CButton m_btStartTimer;
	CButton m_btStopTimer;
	UINT	m_uTimerID;
	BOOL	m_bTimerRun;

	CScrollBar m_hsb;
	CEdit	m_edtHsb;
	CScrollBar m_vsb;
	CEdit	m_edtVsb;

	SubDlg* m_pSubDlg;
	afx_msg void OnBnClickedButtonSave();
};