#include "stdafx.h"
#include "CIniFile.h"



CIniFile::CIniFile()
	: m_strIniPath(_T("")), m_eMode(INI_MODE_READ)
{

}

CIniFile::CIniFile(
	INI_TYPE eIniFile,
	INI_MODE eMode,
	IN LPCTSTR lpszSID,
	IN const SITE_INFO * pSiteInfo,
	IN const USER_INFO * pUserInfo,
	IN LPCTSTR lpszParam1
)
{
	Open(eIniFile, eMode, lpszSID, pSiteInfo, pUserInfo, lpszParam1);
}

CIniFile::CIniFile(IN LPCTSTR lpszIniPath, INI_MODE eMode)
{
	Open(lpszIniPath, eMode);
}

CIniFile::~CIniFile()
{
	if (_IsOpen())
		Close();
}

void CIniFile::Open(
	INI_TYPE eIniFile,
	INI_MODE eMode,
	IN LPCTSTR lpszSID,
	IN const SITE_INFO * pSiteInfo,
	IN const USER_INFO * pUserInfo,
	IN LPCTSTR lpszParam1
)
{
	ASSERT(!_IsOpen());

	CString strSID(lpszSID);

	_CheckParams(eIniFile, strSID, pSiteInfo, pUserInfo, lpszParam1);

	m_eMode = eMode;

	_SetIniPath(eIniFile, strSID, pSiteInfo, pUserInfo, lpszParam1);
	ASSERT(_IsOpen());

	switch (m_eMode)
	{
	case INI_MODE_READ:
		break;
	case INI_MODE_WRITE:
		if (_taccess(m_strIniPath, 0) == -1)
			_Create();

		Migrate();
		break;
	case INI_MODE_MIGRATION:
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CIniFile::Open(IN LPCTSTR lpszIniPath, INI_MODE eMode)
{
	m_eMode = eMode;
	m_strIniPath.Format(_T("%s"), lpszIniPath);

	ASSERT(IsIniFile(m_strIniPath));
	ASSERT(_IsOpen());

	switch (m_eMode)
	{
	case INI_MODE_READ:
		break;
	case INI_MODE_WRITE:
		if (_taccess(m_strIniPath, 0) == -1)
			_Create();

		Migrate();
		break;
	case INI_MODE_MIGRATION:
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CIniFile::Close()
{
	m_strIniPath.Empty();
	m_eMode = INI_MODE_READ;
}

void CIniFile::_CheckParams(
	INI_TYPE eIniFile,
	IN LPCTSTR lpszSID,
	IN const SITE_INFO* pSiteInfo,
	IN const USER_INFO* pUserInfo,
	IN LPCTSTR lpszParam1
) const
{
	// Parameter Check
	switch (eIniFile)
	{
	case INI_TYPE_POLICY_INFO:
	case INI_TYPE_TOTAL_BASE_POLICY_INFO:
	case INI_TYPE_DISKLOCK:
		ASSERT(lpszSID != nullptr);
		break;

	case INI_TYPE_TOTAL_ID_POLICY_INFO:
		ASSERT(lpszSID != nullptr);
		ASSERT(lpszParam1 != nullptr);	// Policy ID 
		break;

	case INI_TYPE_ALARM_INFO:
	case INI_TYPE_IMPORTANT_ALARM_INFO:
	case INI_TYPE_BACKUP:
		ASSERT(pSiteInfo != nullptr);
		ASSERT(pUserInfo != nullptr);
		break;

	case INI_TYPE_SECUREDISK_BACKUP:
	case INI_TYPE_PC_BACKUP:
	case INI_TYPE_COLLECT_POLICY:
		ASSERT(lpszSID != nullptr);
		ASSERT(pSiteInfo != nullptr);
		ASSERT(pUserInfo != nullptr);
		break;

	case INI_TYPE_EXPORT_INFO:
	case INI_TYPE_RECYCLE_BIN:
	case INI_TYPE_LOCAL_APP_CATEGORY:
		ASSERT(lpszParam1 != nullptr);	// Custom Path
		break;

	case INI_TYPE_POLICY:
	case INI_TYPE_COMMON:
	case INI_TYPE_SHARED_WORKDRIVE_INFO:
		//case INI_TYPE_LANG:
		break;
	}

	return;
}


void CIniFile::_SetIniPath(
	INI_TYPE eIniFile,
	IN LPCTSTR lpszSID,
	IN const SITE_INFO* pSiteInfo,
	IN const USER_INFO* pUserInfo,
	IN LPCTSTR lpszParam1
)
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
	case INI_TYPE_COMMON:
	case INI_TYPE_DISKLOCK:
	case INI_TYPE_SHARED_WORKDRIVE_INFO:
	{
		if (!GetWindowsDirectory(strHeadPath.GetBuffer(MAX_PATH), MAX_PATH))
		{
			strHeadPath.ReleaseBuffer();
			wprintf( _T("CIniFile::_SetIniPath - Failed to get windows directory path [%s] Error [%x]\r\n"), m_strIniPath.GetString(), GetLastError());
			return;
		}

		strHeadPath.ReleaseBuffer();
		strHeadPath = strHeadPath.Left(strHeadPath.Find(_T(':')) + 1);
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
			wprintf( _T("CIniFile::_SetIniPath - Failed to get localappdata path [%s] Error [%x]\r\n"), m_strIniPath.GetString(), GetLastError());
			return;
		}

		strHeadPath.ReleaseBuffer();
		break;
	}

	// param
	case INI_TYPE_EXPORT_INFO:
	case INI_TYPE_RECYCLE_BIN:
	case INI_TYPE_LOCAL_APP_CATEGORY:
		strHeadPath = lpszParam1;
		break;
	}

	switch (eIniFile)
	{
	case INI_TYPE_POLICY_INFO:
		m_strIniPath.Format(_T("%s\\DiskLock\\%s\\PolicyInfo.ini"), strHeadPath, lpszSID);
		break;

	case INI_TYPE_TOTAL_BASE_POLICY_INFO:
		m_strIniPath.Format(_T("%s\\DiskLock\\%s\\TotalPolicy\\BASE\\PolicyInfo.ini"), strHeadPath, lpszSID);
		break;

	case INI_TYPE_TOTAL_ID_POLICY_INFO:
		m_strIniPath.Format(_T("%s\\DiskLock\\%s\\TotalPolicy\\%s\\PolicyInfo.ini"), strHeadPath, lpszSID, lpszParam1);
		break;

	case INI_TYPE_COMMON:
		m_strIniPath.Format(_T("%s\\DiskLock\\ReadOnly\\common.ini"), strHeadPath);
		break;

	case INI_TYPE_DISKLOCK:
		m_strIniPath.Format(_T("%s\\DiskLock\\ReadOnly\\%s\\disklock.ini"), strHeadPath, lpszSID);
		break;

	case INI_TYPE_POLICY:
		m_strIniPath.Format(_T("%s\\DiskLock\\ReadOnly\\0\\Policy.ini"), strHeadPath);
		break;

	case INI_TYPE_SHARED_WORKDRIVE_INFO:
		m_strIniPath.Format(_T("%s\\DiskLock\\ReadOnly\\SharedWorkDriveInfo.ini"), strHeadPath);
		break;

	case INI_TYPE_ALARM_INFO:
		m_strIniPath.Format(_T("%s\\NetID\\DiskLock\\%s\\%s\\Alarm.ini"), strHeadPath, pSiteInfo->strSiteID, pUserInfo->strUserID);
		break;

	case INI_TYPE_IMPORTANT_ALARM_INFO:
		m_strIniPath.Format(_T("%s\\NetID\\DiskLock\\%s\\%s\\ImportantAlarmInfo.ini"), strHeadPath, pSiteInfo->strSiteID, pUserInfo->strUserID);
		break;

	case INI_TYPE_SECUREDISK_BACKUP:
		m_strIniPath.Format(_T("%s\\DiskLock\\%s\\%s\\%s\\SecureDiskBackup.ini"), strHeadPath, lpszSID, pSiteInfo->strSiteID, pUserInfo->strUserID);
		break;

	case INI_TYPE_PC_BACKUP:
		m_strIniPath.Format(_T("%s\\DiskLock\\%s\\%s\\%s\\PCBackup.ini"), strHeadPath, lpszSID, pSiteInfo->strSiteID, pUserInfo->strUserID);
		break;

	case INI_TYPE_COLLECT_POLICY:
		m_strIniPath.Format(_T("%s\\DiskLock\\%s\\%s\\%s\\CollectPolicy.ini"), strHeadPath, lpszSID, pSiteInfo->strSiteID, pUserInfo->strUserID);
		break;

	case INI_TYPE_BACKUP:
		m_strIniPath.Format(_T("%s\\NetID\\PlusAutoBackup\\%s\\%s\\backup.ini"), strHeadPath, lpszSID, pSiteInfo->strSiteID, pUserInfo->strUserID);
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

	ASSERT(IsIniFile(m_strIniPath));
	return;
}

BOOL CIniFile::_IsOpen() const
{
	return !m_strIniPath.IsEmpty();
}

BOOL CIniFile::_WriteVersion(INT nVersion) const
{
	ASSERT(_IsOpen());
	ASSERT(m_eMode != INI_MODE_READ);

	CString strVersion;
	strVersion.Format(_T("%d"), nVersion);

	BOOL bResult = TRUE;
	bResult &= WritePrivateProfileString(_T("VersionInfo"), _T("Version"), strVersion, m_strIniPath);
	//bResult &= WritePrivateProfileString(_T("VersionInfo"), _T("Timestamp"), COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), m_strIniPath);

	return bResult;
}

INT CIniFile::_ReadVersion() const
{
	ASSERT(_IsOpen());
	ASSERT(m_eMode != INI_MODE_READ);

	return GetPrivateProfileInt(_T("VersionInfo"), _T("Version"), 0, m_strIniPath);
}

BOOL CIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue) const
{
	ASSERT(_IsOpen());
	ASSERT(m_eMode == INI_MODE_WRITE);

	return WritePrivateProfileString(lpszSection, lpszKey, lpszValue, m_strIniPath);
}

BOOL CIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, INT nValue) const
{
	ASSERT(_IsOpen());
	ASSERT(m_eMode == INI_MODE_WRITE);

	CString strValue;
	strValue.Format(_T("%d"), nValue);
	return WritePrivateProfileString(lpszSection, lpszKey, strValue, m_strIniPath);
}

void CIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue, OUT CString & strValue) const
{
	ASSERT(_IsOpen());

	GetPrivateProfileString(lpszSection, lpszKey, lpszDefaultValue, strValue.GetBuffer(BUFFER_SIZE_VALUE), BUFFER_SIZE_VALUE, m_strIniPath);
	strValue.ReleaseBuffer();
}

void CIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, INT dwDefaultValue, OUT INT & dwValue) const
{
	ASSERT(_IsOpen());

	dwValue = GetPrivateProfileInt(lpszSection, lpszKey, dwDefaultValue, m_strIniPath);
}

BOOL CIniFile::MigrateAll()
{
	wprintf( _T("CIniFile::MigrateAll Start\r\n"));

	// REGISTRY 검사. 
	LPCTSTR lpszKey = _T("Software\\NetID\\Migration");
	CRegKey reg;
	LSTATUS ret = reg.Open(HKEY_CURRENT_USER, lpszKey, KEY_ALL_ACCESS);
	if (ret != ERROR_SUCCESS)
	{
		ret = reg.Create(HKEY_CURRENT_USER, lpszKey);
		if (ret != ERROR_SUCCESS)
		{
			wprintf( _T("CIniFile::MigrateAll - Failed to Create Registry - key [%s], reg error [%ld]\r\n"), lpszKey, ret);
			return FALSE;
		}
	}

	DWORD dwVersion = 0;
	ret = reg.QueryDWORDValue(_T("dwIniVersion"), dwVersion);
	if (ret == ERROR_SUCCESS && dwVersion >= (DWORD)LATEST_VERSION)
		return TRUE;

	CStringArray arrBasePath;
	CString strAddPath;
	if (!GetWindowsDirectory(strAddPath.GetBuffer(MAX_PATH), MAX_PATH))
	{
		strAddPath.ReleaseBuffer();
		wprintf( _T("CIniFile::MigrateAll - Failed to get windows directory Error [%x]\r\n"), GetLastError());
		return FALSE;
	}

	strAddPath.ReleaseBuffer();
	strAddPath = strAddPath.Left(strAddPath.Find(_T(':')) + 1);
	arrBasePath.Add(strAddPath);

	if (!SHGetSpecialFolderPath(NULL, strAddPath.GetBuffer(MAX_PATH), CSIDL_LOCAL_APPDATA, FALSE))
	{
		strAddPath.ReleaseBuffer();
		wprintf( _T("CIniFile::MigrateAll - Failed to get localappdata Error [%x]\r\n"), GetLastError());
		return FALSE;
	}

	strAddPath.ReleaseBuffer();
	arrBasePath.Add(strAddPath);

	BOOL bResult = TRUE;
	for (int i = 0; i < arrBasePath.GetSize(); i++)
	{
		CStringList listPath;
		listPath.AddTail(arrBasePath.GetAt(0));

		while (!listPath.IsEmpty())
		{
			CString curPath = listPath.GetHead();
			listPath.RemoveHead();

			if (curPath.IsEmpty())
				continue;

			CFileFind finder;
			CString strPattern;
			strPattern.Format(_T("%s\\*"), curPath);

			if (!finder.FindFile(strPattern))
				continue;

			while (finder.FindNextFile())
			{
				if (finder.IsDots())
					continue;

				if (finder.IsDirectory())
				{
					listPath.AddTail(finder.GetFilePath());
					continue;
				}

				CString strFileName = finder.GetFileName();
				auto t = strFileName.Right(strFileName.ReverseFind(_T('.')));

				if (!IsIniFile(strFileName))
					continue;

				CIniFile iniFile(strFileName, INI_MODE_WRITE);
				RESULT_MIGRATION eResult = iniFile.Migrate();
				if (eResult == RESULT_MIGRATION_FAILED)
				{
					bResult = FALSE;
					wprintf( _T("CIniFile::MigrateAll - Failed to Migrate - path[%s], [%x]\r\n"), strFileName.GetString(), GetLastError());
					continue;
				}

				wprintf(_T("CIniFile::MigrateAll - Pass  - path[%s], migration result[%d]"), strFileName.GetString(), (int)eResult);
			}
		}
	} // for end

	if (bResult)
	{
		ret = reg.SetDWORDValue(_T("dwIniVersion"), (DWORD)LATEST_VERSION);
		if (ret != ERROR_SUCCESS)
		{
			wprintf( _T("CIniFile::MigrateAll - Failed to write ini version reg - key[%s], reg error[%ld]\r\n"), lpszKey, ret);
			return FALSE;
		}

		//reg.SetStringValue(_T("IniTimeStamp"), COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")));
	}

	wprintf( _T("CIniFile::MigrateAll End\r\n"));
	return TRUE;
}


BOOL CIniFile::IsIniFile(const CString& strFileName)
{
	return strFileName.Right(strFileName.ReverseFind(_T('.'))) == _T(".ini");
}


/// <summary>
/// 마이그레이션. 존재하고 있는 ini 파일에 대해서만 수행한다.
/// </summary>
/// <returns> 마이그레이션 성공 실패 여부</returns>
/// <remarks> 버전 추가 시 LATEST_VERSION 값 갱신할 것.</remarks>
RESULT_MIGRATION CIniFile::Migrate() const
{
	ASSERT(_IsOpen());
	ASSERT(m_eMode != INI_MODE_READ);

	if (_taccess(m_strIniPath, 0) == -1)
		return RESULT_MIGRATION_NOT_FOUND;

	int nCurVersion = _ReadVersion();
	if (LATEST_VERSION <= nCurVersion)
		return RESULT_MIGRATION_NOT_REQUIRED;

	for (int nVersion = nCurVersion + 1; nVersion <= LATEST_VERSION; nVersion++)
	{
		BOOL bSuccess = FALSE;
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
			wprintf( _T("CIniFile::Migrate - Failed to migrate path [%s] version [%d] -> [%d], LATEST_VERSION [%d] Error [%x]\r\n"), m_strIniPath.GetString(), nCurVersion, nVersion, LATEST_VERSION, GetLastError());
			return RESULT_MIGRATION_FAILED;
		}

		if (!_WriteVersion(nVersion))
		{
			wprintf( _T("CIniFile::Migrate - Failed to Write Version path [%s] version [%d] -> [%d], LATEST_VERSION [%d] Error [%x]\r\n"), m_strIniPath.GetString(), nCurVersion, nVersion, LATEST_VERSION, GetLastError());
			return RESULT_MIGRATION_FAILED;
		}
	}

	return RESULT_MIGRATION_SUCCESS;
}


/// <summary>
///  ini 파일을 unicode 화 하는 작업. 
/// </summary>
/// <returns></returns>
BOOL CIniFile::_ConvertToUnicode() const
{
	ASSERT(_IsOpen());

	CString strMigrationPath;
	strMigrationPath.Format(_T("%s.migrate"), m_strIniPath);

	CFile iniMigrationFile;
	if (!iniMigrationFile.Open(strMigrationPath, CFile::modeCreate | CFile::modeWrite))
	{
		wprintf( _T("CIniFile::_ConvertToUnicode - Failed to Create Migration Ini - path[%s], [%x]\r\n"), strMigrationPath.GetString(), GetLastError());
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
						wprintf( _T("CIniFile::_ConvertToUnicode - Failed to Write In MigrationFile - path[%s], section[%s], key[%s], value[%s], [%x]\r\n"), m_strIniPath.GetString(), strSection.GetString(), strKey.GetString(), strValue.GetString(), GetLastError());
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
		wprintf( _T("CIniFile::_ConvertToUnicode - Failed to Copy Migration To Origin - migration path[%s], origin path[%s], [%x]\r\n"), strMigrationPath.GetString(), m_strIniPath.GetString(), GetLastError());
		return FALSE;
	}

	if (!DeleteFile(strMigrationPath))
	{
		wprintf(_T("CIniFile::_ConvertToUnicode - Failed to Delete Migration - migration path[%s], [%x]\r\n"), strMigrationPath.GetString(), GetLastError());
		// 삭제 실패는 실패로 처리하지 않음. 
	}

	return TRUE;
}

void CIniFile::_Create() const
{
	ASSERT(_IsOpen());

	::SHCreateDirectory(NULL, m_strIniPath.Left(m_strIniPath.ReverseFind(_T('\\'))));

	CFile iniFile;
	if (!iniFile.Open(m_strIniPath, CFile::modeCreate | CFile::modeWrite))
	{
		wprintf( _T("CIniFile::_Create - Failed to Create Ini - path[%s], [%x]\r\n"), m_strIniPath.GetString(), GetLastError());
		return;
	}

	WCHAR bom = 0xFEFF;
	iniFile.Write(&bom, sizeof(bom));
	iniFile.Close();

	if (!_WriteVersion(LATEST_VERSION))
		wprintf( _T("CIniFile::_Create - Failed to WriteVersion Ini - path[%s], version[%d], [%x]\r\n"), m_strIniPath.GetString(), LATEST_VERSION, GetLastError());

	return;
}