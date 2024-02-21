
// MFCSampleSolution_1Dlg.h : header file
//

#pragma once


// CMFCSampleSolution1Dlg dialog
class CMFCSampleSolution1Dlg : public CDialogEx
{
// Construction
public:
	CMFCSampleSolution1Dlg(CWnd* pParent = nullptr);	// standard constructor

private:
	void UpdateOptionCheckBoxStr();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCSAMPLESOLUTION_1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange
	(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnRdBnClicked(UINT idx);
	afx_msg void OnCbChanged(UINT idx);

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
	CEdit m_vscrollbar_val;

};
