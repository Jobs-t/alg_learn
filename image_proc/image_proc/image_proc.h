
// image_proc.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cimage_procApp:
// See image_proc.cpp for the implementation of this class
//

class Cimage_procApp : public CWinApp
{
public:
	Cimage_procApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cimage_procApp theApp;