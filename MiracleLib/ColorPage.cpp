// ColorPage.cpp: implementation of the CColorPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ColorPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColorPage::CColorPage()
{
	Param = NULL;
	Profile = NULL;
	PageCount = 0;
	ColorCount = 0;
	PageWidthPix = 0;
	PageHeightPix = 0;
	
}

CColorPage::~CColorPage()
{

}

void CColorPage::CreateParameters()
{
	Param->CreateParam("PageWidthmm","-","NU",190,0,1000);
	Param->CreateParam("PageHeightmm","-","NU",280,0,1000);
	Param->CreateParam("PageDPC","-","NU",50,0,1000);
	Param->CreateParam("BoxWidthmm","-","NU",10,0,1000);
	Param->CreateParam("BoxHeightmm","-","NU",10,0,1000);
	Param->CreateParam("SpaceWidthmm","-","NU",2,0,1000);
	Param->CreateParam("HeaderLineWidthmm","-","NU",2,0,1000);
	Param->CreateParam("BarCodeWidthmm","-","NU",2,0,1000);
	Param->CreateParam("BarCodeHeightmm","-","NU",4,0,1000);
	Param->CreateParam("ColorsPerPage","-","NU",300,0,1000);
	Param->CreateParam("HueStep","-","NU",10,0,60);
	Param->CreateParam("BriStep","-","NU",10,0,60);
	Param->CreateParam("SatStep","-","NU",10,0,60);
	Param->CreateParam("TopMarginmm","-","NU",0,0,60);
	Param->CreateParam("BottomMarginmm","-","NU",5,0,60);
	Param->CreateParam("LeftMarginmm","-","NU",5,0,60); // header'da kullanýlýr.
	Param->CreateParam("RightMarginmm","-","NU",5,0,60);
	Param->CreateParam("HeaderHeightmm","-","NU",32,0,60);

	int hue_count = 360 / Param->GetParam("HueStep");
	int bri_count = 100/ Param->GetParam("BriStep");
	int sat_count = 100/ Param->GetParam("SatStep");
	ColorCount = hue_count * bri_count * sat_count;
	int colorsperpage = Param->GetParam("ColorsPerPage");
	PageCount = ColorCount / colorsperpage;
	if ( (PageCount * colorsperpage) != ColorCount) PageCount++;
	short dpc = Param->GetParam("PageDPC") ;


	PageWidthPix = Param->GetParam("PageWidthmm") * dpc/10;
	PageHeightPix = Param->GetParam("PageHeightmm") * dpc/10;

	BoxWidthPix = Param->GetParam("BoxWidthmm") * dpc /10;
	BoxHeightPix = Param->GetParam("BoxHeightmm") * dpc /10;
	SpacePix = Param->GetParam("SpaceWidthmm") * dpc /10;
}



void CColorPage::SetProfile(CProfile *profile)
{
	Profile = profile;
}

void CColorPage::SetParam(CParam *param)
{
	Param = param;
}

BOOL CColorPage::CreatePage(short pageno)
{
	CreateParameters();
	if (!Surface.Create(NULL,PageWidthPix,PageHeightPix,24))
	{
		ErrorMessage(IDS_MEMORYERROR);
		return FALSE;
	}
	Surface.Fill(0x00FFFFFF);
	CDC *dc = CDC::FromHandle(Surface.SurfaceDC);
	//dc->Rectangle(0,0,Surface.Width-1,Surface.Height-1);
	CreateHeader(dc,pageno);
	DrawColorBoxes(dc,pageno);
	//Graphics graphics(Surface.SurfaceDC);
	//Pen      pen(Color(255, 0, 0, 255));

	//graphics.DrawLine(&pen, 0, 0, 200, 100);


	return true;
}

void CColorPage::CreateHeader(CDC *dc,short pageno)
{
	
	
	int dpc = Param->GetParam("PageDPC");
	int hlinew = Param->GetParam("HeaderLineWidthmm") * dpc /10;
	

	int topmargin = Param->GetParam("TopMarginmm") * dpc /10;
	int leftmargin = Param->GetParam("LeftMarginmm") * dpc /10;
	int rightmargin = Param->GetParam("RightMarginmm") * dpc /10;
	
	int rightx = PageWidthPix - rightmargin;
	int top = topmargin + hlinew + SpacePix;
	// Üst Çizgi
	::BitBlt(Surface.SurfaceDC,leftmargin,topmargin,rightx-leftmargin,hlinew,NULL,0,0,BLACKNESS);
	CString lineformat;
	CString line;
	int x = leftmargin + SpacePix,a;
	int tw = 200;// textwidth
	int th = 20;
	lineformat.LoadString(IDS_NAMEFORMAT);
	line.Format(lineformat,Profile->Name);
	CRect r(x,top,x+tw,top+th);
	dc->DrawText(line,r,DT_LEFT | DT_SINGLELINE|DT_BOTTOM);
	
	lineformat.LoadString(IDS_PAGENOFORMAT);
	line.Format(lineformat,pageno);
	x = x+tw;
	r.SetRect(x,top,x+tw,top+th);
	dc->DrawText(line,r,DT_LEFT | DT_SINGLELINE|DT_BOTTOM);

	lineformat.LoadString(IDS_MEDIAFORMAT);
	line.Format(lineformat,Profile->Substrate);
	x = leftmargin + SpacePix;
	top = top +th;
	r.SetRect(x,top,x+tw,top+th);
	dc->DrawText(line,r,DT_LEFT | DT_SINGLELINE|DT_BOTTOM);

	lineformat.LoadString(IDS_INKFORMAT);
	line.Format(lineformat,Profile->Ink);
	x = x+tw;
	r.SetRect(x,top,x+tw,top+th);
	dc->DrawText(line,r,DT_LEFT | DT_SINGLELINE|DT_BOTTOM);

	top = top +th*2;
	x = leftmargin;
	for (a=0;a<10;a++)
	{
		dc->FillSolidRect(x,top,BoxWidthPix,10,0);
		DrawBox(dc,x,top+15,RGB(a*25.5,a*25.5,a*25.5));
		x += BoxWidthPix + SpacePix;
	
	}
	DrawBarCode(dc,rightx-310,topmargin+hlinew+SpacePix,pageno);
	dc->SetBkColor(0x00FFFFFF);
	

}

void CColorPage::DrawBox(CDC *dc,int x, int y, COLORREF color)
{
	
	dc->FillSolidRect(x,y,BoxWidthPix,BoxHeightPix,color);
}

void CColorPage::DrawBarCode(CDC *dc, int x, int y, int pageno)
{
	int a;
	char e[8] = {128,64,32,16,8,4,2,1};
	for (a=0;a<16;a++)
	{
		dc->FillSolidRect(x+a*20,y,10,20,0);
		if ((pageno & e[a&7]) != 0)
			dc->FillSolidRect(x+a*20,y+30,10,20,0);
	}
}

void CColorPage::DrawColorBoxes(CDC *dc, int pageno)
{
	int hue,sat,bri;
	// Tüm lokal ölçü deðiþkenleri pixel cinsindendir.
	const int boxesperline = 15;
	const int boxesperrow = 20;
	int dpc = Param->GetParam("PageDPC");
	int hlinew = Param->GetParam("HeaderLineWidthmm") * dpc /10;
	

	int topmargin = Param->GetParam("TopMarginmm") * dpc /10;
	int leftmargin = Param->GetParam("LeftMarginmm") * dpc /10;
	int rightmargin = Param->GetParam("RightMarginmm") * dpc /10;
	int headerheight =Param->GetParam("HeaderHeightmm") * dpc /10;
	int rightx = PageWidthPix - rightmargin;
	int top = topmargin + headerheight;
	// Üst Çizgi
	dc->BitBlt(leftmargin,top,rightx-leftmargin,hlinew/2,NULL,0,0,BLACKNESS);

}
