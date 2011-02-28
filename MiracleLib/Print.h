// Print.h: interface for the CPrint class.
//
//**Class:CPint
//**Category:Printing
//**CreationDate: 22/08/2002
//**LastModificationDate: 22/08/2002
//**Revision History:
//**MFCRequired: Evet
//**Description: Programa bask� deste�i sa�lar.
//**Kullan�m �ekli:
//**�ki �ekilde kullan�labilir.
//**1) Kullan�lacak class bu Class'dan t�retilerek.
//**2) Member olarak
//**Member olarak kullan�lmas� i�in �ncelikle bir CPrint'den yeni bir class t�retilmesi
//**yani yeni bir CPrint tabanl� bir class tan�mlanmas� gerekir.Bu nedenle
//**yeni bir class tan�m� gerekmeyece�i i�in birinci y�ntem daha kullan��l�d�r.
//**Nas�l Kullan�l�r:
//**�ncelikle Print.cpp ve print.h projeye eklenir.
//**Ard�ndan kullan�lacak class'a
//**class CMyclass : public CPrint
//**�eklinde class'a eklenir. Bu y�ntem ayn� zamanda class'�n CMiracleObj
//**�zelliklerini de kullanmas�n� sa�lar. Bask�ya ge�meden �nce GetPrinterValues fonksiyonu
//**�a�r�l�r. Bu fonksiyon bask� �ncesi bask� alan� hakk�nda bilgileri ��renmemizi sa�lar
//**Bu bilgiler: CRect ClientRectmm : 0.1 mm hassiyetinde bask� alan�n� verir.
//**ClientWmm,ClientHmm : Bask� ebatlar� (0.1 mm)
//**PrinterDPIX,PrinterDPIY bilgileri bu fonksiyon �a�r�ld�ktan sonra okunabilir.
//**Bu bilgiler ��renildikten sonra PageCount de�i�keni set edilerek ka� sayfa
//**bas�laca�� bildirilmelidir.
//**Art�k bask� i�in OnPrint() fonksiyonu �a�r�l�r. OnPrint() fonksiyonu
//**Bask� penceresini ekrana getirir,Burada Printer ayarlar� yap�ld�ktan sonra
//**OnPrint fonksiyonu her Sayfa i�in OnDraw(pDC) fonksiyonunu �a��r�r.
//**Hangi sayfan�n bas�laca�� 0 endeksli CurrentPage de�i�keni ile okunur.
//**Not:OnPrint foksiyonu Bask� Penceresi �a��rd�ktan sonra printer ayarlar� de�i�tirilirse
//**ba�ta GetPrinterValues() ile okunmu� olan de�erler de�i�mi� olacakt�r.
//**yeni de�erlere g�re bask� yapmak i�in yap�lacak t�m hesaplamalar�n virtual OnDraw()
//**fonksiyonu i�ersinde yap�lmas� daha uygun olur.
//**mm cinsinden al�nm�� olan �l��ler ConvertToPix(CRect*) fonksiyonu ile piksele d�n��t�r�l�r.
//**Bu fonksiyon Scale ve Offset de�i�kenlerini kullan�r. Normalde bunlar etkisizdir.
//**Scale de�i�kenlerinin kullan�lmas�: (Eski ve iptal edilecek ve Windows Mapping'e ge�ilecek)
//**Ekrana Preview bask� yapmak i�in ScaleX,ScaleY ve OffXpix,OffYpix de�i�kenleri kullan�l�r.
//**Bu de�i�kenler ekrana g�re uyarland���nda aynen print eder gibi ekrana bas�labilir.
//**
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINT_H__DC6FDB92_0EB5_4132_93F3_3519737C39BA__INCLUDED_)
#define AFX_PRINT_H__DC6FDB92_0EB5_4132_93F3_3519737C39BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MiracleObj.h"

class CPrint : public CMiracleObj 
{
public:
	short GetPageCount();
	CWnd* ParentWnd;
	void SetParentWnd(CWnd *wnd);
	void SetPrintTitle(LPCTSTR title);
	CPrint();
	virtual ~CPrint();

	void OnPrint();
	BOOL GetPrinterValues();
	void ConvertToPix(CRect *rect);
	void ConvertToMM(CRect *rect);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual BOOL DoPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);
	CString PrintTitle;
	short CurrentPage;
	short PageCount;
	CRect ClientRectmm;
	short ClientWmm;
	short ClientHmm;
	short PrinterDPIX;
	short PrinterDPIY;
	float ScaleX;
	float ScaleY;
	short OffXpix;		// Ekrana bask�da ka��t alan�n�n pencerenin
	short OffYpix;		// istenilen bir alan�na almak i�in kullan�l�r.
	float FontScale;

};

#endif // !defined(AFX_PRINT_H__DC6FDB92_0EB5_4132_93F3_3519737C39BA__INCLUDED_)
