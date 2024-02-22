
#pragma once


class MainDlg : public CDialogEx
{
public:
	MainDlg(CWnd* pParent = nullptr);

private:
	void UpdateOptCheckBoxStr();
	void UpdateHScrollBarVal();
	void UpdateVScrollBarVal();
	void UpdateTimerVal();
	void UpdateTimerElapsedVal();

private:
	void ScrollControl(UINT nSBCode, UINT nPos, CScrollBar& pScrollBar);
	void LogInternal(const char* functionName, const char* format, ...);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCSAMPLESOLUTION_1_DIALOG };
#endif

protected:
	virtual void DoDataExchange
	(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSub();

	afx_msg void OnRdBnClicked(UINT idx);
	afx_msg void OnCbChanged(UINT idx);

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStartTimer();
	afx_msg void OnBnClickedStopTimer();
	afx_msg void OnBnClickedResetTimer();

public:
	CComboBox m_combobox;

	CButton m_radio_active;
	CButton m_radio_hide;
	CButton m_radio_deactive;
	UINT m_radio_idx;

	CButton m_checkbox_opt1;
	CButton m_checkbox_opt2;
	CButton m_checkbox_opt3;
	std::vector<CButton*> m_vec_optcb;
	CEdit m_edit_opts;

	CListBox m_listbox_logs;

	UINT m_val;
	UINT m_elapsed;
	CButton m_btn_start;
	CButton m_btn_stop;
	UINT m_timerID;


	CScrollBar m_hscrollbar;
	CEdit m_edit_hscrollval;

	CScrollBar m_vscrollbar;
	CEdit m_edit_vscrollval;
};