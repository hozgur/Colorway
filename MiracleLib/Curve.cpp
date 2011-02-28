// Curve.cpp: implementation of the CCurve class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Curve.h"
#include <math.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCurve::CCurve()
{
	MarkerCount = 3;
	Linear = false;
	
	Rect.left = 0;
	Rect.top = 0;
	Rect.right = 150;
	Rect.bottom = 150;
	
}

CCurve::~CCurve()
{

}

void CCurve::DrawMarker(CDC *dc,CPoint p)
{
	dc->MoveTo(Rect.left+p.x-3,Rect.top+p.y-3);
	dc->LineTo(Rect.left+p.x+3,Rect.top+p.y-3);
	dc->LineTo(Rect.left+p.x+3,Rect.top+p.y+3);
	dc->LineTo(Rect.left+p.x-3,Rect.top+p.y+3);
	dc->LineTo(Rect.left+p.x-3,Rect.top+p.y-3);
}

void CCurve::Draw(CDC *dc,unsigned char *data)
{
	if ( data == NULL) return;	
	
	int maxdata = DataSize-1;
	int maxheight = Rect.Height()-1;
	int maxwidth = Rect.Width() -1;	
	if (maxwidth == 0) return;
	if (maxheight == 0) return;
	if (maxdata == 0) return;
	short d;
	SetData(data);
	// Drawing Section
	CRgn rgn;
	rgn.CreateRectRgnIndirect(Rect);
	dc->SelectClipRgn(&rgn);
	// Draw Curve
	for(int x=0;x<Rect.Width();x++)
	{		
		d = 255-data[x*maxdata/maxwidth];
		dc->SetPixel(x+Rect.left,d*maxheight/255,0);		
	}
	// Draw Markers
	for (int x=0;x<MarkerCount;x++)
		DrawMarker(dc,MarkerPos[x]);
	
}

void CCurve::MoveMarker(short index, int x, int y)
{	
	if (x<Rect.left) x = Rect.left;
	if (y<Rect.top) y = Rect.top;
	if (x>Rect.right) x = Rect.right;
	if (y>Rect.bottom) y = Rect.bottom;
	if ((index >=0) && (index <MarkerCount))
	{
		if (MarkerType[index] & 1) MarkerPos[index].x = x-Rect.left;
		if (MarkerType[index] & 2) MarkerPos[index].y = y-Rect.top;
	}	
}

short CCurve::IsMarker(CPoint p)
{
	for(int a=0;a<MarkerCount;a++)
	{
		if ((p.x > MarkerPos[a].x+Rect.left-3) && (p.x < MarkerPos[a].x+Rect.left+3) &&
			(p.y > Rect.top+MarkerPos[a].y-3) && (p.y < Rect.top+MarkerPos[a].y+3))
				return a+1;
	}
	return 0;
}


bool CCurve::CalculateBlendData(byte *data,short datasize,short *values,bool linear)
{
	if ( data == NULL) return false;
	if ((datasize <= 0) || (datasize > 1024)) return false;// 1024 kýsýtlamasýnýn bir önemi yok
	if (values[0] <0) values[0] = 0; if (values[1] <0) values[1] = 0;
	if (values[2] <0) values[2] = 0; if (values[3] <0) values[3] = 0;
	if (values[4] <0) values[4] = 0; if (values[5] <0) values[5] = 0;
	if (values[0] >=datasize) values[0] = datasize-1; if (values[1] >255) values[1] = 255;
	if (values[2] >=datasize) values[2] = datasize-1; if (values[3] >255) values[3] = 255;
	if (values[4] >=datasize) values[4] = datasize-1; if (values[5] >255) values[5] = 255;

	float x1,x2,x3,y1,y2,y3,a,b,c,a1,b1,c1,A;
	int x,y,xx;	
	
	x1 = values[0];x2 = values[2];x3 = values[4];
	y1 = values[1];y2 = values[3];y3 = values[5];
	
	// Calculate Coefficients
	a = (y2-y1)/(x1*x1+x2*x2-2*x1*x2);
	b = -2*a*x1;
	c = y2-a*x2*x2-b*x2;
	A = 2*a*x2+b;
	a1 = A/(2*(x2-x3));
	b1 = -2*a1*x3;
	c1 = y2-a1*x2*x2-b1*x2;
	
	for(x=0;x<x1;x++)
	{	
		data[x] = 255-y1;		
	}
	if (!linear)
	{
		for(x=x1;x<x2;x++)
		{			
			y = a*x*x+b*x+c;
			if (y<0) y = 0;
			if (y>255) y = 255;
			data[x] = 255-y;		
		}
		for(x=x2;x<x3;x++)
		{		
			y = a1*x*x+b1*x+c1;
			if (y<0) y = 0;
			if (y>255) y = 255;
			data[x] = 255-y;		
		}
	}
	else
	{
		for (x=x1;x<x2;x++)
		{
			y = y1 + (y2-y1)/(x2-x1)*(x-x1);
			data[x] = 255-y;		
		}
		for (x=x2;x<x3;x++)
		{
			y = y2 + (y3-y2)/(x3-x2)*(x-x2);
			data[x] = 255-y;		
		}
	}
	for(x=x3;x<datasize;x++)
	{			
		data[x] = 255-y;		
	}
	return true;
}

void CCurve::SetData(byte *data)
{
	int maxdata = DataSize-1;
	int maxheight = Rect.Height()-1;
	int maxwidth = Rect.Width() -1;	
	if (maxwidth == 0) return;
	if (maxheight == 0) return;
	val[0] = MarkerPos[0].x*maxdata/maxwidth;
	val[2] = MarkerPos[1].x*maxdata/maxwidth;
	val[4] = MarkerPos[2].x*maxdata/maxwidth;
	val[1] = MarkerPos[0].y*255/maxheight;
	val[3] = MarkerPos[1].y*255/maxheight;
	val[5] = MarkerPos[2].y*255/maxheight;	
	CalculateBlendData(data,DataSize,val,Linear);
}

void CCurve::SetMarkers()
{	
	int maxdata = DataSize-1;
	int d = maxdata/2;
	int maxheight = Rect.Height()-1;
	int maxwidth = Rect.Width() -1;	
	if (maxdata == 0) return;
	MarkerPos[0].x = (val[0]*maxwidth+d)/maxdata;
	MarkerPos[1].x = (val[2]*maxwidth+d)/maxdata;
	MarkerPos[2].x = (val[4]*maxwidth+d)/maxdata;
	MarkerPos[0].y = (val[1]*maxheight+128)/255;
	MarkerPos[1].y = (val[3]*maxheight+128)/255;
	MarkerPos[2].y = (val[5]*maxheight+128)/255;
		
}
