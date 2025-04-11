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
	// ���� ���ۿ� BOM �� ������ �̹� �����ڵ� ���Ϸ� �Ǵ��ϰ� ��ȯ ���� -> ���� ó��
	if (fileSrc.Read(&wch, sizeof(WCHAR)) && wch == 0xFEFF)
		return TRUE;

	fileSrc.SeekToBegin();

	// �ӽ� ���� ��� ���� ex) disklock.ini �� disklock.ini.unicode
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
		// �����ڵ� BOM ����.
		WCHAR bom = 0xFEFF;
	fileDst.Write(&bom, sizeof(bom));

	// ������ �� ����(���� ����)�� ������ ������ ��ȯ ����
	while (TRUE)
	{
		CByteArray arrByte;
		BYTE byte;

		// �� �� �б� 0x0A(\n)�� ���� ������ 1����Ʈ�� ����
		while (fileSrc.Read(&byte, 1))
		{
			arrByte.Add(byte);
			if (byte == 0x0A) break;
		}

		// ���� �����Ͱ� ���ٸ� ����
		if (arrByte.IsEmpty())
			break;

		// �����ڵ�� ��ȯ�� ���� ũ�� ���
		int	nLen = ::MultiByteToWideChar(nCodePage, 0, (LPCSTR)arrByte.GetData(), (int)arrByte.GetSize(), NULL, 0);
		if (nLen == 0)
		{
			// ��ȯ�� �����ʹ� �ִµ� ���� ũ�⸦ ������ �� ���� ��Ȳ.
			TRACE( _T("npConvertFileToUnicode - Failed to get buffer length (zero) - path[%s], byte array size[%d], errno[%d]\r\n"), strDstPath, (int)arrByte.GetSize(), GetLastError());
			continue;
		}

		// ��ȯ ����
		CArray<WCHAR, WCHAR> arrWch;
		arrWch.SetSize(nLen);
		::MultiByteToWideChar(nCodePage, 0, (LPCSTR)arrByte.GetData(), (int)arrByte.GetSize(), arrWch.GetData(), nLen);

		// ��ȯ�� �����ڵ� ���ڿ��� ��� ���Ͽ� ����
		fileDst.Write(arrWch.GetData(), nLen * sizeof(WCHAR));
	}

	// ���� �� �̸� ���� ���� ���� �ݱ�
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

		// ���� ���� ����
		if (!DeleteFile(lpszSrcPath))
		{
			TRACE( _T("npConvertFileToUnicode - Failed to delete dst - path[%s], errno[%d]\r\n"), strDstPath, GetLastError());
			return FALSE;
		}

	// �����ڵ�� ��ȯ�� ������ ���� ���Ϸ� ��ü (�̸� ����)
	if (!MoveFile(strDstPath, lpszSrcPath))
	{
		TRACE( _T("npConvertFileToUnicode - Failed to rename - dst path[%s], src path[%s], errno[%d]\r\n"), strDstPath, lpszSrcPath, GetLastError());
		return FALSE;
	}

	//����
	return TRUE;
}

/// <summary>
///  �⺻ ������
/// </summary>
/// <remarks> ini ��δ� ����ΰ� �б� ��� ����</remarks>
CIniFile::CIniFile()
	: m_bReadOnly(TRUE)
{

}

/// <summary>
///  ������ - ���� ���ڿ� �Ű� ���� ������� �ʱ�ȭ �� Open
/// </summary>
/// <param name="eIniFile">INI_TYPE</param>
/// <param name="bReadOnly">T : �б� ���� / F : �б�/����</param>
/// <param name="lpszSID">SID</param>
/// <param name="lpszSiteID">Site ID</param>
/// <param name="lpszUserID">User ID</param>
/// <param name="lpszParam1">�߰� �Ķ���� (e.g. ��å ID, ��� ��)</param>
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
///  ������ - ���� ��θ� ���� �ʱ�ȭ �� Open
/// </summary>
/// <param name="lpszIniPath">ini ���� ���</param>
/// <param name="bReadOnly">T : �б� ���� / F : �б�/����</param>
CIniFile::CIniFile(
	IN LPCTSTR lpszIniPath,
	BOOL bReadOnly
)
{
	Open(lpszIniPath, bReadOnly);
}

/// <summary>
/// �Ҹ���
/// </summary>
CIniFile::~CIniFile()
{
	Close();
}

/// <summary>
///  ���� ���� ������� ini ���� ����
/// </summary>
/// <param name="eIniFile">INI_TYPE</param>
/// <param name="bReadOnly">T : �б� ���� / F : �б�/����</param>
/// <param name="lpszSID">SID</param>
/// <param name="lpszSiteID">Site ID</param>
/// <param name="lpszUserID">User ID</param>
/// <param name="lpszParam1">�߰� �Ķ���� (e.g. ��å ID, ��� ��)</param>
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
///  ���� ��η� ini ���� ����
/// </summary>
/// <param name="lpszIniPath">ini ���� ���</param>
/// <param name="bReadOnly">T : �б� ���� / F : �б�/����</param>
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
/// CIniFile Ŭ���� �ʱ�ȭ (��� ����, �б� ����)
/// </summary>
void CIniFile::Close()
{
	m_strIniPath.Empty();
	m_bReadOnly = TRUE;
}

/// <summary>
///  �־��� �Ű� �������� Ȱ����, ini ���� ��θ� ������
/// </summary>
/// <param name="eIniFile">INI_TYPE</param>
/// <param name="lpszSID">SID</param>
/// <param name="lpszSiteID">Site ID</param>
/// <param name="lpszUserID">User ID</param>
/// <param name="lpszParam1">�߰� �Ķ���� (e.g. ��å ID, ��� ��)</param>
/// <param name="strIniPath">��ȯ�� ini ���� ���</param>
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
///  ini ���Ͽ� ���� ���� ����
/// </summary>
/// <param name="nVersion">���� ��ȣ</param>
/// <returns>���� ����</returns>
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
/// ini ���Ϸκ��� ���� ���� �б�
///</summary>
///<returns>���� ��</returns>
INT CIniFile::_ReadVersion() const
{
	ASSERT(!m_strIniPath.IsEmpty());
	ASSERT(!m_bReadOnly);

	return GetPrivateProfileInt(_T("VersionInfo"), _T("Version"), 0, m_strIniPath);
}

/// <summary>
/// ���ڿ� ���� ini ���Ͽ� ���
/// </summary>
/// <returns>���� ����</returns>
BOOL CIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue) const
{
	ASSERT(!m_strIniPath.IsEmpty());
	ASSERT(!m_bReadOnly);

	return WritePrivateProfileString(lpszSection, lpszKey, lpszValue, m_strIniPath);
}

/// <summary>
///  ���� ���� ini ���Ͽ� ���
/// </summary>
/// <returns>���� ����</returns>
BOOL CIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, DWORD dwValue) const
{
	ASSERT(!m_strIniPath.IsEmpty());
	ASSERT(!m_bReadOnly);

	CString strValue;
	strValue.Format(_T("%ul"), dwValue);
	return Write(lpszSection, lpszKey, strValue);
}

/// <summary>
///  ���� ���� ini ���Ͽ� ���
/// </summary>
/// <returns>���� ����</returns>
BOOL CIniFile::Write(LPCTSTR lpszSection, LPCTSTR lpszKey, INT nValue) const
{
	ASSERT(!m_strIniPath.IsEmpty());
	ASSERT(!m_bReadOnly);

	CString strValue;
	strValue.Format(_T("%d"), nValue);
	return Write(lpszSection, lpszKey, strValue);
}

/// <summary>
///  ini ���Ͽ��� ���ڿ� ���� ���� (���� ũ�� ����)
/// </summary>
void CIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue, OUT LPTSTR lpszValue, DWORD dwSize) const
{
	GetPrivateProfileString(lpszSection, lpszKey, lpszDefaultValue, lpszValue, dwSize, m_strIniPath);
}

/// <summary>
///  ini ���Ͽ��� ���ڿ� ���� ���� (���� ũ�� ����)
/// </summary>
void CIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue, OUT CString & strValue, DWORD dwSize) const
{
	GetPrivateProfileString(lpszSection, lpszKey, lpszDefaultValue, strValue.GetBuffer(dwSize), dwSize, m_strIniPath);
	strValue.ReleaseBuffer();
}

/// <summary>
///  ini ���Ͽ��� ���ڿ� ���� ���� (���� ũ�� �ڵ�)
/// </summary>
void CIniFile::Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue, OUT CString & strValue) const
{
	ASSERT(!m_strIniPath.IsEmpty());

	constexpr DWORD		MAX_BUFFER_SIZE = 8192;  // �ִ� ���� ũ�� ����
	DWORD				bufferSize = 256;        // �ʱ� ���� ũ��

	while (bufferSize <= MAX_BUFFER_SIZE)
	{
		// CString�� ���۸� ���� �Ҵ��Ͽ� ���
		LPTSTR buffer = strValue.GetBuffer(bufferSize);

		DWORD dwReturn = ::GetPrivateProfileString(
			lpszSection,
			lpszKey,
			lpszDefaultValue,
			buffer,
			bufferSize,
			m_strIniPath
		);

		// CString ���۸� �����Ͽ� ũ�� ����
		strValue.ReleaseBuffer();

		// ���� ���̰� ���۸� �ʰ����� ������ �Ϸ�
		if (dwReturn < bufferSize - 1)
			break;

		// ���� ũ�⸦ �ø��� ��õ�
		bufferSize *= 2;
	}
}

/// <summary>
///  ini ���Ͽ��� ���� ���� ����
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
///  ��� ini ���Ͽ� ���� ���̱׷��̼��� ����
/// </summary>
/// <remarks>
///  �ַ�ǿ��� �ٷ�� ��� ini ���ϵ��� �����ϴ� ���� ���� �׸��� �ּ��� ���� ���� �� 
///  �� ���� ��ȸ�ϸ� Migrate ����.
///  �����ߴٸ�, �ٽ� �ߺ��ؼ� Migrate ���� �ʱ� ���� ������Ʈ���� ��ŷ.
/// </remarks>
void CIniFile::MigrateAll()
{
	// ���� MigrateAll ���������� ���� �ƴ��� ������Ʈ�� Ȯ��.
	LPCTSTR lpszKey = _T("Software\\NetID\\Update");
	CRegKey reg;
	DWORD dwVersion = 0;
	LSTATUS ret = reg.Open(HKEY_CURRENT_USER, lpszKey, KEY_ALL_ACCESS);
	if (ret == ERROR_SUCCESS)
	{
		// �̹� Migrate ����� �ֽ� �����̸� skip
		ret = reg.QueryDWORDValue(_T("dwIniVersion"), dwVersion);
		if (ret == ERROR_SUCCESS && dwVersion >= (DWORD)LATEST_VERSION)
		{
			TRACE( _T("CIniFile::MigrateAll - Version does not require migration, so skipping - Latest Version [%d], Current Version [%d]\r\n"), LATEST_VERSION, (int)dwVersion);
			return;
		}
	}

	// ��ȸ ��� Path ť�� �߰�
	CStringList listPath;
	CString strAddPath;

	{
		TCHAR szPath[MAX_PATH] = { 0 };
		// ������ Directory ��� Path Ȯ�� (C:\DiskLock)
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

	// local app data Path Ȯ�� (C:\Users\<userid>\AppData\Local\NetID)
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

	// ť�� Ȱ���� ����� Directory ��ȸ
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
				// Directory�� ť�� �߰�
				listPath.AddTail(finder.GetFilePath());
				continue;
			}

			// �����̸� ini Ȯ���� ���� Ȯ��
			CString strFilePath = finder.GetFilePath();
			if (!IsIniFile(strFilePath))
				continue;

			// Migrate �õ�
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

	// Ű�� ������ ����
	if (ret != ERROR_SUCCESS)
	{
		ret = reg.Create(HKEY_CURRENT_USER, lpszKey);
		if (ret != ERROR_SUCCESS)
		{
			TRACE( _T("CIniFile::MigrateAll - Failed to Create Registry - key[%s], errno(reg)[%d]\r\n"), lpszKey, ret);
			return;
		}
	}

	// ��ü Migrate ���� �� ������Ʈ���� ������ Ÿ�ӽ����� ���
	if (reg.SetDWORDValue(_T("dwIniVersion"), (DWORD)LATEST_VERSION) != ERROR_SUCCESS)
	{
		TRACE( _T("CIniFile::MigrateAll - Failed to write ini version reg - key[%s], errno(reg)[%d]\r\n"), lpszKey, ret);
		return;
	}

}

/// <summary>
///  ini ���� ���� Ȯ��
/// </summary>
/// <param name="strFilePath">���� ���</param>
/// <returns>Ȯ���� .ini ��ġ ����</returns>
BOOL CIniFile::IsIniFile(const CString& strFilePath)
{
	if (strFilePath.IsEmpty())
		return FALSE;

	return strFilePath.Mid(strFilePath.ReverseFind(_T('.'))) == _T(".ini");
}

/// <summary>
///  ���� ini ���Ͽ� ���� ���̱׷��̼� ����
/// </summary>
/// <returns>���̱׷��̼� ���� ���� ����</returns>
/// <remarks>���� �߰� �� LATEST_VERSION �� ������ ��.</remarks>
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
			// ini ���� �����ڵ�ȭ
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
///  ini ������ ������
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
///  (DEBUG ONLY) �־��� �Ķ������ ��ȿ�� ���� 
/// </summary>
/// <param name="eIniFile">INI_TYPE</param>
/// <param name="lpszSID">SID</param>
/// <param name="lpszSiteID">Site ID</param>
/// <param name="lpszUserID">User ID</param>
/// <param name="lpszParam1">�߰� �Ķ���� (e.g. ��å ID, ��� ��)</param>
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


