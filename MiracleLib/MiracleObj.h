// MiracleObj.h: interface for the CMiracleObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIRACLEOBJ_H__1A420972_EA47_11D4_A752_0000214446E3__INCLUDED_)
#define AFX_MIRACLEOBJ_H__1A420972_EA47_11D4_A752_0000214446E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef MBASEDLL_EXP
#	define MBASEDLL_EXP
#endif

class MBASEDLL_EXP CMiracleObj  
{
public:
	LPCTSTR GetErrorString();	
	void MsgBox(LPCTSTR message);
	void MsgBox(UINT str_id);
	void ClearErrors();
	void CallCount(LPCTSTR progress);
	
	
	//void Init();
	//void Init(LPCTSTR name);
	
	//BOOL MyNameIs(LPCTSTR name);	
	//void DebugMessage(LPCTSTR lpszFormat,...);
	//void ErrorMessage(LPCTSTR lpszFormat,...);
	//void ErrorMessage(UINT str_id);
	//void ProgressEnd(LPCTSTR progress);
	//void ProgressStart(LPCSTR progress);
	CMiracleObj(LPCTSTR name);
	virtual ~CMiracleObj();
protected:
	CString ObjName;
	CString ErrorStr;

};

#endif // !defined(AFX_MIRACLEOBJ_H__1A420972_EA47_11D4_A752_0000214446E3__INCLUDED_)
