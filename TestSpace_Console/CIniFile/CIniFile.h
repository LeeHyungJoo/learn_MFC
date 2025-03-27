
#pragma once

#define BUFFER_SIZE_SECTION		(1024 * 8)
#define BUFFER_SIZE_VALUE		(1024 * 4)

#define LATEST_VERSION	(1)

struct SITE_INFO
{
public:
	SITE_INFO() {};
	SITE_INFO(const SITE_INFO& src) { _Copy(src); };
	SITE_INFO& operator=(const SITE_INFO& src) { _Copy(src); return *this; };

	void _Copy(const SITE_INFO& src)
	{
		strSiteID = src.strSiteID;
		strDomainID = src.strDomainID;
	};

public:
	CString			strSiteID;
	CString			strDomainID;	// 사용하지 않는 값 - USERINFO의 strDomainID 사용
};

struct USER_INFO
{
public:
	USER_INFO() {};
	USER_INFO(const USER_INFO& src) { _Copy(src); };
	USER_INFO& operator=(const USER_INFO& src) { _Copy(src); return *this; };

	void _Copy(const USER_INFO& src)
	{
		strUserID = src.strUserID;
		strDomainID = src.strDomainID;
	};

public:
	CString			strUserID;
	CString			strDomainID;
};



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

enum INI_MODE
{
	INI_MODE_READ,
	INI_MODE_WRITE,
	INI_MODE_MIGRATION,
};

enum RESULT_MIGRATION
{
	RESULT_MIGRATION_SUCCESS,
	RESULT_MIGRATION_FAILED,
	RESULT_MIGRATION_NOT_FOUND,
	RESULT_MIGRATION_NOT_REQUIRED,
};

class CIniFile
{
public:
	CIniFile();
	CIniFile(
		INI_TYPE eIniFile,
		INI_MODE eMode,
		IN LPCTSTR lpszSID = nullptr,
		IN const SITE_INFO* pSiteInfo = nullptr,
		IN const USER_INFO* pUserInfo = nullptr,
		IN LPCTSTR lpszParam1 = nullptr
	);

	CIniFile(
		IN LPCTSTR lpszIniPath,
		INI_MODE eMode
	);

	virtual ~CIniFile();

public:
	void Open(
		INI_TYPE eIniFile,
		INI_MODE eMode,
		IN LPCTSTR lpszSID = nullptr,
		IN const SITE_INFO* pSiteInfo = nullptr,
		IN const USER_INFO* pUserInfo = nullptr,
		IN LPCTSTR lpszParam1 = nullptr
	);

	void Open(
		IN LPCTSTR lpszIniPath,
		INI_MODE eMode
	);

	void Close();

	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue) const;
	BOOL Write(LPCTSTR lpszSection, LPCTSTR lpszKey, INT nValue) const;

	void Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue, OUT CString& strValue) const;
	void Read(LPCTSTR lpszSection, LPCTSTR lpszKey, INT nDefaultValue, OUT INT& nValue) const;

public:
	RESULT_MIGRATION Migrate() const;

	static BOOL MigrateAll();
	static BOOL IsIniFile(const CString& strFileName);

private:
	void _CheckParams(
		INI_TYPE eIniFile,
		IN LPCTSTR lpszSID,
		IN const SITE_INFO* pSiteInfo,
		IN const USER_INFO* pUserInfo,
		IN LPCTSTR lpszParam1
	) const;

	void _SetIniPath(
		INI_TYPE eIniFile,
		IN LPCTSTR lpszSID,
		IN const SITE_INFO* pSiteInfo,
		IN const USER_INFO* pUserInfo,
		IN LPCTSTR lpszParam1
	);

	BOOL _IsOpen() const;

	BOOL _WriteVersion(INT nVersion) const;
	INT  _ReadVersion() const;

	BOOL _ConvertToUnicode() const;
	void _Create() const;

private:
	INI_MODE m_eMode;
	CString m_strIniPath;
};
