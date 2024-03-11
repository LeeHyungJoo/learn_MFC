
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
	virtual ~CAaronMathViewerDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AARONMATHVIEWER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	void ResetPicking();
	void ResetParamCoords();
	BOOL PickCoord(const CPoint& clientPnt);

public:
	BOOL IsPointInBoard(const CPoint& clientPnt) const;
	void UpdatePickCoords();
	void OnMethodRadioChanged(UINT ID);

	void ResetBoard(CPaintDC* dc);
	void DrawMethod(CPaintDC* dc);
	void DrawLine(CPaintDC* dc, const CPoint& start, const CPoint& end);
	void DrawDotLine(CPaintDC* dc, const CPoint& start, const CPoint& end);
	void DrawDotCircle(CPaintDC* dc, const CPoint& point);
	void DrawCircle(CPaintDC* dc, const CPoint& LT, const CPoint& RB);
	void DrawSpecificDotCircle(CPaintDC* dc, const CPoint& point);
	void DrawPolyLine(CPaintDC* dc, const CArray<CPoint>& points, INT64 startIdx, INT64 endIdx);
	void DrawPolyLines(CPaintDC* dc, const CArray<std::pair<POINT*, INT64>>& points, INT64 startIdx, INT64 endIdx);
	void DrawOthogonal(CPaintDC* dc);

	const CPoint ToOthogonalFromClient(const CPoint& client);
	const CPoint ToClientFromOthogonal(const CPoint& othogonal);

public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonRot();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonTestpick();
	//afx_msg void OnLbnDblclkListView();

public:
	UINT m_uMethodID;
	
	CArray<CPoint> m_vecPickedCoord;
	CArray<CPointDouble> m_vecDoubleCoord;
	CPtrArray m_vecCoordEdits;
	CMap<UINT, UINT, INT, INT> m_mPickedCoordCount;

	CArray<std::pair<POINT*, INT64>> m_points;
	INT m_iRotDegree;

	CStatic m_pcBoard;
	CStatic m_stCoord;

	CListBox m_lbxExpr;

	CArray<CPoint> m_vecParamCoord;		//TODO: Param ÁÂÇ¥·Î ÇÕÄ¥°Í (±¸Á¶Ã¼)
	CArray<BOOL> m_bExprDecimal;			//TODO: Param ÁÂÇ¥·Î ÇÕÄ¥°Í (±¸Á¶Ã¼)

	CEdit m_edtDegree;
	CButton m_btnRot;
	BOOL m_bFirstEnter;
	CEdit m_edtTestCoord;
};
