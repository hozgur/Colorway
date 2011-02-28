// Print.cpp: implementation of the CPrint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Print.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL gbAbort;
HWND ghwndAbort;

class CPrintingDialog : public CDialog
{
public:
	//{{AFX_DATA(CPrintingDialog)
	enum { IDD = AFX_IDD_PRINTDLG };
	//}}AFX_DATA
	
	CPrintingDialog::CPrintingDialog(CWnd* pParent)
		{
			Create(CPrintingDialog::IDD, pParent);      // modeless !
			gbAbort = FALSE;
		}
	virtual ~CPrintingDialog() { }

	virtual BOOL OnInitDialog();
	virtual void OnCancel();
};

BOOL CALLBACK AbortProc (HDC hdc, int error)
{
  MSG msg;

  while (!gbAbort && PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))

    if (!ghwndAbort || !IsDialogMessage (ghwndAbort, &msg))
    {
      TranslateMessage (&msg);
      DispatchMessage (&msg);
    }

  return !gbAbort;
}

BOOL CPrintingDialog::OnInitDialog()
{
	SetWindowText(AfxGetAppName());
	CenterWindow();
	return CDialog::OnInitDialog();
}

void CPrintingDialog::OnCancel()
{
	gbAbort = TRUE;  // flag that user aborted print
	CDialog::OnCancel();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrint::CPrint()
{
	gbAbort = FALSE;
	ghwndAbort = NULL;
	ClientWmm = -1;
	ClientHmm = -1;
	PrinterDPIX = 1;
	PrinterDPIY = 1;
	CurrentPage = 0;
	PageCount = 0;
	ParentWnd = NULL;
	OffXpix = OffYpix = 0;
	ScaleX = 1;
	ScaleY = 1;
	FontScale = 1;
}

CPrint::~CPrint()
{

}
void CPrint::ConvertToMM(CRect *rect)
{

}

void CPrint::ConvertToPix(CRect *rect)
{
	rect->left = OffXpix + (long)(ScaleX *rect->left * PrinterDPIX/254);
	rect->top = OffYpix + (long)(ScaleY *rect->top * PrinterDPIY/254);
	rect->right = OffXpix + (long)(ScaleX *rect->right * PrinterDPIX/254);
	rect->bottom = OffYpix + (long)(ScaleY *rect->bottom * PrinterDPIY/254);
}

// Bu fonksiyon MFC->Source->  Viewprnt.cpp'den alýnmýþtýr. CView'e ait.

BOOL CPrint::DoPreparePrinting(CPrintInfo* pInfo)
{
	ASSERT(pInfo != NULL);
	ASSERT(pInfo->m_pPD != NULL);

	if (pInfo->m_pPD->m_pd.nMinPage > pInfo->m_pPD->m_pd.nMaxPage)
		pInfo->m_pPD->m_pd.nMaxPage = pInfo->m_pPD->m_pd.nMinPage;

	// don't prompt the user if we're doing print preview, printing directly,
	// or printing via IPrint and have been instructed not to ask

	CWinApp* pApp = AfxGetApp();
	if (pInfo->m_bPreview || pInfo->m_bDirect ||
		(pInfo->m_bDocObject && !(pInfo->m_dwFlags & PRINTFLAG_PROMPTUSER)))
	{
		if (pInfo->m_pPD->m_pd.hDC == NULL)
		{
			// if no printer set then, get default printer DC and create DC without calling
			//   print dialog.
			if (!pApp->GetPrinterDeviceDefaults(&pInfo->m_pPD->m_pd))
			{
				// bring up dialog to alert the user they need to install a printer.
				if (!pInfo->m_bDocObject || (pInfo->m_dwFlags & PRINTFLAG_MAYBOTHERUSER))
					if (pApp->DoPrintDialog(pInfo->m_pPD) != IDOK)
						return FALSE;
			}

			if (pInfo->m_pPD->m_pd.hDC == NULL)
			{
				// call CreatePrinterDC if DC was not created by above
				if (pInfo->m_pPD->CreatePrinterDC() == NULL)
					return FALSE;
			}
		}

		// set up From and To page range from Min and Max
		pInfo->m_pPD->m_pd.nFromPage = (WORD)pInfo->GetMinPage();
		pInfo->m_pPD->m_pd.nToPage = (WORD)pInfo->GetMaxPage();
	}
	else
	{
		// otherwise, bring up the print dialog and allow user to change things
		// preset From-To range same as Min-Max range
		pInfo->m_pPD->m_pd.nFromPage = (WORD)pInfo->GetMinPage();
		pInfo->m_pPD->m_pd.nToPage = (WORD)pInfo->GetMaxPage();

		if (pApp->DoPrintDialog(pInfo->m_pPD) != IDOK)
			return FALSE;       // do not print
	}

	ASSERT(pInfo->m_pPD != NULL);
	ASSERT(pInfo->m_pPD->m_pd.hDC != NULL);
	if (pInfo->m_pPD->m_pd.hDC == NULL)
		return FALSE;

	pInfo->m_nNumPreviewPages = pApp->m_nNumPreviewPages;
	VERIFY(pInfo->m_strPageDesc.LoadString(AFX_IDS_PREVIEWPAGEDESC));
	return TRUE;
}

BOOL CPrint::GetPrinterValues()
{
		// get default print info
	CPrintInfo printInfo;
	ASSERT(printInfo.m_pPD != NULL);    // must be set

	printInfo.m_bPreview = true;
	if (OnPreparePrinting(&printInfo))
	{
		// hDC must be set (did you remember to call DoPreparePrinting?)
		ASSERT(printInfo.m_pPD->m_pd.hDC != NULL);
		// setup the printing DC
		CDC dcPrint;
		dcPrint.Attach(printInfo.m_pPD->m_pd.hDC);  // attach printer dc
		dcPrint.m_bPrinting = TRUE;		
		OnBeginPrinting(&dcPrint, &printInfo);
	}
	return true;
}
void CPrint::OnPrint()
{
	if (ParentWnd == NULL) return;
	// get default print info
	CPrintInfo printInfo;
	ASSERT(printInfo.m_pPD != NULL);    // must be set
/*
	if (LOWORD(GetCurrentMessage()->wParam) == ID_FILE_PRINT_DIRECT)
	{
		CCommandLineInfo* pCmdInfo = AfxGetApp()->m_pCmdInfo;

		if (pCmdInfo != NULL)
		{
			if (pCmdInfo->m_nShellCommand == CCommandLineInfo::FilePrintTo)
			{
				printInfo.m_pPD->m_pd.hDC = ::CreateDC(pCmdInfo->m_strDriverName,
					pCmdInfo->m_strPrinterName, pCmdInfo->m_strPortName, NULL);
				if (printInfo.m_pPD->m_pd.hDC == NULL)
				{
					AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
					return;
				}
			}
		}

		printInfo.m_bDirect = TRUE;
	}
*/
	if (OnPreparePrinting(&printInfo))
	{
		// hDC must be set (did you remember to call DoPreparePrinting?)
		ASSERT(printInfo.m_pPD->m_pd.hDC != NULL);

		// gather file to print to if print-to-file selected
		CString strOutput;
		if (printInfo.m_pPD->m_pd.Flags & PD_PRINTTOFILE && !printInfo.m_bDocObject)
		{
			// construct CFileDialog for browsing
			CString strDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULTEXT));
			CString strPrintDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULT));
			CString strFilter(MAKEINTRESOURCE(AFX_IDS_PRINTFILTER));
			CString strCaption(MAKEINTRESOURCE(AFX_IDS_PRINTCAPTION));
			CFileDialog dlg(FALSE, strDef, strPrintDef,
				OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);
			dlg.m_ofn.lpstrTitle = strCaption;

			if (dlg.DoModal() != IDOK)
				return;

			// set output device to resulting path name
			strOutput = dlg.GetPathName();
		}

		// set up document info and start the document printing process
		DOCINFO docInfo;
		memset(&docInfo, 0, sizeof(DOCINFO));
		docInfo.cbSize = sizeof(DOCINFO);
		docInfo.lpszDocName = PrintTitle;
		CString strPortName;
		int nFormatID;
		if (strOutput.IsEmpty())
		{
			docInfo.lpszOutput = NULL;
			strPortName = printInfo.m_pPD->GetPortName();
			nFormatID = AFX_IDS_PRINTONPORT;
		}
		else
		{
			docInfo.lpszOutput = strOutput;

			strPortName = "Miracle";
			/*AfxGetFileTitle(strOutput,
				strPortName.GetBuffer(_MAX_PATH), _MAX_PATH);*/
			nFormatID = AFX_IDS_PRINTTOFILE;
		}
		
		// setup the printing DC
		CDC dcPrint;
		if (!printInfo.m_bDocObject)
		{
			dcPrint.Attach(printInfo.m_pPD->m_pd.hDC);  // attach printer dc
			dcPrint.m_bPrinting = TRUE;
		}
		OnBeginPrinting(&dcPrint, &printInfo);

		if (!printInfo.m_bDocObject)
			dcPrint.SetAbortProc(AbortProc);

		// disable main window while printing & init printing status dialog
		AfxGetMainWnd()->EnableWindow(FALSE);
		CPrintingDialog dlgPrintStatus(ParentWnd);
		ghwndAbort = dlgPrintStatus.m_hWnd;
		CString strTemp;
		dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_DOCNAME, PrintTitle);
		dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PRINTERNAME,
			printInfo.m_pPD->GetDeviceName());
		AfxFormatString1(strTemp, nFormatID, strPortName);
		dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PORTNAME, strTemp);
		dlgPrintStatus.ShowWindow(SW_SHOW);
		dlgPrintStatus.UpdateWindow();

		// start document printing process
		if (!printInfo.m_bDocObject && dcPrint.StartDoc(&docInfo) == SP_ERROR)
		{
			// enable main window before proceeding
			AfxGetMainWnd()->EnableWindow(TRUE);

			// cleanup and show error message
			OnEndPrinting(&dcPrint, &printInfo);
			dlgPrintStatus.DestroyWindow();
			dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
			AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
			return;
		}

		// Guarantee values are in the valid range
		UINT nEndPage = printInfo.GetToPage();
		UINT nStartPage = printInfo.GetFromPage();

		if (nEndPage < printInfo.GetMinPage())
			nEndPage = printInfo.GetMinPage();
		if (nEndPage > printInfo.GetMaxPage())
			nEndPage = printInfo.GetMaxPage();

		if (nStartPage < printInfo.GetMinPage())
			nStartPage = printInfo.GetMinPage();
		if (nStartPage > printInfo.GetMaxPage())
			nStartPage = printInfo.GetMaxPage();

		int nStep = (nEndPage >= nStartPage) ? 1 : -1;
		nEndPage = (nEndPage == 0xffff) ? 0xffff : nEndPage + nStep;

		VERIFY(strTemp.LoadString(AFX_IDS_PRINTPAGENUM));

		// If it's a doc object, we don't loop page-by-page
		// because doc objects don't support that kind of levity.

		BOOL bError = FALSE;
		
		// Print Loop // Burada DocObject kýsmý silindi
		{
			// begin page printing loop
			for (printInfo.m_nCurPage = nStartPage;
				printInfo.m_nCurPage != nEndPage; printInfo.m_nCurPage += nStep)
			{
				OnPrepareDC(&dcPrint, &printInfo);

				// check for end of print
				if (!printInfo.m_bContinuePrinting)
					break;

				// write current page
				TCHAR szBuf[80];
				wsprintf(szBuf, strTemp, printInfo.m_nCurPage);
				dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PAGENUM, szBuf);

				// set up drawing rect to entire page (in logical coordinates)
				printInfo.m_rectDraw.SetRect(0, 0,
					dcPrint.GetDeviceCaps(HORZRES),
					dcPrint.GetDeviceCaps(VERTRES));
				dcPrint.DPtoLP(&printInfo.m_rectDraw);

				// attempt to start the current page
				if (dcPrint.StartPage() < 0)
				{
					DebugMessage("Error on print 2");
					bError = TRUE;
					break;
				}

				// must call OnPrepareDC on newer versions of Windows because
				// StartPage now resets the device attributes.
				//if (afxData.bMarked4)
					OnPrepareDC(&dcPrint, &printInfo);

				ASSERT(printInfo.m_bContinuePrinting);

				// page successfully started, so now render the page
				OnPrint(&dcPrint, &printInfo);
				TRACE("on print\n");
				int stat = dcPrint.EndPage();
				if (stat < 0 || !AbortProc(dcPrint.m_hDC, 0))
				{
					bError = TRUE;
					CString str;
					str.Format("End Page = %ld,Abort = %ld",stat,gbAbort);
					DebugMessage(str);
					DebugMessage("Error on print");
					break;
				}
			}
		}

		// cleanup document printing process
		if (!printInfo.m_bDocObject)
		{
			if (!bError)
				dcPrint.EndDoc();
			else
				dcPrint.AbortDoc();
		}

		AfxGetMainWnd()->EnableWindow();    // enable main window

		OnEndPrinting(&dcPrint, &printInfo);    // clean up after printing
		dlgPrintStatus.DestroyWindow();

		dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
	}
}


/////////////////////////////////////////////////////////////////////////////
// Printing support virtual functions (others in viewpr.cpp)

void CPrint::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(pDC);
	UNUSED(pDC); // unused in release builds
	
	if (pInfo != NULL)
		pInfo->m_bContinuePrinting =
			(pInfo->GetMaxPage() != 0xffff || (pInfo->m_nCurPage == 1));
}

BOOL CPrint::OnPreparePrinting(CPrintInfo* pInfo)
{	
	pInfo->SetMaxPage(PageCount);
	CurrentPage = 0;
	return DoPreparePrinting(pInfo);
}

void CPrint::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(pDC);
	UNUSED(pDC);     // unused in release builds
	
	//if (pInfo->m_bPreview)
	{
		// m_bPreview baský öncesi Preview penceresi açýlýrken GetPrinterValues
		// fonksiyonu tarafýndan set edilir.
		// Bu tanýmlamalar sadece ilk baþta yapýlmalýdýr.
		LPDEVMODE DevMode = (LPDEVMODE) ::GlobalLock(pInfo->m_pPD->m_pd.hDevMode);
		ASSERT (DevMode != NULL);
		PrinterDPIX = DevMode->dmPrintQuality;
		PrinterDPIY = DevMode->dmYResolution;
		if (PrinterDPIX < 0) PrinterDPIX = PrinterDPIY;//1.52
		if((PrinterDPIX == 0) || (PrinterDPIY == 0))	// 1.53
		{
			PrinterDPIX = pDC->GetDeviceCaps(LOGPIXELSX);
			PrinterDPIY = pDC->GetDeviceCaps(LOGPIXELSY);
		}
		if((PrinterDPIX == 0) || (PrinterDPIY == 0))
		{
			logError("Problem on Printer Driver");
			return;
		}
		ClientWmm = pDC->GetDeviceCaps(HORZRES)* 254 /PrinterDPIX;
		ClientHmm = pDC->GetDeviceCaps(VERTRES)* 254 /PrinterDPIY;
		ClientRectmm.left = 0;
		ClientRectmm.top = 0;
		ClientRectmm.right = ClientWmm;
		ClientRectmm.bottom = ClientHmm;
		::GlobalUnlock(pInfo->m_pPD->m_pd.hDevMode);	
	}	
}

void CPrint::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(pDC);
	UNUSED(pDC);     // unused in release builds	
	CurrentPage = 0;
}


void CPrint::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(pDC);
	// Override and set printing variables based on page number
	CurrentPage = pInfo->m_nCurPage -1 ;
	OnDraw(pDC);                    // Call Draw
}

void CPrint::OnDraw(CDC* pDC)
{
	
	if(!pDC->IsPrinting())
	{				
	
	}
	else
	{
	
	}	
			
}

void CPrint::SetPrintTitle(LPCTSTR title)
{
	PrintTitle = title;
	if (PrintTitle.GetLength() > 31)
		PrintTitle.ReleaseBuffer(31);
}

void CPrint::SetParentWnd(CWnd *wnd)
{
	ParentWnd = wnd;
}

#define OFFX 10
#define OFFYUP 50
#define OFFYDOWN 10


short CPrint::GetPageCount()
{
	return PageCount;
}
