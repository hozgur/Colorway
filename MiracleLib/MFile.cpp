// MFile.cpp: implementation of the CMFile class.
//
// Bu Class MFC'nin CFile'ýna baðlýdýr 
//baþka bir platformda CFile Class'ý dönüþtürülmelidir.

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MFile.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CMFile,CObject)

CMFile::CMFile()
{
	File = new CFile();
	FileOpen = false;
	ReadOnly = false;
	Hidden = false;
	System = false;
	Normal = false;
	Archive = false;
	bDontClose = false;
}
CMFile::CMFile(CFile *file)
{
	File = file;
	ReadOnly = false;
	Hidden = false;
	System = false;
	Normal = false;
	Archive = false;
	bDontClose = true;
	FileOpen = (file->m_hFile != NULL);
}

CMFile::~CMFile()
{	
	if(!bDontClose)
	{
		FClose();
		if ( File != NULL)	delete File;
	}
}


BOOL CMFile::FOpen(LPCSTR filename,int opentype)
{
	if (File == NULL) return false;
	if(bDontClose) return false;//bDontClose true ise CFile dýþardan set edilmiþ demektir.
	UINT openflag;
	CFileStatus status;
	if (FileOpen)
	{
		logError("Dosya zaten açýk");
		return false;
	}
	switch (opentype)
	{
	case OT_READ		: openflag = CFile::modeRead |CFile::typeBinary;break;
	case OT_WRITE		: openflag = CFile::modeCreate |CFile::modeWrite|CFile::typeBinary;break;
	case OT_APPEND		: openflag = CFile::modeWrite | CFile::modeNoTruncate|CFile::typeBinary;break;
	case OT_READWRITE	: openflag = CFile::modeReadWrite|CFile::typeBinary;break;		
	default: ASSERT(false);return false;break;
	}
	CFileException e;
	
	if (File->Open(filename,openflag,&e))
	{
		if (File->GetStatus(status))
		{
			ReadOnly = ( (status.m_attribute & 1) != 0);
			Hidden = ( (status.m_attribute & 2) != 0);
			System = ( (status.m_attribute & 4) != 0);
			Archive = ( (status.m_attribute & 0x20) != 0);
			Normal =  status.m_attribute == 0;
			FileOpen = true;
			OpenType = opentype;
			if (opentype == OT_APPEND) 
				File->SeekToEnd();
			return true;
		}
		else
		{
			CString temp;
			temp.Format("(%s)Could not read File Status. Process Terminated.",filename);
			logError(temp);
			return false;
		}
	}
	else
	{		
		char error[200];
		e.GetErrorMessage(error,200);
		logError(error);		
		return false;
	}
		
}
BOOL CMFile::FClose()
{
	if(bDontClose) return false;
	if (FileOpen)
		File->Close();
	FileOpen = false;
	return true;
}
unsigned long CMFile::FRead(void *buffer,unsigned long size)
{	
	if(bDontClose)
		return File->Read(buffer,size);
	if(FileOpen)
		if ((OpenType == OT_READ) || (OpenType == OT_READWRITE))
			return File->Read(buffer,size);
	return -1;	
}
unsigned long CMFile::FWrite(void *buffer,unsigned long size)
{
	if(bDontClose)
	{
		File->Write(buffer,size);
		return size;
	}
	if ((FileOpen) && (OpenType != OT_READ))		
	{
		File->Write(buffer,size);
		return size;
	}
	else
	{
		if (!FileOpen)
			logError("Internal Program Error on FWrite (File closed)");
		else
			logError("File Opened for reading but it excepted Write process");
		return 0;
	}	
}
BOOL CMFile::FSeekToBegin(unsigned long pos)
{
	if (FileOpen)
	{
		unsigned long newpos = (unsigned long)File->Seek(pos,CFile::begin);
		return (newpos == pos);
	}
	else
		return false;
}
BOOL CMFile::FSeekRelative(unsigned long pos)
{
	if (FileOpen)
	{
		unsigned long curpos = (unsigned long)File->GetPosition();
		unsigned long newpos = (unsigned long)File->Seek(pos,CFile::current);
		return (newpos == curpos+pos);
	}
	else
		return false;
}

unsigned long CMFile::GetCurrentPosition()
{
	return (unsigned long)File->GetPosition();
}

unsigned long CMFile::FGetLenght()
{
	if (FileOpen)
		return (unsigned long)File->GetLength();
	else
		return 0;
}

BOOL CMFile::GetCreateTime(LPCTSTR filepath,CTime *time)
{
	CFileStatus status;

	if (CFile::GetStatus( filepath, status ) )
	{
		*time = status.m_ctime;
		return true;
	}

	return false;
}

BOOL CMFile::GetModifiedTime(LPCTSTR filepath,CTime *time)
{
	CFileStatus status;

	if (CFile::GetStatus( filepath, status ) )
	{
		*time = status.m_mtime;
		return true;
	}

	return false;
}

BOOL CMFile::GetModifiedTime(CTime *time)
{
	CFileStatus status;

	if (!FileOpen) return false;
	if (File->GetStatus(status ))
	{
		*time = status.m_mtime;
		return true;
	}

	return false;
}

void CMFile::GetFileTitle(LPSTR name)
{
	if (File == NULL) return;
	CString str = File->GetFileTitle();
	CString nm = File->GetFileName();
	if (str == nm)
	{
		short pos = str.Find('.',0);
		str = str.Left(pos);
	}
	strcpy(name,str);	
}

CString CMFile::GetFileTitle()
{
	
	if (File == NULL)
	{	
		CString str = "Program Hatasý (MFile)";
		return str;
	}
	CString str = File->GetFileTitle();
	CString name = File->GetFileName();
	if (str == name)
	{
		short pos = str.Find('.',0);
		str = str.Left(pos);
	}
	return 	str;
}

BOOL CMFile::IsOpened()
{
	return FileOpen;
}

BOOL CMFile::FDeleteFile()
{
	if(bDontClose) return FALSE;
	if (FileOpen)
		FClose();
	return DeleteFile(File->GetFilePath());
}

LPCTSTR CMFile::GetErrorString(int err)
{
	switch(err)
	{
	case CFileException::none: ErrorStr ="No error occurred.";break;
	case CFileException::fileNotFound: ErrorStr ="The file could not be located.";break;
	case CFileException::badPath: ErrorStr ="All or part of the path is invalid.";break;
	case CFileException::tooManyOpenFiles: ErrorStr ="The permitted number of open files was exceeded.";break;
	case CFileException::accessDenied: ErrorStr ="The file could not be accessed.";break;
	case CFileException::invalidFile: ErrorStr ="There was an attempt to use an invalid file handle.";break;
	case CFileException::removeCurrentDir: ErrorStr ="The current working directory cannot be removed.";break;
	case CFileException::directoryFull: ErrorStr ="There are no more directory entries.";break;
	case CFileException::badSeek: ErrorStr ="There was an error trying to set the file pointer.(BadSeek)";break;
	case CFileException::hardIO: ErrorStr ="There was a hardware error.";break;
	case CFileException::sharingViolation: ErrorStr ="SHARE.EXE was not loaded, or a shared region was locked.";break;
	case CFileException::lockViolation: ErrorStr ="There was an attempt to lock a region that was already locked.";break;
	case CFileException::diskFull: ErrorStr ="The disk is full.";break;
	case CFileException::endOfFile: ErrorStr ="The end of file was reached.";break;
	default : ErrorStr = "Unexpexted Error";		
	}

   return ErrorStr;
}

BOOL CMFile::WriteString(LPCTSTR str)
{
	int len = strlen(str)+1;
	if (len >300) len = 300;
	short l = len;
	FWrite(&l,sizeof(short));
	FWrite((LPSTR)str,l);
	return TRUE;
	
}
BOOL CMFile::ReadString(CString *str)
{
	short l;
	FRead(&l,sizeof(short));
	if ((l<=0) || (l>300)) return FALSE;
	LPSTR s = str->GetBuffer(l);
	FRead(s,l);
	str->ReleaseBuffer();
	return TRUE;
}

void CMFile::ReadString(CString *str, CException *e)
{
	short l;
	FRead(&l,sizeof(short));
	if ((l<=0) || (l>300)) {throw e;};
	LPSTR s = str->GetBuffer(l);
	FRead(s,l);
	str->ReleaseBuffer();
}