// ColorPage.h: interface for the CColorPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORPAGE_H__9A8F368A_656B_45EA_8E06_DC1679A1797A__INCLUDED_)
#define AFX_COLORPAGE_H__9A8F368A_656B_45EA_8E06_DC1679A1797A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\COMMON\MiracleObj.h"
#include "..\common\surface.h"
#include "Profile.h"
#include "..\common\param.h"	
class CColorPage : public CMiracleObj  
{
public:
	void DrawColorBoxes(CDC *dc,int pageno);
	void DrawBarCode(CDC *dc,int x,int y,int pageno);
	void DrawBox(CDC *dc,int x,int y,COLORREF color);
	int BoxHeightPix;
	int SpacePix;
	int BoxWidthPix;
	void CreateHeader(CDC *dc,short pageno);
	int ColorCount;
	int PageCount;
	BOOL CreatePage(short pageno);
	void SetParam(CParam *param);
	void SetProfile(CProfile *profile);
	void CreateParameters();
	CColorPage();
	virtual ~CColorPage();

	CSurface Surface;
	CProfile *Profile;
	CParam *Param;
	int PageWidthPix;
	int PageHeightPix;


};

#endif // !defined(AFX_COLORPAGE_H__9A8F368A_656B_45EA_8E06_DC1679A1797A__INCLUDED_)
