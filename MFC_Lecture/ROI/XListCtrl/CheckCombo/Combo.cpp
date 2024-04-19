#include "pch.h"
#include "Combo.h"
#include <WinUser.h>


static WNDPROC pr_pWndProcM = 0;
#pragma comment(lib, "uxtheme.lib")
ChkCombo* ChkCombo::pr_pComboBox;
WNDPROC ChkCombo::pr_pWndProc;
WNDPROC ChkCombo::pr_pWndProcMain;

BEGIN_MESSAGE_MAP(ChkCombo, CComboBox)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_CTLCOLORLISTBOX, OnCtlColorListBox)
	ON_MESSAGE(WM_GETTEXT, OnGetText)
	ON_MESSAGE(WM_GETTEXTLENGTH, OnGetTextLength)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropDown)
END_MESSAGE_MAP()


LRESULT ChkCombo::ComboBoxListBoxProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch (nMsg) {
		
		case WM_MOUSEMOVE :{
			int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);
			CRect my_Rec;
			::GetClientRect(hWnd, my_Rec);
			if(xPos > my_Rec.right || xPos < 0)
				return 1;
			if(yPos > my_Rec.bottom)
				return 1;
		}
		
		case WM_RBUTTONDOWN: {
			break;
		}

		case LB_GETCURSEL: {
			return -1;
		}

		case WM_CHAR: {
			if (wParam == VK_SPACE) {
				// Get the current selection
				INT nIndex = static_cast<INT>(CallWindowProcA(pr_pWndProc, hWnd, LB_GETCURSEL, wParam, lParam));
				CRect rcItem;
				::SendMessage(hWnd, LB_GETITEMRECT, nIndex, reinterpret_cast<LPARAM>(&rcItem));
				::InvalidateRect(hWnd, rcItem, FALSE);
				// Invert the check mark
				pr_pComboBox->SetCheck(nIndex, !pr_pComboBox->GetCheck(nIndex));
				// Notify that selection has changed
				pr_pComboBox->GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetWindowLongPtr(pr_pComboBox->m_hWnd, GWL_ID), CBN_SELCHANGE), (LPARAM)pr_pComboBox->m_hWnd);
				return 0;
			}
			break;
		}
		
		case WM_LBUTTONDBLCLK: {
			//TRACE("\n DC");
			return 0;
		}

		case WM_LBUTTONDOWN: {
			
			CRect rcClient;
			::GetClientRect(hWnd, rcClient);
			CPoint pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			if (PtInRect(rcClient, pt)) {
				INT nItemHeight = static_cast<INT>(::SendMessage(hWnd, LB_GETITEMHEIGHT, 0, 0));
				INT nTopIndex   = static_cast<INT>(::SendMessage(hWnd, LB_GETTOPINDEX, 0, 0));
				INT nIndex = nTopIndex + pt.y / nItemHeight;
				CRect rcItem;
				::SendMessage(hWnd, LB_GETITEMRECT, nIndex, reinterpret_cast<LPARAM>(&rcItem));
				if (PtInRect(rcItem, pt)) {
					::InvalidateRect(hWnd, rcItem, FALSE);
					pr_pComboBox->pressed = TRUE;
				    pr_pComboBox->SendMessage(WM_PAINT,0,0);
				}
				return 0;
			}
			break;
		}

		case WM_LBUTTONUP: {
			
			CRect rcClient;
			::GetClientRect(hWnd, rcClient);
			CPoint pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			if (PtInRect(rcClient, pt)) {
				INT nItemHeight = static_cast<INT>(::SendMessage(hWnd, LB_GETITEMHEIGHT, 0, 0));
				INT nTopIndex   = static_cast<INT>(::SendMessage(hWnd, LB_GETTOPINDEX, 0, 0));
				INT nIndex = nTopIndex + pt.y / nItemHeight;
				CRect rcItem;
				::InvalidateRect(hWnd, rcItem, FALSE);
				pr_pComboBox->SetCheck(nIndex, !pr_pComboBox->GetCheck(nIndex));
				pr_pComboBox->GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetWindowLongPtr(pr_pComboBox->m_hWnd, GWL_ID), CBN_SELCHANGE), (LPARAM)pr_pComboBox->m_hWnd);
				pr_pComboBox->pressed = FALSE;
				pr_pComboBox->SendMessage(WM_PAINT,0,0);
				::SendMessageW(hWnd, WM_PAINT, wParam, lParam);
				return 0;
			}
			
		}
		
	}
	return CallWindowProc(pr_pWndProc, hWnd, nMsg, wParam, lParam);
}


LRESULT ChkCombo::ComboBoxProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	/*
		When application is start close procedure u get this messages:
		WM_DESTROY
		WM_NCDESTROY
		WM_DELETEITEM
	*/

	//TRACE("\n ___ %d === %d", nMsg, GetTickCount());
	//if(nMsg == CB_GETDROPPEDSTATE)
	//	::EnableWindow(hWnd, FALSE);
	if(nMsg == CB_DELETESTRING && hWnd!=NULL) {
		#ifndef ITEM_CHK
			int index_for_remove = (int)wParam;
			ChkCombo *bx;
			bx = (ChkCombo*)CWnd::FromHandle(hWnd);
			if(index_for_remove < MAX_SIZE_CHK)
			{
				if(index_for_remove == 0) {
					memcpy(bx->checked_arr, &bx->checked_arr[1], MAX_SIZE_CHK-1);
				}
				if(index_for_remove == MAX_SIZE_CHK - 1){
					bx->checked_arr[index_for_remove] = false;
				}
				if (index_for_remove > 0 && index_for_remove < MAX_SIZE_CHK - 1) {
					memcpy(&bx->checked_arr[index_for_remove], &bx->checked_arr[index_for_remove + 1], MAX_SIZE_CHK - 1 - index_for_remove);
				}
			}
			bx->pr_bTextUpdated = FALSE;
			bx->RecalcText();
			bx->SendMessage(WM_PAINT,0,0);
		#endif
	}

	if(nMsg == WM_SETFOCUS  && hWnd!=NULL) {
		ChkCombo *bx;
		bx = (ChkCombo*)CWnd::FromHandle(hWnd);
		bx->isFocus = TRUE;
	}

	if(nMsg == WM_KILLFOCUS  && hWnd!=NULL) {
		ChkCombo *bx;
		bx = (ChkCombo*)CWnd::FromHandle(hWnd);
		bx->isFocus = FALSE;
	}
	if(nMsg == WM_ENABLE) {
		ChkCombo *bx;
		bx = (ChkCombo*)CWnd::FromHandle(hWnd);
		bx->isEnable = wParam;
		bx->SendMessage(WM_PAINT,0,0);
	}

	/*
		The main question: Why i get this message?????
		For save right visualization when app is close, 
		we don't need to processing WM_KILLFOCUS message
		when the recipient is the parent.
	*/
	if(nMsg == WM_KILLFOCUS) {
		ChkCombo *bx;
		bx = (ChkCombo*)CWnd::FromHandle(hWnd);
		if(bx->GetParent()->GetSafeHwnd() == (HWND)wParam) {
			return 0;
		}
	}
	return CallWindowProc(pr_pWndProcMain, hWnd, nMsg, wParam, lParam);
}


BOOL ChkCombo::PreTranslateMessage( MSG* pMsg)
{	
	//If i press some key  a-Z , 0-9 the default combobox set the item like a general in a main string. It's not good for us
	//And others button not good to.... so i will be processing only Tab and F4. 
	//But we will use this rule only if combo box is not dropped(don't see list).

	if(pMsg->message == WM_KEYUP || pMsg->message == WM_KEYDOWN) {
		if(pMsg->wParam != VK_TAB)  {
			if(pMsg->wParam != VK_F4)
				if(!GetDroppedState()) {
					return TRUE;
				}
		}
	}

	if(pMsg->message == WM_MOUSEFIRST)
		if( pMsg->lParam&&WM_MOUSEHOVER) {
			hovered = TRUE;
		}
	if (pMsg->message == WM_MOUSELEAVE) {
		hovered = FALSE;
	}

	return CComboBox::PreTranslateMessage( pMsg);
}

void ChkCombo::PreSubclassWindow()
{
	//pr_pWndProcM = (WNDPROC)GetWindowLong(m_hWnd, GWLP_WNDPROC);
	//SetWindowLong(m_hWnd, GWLP_WNDPROC, (LONG)ComboBoxProcM);

	pr_pWndProcMain = (WNDPROC)GetWindowLongPtr(m_hWnd, GWLP_WNDPROC);
	SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)ComboBoxProc);

	CComboBox::PreSubclassWindow();
}

ChkCombo::ChkCombo()
{
	pr_ListBox = 0;
	pr_bTextUpdated = FALSE;
	pr_bItemHeightSet = FALSE;
	hovered = FALSE;
	memset(checked_arr, FALSE, MAX_SIZE_CHK);
	isFocus = false;
	isEnable = true;
	pressed = false;
}


ChkCombo::~ChkCombo()
{
	return;
}


BOOL ChkCombo::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	dwStyle &= ~0xF;
	dwStyle |= CBS_DROPDOWNLIST;
	dwStyle |= CBS_OWNERDRAWVARIABLE;
	dwStyle |= CBS_HASSTRINGS;
	return CComboBox::Create(dwStyle, rect, pParentWnd, nID);
}

LRESULT ChkCombo::OnCtlColorListBox(WPARAM wParam, LPARAM lParam) 
{
	if (pr_ListBox == 0) {
		HWND hWnd = (HWND)lParam;
		if (hWnd != 0 && hWnd != m_hWnd) {
			pr_ListBox = hWnd;
			pr_pWndProc = (WNDPROC)GetWindowLongPtr(pr_ListBox, GWLP_WNDPROC);
			SetWindowLongPtr(pr_ListBox, GWLP_WNDPROC, (LONG_PTR)ComboBoxListBoxProc);
		}
	}
	return DefWindowProc(WM_CTLCOLORLISTBOX, wParam, lParam);
}

void DrawGradientRect( CDC *pDC, CRect &rect, COLORREF col_from, COLORREF col_to, bool vert_grad )
{
	TRIVERTEX        vert[2];
	GRADIENT_RECT    mesh;

	vert[0].x      = rect.left;
	vert[0].y      = rect.top;
	vert[0].Alpha  = 0x0000;
	vert[0].Blue   = GetBValue(col_from) << 8;
	vert[0].Green  = GetGValue(col_from) << 8;
	vert[0].Red    = GetRValue(col_from) << 8;

	vert[1].x      = rect.right;
	vert[1].y      = rect.bottom; 
	vert[1].Alpha  = 0x0000;
	vert[1].Blue   = GetBValue(col_to) << 8;
	vert[1].Green  = GetGValue(col_to) << 8;
	vert[1].Red    = GetRValue(col_to) << 8;

	mesh.UpperLeft  = 0;
	mesh.LowerRight = 1;
#if _MSC_VER >= 1300  // only VS7 and above has GradientFill as a pDC member
	pDC->GradientFill( vert, 2, &mesh, 1, vert_grad ? GRADIENT_FILL_RECT_V : 
		GRADIENT_FILL_RECT_H );
#else
	GradientFill( pDC->m_hDC, vert, 2, &mesh, 1, vert_grad ? GRADIENT_FILL_RECT_V : 
		GRADIENT_FILL_RECT_H );
#endif
}

void ChkCombo::OnPaint()
{
	HTHEME htheme = OpenThemeData(m_hWnd, WC_COMBOBOXW );
	CPaintDC dc(this);
	dc.SelectObject(GetFont());
	CRect rect;
	GetClientRect(&rect);
	rect.right-=1;
	rect.bottom-=1;
	CBrush newBrush;

	if(isEnable) {
		if(!hovered)
			DrawGradientRect(&dc, rect, RGB(240,240,240), RGB(229,229,229), true);
		else 
			DrawGradientRect(&dc, rect, RGB(236,244,252), RGB(220,236,252), true);
	} else {
		HBRUSH disable_brush = CreateSolidBrush(RGB(239, 239, 239));
		FillRect(dc,&rect ,disable_brush);
	}

	CPen penBorder;
	if(isEnable) {
		if(!hovered)
			penBorder.CreatePen(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));
		else
			penBorder.CreatePen(PS_SOLID, 0, RGB(126, 180, 234));
	} else {
		penBorder.CreatePen(PS_SOLID, 0, RGB(217, 217, 217));
	}
	dc.SelectObject(&penBorder);

	dc.MoveTo(0,0);
	dc.LineTo(rect.right, 0);

	dc.MoveTo(rect.right, 0);
	dc.LineTo(rect.right, rect.bottom);

	dc.MoveTo(rect.right, rect.bottom);
	dc.LineTo(0, rect.bottom);

	dc.MoveTo(0,rect.top);
	dc.LineTo(rect.top, rect.bottom+1);


	if(isFocus){
		RECT r;
		memcpy(&r, &rect, sizeof(RECT));
		r.right-=(ARROW_W);
		r.left+=2;
		r.bottom-=2;
		r.top+=3;
		DrawFocusRect(dc, &r);
	}



//	UINT nState = DFCS_SCROLLCOMBOBOX;
//	nState |= DFCS_INACTIVE;
//	nState |= DFCS_TRANSPARENT;

	//= DFCS_BUTTONCHECK;
	//nState |= DFCS_CHECKED;
	CRect button_rect;
	button_rect.left = rect.right - ARROW_W;
	button_rect.right = rect.right;
	button_rect.top = rect.top;
	button_rect.bottom = rect.bottom;
//	CPen penText;
//	if(isEnable)
//		penText.CreatePen(PS_SOLID, 3, GetSysColor(COLOR_BTNTEXT));
//	else
//		penText.CreatePen(PS_SOLID, 3, GetSysColor(COLOR_GRAYTEXT));
		
//	dc.SelectObject(&penText);


	SetBkMode (dc, TRANSPARENT);
	/*
	if(!hovered)
		SetBkColor(dc, RGB(234,234,234));
	else
		SetBkColor(dc, RGB(233,243,253));
	*/
	if(isEnable)
		SetTextColor(dc, GetSysColor(COLOR_BTNTEXT));
	else
		SetTextColor(dc, GetSysColor(COLOR_GRAYTEXT));
	rect.left+=2;
	rect.right-=ARROW_W;
	rect.bottom+=1;
	rect.top+=1;
	DrawText(dc, ' ' + pr_strText, pr_strText.GetLength() + 1, &rect, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);


	int state_c;
	if(hovered)
		state_c = CBXS_HOT;
	else
		state_c = CBXS_NORMAL;
	if(!isEnable)
		state_c = CBXS_DISABLED;
	CRect rec_z;
	button_rect.left+=4;
	button_rect.right+=1;
	button_rect.top+=1;
	button_rect.bottom+=1;
	memcpy(&rec_z, &button_rect, sizeof(CRect));
	rec_z.left+=5;
	rec_z.right-=5;
	rec_z.top+=5;
	rec_z.bottom-=5;
	
	DrawThemeBackground(htheme, dc, CP_DROPDOWNBUTTON, state_c, &button_rect, &rec_z);



	CloseThemeData(htheme);
	DeleteObject(penBorder);

}

void ChkCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	TRACE("\n DRAW %d", GetTickCount());
	bool H = GetDroppedState();
	HDC dc = lpDrawItemStruct->hDC;
	
	
	//TRACE("\n ID = %d", lpDrawItemStruct->itemID);
	CRect rcBitmap = lpDrawItemStruct->rcItem;
	CRect rcText   = lpDrawItemStruct->rcItem;
	CString strText;
	INT nCheck = 0;
	if ((LONG)lpDrawItemStruct->itemID < 0) {
		RecalcText();
		strText = pr_strText;
		Invalidate();
		return;
	} else {
		GetLBText(lpDrawItemStruct->itemID, strText);
		#ifdef ITEM_CHK
			nCheck = 1 + (GetItemData(lpDrawItemStruct->itemID) != 0);
		#else
			nCheck = 1 + checked_arr[lpDrawItemStruct->itemID];
		#endif
		TEXTMETRIC metrics;
		GetTextMetrics(dc, &metrics);

		rcBitmap.left    = 0;
		rcBitmap.right   = rcBitmap.left + metrics.tmHeight + metrics.tmExternalLeading + 6;
		rcBitmap.top    += 1;
		rcBitmap.bottom -= 1;

		rcText.left = rcBitmap.right;
	}

	if (nCheck > 0) {
		SetBkColor(dc, GetSysColor(COLOR_WINDOW));
		SetTextColor(dc, GetSysColor(COLOR_WINDOWTEXT));

		UINT nState = DFCS_BUTTONCHECK;
	
		if (nCheck > 1)
			nState |= DFCS_CHECKED;
		if (lpDrawItemStruct->itemState & ODS_SELECTED) // Only selected item can have DFCS_PUSHED attribute
			if(pressed)
				nState |= DFCS_PUSHED;
		//Draw checkbox
		DrawFrameControl(dc, rcBitmap, DFC_BUTTON, nState);
	}

	if (lpDrawItemStruct->itemState & ODS_SELECTED) {
		SetBkColor(dc, GetSysColor(COLOR_HIGHLIGHT));
		SetTextColor(dc, GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else {
		if ((LONG)lpDrawItemStruct->itemID < 0)
			SetBkColor(dc, GetSysColor(COLOR_MENU));
		else
			SetBkColor(dc, GetSysColor(COLOR_WINDOW));
		SetTextColor(dc, GetSysColor(COLOR_WINDOWTEXT));
	}

	// Erase and draw
	ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rcText, 0, 0, 0);
	DrawText(dc, ' ' + strText, strText.GetLength() + 1, &rcText, DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	if ((lpDrawItemStruct->itemState & (ODS_FOCUS|ODS_SELECTED)) == (ODS_FOCUS|ODS_SELECTED))
		DrawFocusRect(dc, &rcText);
	RecalcText();
}


void ChkCombo::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	CClientDC dc(this);
	CFont *pFont = dc.SelectObject(GetFont());

	if (pFont != 0) {
		TEXTMETRIC metrics;
		dc.GetTextMetrics(&metrics);
		lpMeasureItemStruct->itemHeight = metrics.tmHeight + metrics.tmExternalLeading;
		lpMeasureItemStruct->itemHeight += 2;
		if (!pr_bItemHeightSet) {
			pr_bItemHeightSet = TRUE;
			SetItemHeight(-1, lpMeasureItemStruct->itemHeight);
		}

		dc.SelectObject(pFont);
	}
}


void ChkCombo::OnDropDown() 
{
	pr_pComboBox = this;
}


void ChkCombo::OnKillFocus(CWnd* pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	if (pNewWnd->GetSafeHwnd() != pr_ListBox)
	{
		CWnd *pWnd = GetParent();
		if (pWnd)
			pWnd->SendMessage(WM_CHECKCOMBO_COMPLETE, 0, 0);
	}
}

LRESULT ChkCombo::OnGetText(WPARAM wParam, LPARAM lParam)
{
	RecalcText();
	if (lParam == 0)
		return 0;
	lstrcpyn((LPWSTR)lParam, pr_strText, (INT)wParam);
	return pr_strText.GetLength();
}

LRESULT ChkCombo::OnGetTextLength(WPARAM, LPARAM)
{
	RecalcText();
	return pr_strText.GetLength();
}


void ChkCombo::RecalcText()
{
	if (!pr_bTextUpdated) {
		CString strText;

		// Get the list count
		INT nCount    = GetCount();

		// Get the list separator
		TCHAR szBuffer[10] = {0};
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLIST, szBuffer, sizeof(szBuffer));
		CString strSeparator = szBuffer;
		if (strSeparator.GetLength() == 0)
			strSeparator = ';';
		strSeparator.TrimRight();
		strSeparator += ' ';
		for (INT i = 0; i < nCount; i++) {
#ifdef ITEM_CHK
			if (GetItemData(i)) {
#else
			if(checked_arr[i]) {
#endif
				CString strItem;
				GetLBText(i, strItem);
				if (!strText.IsEmpty())
					strText += strSeparator;
				strText += strItem;
			}
		}
		pr_strText = strText;

		pr_bTextUpdated = TRUE;
	}
}

int ChkCombo::SetCheck(INT nIndex, BOOL bFlag)
{
#ifdef ITEM_CHK
	INT nResult = SetItemData(nIndex, bFlag);
	if (nResult < 0)
		return nResult;
#else
	if(nIndex >= MAX_SIZE_CHK)
		return -1;
	checked_arr[nIndex] = bFlag;
#endif
	// Signal that the text need updating
	pr_bTextUpdated = FALSE;
	WPARAM this_param = MAKEWPARAM(nIndex,bFlag);
	LPARAM this_ID = (LPARAM)GetDlgCtrlID();
	GetParent()->SendMessageW(THIS_CHECKED, this_param, this_ID);
	// Redraw the window
	Invalidate(FALSE);
	RecalcText();
	return 1;
}

bool ChkCombo::GetCheck(INT nIndex)
{
#ifdef ITEM_CHK
	return GetItemData(nIndex);
#else
	if(nIndex >= MAX_SIZE_CHK)
		return FALSE;
	return checked_arr[nIndex];
#endif
}


DWORD ChkCombo::GetChecked()
{
#ifndef ITEM_CHK
	DWORD TMP = 0;
	int len = 32;
	if(len > MAX_SIZE_CHK) len = MAX_SIZE_CHK;
	for(int i = len-1; i >= 0; i--) {
		TMP<<=1;
		if(checked_arr[i])
			TMP^=(checked_arr[i] != 0);
			//TMP^=checked_arr[i];
	}
	return TMP;
#else
	return 0;
#endif
}

CString ChkCombo::GetText()
{
	return pr_strText;
}

