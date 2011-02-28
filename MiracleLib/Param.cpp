// Param.cpp: implementation of the CParam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Param.h"
#include "tools.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CPrm,CObject)
IMPLEMENT_DYNAMIC(CParam,CObject)

CParam::CParam(LPCTSTR inifilename)
{
	ParamCount = 0;
	for (int a = 0;a<MAX_PARAM_COUNT;a++)
		Prm[a] = NULL;
	
	IniFileName = inifilename;	
	LoadParameters();	
}

CParam::~CParam()
{
	SaveParameters();
	for (int a=0;a <ParamCount;a++)
		if (Prm[a]!= NULL) delete Prm[a];
}


BOOL CParam::CreateParam(LPCTSTR name,LPCTSTR cat,LPCTSTR cval,long lval,long min,long max)
{
	// Eðer Parametre daha önce yüklendi ise Category ve Description'ý yenile.

	CPrm *prm = SearchPrm(name);
	if (prm != NULL)
	{
		prm->SetCategory(cat);		
		return true;
	}

	

	if (ParamCount == MAX_PARAM_COUNT)
	{
		logError("Maksimum Parametre sayýsýna eriþildi.");
		return false;
	}

	// Eðer daha önce yüklenmedi ise yenisini yarat;

	Prm[ParamCount] = new CPrm(name,cat,cval,lval,min,max);

	if (Prm[ParamCount] == NULL)
	{
		logError("Parametre Oluþturken hata (CreateParam Str)");
		return false;
	}
	ParamCount++;
	return true;
}

char* CParam::GetParam(LPCTSTR prm_name,short type)
{
	CPrm *prm = SearchPrm(prm_name);
	if (prm == NULL)
	{
		CString str;
		str.Format("Tanýmsýz Parametre ('%s')",prm_name);
		logError(str);
		//ASSERT(FALSE);
		return NULL;
	}

	return prm->GetValue(type);
}

long CParam::GetParam(LPCTSTR prm_name)
{
	CPrm *prm = SearchPrm(prm_name);
	if (prm == NULL)
	{
		CString str;
		str.Format("Tanýmsýz Parametre ('%s')",prm_name);
		logError(str);
//		ASSERT(FALSE);
		return 0;
	}

	return prm->GetValue();
}


void CParam::SetParam(LPCTSTR prm_name,long val)
{
	CPrm *prm = SearchPrm(prm_name);
	if (prm == NULL)
	{

		CString str;
		str.Format("Tanýmsýz Parametre ('%s')",prm_name);
		logError(str);
//		ASSERT(FALSE);
		return ;
	}
	prm->SetValue(val);
}

void CParam::SetParam(LPCTSTR prm_name,LPCTSTR val)
{
	CPrm *prm = SearchPrm(prm_name);
	if (prm == NULL)
	{
		CString str;
		str.Format("Tanýmsýz Parametre ('%s')",prm_name);
		logError(str);
		//ASSERT(FALSE);
		return ;
	}
	prm->SetValue(val);
}
CPrm* CParam::SearchPrm(LPCTSTR name)
{
	for (int a=0;a<ParamCount;a++)
	{
		if (Prm[a]== NULL)
		{
			logError("Parametre kayýtlarý bozuk (SearchPrm)");
			return NULL;
		}
		
		if (strcmp(Prm[a]->Name,name) == 0) return Prm[a];
	}

	return NULL;
}

BOOL CParam::LoadParameters()
{
	char name[MAX_LEN];
	char category[MAX_LEN];
	char cval[MAX_VALUE_LEN];
	long lval,min,max;
	char nametemp[MAX_LEN];
	char cattemp[MAX_LEN];
	char valtemp[MAX_VALUE_LEN];
	int a;
	/*FILE *f;

	f = fopen(IniFileName,"rt");
	if ( f == NULL)
		return false;*/

	for (a=0;a <ParamCount;a++)
		if (Prm[a]!= NULL) delete Prm[a];

	ParamCount = 0;

	CStdioFile file;
	if(!file.Open(IniFileName,CFile::modeRead))
		return false;

	try
	{
		while(1)
		{
			CString line;
			if(!file.ReadString(line))
				return true;
			if(line.GetLength() != 0)
			{
				a = sscanf(line,"%s -> %s = %s ,%ld,%ld,%ld",category,name,cval,&lval,&min,&max);
				if (a == 6)
				{
					DoSpace(nametemp,name);
					DoSpace(cattemp,category);
					DoSpace(valtemp,cval);
					Prm[ParamCount] = new CPrm(nametemp,cattemp,valtemp,lval,min,max);
					if (Prm[ParamCount] != NULL) ParamCount++;

				}
			}
		}
	}
	catch (CMemoryException* e)
	{
		char message[1024];
		e->GetErrorMessage(message,1024);
		logError(message);
		e->Delete();
		return false;
	}
	catch (CFileException* e)
	{
		char message[1024];
		e->GetErrorMessage(message,1024);
		logError(message);
		BOOL stat = 0;
		if(e->m_cause == CFileException::endOfFile)
			stat = 1;
		e->Delete();
		return stat;
	}
	catch (CException* e)
	{
		char message[1024];
		e->GetErrorMessage(message,1024);
		logError(message);
		e->Delete();
		return false;
	}
	catch(...)
	{
		return false;
	}
	//try
	//{
	//	while(!feof(f))
	//	{

	//		a = fscanf(f,"%s -> %s = %s ,%ld,%ld,%ld",category,name,cval,&lval,&min,&max);
	//		if (a == 6)
	//		{
	//			DoSpace(temp1,name);
	//			DoSpace(temp2,category);
	//			DoSpace(temp3,cval);
	//			Prm[ParamCount] = new CPrm(temp1,temp2,temp3,lval,min,max);
	//			if (Prm[ParamCount] != NULL) ParamCount++;

	//		}
	//	}
	//}
	//catch (CException* e)
	//{
	//	//logError("Parametre dosyasý hatasý.");
	//	char message[1024];
	//	e->GetErrorMessage(message,1024);
	//	logError(message);
	//	e->Delete();
	//}
	//catch(...)
	//{
	//	//logError("Parametre dosyasý hatasý.");
	//}
	//fclose(f);
	//DEBUG_ONLY(logDebug("Parameters Loaded"));
	return true;
}

BOOL CParam::SaveParameters()
{
	FILE *f;
	char valtemp[MAX_VALUE_LEN];
	char cattemp[MAX_LEN];
	char nametemp[MAX_LEN];
	if (ParamCount == 0) return true;

	f = fopen(IniFileName,"wt");
	if ( f == NULL)
	{
		logError("Parametreler kaydedilirken hata oluþtu");
		return false;
	}
	
	for (int a= 0;a<ParamCount;a++)
	{		
		NoSpace(cattemp,Prm[a]->Category);
		NoSpace(nametemp,Prm[a]->Name);
		NoSpace(valtemp,Prm[a]->Value);
		fprintf(f,"%s -> %s = %s ,%ld,%ld,%ld \n",cattemp,nametemp,valtemp,Prm[a]->LValue,Prm[a]->Min,Prm[a]->Max);
	}

	fclose(f);
	DEBUG_ONLY(logDebug("Parameters Saved."));
	return true;
}
//Space yerine 127 , yerine 126 kullanýlýyor.
void CParam::NoSpace(LPSTR dest,LPCTSTR val)
{
	unsigned int len = strlen(val);
	if(len>MAX_VALUE_LEN) len = MAX_VALUE_LEN;
	//Burada for döngüsü len+1'e ayarlanýr çünkü string sonu 0 deðerinin kopyalanmasý gerekir. ÇOK ÖNEMLÝ
	for (unsigned int a= 0;a<len+1;a++)
	{
		if (val[a] == ' ') 
			dest[a] = 127;
		else
		if (val[a] == ',') 
			dest[a] = 126;
		else
			dest[a] = val[a];
	}
	
}

void CParam::DoSpace(LPSTR dest,LPCTSTR val)
{
	unsigned int len = strlen(val);
	if(len>MAX_VALUE_LEN) len = MAX_VALUE_LEN;
	//Burada for döngüsü len+1'e ayarlanýr çünkü string sonu 0 deðerinin kopyalanmasý gerekir. ÇOK ÖNEMLÝ
	for (unsigned int a= 0;a<len+1;a++)
		if (val[a] == 127) 
			dest[a] = ' ';
		else
		if (val[a] == 126) 
			dest[a] = ',';
		else
			dest[a] = val[a];
	
}


CPrm::CPrm(LPCTSTR name,LPCTSTR cat,LPCTSTR cval,long lval,long min,long max)
{


	if ((name == NULL) || (cat == NULL) )
	{
		logError(" Parametre yaratma hatasý (CPrm Constructor)");
		return;
	}
	
	strcpy(Name,name);	
	strcpy(Category,cat);
	if ((cval!=NULL) && (strlen(cval) <MAX_VALUE_LEN))
		strcpy(Value,cval);
	else
		strcpy(Value,"Parametre Hatasý!!!");
	LValue = lval;
	Min = min;
	Max = max;

}
CPrm::~CPrm()
{
}
long CPrm::GetValue()
{
	if (LValue > Max) return Max;
	if (LValue <Min) return Min;
	return LValue;
}
char *CPrm::GetValue(short type)
{
	return Value;
}

void CPrm::SetValue(long val)
{
	if (val >Max) val = Max;
	if (val <Min) val = Min;
	LValue = val;
}
void CPrm::SetValue(LPCTSTR val)
{
	if(val == NULL)
		return;
	ASSERT(strlen(val) < MAX_VALUE_LEN);
	strcpy(Value,val);
}

void CPrm::SetCategory(LPCTSTR category)
{
	strcpy(Category,category);
}

//******************************************************************//
//Database Section
//******************************************************************//

BOOL CParam::CreateField(LPCTSTR name,LPCTSTR desc)
{
	return CreateParam(name,"DB",desc,0,0,MAX_RECORD_COUNT);
}

LPCTSTR CParam::GetFieldDesc(LPCTSTR name)
{
	return GetParam(name,PRM_TEXT);
}

BOOL CParam::AddRecord(LPCTSTR name,LPCTSTR strvalue,long val)
{
	CPrm *prm = SearchPrm(name);
	if (prm == NULL)
	{		
		logErrorVA("Undefined field ('%s')",name);		
		return false;
	}
	short recordcount = (short)prm->GetValue();
	if (recordcount >=MAX_RECORD_COUNT)
	{		
		logErrorVA("Max Record Count reached ('%s')",name);		
		return false;
	}
	CString prmname;
	prmname.Format("%s%03d",name,recordcount);
	if (!CreateParam(prmname,"DB",strvalue,val,0xffffffff,0x7fffffff))
	{		
		logErrorVA("Error on creating record ('%s')",name);		
		return false;
	}
	SetParam(prmname,val);
	SetParam(prmname,strvalue);
	prm->SetValue(recordcount+1);
	return TRUE;		

}
short CParam::GetRecordCount(LPCTSTR name)// return -1 on error
{
	CPrm *prm = SearchPrm(name);
	if (prm == NULL)
	{		
		logErrorVA("Undefined field on Get Record Count('%s')",name);		
		return -1;
	}
	return (short) prm->GetValue();
}
char *CParam::GetRecord(LPCTSTR name,short index,short type)
{
	CPrm *prm = SearchPrm(name);
	if (prm == NULL)
	{		
		logErrorVA("Undefined field on GetRecord(str)('%s')",name);		
		return NULL;
	}
	short rc = (short)prm->GetValue();
	if ((index<0) || (index >=rc))
	{		
		logErrorVA("Wrong Index Number on GetRecord(str) ('%s')",name);		
		return NULL;
	}
	CString prmname;
	prmname.Format("%s%03d",name,index);
	return GetParam(prmname,PRM_TEXT);
}
long CParam::GetRecord(LPCTSTR name,short index)
{
	CPrm *prm = SearchPrm(name);
	if (prm == NULL)
	{		
		logErrorVA("Undefined field on GetRecord(long) ('%s')",name);		
		return 0;
	}
	short rc = (short)prm->GetValue();
	if ((index<0) || (index >=rc))
	{		
		logErrorVA("Wrong Index Number on GetRecord(long) ('%s')",name);		
		return 0;
	}
	CString prmname;
	prmname.Format("%s%03d",name,index);
	return GetParam(prmname);
}
BOOL CParam::SetRecord(LPCTSTR name,short index,LPCTSTR strval)
{
	CPrm *prm = SearchPrm(name);
	if (prm == NULL)
	{		
		logErrorVA("Undefined field on SetRecord(str) ('%s')",name);		
		return false;
	}
	short rc = (short)prm->GetValue();
	if ((index<0) || (index >=rc))
	{		
		logErrorVA("Wrong Index Number on SetRecord(str) ('%s')",name);		
		return false;
	}
	CString prmname;
	prmname.Format("%s%03d",name,index);
	SetParam(prmname,strval);
	return TRUE;
}
BOOL CParam::SetRecord(LPCTSTR name,short index,long val)
{
	CPrm *prm = SearchPrm(name);
	if (prm == NULL)
	{		
		logErrorVA("Undefined field on SetRecord(long) ('%s')",name);		
		return false;
	}
	short rc = (short)prm->GetValue();
	if ((index<0) || (index >=rc))
	{		
		logErrorVA("Wrong Index Number on SetRecord(long) ('%s')",name);		
		return false;
	}
	CString prmname;
	prmname.Format("%s%03d",name,index);
	SetParam(prmname,val);
	return TRUE;
}
BOOL CParam::DeleteRecord(LPCTSTR name,short index)
{
	CPrm *prm = SearchPrm(name);
	if (prm == NULL)
	{		
		logErrorVA("Undefined field on DeleteRecord(long) ('%s')",name);		
		return false;
	}
	short rc = (short)prm->GetValue();
	if (rc == 0)
	{		
		logErrorVA("NoRecord on DeleteRecord(long) ('%s')",name);		
		return false;
	}
	if ((index<0) || (index >=rc))
	{		
		logErrorVA("Wrong Index Number on SetRecord(long) ('%s')",name);		
		return false;
	}
	CString p1,p2;
	for (int a =index;a<rc-1;a++)
	{
		p1.Format("%s%03d",name,a);
		p2.Format("%s%03d",name,a+1);
		SetParam(p1,GetParam(p2));// long
		SetParam(p1,GetParam(p2,PRM_TEXT));// long
	}
	SetParam(name,rc-1);
	return TRUE;
}

