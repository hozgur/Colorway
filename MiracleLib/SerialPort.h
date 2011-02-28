#pragma once

// CSerialPort command target
#include "..\MiracleLib\Duration.h"
class MBASEDLL_EXP CSerialPort : public CObject
{
public:
	CSerialPort();
	virtual ~CSerialPort();
	bool Open(LPCTSTR lpszComPort,LPCTSTR lpszConString,int delay_us);
	void Close();
	int Read(byte *buf,int width);
	int Write(byte *buf,int width,bool bFast = false);
	bool WriteByte(byte val);
	int ReadByte();
	int Delay_US;
protected:
	HANDLE m_hComm;
	void GetErrorStr(CString& Str);
	CDuration dur;
	
	void Wait(int microseconds);
};


