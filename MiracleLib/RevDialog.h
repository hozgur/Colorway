#if !defined(AFX_REVDIALOG_H__86339905_F6BC_11D4_A2A9_000244136212__INCLUDED_)
#define AFX_REVDIALOG_H__86339905_F6BC_11D4_A2A9_000244136212__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RevDialog.h : header file
//

#include "param.h"
#define IDD_DIALOG2                     131
#define IDC_EDIT1                       1000
#define IDC_BUILD                       1001
/////////////////////////////////////////////////////////////////////////////
// CRevDialog dialog

class CRevDialog : public CDialog
{
// Construction
public:
	bool Run();
	BOOL SaveRevText();
	BOOL IsModified();
	CRevDialog(CWnd* pParent = NULL);   // standard constructor
	char AppName[100];
	char RevFileName[100];
	CParam *RevParam;
	~CRevDialog();
// Dialog Data
	//{{AFX_DATA(CRevDialog)
	enum { IDD = IDD_DIALOG2 };
	CString	m_revtext;
	CString	m_build;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRevDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRevDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REVDIALOG_H__86339905_F6BC_11D4_A2A9_000244136212__INCLUDED_)
