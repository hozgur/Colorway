#if !defined(AFX_LISTBOXEX_H__C6FF5FC4_696A_406B_9347_7C0080CBA33B__INCLUDED_)
#define AFX_LISTBOXEX_H__C6FF5FC4_696A_406B_9347_7C0080CBA33B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define LBE_MAXCOLUMN 10
#define LBE_MINITEMHEIGHT 4
#define LBE_MAXITEMHEIGHT 100
// ListBoxEx.h : header file
//
class LBEItemData
{
public:
	LBEItemData() {ItemCount = 1;};
	CString Items[LBE_MAXCOLUMN];
	COLORREF TextColors[LBE_MAXCOLUMN];
	COLORREF BackColors[LBE_MAXCOLUMN];
	int ItemCount;
};
#define BACKCOLORMODE_FRAME 0
#define BACKCOLORMODE_CLASSIC 1
/////////////////////////////////////////////////////////////////////////////
// CListBoxEx window


class CListBoxEx : public CListBox
{

// Construction
public:
	CListBoxEx();

// Attributes
public:

// Operations
public:

protected:
	int ColumnCount;
	int ColumnWidths[LBE_MAXCOLUMN];
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListBoxEx)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	int TotalWidth;
	int SelectionBackColorMode;
	BOOL SetSelectBackColorMode(int mode);
	int GetCurrentBackColor(int sub);
	int GetItemBackColor(int itemno,int sub);
	int Height;
	BOOL SetHeight(short height);
	BOOL SetSubLine(int itemno,int subitemno,LPCTSTR val);
	BOOL SetSubLine(int itemno,int subitemno,LPCTSTR val,COLORREF textcolor,COLORREF backcolor);
	unsigned int Alignment;
	void SetTextAlignment(UINT alignment);
	BOOL DeleteAllItems();
	int AddSubLine(int itemno,LPCTSTR val,COLORREF textcolor,COLORREF backcolor);
	int AddSubLine(int itemno,LPCTSTR val);
	int AddLine(LPCTSTR val,COLORREF textcolor,COLORREF backcolor);
	int AddLine(LPCTSTR val);
	BOOL AddColumn(int columnwidth);
	virtual ~CListBoxEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListBoxEx)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTBOXEX_H__C6FF5FC4_696A_406B_9347_7C0080CBA33B__INCLUDED_)
