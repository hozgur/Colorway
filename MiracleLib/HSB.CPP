// HSB.cpp: implementation of the CHSB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSB.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHSB::CHSB()
{
	CreateHueTables();
}

CHSB::~CHSB()
{

}

// for HSB
//
// Hue --0----60---120--180--240--300--360
//       I	  I    I    I    I    I    I
//255-   ------    I    I    I   /------
//       I    I\   I    I    I  / I    I
//       I    I \  I    I    I /  I    I
//       I    I  \ I    I    I/   I    I
//Red    I    I   \----------/    I    I
//255-   I   /----------\    I    I    I
//       I  / I    I    I\   I    I    I
//       I /  I    I    I \  I    I    I
//       I/   I    I    I  \ I    I    I
//Grn    /    I    I    I   \-----------
//255-   I    I    I    /---------\    I
//       I    I    I   /I    I    I\   I
//       I    I    I  / I    I    I \  I
//       I    I    I /  I    I    I  \ I
//Blue   -----------/   I    I    I   \I
//
//
//
//

void CHSB::CreateHueTables()
{
	int a;
	for (a=0;a<60;a++)
	{
		HueRed[a] = 255;
		HueGreen[a] =(byte)( a*4.25 +0.5);
		HueBlue[a] = 0;	
	}
	for (a=60;a<120;a++)
	{
		HueRed[a] =(byte)( 255-(a-60)*4.25);
		HueGreen[a] = 255;
		HueBlue[a] = 0;	
	}
	for (a=120;a<180;a++)
	{
		HueRed[a] = 0;
		HueGreen[a] = 255;
		HueBlue[a] =(byte)( (a-120)*4.25 +0.5);	
	}

	for (a=180;a<240;a++)
	{
		HueRed[a] = 0;
		HueGreen[a] = (byte)(255-(a-180)*4.25);
		HueBlue[a] = 255;	
	}
	for (a=240;a<300;a++)
	{
		HueRed[a] = (byte)((a-240)*4.25 +0.5);
		HueGreen[a] = 0;
		HueBlue[a] = 255;	
	}
	for (a=300;a<360;a++)
	{
		HueRed[a] = 255;	
		HueGreen[a] = 0;
		HueBlue[a] = (byte)(255-(a-300)*4.25);
	}

}

#define MAXBRIGHTNESS 255
#define MAXSATURATION 255
#define MAXHUE	255

byte CHSB::GetRVal(unsigned short hue, byte sat, byte brightness)
{
	if(hue >359) return 0;
	return (brightness * HueRed[hue])/MAXBRIGHTNESS + (brightness * (MAXSATURATION - sat) * (MAXHUE - HueRed[hue] ))/MAXSATURATION/MAXBRIGHTNESS ;
	
	
}

byte CHSB::GetGVal(unsigned short hue, byte sat, byte brightness)
{
	if(hue >359) return 0;
	return (brightness * HueGreen[hue])/MAXBRIGHTNESS + (brightness * (MAXSATURATION - sat) * (MAXHUE - HueGreen[hue] ))/MAXSATURATION/MAXBRIGHTNESS ;
}

byte CHSB::GetBVal(unsigned short hue, byte sat, byte brightness)
{
	if(hue >359) return 0;
	return (brightness * HueBlue[hue])/MAXBRIGHTNESS + (brightness * (MAXSATURATION - sat) * (MAXHUE - HueBlue[hue] ))/MAXSATURATION/MAXBRIGHTNESS ;
}

short CHSB::GetHUEVal(byte red, byte green, byte blue)
{
	byte bri,sat,val1,val2;
	if (red>green)
	{
		if (red>blue) { val1 = 1; bri = red;} else {val1 = 3;bri = blue;}	
	}
	else
	{
		if(green>blue){val1 = 2;bri = green;} else {val1 = 3;bri = blue;}		
	}

	if (red<green)
	{
		if (red<blue) { val2 = 1; sat = (red*255)/bri;} else {val2 = 3;sat = (blue*255)/bri;}	
	}
	else
	{
		if(green<blue){val2 = 2;sat = (green*255)/bri;} else {val2 = 3;sat = (blue*255)/bri;}		
	}
	red		= (red  *255)/bri;
	green	= (green*255)/bri;
	blue	= (blue *255)/bri;
	switch(val1+3*val2)
	{
	case 5:	return (short)(120+(60*blue/255));
	case 6: return (short)(180+60*(255-green)/255);
	case 7: return (short)(300+60*(255-blue)/255);
	case 9: return (short)(240+60*red/255);
	case 10:return (short)(60*green/255);
	case 11:return (short)(60+60*(255-red)/255);
	case 12:return 0;
	default:ASSERT(false);
	}
	return 0;
}

void CHSB::GetHSBVal(short *hue1,byte *sat1,byte *bri1,byte red,byte green,byte blue)
{
	byte bri,sat,val1,val2;
	if (red>=green)
	{
		if (red>=blue) { val1 = 1; bri = red;} else {val1 = 3;bri = blue;}	
	}
	else
	{
		if(green>=blue){val1 = 2;bri = green;} else {val1 = 3;bri = blue;}		
	}
	if (bri !=0)
	{
		if (red<=green)
		{
			if (red<=blue) { val2 = 1; sat = 255-((red*255)/bri);} else {val2 = 3;sat = 255-((blue*255)/bri);}	
		}
		else
		{
			if(green<=blue){val2 = 2;sat = 255-((green*255)/bri);} else {val2 = 3;sat = 255-((blue*255)/bri);}		
		}
		red		= red  *255/bri;
		green	= green*255/bri;
		blue	= blue *255/bri;
		if (sat !=0)
		{
			red = (255*(red+sat-255))/sat;
			green = (255*(green+sat-255))/sat;
			blue = (255*(blue+sat-255))/sat;		
		}
		
	}
	else
	{
		*sat1 = 0;
		*bri1 = 0;
		*hue1 = 0;
		return;		
	}
	*sat1=sat;
	*bri1=bri;
	
	switch(val1+3*val2)
	{
	case 5:	*hue1= (short)(120+(59*blue/255));break;
	case 6: *hue1= (short)(180+59*(255-green)/255);break;
	case 7: *hue1= (short)(300+59*(255-blue)/255);break;
	case 9: *hue1= (short)(240+59*red/255);break;
	case 10:*hue1= (short)(59*green/255);break;
	case 11:*hue1= (short)(60+59*(255-red)/255);break;
	case 12:*hue1= 0;break;
	default:*hue1 = 0;
	}
	
}
byte CHSB::GetSATVal(byte red,byte green,byte blue)
{
	byte bri;
	//Maksimum deðeri hesapla
	if (red>green)
	{
		if (red>blue)
			bri= red;
		else
			bri = blue;
	
	}
	else
	{
		if(green>blue)
			bri = green;
		else
			bri= blue;
	}
	// Minimum deðeri hesapla
	if (red<green)
	{
		if (red<blue)
			return red*255/bri;
		else
			return blue*255/bri;
	
	}
	else
	{
		if(green<blue)
			return green*255/bri;
		else
			return blue*255/bri;
	}
}

byte CHSB::GetBRIVal(byte red,byte green,byte blue)
{
	//Maksimum deðeri hesapla
	if (red>green)
	{
		if (red>blue)
			return red;
		else
			return blue;
	
	}
	else
	{
		if(green>blue)
			return green;
		else
			return blue;
	}
}