// Mail.cpp: implementation of the CMail class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\include\mail.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMail::CMail():CMiracleObj("Mail")
{
	nMessageSize = 0x000004000;
	lpszSeedMessageID = &szSeedMessageID[0];
	lpszMessageID = &szMessageID[0];
	pszSubject = &szSubject[0];
	pszNoteText = &szNoteText[0];
	pszDateReceived = "1991/03/23 12:00";
	
	rdOriginator.lpEntryID = 0L;
	rdOriginator.lpszAddress = MAPI_ORIG;
	rdOriginator.lpszName = (LPSTR) "ONE";
	rdOriginator.ulRecipClass = NULL;
	IsLogon = FALSE;
	FLAGS flFlag = 0L;
	ULONG ulResult;
	wnd = NULL;
	if (InitMAPI() != 0)
	{
		MsgBox("Init MAPI Error");		
		return;
	}

	InitMessage(&mmMapiMessage);

	flFlag =MAPI_NEW_SESSION | MAPI_LOGON_UI;
	ulResult = (lpfnMAPILogon)(wnd, NULL, NULL, flFlag, 0L, (LPLHANDLE)&hMAPISession);
	if (ulResult !=0)
	{
		MsgBox("Init Logon Error");		
		return;
	}
    IsLogon = TRUE;

}

CMail::~CMail()
{
	if(IsLogon)
		(*lpfnMAPILogoff)(hMAPISession, wnd, 0L, 0L);
	DeInitMAPI();
}

void CMail::InitMessage(lpMapiMessage pmmMessage)
{
  pmmMessage->ulReserved = 0L;
  pmmMessage->lpszSubject ="";
  pmmMessage->lpszNoteText ="";
  pmmMessage->lpszMessageType = NULL;
  pmmMessage->lpszDateReceived = pszDateReceived;
  pmmMessage->flFlags = MAPI_UNREAD;
  pmmMessage->lpOriginator = &rdOriginator;
  pmmMessage->nRecipCount = 0L;
  pmmMessage->lpRecips = NULL;
  pmmMessage->nFileCount = 0L;
  pmmMessage->lpFiles = NULL;
}

int FAR PASCAL CMail::InitMAPI()
{

  if ((hLibrary = LoadLibrary(MAPIDLL)) < (HANDLE)32)
    return(ERR_LOAD_LIB);

  if ((lpfnMAPILogon = (LPMAPILOGON)GetProcAddress(hLibrary,SZ_MAPILOGON)) == NULL)
    return(ERR_LOAD_FUNC);

  if ((lpfnMAPILogoff= (LPMAPILOGOFF)GetProcAddress(hLibrary,SZ_MAPILOGOFF)) == NULL)
    return(ERR_LOAD_FUNC);

  if ((lpfnMAPISendMail= (LPMAPISENDMAIL)GetProcAddress(hLibrary,SZ_MAPISENDMAIL)) == NULL)
    return(ERR_LOAD_FUNC);

  if ((lpfnMAPISendDocuments= (LPMAPISENDDOCUMENTS)GetProcAddress(hLibrary,SZ_MAPISENDDOC)) == NULL)
    return(ERR_LOAD_FUNC);

  if ((lpfnMAPIFindNext= (LPMAPIFINDNEXT)GetProcAddress(hLibrary,SZ_MAPIFINDNEXT)) == NULL)
    return(ERR_LOAD_FUNC);

  if ((lpfnMAPIReadMail= (LPMAPIREADMAIL)GetProcAddress(hLibrary,SZ_MAPIREADMAIL)) == NULL)
    return(ERR_LOAD_FUNC);

  if ((lpfnMAPISaveMail= (LPMAPISAVEMAIL)GetProcAddress(hLibrary,SZ_MAPISAVEMAIL)) == NULL)
    return(ERR_LOAD_FUNC);

  if ((lpfnMAPIDeleteMail= (LPMAPIDELETEMAIL)GetProcAddress(hLibrary,SZ_MAPIDELMAIL)) == NULL)
    return(ERR_LOAD_FUNC);

  if ((lpfnMAPIFreeBuffer= (LPMAPIFREEBUFFER)GetProcAddress(hLibrary,SZ_MAPIFREEBUFFER)) == NULL)
    return(ERR_LOAD_FUNC);

  if ((lpfnMAPIAddress= (LPMAPIADDRESS)GetProcAddress(hLibrary,SZ_MAPIADDRESS)) == NULL)
    return(ERR_LOAD_FUNC);

  if ((lpfnMAPIDetails= (LPMAPIDETAILS)GetProcAddress(hLibrary,SZ_MAPIDETAILS)) == NULL)
    return(ERR_LOAD_FUNC);

  if ((lpfnMAPIResolveName= (LPMAPIRESOLVENAME)GetProcAddress(hLibrary,SZ_MAPIRESOLVENAME)) == NULL)
    return(ERR_LOAD_FUNC);

  return(0);
}


int FAR PASCAL CMail::DeInitMAPI()
{

  lpfnMAPILogon = NULL;
  lpfnMAPILogoff= NULL;
  lpfnMAPISendMail= NULL;
  lpfnMAPISendDocuments= NULL;
  lpfnMAPIFindNext= NULL;
  lpfnMAPIReadMail= NULL;
  lpfnMAPISaveMail= NULL;
  lpfnMAPIDeleteMail= NULL;
  lpfnMAPIFreeBuffer = NULL;
  lpfnMAPIAddress= NULL;
  lpfnMAPIDetails = NULL;
  lpfnMAPIResolveName;

  FreeLibrary(hLibrary);

  return(0);
}

BOOL CMail::SendMail()
{	
    ULONG ulResult = (*lpfnMAPISendMail)(hMAPISession, wnd, &mmMapiMessage, (FLAGS)MAPI_DIALOG, 0L);
	if (ulResult != 0)
	{		
		return FALSE;
	}
	return TRUE;
}

BOOL CMail::SendFile(LPCTSTR path, LPCTSTR name)
{
	ULONG ulResult = (*lpfnMAPISendDocuments)(wnd, ";", (LPSTR)path,(LPSTR)name, 0L);
	if (ulResult != 0)
	{		
		return FALSE;
	}
	return TRUE;
}
