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
	, m_uTimerID(0U)
	, m_uElapsed(100U)
	, m_uCnt(0)
	, m_bTimerRun(FALSE)
{
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO2, m_cmbx);

	DDX_Control(pDX, IDC_RADIO_ACTIVE, m_rbtActive);
	DDX_Control(pDX, IDC_RADIO_HIDE, m_rbtHide);
	DDX_Control(pDX, IDC_RADIO_DEACTIVE, m_rbtDeactive);

	DDX_Control(pDX, IDC_CHECK1, m_cbxOpt1);
	DDX_Control(pDX, IDC_CHECK2, m_cbxOpt2);
	DDX_Control(pDX, IDC_CHECK3, m_cbxOpt3);
	DDX_Control(pDX, IDC_EDIT1, m_edtOpt);

	DDX_Control(pDX, IDC_LIST1, m_lstbLog);

	DDX_Text(pDX, IDC_TEXT, m_uCnt);
	DDX_Text(pDX, IDC_EDIT2, m_uElapsed);

	DDX_Control(pDX, IDC_SCROLLBAR1, m_hsb);
	DDX_Control(pDX, IDC_EDIT3, m_edtHsb);
	DDX_Control(pDX, IDC_SCROLLBAR2, m_vsb);
	DDX_Control(pDX, IDC_EDIT4, m_edtVsb);

	DDX_Control(pDX, IDC_BUTTON1, m_btStartTimer);
	DDX_Control(pDX, IDC_BUTTON2, m_btStopTimer);
}

BOOL MainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_cmbx.AddString(TEXT("default"));
	m_cmbx.AddString(TEXT("foo"));
	m_cmbx.AddString(TEXT("boo"));
	m_cmbx.SetCurSel(0);

	m_rbtActive.SetCheck(1);

	m_vcbxOpt.clear();
	m_vcbxOpt.push_back(&m_cbxOpt1);
	m_vcbxOpt.push_back(&m_cbxOpt2);
	m_vcbxOpt.push_back(&m_cbxOpt3);

	m_hsb.SetScrollRange(0, 255, FALSE);
	m_hsb.SetScrollPos(0);
	m_vsb.SetScrollRange(0, 255, FALSE);
	m_vsb.SetScrollPos(0);

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
	CDialogEx::OnPaint();
}

BEGIN_MESSAGE_MAP(MainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_TIMER()
	ON_COMMAND_RANGE(IDC_RADIO_ACTIVE, IDC_RADIO_DEACTIVE, OnRdBnClicked)
	ON_COMMAND_RANGE(IDC_CHECK1, IDC_CHECK3, OnCbChanged)
	ON_BN_CLICKED(IDCANCEL,		&MainDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDSUB,		&MainDlg::OnBnClickedSub)
	ON_BN_CLICKED(IDC_BUTTON1,	&MainDlg::OnBnClickedStartTimer)
	ON_BN_CLICKED(IDC_BUTTON2,	&MainDlg::OnBnClickedStopTimer)
	ON_BN_CLICKED(IDC_BUTTON3,	&MainDlg::OnBnClickedResetTimer)
	ON_CBN_SELCHANGE(IDC_COMBO2, &MainDlg::OnSelchangeCombo)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void MainDlg::UpdateOptCheckBoxStr()
{
	CString optnames;

	for (const auto cb : m_vcbxOpt)
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
	val.Format(TEXT("%d"), m_uCnt);

	SetDlgItemText(IDC_TEXT, val);
}

void MainDlg::UpdateTimerElapsedVal()
{
	CString val;
	val.Format(TEXT("%d"), m_uElapsed);

	SetDlgItemText(IDC_EDIT2, val);
}

void MainDlg::UpdateHScrollBarVal()
{
	CString val;
	val.Format(TEXT("%d"), m_hsb.GetScrollPos());

	SetDlgItemText(IDC_EDIT3, val);
}

void MainDlg::UpdateVScrollBarVal()
{
	CString val;
	val.Format(TEXT("%d"), m_vsb.GetScrollPos());

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
	m_lstbLog.AddString(logmsg);

	va_end(args);
}

void MainDlg::MakeDTO(OUT DTO * dto)
{
	CString cbs;
	m_cmbx.GetLBText(m_cmbx.GetCurSel(), cbs);
	dto->sComboSelect = cbs;

	CString rbs;
	if (m_rbtActive.GetCheck())
		rbs = "active";
	else if (m_rbtDeactive.GetCheck())
		rbs = "deactive";
	else if (m_rbtHide.GetCheck())
		rbs = "hide";
	dto->sOptionType = rbs;

	CString opts;
	m_edtOpt.GetWindowText(opts);
	dto->sOption = opts;

	dto->uCounter = m_uCnt;
	dto->uElapsed = m_uElapsed;

	dto->uHScrollbarPos = m_hsb.GetScrollPos();
	dto->uVScrollbarPos = m_vsb.GetScrollPos();
}

void MainDlg::OnSelchangeCombo()
{
	CString s;
	m_cmbx.GetLBText(m_cmbx.GetCurSel(), s);

	LOG("ComboBox - %d, %ls", m_cmbx.GetCurSel(), s);
}

void MainDlg::OnRdBnClicked(UINT idx)
{
	if (m_urbtActiveIdx == idx)
		return;

	switch (idx)
	{
	case IDC_RADIO_ACTIVE:
		for (const auto cb : m_vcbxOpt)
		{
			cb->ShowWindow(SW_SHOWNORMAL);
			cb->EnableWindow(TRUE);
		}
		m_edtOpt.ShowWindow(SW_SHOWNORMAL);
		m_edtOpt.EnableWindow(TRUE);

		LOG("RadioBtn - Active");
		break;
	case IDC_RADIO_HIDE:
		for (const auto cb : m_vcbxOpt)
		{
			cb->ShowWindow(SW_HIDE);
		}
		m_edtOpt.ShowWindow(SW_HIDE);

		LOG("RadioBtn - Hide");
		break;
	case IDC_RADIO_DEACTIVE:
		for (const auto cb : m_vcbxOpt)
		{
			cb->ShowWindow(SW_SHOWNORMAL);
			cb->EnableWindow(FALSE);
		}
		m_edtOpt.ShowWindow(SW_SHOWNORMAL);
		m_edtOpt.EnableWindow(FALSE);

		LOG("RadioBtn - Deactive");
		break;
	}

	m_urbtActiveIdx = idx;
}

void MainDlg::OnCbChanged(UINT idx)
{
	UpdateOptCheckBoxStr();

	CString s;
	m_edtOpt.GetWindowText(s);

	LOG("CheckBox - %ls", s.IsEmpty() ? L"n/a" : s);
}

void MainDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != nullptr)
	{
		if (pScrollBar->GetSafeHwnd() == m_hsb.GetSafeHwnd())
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
		if (pScrollBar->GetSafeHwnd() == m_vsb.GetSafeHwnd())
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

	if (m_uTimerID == nIDEvent)
	{
		m_uCnt += 1;
		UpdateTimerVal();
	}
}

void MainDlg::OnBnClickedStartTimer()
{
	if (m_bTimerRun)
		return;

	UpdateData();
	if (m_uElapsed == 0)
		m_uElapsed = 100;

	SetTimer(++m_uTimerID, m_uElapsed, nullptr);
	UpdateTimerElapsedVal();

	m_btStartTimer.EnableWindow(FALSE);
	m_btStopTimer.EnableWindow(TRUE);
	m_bTimerRun = TRUE;

	LOG("Start Timer - ID : %d, Elapsed : %d", m_uTimerID, m_uElapsed);
}

void MainDlg::OnBnClickedStopTimer()
{
	if (!m_bTimerRun)
		return;

	KillTimer(m_uTimerID);
	m_btStartTimer.EnableWindow(TRUE);
	m_btStopTimer.EnableWindow(FALSE);
	m_bTimerRun = FALSE;

	LOG("Stop Timer - ID : %d", m_uTimerID);
}

void MainDlg::OnBnClickedResetTimer()
{
	KillTimer(m_uTimerID);
	m_uCnt = 0U;

	UpdateTimerVal();
	m_btStartTimer.EnableWindow(TRUE);
	m_btStopTimer.EnableWindow(FALSE);
	m_bTimerRun = FALSE;

	LOG("Reset Timer - ID : %d", m_uTimerID);
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
	CRect mainRect;
	this->GetWindowRect(&mainRect);
	m_pSubDlg->SetWindowPos(nullptr, mainRect.right, mainRect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_pSubDlg->ShowWindow(SW_SHOW);

	LOG("Button - Open Sub Dlg");
}

void MainDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	delete m_pSubDlg;
}
