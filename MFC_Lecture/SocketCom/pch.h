// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include <string>
#include "SerialPort.h"
#include "Protocols.h"

enum ENUM_COMM_SET
{
	COMM_SERIAL1 = 1,
};

#endif //PCH_H

//#pragma comment(lib, "CommSetDLL")
//
//extern "C"  __declspec(dllimport) BOOL WINAPI CommSet_Initialize(int nCount);
//extern "C"  __declspec(dllimport) void WINAPI CommSet_Finalize();
//extern "C"  __declspec(dllimport) BOOL WINAPI SetCallback_DataReceived(int nIndex, LPVOID lpFunc);
//extern "C"  __declspec(dllimport) BOOL WINAPI SetCallback_Error(int nIndex, LPVOID lpFunc);
//
//extern "C"  __declspec(dllimport) BOOL WINAPI StopAll(int nIndex);
//
//
//extern "C"  __declspec(dllimport) BOOL WINAPI SetParameter(int nIndex, TCHAR * port, unsigned int baudrate, unsigned int parity, unsigned int databit, unsigned int stopbit);
//extern "C"  __declspec(dllimport) BOOL WINAPI Serial_Open(int nIndex, HWND hWnd, TCHAR * tcPort, int nBaud);
//extern "C"  __declspec(dllimport) BOOL WINAPI Serial_Clear(int nIndex);
//extern "C"  __declspec(dllimport) BOOL WINAPI Serial_Send(int nIndex, unsigned char *outbuf, int len);
//extern "C"  __declspec(dllimport) BOOL WINAPI Serial_Close(int nIndex);
//extern "C"  __declspec(dllimport) BOOL WINAPI Serial_Reset(int nIndex);
//
//extern "C"  __declspec(dllimport) BOOL WINAPI Serial_IsOpen(int nIndex);