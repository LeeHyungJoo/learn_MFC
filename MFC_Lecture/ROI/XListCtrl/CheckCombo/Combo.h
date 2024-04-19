/****************************************************************************
*                                                                           *
* Combo.h -- ChkCombo class declaration                                     *
* This class allows to use Check Box as drop-down list items.               *
* Copyright (c) Timur Yunyaev. All rights reserved.                         *
*                    02.12.2020                                             *
*		The especial signals from this widget is:							*
*																			*
*	THIS_CHECKED - this signal will be sent every time when					*
*	user check(or uncheck) any items in drop down list.						*
*	The message send a 3 parameters:										*
*		- index- the linearly index item in a list (1.....)					*
*		- flag - checked state (true:false)									*
*		- control ID.														*
****************************************************************************/

#include "afxwin.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#define ITEM_CHK // uncomment this string if you want to use ItemData for save checked item state
#define MAX_SIZE_CHK 64
#define ARROW_W 20
#define THIS_CHECKED (WM_USER + 100)
#define CHECKCOMBOBOXCTRL_CLASSNAME    _T("CheckComboBox")  // Window class name

/*
	Don't forget set:
		Owner Draw - Variable
		Has String - True
*/

static UINT WM_CHECKCOMBO_ESCAPE = ::RegisterWindowMessage(_T("WM_CHECKCOMBO_ESCAPE"));
static UINT WM_CHECKCOMBO_COMPLETE = ::RegisterWindowMessage(_T("WM_CHECKCOMBO_COMPLETE"));

class ChkCombo : public CComboBox
{
public:
	ChkCombo(void);
	virtual ~ChkCombo(void);
	bool isEnable;
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	int SetCheck(INT nIndex, BOOL bFlag);
	bool GetCheck(INT nIndex);
	DWORD GetChecked();
	HDC p_dc;
	bool checked_arr[MAX_SIZE_CHK];
	void PreSubclassWindow();
	void RecalcText();
	BOOL pr_bTextUpdated;
	bool isFocus;
	bool pressed;
	static ChkCombo *pr_pComboBox;
	static WNDPROC pr_pWndProc;
	static WNDPROC pr_pWndProcMain;
	CString GetText();

protected:
	
	static  LRESULT ComboBoxListBoxProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	static  LRESULT ComboBoxProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	bool hovered;
	HWND pr_ListBox;
	CString pr_strText;
	BOOL pr_bItemHeightSet;
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage( MSG* pMsg);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnCtlColorListBox(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextLength(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDropDown();
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	DECLARE_MESSAGE_MAP()
};

