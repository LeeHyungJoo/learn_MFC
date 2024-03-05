
#pragma once

struct CPointDouble
{
	CPointDouble(CPoint point) : x(point.x), y(point.y) {}
	CPointDouble(double x, double y) :x(x), y(y) {}
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
	void DrawPolyLine(const std::vector<CPoint>& points, INT startIdx, INT endIdx);

	const CPoint ToOthogonalFromClient(const CPoint& client);
	const CPoint ToClientFromOthogonal(const CPoint& othogonal);
	void DrawOthogonal();

public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonReset();
	//afx_msg void OnLbnDblclkListView();

public:
	UINT m_uMethodID;
	

	std::vector<CPoint> m_vecPickedCoord;
	std::vector<CPointDouble> m_vecDoubleCoord;	//TODO: Param ÁÂÇ¥·Î ÇÕÄ¥°Í (±¸Á¶Ã¼)
	std::vector<CPoint> m_vecParamCoord;		//TODO: Param ÁÂÇ¥·Î ÇÕÄ¥°Í (±¸Á¶Ã¼)
	std::vector<CEdit*> m_vecCoordEdits;
	std::unordered_map<UINT, INT> m_mPickedCoordCount;

	CStatic m_pcBoard;
	CStatic m_stCoord;

	CListBox m_lbxExpr;
	std::vector<BOOL> m_bExprDecimal;			//TODO: Param ÁÂÇ¥·Î ÇÕÄ¥°Í (±¸Á¶Ã¼)

	CEdit m_edtDegree;
	CButton m_btnRot;
	afx_msg void OnBnClickedButtonRot();
};
