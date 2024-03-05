
#pragma once

class CAaronMathViewerDlg : public CDialogEx
{
public:
	CAaronMathViewerDlg(CWnd* pParent = nullptr);	// standard constructor

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AARONMATHVIEWER_DIALOG };
#endif

private:
	void TESTPICKCOORDS();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	void ResetPicking();

public:
	BOOL IsScreenPointInRect(const CPoint& screenPoint, const CRect& wRect) const;
	void OnMethodRadioChanged(UINT ID);
	void UpdatePickCoords();


public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnLbnDblclkListView();

public:
	UINT m_lastMethodRadioID;
	std::vector<CPoint> m_vecCoord;
	std::vector<CEdit*> m_vecCoordEdits;
	std::unordered_map<UINT, INT> m_mPickedCoordCount;

	CStatic m_pcBoard;
	CStatic m_stCoord;

	CListBox m_lbxExpr;
	std::vector<BOOL> m_bExprDecimal;
};
