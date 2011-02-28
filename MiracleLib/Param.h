// Param.h: interface for the CParam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAM_H__125D94E3_F626_11D4_A752_0000214446E3__INCLUDED_)
#define AFX_PARAM_H__125D94E3_F626_11D4_A752_0000214446E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum PRM_TYPES {PRM_UINT,PRM_STRING};

#define MAX_PARAM_COUNT 1000
#define MAX_RECORD_COUNT 1000
#define MAX_VALUE_LEN 1500
#define MAX_LEN 50
#define PRM_TEXT 1
class MBASEDLL_EXP CPrm: public CObject
{
	DECLARE_DYNAMIC(CPrm)
public:
	CPrm(LPCTSTR name,LPCTSTR cat,LPCTSTR cval,long lval,long min,long max);
	~CPrm();
	long GetValue();
	char *GetValue(short type);
	void SetValue(long val);
	void SetValue(LPCTSTR val);
	void SetCategory(LPCTSTR category);

	byte Type;
	long LValue;
	long Min;
	long Max;
	char Value[MAX_VALUE_LEN];
	char Name[MAX_LEN];
	char Category[MAX_LEN];
	

};


class MBASEDLL_EXP CParam : public CObject  
{
	DECLARE_DYNAMIC(CParam)
public:
	LPCTSTR GetFieldDesc(LPCTSTR name);
	
	void DoSpace(LPSTR dest,LPCTSTR val);
	void NoSpace(LPSTR dest,LPCTSTR val);
	BOOL SaveParameters();
	BOOL LoadParameters();
	CPrm* SearchPrm(LPCTSTR name);
	CParam(LPCTSTR inifilename);
	CParam(){};
	BOOL SetFileName(LPCTSTR lpszFileName)
	{
		IniFileName = lpszFileName;
		return TRUE;
	}
	virtual ~CParam();

	BOOL CreateParam(LPCTSTR name,LPCTSTR cat,LPCTSTR cval,long lval,long min,long max);
	char *GetParam(LPCTSTR prm_name,short type);
	long GetParam(LPCTSTR prm_name);
	void SetParam(LPCTSTR prm_name,long val);
	void SetParam(LPCTSTR prm_name,LPCTSTR val);

// DataBase Section
	BOOL CreateField(LPCTSTR name,LPCTSTR desc);
	BOOL AddRecord(LPCTSTR name,LPCTSTR strvalue,long val);
	short GetRecordCount(LPCTSTR name);// return -1 on error
	char *GetRecord(LPCTSTR name,short index,short type);
	long GetRecord(LPCTSTR name,short index);
	BOOL SetRecord(LPCTSTR name,short index,LPCTSTR strval);
	BOOL SetRecord(LPCTSTR name,short index,long val);
	BOOL DeleteRecord(LPCTSTR name,short index);
// end of Database Section

	
protected:
	char *Name;
	CPrm *Prm[MAX_PARAM_COUNT];
	short ParamCount;
	CString IniFileName;

};
//extern CParam mirParam;

#endif // !defined(AFX_PARAM_H__125D94E3_F626_11D4_A752_0000214446E3__INCLUDED_)
