// SerialPort.cpp : implementation file
//

#include "stdafx.h"
#include "MiracleLibDLL.h"
#include "SerialPort.h"


// CSerialPort

//for logging
#define LogMessage logError

CSerialPort::CSerialPort()
{
	Delay_US = 0;
	m_hComm = 0;
}

CSerialPort::~CSerialPort()
{
	Close();
}
void CSerialPort::GetErrorStr(CString& Str)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		0, // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	Str = (LPCTSTR)lpMsgBuf;	
	// Free the buffer.
	LocalFree( lpMsgBuf );
}
bool CSerialPort::Open( LPCTSTR lpszComPort,LPCTSTR lpszConString ,int delay_us)
{
	CString sPort;
	sPort.Format(_T("\\\\.\\%s"), lpszComPort);

	m_hComm = CreateFile(sPort,  GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,0,0);
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		CString str;
		GetErrorStr(str);
		LogMessage(str);
		return false;
	}
	Delay_US = delay_us;
	// Set Com Settings
	DCB dcb;
	FillMemory(&dcb, sizeof(dcb), 0);
	dcb.DCBlength = sizeof(dcb);
	if (!BuildCommDCB(lpszConString, &dcb))
	{   
		// Couldn't build the DCB. Usually a problem
		// with the communications specification string.
		CString str;
		GetErrorStr(str);
		LogMessage(str);
		LogMessage("Seri Port Hatasý. (BuildComDCB)");		
		CloseHandle(m_hComm);
		m_hComm = NULL;
		return false;
	}
	if (!SetCommState(m_hComm, &dcb))
	{
		CString str;
		GetErrorStr(str);
		LogMessage(str);
		LogMessage("Seri Port Hatasý. (SetCommState)");
		CloseHandle(m_hComm);
		m_hComm = NULL;
		return false;
	}
	COMMTIMEOUTS cm;
	FillMemory(&cm, sizeof(cm), 0);
	cm.ReadIntervalTimeout = 10;
	cm.ReadTotalTimeoutConstant = 10;
	if(!SetCommTimeouts(m_hComm,&cm))
	{
		CString str;
		GetErrorStr(str);
		LogMessage(str);
		LogMessage("Seri Port Hatasý. (SetCommTimeouts)");
		CloseHandle(m_hComm);
		m_hComm = NULL;
		return false;
	}	
	LogMessage("Seri port açýldý.");
	return true;
}

void CSerialPort::Close()
{
	CloseHandle(m_hComm);
	m_hComm = NULL;
	LogMessage("Seri port kapatýldý.");
}

CString  mirGetLastSystemMessage()
{
	LPTSTR lpMsgBuf;
	CString str;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,0,NULL);

	str = lpMsgBuf;
	// Free the buffer.
	LocalFree( lpMsgBuf );
	return str;
}
int CSerialPort::ReadByte()
{
	if(m_hComm == NULL) return -1;
	DWORD dwRead=0;
	byte val;
	if (!ReadFile(m_hComm, &val, 1, &dwRead, NULL))
	{
		logError(mirGetLastSystemMessage());
		return -1;
	}
	if(dwRead == 1)
	{
		Wait(Delay_US);
		return val;
	}
	else
		return -1;
}
int CSerialPort::Read(byte *buf,int width )
{
	if(m_hComm == NULL) return -1;
	DWORD dwRead=0;
	if (!ReadFile(m_hComm, buf, width, &dwRead, NULL))
	{
		logError(mirGetLastSystemMessage());
		return -1;
	}
	return dwRead;
}
bool CSerialPort::WriteByte(byte val)
{
	if(m_hComm == NULL) return false;
	DWORD dwWrite=0;
	if (!WriteFile(m_hComm, &val,1,&dwWrite, NULL))
	{
		logError(mirGetLastSystemMessage());
		return false;
	}
	Wait(Delay_US);	
	return true;
}

int CSerialPort::Write(byte *buf,int width,bool bFast )
{
	if(m_hComm == NULL) return -1;
	if(bFast)
	{
		DWORD dwWrite =0; 
		if (!WriteFile(m_hComm, buf,width,&dwWrite, NULL))
		{
			logError(mirGetLastSystemMessage());
			return -1;
		}
		if(dwWrite != width) return -1;
	}
	else
	{
		for(int a=0;a<width;a++)
			if(WriteByte(buf[a])<=0) return -1;
	}
	
	return width;
}

void CSerialPort::Wait( int microseconds )
{
	dur.Start();
	do 
	{
		dur.Stop();
	}
	while (dur.GetDuration()<microseconds);
}
// CSerialPort member functions
