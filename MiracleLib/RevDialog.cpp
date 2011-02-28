// RevDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RevDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRevDialog dialog


CRevDialog::CRevDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRevDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRevDialog)
	m_revtext = _T("");
	m_build = _T("");
	//}}AFX_DATA_INIT

	RevParam = new CParam("RevSettings.ini");

}

CRevDialog::~CRevDialog()
{
	if (RevParam != NULL)
		delete RevParam;

}
void CRevDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRevDialog)
	DDX_Text(pDX, IDC_EDIT1, m_revtext);
	DDX_Text(pDX, IDC_BUILD, m_build);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRevDialog, CDialog)
	//{{AFX_MSG_MAP(CRevDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRevDialog message handlers


BOOL CRevDialog::OnInitDialog() 
{
	if (RevParam == NULL) return false;
	CDialog::OnInitDialog();
	RevParam->CreateParam("Build","Build Sayýsý","empty",0,0,100000);
	char temp[40];
	sprintf(temp,"Build%5d :",RevParam->GetParam("Build"));
	m_build = temp;
	m_revtext = "";
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRevDialog::IsModified()
{
	CTime AppTime;
	CTime RevTime;
	CMFile File;
	strcpy(AppName,AfxGetAppName());
	strcpy(RevFileName,AppName);
	strcat(RevFileName," Revision History.txt");
	strcat(AppName,".exe");
	
	if (File.GetModifiedTime(RevFileName,&RevTime) == false)
	{
		FILE *f;
		f = fopen(RevFileName,"wt");
		if (f == NULL)
		{
			MessageBox("Revision History can't create.");
			return false;
		}
		fprintf(f,"Revision History File");
		fclose(f);		
		File.GetModifiedTime(RevFileName,&RevTime);
	}

	if (File.GetModifiedTime(AppName,&AppTime) == false)
	{
		char temp[100];
#ifdef _DEBUG
		strcpy(temp,"debug\\");
#else
		strcpy(temp,"release\\");
#endif
		strcat(temp,AppName);
		if (File.GetModifiedTime(temp,&AppTime) == false)
		{
			MessageBox("Exe can't find. Revision won't be create.");
			return false;	
		}
	}
	
	if (AppTime > RevTime) 
		return true; 
	else 
		return false;
}

BOOL CRevDialog::SaveRevText()
{
	
	if (strlen(RevFileName) == 0) return false;
	CString Str;
	FILE *f;
	CTime Time = CTime::GetCurrentTime();
	
	Str = Time.Format( "\n\n%d/%m/%Y-%H:%M ");
	
	unsigned long build = RevParam->GetParam("Build");
	build++;
	RevParam->SetParam("Build",build);
	RevParam->SaveParameters();
	char temp[30];
	if (strlen(m_revtext) != 0)
	{
		sprintf(temp,"Build%5d :",build);

		Str = Str + temp + m_revtext;
	}
	else
		Str = " ";

	f = fopen(RevFileName,"a+");
	if (f== NULL)
	{
		MessageBox("Unable to Load Revision file.");
		return false;
	}
	fprintf(f,"%s",Str);
	fclose(f);
	return true;
}

bool CRevDialog::Run()
{
	if (IsModified())
	{
		DoModal();
		SaveRevText();
	}
	return true;
}
