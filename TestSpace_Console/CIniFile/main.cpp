#include "stdafx.h"
#include "CIniFile.h"


using namespace std;

#define SID_SAMPLE (_T("SAMPLE_SID"))

// ���� ���� �ʴ� ���� �б� �õ�
void TESTCASE_1()
{
	SITE_INFO siteInfo;
	siteInfo.strSiteID = _T("HyungJooSite");
	siteInfo.strDomainID = _T("HyungJooDomain in SITE_INFO");	//deprecated

	USER_INFO userInfo;
	userInfo.strDomainID = _T("HyungJooDomain In USER_INFO");
	userInfo.strUserID = _T("HYUNGJOO");

	CIniFile ini;

	ini.Open(INI_TYPE_PC_BACKUP, INI_MODE_READ, SID_SAMPLE, &siteInfo, &userInfo);
	/* ���
	������ �������� ����.
	ASSERT �ɸ��� �� ����.
	*/
}

// �������� ���� ���� �б� �õ� + SID ���� ��. 
void TESTCASE_2()
{
	SITE_INFO siteInfo;
	siteInfo.strSiteID = _T("HyungJooSite");
	siteInfo.strDomainID = _T("HyungJooDomain in SITE_INFO");	//deprecated

	USER_INFO userInfo;
	userInfo.strDomainID = _T("HyungJooDomain In USER_INFO");
	userInfo.strUserID = _T("HYUNGJOO");

	CIniFile ini;

	ini.Open(INI_TYPE_PC_BACKUP, INI_MODE_READ, nullptr, &siteInfo, &userInfo);
	/* ���
	������ �������� ����.
	ASSERT �ɸ��� �� ����.
	*/
}

// �������� ���� ���� �б� �õ� + USER_INFO or SITE_INFO ���� ������.  
void TESTCASE_3()
{
	SITE_INFO siteInfo;
	siteInfo.strSiteID = _T("HyungJooSite");
	siteInfo.strDomainID = _T("HyungJooDomain in SITE_INFO");	//deprecated

	USER_INFO userInfo;
	userInfo.strDomainID = _T("HyungJooDomain In USER_INFO");
	userInfo.strUserID = _T("HYUNGJOO");

	CIniFile ini;

	ini.Open(INI_TYPE_PC_BACKUP, INI_MODE_READ, SID_SAMPLE, &siteInfo, nullptr);
	/* ���
	������ �������� ����.
	ASSERT �ɸ� SITE_INFO, USER_INFO �� CheckParam�� ���� nullptr�̸� �ȵ�. 
	*/
}

// ANSI ������ ������ ���� ���� �б� �õ�.
void TESTCASE_4()
{
	SITE_INFO siteInfo;
	siteInfo.strSiteID = _T("Manual_HyungJooSite");
	siteInfo.strDomainID = _T("HyungJooDomain in SITE_INFO");	//deprecated

	USER_INFO userInfo;
	userInfo.strDomainID = _T("HyungJooDomain In USER_INFO");
	userInfo.strUserID = _T("Manual_HYUNGJOO");

	CIniFile ini;
	ini.Open(INI_TYPE_PC_BACKUP, INI_MODE_READ, SID_SAMPLE, &siteInfo, &userInfo);
	//ini.TEST_CREATE();
	/* ���
	����. ���� ���� ����. 
	*/
}

// ANSI ������ ������ ���� ���� Migration �õ�  
void TESTCASE_5()
{
	SITE_INFO siteInfo;
	siteInfo.strSiteID = _T("Manual_HyungJooSite");
	siteInfo.strDomainID = _T("HyungJooDomain in SITE_INFO");	//deprecated

	USER_INFO userInfo;
	userInfo.strDomainID = _T("HyungJooDomain In USER_INFO");
	userInfo.strUserID = _T("Manual_HYUNGJOO");

	CIniFile ini;

	ini.Open(INI_TYPE_PC_BACKUP, INI_MODE_MIGRATION, SID_SAMPLE, &siteInfo, &userInfo);
	/* ���
	����, ����� UTF-16 LE �� ��ȯ�ǰ�, Version ������ ��ϵ�. 
	*/
}

// �������� ���� ���� ���� �õ�
void TESTCASE_6()
{
	SITE_INFO siteInfo;
	siteInfo.strSiteID = _T("HyungJooSite");
	siteInfo.strDomainID = _T("HyungJooDomain in SITE_INFO");	//deprecated

	USER_INFO userInfo;
	userInfo.strDomainID = _T("HyungJooDomain In USER_INFO");
	userInfo.strUserID = _T("HYUNGJOO");

	CIniFile ini;

	ini.Open(INI_TYPE_PC_BACKUP, INI_MODE_WRITE, SID_SAMPLE, &siteInfo, &userInfo);
	/* ���
	����, UTF-16 LE �������� ���� �����, Version ������ ��ϵ�. 
	*/
}


// Migration ��ü 
void TESTCASE_7()
{
	CIniFile::MigrateAll();
}

void main()
{
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), SW_SHOW))
	{
		// �ʱ�ȭ ���� �� ó��
		fprintf(stderr, "MFC �ʱ�ȭ ����\n");
	}


	TESTCASE_7();
    return;
}

