#include "stdafx.h"
#include "CIniFile.h"


CIniFile::CIniFile()
{
	Close();
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

	m_eMode = eMode;

	if (lpszSID == nullptr)
	{
		CString strSID(_T("TEMPSID!!"));
		_CheckParams(eIniFile, strSID, pSiteInfo, pUserInfo, lpszParam1);
		_SetIniPath(eIniFile, strSID, pSiteInfo, pUserInfo, lpszParam1);
	}
	else
	{
		_CheckParams(eIniFile, lpszSID, pSiteInfo, pUserInfo, lpszParam1);
		_SetIniPath(eIniFile, lpszSID, pSiteInfo, pUserInfo, lpszParam1);
	}


	ASSERT(_IsOpen());

	switch (m_eMode)
	{
	case INI_MODE_READ:
		break;
	case INI_MODE_WRITE:
		if (_taccess(m_strIniPath, 0) == -1)
			_Create();

		_Migrate();
		break;
	case INI_MODE_MIGRATE:
		_Migrate();
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
	case INI_TYPE_LANG:
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

	return;
}

BOOL CIniFile::_IsOpen() const
{
	return !m_strIniPath.IsEmpty();
}

void CIniFile::_WriteVersion(INT nVersion) const
{
	ASSERT(_IsOpen());
	ASSERT(m_eMode != INI_MODE_READ);

	CString strVersion;
	strVersion.Format(_T("%d"), nVersion);
	WritePrivateProfileString(_T("VersionInfo"), _T("Version"), strVersion, m_strIniPath);
}

INT CIniFile::_ReadVersion() const
{
	ASSERT(_IsOpen());
	ASSERT(m_eMode != INI_MODE_READ);

	return GetPrivateProfileInt(_T("VersionInfo"), _T("Version"), 0, m_strIniPath);;
}

void CIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue) const
{
	ASSERT(_IsOpen());
	ASSERT(m_eMode == INI_MODE_WRITE);

	WritePrivateProfileString(lpszSection, lpszKey, lpszValue, m_strIniPath);
}

void CIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, INT nValue) const
{
	ASSERT(_IsOpen());
	ASSERT(m_eMode == INI_MODE_WRITE);

	CString strValue;
	strValue.Format(_T("%d"), nValue);
	WritePrivateProfileString(lpszSection, lpszKey, strValue, m_strIniPath);
}

void CIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue, OUT CString & strValue) const
{
	ASSERT(_IsOpen());
	ASSERT(m_eMode != INI_MODE_MIGRATE);

	GetPrivateProfileString(lpszSection, lpszKey, lpszDefaultValue, strValue.GetBuffer(BUFFER_SIZE_VALUE), BUFFER_SIZE_VALUE, m_strIniPath);
	strValue.ReleaseBuffer();
}

void CIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, INT dwDefaultValue, OUT INT & dwValue) const
{
	ASSERT(_IsOpen());
	ASSERT(m_eMode != INI_MODE_MIGRATE);

	dwValue = GetPrivateProfileInt(lpszSection, lpszKey, dwDefaultValue, m_strIniPath);
}

/// <summary>
/// 마이그레이션. 존재하고 있는 ini 파일에 대해서만 수행한다.
/// </summary>
/// <remarks> 버전 추가 시 LATEST_VERSION 값 갱신할 것.</remarks>
void CIniFile::_Migrate() const
{
	ASSERT(_IsOpen());
	ASSERT(m_eMode != INI_MODE_READ);

	if (_taccess(m_strIniPath, 0) == -1)
		return;

	int nCurVersion = _ReadVersion();
	if (LATEST_VERSION <= nCurVersion)
		return;

	for (int nVersion = nCurVersion + 1; nVersion <= LATEST_VERSION; nVersion++)
	{
		BOOL bSuccess = FALSE;
		switch (nVersion)
		{
		case 1:
		{
			bSuccess &= _ConvertToUnicode();
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
	ASSERT(_IsOpen());

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
					if (WritePrivateProfileString(strSection, strKey, strValue, strMigrationPath))
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
	ASSERT(_IsOpen());

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