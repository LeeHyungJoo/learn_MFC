#include "stdafx.h"
#include <afx.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <tchar.h>
#include <Windows.h>
#include <algorithm>

using namespace std;

#define BUFFER_SIZE 1024
#define STREAM_MODE

#define PATH_DISKLOCK (_T("D:\\TestRead\\disklock.ini"))


/// <summary>
///		exclusive 하게 1초 열고, 0.5초 휴식 반복
/// </summary>
void Scenario_1()
{
	try
	{
		while (TRUE)
		{
			CFile file;
			if (file.Open(PATH_DISKLOCK, CFile::modeRead | CFile::shareExclusive))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				wcout << _T("Scenario_1::Exclusive open") << endl;
				file.Close();
			}
			else
			{
				wcerr << _T("Scenario_1::Fail file open errno ") << GetLastError() << endl;
			}

			wcout << _T("Scenario_1::sleep") << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
	catch (CFileException* e) {
		TCHAR szError[BUFFER_SIZE];
		e->GetErrorMessage(szError, BUFFER_SIZE);
		wcerr << _T("Scenario_1::Exception ") << szError << endl;
		e->Delete();
	}
}

/// <summary>
/// GetPrivateProfileInt로 0.7초마다 읽음
/// </summary>
void Scenario_2()
{
	try
	{
		while (TRUE)
		{
			TCHAR buffer[BUFFER_SIZE] = { 0 };
			GetPrivateProfileString(_T("Section"), _T("Key"), _T("Default"), buffer, BUFFER_SIZE, PATH_DISKLOCK);
			wcout << _T("Scenario_2::GetPrivateProfileString : ") << buffer << _T("\n");
			std::this_thread::sleep_for(std::chrono::milliseconds(700));
		}
	}
	catch (CFileException* e) {
		TCHAR szError[BUFFER_SIZE];
		e->GetErrorMessage(szError, BUFFER_SIZE);
		wcerr << _T("Scenario_2::Exception ") << szError << endl;
		e->Delete();
	}
}

/// <summary>
///		share 하게 0.8초 열고, 0.2초 휴식 반복
/// </summary>
void Scenario_3()
{
	try
	{
		while (TRUE)
		{
			CFile file;
			if (file.Open(PATH_DISKLOCK, CFile::modeRead | CFile::shareDenyNone))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(800));
				wcout << _T("Scenario_3::Exclusive open") << endl;
				file.Close();
			}
			else
			{
				wcerr << _T("Scenario_3::Fail file open errno ") << GetLastError() << endl;
			}

			wcout << _T("Scenario_3::sleep") << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	}
	catch (CFileException* e)
	{
		TCHAR szError[BUFFER_SIZE];
		e->GetErrorMessage(szError, BUFFER_SIZE);
		wcerr << _T("Scenario_3::Exception ") << szError << endl;
		e->Delete();
	}
}

/// <summary>
/// profile 함수 write 0.4초마다 반복
/// </summary>
void Scenario_4()
{
	try
	{
		while (TRUE)
		{
			auto now = std::chrono::steady_clock::now();

			CString strValue;
			strValue.Format(_T("val-%I64d"), now.time_since_epoch().count());

			BOOL bResult = WritePrivateProfileString(_T("Section"), _T("Key"), strValue, PATH_DISKLOCK);
			if (bResult)
			{
				wcout << _T("Scenario_4::Write") << endl;
			}
			else
			{
				wcerr << _T("Scenario_4::Fail to Write errno ") << GetLastError() << endl;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(400));
		}
	}
	catch (CFileException* e)
	{
		TCHAR szError[BUFFER_SIZE];
		e->GetErrorMessage(szError, BUFFER_SIZE);
		wcerr << _T("Scenario_4::Exception ") << szError << endl;
		e->Delete();
	}
}


/// <summary>
/// 0.5초마다 변환 및 복사 작업 (UTF -> unicode)
/// </summary>
void Scenario_5()
{
	try
	{
		while (TRUE)
		{
			CString strPath = PATH_DISKLOCK;
			CString strMigrationPath = _T("D:\\TestRead\\disklock.ini.migration");
			BOOL bNeedToConvert = FALSE;
			if (_taccess(strPath, 0) == -1)
			{
				wcerr << _T("Scenario_5::No File") << endl;
				std::this_thread::sleep_for(std::chrono::seconds(500));
				continue;
			}
			else
			{
				CString strVersion;
				if (GetPrivateProfileString(_T("VersionInfo"), _T("Version"), _T("noVersion"), strVersion.GetBuffer(BUFFER_SIZE), BUFFER_SIZE, strPath))
				{
					wcout << _T("Scenario_5::Version Read") << endl;
				}
				else
				{
					wcout << _T("Scenario_5::Fail Version Read errno ") << GetLastError() << endl;
				}

				if (strVersion != _T("ver1.1"))
					bNeedToConvert = TRUE;
			}

			if (bNeedToConvert)
			{

#ifdef STREAM_MODE


#else
				CFile iniFile;
				if (iniFile.Open(strPath, CFile::modeRead | CFile::shareExclusive))
				{
					wcout << _T("Scenario_5::Open") << endl;
					ULONGLONG nFileSize = iniFile.GetLength();

					// 파일 전체 내용을 담을 버퍼 할당
					CHAR strBuffer[BUFFER_SIZE] = { 0 };
					iniFile.Read(strBuffer, BUFFER_SIZE);
					int t1 = strBuffer[0];
					int t2 = strBuffer[1];
					int t3 = strBuffer[2];

					CFile iniMigrationFile;
					if (iniMigrationFile.Open(strMigrationPath, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive))
					{
						wcout << _T("Scenario_5::Create migration") << endl;
						iniMigrationFile.SeekToBegin();
						WCHAR	szUnicodeBOM = 0xFEFF;
						iniMigrationFile.Write(&szUnicodeBOM, sizeof(WCHAR));
						iniMigrationFile.SeekToEnd();
						iniMigrationFile.Write(strBuffer, BUFFER_SIZE);

						//CString strUnicodeTest = _T("C:\\Users\\Public\\Documents\\😀_이모지폴더\\파일.txt");
						//iniMigrationFile.Write(strUnicodeTest.GetBuffer(BUFFER_SIZE), BUFFER_SIZE);
						iniMigrationFile.Flush();

						wcout << _T("Scenario_5::Copy Success") << endl;

						iniMigrationFile.Close();
						iniFile.Close();
						CFile::Remove(strPath);
						wcout << _T("Scenario_5::Remove Success ") << strPath.GetString() << endl;
						CFile::Rename(strMigrationPath, strPath);

						WritePrivateProfileString(_T("VersionInfo"), _T("Version"), _T("ver1.1"), strPath);
						wcout << _T("Scenario_5::Rename Success ") << strMigrationPath.GetString() << _T(" to ") << strPath.GetString() << endl;
					}
					else
					{
						wcerr << _T("Scenario_5::Fail create migration") << endl;
					}
				}
				else
				{
					wcerr << _T("Scenario_5::Fail open errno ") << GetLastError() << endl;
				}
			}
			else
			{
				wcout << _T("Scenario_5::No need to migrate only copy") << endl;

				CFile iniFile;
				if (iniFile.Open(strPath, CFile::modeRead | CFile::shareExclusive))
				{
					wcout << _T("Scenario_5::Open") << endl;
					ULONGLONG nFileSize = iniFile.GetLength();

					// 파일 전체 내용을 담을 버퍼 할당
					BYTE* pBuffer = new BYTE[(size_t)nFileSize];
					iniFile.Read(pBuffer, (UINT)nFileSize);


					CFile iniMigrationFile;
					if (iniMigrationFile.Open(strMigrationPath, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive))
					{
						wcout << _T("Scenario_5::Create temp file") << endl;

						iniMigrationFile.Write(pBuffer, (UINT)nFileSize);
						iniMigrationFile.Close();
						iniMigrationFile.Flush();
						delete[] pBuffer;

						wcout << _T("Scenario_5::Copy to Temp Success") << endl;

						iniFile.Close();
						DeleteFile(strPath);
						CFile::Rename(strMigrationPath, strPath);
					}
					else
					{
						wcerr << _T("Scenario_5::Fail create temp file") << endl;
					}
				}
				else
				{
					wcerr << _T("Scenario_5::Fail open errno ") << GetLastError() << endl;
				}
#endif // STREAM_MODE


			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
	catch (CFileException* e)
	{
		TCHAR szError[BUFFER_SIZE];
		e->GetErrorMessage(szError, BUFFER_SIZE);
		wcerr << _T("Scenario_5::Exception ") << szError << endl;
		e->Delete();
	}

}

int _tmain(int argc, TCHAR* argv[])
{
	CString strScenario(argv[1]);

	if (strScenario.CompareNoCase(_T("1")) == 0)
	{
		Scenario_1();
	}
	else if (strScenario.CompareNoCase(_T("2")) == 0)
	{
		Scenario_2();
	}
	else if (strScenario.CompareNoCase(_T("3")) == 0)
	{
		Scenario_3();
	}
	else if (strScenario.CompareNoCase(_T("4")) == 0)
	{
		Scenario_4();
	}
	else if (strScenario.CompareNoCase(_T("5")) == 0)
	{
		Scenario_5();
	}
	else
	{
		wcerr << _T("invalid strScenario !!") << endl;
	}

	return 0;
}
