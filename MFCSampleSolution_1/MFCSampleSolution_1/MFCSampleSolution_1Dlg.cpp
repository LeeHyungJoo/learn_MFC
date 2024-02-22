#include "pch.h"
#include "framework.h"
#include "MFCSampleSolution_1.h"
#include "MFCSampleSolution_1Dlg.h"
#include "afxdialogex.h"
#include "SubDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define LOG(msg, ...) LogInternal(__FUNCTION__, msg, ##__VA_ARGS__)

MainDlg::MainDlg(CWnd* pParent)
	: CDialogEx(IDD_MFCSAMPLESOLUTION_1_DIALOG, pParent)
	, m_timerID(0U)
	, m_elapsed(100U)
	, m_val(0)
	, m_b_timerrun(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO2, m_combobox);

	DDX_Control(pDX, IDC_RADIO_ACTIVE, m_radio_active);
	DDX_Control(pDX, IDC_RADIO_HIDE, m_radio_hide);
	DDX_Control(pDX, IDC_RADIO_DEACTIVE, m_radio_deactive);

	DDX_Control(pDX, IDC_CHECK1, m_checkbox_opt1);
	DDX_Control(pDX, IDC_CHECK2, m_checkbox_opt2);
	DDX_Control(pDX, IDC_CHECK3, m_checkbox_opt3);
	DDX_Control(pDX, IDC_EDIT1, m_edit_opts);

	DDX_Control(pDX, IDC_LIST1, m_listbox_logs);

	DDX_Text(pDX, IDC_TEXT, m_val);
	DDX_Text(pDX, IDC_EDIT2, m_elapsed);

	DDX_Control(pDX, IDC_SCROLLBAR1, m_hscrollbar);
	DDX_Control(pDX, IDC_EDIT3, m_edit_hscrollval);
	DDX_Control(pDX, IDC_SCROLLBAR2, m_vscrollbar);
	DDX_Control(pDX, IDC_EDIT4, m_edit_vscrollval);

	DDX_Control(pDX, IDC_BUTTON1, m_btn_start);
	DDX_Control(pDX, IDC_BUTTON2, m_btn_stop);
}

BOOL MainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_combobox.AddString(TEXT("default"));
	m_combobox.AddString(TEXT("foo"));
	m_combobox.AddString(TEXT("boo"));
	m_combobox.SetCurSel(0);

	m_radio_active.SetCheck(1);

	m_vec_optcb.clear();
	m_vec_optcb.push_back(&m_checkbox_opt1);
	m_vec_optcb.push_back(&m_checkbox_opt2);
	m_vec_optcb.push_back(&m_checkbox_opt3);

	m_hscrollbar.SetScrollRange(0, 255, FALSE);
	m_hscrollbar.SetScrollPos(0);
	m_vscrollbar.SetScrollRange(0, 255, FALSE);
	m_vscrollbar.SetScrollPos(0);

	UpdateOptCheckBoxStr();
	UpdateHScrollBarVal();
	UpdateVScrollBarVal();
	UpdateTimerVal();
	UpdateTimerElapsedVal();

	LOG("--Initialize Main Dlg");
	return TRUE;
}

void MainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR MainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_MESSAGE_MAP(MainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_TIMER()

	ON_COMMAND_RANGE(IDC_RADIO_ACTIVE, IDC_RADIO_DEACTIVE, OnRdBnClicked)
	ON_COMMAND_RANGE(IDC_CHECK1, IDC_CHECK3, OnCbChanged)

	ON_BN_CLICKED(IDCANCEL, &MainDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDSUB, &MainDlg::OnBnClickedSub)
	ON_BN_CLICKED(IDC_BUTTON1, &MainDlg::OnBnClickedStartTimer)
	ON_BN_CLICKED(IDC_BUTTON2, &MainDlg::OnBnClickedStopTimer)
	ON_BN_CLICKED(IDC_BUTTON3, &MainDlg::OnBnClickedResetTimer)

	ON_CBN_SELCHANGE(IDC_COMBO2, &MainDlg::OnSelchangeCombo)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void MainDlg::UpdateOptCheckBoxStr()
{
	CString optnames;

	for (const auto cb : m_vec_optcb)
	{
		if (!cb->GetCheck())
			continue;

		CString optname;
		cb->GetWindowTextW(optname);

		if (!optnames.IsEmpty()) 
			optnames.Append(TEXT(", "));
		optnames.Append(optname);
	}

	SetDlgItemText(IDC_EDIT1, optnames);
}

void MainDlg::UpdateTimerVal()
{
	CString val;
	val.Format(TEXT("%d"), m_val);

	SetDlgItemText(IDC_TEXT, val);
}

void MainDlg::UpdateTimerElapsedVal()
{
	CString val;
	val.Format(TEXT("%d"), m_elapsed);

	SetDlgItemText(IDC_EDIT2, val);
}

void MainDlg::UpdateHScrollBarVal()
{
	CString val;
	val.Format(TEXT("%d"), m_hscrollbar.GetScrollPos());

	SetDlgItemText(IDC_EDIT3, val);
}

void MainDlg::UpdateVScrollBarVal()
{
	CString val;
	val.Format(TEXT("%d"), m_vscrollbar.GetScrollPos());

	SetDlgItemText(IDC_EDIT4, val);
}

void MainDlg::ScrollControl(UINT nSBCode, UINT nPos, CScrollBar & pScrollBar)
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

void MainDlg::LogInternal(const char* functionName, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	
	std::string s;
	int len = vsnprintf(nullptr, 0, format, args);
	s.resize(len + 1);
	vsnprintf(&s[0], len + 1, format, args);
	s.resize(len);
	
	CString logmsg;
	//logmsg.Format(L"%s - %s", (CString)functionName, CString(s.c_str()));
	logmsg.Format(L"%s", CString(s.c_str()));
	m_listbox_logs.AddString(logmsg);

	va_end(args);
}

void MainDlg::MakeDTO(OUT DTO * dto)
{
	CString cbs;
	m_combobox.GetLBText(m_combobox.GetCurSel(), cbs);
	dto->combo = cbs;

	CString rbs;
	if (m_radio_active.GetCheck())
		rbs = "active";
	else if (m_radio_deactive.GetCheck())
		rbs = "deactive";
	else if (m_radio_hide.GetCheck())
		rbs = "hide";
	dto->opttype = rbs;

	CString opts;
	m_edit_opts.GetWindowText(opts);
	dto->opt = opts;

	dto->val = m_val;
	dto->elapse = m_elapsed;

	dto->hscroll_val = m_hscrollbar.GetScrollPos();
	dto->vscroll_val = m_vscrollbar.GetScrollPos();
}

void MainDlg::OnSelchangeCombo()
{
	CString s;
	m_combobox.GetLBText(m_combobox.GetCurSel(), s);

	LOG("ComboBox - %d, %ls", m_combobox.GetCurSel(), s);
}

void MainDlg::OnRdBnClicked(UINT idx)
{
	if (m_radio_last_idx == idx)
		return;

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

		LOG("RadioBtn - Active");
		break;
	case IDC_RADIO_HIDE:
		for (const auto cb : m_vec_optcb)
		{
			cb->ShowWindow(SW_HIDE);
		}
		m_edit_opts.ShowWindow(SW_HIDE);

		LOG("RadioBtn - Hide");
		break;
	case IDC_RADIO_DEACTIVE:
		for (const auto cb : m_vec_optcb)
		{
			cb->ShowWindow(SW_SHOWNORMAL);
			cb->EnableWindow(FALSE);
		}
		m_edit_opts.ShowWindow(SW_SHOWNORMAL);
		m_edit_opts.EnableWindow(FALSE);

		LOG("RadioBtn - Deactive");
		break;
	}

	m_radio_last_idx = idx;
}

void MainDlg::OnCbChanged(UINT idx)
{
	UpdateOptCheckBoxStr();

	CString s;
	m_edit_opts.GetWindowText(s);

	LOG("CheckBox - %ls", s.IsEmpty() ? L"n/a" : s);
}

void MainDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != nullptr)
	{
		if (pScrollBar->GetSafeHwnd() == m_hscrollbar.GetSafeHwnd())
		{
			ScrollControl(nSBCode, nPos, *pScrollBar);
			UpdateHScrollBarVal();

			if(nSBCode == SB_ENDSCROLL) 
				LOG("Horizontal Scroll - pos : %d", pScrollBar->GetScrollPos());
		}
	}
	else
	{
		CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}

void MainDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != nullptr)
	{
		if (pScrollBar->GetSafeHwnd() == m_vscrollbar.GetSafeHwnd())
		{
			ScrollControl(nSBCode, nPos, *pScrollBar);
			UpdateVScrollBarVal();

			if (nSBCode == SB_ENDSCROLL) 
				LOG("Vertical Scroll - pos : %d", pScrollBar->GetScrollPos());
		}
	}
	else
	{
		CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}

void MainDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	if (m_timerID == nIDEvent)
	{
		m_val += 1;
		UpdateTimerVal();
	}
}

void MainDlg::OnBnClickedStartTimer()
{
	if (m_b_timerrun)
		return;

	UpdateData();
	if (m_elapsed == 0)
		m_elapsed = 100;

	SetTimer(++m_timerID, m_elapsed, nullptr);
	UpdateTimerElapsedVal();

	m_btn_start.EnableWindow(FALSE);
	m_btn_stop.EnableWindow(TRUE);
	m_b_timerrun = TRUE;

	LOG("Start Timer - ID : %d, Elapsed : %d", m_timerID, m_elapsed);
}

void MainDlg::OnBnClickedStopTimer()
{
	if (!m_b_timerrun)
		return;

	KillTimer(m_timerID);
	m_btn_start.EnableWindow(TRUE);
	m_btn_stop.EnableWindow(FALSE);
	m_b_timerrun = FALSE;

	LOG("Stop Timer - ID : %d", m_timerID);
}

void MainDlg::OnBnClickedResetTimer()
{
	KillTimer(m_timerID);
	m_val = 0U;

	UpdateTimerVal();
	m_btn_start.EnableWindow(TRUE);
	m_btn_stop.EnableWindow(FALSE);
	m_b_timerrun = FALSE;

	LOG("Reset Timer - ID : %d", m_timerID);
}

void MainDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void MainDlg::OnBnClickedSub()
{
	if (m_pSubDlg != nullptr && IsWindow(m_pSubDlg->m_hWnd))
		return;

	DTO dto;
	MakeDTO(&dto);

	m_pSubDlg = new SubDlg(this, dto);
	m_pSubDlg->Create(IDD_SubDlg, this);
	m_pSubDlg->ShowWindow(SW_SHOW);

	LOG("Button - Open Sub Dlg");
}

void MainDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	delete m_pSubDlg;
}
