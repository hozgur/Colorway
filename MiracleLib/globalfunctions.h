// GLOBAL FUNCTIONS
// HAKAN ÖZGÜR 2001
#if !defined GLOBALFUNCTIONS_HAKANOZGUR
#define GLOBALFUNCTIONS_HAKANOZGUR

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
MBASEAPI LPCTSTR MIRACLEEXPORT mirGetFileTitle(LPCTSTR lpszPath);
MBASEAPI CString MIRACLEEXPORT mirGetFileTitleNoExt(LPCTSTR lpszPath);
MBASEAPI BOOL MIRACLEEXPORT mirGetAppPath(CString &path);
MBASEAPI BOOL MIRACLEEXPORT TestIsValidForFileName(LPCTSTR name);
MBASEAPI LPVOID MIRACLEEXPORT mirLoadResourceData(UINT id,LPCTSTR lpszResourceType);
MBASEAPI HGLOBAL MIRACLEEXPORT mirLoadResourceData2(UINT id,LPCTSTR lpszResourceType);
MBASEAPI DWORD MIRACLEEXPORT mirGetResourceSize(UINT id,LPCTSTR lpszResourceType);
MBASEAPI void MIRACLEEXPORT logInit(LPCTSTR path = NULL,LPCTSTR prefix = NULL,bool bDeleteLast=TRUE);
MBASEAPI BOOL MIRACLEEXPORT mirSetDebugWindow(CWnd *wnd);
MBASEAPI void MIRACLEEXPORT _logError(LPCTSTR msg);
MBASEAPI void MIRACLEEXPORT _logDebug(LPCTSTR msg);
MBASEAPI void MIRACLEEXPORT logError(UINT nID);
MBASEAPI void MIRACLEEXPORT logDebug(UINT nID);
MBASEAPI void MIRACLEEXPORT logErrorVA(LPCTSTR lpszFormat,...);
MBASEAPI void MIRACLEEXPORT logDebugVA(LPCTSTR lpszFormat,...);
MBASEAPI void MIRACLEEXPORT logError(LPCTSTR lpszMsg);
MBASEAPI void MIRACLEEXPORT logDebug(LPCTSTR lpszMsg);
MBASEAPI BOOL MIRACLEEXPORT mirGetTempFileName(CString &FileName);
MBASEAPI void MIRACLEEXPORT mirGetLastSystemMessage(CString &strMessage);// GetLastError() ile alýnacak kod yerine hata stringi dönülür.
MBASEAPI void MIRACLEEXPORT mirShowLastSystemMessage(CString &strMessage);// GetLastError() ile öðrenilebilecek hata mesajý direkt olarak ekrana getirilir.
MBASEAPI void MIRACLEEXPORT mirShowSystemMessage(DWORD dwMessage);
MBASEAPI BOOL MIRACLEEXPORT mirTestIsValidForFileName(LPCTSTR lpszName);
MBASEAPI CString MIRACLEEXPORT mirGetFileVersionStr(LPCTSTR lpszPath);

//Resim görüntülemede bir resmi bir dikdörgen içerisine yerleþtirme hep sýkýntý olmuþtur.
//Bunu çözmek için bu fonksiyon geliþtirildi. Ýlk kullaným olarak MIFExtensions'da PreviewDlg Logo yükleme bölümüne kondu.
MBASEAPI double MIRACLEEXPORT mirAdjustRect(CRect &outside,CRect &inside,bool btouchInside = true);

#define ERRORMESSAGE(str) logErrorVA("%s at File = %s Line = %d",str,mirGetFileTitle(__FILE__),__LINE__)
#define INFOMESSAGE(str)  logDebugVA("%s at File = %s Line = %d",str,mirGetFileTitle(__FILE__),__LINE__)
#define INT_ERROR _T("Internal Error.")
#define MEM_ERROR _T("Memory Fault.")
#define FILE_ERROR _T("Invalid File.")
#define UNSUPPORTED_FILE_ERROR _T("Unsupported File Type.")
#define VALID_ERROR _T("Invalid Object.")
#define INVALID_INDEX _T("Invalid Index.")
#define PRM_ERROR _T("Parameter Error.")
#define USER_ABORT _T("Aborted by User.")
#define _MLT(T) _T(T)
#endif // GLOBALFUNCTIONS_HAKANOZGUR