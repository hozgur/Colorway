// Print.h: interface for the CPrint class.
//
//**Class:CPint
//**Category:Printing
//**CreationDate: 22/08/2002
//**LastModificationDate: 22/08/2002
//**Revision History:
//**MFCRequired: Evet
//**Description: Programa baský desteði saðlar.
//**Kullaným Þekli:
//**Ýki þekilde kullanýlabilir.
//**1) Kullanýlacak class bu Class'dan türetilerek.
//**2) Member olarak
//**Member olarak kullanýlmasý için öncelikle bir CPrint'den yeni bir class türetilmesi
//**yani yeni bir CPrint tabanlý bir class tanýmlanmasý gerekir.Bu nedenle
//**yeni bir class tanýmý gerekmeyeceði için birinci yöntem daha kullanýþlýdýr.
//**Nasýl Kullanýlýr:
//**Öncelikle Print.cpp ve print.h projeye eklenir.
//**Ardýndan kullanýlacak class'a
//**class CMyclass : public CPrint
//**þeklinde class'a eklenir. Bu yöntem ayný zamanda class'ýn CMiracleObj
//**özelliklerini de kullanmasýný saðlar. Baskýya geçmeden önce GetPrinterValues fonksiyonu
//**çaðrýlýr. Bu fonksiyon baský öncesi baský alaný hakkýnda bilgileri öðrenmemizi saðlar
//**Bu bilgiler: CRect ClientRectmm : 0.1 mm hassiyetinde baský alanýný verir.
//**ClientWmm,ClientHmm : Baský ebatlarý (0.1 mm)
//**PrinterDPIX,PrinterDPIY bilgileri bu fonksiyon çaðrýldýktan sonra okunabilir.
//**Bu bilgiler öðrenildikten sonra PageCount deðiþkeni set edilerek kaç sayfa
//**basýlacaðý bildirilmelidir.
//**Artýk baský için OnPrint() fonksiyonu çaðrýlýr. OnPrint() fonksiyonu
//**Baský penceresini ekrana getirir,Burada Printer ayarlarý yapýldýktan sonra
//**OnPrint fonksiyonu her Sayfa için OnDraw(pDC) fonksiyonunu çaðýrýr.
//**Hangi sayfanýn basýlacaðý 0 endeksli CurrentPage deðiþkeni ile okunur.
//**Not:OnPrint foksiyonu Baský Penceresi çaðýrdýktan sonra printer ayarlarý deðiþtirilirse
//**baþta GetPrinterValues() ile okunmuþ olan deðerler deðiþmiþ olacaktýr.
//**yeni deðerlere göre baský yapmak için yapýlacak tüm hesaplamalarýn virtual OnDraw()
//**fonksiyonu içersinde yapýlmasý daha uygun olur.
//**mm cinsinden alýnmýþ olan ölçüler ConvertToPix(CRect*) fonksiyonu ile piksele dönüþtürülür.
//**Bu fonksiyon Scale ve Offset deðiþkenlerini kullanýr. Normalde bunlar etkisizdir.
//**Scale deðiþkenlerinin kullanýlmasý: (Eski ve iptal edilecek ve Windows Mapping'e geçilecek)
//**Ekrana Preview baský yapmak için ScaleX,ScaleY ve OffXpix,OffYpix deðiþkenleri kullanýlýr.
//**Bu deðiþkenler ekrana göre uyarlandýðýnda aynen print eder gibi ekrana basýlabilir.
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
	short OffXpix;		// Ekrana baskýda kaðýt alanýnýn pencerenin
	short OffYpix;		// istenilen bir alanýna almak için kullanýlýr.
	float FontScale;

};

#endif // !defined(AFX_PRINT_H__DC6FDB92_0EB5_4132_93F3_3519737C39BA__INCLUDED_)
