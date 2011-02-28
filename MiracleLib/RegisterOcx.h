//RegisterOcx.h
//Written by Hakan OZGUR
// 11/09/2008
BOOL RegisterOCX(LPCTSTR pszDllName)
{
	BOOL status = FALSE;
	HRESULT (STDAPICALLTYPE * lpDllEntryPoint)(void);
	LPCSTR pszDllEntryPoint = "DllRegisterServer";
	// Initialize OLE.
	if (FAILED(OleInitialize(NULL))) 
		return FALSE;
	SetErrorMode(SEM_FAILCRITICALERRORS);       // Make sure LoadLib fails.
	// Load the library.
	HINSTANCE hLib = LoadLibraryEx(pszDllName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (hLib < (HINSTANCE)HINSTANCE_ERROR)	
		goto CleanupOle;
	// Find the entry point.
	(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, pszDllEntryPoint);
	if (lpDllEntryPoint == NULL) 
		goto CleanupLibrary;
	
	// Call the entry point.
	if (FAILED((*lpDllEntryPoint)()))		
		goto CleanupLibrary;
	
	status = TRUE;
CleanupLibrary:
		FreeLibrary(hLib);
CleanupOle:
	OleUninitialize();	
	return status;
}