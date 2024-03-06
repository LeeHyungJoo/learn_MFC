
#pragma once

struct CPointDouble
{
	CPointDouble() : x(0), y(0) {};
	CPointDouble(const CPoint& point) : x(point.x), y(point.y) {}
	double x;
	double y;
};

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
	void ResetParamCoords();

public:
	BOOL IsScreenPointInRect(const CPoint& screenPoint, const CRect& wRect) const;
	void UpdatePickCoords();
	void OnMethodRadioChanged(UINT ID);

	void ResetBoard();
	void DrawMethod();
	void DrawLine(const CPoint& start, const CPoint& end);
	void DrawDotLine(const CPoint& start, const CPoint& end);
	void DrawDotCircle(const CPoint& point);
	void DrawSpecificDotCircle(const CPoint& point);
	void DrawPolyLine(const CArray<CPoint>& points, INT startIdx, INT endIdx);

	const CPoint ToOthogonalFromClient(const CPoint& client);
	const CPoint ToClientFromOthogonal(const CPoint& othogonal);
	void DrawOthogonal();

public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonRot();
	afx_msg void OnBnClickedButtonReset();
	//afx_msg void OnLbnDblclkListView();

public:
	UINT m_uMethodID;
	
	CArray<CPoint> m_vecPickedCoord;
	CArray<CPointDouble> m_vecDoubleCoord;	//TODO: Param ÁÂÇ¥·Î ÇÕÄ¥°Í (±¸Á¶Ã¼)
	CArray<CPoint> m_vecParamCoord;		//TODO: Param ÁÂÇ¥·Î ÇÕÄ¥°Í (±¸Á¶Ã¼)
	CPtrArray m_vecCoordEdits;
	CMap<UINT, UINT, INT, INT> m_mPickedCoordCount;

	CStatic m_pcBoard;
	CStatic m_stCoord;

	CListBox m_lbxExpr;
	CArray<BOOL> m_bExprDecimal;			//TODO: Param ÁÂÇ¥·Î ÇÕÄ¥°Í (±¸Á¶Ã¼)

	CEdit m_edtDegree;
	CButton m_btnRot;
};
