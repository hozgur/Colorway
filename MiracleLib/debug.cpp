// Debug.cpp
// For debugging purposes

#include "stdafx.h"

//CString ErrorStr;
//BOOL gInfoEnable = TRUE;
//MBASEAPI void MIRACLEEXPORT ErrorMessage(LPCTSTR lpszFormat,...)
//{
//	ASSERT(AfxIsValidString(lpszFormat));
//
//	va_list argList;
//	va_start(argList, lpszFormat);
//	ErrorStr.FormatV(lpszFormat, argList);
//	CString str = ErrorStr;
//	//str = str + " This message was created by Miracle(R)\n";
//	TRACE(str);
//	va_end(argList);
//}
//#ifdef _DEBUG
//MBASEAPI void MIRACLEEXPORT InfoMessage(LPCTSTR lpszFormat,...)
//{
//	if(!gInfoEnable) return;
//	ASSERT(AfxIsValidString(lpszFormat));
//	va_list argList;
//	va_start(argList, lpszFormat);
//	CString str ;
//	str.FormatV(lpszFormat, argList);
//	str = "(i) " + str;
//	//str = str + " This Info message was created by Miracle(R)\n";
//	TRACE(str);
//	va_end(argList);
//}
//#endif