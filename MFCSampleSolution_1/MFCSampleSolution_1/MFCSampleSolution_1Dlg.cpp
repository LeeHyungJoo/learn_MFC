
// MFCSampleSolution_1Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCSampleSolution_1.h"
#include "MFCSampleSolution_1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMFCSampleSolution1Dlg::CMFCSampleSolution1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCSAMPLESOLUTION_1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCSampleSolution1Dlg::UpdateOptCheckBoxStr()
{
	CString optnames;

	for (const auto cb : m_vec_optcb)
	{
		if (!cb->GetCheck())
			continue;

		CString optname;
		cb->GetWindowTextW(optname);

		if (!optnames.IsEmpty()) 
			optnames.AppendChar(',');
		optnames.Append(optname);
	}

	SetDlgItemText(IDC_EDIT1, optnames);
}

void CMFCSampleSolution1Dlg::ScrollControl(UINT nSBCode, UINT nPos, CScrollBar & pScrollBar)
{
	int delta = 0;

	switch (nSBCode)
	{
	case SB_THUMBTRACK:
		pScrollBar.SetScrollPos(nPos);
		break;
	case SB_LINEUP :	/* = SB_LINELEFT*/
		delta -= 4;
		break;
	case SB_LINEDOWN:	/* = SB_LINERIGHT*/
		delta += 4;
		break;
	case SB_PAGEUP:		/* = SB_PAGELEFT*/
		delta -= 16;
		break;
	case SB_PAGEDOWN:	/* = SB_PAGERIGHT*/
		delta += 16;
		break;
	}

	if (delta == 0)
		return;

	pScrollBar.SetScrollPos(pScrollBar.GetScrollPos() + delta);
}

void CMFCSampleSolution1Dlg::UpdateHScrollBarVal()
{
	CString val;
	val.Format(TEXT("%d"), m_hscrollbar.GetScrollPos());

	SetDlgItemText(IDC_EDIT3, val);
}

void CMFCSampleSolution1Dlg::UpdateVScrollBarVal()
{
	CString val;
	val.Format(TEXT("%d"), m_vscrollbar.GetScrollPos());

	SetDlgItemText(IDC_EDIT4, val);
}

void CMFCSampleSolution1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	TRACE(L"DoDataExchange");
	DDX_Control(pDX, IDC_COMBO2, m_combobox);

	DDX_Control(pDX, IDC_RADIO_ACTIVE, m_radio_active);
	DDX_Control(pDX, IDC_RADIO_HIDE, m_radio_hide);
	DDX_Control(pDX, IDC_RADIO_DEACTIVE, m_radio_deactive);

	DDX_Control(pDX, IDC_CHECK1, m_checkbox_opt1);
	DDX_Control(pDX, IDC_CHECK2, m_checkbox_opt2);
	DDX_Control(pDX, IDC_CHECK3, m_checkbox_opt3);
	DDX_Control(pDX, IDC_EDIT1, m_edit_opts);

	DDX_Control(pDX, IDC_LIST1, m_listbox_logs);

	DDX_Control(pDX, IDC_TEXT, m_statictext_val);
	DDX_Control(pDX, IDC_BUTTON1, m_btn_start);
	DDX_Control(pDX, IDC_BUTTON2, m_btn_stop);
	DDX_Control(pDX, IDC_BUTTON3, m_bt_reset);
	DDX_Control(pDX, IDC_EDIT2, m_edit_interval);

	DDX_Control(pDX, IDC_SCROLLBAR1, m_hscrollbar);
	DDX_Control(pDX, IDC_EDIT3, m_edit_hscrollval);
	DDX_Control(pDX, IDC_SCROLLBAR2, m_vscrollbar);
	DDX_Control(pDX, IDC_EDIT4, m_edit_vscrollval);

}

BEGIN_MESSAGE_MAP(CMFCSampleSolution1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMFCSampleSolution1Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CMFCSampleSolution1Dlg::OnBnClickedSub)
	ON_COMMAND_RANGE(IDC_RADIO_ACTIVE, IDC_RADIO_DEACTIVE, OnRdBnClicked)
	ON_COMMAND_RANGE(IDC_CHECK1, IDC_CHECK3, OnCbChanged)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CMFCSampleSolution1Dlg message handlers

BOOL CMFCSampleSolution1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_combobox.AddString(TEXT("default"));
	m_combobox.AddString(TEXT("foo"));
	m_combobox.AddString(TEXT("boo"));
	m_combobox.SetCurSel(0);

	m_radio_idx = 0U;
	m_radio_active.SetCheck(1);

	m_vec_optcb.clear();
	m_vec_optcb.push_back(&m_checkbox_opt1);
	m_vec_optcb.push_back(&m_checkbox_opt2);
	m_vec_optcb.push_back(&m_checkbox_opt3);

	m_hscrollbar.SetScrollRange(0, 255, FALSE);
	m_hscrollbar.SetScrollPos(0);
	m_vscrollbar.SetScrollRange(0, 255, FALSE);
	m_vscrollbar.SetScrollPos(0);

	//Update value
	UpdateOptCheckBoxStr();
	UpdateHScrollBarVal();
	UpdateVScrollBarVal();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCSampleSolution1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCSampleSolution1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCSampleSolution1Dlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CMFCSampleSolution1Dlg::OnBnClickedSub()
{
	// TODO: Add your control notification handler code here
}

void CMFCSampleSolution1Dlg::OnRdBnClicked(UINT idx)
{
	switch (idx)
	{
	case IDC_RADIO_ACTIVE:
		for (const auto cb : m_vec_optcb)
		{
			cb->ShowWindow(SW_SHOWNORMAL);
			cb->EnableWindow(TRUE);
		}
		m_edit_opts.ShowWindow(SW_SHOWNORMAL);
		m_edit_opts.EnableWindow(TRUE);
		break;
	case IDC_RADIO_HIDE:
		for (const auto cb : m_vec_optcb)
		{
			cb->ShowWindow(SW_HIDE);
		}
		m_edit_opts.ShowWindow(SW_HIDE);
		break;
	case IDC_RADIO_DEACTIVE:
		for (const auto cb : m_vec_optcb)
		{
			cb->ShowWindow(SW_SHOWNORMAL);
			cb->EnableWindow(FALSE);
		}
		m_edit_opts.ShowWindow(SW_SHOWNORMAL);
		m_edit_opts.EnableWindow(FALSE);
		break;
	default:
		TRACE(L"INVALID IDC_ID\n");
		break;
	}
}

void CMFCSampleSolution1Dlg::OnCbChanged(UINT idx)
{
	UpdateOptCheckBoxStr();
}


void CMFCSampleSolution1Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != nullptr)
	{
		if (pScrollBar->GetSafeHwnd() == m_hscrollbar.GetSafeHwnd())
		{
			ScrollControl(nSBCode, nPos, *pScrollBar);
			UpdateHScrollBarVal();
		}
	}
	else
	{
		CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}


void CMFCSampleSolution1Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != nullptr)
	{
		if (pScrollBar->GetSafeHwnd() == m_vscrollbar.GetSafeHwnd())
		{
			ScrollControl(nSBCode, nPos, *pScrollBar);
			UpdateVScrollBarVal();
		}
	}
	else
	{
		CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}
