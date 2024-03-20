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
	, m_lightIntensity(0)
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
	DDX_Control(pDX, IDC_BTN_LIGHT_SEND, m_btnLightSend);
	DDX_Control(pDX, IDC_BTN_LIGHT_STOP, m_btnLightStop);
	DDX_Text(pDX, IDC_EDIT_LIGHT_INTENSITY, m_lightIntensity);
	DDV_MinMaxInt(pDX, m_lightIntensity, 0, 99);
}

BEGIN_MESSAGE_MAP(CSocketComDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT_LIGHT, &CSocketComDlg::OnBnClickedBtnConnectLight)
	ON_CBN_SELCHANGE(IDC_CMB_LIGHT_TYPE, &CSocketComDlg::OnCbnSelchangeCmbLightType)
	ON_BN_CLICKED(IDC_BTN_LIGHT_SEND, &CSocketComDlg::OnBnClickedBtnLightSend)
	ON_BN_CLICKED(IDC_BTN_LIGHT_STOP, &CSocketComDlg::OnBnClickedBtnLightStop)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LIGHT_INTENSITY, &CSocketComDlg::OnDeltaposSpinLightIntensity)
	ON_EN_UPDATE(IDC_EDIT_LIGHT_INTENSITY, &CSocketComDlg::OnEnUpdateEditLightIntensity)
END_MESSAGE_MAP()

BOOL CSocketComDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_cmbLightType.SetCurSel(0);

	return TRUE;
}

void CSocketComDlg::OnPaint()
{
	CDialogEx::OnPaint();

	m_btnConnect.EnableWindow(!m_port->IsPortReady());
	m_btnLightSend.EnableWindow(m_port->IsPortReady());
	m_btnLightStop.EnableWindow(m_port->IsPortReady());
	m_cxLightConnect.SetCheck(m_port->IsPortReady());
}

void CSocketComDlg::OnBnClickedBtnConnectLight()
{
	if (m_port->OpenPort(L"COM7") && m_port->ConfigurePort(38400, 8, 0, 0, 1))
		return;

	AfxMessageBox(L"직렬 포트를 여는데 실패 했습니다.", MB_ICONWARNING | MB_OK);
}

void CSocketComDlg::OnCbnSelchangeCmbLightType()
{
	static int lastType = -1;
	int curType = m_cmbLightType.GetCurSel();

	if (lastType == curType)
		return;

	lastType = curType;

	switch (curType)
	{
	case 0:
		m_msg.SetType(LightMessage::L_ALL_CH_12V);
		break;
	case 1:
		m_msg.SetType(LightMessage::L_4CH_12V);
		break;
	case 2:
		m_msg.SetType(LightMessage::L_4CH_38mA);
		break;
	}
}

void CSocketComDlg::OnBnClickedBtnLightSend()
{
	m_msg.SetBrightness(m_lightIntensity);

	unsigned char * msg = nullptr;
	DWORD size;

	m_msg.Serialize(&msg, size);
	m_port->WriteByte(msg, size);
}

void CSocketComDlg::OnBnClickedBtnLightStop()
{
	m_msg.SetBrightness(0);

	unsigned char * msg = nullptr;
	DWORD size;

	m_msg.Serialize(&msg, size);
	m_port->WriteByte(msg, size);
}

void CSocketComDlg::OnEnUpdateEditLightIntensity()
{
	UpdateData();

	static int lastIntensity = 0;

	if (m_lightIntensity > 99 || m_lightIntensity < 0)
	{
		if(m_lightIntensity > 99)
			m_lightIntensity = lastIntensity;
		if (m_lightIntensity < 0)
			m_lightIntensity = lastIntensity;

		CString show;
		show.Format(L"%d", m_lightIntensity);
		GetDlgItem(IDC_EDIT_LIGHT_INTENSITY)->SetWindowText(show);
	}

	if (lastIntensity == m_lightIntensity)
		return;

	lastIntensity = m_lightIntensity;

	CString show;
	show.Format(L"%d", m_lightIntensity);
	GetDlgItem(IDC_EDIT_LIGHT_INTENSITY)->SetWindowText(show);
}

void CSocketComDlg::OnDeltaposSpinLightIntensity(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_lightIntensity += (-pNMUpDown->iDelta);

	CString show;
	show.Format(L"%d", m_lightIntensity);
	GetDlgItem(IDC_EDIT_LIGHT_INTENSITY)->SetWindowText(show);

	*pResult = 0;
}