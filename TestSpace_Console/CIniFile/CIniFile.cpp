#include "stdafx.h"
#include "CIniFile.h"


CIniFile::CIniFile()
{
	Unbind();
}

/// <summary>
/// 생성자 내부에서 Bind 함수 호출. 매개 변수 관한 설명 필히 확인. (_CheckParams 함수)
/// </summary>
/// <param name="eIniFile">ini 파일 종류</param>
/// <param name="eMode">bind 모드</param>
/// <param name="pArrParams">경로를 만들때 활용하는 배열. _CheckParams  필수 확인.</param>
CIniFile::CIniFile(INI_TYPE eIniFile, INI_MODE eMode, IN CStringArray * pArrParams)
{
	Bind(eIniFile, eMode, pArrParams);
}

CIniFile::~CIniFile()
{
	if(_IsBind())
		Unbind();
}

/// <summary>
/// 어떤 ini 파일에 대해서 다룰 것인지 정하는 함수. 매개 변수 관한 설명 필히 확인. (_CheckParams 함수)
/// </summary>
/// <param name="eIniFile">ini 파일 종류</param>
/// <param name="eMode">bind 모드</param>
/// <param name="pArrParams">경로를 만들때 활용하는 배열.</param>
void CIniFile::Bind(INI_TYPE eIniFile, INI_MODE eMode, IN CStringArray* pArrParams)
{
	ASSERT(!_IsBind());
	_CheckParams(eIniFile, pArrParams);

	m_eMode = eMode;

	_SetIniPath(eIniFile, pArrParams);
	ASSERT(_IsBind());

	switch (m_eMode)
	{
	case INI_MODE_WRITE:
		if (_taccess(m_strIniPath, 0) == -1)
			_Create();

	case INI_MODE_MIGRATE:
		_Migrate();

	case INI_MODE_READ:
		break;

	default:
		ASSERT(FALSE);
		break;
	}
}

void CIniFile::Unbind()
{
	m_strIniPath.Empty();
	m_eMode = INI_MODE_READ;
}

BOOL CIniFile::_IsBind() const
{
	return !m_strIniPath.IsEmpty();
}

void CIniFile::_WriteVersion(INT nVersion) const
{
	ASSERT(_IsBind());
	ASSERT(m_eMode != INI_MODE_READ);

	CString strVersion;
	strVersion.Format(_T("%d"), nVersion);
	WritePrivateProfileString(_T("VersionInfo"), _T("Version"), strVersion, m_strIniPath);
}

INT CIniFile::_ReadVersion() const
{
	ASSERT(_IsBind());
	ASSERT(m_eMode != INI_MODE_READ);

	return GetPrivateProfileInt(_T("VersionInfo"), _T("Version"), 0, m_strIniPath);;
}

void CIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue) const
{
	ASSERT(_IsBind());
	ASSERT(m_eMode == INI_MODE_WRITE);

	WritePrivateProfileString(lpszSection, lpszKey, lpszValue, m_strIniPath);
}

void CIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, INT nValue) const
{
	ASSERT(_IsBind());
	ASSERT(m_eMode == INI_MODE_WRITE);

	CString strValue;
	strValue.Format(_T("%d"), nValue);
	WritePrivateProfileString(lpszSection, lpszKey, strValue, m_strIniPath);
}

void CIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue, OUT CString & strValue) const
{
	ASSERT(_IsBind());
	ASSERT(m_eMode != INI_MODE_MIGRATE);

	GetPrivateProfileString(lpszSection, lpszKey, lpszDefaultValue, strValue.GetBuffer(BUFFER_SIZE_VALUE), BUFFER_SIZE_VALUE, m_strIniPath);
	strValue.ReleaseBuffer();
}

void CIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, INT dwDefaultValue, OUT INT & dwValue) const
{
	ASSERT(_IsBind());
	ASSERT(m_eMode != INI_MODE_MIGRATE);

	dwValue = GetPrivateProfileInt(lpszSection, lpszKey, dwDefaultValue, m_strIniPath);
}

/// <summary>
/// 마이그레이션. 존재하고 있는 ini 파일에 대해서만 수행한다.
/// </summary>
/// <remarks> 버전 추가 시 LATEST_VERSION 값 갱신할 것.</remarks>
void CIniFile::_Migrate() const
{
	ASSERT(_IsBind());
	ASSERT(m_eMode != INI_MODE_READ);

	if (_taccess(m_strIniPath, 0) == -1)
		return;

	int nCurVersion = _ReadVersion();
	if (LATEST_VERSION <= nCurVersion)
		return;

	for (int nVersion = nCurVersion + 1; nVersion <= LATEST_VERSION; nVersion++)
	{
		BOOL bSuccess = TRUE;
		switch (nVersion)
		{
		case 1:
		{
			bSuccess = _ConvertToUnicode();
			break;
		}
		default:
			ASSERT(FALSE);
			break;
		}

		if (!bSuccess)
		{
			TRACE(_T("CIniFile::_Migrate - Failed to migrate path [%s] version [%d] -> [%d], LATEST_VERSION [%d] Error [%x]\r\n"), m_strIniPath, nCurVersion, nVersion, LATEST_VERSION, GetLastError());
			break;
		}

		_WriteVersion(nVersion);
	}
	
	return;
}

/// <summary>
///  ini 파일을 unicode 화 하는 작업. 
/// </summary>
/// <returns></returns>
BOOL CIniFile::_ConvertToUnicode() const
{
	ASSERT(_IsBind());

	CString strMigrationPath;
	strMigrationPath.Format(_T("%s.migrate"), m_strIniPath);

	CFile iniMigrationFile;
	if (!iniMigrationFile.Open(strMigrationPath, CFile::modeCreate | CFile::modeWrite))
	{
		TRACE(_T("CIniFile::_ConvertToUnicode - Failed to Create Migration Ini - path[%s], [%x]\r\n"), strMigrationPath, GetLastError());
		return FALSE;
	}

	WCHAR bom = 0xFEFF;
	iniMigrationFile.Write(&bom, sizeof(bom));
	iniMigrationFile.Close();

	TCHAR arrSections[BUFFER_SIZE_SECTION] = { 0 };
	DWORD dwSize = GetPrivateProfileSectionNames(arrSections, BUFFER_SIZE_SECTION, m_strIniPath);
	if (dwSize == 0)
		return TRUE;

	BOOL bSuccess = TRUE;
	TCHAR* pSection = arrSections;
	while (*pSection != _T('\0'))
	{
		CString strSection = pSection;

		TCHAR arrKeys[BUFFER_SIZE_SECTION] = { 0 };
		DWORD dwKeySize = GetPrivateProfileSection(strSection, arrKeys, BUFFER_SIZE_SECTION, m_strIniPath);
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
					if (!WritePrivateProfileString(strSection, strKey, strValue, strMigrationPath))
					{
						TRACE(_T("CIniFile::_ConvertToUnicode - Failed to Write In MigrationFile - path[%s], section[%s], key[%s], value[%s], [%x]\r\n"), m_strIniPath, strSection, strKey, strValue, GetLastError());
						bSuccess = FALSE;
					}
				}

				pKeyValue += lstrlen(pKeyValue) + 1;
			}
		}
		pSection += lstrlen(pSection) + 1;
	}

	if (!bSuccess)
		return FALSE;

	if (!CopyFile(strMigrationPath, m_strIniPath, FALSE))
	{
		TRACE(_T("CIniFile::_ConvertToUnicode - Failed to Copy Migration To Origin - migration path[%s], origin path[%s], [%x]\r\n"), strMigrationPath, m_strIniPath, GetLastError());
		return FALSE;
	}

	if (!DeleteFile(strMigrationPath))
	{
		TRACE(_T("CIniFile::_ConvertToUnicode - Failed to Delete Migration - migration path[%s], [%x]\r\n"), strMigrationPath, GetLastError());
		// 삭제 실패는 실패로 처리하지 않음. 
	}

	return TRUE;
}

void CIniFile::_Create() const
{
	ASSERT(_IsBind());

	::SHCreateDirectory(NULL, m_strIniPath.Left(m_strIniPath.ReverseFind(_T('\\'))));

	CFile iniFile;
	if (!iniFile.Open(m_strIniPath, CFile::modeCreate | CFile::modeWrite))
	{
		TRACE(_T("CIniFile::_Create - Failed to Create Ini - path[%s], [%x]\r\n"), m_strIniPath, GetLastError());
		return;
	}

	WCHAR bom = 0xFEFF;
	iniFile.Write(&bom, sizeof(bom));
	iniFile.Close();
	
	_WriteVersion(1);
	return;
}


/// <summary>
/// 경로에 추가 정보가 필요한 경우. 배열로 받는데. 유효한지 판단하는 함수. 
/// 주석에 맞게. 배열을 할당해서 넘겨야 한다. 
/// </summary>
/// <param name="eIniFile"></param>
/// <param name="pArrParams"></param>
/// <remarks> ini 타입 추가 시 !주석 필수 작성! </remarks>
void CIniFile::_CheckParams(INI_TYPE eIniFile, CStringArray* pArrParams) const
{
	// Parameter Check
	switch (eIniFile)
	{
	case INI_TYPE_POLICY_INFO:
	case INI_TYPE_DISKLOCK:
		ASSERT(pArrParams != nullptr && pArrParams->GetSize() == 1);
		ASSERT(!pArrParams->GetAt(0).IsEmpty()); // SID
		break;

	case INI_TYPE_ALARM_INFO:
	case INI_TYPE_IMPORTANT_ALARM_INFO:
		ASSERT(pArrParams != nullptr && pArrParams->GetSize() == 2);
		ASSERT(!pArrParams->GetAt(0).IsEmpty()); // SiteID
		ASSERT(!pArrParams->GetAt(1).IsEmpty()); // UserID
		break;

	case INI_TYPE_SECUREDISK_BACKUP:
	case INI_TYPE_PC_BACKUP:
	case INI_TYPE_COLLECT_POLICY:
		ASSERT(pArrParams != nullptr && pArrParams->GetSize() == 3);
		ASSERT(!pArrParams->GetAt(0).IsEmpty()); // SID
		ASSERT(!pArrParams->GetAt(1).IsEmpty()); // SiteID
		ASSERT(!pArrParams->GetAt(2).IsEmpty()); // UserID
		break;

	case INI_TYPE_BACKUP:
		ASSERT(pArrParams != nullptr && pArrParams->GetSize() == 3);
		ASSERT(!pArrParams->GetAt(0).IsEmpty()); // SiteID
		ASSERT(!pArrParams->GetAt(1).IsEmpty()); // DomainID
		ASSERT(!pArrParams->GetAt(2).IsEmpty()); // UserID
		break;

	case INI_TYPE_EXPORT_INFO:
		ASSERT(pArrParams != nullptr && pArrParams->GetSize() == 1);
		ASSERT(!pArrParams->GetAt(0).IsEmpty()); // folder
		break;

	case INI_TYPE_RECYCLE_BIN:
		ASSERT(pArrParams != nullptr && pArrParams->GetSize() == 1);
		ASSERT(!pArrParams->GetAt(0).IsEmpty()); // folder
		break;

	case INI_TYPE_LOCAL_APP_CATEGORY:
		ASSERT(pArrParams != nullptr && pArrParams->GetSize() == 1);
		ASSERT(!pArrParams->GetAt(0).IsEmpty()); // DRM drive letter
		break;

	// No Contract
	case INI_TYPE_TOTAL_BASE_POLICY_INFO:
	case INI_TYPE_TOTAL_ID_POLICY_INFO:
	case INI_TYPE_POLICY:
	case INI_TYPE_COMMON:
	case INI_TYPE_SHARED_WORKDRIVE_INFO:
	case INI_TYPE_LANG:
		ASSERT(pArrParams == nullptr);
		break;
	}

	return;
}

/// <summary>
///  ini 파일 경로를 할당하는 부분. 
/// </summary>
/// <param name="eIniFile">ini 파일 enum 타입</param>
/// <param name="pArrParams"> 선택적, 경로에 추가 정보가 필요한 경우 </param>
/// <returns></returns>
/// <remarks>m_strIniPath</remarks>
void CIniFile::_SetIniPath(INI_TYPE eIniFile, CStringArray * pArrParams)
{
	CString strHeadPath;

	switch (eIniFile)
	{
	// Window Drive
	case INI_TYPE_POLICY_INFO:
	case INI_TYPE_TOTAL_BASE_POLICY_INFO:
	case INI_TYPE_TOTAL_ID_POLICY_INFO:
	case INI_TYPE_POLICY:
	case INI_TYPE_SECUREDISK_BACKUP:
	case INI_TYPE_PC_BACKUP:
	case INI_TYPE_COLLECT_POLICY:
	{
		if (!GetWindowsDirectory(strHeadPath.GetBuffer(MAX_PATH), MAX_PATH))
		{
			strHeadPath.ReleaseBuffer();
			TRACE(_T("CIniFile::_SetIniPath - Failed to get windows directory path [%s] Error [%x]\r\n"), m_strIniPath, GetLastError());
			return;
		}

		strHeadPath.ReleaseBuffer();
		strHeadPath = strHeadPath.Left(strHeadPath.Find(_T(':')) + 1);
		break;
	}

	// System Drive
	case INI_TYPE_COMMON:
	case INI_TYPE_DISKLOCK:
	{
		if (!GetSystemDirectory(strHeadPath.GetBuffer(MAX_PATH), MAX_PATH))
		{
			strHeadPath.ReleaseBuffer();
			TRACE(_T("CIniFile::_SetIniPath - Failed to get system directory path [%s] Error [%x]\r\n"), m_strIniPath, GetLastError());
			return;
		}

		strHeadPath.ReleaseBuffer();
		strHeadPath = strHeadPath.Left(strHeadPath.Find(_T(':')) + 1);
		break;
	}

	// C: Fixed
	case INI_TYPE_SHARED_WORKDRIVE_INFO:
	{
		strHeadPath.Format(_T("%c:"), _T('C'));
		break;
	}

	// local app data
	case INI_TYPE_ALARM_INFO:
	case INI_TYPE_IMPORTANT_ALARM_INFO:
	case INI_TYPE_BACKUP:
	{
		if (!SHGetSpecialFolderPath(NULL, strHeadPath.GetBuffer(MAX_PATH), CSIDL_LOCAL_APPDATA, FALSE))
		{
			strHeadPath.ReleaseBuffer();
			TRACE(_T("CIniFile::_SetIniPath - Failed to get localappdata path [%s] Error [%x]\r\n"), m_strIniPath, GetLastError());
			return;
		}

		strHeadPath.ReleaseBuffer();
		break;
	}

	// Program Files
	case INI_TYPE_LANG:
	{
		if (!SHGetSpecialFolderPathW(NULL, strHeadPath.GetBuffer(MAX_PATH), CSIDL_PROGRAM_FILES, FALSE))
		{
			strHeadPath.ReleaseBuffer();
			TRACE(_T("CIniFile::_SetIniPath - Failed to get program files path [%s] Error [%x]\r\n"), m_strIniPath, GetLastError());
			return;
		}

		strHeadPath.ReleaseBuffer();
		break;
	}

	// param
	case INI_TYPE_EXPORT_INFO:
		strHeadPath = pArrParams->GetAt(0);
		break;

	case INI_TYPE_RECYCLE_BIN:
		strHeadPath = pArrParams->GetAt(0);
		break;

	case INI_TYPE_LOCAL_APP_CATEGORY:
		strHeadPath = pArrParams->GetAt(0);
		break;
	}


	switch (eIniFile)
	{
	case INI_TYPE_POLICY_INFO:
		m_strIniPath.Format(_T("%s\\DiskLock\\%s\\PolicyInfo.ini"), strHeadPath, pArrParams->GetAt(0));
		break;

	case INI_TYPE_TOTAL_BASE_POLICY_INFO:
		m_strIniPath.Format(_T("%s\\DiskLock\\TotalPolicy\\BASE\\PolicyInfo.ini"), strHeadPath);
		break;

	case INI_TYPE_TOTAL_ID_POLICY_INFO:
		m_strIniPath.Format(_T("%s\\DiskLock\\TotalPolicy\\%s\\PolicyInfo.ini"), strHeadPath, pArrParams->GetAt(0));
		break;

	case INI_TYPE_COMMON:
		m_strIniPath.Format(_T("%s\\DiskLock\\ReadOnly\\common.ini"), strHeadPath);
		break;

	case INI_TYPE_DISKLOCK:
		m_strIniPath.Format(_T("%s\\DiskLock\\ReadOnly\\%s\\disklock.ini"), strHeadPath, pArrParams->GetAt(0));
		break;

	case INI_TYPE_POLICY:
		m_strIniPath.Format(_T("%s\\DiskLock\\ReadOnly\\0\\Policy.ini"), strHeadPath);
		break;

	case INI_TYPE_SHARED_WORKDRIVE_INFO:
		m_strIniPath.Format(_T("%s\\DiskLock\\ReadOnly\\SharedWorkDriveInfo.ini"), strHeadPath);
		break;

	case INI_TYPE_ALARM_INFO:
		m_strIniPath.Format(_T("%s\\NetID\\DiskLock\\%s\\%s\\Alarm.ini"), strHeadPath, pArrParams->GetAt(0), pArrParams->GetAt(1));
		break;

	case INI_TYPE_IMPORTANT_ALARM_INFO:
		m_strIniPath.Format(_T("%s\\NetID\\DiskLock\\%s\\%s\\ImportantAlarmInfo.ini"), strHeadPath, pArrParams->GetAt(0), pArrParams->GetAt(1));
		break;

	case INI_TYPE_SECUREDISK_BACKUP:
		m_strIniPath.Format(_T("%s\\DiskLock\\%s\\%s\\%s\\SecureDiskBackup.ini"), strHeadPath, pArrParams->GetAt(0), pArrParams->GetAt(1), pArrParams->GetAt(2));
		break;

	case INI_TYPE_PC_BACKUP:
		m_strIniPath.Format(_T("%s\\DiskLock\\%s\\%s\\%s\\PCBackup.ini"), strHeadPath, pArrParams->GetAt(0), pArrParams->GetAt(1), pArrParams->GetAt(2));
		break;

	case INI_TYPE_COLLECT_POLICY:
		m_strIniPath.Format(_T("%s\\DiskLock\\%s\\%s\\%s\\CollectPolicy.ini"), strHeadPath, pArrParams->GetAt(0), pArrParams->GetAt(1), pArrParams->GetAt(2));
		break;

	case INI_TYPE_BACKUP:
		m_strIniPath.Format(_T("%s\\NetID\\PlusAutoBackup\\%s\\%s\\%s\\backup.ini"), strHeadPath, pArrParams->GetAt(0), pArrParams->GetAt(1), pArrParams->GetAt(2));
		break;

	case INI_TYPE_EXPORT_INFO:
		m_strIniPath.Format(_T("%s\\exportInfo.ini"), strHeadPath);
		break;

	case INI_TYPE_RECYCLE_BIN:
		m_strIniPath.Format(_T("%s\\RecycleBin.ini"), strHeadPath);
		break;

	case INI_TYPE_LOCAL_APP_CATEGORY:
		m_strIniPath.Format(_T("%s\\LocalAppCategory.ini"), strHeadPath);
		break;
	}

	return;
}
