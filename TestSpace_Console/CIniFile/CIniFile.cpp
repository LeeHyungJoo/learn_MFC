#include "stdafx.h"
#include "CIniFile.h"

BOOL npConvertFileToUnicode(LPCTSTR lpszSrcPath, int nCodePage = CP_ACP)
{
	CFile fileSrc;
	if (!fileSrc.Open(lpszSrcPath, CFile::modeRead))
	{
		TRACE( _T("npConvertFileToUnicode - Failed to open src - path[%s], errno[%d]\r\n"), lpszSrcPath, GetLastError());
		return FALSE;
	}

	WCHAR wch;
	// 파일 시작에 BOM 이 있으면 이미 유니코드 파일로 판단하고 변환 생략 -> 성공 처리
	if (fileSrc.Read(&wch, sizeof(WCHAR)) && wch == 0xFEFF)
		return TRUE;

	fileSrc.SeekToBegin();

	// 임시 파일 경로 생성 ex) disklock.ini → disklock.ini.unicode
	CString strDstPath;
	strDstPath.Format(_T("%s.unicode"), lpszSrcPath);

	CFile fileDst;
	if (!fileDst.Open(strDstPath, CFile::modeCreate | CFile::modeWrite))
	{
		TRACE( _T("npConvertFileToUnicode - Failed to create dst - path[%s], errno[%d]\r\n"), strDstPath, GetLastError());
		return FALSE;
	}

	TRY
	{
		// 유니코드 BOM 쓰기.
		WCHAR bom = 0xFEFF;
	fileDst.Write(&bom, sizeof(bom));

	// 파일을 줄 단위(라인 단위)로 끝까지 읽으며 변환 수행
	while (TRUE)
	{
		CByteArray arrByte;
		BYTE byte;

		// 한 줄 읽기 0x0A(\n)을 만날 때까지 1바이트씩 읽음
		while (fileSrc.Read(&byte, 1))
		{
			arrByte.Add(byte);
			if (byte == 0x0A) break;
		}

		// 읽은 데이터가 없다면 종료
		if (arrByte.IsEmpty())
			break;

		// 유니코드로 변환할 버퍼 크기 계산
		int	nLen = ::MultiByteToWideChar(nCodePage, 0, (LPCSTR)arrByte.GetData(), (int)arrByte.GetSize(), NULL, 0);
		if (nLen == 0)
		{
			// 변환할 데이터는 있는데 버퍼 크기를 산정할 수 없는 상황.
			TRACE( _T("npConvertFileToUnicode - Failed to get buffer length (zero) - path[%s], byte array size[%d], errno[%d]\r\n"), strDstPath, (int)arrByte.GetSize(), GetLastError());
			continue;
		}

		// 변환 수행
		CArray<WCHAR, WCHAR> arrWch;
		arrWch.SetSize(nLen);
		::MultiByteToWideChar(nCodePage, 0, (LPCSTR)arrByte.GetData(), (int)arrByte.GetSize(), arrWch.GetData(), nLen);

		// 변환된 유니코드 문자열을 출력 파일에 쓰기
		fileDst.Write(arrWch.GetData(), nLen * sizeof(WCHAR));
	}

	// 삭제 및 이름 변경 전에 파일 닫기
	fileSrc.Close();
	fileDst.Close();
	}
		CATCH(CFileException, e)
	{
		TCHAR szErr[MAX_PATH] = { 0 };
		e->GetErrorMessage(szErr, MAX_PATH);
		TRACE( _T("npConvertFileToUnicode - Failed to write data - [%s], path[%s], errno[%d]\r\n"), szErr, strDstPath, GetLastError());
		e->Delete();
		return FALSE;
	}
	END_CATCH

		// 기존 파일 삭제
		if (!DeleteFile(lpszSrcPath))
		{
			TRACE( _T("npConvertFileToUnicode - Failed to delete dst - path[%s], errno[%d]\r\n"), strDstPath, GetLastError());
			return FALSE;
		}

	// 유니코드로 변환된 파일을 기존 파일로 대체 (이름 변경)
	if (!MoveFile(strDstPath, lpszSrcPath))
	{
		TRACE( _T("npConvertFileToUnicode - Failed to rename - dst path[%s], src path[%s], errno[%d]\r\n"), strDstPath, lpszSrcPath, GetLastError());
		return FALSE;
	}

	//성공
	return TRUE;
}

/// <summary>
///  기본 생성자
/// </summary>
/// <remarks> ini 경로는 비워두고 읽기 모드 설정</remarks>
CIniFile::CIniFile()
	: m_bReadOnly(TRUE)
{

}

/// <summary>
///  생성자 - 여러 문자열 매개 변수 기반으로 초기화 후 Open
/// </summary>
/// <param name="eIniFile">INI_TYPE</param>
/// <param name="bReadOnly">T : 읽기 전용 / F : 읽기/쓰기</param>
/// <param name="lpszSID">SID</param>
/// <param name="lpszSiteID">Site ID</param>
/// <param name="lpszUserID">User ID</param>
/// <param name="lpszParam1">추가 파라미터 (e.g. 정책 ID, 경로 등)</param>
CIniFile::CIniFile(
	INI_TYPE eIniFile,
	BOOL bReadOnly,
	IN LPCTSTR lpszSID,
	IN LPCTSTR lpszSiteID,
	IN LPCTSTR lpszUserID,
	IN LPCTSTR lpszParam1
)
{
	Open(eIniFile, bReadOnly, lpszSID, lpszSiteID, lpszUserID, lpszParam1);
}

/// <summary>
///  생성자 - 파일 경로를 통해 초기화 후 Open
/// </summary>
/// <param name="lpszIniPath">ini 파일 경로</param>
/// <param name="bReadOnly">T : 읽기 전용 / F : 읽기/쓰기</param>
CIniFile::CIniFile(
	IN LPCTSTR lpszIniPath,
	BOOL bReadOnly
)
{
	Open(lpszIniPath, bReadOnly);
}

/// <summary>
/// 소멸자
/// </summary>
CIniFile::~CIniFile()
{
	Close();
}

/// <summary>
///  설정 정보 기반으로 ini 파일 열기
/// </summary>
/// <param name="eIniFile">INI_TYPE</param>
/// <param name="bReadOnly">T : 읽기 전용 / F : 읽기/쓰기</param>
/// <param name="lpszSID">SID</param>
/// <param name="lpszSiteID">Site ID</param>
/// <param name="lpszUserID">User ID</param>
/// <param name="lpszParam1">추가 파라미터 (e.g. 정책 ID, 경로 등)</param>
BOOL CIniFile::Open(
	INI_TYPE eIniFile,
	BOOL bReadOnly,
	IN LPCTSTR lpszSID,
	IN LPCTSTR lpszSiteID,
	IN LPCTSTR lpszUserID,
	IN LPCTSTR lpszParam1
)
{
	CString strSID;
	if (lpszSID == nullptr)
		GetSID(strSID);
	else
		strSID = lpszSID;

#ifdef DEBUG
	_AssertParams(eIniFile, strSID, lpszSiteID, lpszUserID, lpszParam1);
#endif

	CString strIniPath;
	_MakeIniPath(eIniFile, strSID, lpszSiteID, lpszUserID, lpszParam1, strIniPath);
	return Open(strIniPath, bReadOnly);
}

/// <summary>
///  파일 경로로 ini 파일 열기
/// </summary>
/// <param name="lpszIniPath">ini 파일 경로</param>
/// <param name="bReadOnly">T : 읽기 전용 / F : 읽기/쓰기</param>
BOOL CIniFile::Open(IN LPCTSTR lpszIniPath, BOOL bReadOnly)
{
	m_bReadOnly = bReadOnly;
	m_strIniPath = lpszIniPath;

	ASSERT(IsIniFile(m_strIniPath));

	if (bReadOnly)
		return TRUE;

	if (_taccess(m_strIniPath, 0) != -1)
		return TRUE;

	return _Create();
}

/// <summary>
/// CIniFile 클래스 초기화 (경로 비우고, 읽기 전용)
/// </summary>
void CIniFile::Close()
{
	m_strIniPath.Empty();
	m_bReadOnly = TRUE;
}

/// <summary>
///  주어진 매개 변수들을 활용해, ini 파일 경로를 설정함
/// </summary>
/// <param name="eIniFile">INI_TYPE</param>
/// <param name="lpszSID">SID</param>
/// <param name="lpszSiteID">Site ID</param>
/// <param name="lpszUserID">User ID</param>
/// <param name="lpszParam1">추가 파라미터 (e.g. 정책 ID, 경로 등)</param>
/// <param name="strIniPath">반환할 ini 파일 경로</param>
void CIniFile::_MakeIniPath(
	INI_TYPE eIniFile,
	IN  LPCTSTR lpszSID,
	IN  LPCTSTR lpszSiteID,
	IN  LPCTSTR lpszUserID,
	IN  LPCTSTR lpszParam1,
	OUT CString& strIniPath
) const
{
	CString strHeadPath;

	switch (eIniFile)
	{
		// Window Drive
	case INI_TYPE_POLICY_INFO:
	case INI_TYPE_TOTAL_BASE_POLICY_INFO:
	case INI_TYPE_TOTAL_ID_POLICY_INFO:
	case INI_TYPE_SECUREDISK_INFO:
	case INI_TYPE_POLICY:
	case INI_TYPE_SECUREDISK_BACKUP:
	case INI_TYPE_PC_BACKUP:
	case INI_TYPE_COLLECT_POLICY:
	case INI_TYPE_COMMON:
	case INI_TYPE_DISKLOCK:
	case INI_TYPE_SHARED_WORKDRIVE_INFO:
	{
		TCHAR szPath[MAX_PATH] = { 0 };
		if (GetWindowsDirectory(szPath, MAX_PATH))
		{
			strHeadPath = szPath;
			strHeadPath = strHeadPath.Left(strHeadPath.Find(_T(':')) + 1);
		}
		else
		{
			TRACE( _T("CIniFile::_SetIniPath - Failed to get windows directory - path[%s] errno[%d]\r\n"), m_strIniPath, GetLastError());
			strHeadPath = _T('C');
		}

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
			TRACE( _T("CIniFile::_SetIniPath - Failed to get localappdata - path[%s] errno[%d]\r\n"), m_strIniPath, GetLastError());
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

	if (strHeadPath.IsEmpty())
		return;

	strHeadPath.Replace(_T('/'), _T('\\'));

	TCHAR chLast = strHeadPath.GetAt(strHeadPath.GetLength() - 1);
	if (chLast == _T('\\'))
		strHeadPath = strHeadPath.Left(strHeadPath.GetLength() - 1);

	switch (eIniFile)
	{
	case INI_TYPE_POLICY_INFO:
		strIniPath.Format(_T("%s\\DiskLock\\%s\\PolicyInfo.ini"), strHeadPath, lpszSID);
		break;

	case INI_TYPE_TOTAL_BASE_POLICY_INFO:
		strIniPath.Format(_T("%s\\DiskLock\\%s\\TotalPolicy\\BASE\\PolicyInfo.ini"), strHeadPath, lpszSID);
		break;

	case INI_TYPE_TOTAL_ID_POLICY_INFO:
		strIniPath.Format(_T("%s\\DiskLock\\%s\\TotalPolicy\\%s\\PolicyInfo.ini"), strHeadPath, lpszSID, lpszParam1);
		break;

	case INI_TYPE_SECUREDISK_INFO:
		strIniPath.Format(_T("%s\\DiskLock\\%s%s_SecreDisk.ini"), strHeadPath, lpszSiteID, lpszUserID);
		break;

	case INI_TYPE_COMMON:
		strIniPath.Format(_T("%s\\DiskLock\\ReadOnly\\common.ini"), strHeadPath);
		break;

	case INI_TYPE_DISKLOCK:
		strIniPath.Format(_T("%s\\DiskLock\\ReadOnly\\%s\\disklock.ini"), strHeadPath, lpszSID);
		break;

	case INI_TYPE_POLICY:
		strIniPath.Format(_T("%s\\DiskLock\\ReadOnly\\0\\Policy.ini"), strHeadPath);
		break;

	case INI_TYPE_SHARED_WORKDRIVE_INFO:
		strIniPath.Format(_T("%s\\DiskLock\\ReadOnly\\SharedWorkDriveInfo.ini"), strHeadPath);
		break;

	case INI_TYPE_ALARM_INFO:
		strIniPath.Format(_T("%s\\NetID\\DiskLock\\%s\\%s\\Alarm.ini"), strHeadPath, lpszSiteID, lpszUserID);
		break;

	case INI_TYPE_IMPORTANT_ALARM_INFO:
		strIniPath.Format(_T("%s\\NetID\\DiskLock\\%s\\%s\\ImportantAlarmInfo.ini"), strHeadPath, lpszSiteID, lpszUserID);
		break;

	case INI_TYPE_SECUREDISK_BACKUP:
		strIniPath.Format(_T("%s\\DiskLock\\%s\\%s\\%s\\SecureDiskBackup.ini"), strHeadPath, lpszSID, lpszSiteID, lpszUserID);
		break;

	case INI_TYPE_PC_BACKUP:
		strIniPath.Format(_T("%s\\DiskLock\\%s\\%s\\%s\\PCBackup.ini"), strHeadPath, lpszSID, lpszSiteID, lpszUserID);
		break;

	case INI_TYPE_COLLECT_POLICY:
		strIniPath.Format(_T("%s\\DiskLock\\%s\\%s\\%s\\CollectPolicy.ini"), strHeadPath, lpszSID, lpszSiteID, lpszUserID);
		break;

	case INI_TYPE_BACKUP:
		strIniPath.Format(_T("%s\\NetID\\PlusAutoBackup\\%s\\%s\\backup.ini"), strHeadPath, lpszSID, lpszSiteID, lpszUserID);
		break;

	case INI_TYPE_EXPORT_INFO:
		strIniPath.Format(_T("%s\\exportInfo.ini"), strHeadPath);
		break;

	case INI_TYPE_RECYCLE_BIN:
		strIniPath.Format(_T("%s\\RecycleBin.ini"), strHeadPath);
		break;

	case INI_TYPE_LOCAL_APP_CATEGORY:
		strIniPath.Format(_T("%s\\LocalAppCategory.ini"), strHeadPath);
		break;
	}

	ASSERT(IsIniFile(strIniPath));

	return;
}

/// <summary>
///  ini 파일에 버전 정보 쓰기
/// </summary>
/// <param name="nVersion">버전 번호</param>
/// <returns>성공 여부</returns>
BOOL CIniFile::_WriteVersion(INT nVersion) const
{
	ASSERT(!m_strIniPath.IsEmpty());
	ASSERT(!m_bReadOnly);

	CString strVersion;
	strVersion.Format(_T("%d"), nVersion);

	BOOL bResult = TRUE;
	bResult &= WritePrivateProfileString(_T("VersionInfo"), _T("Version"), strVersion, m_strIniPath);

	return bResult;
}

///<summary>
/// ini 파일로부터 버전 정보 읽기
///</summary>
///<returns>버전 값</returns>
INT CIniFile::_ReadVersion() const
{
	ASSERT(!m_strIniPath.IsEmpty());
	ASSERT(!m_bReadOnly);

	return GetPrivateProfileInt(_T("VersionInfo"), _T("Version"), 0, m_strIniPath);
}

/// <summary>
/// 문자열 값을 ini 파일에 기록
/// </summary>
/// <returns>성공 여부</returns>
BOOL CIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue) const
{
	ASSERT(!m_strIniPath.IsEmpty());
	ASSERT(!m_bReadOnly);

	return WritePrivateProfileString(lpszSection, lpszKey, lpszValue, m_strIniPath);
}

/// <summary>
///  정수 값을 ini 파일에 기록
/// </summary>
/// <returns>성공 여부</returns>
BOOL CIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, DWORD dwValue) const
{
	ASSERT(!m_strIniPath.IsEmpty());
	ASSERT(!m_bReadOnly);

	CString strValue;
	strValue.Format(_T("%ul"), dwValue);
	return Write(lpszSection, lpszKey, strValue);
}

/// <summary>
///  정수 값을 ini 파일에 기록
/// </summary>
/// <returns>성공 여부</returns>
BOOL CIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, INT nValue) const
{
	ASSERT(!m_strIniPath.IsEmpty());
	ASSERT(!m_bReadOnly);

	CString strValue;
	strValue.Format(_T("%d"), nValue);
	return Write(lpszSection, lpszKey, strValue);
}

/// <summary>
///  ini 파일에서 문자열 값을 읽음 (버퍼 크기 수동)
/// </summary>
void CIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue, OUT LPTSTR lpszValue, DWORD dwSize) const
{
	GetPrivateProfileString(lpszSection, lpszKey, lpszDefaultValue, lpszValue, dwSize, m_strIniPath);
}

/// <summary>
///  ini 파일에서 문자열 값을 읽음 (버퍼 크기 수동)
/// </summary>
void CIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue, OUT CString & strValue, DWORD dwSize) const
{
	GetPrivateProfileString(lpszSection, lpszKey, lpszDefaultValue, strValue.GetBuffer(dwSize), dwSize, m_strIniPath);
	strValue.ReleaseBuffer();
}

/// <summary>
///  ini 파일에서 문자열 값을 읽음 (버프 크기 자동)
/// </summary>
void CIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue, OUT CString & strValue) const
{
	ASSERT(!m_strIniPath.IsEmpty());

	constexpr DWORD		MAX_BUFFER_SIZE = 8192;  // 최대 버퍼 크기 제한
	DWORD				bufferSize = 256;        // 초기 버퍼 크기

	while (bufferSize <= MAX_BUFFER_SIZE)
	{
		// CString의 버퍼를 직접 할당하여 사용
		LPTSTR buffer = strValue.GetBuffer(bufferSize);

		DWORD dwReturn = ::GetPrivateProfileString(
			lpszSection,
			lpszKey,
			lpszDefaultValue,
			buffer,
			bufferSize,
			m_strIniPath
		);

		// CString 버퍼를 해제하여 크기 설정
		strValue.ReleaseBuffer();

		// 읽은 길이가 버퍼를 초과하지 않으면 완료
		if (dwReturn < bufferSize - 1)
			break;

		// 버퍼 크기를 늘리고 재시도
		bufferSize *= 2;
	}
}

/// <summary>
///  ini 파일에서 정수 값을 읽음
/// </summary>
DWORD CIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, DWORD dwDefaultValue) const
{
	ASSERT(!m_strIniPath.IsEmpty());

	return GetPrivateProfileInt(lpszSection, lpszKey, (INT)dwDefaultValue, m_strIniPath);
}

INT CIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, INT nDefaultValue) const
{
	ASSERT(!m_strIniPath.IsEmpty());

	return (INT)GetPrivateProfileInt(lpszSection, lpszKey, nDefaultValue, m_strIniPath);
}

void CIniFile::DeleteKey(LPCTSTR lpszSection, LPCTSTR lpszKey) const
{
	WritePrivateProfileString(lpszSection, lpszKey, NULL, m_strIniPath);
}

void CIniFile::DeleteSection(LPCTSTR lpszSection) const
{
	WritePrivateProfileString(lpszSection, NULL, NULL, m_strIniPath);
}

/// <summary>
///  모든 ini 파일에 대해 마이그레이션을 수행
/// </summary>
/// <remarks>
///  솔루션에서 다루는 모든 ini 파일들을 포괄하는 가장 상위 그리고 최소의 폴더 선정 후 
///  그 안을 순회하며 Migrate 시행.
///  성공했다면, 다시 중복해서 Migrate 하지 않기 위해 레지스트리에 마킹.
/// </remarks>
void CIniFile::MigrateAll()
{
	// 이전 MigrateAll 성공적으로 시행 됐는지 레지스트리 확인.
	LPCTSTR lpszKey = _T("Software\\NetID\\Update");
	CRegKey reg;
	DWORD dwVersion = 0;
	LSTATUS ret = reg.Open(HKEY_CURRENT_USER, lpszKey, KEY_ALL_ACCESS);
	if (ret == ERROR_SUCCESS)
	{
		// 이미 Migrate 시행된 최신 버전이면 skip
		ret = reg.QueryDWORDValue(_T("dwIniVersion"), dwVersion);
		if (ret == ERROR_SUCCESS && dwVersion >= (DWORD)LATEST_VERSION)
		{
			TRACE( _T("CIniFile::MigrateAll - Version does not require migration, so skipping - Latest Version [%d], Current Version [%d]\r\n"), LATEST_VERSION, (int)dwVersion);
			return;
		}
	}

	// 순회 대상 Path 큐에 추가
	CStringList listPath;
	CString strAddPath;

	{
		TCHAR szPath[MAX_PATH] = { 0 };
		// 윈도우 Directory 기반 Path 확보 (C:\DiskLock)
		if (!GetWindowsDirectory(szPath, MAX_PATH))
		{
			TRACE( _T("CIniFile::MigrateAll - Failed to get windows directory - errno[%d]\r\n"), GetLastError());
			return;
		}

		strAddPath = szPath;
		strAddPath = strAddPath.Left(strAddPath.Find(_T(':')) + 1);
		strAddPath.Append(_T("\\DiskLock"));
		listPath.AddHead(strAddPath);
	}

	// local app data Path 확보 (C:\Users\<userid>\AppData\Local\NetID)
	if (!SHGetSpecialFolderPath(NULL, strAddPath.GetBuffer(MAX_PATH), CSIDL_LOCAL_APPDATA, FALSE))
	{
		strAddPath.ReleaseBuffer();
		TRACE( _T("CIniFile::MigrateAll - Failed to get localappdata - errno[%d]\r\n"), GetLastError());
		return;
	}

	strAddPath.ReleaseBuffer();
	strAddPath.Append(_T("\\NetID"));
	listPath.AddHead(strAddPath);

	BOOL bResult = TRUE;

	// 큐를 활용해 재귀적 Directory 순회
	while (!listPath.IsEmpty())
	{
		CString curPath = listPath.RemoveHead();

		CFileFind finder;
		CString strPattern;
		strPattern.Format(_T("%s\\*"), curPath);

		BOOL bFind = finder.FindFile(strPattern);
		while (bFind)
		{
			bFind = finder.FindNextFile();
			if (finder.IsDots())
				continue;

			if (finder.IsDirectory())
			{
				// Directory면 큐에 추가
				listPath.AddTail(finder.GetFilePath());
				continue;
			}

			// 파일이면 ini 확장자 여부 확인
			CString strFilePath = finder.GetFilePath();
			if (!IsIniFile(strFilePath))
				continue;

			// Migrate 시도
			CIniFile iniFile(strFilePath, FALSE);
			if (!iniFile._Migrate())
			{
				bResult = FALSE;
				TRACE( _T("CIniFile::MigrateAll - Failed to Migrate - path[%s], errno[%d]\r\n"), strFilePath, GetLastError());
			}
		}
	}

	if (!bResult)
		return;

	// 키가 없으면 생성
	if (ret != ERROR_SUCCESS)
	{
		ret = reg.Create(HKEY_CURRENT_USER, lpszKey);
		if (ret != ERROR_SUCCESS)
		{
			TRACE( _T("CIniFile::MigrateAll - Failed to Create Registry - key[%s], errno(reg)[%d]\r\n"), lpszKey, ret);
			return;
		}
	}

	// 전체 Migrate 성공 시 레지스트리에 버전과 타임스탬프 기록
	if (reg.SetDWORDValue(_T("dwIniVersion"), (DWORD)LATEST_VERSION) != ERROR_SUCCESS)
	{
		TRACE( _T("CIniFile::MigrateAll - Failed to write ini version reg - key[%s], errno(reg)[%d]\r\n"), lpszKey, ret);
		return;
	}

}

/// <summary>
///  ini 파일 여부 확인
/// </summary>
/// <param name="strFilePath">파일 경로</param>
/// <returns>확장자 .ini 일치 여부</returns>
BOOL CIniFile::IsIniFile(const CString& strFilePath)
{
	if (strFilePath.IsEmpty())
		return FALSE;

	return strFilePath.Mid(strFilePath.ReverseFind(_T('.'))) == _T(".ini");
}

/// <summary>
///  현재 ini 파일에 대해 마이그레이션 수행
/// </summary>
/// <returns>마이그레이션 성공 실패 여부</returns>
/// <remarks>버전 추가 시 LATEST_VERSION 값 갱신할 것.</remarks>
BOOL CIniFile::_Migrate() const
{
	ASSERT(!m_strIniPath.IsEmpty());
	ASSERT(!m_bReadOnly);

	if (_taccess(m_strIniPath, 0) == -1)
		return TRUE;

	int nCurVersion = _ReadVersion();
	if (LATEST_VERSION <= nCurVersion)
		return TRUE;

	for (int nVersion = nCurVersion + 1; nVersion <= LATEST_VERSION; nVersion++)
	{
		BOOL bSuccess = FALSE;
		switch (nVersion)
		{
		case 1:
		{
			// ini 파일 유니코드화
			bSuccess = npConvertFileToUnicode(m_strIniPath);
			break;
		}
		default:
			ASSERT(FALSE);
			break;
		}

		if (!bSuccess)
		{
			TRACE( _T("CIniFile::Migrate - Failed to migrate - path[%s] version[%d] -> [%d], LATEST_VERSION[%d] errno[%d]\r\n"), m_strIniPath, nCurVersion, nVersion, LATEST_VERSION, GetLastError());
			return FALSE;
		}

		if (!_WriteVersion(nVersion))
		{
			TRACE( _T("CIniFile::Migrate - Failed to Write Version - path[%s] version[%d] -> [%d], LATEST_VERSION[%d] errno[%d]\r\n"), m_strIniPath, nCurVersion, nVersion, LATEST_VERSION, GetLastError());
			return FALSE;
		}
	}

	return TRUE;
}

/// <summary>
///  ini 파일을 생성함
/// </summary>
BOOL CIniFile::_Create() const
{
	ASSERT(!m_strIniPath.IsEmpty());

	::SHCreateDirectory(NULL, m_strIniPath.Left(m_strIniPath.ReverseFind(_T('\\'))));

	CFile iniFile;
	if (!iniFile.Open(m_strIniPath, CFile::modeCreate | CFile::modeWrite))
	{
		TRACE( _T("CIniFile::_Create - Failed to Create Ini - path[%s], errno[%d]\r\n"), m_strIniPath, GetLastError());
		return FALSE;
	}

	WCHAR bom = 0xFEFF;
	iniFile.Write(&bom, sizeof(bom));
	iniFile.Close();

	if (!_WriteVersion(LATEST_VERSION))
	{
		TRACE( _T("CIniFile::_Create - Failed to WriteVersion Ini - path[%s], version[%d], errno[%d]\r\n"), m_strIniPath, LATEST_VERSION, GetLastError());
		return FALSE;
	}

	return TRUE;
}


#ifdef DEBUG

/// <summary>
///  (DEBUG ONLY) 주어진 파라미터의 유효성 검증 
/// </summary>
/// <param name="eIniFile">INI_TYPE</param>
/// <param name="lpszSID">SID</param>
/// <param name="lpszSiteID">Site ID</param>
/// <param name="lpszUserID">User ID</param>
/// <param name="lpszParam1">추가 파라미터 (e.g. 정책 ID, 경로 등)</param>
void CIniFile::_AssertParams(
	INI_TYPE eIniFile,
	IN LPCTSTR lpszSID,
	IN LPCTSTR lpszSiteID,
	IN LPCTSTR lpszUserID,
	IN LPCTSTR lpszParam1
) const
{
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

	case INI_TYPE_SECUREDISK_INFO:
		ASSERT(lpszSiteID != nullptr);
		ASSERT(lpszSiteID != nullptr);	// Policy ID 
		break;

	case INI_TYPE_ALARM_INFO:
	case INI_TYPE_IMPORTANT_ALARM_INFO:
	case INI_TYPE_BACKUP:
		ASSERT(lpszSiteID != nullptr);
		ASSERT(lpszUserID != nullptr);
		break;

	case INI_TYPE_SECUREDISK_BACKUP:
	case INI_TYPE_PC_BACKUP:
	case INI_TYPE_COLLECT_POLICY:
		ASSERT(lpszSID != nullptr);
		ASSERT(lpszSiteID != nullptr);
		ASSERT(lpszUserID != nullptr);
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
}

#endif // DEBUG


