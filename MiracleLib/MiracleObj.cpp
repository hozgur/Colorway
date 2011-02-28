// MiracleObj.cpp: implementation of the CMiracleObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MiracleObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_INTENT 40
#define DEBUGNAME  "c:\\debug.txt"
#define ERRORNAME  "c:\\error.txt"
short Intent = 0;
CString Head = ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
long _CallCount = 0;
CString CallCountStr;
CString TotalErrorMessage;
CString DebugFileName = DEBUGNAME;
CString ErrorFileName = ERRORNAME;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMiracleObj::CMiracleObj(LPCTSTR name)
{
	ObjName = name;
	CString temp;
	temp.Format("%s created.",ObjName);
	logDebug(temp);
}

CMiracleObj::~CMiracleObj()
{
	CString temp;
	temp.Format("%s destroyed.",ObjName);
	logDebug(temp);
}


//BOOL CMiracleObj::MyNameIs(LPCTSTR name)
//{
//	if (strlen(name) == 0) return false;
//	ObjName = name;
//	return true;
//}

//void CMiracleObj::Init(LPCTSTR name)
//{
//	DebugFileName = "";
//	DebugFileName = DebugFileName +"c:\\"+name+"debug.txt";
//	ErrorFileName = "";
//	ErrorFileName = ErrorFileName +"c:\\"+name+"error.txt";
//	Init();
//}
//void CMiracleObj::Init()
//{
//	DeleteFile(DebugFileName);
//	DeleteFile(ErrorFileName);
//	CTime Time = CTime::GetCurrentTime();
//	
//	CString Str = Time.Format("%d/%m/%Y-%H:%M:");
//	Str = Str +"Debug Messaging System Written by Hakan ÖZGÜR";
//	DebugMessage(Str);
//}

//void CMiracleObj::ErrorMessage(LPCTSTR lpszFormat,...)
//{
//	ASSERT(AfxIsValidString(lpszFormat));
//
//	va_list argList;
//	va_start(argList, lpszFormat);
//	ErrorStr.FormatV(lpszFormat, argList);
//	va_end(argList);
//	TotalErrorMessage = TotalErrorMessage + ErrorStr;
//	ErrorStr = Head.Left(Intent) + ErrorStr;
//	FILE *f = fopen(ErrorFileName,"a+t");
//	if (f!=NULL)
//	{
//		CTime Time = CTime::GetCurrentTime();
//
//		CString TimeStr = Time.Format("%d/%m/%Y-%H:%M:%S:");
//		fprintf(f,"%s%s %s\n",TimeStr,ObjName,ErrorStr);
//		fclose(f);
//	}	
//	DebugMessage(ErrorStr);	
//}
//
//void CMiracleObj::DebugMessage(LPCTSTR lpszFormat,...)
//{
//	ASSERT(AfxIsValidString(lpszFormat));
//	va_list argList;
//	va_start(argList, lpszFormat);
//	ErrorStr.FormatV(lpszFormat, argList);
//	va_end(argList);
//	ErrorStr = Head.Left(Intent) + ErrorStr;
//	if (_CallCount !=0)
//	{
//		CString str;
//		str.Format(": Count = %ld\n",_CallCount);
//		str = Head.Left(Intent) + str;
//		ErrorStr = CallCountStr + str + ErrorStr;
//		_CallCount = 0;
//	}
//	FILE *f = fopen(DebugFileName,"a+t");
//	if (f!=NULL)
//	{
//		CTime Time = CTime::GetCurrentTime();
//
//		CString TimeStr = Time.Format("%d/%m/%Y-%H:%M:%S:");
//		fprintf(f,"%s%s\n",TimeStr,ErrorStr);
//		fclose(f);
//	}	
//}

//void CMiracleObj::ProgressStart(LPCSTR progress)
//{
//	CString str = "Progress Start: ";
//	str = str + ObjName + progress;
//	DebugMessage(str);
//	if (Intent<MAX_INTENT) Intent++;
//}
//
//void CMiracleObj::ProgressEnd(LPCTSTR progress)
//{
//	CString str = "Progress End: ";
//	str = str + progress;
//	if (Intent>0) Intent--;
//	DebugMessage(str);
//}

void CMiracleObj::CallCount(LPCTSTR progress)
{
	_CallCount++;
	CallCountStr = progress;
}

void CMiracleObj::ClearErrors()
{
	TotalErrorMessage = "\n(AIT Exception Control Center)\n";
}

void CMiracleObj::MsgBox(LPCTSTR message)
{
	CString str = message;
	str = str + "\n" + TotalErrorMessage;
	ClearErrors();
	AfxMessageBox(str,MB_OK| MB_ICONWARNING,0);	
}

void CMiracleObj::MsgBox(UINT str_id)
{
	CString str;
	str.LoadString(str_id);
	str = str + "\n" + TotalErrorMessage;
	ClearErrors();
	AfxMessageBox(str,MB_OK| MB_ICONWARNING,0);	
}
//void CMiracleObj::ErrorMessage(UINT str_id)
//{
//	CString str;
//	str.LoadString(str_id);
//	ErrorMessage(str);
//}


LPCTSTR CMiracleObj::GetErrorString()
{
	return ErrorStr;
}
