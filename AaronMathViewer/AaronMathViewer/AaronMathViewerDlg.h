
#pragma once

class CAaronMathViewerDlg : public CDialogEx
{
public:
	CAaronMathViewerDlg(CWnd* pParent = nullptr);	// standard constructor

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AARONMATHVIEWER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	BOOL IsScreenPointInRect(const CPoint& screenPoint, const CRect& wRect) const;
	void OnMethodRadioChanged(UINT ID);

public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

public:
	UINT m_lastMethodRadioIdx;
	std::vector<CPoint> m_vecCoords;
	CStatic m_pcBoard;
	CStatic m_stCoord;
};
