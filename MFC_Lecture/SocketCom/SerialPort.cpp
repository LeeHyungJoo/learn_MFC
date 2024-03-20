#include "pch.h"
#include "SerialPort.h"

using namespace std;

BOOL SerialPort::OpenPort(CString portname)
{
	m_hComm = CreateFile(L"//./" + portname, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

	if (m_hComm == INVALID_HANDLE_VALUE)
		return FALSE;
	else
		return TRUE;
}

VOID SerialPort::ClosePort()
{
	CloseHandle(m_hComm);
	return;
}

BOOL SerialPort::ConfigurePort(DWORD BaudRate, BYTE ByteSize, DWORD fParity, BYTE Parity, BYTE StopBits)
{
	m_bPortReady = GetCommState(m_hComm, &m_dcb);

	if (!m_bPortReady)
	{
		::OutputDebugString(_T("GetCommState Error"));
		CloseHandle(m_hComm);

		return false;
	}

	m_dcb.BaudRate = BaudRate;
	m_dcb.ByteSize = ByteSize;
	m_dcb.Parity = Parity;
	m_dcb.StopBits = StopBits;
	m_dcb.fBinary = true;
	m_dcb.fDsrSensitivity = false;
	m_dcb.fParity = fParity;
	m_dcb.fOutX = false;
	m_dcb.fInX = false;
	m_dcb.fNull = false;
	m_dcb.fAbortOnError = true;
	m_dcb.fOutxCtsFlow = false;
	m_dcb.fOutxDsrFlow = false;
	m_dcb.fDtrControl = DTR_CONTROL_DISABLE;
	m_dcb.fDsrSensitivity = false;
	m_dcb.fRtsControl = RTS_CONTROL_DISABLE;
	m_dcb.fOutxCtsFlow = false;
	m_dcb.fOutxCtsFlow = false;

	m_bPortReady = SetCommState(m_hComm, &m_dcb);

	if (m_bPortReady == 0)
	{
		::OutputDebugString(_T("SetCommState Error"));
		CloseHandle(m_hComm);

		return false;
	}

	return true;
}

BOOL SerialPort::IsPortReady()
{
	return m_bPortReady;
}

BOOL SerialPort::WriteByte(unsigned char * data, DWORD dataSize)
{
	return WriteFile(m_hComm, data, dataSize, &m_iBytesWritten, NULL);
}