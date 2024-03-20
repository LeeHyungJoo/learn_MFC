#include "pch.h"
#include "framework.h"
#include "SocketCom.h"
#include "SocketComDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSocketComDlg::CSocketComDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOCKETCOM_DIALOG, pParent),
	m_port(new SerialPort())
{
	if (m_port->OpenPort(L"COM7"))
	{
		m_port->ConfigurePort(38400, 8, 0, 0, 1);
	}
}

CSocketComDlg::~CSocketComDlg()
{
	m_port->ClosePort();
	delete m_port;
}

void CSocketComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CONNECT_LIGHT, m_btnConnect);
	DDX_Control(pDX, IDC_CHECK_LIGHTCONNECT, m_cxLightConnect);
	DDX_Control(pDX, IDC_CMB_LIGHT_TYPE, m_cmbLightType);
}

BOOL CSocketComDlg::ConnectToComPort(ENUM_COMM_SET commType)
{
	
	return 0;
}

BOOL CSocketComDlg::DisconnectToComPort(ENUM_COMM_SET commType)
{
	return 0;
}

BEGIN_MESSAGE_MAP(CSocketComDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT_LIGHT, &CSocketComDlg::OnBnClickedBtnConnectLight)
	ON_CBN_SELCHANGE(IDC_CMB_LIGHT_TYPE, &CSocketComDlg::OnCbnSelchangeCmbLightType)
END_MESSAGE_MAP()


BOOL CSocketComDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_cmbLightType.SetCurSel(0);

	m_btnConnect.EnableWindow(!m_port->IsPortReady());
	m_cxLightConnect.SetCheck(m_port->IsPortReady());

	//TEST
	m_msg.SetType(LightMessage::L_4CH_38mA);
	m_msg.SetBrightness(20);

	unsigned char * msg = nullptr;
	DWORD size;
	m_msg.Serialize(&msg, size);

	m_port->WriteByte(msg, size);
	return TRUE;
}

void CSocketComDlg::OnPaint()
{
	CDialogEx::OnPaint();
}

void CSocketComDlg::OnBnClickedBtnConnectLight()
{
	if (m_port->OpenPort(L"COM7"))
	{
		m_port->ConfigurePort(38400, 8, 0, 0, 1);
	}
	else
	{
		AfxMessageBox(_T("직렬 포트를 여는데 실패 했습니다.", MB_ICONWARNING | MB_OK));
	}
}


void CSocketComDlg::OnCbnSelchangeCmbLightType()
{
	static int lastType = -1;
	int curType = m_cmbLightType.GetCurSel();
}
