// MFile.h: interface for the CMFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFILE_H__1A420979_EA47_11D4_A752_0000214446E3__INCLUDED_)
#define AFX_MFILE_H__1A420979_EA47_11D4_A752_0000214446E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MiracleObj.h"

enum OPEN_TYPES {OT_READ,OT_WRITE,OT_APPEND,OT_READWRITE};

class MBASEDLL_EXP CMFile : public CObject  
{
	DECLARE_DYNAMIC(CMFile)
protected:
	BOOL FileOpen;
	byte OpenType;
	CFile *File;
	BOOL ReadOnly;
	BOOL Hidden;
	BOOL System;
	BOOL Normal;
	BOOL Archive;
	BOOL bDontClose;

public:
	LPCTSTR GetErrorString(int err);
	
// File Access Functions
	CString ErrorStr;
	BOOL FDeleteFile();
	BOOL IsOpened();
	BOOL FOpen(LPCTSTR filename,int opentype);
	BOOL FClose();
	unsigned long FRead(void *buffer,unsigned long size);
	unsigned long FWrite(void *buffer,unsigned long size);
	BOOL FSeekToBegin(unsigned long pos);
	BOOL FSeekRelative(unsigned long pos);
	unsigned long GetCurrentPosition();
	unsigned long FGetLenght();
	static BOOL GetCreateTime(LPCTSTR filepath,CTime *time);
	static BOOL GetModifiedTime(LPCTSTR filepath,CTime *time);
	BOOL GetModifiedTime(CTime *time);
	BOOL IsReadOnly() {return ReadOnly;};
	BOOL IsHidden() {return Hidden;};
	BOOL IsSystemFile() {return System;};
	BOOL IsNormalFile() {return Normal;};
	BOOL IsArchive() {return Archive;};
	void GetFileTitle(LPSTR name);
	CString GetFileTitle();
	BOOL ReadString(CString *str);
	void ReadString(CString *str,CException *e); // exception version
	BOOL WriteString(LPCTSTR str);
	CMFile();
	CMFile(CFile *file);
	virtual ~CMFile();
	
};

#endif // !defined(AFX_MFILE_H__1A420979_EA47_11D4_A752_0000214446E3__INCLUDED_)
