#include "stdafx.h"

#include "CIniFile.h"

using namespace std;

int main()
{

	SITE_INFO siteInfo;
	siteInfo.strSiteID = _T("HyungJooSite");
	siteInfo.strDomainID = _T("HyungJooDomain in SITE_INFO");	//deprecated

	USER_INFO userInfo;
	userInfo.strDomainID = _T("HyungJooDomain In USER_INFO");
	userInfo.strUserID = _T("HYUNGJOO");

	CIniFile ini;

	//ini.Open(INI_TYPE_PC_BACKUP, INI_MODE_READ,_T("SAMPLE_SID"), &siteInfo, &userInfo);
	/* 결과
		파일이 생성되지 않음. 
		ASSERT 걸리는 곳 없음. 
	*/

	ini.Open(INI_TYPE_PC_BACKUP, INI_MODE_READ, nullptr, &siteInfo, &userInfo);
	/* 결과
	파일이 생성되지 않음.
	ASSERT 걸리는 곳 없음.
	*/


	/* 결과
	파일이 생성되지 않음.
	ASSERT 걸리는 곳 없음.
	*/


	//ini.Open(INI_TYPE_PC_BACKUP, INI_MODE_READ, _T("SAMPLE_SID"), &siteInfo, &userInfo);
	ini.Close();


    return 0;
}

