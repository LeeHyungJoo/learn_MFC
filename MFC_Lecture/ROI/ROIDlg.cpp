#include "pch.h"
#include "framework.h"
#include "ROI.h"
#include "ROIDlg.h"
#include "afxdialogex.h"
#include "CustomListCtrl.h"
#include "CustomListItem.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CROIDlg::CROIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ROI_DIALOG, pParent), m_lbxROI (new CustomListCtrl())
{
}

void CROIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ROI, *m_lbxROI);
}


BEGIN_MESSAGE_MAP(CROIDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


BOOL CROIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//TODO : 데이터 저장 읽기가 되어야 함. 
	//TODO : 컬럼 순서와, 입력 타입을 맞춰야 함. 
	//m_lcROI.InsertColumn(0, _T("ROI")		, LVCFMT_CENTER, 100);
	//m_lcROI.InsertColumn(1, _T("Center X")	, LVCFMT_CENTER, 100);
	//m_lcROI.InsertColumn(2, _T("Center Y")	, LVCFMT_CENTER, 100);
	//m_lcROI.InsertColumn(3, _T("Width")		, LVCFMT_CENTER, 100);
	//m_lcROI.InsertColumn(4, _T("Height")	, LVCFMT_CENTER, 100);
	//m_lcROI.InsertColumn(5, _T("Input")		, LVCFMT_CENTER, 100);
	//m_lcROI.InsertColumn(6, _T("OFfset")	, LVCFMT_CENTER, 100);

	//m_lcROI.InsertItem(0, _T("Item 1"));
	//m_lcROI.SetItemText(0, 1, _T("Text"));
	//m_lcROI.SetItemText(0, 2, _T("123"));

	// 리스트 컨트롤 생성
	CustomListCtrl listCtrl;
	listCtrl.Create(WS_VISIBLE | WS_CHILD | LVS_REPORT, CRect(10, 10, 400, 200), this, IDC_LIST_ROI);

	// 컬럼 타입 설정
	listCtrl.SetColumnType(0, TEXT_COLUMN);
	listCtrl.SetColumnType(1, CHECKBOX_COLUMN);
	listCtrl.SetColumnType(2, COMBOBOX_COLUMN);

	// 리스트 아이템 생성 및 추가
	CustomListItem* pItem1 = new CustomListItem();
	pItem1->SetText(_T("Text"));
	pItem1->SetChecked(true);
	CStringList comboItems;
	comboItems.AddTail(_T("Option 1"));
	comboItems.AddTail(_T("Option 2"));
	comboItems.AddTail(_T("Option 3"));

	pItem1->SetComboBoxItems(comboItems);

	return TRUE;
}

void CROIDlg::OnPaint()
{
	CDialogEx::OnPaint();
}
