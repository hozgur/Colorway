// Tools.cpp: implementation of the CTools class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tools.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTools::CTools()
{
	
}

CTools::~CTools()
{

}

void CTools::LoadStr(char *appname,char *key, char *defvalue,char *returnvalue,int maxlenght)
{	
	GetProfileString(appname,key,defvalue,returnvalue,maxlenght);	
}

BOOL CTools::SaveStr(char *appname,char *key, char *param)
{
	return WriteProfileString(appname,key,param);
}

int CTools::LoadInt(char *appname,char *key, int defvalue)
{
	return GetProfileInt(appname,key,defvalue);
}

BOOL CTools::SaveInt(char *appname,char *key, int value)
{
	char buffer[20];
	return WriteProfileString(appname,key,itoa(value,buffer,10));
}

const char *CTools::GetAppName()
{
	return AfxGetAppName();
}