
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LATEST_VERSION	(1)
#define GetSID 


enum INI_TYPE
{
	INI_TYPE_POLICY_INFO,
	INI_TYPE_TOTAL_BASE_POLICY_INFO,
	INI_TYPE_TOTAL_ID_POLICY_INFO,
	INI_TYPE_COMMON,
	INI_TYPE_DISKLOCK,
	INI_TYPE_POLICY,
	INI_TYPE_SHARED_WORKDRIVE_INFO,
	INI_TYPE_ALARM_INFO,
	INI_TYPE_IMPORTANT_ALARM_INFO,
	INI_TYPE_SECUREDISK_BACKUP,
	INI_TYPE_PC_BACKUP,
	INI_TYPE_COLLECT_POLICY,
	INI_TYPE_BACKUP,
	INI_TYPE_EXPORT_INFO,
	INI_TYPE_RECYCLE_BIN,
	INI_TYPE_LOCAL_APP_CATEGORY,
	//INI_TYPE_LANG
};

class AFX_EXT_CLASS CIniFile
{
public:
	CIniFile();
	CIniFile(
		INI_TYPE eIniFile,
		BOOL bReadOnly = TRUE,
		IN LPCTSTR lpszSID = nullptr,
		IN LPCTSTR lpszSiteID = nullptr,
		IN LPCTSTR lpszUserID = nullptr,
		IN LPCTSTR lpszParam1 = nullptr
	);

	CIniFile(
		IN LPCTSTR lpszIniPath,
		BOOL bReadOnly = TRUE
	);

	virtual ~CIniFile();

public:
	BOOL Open(
		INI_TYPE eIniFile,
		BOOL bReadOnly = TRUE,
		IN LPCTSTR lpszSID = nullptr,
		IN LPCTSTR lpszSiteID = nullptr,
		IN LPCTSTR lpszUserID = nullptr,
		IN LPCTSTR lpszParam1 = nullptr
	);

	BOOL Open(
		IN LPCTSTR lpszIniPath,
		BOOL bReadOnly = TRUE
	);

	void Close();

	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue) const;
	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, DWORD dwValue) const;
	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, INT nValue) const;

	//void Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue, DWORD dwSize, OUT CString&  strValue) const;
	void Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue, OUT CString& strValue) const;
	void Read(LPCTSTR lpszSection, LPCTSTR lpszKey, DWORD dwDefaultValue, OUT DWORD& dwValue) const;
	void Read(LPCTSTR lpszSection, LPCTSTR lpszKey, INT nDefaultValue, OUT INT& nValue) const;

public:
	static void MigrateAll();
	static BOOL IsIniFile(const CString& strFilePath);

private:
	BOOL _Migrate() const;
	void _MakeIniPath(
		INI_TYPE eIniFile,
		IN	LPCTSTR lpszSID,
		IN	LPCTSTR lpszSiteID,
		IN	LPCTSTR lpszUserID,
		IN	LPCTSTR lpszParam1,
		OUT CString& strIniPath
	) const;

	BOOL _WriteVersion(INT nVersion) const;
	INT  _ReadVersion() const;

	BOOL _Create() const;
	BOOL _ConvertToUnicode() const;

private:
	BOOL m_bReadOnly;
	CString m_strIniPath;

#ifdef DEBUG
private:
	void _AssertParams(
		INI_TYPE eIniFile,
		IN LPCTSTR lpszSID,
		IN LPCTSTR lpszSiteID,
		IN LPCTSTR lpszUserID,
		IN LPCTSTR lpszParam1
	) const;
#endif // DEBUG
};
