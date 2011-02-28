// HSB.h: interface for the CHSB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSB_H__71F6A657_2A14_11D5_A752_0000214446E3__INCLUDED_)
#define AFX_HSB_H__71F6A657_2A14_11D5_A752_0000214446E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MBASEDLL_EXP CHSB  
{
public:
	void GetHSBVal(short *hue1,byte *sat1,byte *bri1,byte red,byte green,byte blue);
	short GetHUEVal(byte red,byte green,byte blue);
	byte GetSATVal(byte red,byte green,byte blue);
	byte GetBRIVal(byte red,byte green,byte blue);
	CHSB();
	virtual ~CHSB();
	byte GetRVal(unsigned short hue,byte sat,byte brightness);
	byte GetGVal(unsigned short hue,byte sat,byte brightness);
	byte GetBVal(unsigned short hue,byte sat,byte brightness);

	void CreateHueTables();
	// Hue Table deðiþkenleri
public:
	byte HueBlue[360];
	byte HueGreen[360];
	byte HueRed[360];


};

#endif // !defined(AFX_HSB_H__71F6A657_2A14_11D5_A752_0000214446E3__INCLUDED_)
