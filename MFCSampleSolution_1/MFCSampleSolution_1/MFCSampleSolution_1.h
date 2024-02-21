
// MFCSampleSolution_1.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCSampleSolution1App:
// See MFCSampleSolution_1.cpp for the implementation of this class
//

class CMFCSampleSolution1App : public CWinApp
{
public:
	CMFCSampleSolution1App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCSampleSolution1App theApp;
