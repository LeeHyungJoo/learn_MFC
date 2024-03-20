#pragma once

class SerialPort;
class LightMessage;

class CSocketComDlg : public CDialogEx
{
public:
	CSocketComDlg(CWnd* pParent = nullptr);
	virtual ~CSocketComDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOCKETCOM_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

private:
	BOOL ConnectToComPort(ENUM_COMM_SET commType);
	BOOL DisconnectToComPort(ENUM_COMM_SET commType);
public:

	afx_msg void OnBnClickedBtnConnectLight();
	afx_msg void OnCbnSelchangeCmbLightType();

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	SerialPort* m_port;
	LightMessage m_msg;

	CButton m_btnConnect;
	CButton m_cxLightConnect;
	CComboBox m_cmbLightType;
};
