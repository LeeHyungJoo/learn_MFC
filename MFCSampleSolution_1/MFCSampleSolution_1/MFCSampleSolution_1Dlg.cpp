
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


// CMFCSampleSolution1Dlg dialog



CMFCSampleSolution1Dlg::CMFCSampleSolution1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCSAMPLESOLUTION_1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCSampleSolution1Dlg::DoDataExchange(CDataExchange* pDX)
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
}

BEGIN_MESSAGE_MAP(CMFCSampleSolution1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CMFCSampleSolution1Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDCANCEL, &CMFCSampleSolution1Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CMFCSampleSolution1Dlg::OnBnClickedOk)
	ON_COMMAND_RANGE(IDC_RADIO_ACTIVE, IDC_RADIO_DEACTIVE, OnRdBnClicked)
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



void CMFCSampleSolution1Dlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CMFCSampleSolution1Dlg::OnBnClickedRadioDeactive()
{
	// TODO: Add your control notification handler code here
}


void CMFCSampleSolution1Dlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CMFCSampleSolution1Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
}

void CMFCSampleSolution1Dlg::OnRdBnClicked(UINT idx)
{
	switch (idx)
	{
	case IDC_RADIO_ACTIVE:
		MessageBox(CString("active!!"), TEXT("radio btn"), MB_ICONINFORMATION);
		break;
	case IDC_RADIO_HIDE:
		MessageBox(CString("hide!!"), TEXT("radio btn"), MB_ICONINFORMATION);
		break;
	case IDC_RADIO_DEACTIVE:
		MessageBox(CString("deactive!!"), TEXT("radio btn"), MB_ICONINFORMATION);
		break;
	}
}
