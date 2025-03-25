#include "stdafx.h"

#include "CIniFile.h"

using namespace std;

int main()
{
	CStringArray arrParams;
	arrParams.Add(_T("TEST-SID"));
	CIniFile ini;

	ini.Bind(INI_TYPE_DISKLOCK, INI_MODE_WRITE, &arrParams);
	//ini.Bind(INI_TYPE_DISKLOCK, INI_MODE_MIGRATE, &arrParams);

    return 0;
}

