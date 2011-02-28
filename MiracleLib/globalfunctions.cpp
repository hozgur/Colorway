// GLOBALFUNCTIONS.CPP
// FROM HAKANOZGUR

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif
#include "stdafx.h"
#include "globalfunctions.h"
#include <shlwapi.h>
#include <string.h>
#define FILE_PATH_MAX 4096 
CWnd *gwndDebug = NULL;
//typedef VOID (CALLBACK* CALLBACKPROC)(WPARAM, LPARAM);
//typedef FARPROC CALLBACKPROC;

MBASEAPI LPCTSTR MIRACLEEXPORT mirGetFileTitle(LPCTSTR lpszPath)
{	
	return PathFindFileName(lpszPath);	
}
MBASEAPI CString MIRACLEEXPORT mirGetFileTitleNoExt(LPCTSTR lpszPath)
{	
	TCHAR szFname[FILE_PATH_MAX];
	_tsplitpath_s(lpszPath, NULL,0, NULL,0, szFname,4096, NULL,0);
	return  CString(szFname);
}
MBASEAPI BOOL MIRACLEEXPORT TestIsValidForFileName(LPCTSTR name)
{
	if (strlen(name) == 0) 	
		return false;
	
	char invalidchars[25] = " :\\/%%.*?+,'^|&;é@`,<>";
	invalidchars[0] = 34; // Çift týrnak eklenir.


	CString str = name;
	if (str.FindOneOf(invalidchars) != -1)	
		return false;
	return true;
}

MBASEAPI BOOL MIRACLEEXPORT mirGetTempFileName(CString &FileName)
{
	const int buffersize = FILE_PATH_MAX;
	TCHAR szTempPath[buffersize];
	TCHAR szTempName[buffersize];

	DWORD dwRetVal = GetTempPath(buffersize,szTempPath); // buffer for path 
	if (dwRetVal > buffersize || (dwRetVal == 0))
	{
		logErrorVA("GetTempPath failed (%d)\n", GetLastError());
		return FALSE;
	}
	UINT uRetVal = GetTempFileName(szTempPath, // directory for tmp files
		TEXT("MIF"),  // temp file name prefix 
		0,            // create unique name 
		szTempName);  // buffer for name 
	if (uRetVal == 0)
	{
		logErrorVA("GetTempFileName failed (%d)\n", GetLastError());
		return FALSE;
	}	
	FileName = szTempName;
	return TRUE;
}

MBASEAPI BOOL MIRACLEEXPORT mirGetAppPath(CString &path)
{
	CString strname;
	const int maxpath = FILE_PATH_MAX;
	char *name = strname.GetBuffer(maxpath);
	if(GetModuleFileName(NULL,name,maxpath)== 0)
	{
		logError("Path Alma hatasý!");
		return FALSE;
	}
	strname.ReleaseBuffer();
	int pos = strname.ReverseFind('\\');
	strname = strname.Left(pos);
	path = strname;
	return TRUE;
}

MBASEAPI LPVOID MIRACLEEXPORT mirLoadResourceData(UINT id,LPCTSTR lpszResourceType)
{
	CString str;
	str.Format("#%d",id);
	HRSRC rsrc = FindResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(id),lpszResourceType);
	if(rsrc == NULL) 
	{
		DWORD errorid = GetLastError();
		TRACE("Error =%ld",errorid);
		return NULL;
	}
	HGLOBAL handle = LoadResource(AfxGetResourceHandle(),rsrc);
	if(handle == NULL) return NULL;
	return LockResource(handle);	
}


MBASEAPI HGLOBAL MIRACLEEXPORT mirLoadResourceData2(UINT id,LPCTSTR lpszResourceType)
{
	CString str;
	str.Format("#%d",id);
	HRSRC rsrc = FindResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(id),lpszResourceType);
	if(rsrc == NULL) 
	{
		DWORD errorid = GetLastError();
		TRACE("Error =%ld",errorid);
		return NULL;
	}
	HGLOBAL handle = LoadResource(AfxGetResourceHandle(),rsrc);
	return handle;	//Unnecessary to unlock or delete. 
}
MBASEAPI DWORD MIRACLEEXPORT mirGetResourceSize(UINT id,LPCTSTR lpszResourceType)
{
	CString str;
	str.Format("#%d",id);
	HRSRC rsrc = FindResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(id),lpszResourceType);
	if(rsrc == NULL) 
	{
		DWORD errorid = GetLastError();
		TRACE("Error =%ld",errorid);
		return NULL;
	}
	return SizeofResource(AfxGetResourceHandle(),rsrc);
}

CString strDebugFile = "c:\\debug.txt";
CString strErrorFile = "c:\\error.txt";
bool initlog = false;
MBASEAPI void MIRACLEEXPORT logInit( LPCTSTR path /*= NULL*/,LPCTSTR prefix /*= NULL*/,bool bDeleteLast/*=TRUE*/ )
{
	DeleteFile(strDebugFile);
	DeleteFile(strErrorFile);
	if(path != NULL)
	{
		if(PathIsDirectory(path))
		{
			CString prestr = prefix;
			if(prestr.GetLength() != 0)
				if(!mirTestIsValidForFileName(prestr))
				{
					ASSERT(FALSE);
					prestr = "";
				}
			char path2[FILE_PATH_MAX];
			strcpy_s(path2,FILE_PATH_MAX,path);
			PathRemoveBackslash(path2);
			strcat_s(path2,FILE_PATH_MAX,"\\");
			strDebugFile = path2 ;
			strErrorFile = path2;
			strDebugFile = strDebugFile + prestr +"Debug.txt";
			strErrorFile = strErrorFile + prestr +"Error.txt";
		}
	}
	if(bDeleteLast)
	{
		DeleteFile(strDebugFile);
		DeleteFile(strErrorFile);	
	}
	initlog = true;
	logDebug("Debug Messaging System Written by Hakan ÖZGÜR");
}
MBASEAPI BOOL MIRACLEEXPORT mirSetDebugWindow(CWnd *wnd)
{
	if(wnd != NULL)
	{
		if((wnd->GetSafeHwnd() != NULL) && ::IsWindow(wnd->GetSafeHwnd()))		
		{
			gwndDebug = wnd;
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		gwndDebug = NULL;
		return TRUE;
	}	
}

MBASEAPI void MIRACLEEXPORT _logError(LPCTSTR msg)
{
	if(!initlog) logInit();
	FILE *f = fopen(strErrorFile,"a+t");
	if (f!=NULL)
	{
		CTime Time = CTime::GetCurrentTime();
		CString TimeStr = Time.Format("%d/%m/%Y-%H:%M:%S:");
		fprintf(f,"%s%s \n",TimeStr,msg);
		fclose(f);
	}	
	_logDebug(msg);	
}
MBASEAPI void MIRACLEEXPORT _logDebug(LPCTSTR msg)
{
	if(!initlog) logInit();
	FILE *f = fopen(strDebugFile,"a+t");
	if (f!=NULL)
	{
		try
		{
			CTime Time = CTime::GetCurrentTime();
			CString TimeStr = Time.Format("%d/%m/%Y-%H:%M:%S:");
			fprintf(f,"%s%s \n",TimeStr,msg);
		}
		catch(...)
		{
			TRACE0("Error on Log.");
			ASSERT(FALSE);
		}
		fclose(f);
	}
	TRACE0(msg);TRACE("\n");
	if(gwndDebug != NULL)
	{
		if((gwndDebug->GetSafeHwnd() != NULL) && ::IsWindow(gwndDebug->GetSafeHwnd()))
		{
			try
			{
				CString str = msg;
				str.Replace(_T("\n"), _T("\r\n"));
				if(str.Find('\n')<0)
					str +="\r\n";
				CString strWndText;
				gwndDebug->GetWindowText(strWndText);
				strWndText += str;
				gwndDebug->SetWindowText(strWndText);
				CEdit e;
				CRuntimeClass *r = e.GetRuntimeClass();
				if(gwndDebug->IsKindOf(r))
				{
					CEdit *wndEdit = (CEdit*) gwndDebug;
					wndEdit->LineScroll(-wndEdit->GetLineCount());
					wndEdit->LineScroll(wndEdit->GetLineCount()-4);
				}
				//gwndDebug->SendMessage(EM_SCROLLCARET);
			}
			catch (...)
			{
				//16/11/10 Tarihinde eklendi progrma çýkýþýnda hata vermeyi engellemek için.
				gwndDebug = NULL;
			}
			
		}
	}
}
MBASEAPI void MIRACLEEXPORT logError(UINT nID)
{
	CString msg;
	msg.LoadString(nID);
	_logError(msg);
}
MBASEAPI void MIRACLEEXPORT logDebug(UINT nID)
{
	CString msg;
	msg.LoadString(nID);
	_logDebug(msg);
}

MBASEAPI void MIRACLEEXPORT logError(LPCTSTR lpszMsg)
{
	ASSERT(AfxIsValidString(lpszMsg));
	_logError(lpszMsg);
}
MBASEAPI void MIRACLEEXPORT logDebug(LPCTSTR lpszMsg)
{
	ASSERT(AfxIsValidString(lpszMsg));
	_logDebug(lpszMsg);
}

MBASEAPI void MIRACLEEXPORT logErrorVA(LPCTSTR lpszFormat,...)
{
	CString msg;
	ASSERT(AfxIsValidString(lpszFormat));
	va_list argList;
	va_start(argList, lpszFormat);
	msg.FormatV(lpszFormat, argList);
	va_end(argList);
	_logError(msg);
}
MBASEAPI void MIRACLEEXPORT logDebugVA(LPCTSTR lpszFormat,...)
{
	CString msg;
	ASSERT(AfxIsValidString(lpszFormat));
	va_list argList;
	va_start(argList, lpszFormat);
	msg.FormatV(lpszFormat, argList);
	va_end(argList);
	_logDebug(msg);
}
MBASEAPI void MIRACLEEXPORT mirGetLastSystemMessage(CString &strMessage)
{
	LPTSTR lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,0,NULL);

	strMessage = lpMsgBuf;
	// Free the buffer.
	LocalFree( lpMsgBuf );
}

MBASEAPI void MIRACLEEXPORT mirShowSystemMessage(DWORD dwMessage)
{
	LPTSTR lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,dwMessage,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,0,NULL);
	AfxMessageBox(lpMsgBuf);
	LocalFree( lpMsgBuf );
}

MBASEAPI void MIRACLEEXPORT mirShowLastSystemMessage(void)
{
	LPTSTR lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,0,NULL);
	AfxMessageBox(lpMsgBuf);
	// Free the buffer.
	LocalFree( lpMsgBuf );
}

//TODO: Unicode ?
MBASEAPI BOOL MIRACLEEXPORT mirTestIsValidForFileName(LPCTSTR lpszName)
{	
	if(!AfxIsValidString(lpszName))
	{
		ERRORMESSAGE(_MLT("Geçersiz dizi."));
		return FALSE;
	}
	if(strlen(lpszName)	== 0)
	{
		ERRORMESSAGE(_MLT("Sýfýr uzunlukta dizi."));
		return FALSE;
	}
	char invalidchars[25] = " :\\/%%.*?+,'^|&;é@`,<>";
	invalidchars[0] = 34; // Çift týrnak eklenir.


	CString str = lpszName;
	if (str.FindOneOf(invalidchars) != -1)	
		return FALSE;
	return TRUE;
}

MBASEAPI CString MIRACLEEXPORT mirGetFileVersionStr( LPCTSTR lpszPath)
{
	CString strVersion;
	DWORD size = GetFileVersionInfoSize(lpszPath,NULL);
	LPVOID pData = (LPVOID) new byte[size];
	if(!GetFileVersionInfo(lpszPath,NULL,size,pData))
	{
		delete [] pData;
		return strVersion;
	}
	VS_FIXEDFILEINFO *vs;
	UINT DataSize;
	if(!VerQueryValue(pData,"\\",(LPVOID*)&vs,&DataSize))
	{
		delete [] pData;
		return strVersion;
	}
	CString title = mirGetFileTitle(lpszPath);	 
	strVersion.Format("%s v%d.%d.%d.%d",title,HIWORD(vs->dwProductVersionMS),LOWORD(vs->dwProductVersionMS),HIWORD(vs->dwProductVersionLS),LOWORD(vs->dwProductVersionLS));
	delete [] pData;
	return strVersion;
}

MBASEAPI double MIRACLEEXPORT mirAdjustRect(CRect &outside,CRect &inside,bool btouchInside)
{
	if(outside.IsRectEmpty() || inside.IsRectEmpty())
		return 0;

	//Touch Outside test edilmedi.
	ASSERT(btouchInside);
	double rx = (double)outside.Width()/inside.Width();
	double ry = (double)outside.Height()/inside.Height();
	double r = 1;

	//Not: Alttaki kýsým iptal edildi bu sayede inside rect küçük olduðu durumda da büyütme yapýlýr.
	// outside inside'dan büyük ise
	// outside'ýn left ve top'ý ayarlanýr.
	//if((rx>1) && (ry>1))
	//{
	//	outside.left = (outside.Width()-inside.Width())/2;
	//	outside.top = (outside.Height()-inside.Height())/2;
	//	// scale yapýlmaz
	//	return r;
	//}
	// Eðer Outside, Inside'dan küçük ise scale'in þekline göre scale oraný seçilir.
	if(btouchInside)
	{
		if(rx<ry) r = rx; else r = ry;
		int w = inside.Width();
		int h = inside.Height();
		outside.DeflateRect((outside.Width() -w*r)/2,(outside.Height() -h*r)/2);
	}
	else
	{
		if(rx<ry) r = ry; else r = rx;

		int w = inside.Width();
		int h = inside.Height();
		inside.DeflateRect((w-outside.Width()/r)/2,(h-outside.Height()/r)/2);
	}
	return r;
}