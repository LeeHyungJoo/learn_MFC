
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#undef	AFX_EXT_CLASS
#ifdef	_NPCTL32_IMPL
#define	AFX_EXT_CLASS	AFX_CLASS_EXPORT
#else
#define	AFX_EXT_CLASS	AFX_CLASS_IMPORT
#endif

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
	INI_TYPE_LANG
};

enum INI_MODE
{
	INI_MODE_READ,		// 읽기 전용
	INI_MODE_WRITE,		// 읽기 쓰기 생성 마이그레이션
	INI_MODE_MIGRATE,	// 마이그레이션 전용
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
	void Close();

	void Write(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue) const;
	void Write(LPCTSTR lpszSection, LPCTSTR lpszKey, INT nValue) const;

	void Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue, OUT CString& strValue) const;
	void Read(LPCTSTR lpszSection, LPCTSTR lpszKey, INT nDefaultValue, OUT INT& nValue) const;

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
	void _Migrate() const;

	void _WriteVersion(INT nVersion) const;
	INT _ReadVersion() const;

	BOOL _ConvertToUnicode() const;
	void _Create() const;

private:
	INI_MODE m_eMode;
	CString m_strIniPath;
};

#undef	AFX_EXT_CLASS
#define	AFX_EXT_CLASS
