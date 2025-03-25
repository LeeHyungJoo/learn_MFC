
#define BUFFER_SIZE_SECTION		(1024 * 8)
#define BUFFER_SIZE_VALUE		(1024 * 4)

#define LATEST_VERSION	(1)

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
	INI_MODE_WRITE,		// 읽기 쓰기 생성 마이그레이션
	INI_MODE_MIGRATE,	// 마이그레이션 전용
	INI_MODE_READ,		// 읽기 전용
};

class CIniFile
{
public:
	CIniFile();
	CIniFile(INI_TYPE eIniFile, INI_MODE eMode, IN CStringArray* pArrParams = nullptr);
	virtual ~CIniFile();

private:
	void _CheckParams(INI_TYPE eIniFile, CStringArray* pArrParams) const;
	void _SetIniPath(INI_TYPE eIniFile, CStringArray* pArrParams);
	BOOL _IsBind() const;

public:
	void Bind(INI_TYPE eIniFile, INI_MODE eMode, IN CStringArray* pArrParams = nullptr);
	void Unbind();

	void Write(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue) const;
	void Write(LPCTSTR lpszSection, LPCTSTR lpszKey, INT nValue) const;

	void Read(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefaultValue, OUT CString& strValue) const;
	void Read(LPCTSTR lpszSection, LPCTSTR lpszKey, INT nDefaultValue, OUT INT& nValue) const;

private:
	void _Migrate() const;

	void _WriteVersion(INT nVersion) const;
	INT _ReadVersion() const;

	BOOL _ConvertToUnicode() const;
	void _Create() const;

private:
	INI_MODE m_eMode;
	CString m_strIniPath;
};
