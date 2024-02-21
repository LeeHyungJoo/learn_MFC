
#pragma once


class CMFCSampleSolution1Dlg : public CDialogEx
{
public:
	CMFCSampleSolution1Dlg(CWnd* pParent = nullptr);

private:
	void UpdateOptCheckBoxStr();
	void UpdateHScrollBarVal();
	void UpdateVScrollBarVal();

private:
	void ScrollControl(UINT nSBCode, UINT nPos, CScrollBar& pScrollBar);

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

	CStatic m_statictext_val;
	CEdit m_edit_interval;
	CButton m_btn_start;
	CButton m_btn_stop;
	CButton m_bt_reset;

	CScrollBar m_hscrollbar;
	CEdit m_edit_hscrollval;

	CScrollBar m_vscrollbar;
	CEdit m_edit_vscrollval;
};
