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
	/* ���
		������ �������� ����. 
		ASSERT �ɸ��� �� ����. 
	*/

	ini.Open(INI_TYPE_PC_BACKUP, INI_MODE_READ, nullptr, &siteInfo, &userInfo);
	/* ���
	������ �������� ����.
	ASSERT �ɸ��� �� ����.
	*/


	/* ���
	������ �������� ����.
	ASSERT �ɸ��� �� ����.
	*/


	//ini.Open(INI_TYPE_PC_BACKUP, INI_MODE_READ, _T("SAMPLE_SID"), &siteInfo, &userInfo);
	ini.Close();


    return 0;
}

