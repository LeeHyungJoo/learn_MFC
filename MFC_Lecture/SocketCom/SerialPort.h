#pragma once

using namespace std;

class SerialPort
{
public:
	BOOL OpenPort(CString portname);
	void ClosePort();
	BOOL ConfigurePort(DWORD BaudRate, BYTE ByteSize, DWORD fParity, BYTE Parity, BYTE StopBits);
	BOOL IsPortReady();

	BOOL WriteByte(unsigned char* data, DWORD dataSize);

private:
	HANDLE          m_hComm;
	DCB             m_dcb;
	BOOL            m_bPortReady;
	DWORD           m_iBytesWritten;
};