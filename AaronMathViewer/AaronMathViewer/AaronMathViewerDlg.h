
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
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
