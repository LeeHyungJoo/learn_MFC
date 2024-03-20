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

public:

	afx_msg void OnBnClickedBtnConnectLight();
	afx_msg void OnCbnSelchangeCmbLightType();
	afx_msg void OnBnClickedBtnLightSend();
	afx_msg void OnBnClickedBtnLightStop();

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	SerialPort* m_port;
	LightMessage m_msg;

	INT m_lightIntensity;

	CComboBox m_cmbLightType;

	CButton m_btnConnect;
	CButton m_cxLightConnect;
	CButton m_btnLightSend;
	CButton m_btnLightStop;
	afx_msg void OnEnKillfocusEditLightIntensity();
};
