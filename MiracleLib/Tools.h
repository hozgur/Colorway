// Tools.h: interface for the CTools class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLS_H__7C2A55C0_EA66_11D4_A752_0000214446E3__INCLUDED_)
#define AFX_TOOLS_H__7C2A55C0_EA66_11D4_A752_0000214446E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MFile.h"



class CTools : public CMiracleObj  
{
public:
	CTools();
	virtual ~CTools();
// Registry Functions
	BOOL SaveInt(char *appname,char *key,int value);
	int LoadInt(char *appname,char *key,int defvalue);
	BOOL SaveStr(char *appname,char *key, char *param);
	void LoadStr(char *appname,char *key,char *defvalue,char *returnvalue,int maxlenght);
	const char *GetAppName();
};

extern CTools Tools;

#endif // !defined(AFX_TOOLS_H__7C2A55C0_EA66_11D4_A752_0000214446E3__INCLUDED_)
