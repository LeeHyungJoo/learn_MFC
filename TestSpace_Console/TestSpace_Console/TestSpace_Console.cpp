#include "stdafx.h"
#include <afx.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <tchar.h>
#include <Windows.h>

#include <algorithm>

using namespace std;

#define BUFFER_SIZE 1024
#define BUFFER_SIZE_SECTION 4096
//#define STREAM_MODE

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
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
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
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
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
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				wcout << _T("Scenario_3::share open") << endl;
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
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
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


#ifdef STREAM_MODE
			std::ifstream input(strPath.GetString(), std::ios::binary);
			if (!input)
			{
				std::wcerr << _T("Scenario_5::Failed to open source file: ") << strPath.GetString() << std::endl;
				continue;
			}

			std::ofstream output(strMigrationPath.GetString(), std::ios::binary);
			if (!output)
			{
				std::wcerr << _T("Scenario_5::Failed to create Migration file: ") << strMigrationPath.GetString() << std::endl;
				continue;
			}

			WCHAR bom = 0xFEFF;
			output.write(reinterpret_cast<const char*>(&bom), sizeof(WCHAR));
			output << input.rdbuf();
			output.flush();
#else
			CString strUnicodeTest = _T("C:\\Users\\Public\\Documents\\😀_이모지폴더\\파일.txt");

			TCHAR arrSections[BUFFER_SIZE_SECTION] = { 0 };
			DWORD dwSize = GetPrivateProfileSectionNames(arrSections, BUFFER_SIZE_SECTION, strPath);
			if (dwSize == 0)
			{
				std::wcerr << _T("Scenario_5:: no Section in ini") << strPath.GetString() << std::endl;
				continue;
			}

			CFile iniMigrationFile;
			if (!iniMigrationFile.Open(strMigrationPath, CFile::modeCreate | CFile::modeWrite))
				continue;

			WCHAR bom = 0xFEFF;
			iniMigrationFile.Write(&bom, sizeof(bom));
			iniMigrationFile.Close();

			TCHAR* pSection = arrSections;
			while (*pSection != _T('\0'))
			{
				CString strSection = pSection;

				TCHAR arrKeys[BUFFER_SIZE_SECTION] = { 0 };
				DWORD dwKeySize = GetPrivateProfileSection(strSection, arrKeys, BUFFER_SIZE_SECTION, strPath);
				if (dwKeySize > 0)
				{
					TCHAR* pKeyValue = arrKeys;
					while (*pKeyValue != _T('\0'))
					{
						CString strKeyValue = pKeyValue;

						int nPos = strKeyValue.Find(_T('='));
						if (nPos != -1)
						{
							CString strKey = strKeyValue.Left(nPos);
							CString strValue = strKeyValue.Mid(nPos + 1);
							WritePrivateProfileString(strSection, strKey, strValue, strMigrationPath);
						}

						pKeyValue += lstrlen(pKeyValue) + 1;
					}
				}
				pSection += lstrlen(pSection) + 1;
			}


			CopyFile(strMigrationPath, strPath, FALSE);
			wcout << _T("Scenario_5::Remove Success ") << strPath.GetString() << endl;
			DeleteFile(strMigrationPath);

			WritePrivateProfileString(_T("VersionInfo"), _T("Version"), _T("ver1.1"), strPath);
			wcout << _T("Scenario_5::Rename Success ") << strMigrationPath.GetString() << _T(" to ") << strPath.GetString() << endl;
			WritePrivateProfileString(_T("VersionInfo"), _T("TestField"), strUnicodeTest, strPath);

#endif // STREAM_MODE

			//system("pause");
			//break;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
	catch (CFileException* e)
	{
		TCHAR szError[256];
		e->GetErrorMessage(szError, 256);
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
