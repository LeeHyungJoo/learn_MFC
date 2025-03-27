#include "stdafx.h"
#include "CIniFile.h"


using namespace std;

#define SID_SAMPLE (_T("SAMPLE_SID"))

// 존재 하지 않는 파일 읽기 시도
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
	/* 결과
	파일이 생성되지 않음.
	ASSERT 걸리는 곳 없음.
	*/
}

// 존재하지 않은 파일 읽기 시도 + SID 없을 때. 
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
	/* 결과
	파일이 생성되지 않음.
	ASSERT 걸리는 곳 없음.
	*/
}

// 존재하지 않은 파일 읽기 시도 + USER_INFO or SITE_INFO 넣지 않을때.  
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
	/* 결과
	파일이 생성되지 않음.
	ASSERT 걸림 SITE_INFO, USER_INFO 는 CheckParam에 의해 nullptr이면 안됨. 
	*/
}

// ANSI 형식의 파일이 존재 파일 읽기 시도.
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
	/* 결과
	정상. 문제 없이 동작. 
	*/
}

// ANSI 형식의 파일이 존재 파일 Migration 시도  
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
	/* 결과
	정상, 제대로 UTF-16 LE 로 변환되고, Version 섹션이 등록됨. 
	*/
}

// 존재하지 않은 파일 쓰기 시도
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
	/* 결과
	정상, UTF-16 LE 형식으로 파일 생겼고, Version 섹션이 등록됨. 
	*/
}


// Migration 전체 
void TESTCASE_7()
{
	CIniFile::MigrateAll();
}

void main()
{
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), SW_SHOW))
	{
		// 초기화 실패 시 처리
		fprintf(stderr, "MFC 초기화 실패\n");
	}


	TESTCASE_7();
    return;
}

