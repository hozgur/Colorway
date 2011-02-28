// MiracleLibDLL.h : main header file for the MiracleLib DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMiracleLibApp
// See MiracleLib.cpp for the implementation of this class
//

class CMiracleLibApp : public CWinApp
{
public:
	CMiracleLibApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
