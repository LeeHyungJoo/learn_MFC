#include "pch.h"
#include "framework.h"
#include "MFC_Number.h"
#include "MFC_NumberDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMFCNumberDlg::CMFCNumberDlg(CWnd* pParent)
	: CDialogEx(IDD_MFC_NUMBER_DIALOG, pParent)
	, m_uCnt(10U)
{
	
}

void CMFCNumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RANDLIST, m_lsbNums);
	DDX_Control(pDX, IDC_EDIT_AVG, m_editAvg);
	DDX_Control(pDX, IDC_EDIT_SD, m_editSd);
	DDX_Control(pDX, IDC_EDIT_MNX, m_editMnx);
	DDX_Text(pDX, IDC_EDIT_CNT, m_uCnt);
	DDV_MinMaxUInt(pDX, m_uCnt, 10u, MAX_LIST_CNT);
}

BEGIN_MESSAGE_MAP(CMFCNumberDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_AVG, &CMFCNumberDlg::OnBnClickedButtonAvg)
	ON_BN_CLICKED(IDC_BUTTON_SD, &CMFCNumberDlg::OnBnClickedButtonSd)
	ON_BN_CLICKED(IDC_BUTTON_MNX, &CMFCNumberDlg::OnBnClickedButtonMnx)
	ON_BN_CLICKED(IDC_BUTTON_GEN, &CMFCNumberDlg::OnBnClickedButtonGen)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMFCNumberDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CMFCNumberDlg::OnBnClickedButtonReset)
END_MESSAGE_MAP()


BOOL CMFCNumberDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	return TRUE;
}

void CMFCNumberDlg::MakeValArr(OUT DOUBLE * pVals, UINT cnt)
{
	for (UINT idx = 0; idx < cnt; idx++)
	{
		CString val;
		m_lsbNums.GetText(idx, val);
		pVals[idx] = _wtof(val);
	}
}

void CMFCNumberDlg::CalculateAverage(DOUBLE* pVals, UINT cnt, OUT DOUBLE* average)
{
	auto oper = [](double acc, double val) {return acc + val / MAX_LIST_CNT; };
	auto sum = std::accumulate(pVals, pVals + cnt, 0.0, oper);
	*average =  sum / cnt * MAX_LIST_CNT;
}

void CMFCNumberDlg::CalculateStandardDeviation(DOUBLE* pVals, UINT cnt, OUT DOUBLE* SD)
{
	auto oper = [](double acc, double val) {return acc + val / MAX_LIST_CNT; };
	auto sum = std::accumulate(pVals, pVals + cnt, 0.0, oper);
	auto average = sum / cnt * MAX_LIST_CNT;

	DOUBLE deviationSum = 0.0;

	for (UINT i = 0; i < cnt; i++) {
		DOUBLE deviation = (pVals[i] - average) * (pVals[i] - average);
		deviationSum += deviation;
	}

	*SD = std::sqrt(deviationSum / (cnt - 1));
}

void CMFCNumberDlg::GetMinMax(DOUBLE * pVals, UINT cnt, OUT DOUBLE * minv, OUT DOUBLE * maxv)
{
	*minv = (std::numeric_limits<DOUBLE>::max)();
	*maxv = (std::numeric_limits<DOUBLE>::lowest)();

	for (UINT idx = 0; idx < cnt; idx++)
	{
		*minv = (std::min)(pVals[idx], *minv);
		*maxv = (std::max)(pVals[idx], *maxv);
	}
}

void CMFCNumberDlg::OnBnClickedButtonGen()
{
	UpdateData();

	int maxv = -1;
	for (UINT cnt = 0; cnt < m_uCnt; cnt++)
	{
		auto val = Utill::GetRandomTest();
		//auto val = Utill::GetRandomDouble();
		CString valStr;
		valStr.Format(L"%.3lf", val);
		m_lsbNums.AddString(valStr);
		maxv = (std::max)(maxv, valStr.GetLength() *10);
	}
	m_lsbNums.SetHorizontalExtent(maxv);
}

void CMFCNumberDlg::OnBnClickedButtonAvg()
{
	UpdateData();

	UINT cnt = m_lsbNums.GetCount();
	DOUBLE* pVals = new DOUBLE[cnt];
	MakeValArr(pVals, cnt);

	DOUBLE avg;
	CalculateAverage(pVals, cnt, &avg);

	CString show;
	show.Format(L"%.3lf", avg);
	m_editAvg.SetWindowTextW(show);
}


void CMFCNumberDlg::OnBnClickedButtonSd()
{
	UpdateData();

	UINT cnt = m_lsbNums.GetCount();
	DOUBLE* pVals = new DOUBLE[cnt];
	MakeValArr(pVals, cnt);

	DOUBLE sd;
	CalculateStandardDeviation(pVals, cnt, &sd);

	CString show;
	show.Format(L"%.3lf", sd);
	m_editSd.SetWindowTextW(show);
}


void CMFCNumberDlg::OnBnClickedButtonMnx()
{
	UpdateData();

	UINT cnt = m_lsbNums.GetCount();
	DOUBLE* pVals = new DOUBLE[cnt];
	MakeValArr(pVals, cnt);

	DOUBLE minv, maxv;
	GetMinMax(pVals, cnt, &minv, &maxv);

	CString show;
	show.Format(L"%.3lf\r\n%.3lf", minv, maxv);
	m_editMnx.SetWindowTextW(show);
}


void CMFCNumberDlg::OnBnClickedButtonSave()
{
	UpdateData();

	UINT cnt = m_lsbNums.GetCount();
	DOUBLE* pVals = new DOUBLE[cnt];
	MakeValArr(pVals, cnt);

	CFile file;
	if (file.Open(L"vals",CFile::modeCreate | CFile::modeWrite))
	{
		for (UINT i = 0; i < cnt; i++)
		{
			CString format;
			format.Format(L"%.3lf\r\n", pVals[i]);
			file.Write(format, format.GetLength() * sizeof(TCHAR));
		}
	}
}

void CMFCNumberDlg::OnBnClickedButtonReset()
{
	m_lsbNums.ResetContent();
	m_editAvg.Clear();
	m_editSd.Clear();
	m_editMnx.Clear();
}
