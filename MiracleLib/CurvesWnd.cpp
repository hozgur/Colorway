// CurvesWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CurvesWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define ID_HELLO WM_USER+10
/////////////////////////////////////////////////////////////////////////////
// CCurvesWnd

CCurvesWnd::CCurvesWnd()
{
	Buttondown = false;
	Arrow = LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(1000));
	ActiveMarker = 0;
	CurveRect.left = 0;
	CurveRect.right = 150;
	CurveRect.top = 0;
	CurveRect.bottom = 150;
	GridCount = 3;
}

CCurvesWnd::~CCurvesWnd()
{
}


BEGIN_MESSAGE_MAP(CCurvesWnd, CStatic)
	//{{AFX_MSG_MAP(CCurvesWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCurvesWnd message handlers

void CCurvesWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC *cdc;
	COLORREF color = GetSysColor(COLOR_3DFACE);
	cdc = cdc->FromHandle(Surface.GetDC());
	cdc->FillSolidRect(&CurveRect,color);
	cdc->Draw3dRect(&CurveRect,0,0);
	DrawGrid(cdc);
	Curve.Draw(cdc,Data);
	
	dc.BitBlt(0,0,SizeX,SizeY,cdc,0,0,SRCCOPY);
}




void CCurvesWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	Buttondown = true;
	ActiveMarker = Curve.IsMarker(point);
	if (ActiveMarker!= 0)
		::SetCursor(Arrow);

	SetCapture();
	
	CStatic::OnLButtonDown(nFlags, point);
}

void CCurvesWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	
	if (Buttondown)
	{
		if (ActiveMarker!= 0)
		{
			::SetCursor(Arrow);
			Curve.MoveMarker(ActiveMarker-1,point.x,point.y);
			InvalidateRect(NULL,false);
			CallBackProc(0,0);
		}

	}
	else
	{
		
		if ( Curve.IsMarker(point) != 0)
		{
			::SetCursor(Arrow);
		}
		else
			::SetCursor(LoadCursor(NULL,IDC_ARROW));
	
	}
	
		
	
	
	CStatic::OnMouseMove(nFlags, point);
}


void CCurvesWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	Buttondown = false;
	ActiveMarker = 0;
	ReleaseCapture();
	CStatic::OnLButtonUp(nFlags, point);
	CallBackProc(0,0);
	

}

void CCurvesWnd::Init(unsigned char *data,short sizex,short sizey,short datasize,UINT l_bmp_handle,UINT b_bmp_handle,short gridcount,CALLBACKPROC callbackproc)
{
	Buttondown = false;
	
	CBitmap LeftBmp,BottomBmp;
	SizeX = sizex;
	SizeY = sizey;
	Data = data;
	DataSize = datasize;
	Curve.DataSize = datasize;
	
	CallBackProc = callbackproc;
	if ((GridCount >1) && (GridCount <21)) GridCount = gridcount;
	BITMAP bmp;
	LeftBmp.LoadBitmap(MAKEINTRESOURCE(l_bmp_handle));
	BottomBmp.LoadBitmap(MAKEINTRESOURCE(b_bmp_handle));
	LeftBmp.GetBitmap(&bmp);
	CurveRect.left = bmp.bmWidth;
	CurveRect.top = 0;
	CurveRect.right = sizex;
	BottomBmp.GetBitmap(&bmp);
	CurveRect.bottom = sizey-bmp.bmHeight;
	Curve.Rect = CurveRect;
	Surface.Create(NULL,sizex,sizey,24);
	CDC *cdc;
	cdc = cdc->FromHandle(Surface.GetDC());
	CDC cdc2;
	cdc2.CreateCompatibleDC(cdc);
	HBITMAP oldbitmap = (HBITMAP) cdc2.SelectObject(LeftBmp);
	cdc->BitBlt(0,0,CurveRect.left,CurveRect.Height(),&cdc2,0,0,SRCCOPY);
	cdc2.SelectObject(BottomBmp);
	cdc->BitBlt(0,CurveRect.bottom,bmp.bmWidth,bmp.bmHeight,&cdc2,0,0,SRCCOPY);
	cdc2.SelectObject(oldbitmap);
	// MarkerType 1 = only x 2 = only y 3 = x and y movable
	Curve.MarkerType[0] = 3;
	Curve.MarkerType[1] = 3;
	Curve.MarkerType[2] = 3;
	Curve.MarkerPos[0].x = 0;
	Curve.MarkerPos[0].y = CurveRect.Height()-1;
	Curve.MarkerPos[1].x = CurveRect.Width()/2;
	Curve.MarkerPos[1].y = CurveRect.Height()/2;
	Curve.MarkerPos[2].x = CurveRect.Width()-1;
	Curve.MarkerPos[2].y = 0;	
	cdc->Draw3dRect(0,0,sizex,sizey,0,0);
	
}

void CCurvesWnd::DrawGrid(CDC *dc)
{
	int gapx = CurveRect.Width()/GridCount;
	int gapy = CurveRect.Height()/GridCount;
	CPen pen;
	pen.CreatePen(PS_SOLID,0,RGB(128,128,128));
	HPEN oldpen = (HPEN) dc->SelectObject(pen);
	for (int a= 0;a<GridCount;a++)
	{
		dc->MoveTo(CurveRect.left,CurveRect.top+a*gapy);
		dc->LineTo(CurveRect.right,CurveRect.top+a*gapy);
		dc->MoveTo(CurveRect.left+a*gapx,CurveRect.top);
		dc->LineTo(CurveRect.left+a*gapx,CurveRect.bottom);
	}
	dc->SelectObject(oldpen);
}

void CCurvesWnd::OnSimReset()
{
	Curve.MarkerPos[0].x = 0;
	Curve.MarkerPos[0].y = CurveRect.Height()-1;
	Curve.MarkerPos[1].x = CurveRect.Width()/2;
	Curve.MarkerPos[1].y = CurveRect.Height()/2;
	Curve.MarkerPos[2].x = CurveRect.Width()-1;
	Curve.MarkerPos[2].y = 0;	
	Curve.SetData(Data);
	InvalidateRect(NULL,false);	
}

void CCurvesWnd::SetData()
{
	Curve.SetData(Data);
}

void CCurvesWnd::LoadValues()
{
	Curve.SetMarkers();
	Curve.SetData(Data);
}
