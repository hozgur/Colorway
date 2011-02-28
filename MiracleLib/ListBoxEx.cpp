// ListBoxEx.cpp : implementation file
//

#include "stdafx.h"
#include "ListBoxEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListBoxEx

CListBoxEx::CListBoxEx()
{
	ColumnCount = 0;
	Alignment = DT_LEFT;
	Height = 8;
	SelectionBackColorMode = BACKCOLORMODE_FRAME;
	TotalWidth = 0;
}

CListBoxEx::~CListBoxEx()
{
}


BEGIN_MESSAGE_MAP(CListBoxEx, CListBox)
	//{{AFX_MSG_MAP(CListBoxEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListBoxEx message handlers

void CListBoxEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (ColumnCount == 0) return;
	if ((int)lpDrawItemStruct->itemID <0) return;
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);	
	CRect rect(lpDrawItemStruct->rcItem);
	LBEItemData *ItemData = (LBEItemData*)GetItemData(lpDrawItemStruct->itemID);
	if (ItemData == NULL) return;
	COLORREF textcolor = 0x00000000;
	if ((lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		if(SelectionBackColorMode == BACKCOLORMODE_FRAME)
			pDC->Draw3dRect(rect,RGB(0,0,128),RGB(0,0,128));
		else
		{
			pDC->FillSolidRect(rect,GetSysColor(COLOR_HIGHLIGHT));
			pDC->SetTextColor(RGB(255,255,255));
		}
	}
	else
	{
		if(SelectionBackColorMode == BACKCOLORMODE_FRAME)
			pDC->Draw3dRect(rect,RGB(255,255,255),RGB(255,255,255));
		else
		{
			pDC->FillSolidRect(rect,GetSysColor(COLOR_WINDOW));
			pDC->SetTextColor(textcolor);
		}
	}
	rect.DeflateRect(1,1,1,1);
	CRect r = rect;
	for(int a =0;a<ItemData->ItemCount;a++)
	{
		if (a<ColumnCount)
		{
			r.right = r.left +ColumnWidths[a];			
			
			if(SelectionBackColorMode == BACKCOLORMODE_FRAME)
			{
				pDC->FillSolidRect(r,ItemData->BackColors[a]);			
				pDC->SetTextColor(ItemData->TextColors[a]);
			}
			pDC->DrawText(ItemData->Items[a],r,Alignment);
			r.left = r.right;
		}
	
	}
	
}

void CListBoxEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = Height;
	
}

BOOL CListBoxEx::AddColumn(int columnwidth)
{
	if (ColumnCount <LBE_MAXCOLUMN)
	{
		ColumnWidths[ColumnCount++] = columnwidth;
		TotalWidth +=columnwidth;
		//SetHorizontalExtent(TotalWidth);
		return TRUE;
	}

	return FALSE;
}

int CListBoxEx::AddLine(LPCTSTR val, COLORREF textcolor,COLORREF backcolor)
{
	LBEItemData *ItemData = new LBEItemData;

	if (ItemData == NULL) return -1;
	ItemData->Items[0] = val;
	ItemData->TextColors[0] = textcolor &0x00FFFFFF;
	ItemData->BackColors[0] = backcolor &0x00FFFFFF;
	int index = AddString((LPSTR) ItemData);
	if (index == LB_ERR)
	{
		delete ItemData;
		return -1;
	}
	return index;
}

int CListBoxEx::AddLine(LPCTSTR val)
{
	LBEItemData *ItemData = new LBEItemData;

	if (ItemData == NULL) return -1;
	ItemData->Items[0] = val;
	ItemData->TextColors[0] = GetSysColor(COLOR_WINDOWTEXT);
	ItemData->BackColors[0] = GetSysColor(COLOR_WINDOW);
	int index = AddString((LPSTR) ItemData);
	if (index == LB_ERR)
	{
		delete ItemData;
		return -1;
	}
	return index;
}
int CListBoxEx::AddSubLine(int itemno, LPCTSTR val,COLORREF textcolor,COLORREF backcolor)
{
	LBEItemData *ItemData = (LBEItemData*)GetItemData(itemno);
	if (ItemData == NULL) return -1;
	int a = ItemData->ItemCount;
	if (a == ColumnCount) return -1;
	ItemData->Items[a] = val;
	ItemData->TextColors[a] = textcolor&0x00FFFFFF;
	ItemData->BackColors[a] = backcolor&0x00FFFFFF;
	ItemData->ItemCount++;
	return a;
}

int CListBoxEx::AddSubLine(int itemno, LPCTSTR val)
{
	LBEItemData *ItemData = (LBEItemData*)GetItemData(itemno);
	if (ItemData == NULL) return -1;
	int a = ItemData->ItemCount;
	if (a == ColumnCount) return -1;
	ItemData->Items[a] = val;
	ItemData->TextColors[a] = GetSysColor(COLOR_WINDOWTEXT);
	ItemData->BackColors[a] = GetSysColor(COLOR_WINDOW);
	ItemData->ItemCount++;
	return a;
}
BOOL CListBoxEx::DeleteAllItems()
{
	ResetContent();
	return TRUE;
}

void CListBoxEx::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
	LBEItemData *ItemData = (LBEItemData*) lpDeleteItemStruct->itemData;
		if (ItemData != NULL) delete ItemData;	
	CListBox::DeleteItem(lpDeleteItemStruct);
}

void CListBoxEx::SetTextAlignment(UINT alignment)
{
	Alignment = alignment;
}

BOOL CListBoxEx::SetSubLine(int itemno, int subitemno, LPCTSTR val)
{
	if ((itemno <0) || (itemno >=GetCount())) return FALSE;
	LBEItemData *ItemData = (LBEItemData*)GetItemData(itemno);
	if (ItemData == NULL) return FALSE;
	int a = ItemData->ItemCount;
	if (subitemno >= ColumnCount) return FALSE;
	if (subitemno >= a) ItemData->ItemCount = subitemno+1;
	ItemData->Items[subitemno] = val;
	ItemData->TextColors[subitemno] = GetSysColor(COLOR_WINDOWTEXT);
	ItemData->BackColors[subitemno] = GetSysColor(COLOR_WINDOW);
	InvalidateRect(NULL,false);
	return TRUE;
	
}
BOOL CListBoxEx::SetSubLine(int itemno, int subitemno, LPCTSTR val, COLORREF textcolor, COLORREF backcolor)
{
	if ((itemno <0) || (itemno >=GetCount())) return FALSE;
	LBEItemData *ItemData = (LBEItemData*)GetItemData(itemno);
	if (ItemData == NULL) return FALSE;
	int a = ItemData->ItemCount;
	if (subitemno >= ColumnCount) return FALSE;
	if (subitemno >= a) ItemData->ItemCount = subitemno+1;
	ItemData->Items[subitemno] = val;
	ItemData->TextColors[subitemno] = textcolor;
	ItemData->BackColors[subitemno] = backcolor;
	InvalidateRect(NULL,false);
	return TRUE;
	
}

BOOL CListBoxEx::SetHeight(short height)
{
	if ((height>=LBE_MINITEMHEIGHT) && (height<LBE_MAXITEMHEIGHT))
	{
		Height = height;
		return TRUE;
	}
	return FALSE;
}

int CListBoxEx::GetCurrentBackColor(int sub)
{
	int sel = GetCurSel();	
	if ((sel <0) || (sel >=GetCount())) return -1;
	LBEItemData *ItemData = (LBEItemData*)GetItemData(sel);
	if (ItemData == NULL) return -1;
	int a = ItemData->ItemCount;
	if (sub >= ColumnCount) return -1;
	if (sub >= a) return -1;	
	return ItemData->BackColors[sub];
}

int CListBoxEx::GetItemBackColor(int itemno,int sub)
{
	
	
	if ((itemno <0) || (itemno >=GetCount())) return -1;
	LBEItemData *ItemData = (LBEItemData*)GetItemData(itemno);
	if (ItemData == NULL) return -1;
	int a = ItemData->ItemCount;
	if (sub >= ColumnCount) return -1;
	if (sub >= a) return -1;	
	return ItemData->BackColors[sub];
}

int CListBoxEx::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	if ((lpCompareItemStruct->itemID1 < 0) || (lpCompareItemStruct->itemID2 < 0)) return 0;
	
	LBEItemData *ItemData1 = (LBEItemData*) lpCompareItemStruct->itemData1;
	LBEItemData *ItemData2 = (LBEItemData*) lpCompareItemStruct->itemData2;
	// Burasý tamamlanmadý çünkü içindeki veri tipine göre deðiþkendir.

	// TODO: Add your code to determine the sorting order of the specified items
	// return -1 = item 1 sorts before item 2
	// return 0 = item 1 and item 2 sort the same
	// return 1 = item 1 sorts after item 2
	
	return 0;
}

BOOL CListBoxEx::SetSelectBackColorMode(int mode)
{
	SelectionBackColorMode = mode;
	return TRUE;
}
