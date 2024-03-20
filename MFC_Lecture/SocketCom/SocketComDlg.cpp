
// SocketComDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SocketCom.h"
#include "SocketComDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSocketComDlg::CSocketComDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOCKETCOM_DIALOG, pParent)
{

}

void CSocketComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSocketComDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_LIGHT_INTENSITY, &CSocketComDlg::OnEnChangeEditLightIntensity)
END_MESSAGE_MAP()


BOOL CSocketComDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;
}

void CSocketComDlg::OnPaint()
{
	CDialogEx::OnPaint();
}



void CSocketComDlg::OnEnChangeEditLightIntensity()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
