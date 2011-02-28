/***************************************************************************
	CDockBarControl: Docking able window control class
	© 2003 Erdinc AKAN, AIT Ltd.
	
	File Name		: DockBarControl.cpp
	Version			: 1.0
	Release Date	: 10/10/2003
	Contact			: erakan_AT_softhome.net
	Mobile			: +90535 932 SAKARYA SAKARYA


 ***************************************************************************/

#include "stdafx.h"
#include "DockBarControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Handlers.
static LPCTSTR CCls();
IMPLEMENT_DYNAMIC(CDockBarControl, CControlBar);

BEGIN_MESSAGE_MAP(CDockBarControl, CControlBar)
	ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_NCPAINT()
    ON_WM_NCCALCSIZE()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_CAPTURECHANGED()
    ON_WM_SETTINGCHANGE()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_WM_NCMOUSEMOVE()
    ON_WM_NCHITTEST()
    ON_WM_CLOSE()
    ON_WM_SIZE()
END_MESSAGE_MAP()


// Create funtions.
BOOL CDockBarControl::Create(LPCTSTR wndname, CWnd *powner, UINT nid, DWORD style)
{
	InitDefaultValues();
	ASSERT_VALID(powner); //must have an owner!
	//Can not be fixed and diynamic!
	ASSERT(!((style & CBRS_SIZE_FIXED) && (style & CBRS_SIZE_DYNAMIC)));

	m_dwStyle = style & CBRS_ALL;

	style &= ~CBRS_ALL;
	style |= WS_CLIPCHILDREN;
    if(!CWnd::Create(CCls(), wndname, style,
        CRect(0, 0, 0, 0), powner, nid))
        return FALSE;
	return TRUE;
}

int CDockBarControl::OnCreate(LPCREATESTRUCT cs)
{
	if(CControlBar::OnCreate(cs) == -1)
		return -1;

	bDragShowContent = FALSE;
    ::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0,
        &bDragShowContent, 0);

	return 0;
}

BOOL CDockBarControl::IsFloating() const
{
	return (!IsHorzDocked() && !IsVertDocked());
}

BOOL CDockBarControl::IsHorzDocked() const
{
	return (DockBarID == AFX_IDW_DOCKBAR_TOP || DockBarID == AFX_IDW_DOCKBAR_BOTTOM);
}


BOOL CDockBarControl::IsVertDocked() const
{
	return (DockBarID == AFX_IDW_DOCKBAR_LEFT || DockBarID == AFX_IDW_DOCKBAR_RIGHT);
}

BOOL CDockBarControl::IsSideTracking() const
{
	ASSERT(bTracking && !IsFloating());
	return (htEdge == HTLEFT || htEdge == HTRIGHT)?IsHorzDocked() : IsVertDocked();
}

CSize CDockBarControl::CalcFixedLayout(BOOL bstretch, BOOL bhorz)
{
    if(bstretch) // if the bar is stretched (is not the child of a dockbar)
        if(bhorz)
            return CSize(32767, szHorz.cy);
        else
            return CSize(szVert.cx, 32767);

	// cheater casting to get DockBars protected members.
    CCastDockBar* pdockbar = (CCastDockBar*) m_pDockBar;

    CDBCArray arrbars;
    GetRowSizingBars(arrbars);
    AFX_SIZEPARENTPARAMS layout;
    layout.hDWP = pdockbar->m_bLayoutQuery ?
        NULL : ::BeginDeferWindowPos(arrbars.GetSize());
    for(int i = 0; i < arrbars.GetSize(); i++)
        if(arrbars[i]->m_nStateFlags & (delayHide|delayShow))
            arrbars[i]->RecalcDelayShow(&layout);
    if(layout.hDWP != NULL)
        ::EndDeferWindowPos(layout.hDWP);

   // get available length
    CRect rc = pdockbar->m_rectLayout;
    if(rc.IsRectEmpty())
        m_pDockSite->GetClientRect(&rc);
    int nlengthtotal = bhorz ? rc.Width() + 2 : rc.Height() - 2;

    if(IsVisible() && !IsFloating() &&
			bParentSizing && arrbars[0] == this)
        if(NegotiateSpace(nlengthtotal, (bhorz != FALSE)))
            AlignControlBars();

    bParentSizing = FALSE;

    if(bhorz)
        return CSize(max(MinHorz.cx, szHorz.cx),
                     max(MinHorz.cy, szHorz.cy));

    return CSize(max(MinVert.cx, szVert.cx),
                 max(MinVert.cy, szVert.cy));
}

void CDockBarControl::GetRowSizingBars(CDBCArray &arrbars)
{
	int nthis; // dummy
    GetRowSizingBars(arrbars, nthis);
}

void CDockBarControl::GetRowSizingBars(CDBCArray &arrbars, int &nthis)
{
    arrbars.RemoveAll();

    int nfirstt, nlastt, nthist;
    GetRowInfo(nfirstt, nlastt, nthist);

    nthis = -1;
    for(int i = nfirstt; i <= nlastt; i++)
    {
        CDockBarControl * pbar =
            (CDockBarControl*) m_pDockBar->m_arrBars[i];
        if(HIWORD(pbar) == 0) continue; // placeholder
        if(!pbar->IsVisible()) continue;
        if(pbar->IsKindOf(RUNTIME_CLASS(CDockBarControl)))
        {
            if(pbar == this)
                nthis = arrbars.GetSize();

            arrbars.Add(pbar);
        }
    }
}

void CDockBarControl::GetRowInfo(int &nfirst, int &nlast, int &nthis)
{
    ASSERT_VALID(m_pDockBar); // verify bounds

    nthis = m_pDockBar->FindBar(this);
    ASSERT(nthis != -1);

    int i, nbars = m_pDockBar->m_arrBars.GetSize();

    // find the first and the last bar in row
    for(nfirst = -1, i = nthis - 1; i >= 0 && nfirst == -1; i--)
        if(m_pDockBar->m_arrBars[i] == NULL)
            nfirst = i + 1;
    for(nlast = -1, i = nthis + 1; i < nbars && nlast == -1; i++)
        if(m_pDockBar->m_arrBars[i] == NULL)
            nlast = i - 1;

    ASSERT((nlast != -1) && (nfirst != -1));
}

BOOL CDockBarControl::NegotiateSpace(int nlengthtotal, BOOL bhorz)
{
    ASSERT(bhorz == IsHorzDocked());

    int nfirst, nlast, nthis;
    GetRowInfo(nfirst, nlast, nthis);

    int nlengthavail = nlengthtotal;
    int nlengthactual = 0;
    int nlengthmin = 2;
    int nwidthmax = 0;
    CDockBarControl* pbar;

    for(int i = nfirst; i <= nlast; i++)
    {
        pbar = (CDockBarControl*) m_pDockBar->m_arrBars[i];
        if(HIWORD(pbar) == 0) continue; // placeholder
        if(!pbar->IsVisible()) continue;
        BOOL issizingbar = pbar->IsKindOf(RUNTIME_CLASS(CDockBarControl));

        int nlengthbar; // minimum length of the bar
        if(issizingbar)
            nlengthbar = bhorz ? pbar->MinHorz.cx - 2 :
                pbar->MinVert.cy - 2;
        else
        {
            CRect rcbar;
            pbar->GetWindowRect(&rcbar);
            nlengthbar = bhorz ? rcbar.Width() - 2 : rcbar.Height() - 2;
        }

        nlengthmin += nlengthbar;
        if(nlengthmin > nlengthtotal)
        {
            // split the row after fixed bar
            if(i < nthis)
            {
                m_pDockBar->m_arrBars.InsertAt(i + 1,
                    (CControlBar*) NULL);
                return FALSE;
            }
            
            // only this sizebar remains on the row, adjust it to minsize
            if(i == nthis)
            {
                if(bhorz)
                    szHorz.cx = MinHorz.cx;
                else
                    szVert.cy = MinVert.cy;

                return TRUE; // the dockbar will split the row for us
            }

            // we have enough bars - go negotiate with them
            m_pDockBar->m_arrBars.InsertAt(i, (CControlBar*) NULL);
            nlast = i - 1;
            break;
        }
        if(issizingbar)
        {
            nlengthactual += bhorz ? pbar->szHorz.cx - 2 : 
                pbar->szVert.cy - 2;
            nwidthmax = max(nwidthmax, bhorz ? pbar->szHorz.cy :
                pbar->szVert.cx);
        }
        else
            nlengthavail -= nlengthbar;
    }

    CDBCArray arrbars;
    GetRowSizingBars(arrbars);
    int nnumbars = arrbars.GetSize();
    int ndelta = nlengthavail - nlengthactual;

    // return faster when there is only one sizing bar per row (this one)
    if(nnumbars == 1)
    {
        ASSERT(arrbars[0] == this);

        if(ndelta == 0)
            return TRUE;
        
        bKeepSize = FALSE;
        (bhorz ? szHorz.cx : szVert.cy) += ndelta;

        return TRUE;
    }

    // make all the bars the same width
    for(i = 0; i < nnumbars; i++)
        if(bhorz)
            arrbars[i]->szHorz.cy = nwidthmax;
        else
            arrbars[i]->szVert.cx = nwidthmax;

    // distribute the difference between the bars,
    // but don't shrink them below their minsizes
    while(ndelta != 0)
    {
        int ndeltaold = ndelta;
        for(i = 0; i < nnumbars; i++)
        {
            pbar = arrbars[i];
            int nlmin = bhorz ?
                pbar->MinHorz.cx : pbar->MinVert.cy;
            int nl = bhorz ? pbar->szHorz.cx : pbar->szVert.cy;

            if((nl == nlmin) && (ndelta < 0) || // already at min length
                pbar->bKeepSize) // or wants to keep its size
                continue;

            // sign of nDelta
            int ndelta2 = (ndelta == 0) ? 0 : ((ndelta < 0) ? -1 : 1);

            (bhorz ? pbar->szHorz.cx : pbar->szVert.cy) += ndelta2;
            ndelta -= ndelta2;
            if(ndelta == 0) break;
        }
        // clear bKeepSize flags
        if((ndeltaold == ndelta) || (ndelta == 0))
            for(i = 0; i < nnumbars; i++)
                arrbars[i]->bKeepSize = FALSE;
    }

    return TRUE;
}

void CDockBarControl::AlignControlBars()
{
    int nfirst, nlast, nthis;
    GetRowInfo(nfirst, nlast, nthis);

    BOOL bhorz = IsHorzDocked();
    BOOL bneedrecalc = FALSE;
    int nalign = bhorz ? -2 : 0;

    CRect rc, rcdock;
    m_pDockBar->GetWindowRect(&rcdock);

    for(int i = nfirst; i <= nlast; i++)
    {
        CDockBarControl* pbar =
            (CDockBarControl*) m_pDockBar->m_arrBars[i];
        if(HIWORD(pbar) == 0) continue; // placeholder
        if(!pbar->IsVisible()) continue;

        pbar->GetWindowRect(&rc);
        rc.OffsetRect(-rcdock.TopLeft());

        if(pbar->IsKindOf(RUNTIME_CLASS(CDockBarControl)))
            rc = CRect(rc.TopLeft(),
                bhorz ? pbar->szHorz : pbar->szVert);

        if((bhorz ? rc.left : rc.top) != nalign)
        {
            if(!bhorz)
                rc.OffsetRect(0, nalign - rc.top - 2);
            else if(DockBarID == AFX_IDW_DOCKBAR_TOP)
                rc.OffsetRect(nalign - rc.left, -2);
            else
                rc.OffsetRect(nalign - rc.left, 0);
            pbar->MoveWindow(rc);
            bneedrecalc = TRUE;
        }
        nalign += (bhorz ? rc.Width() : rc.Height()) - 2;
    }

    if(bneedrecalc)
        m_pDockSite->DelayRecalcLayout();
}

void CDockBarControl::OnNcPaint()
{
    CWindowDC dc(this); // Get WindowDC to draw all around window.

    CRect rcclient, rcbar;
    GetClientRect(rcclient);
    ClientToScreen(rcclient);
    GetWindowRect(rcbar);
    rcclient.OffsetRect(-rcbar.TopLeft());
    rcbar.OffsetRect(-rcbar.TopLeft());

    CDC mdc;
    mdc.CreateCompatibleDC(&dc);
    
    CBitmap bm;
    bm.CreateCompatibleBitmap(&dc, rcbar.Width(), rcbar.Height());
    CBitmap* poldbm = mdc.SelectObject(&bm);

    // draw borders in non-client area
    CRect rcdraw = rcbar;
    DrawBorders(&mdc, rcdraw);

    // erase the NC background
    mdc.FillRect(rcdraw, CBrush::FromHandle(
        (HBRUSH) GetClassLong(m_hWnd, GCL_HBRBACKGROUND)));

    if(Style & DBC_SHOWEDGES && 0)
    {
        CRect rcedge; // paint the sizing edges
        for(int i = 0; i < 4; i++)
            if(GetEdgeRect(rcbar, GetEdgeHTCode(i), rcedge))
                mdc.Draw3dRect(rcedge, ::GetSysColor(COLOR_BTNHIGHLIGHT),
                    ::GetSysColor(COLOR_BTNSHADOW));
    }

	// Call grabber painter.
	if(HasGripper())
		NcPaintGripper(&mdc, rcclient);

	// Client area is not our business, so take out it..
    dc.IntersectClipRect(rcbar);
    dc.ExcludeClipRect(rcclient);

    dc.BitBlt(0, 0, rcbar.Width(), rcbar.Height(), &mdc, 0, 0, SRCCOPY);

    ReleaseDC(&dc);

    mdc.SelectObject(poldbm);
    bm.DeleteObject();
    mdc.DeleteDC();
}

void CDockBarControl::NcPaintGripper(CDC *pdc, CRect rcclient)
{
	CRect box(0,0,0,0);
	//CPoint pos;
	BOOL bhorz = IsHorzDocked();

	if(bhorz)
	{
		if(ShowCloseBtn)
		{
			CloseBtn.SetRect(5, 4, 12 +4, 12 + 4);
			pdc->DrawFrameControl(CloseBtn, DFC_CAPTION, DFCS_CAPTIONCLOSE | (ClosePushed?DFCS_PUSHED:0));
			rcclient.top  += CloseBtn.Height();
			rcclient.bottom -= 3;
		}
		box.left = rcclient.left -= GripperYSize - 2;
		box.top = rcclient.top;
		box.right = rcclient.right;
		box.bottom = rcclient.bottom;
	}
	else
	{
		if(ShowCloseBtn)
		{
//#define DFCS_TRANSPARENT	0x800
//#define DFCS_HOT	0x1000
			CloseBtn.SetRect(rcclient.right - 12, 3, rcclient.right - 1, 12 + 3);
			pdc->DrawFrameControl(CloseBtn, DFC_CAPTION, DFCS_CAPTIONCLOSE | (ClosePushed?DFCS_PUSHED:0) | DFCS_HOT | DFCS_TRANSPARENT);
			rcclient.right  -= CloseBtn.Width() + 3;
		}
		box.left = GRIPPERWH + GRIPPERSPACE + 1;
		box.top = rcclient.top -= GripperYSize;
		box.bottom = rcclient.top + GripperYSize;
		box.right = rcclient.right;
	}
	DrawGripper(pdc, &box);
	CRect rcbar;
	GetWindowRect(rcbar);
	CloseBtn.OffsetRect(rcbar.TopLeft());
}

void CDockBarControl::DrawGripper(CDC *pdc, CRect *pr)
{
	BOOL bhorz = IsHorzDocked();

	//GripperType = 255; //DBC_GRIPPER_DOUBLE_LINES;

	switch(GripperType)
	{
		case 255:
#define DC_GRADIENT 0x0020
			::DrawCaption(GetSafeHwnd(), pdc->GetSafeHdc(), pr, DC_TEXT | DC_GRADIENT | DC_ACTIVE | DC_SMALLCAP);
		break;
		case DBC_GRIPPER_DOUBLE_LINES:
		{
			CRect r(pr);
			if(bhorz)
			{
				r.right = r.left + 3;
			}
			else
			{
				r.top++; r.bottom = r.top + 3;
			}
			pdc->Draw3dRect(&r, ::GetSysColor(COLOR_BTNHIGHLIGHT), ::GetSysColor(COLOR_BTNSHADOW));
			r.OffsetRect(bhorz?4:0,bhorz?0:4);
			pdc->Draw3dRect(&r, ::GetSysColor(COLOR_BTNHIGHLIGHT), ::GetSysColor(COLOR_BTNSHADOW));
		}
		break;
		case DBC_GRIPPER_LINE:
		{
			CRect r(pr);
			if(bhorz)
			{
				r.left = (GripperYSize+3)/2; r.right = r.left + 3;
			}
			else
			{
				r.top = (GripperYSize+3)/2; r.bottom = r.top + 3;
			}
			pdc->Draw3dRect(&r, ::GetSysColor(COLOR_BTNHIGHLIGHT), ::GetSysColor(COLOR_BTNSHADOW));
		}
		break;
		case DBC_GRIPPER_DOTS:
		{
			int i;
			int howmany;
			CPoint pos;
			pos.x = pr->left;
			pos.y = pr->top + 1;
			howmany = bhorz?pr->Height():pr->Width();
			for(i=0; i<howmany; i += GRIPPERWH + GRIPPERSPACE)
				pdc->FillSolidRect(CRect(pos.x + (bhorz?0:i),
										 pos.y + (bhorz?i:0),
										 pos.x + (bhorz?0:i) + GRIPPERWH,
										 pos.y + (bhorz?i:0) + GRIPPERWH),
										 ::GetSysColor(COLOR_BTNHIGHLIGHT));
			pos.y--;pos.x--;
			for(i=0; i<howmany; i += GRIPPERWH + GRIPPERSPACE)
				pdc->FillSolidRect(CRect(pos.x + (bhorz?0:i),
										 pos.y + (bhorz?i:0),
										 pos.x + (bhorz?0:i) + GRIPPERWH,
										 pos.y + (bhorz?i:0) + GRIPPERWH),
										 ::GetSysColor(COLOR_BTNSHADOW));
			pos.y += bhorz?GRIPPERWH + GRIPPERSPACE:pos.y + GRIPPERWH; 
			pos.x += pos.x + 1;
			howmany -= bhorz?(GRIPPERWH + GRIPPERSPACE)*2:pos.x;
			for(i=0; i<howmany; i += GRIPPERWH + GRIPPERSPACE)
				pdc->FillSolidRect(CRect(pos.x + (bhorz?0:i),
										 pos.y + (bhorz?i:0),
										 pos.x + (bhorz?0:i) + GRIPPERWH,
										 pos.y + (bhorz?i:0) + GRIPPERWH),
										 ::GetSysColor(COLOR_BTNHIGHLIGHT));
			pos.y--;pos.x--;
			for(i=0; i<howmany; i += GRIPPERWH + GRIPPERSPACE)
				pdc->FillSolidRect(CRect(pos.x + (bhorz?0:i),
										 pos.y + (bhorz?i:0),
										 pos.x + (bhorz?0:i) + GRIPPERWH,
										 pos.y + (bhorz?i:0) + GRIPPERWH),
										 ::GetSysColor(COLOR_BTNSHADOW));
		}
		break;
	default:
		ASSERT(FALSE); // shouldn't happen.
	}
}



BOOL CDockBarControl::GetEdgeRect(CRect rcwnd, UINT nhittest, CRect &rcedge)
{
    rcedge = rcwnd;
    if(Style & DBC_SHOWEDGES)
        rcedge.DeflateRect(1, 1);
    BOOL bhorz = IsHorzDocked();

    switch (nhittest)
    {
    case HTLEFT:
        if(!(Style & DBC_EDGELEFT)) return FALSE;
        rcedge.right = rcedge.left + cxEdge;
        rcedge.DeflateRect(0, bhorz ? cxEdge: 0);
        break;
    case HTTOP:
        if(!(Style & DBC_EDGETOP)) return FALSE;
        rcedge.bottom = rcedge.top + cxEdge;
        rcedge.DeflateRect(bhorz ? 0 : cxEdge, 0);
        break;
    case HTRIGHT:
        if(!(Style & DBC_EDGERIGHT)) return FALSE;
        rcedge.left = rcedge.right - cxEdge;
        rcedge.DeflateRect(0, bhorz ? cxEdge: 0);
        break;
    case HTBOTTOM:
        if(!(Style & DBC_EDGEBOTTOM)) return FALSE;
        rcedge.top = rcedge.bottom - cxEdge;
        rcedge.DeflateRect(bhorz ? 0 : cxEdge, 0);
        break;
    default:
        ASSERT(FALSE); // invalid hit test code
    }
    return TRUE;
}

UINT CDockBarControl::GetEdgeHTCode(int edge)
{
    if(edge == 0) return HTLEFT;
    if(edge == 1) return HTTOP;
    if(edge == 2) return HTRIGHT;
    if(edge == 3) return HTBOTTOM;
    ASSERT(FALSE); // For debug only invalid edge code
    return HTNOWHERE;
}

void CDockBarControl::OnNcCalcSize(BOOL bcalcvalidrects, NCCALCSIZE_PARAMS *lpncsp)
{
    UNUSED_ALWAYS(bcalcvalidrects);
    // Enable diagonal resizing for floating miniframe
    if(IsFloating())
    {
        CFrameWnd* pframe = GetParentFrame();
        if(pframe != NULL &&
            pframe->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
        {
            DWORD dwstyle = ::GetWindowLong(pframe->m_hWnd, GWL_STYLE);
            if((dwstyle & MFS_4THICKFRAME) != 0)
            {
                pframe->ModifyStyle(MFS_4THICKFRAME, 0); // clear
                GetParent()->ModifyStyle(0, WS_CLIPCHILDREN);
            }
			if(!ShowCloseBtn) // If we don't want so show close button
			{
				// Probabiltiy 1
				/*
				CMenu *pmenu = pframe->GetSystemMenu(FALSE);
				if(pmenu)
					pmenu->ModifyMenu(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
				*/
				// Probabiltiy 2
				pframe->ModifyStyle(WS_SYSMENU, 0);
				// I think completely removing system menu is better, however one can
				// use the other probability (leaved as a reminder).
			}
        }
    }
    // compute the the client area
    Style &= ~DBC_EDGEALL;

    // add resizing edges between bars on the same row
    if(!IsFloating() && m_pDockBar != NULL)
    {
        CDBCArray arrbars;
        int nthis;
        GetRowSizingBars(arrbars, nthis);

        BOOL bhorz = IsHorzDocked();
        if(nthis > 0)
            Style |= bhorz ? DBC_EDGELEFT : DBC_EDGETOP;

        if(nthis < arrbars.GetUpperBound())
            Style |= bhorz ? DBC_EDGERIGHT : DBC_EDGEBOTTOM;
    }

    NcCalcClient(&lpncsp->rgrc[0], DockBarID);
}

void CDockBarControl::NcCalcClient(LPRECT prc, UINT dockbarid)
{
    CRect rc(prc);

    rc.DeflateRect(3, 5, 3, 3);
    if(dockbarid != AFX_IDW_DOCKBAR_FLOAT)
        rc.DeflateRect(2, 0, 2, 2);

    switch(dockbarid)
    {
    case AFX_IDW_DOCKBAR_TOP:
        Style |= DBC_EDGEBOTTOM;
        break;
    case AFX_IDW_DOCKBAR_BOTTOM:
        Style |= DBC_EDGETOP;
        break;
    case AFX_IDW_DOCKBAR_LEFT:
        Style |= DBC_EDGERIGHT;
        break;
    case AFX_IDW_DOCKBAR_RIGHT:
        Style |= DBC_EDGELEFT;
        break;
    }

    // make room for edges only if they will be painted
    if(Style & DBC_SHOWEDGES)
        rc.DeflateRect(
            (Style & DBC_EDGELEFT)   ? cxEdge : 0,
            (Style & DBC_EDGETOP)	 ? cxEdge : 0,
            (Style & DBC_EDGERIGHT)	 ? cxEdge : 0,
            (Style & DBC_EDGEBOTTOM) ? cxEdge : 0);

	if(HasGripper())
	{
		// Set gripper size in height (Y direction)
		BOOL bhorz = (dockbarid == AFX_IDW_DOCKBAR_TOP) ||
					 (dockbarid == AFX_IDW_DOCKBAR_BOTTOM);
		if(bhorz)
			rc.DeflateRect(GripperYSize, 0, 0, 0);
		else
			rc.DeflateRect(0, GripperYSize, 0, 0);
	}
    *prc = rc;
}

void CDockBarControl::OnWindowPosChanging(WINDOWPOS *lpwndpos)
{
    // force non-client recalc if moved or resized
    lpwndpos->flags |= SWP_FRAMECHANGED;

    CControlBar::OnWindowPosChanging(lpwndpos);

    // find on which side are we docked
    DockBarID = GetParent()->GetDlgCtrlID();

    if(!IsFloating())
        if(lpwndpos->flags & SWP_SHOWWINDOW)
            bKeepSize = TRUE;
}

void CDockBarControl::OnCaptureChanged(CWnd *pWnd)
{
    if(bTracking && (pWnd != this))
        StopTracking();

    CControlBar::OnCaptureChanged(pWnd);	
}

void CDockBarControl::StopTracking()
{
    OnTrackInvertTracker(); // erase tracker

    bTracking = FALSE;
    ReleaseCapture();

    m_pDockSite->DelayRecalcLayout();
}

void CDockBarControl::OnTrackInvertTracker()
{
    ASSERT(bTracking);

    if(bDragShowContent)
        return; // don't show tracker if DragFullWindows is on

    BOOL bhorz = IsHorzDocked();
    CRect rc, rcbar, rcdock, rcframe;
    GetWindowRect(rcbar);
    m_pDockBar->GetWindowRect(rcdock);
    m_pDockSite->GetWindowRect(rcframe);
    VERIFY(GetEdgeRect(rcbar, htEdge, rc));
    if(!IsSideTracking())
        rc = bhorz ? 
            CRect(rcdock.left + 1, rc.top, rcdock.right - 1, rc.bottom) :
            CRect(rc.left, rcdock.top + 1, rc.right, rcdock.bottom - 1);

    BOOL bhorztracking = (htEdge == HTLEFT || htEdge == HTRIGHT);
    int nofs = nTrackPosOld - nTrackEdgeOfs;
    nofs -= bhorztracking ? rc.CenterPoint().x : rc.CenterPoint().y;
    rc.OffsetRect(bhorztracking ? nofs : 0, bhorztracking ? 0 : nofs);
    rc.OffsetRect(-rcframe.TopLeft());

    CDC *pdc = m_pDockSite->GetDCEx(NULL, DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE);
    CBrush* pbrush = CDC::GetHalftoneBrush();
    CBrush* pbrushold = pdc->SelectObject(pbrush);

    pdc->PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATINVERT);
    
    pdc->SelectObject(pbrushold);
    m_pDockSite->ReleaseDC(pdc);
}

void CDockBarControl::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CControlBar::OnSettingChange(uFlags, lpszSection);

    bDragShowContent = FALSE;
    ::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0,
        &bDragShowContent, 0); // update
}

void CDockBarControl::OnLButtonUp(UINT nFlags, CPoint point)
{
    if(bTracking)
        StopTracking();

    CControlBar::OnLButtonUp(nFlags, point);
}

void CDockBarControl::OnMouseMove(UINT nFlags, CPoint point)
{
    if(bTracking)
    {
        CPoint ptscreen = point;
        ClientToScreen(&ptscreen);

        OnTrackUpdateSize(ptscreen);
    }

    CControlBar::OnMouseMove(nFlags, point);
}

void CDockBarControl::OnTrackUpdateSize(CPoint &point)
{
    ASSERT(!IsFloating());

    BOOL bhorztrack = htEdge == HTLEFT || htEdge == HTRIGHT;

    int ntrackpos = bhorztrack ? point.x : point.y;
    ntrackpos = max(nTrackPosMin, min(nTrackPosMax, ntrackpos));

    int ndelta = ntrackpos - nTrackPosOld;

    if(ndelta == 0)
        return; // no pos change

    OnTrackInvertTracker(); // erase tracker

    nTrackPosOld = ntrackpos;
    
    BOOL bhorz = IsHorzDocked();

    CSize sizenew = bhorz ? szHorz : szVert;
    switch (htEdge)
    {
    case HTLEFT:    sizenew -= CSize(ndelta, 0); break;
    case HTTOP:     sizenew -= CSize(0, ndelta); break;
    case HTRIGHT:   sizenew += CSize(ndelta, 0); break;
    case HTBOTTOM:  sizenew += CSize(0, ndelta); break;
    }

    CDBCArray arrbars;
    int nthis;
    GetRowSizingBars(arrbars, nthis);

    if(!IsSideTracking())
        for(int i = 0; i < arrbars.GetSize(); i++)
        {
            CDockBarControl* pbar = arrbars[i];
            // make same width (or height)
            (bhorz ? pbar->szHorz.cy : pbar->szVert.cx) = bhorz ? sizenew.cy : sizenew.cx;
        }
    else
    {
        int ngrowingbar = nthis;
        BOOL bbefore = htEdge == HTTOP || htEdge == HTLEFT;
        if(bbefore && ndelta > 0)
            ngrowingbar--;
        if(!bbefore && ndelta < 0)
            ngrowingbar++;
        if(ngrowingbar != nthis)
            bbefore = !bbefore;

        // nGrowing is growing
        ndelta = abs(ndelta);
        CDockBarControl* pbar = arrbars[ngrowingbar];
        (bhorz ? pbar->szHorz.cx : pbar->szVert.cy) += ndelta;

        // the others are shrinking
        int nfirst = bbefore ? ngrowingbar - 1 : ngrowingbar + 1;
        int nlimit = bbefore ? -1 : arrbars.GetSize();

        for(int i = nfirst; ndelta != 0 && i != nlimit; i += (bbefore ? -1 : 1))
        {
            CDockBarControl* pbar = arrbars[i];
                
            int ndeltat = min(ndelta,
                (bhorz ? pbar->szHorz.cx : pbar->szVert.cy) -
                (bhorz ? pbar->MinHorz.cx : pbar->MinVert.cy));

            (bhorz ? pbar->szHorz.cx : pbar->szVert.cy) -= ndeltat;
            ndelta -= ndeltat;
        }
    }

    OnTrackInvertTracker(); // redraw tracker at new pos

    if(bDragShowContent)
        m_pDockSite->DelayRecalcLayout();
}

void CDockBarControl::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    UNUSED_ALWAYS(point);

    if(bTracking || IsFloating())
        return;

    if((nHitTest >= HTSIZEFIRST) && (nHitTest <= HTSIZELAST))
        StartTracking(nHitTest, point); // sizing edge hit
}

void CDockBarControl::StartTracking(UINT nhittest, CPoint point)
{
    SetCapture();

    // make sure no updates are pending
    if(!bDragShowContent)
        RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);

    htEdge = nhittest;
    bTracking = TRUE;

    BOOL bhorz = IsHorzDocked();
    BOOL bhorztracking = htEdge == HTLEFT || htEdge == HTRIGHT;

    nTrackPosOld = bhorztracking ? point.x : point.y;

    CRect rcbar, rcedge;
    GetWindowRect(rcbar);
    GetEdgeRect(rcbar, htEdge, rcedge);
    nTrackEdgeOfs = nTrackPosOld -
        (bhorztracking ? rcedge.CenterPoint().x : rcedge.CenterPoint().y);
    
    CDBCArray arrbars;
    int nthis;
    GetRowSizingBars(arrbars, nthis);

    nTrackPosMin = nTrackPosMax = nTrackPosOld;
    if(!IsSideTracking())
    {
        // calc minwidth as the max minwidth of the sizing bars on row
        int nminwidth = bhorz ? MinHorz.cy : MinVert.cx;
        for(int i = 0; i < arrbars.GetSize(); i++)
            nminwidth = max(nminwidth, bhorz ? 
                arrbars[i]->MinHorz.cy :
                arrbars[i]->MinVert.cx);
        int nexcesswidth = (bhorz ? szHorz.cy : szVert.cx) - nminwidth;

        // the control bar cannot grow with more than the width of
        // remaining client area of the mainframe
        CRect rct;
        m_pDockSite->RepositionBars(0, 0xFFFF, AFX_IDW_PANE_FIRST,
            reposQuery, &rct, NULL, TRUE);
        int nmaxwidth = bhorz ? rct.Height() - 2 : rct.Width() - 2;

        BOOL btoporleft = htEdge == HTTOP || htEdge == HTLEFT;

        nTrackPosMin -= btoporleft ? nmaxwidth : nexcesswidth;
        nTrackPosMax += btoporleft ? nexcesswidth : nmaxwidth;
    }
    else
    {
        // side tracking:
        // max size is the actual size plus the amount the other
        // sizing bars can be decreased until they reach their minsize
        if(htEdge == HTBOTTOM || htEdge == HTRIGHT)
            nthis++;

        for(int i = 0; i < arrbars.GetSize(); i++)
        {
            CDockBarControl* pbar = arrbars[i];

            int nexcesswidth = bhorz ? 
                pbar->szHorz.cx - pbar->MinHorz.cx :
                pbar->szVert.cy - pbar->MinVert.cy;

            if(i < nthis)
                nTrackPosMin -= nexcesswidth;
            else
                nTrackPosMax += nexcesswidth;
        }
    }

    OnTrackInvertTracker(); // draw tracker
}

void CDockBarControl::OnLButtonDown(UINT nFlags, CPoint point)
{
    if(m_pDockBar != NULL)
    {
        // start the drag
        ASSERT(m_pDockContext != NULL);
        ClientToScreen(&point);
        m_pDockContext->StartDrag(point);
    }
    else
        CControlBar::OnLButtonDown(nFlags, point);
}

static LPCTSTR CCls()
{
	WNDCLASS wndcls;
	HINSTANCE hinst = AfxGetInstanceHandle();
	LPCTSTR pszname;

	pszname = "DocAbleWnd-v1.x-ErdincAkan2003";
	hinst = AfxGetInstanceHandle();
	if(::GetClassInfo(hinst, pszname, &wndcls))
	{
		ASSERT(wndcls.style == CS_DBLCLKS);
		return pszname;
	}

	wndcls.style = CS_DBLCLKS;
	wndcls.lpfnWndProc = DefWindowProc;
	wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
	wndcls.hInstance = hinst;
	wndcls.hIcon = NULL;
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = ::GetSysColorBrush(COLOR_BTNFACE);
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = pszname;
	if(!AfxRegisterClass(&wndcls))
		AfxThrowResourceException();
	return pszname;
}

void CDockBarControl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if(m_pDockBar != NULL)
    {
        // toggle docking
        ASSERT(m_pDockContext != NULL);
        m_pDockContext->ToggleDocking();
    }
    else
        CControlBar::OnLButtonDblClk(nFlags, point);
}

void CDockBarControl::OnRButtonDown(UINT nFlags, CPoint point)
{
    if(bTracking)
        StopTracking();

    CControlBar::OnRButtonDown(nFlags, point);
}

UINT CDockBarControl::OnNcHitTest(CPoint point)
{
    CRect rcbar, rcedge;
    GetWindowRect(rcbar);

    if(!IsFloating())
    {
		for(int i = 0; i < 4; i++)
        {
			if(GetEdgeRect(rcbar, GetEdgeHTCode(i), rcedge))
                if(rcedge.PtInRect(point))
                    return GetEdgeHTCode(i);
		}
		if(ShowCloseBtn && CloseBtn.PtInRect(point))
			return HTCLOSE;
	}

    return HTCLIENT;
}

void CDockBarControl::OnSize(UINT nType, int cx, int cy)
{
    UNUSED_ALWAYS(nType);
    
    if((Style & DBC_SIZECHILD) != 0)
    {
        // automatic child resizing - only one child is allowed
        CWnd* pWnd = GetWindow(GW_CHILD);
        if(pWnd != NULL)
        {
            pWnd->MoveWindow(0, 0, cx, cy);
            ASSERT(pWnd->GetWindow(GW_HWNDNEXT) == NULL);
        }
    }
}

static void AdjustRectangle(CRect& rect, CPoint pt)
{
    int nxoffset = (pt.x < rect.left) ? (pt.x - rect.left) :
                    (pt.x > rect.right) ? (pt.x - rect.right) : 0;
    int nyoffset = (pt.y < rect.top) ? (pt.y - rect.top) :
                    (pt.y > rect.bottom) ? (pt.y - rect.bottom) : 0;
    rect.OffsetRect(nxoffset, nyoffset);
}

void CDBContext::StartDrag(CPoint pt)
{
    ASSERT_VALID(m_pBar);
    m_bDragging = TRUE;

    InitLoop();

    ASSERT((m_pBar->m_dwStyle & CBRS_SIZE_DYNAMIC) != 0);

    // get true bar size (including borders)
    CRect rect;
    m_pBar->GetWindowRect(rect);
    m_ptLast = pt;
    CSize sizeHorz = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_HORZDOCK);
    CSize sizeVert = m_pBar->CalcDynamicLayout(0, LM_VERTDOCK);
    CSize sizeFloat = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH);

    m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
    m_rectDragVert = CRect(rect.TopLeft(), sizeVert);

    // calculate frame dragging rectangle
    m_rectFrameDragHorz = CRect(rect.TopLeft(), sizeFloat);

#ifdef _MAC
    CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz,
        WS_THICKFRAME, WS_EX_FORCESIZEBOX);
#else
    CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz, WS_THICKFRAME);
#endif
    m_rectFrameDragHorz.DeflateRect(2, 2);
    m_rectFrameDragVert = m_rectFrameDragHorz;
    
    // adjust rectangles so that point is inside
    AdjustRectangle(m_rectDragHorz, pt);
    AdjustRectangle(m_rectDragVert, pt);
    AdjustRectangle(m_rectFrameDragHorz, pt);
    AdjustRectangle(m_rectFrameDragVert, pt);

    // initialize tracking state and enter tracking loop
    m_dwOverDockStyle = CanDock();
    Move(pt);   // call it here to handle special keys
    Track();
}

void CDockBarControl::SetDBCStyle(DWORD style)
{
	Style = (style & ~DBC_EDGEALL);
}

DWORD CDockBarControl::GetDBCStyle()
{
	return Style;
}

void CDockBarControl::OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisableIfNoHndler)
{
	UNUSED_ALWAYS(bDisableIfNoHndler);
    UNUSED_ALWAYS(pTarget);

    if(!HasGripper() || !ShowCloseBtn)
        return;
    
    CPoint pt;
    ::GetCursorPos(&pt);
    BOOL bhit = (OnNcHitTest(pt) == HTCLOSE);
    BOOL blbtndown = (::GetKeyState(VK_LBUTTON) < 0);
	BOOL WasPushed = ClosePushed;
	if(bhit && blbtndown)
		ClosePushed = TRUE;
	else
		if(ClosePushed)
			ClosePushed = FALSE;

	if(WasPushed ^ ClosePushed)
        SendMessage(WM_NCPAINT);
}

CSize CDockBarControl::CalcDynamicLayout(int nlength, DWORD dwmode)
{
    if(dwmode & (LM_HORZDOCK | LM_VERTDOCK)) // docked ?
    {
        if(nlength == -1)
            bParentSizing = TRUE;

        return CControlBar::CalcDynamicLayout(nlength, dwmode);
    }

    if(dwmode & LM_MRUWIDTH) return szFloat;
    if(dwmode & LM_COMMIT) return szFloat; // already committed

    // check for dialgonal resizing hit test
    int nhittest = m_pDockContext->m_nHitTest;
    if(IsFloating() &&
        (nhittest == HTTOPLEFT || nhittest == HTBOTTOMLEFT ||
        nhittest == HTTOPRIGHT || nhittest == HTBOTTOMRIGHT))
    {
        CPoint ptcursor;
        ::GetCursorPos(&ptcursor);

        CRect rframe, rbar;
        GetParentFrame()->GetWindowRect(&rframe);
        GetWindowRect(&rbar);
        
        if(nhittest == HTTOPLEFT || nhittest == HTBOTTOMLEFT)
        {
            szFloat.cx = rframe.left + rbar.Width() - ptcursor.x;
            m_pDockContext->m_rectFrameDragHorz.left = 
                min(ptcursor.x, rframe.left + rbar.Width() - MinFloat.cx);
        }

        if(nhittest == HTTOPLEFT || nhittest == HTTOPRIGHT)
        {
            szFloat.cy = rframe.top + rbar.Height() - ptcursor.y;
            m_pDockContext->m_rectFrameDragHorz.top =
                min(ptcursor.y, rframe.top + rbar.Height() - MinFloat.cy);
        }

        if(nhittest == HTTOPRIGHT || nhittest == HTBOTTOMRIGHT)
            szFloat.cx = rbar.Width() + ptcursor.x - rframe.right;

        if(nhittest == HTBOTTOMLEFT || nhittest == HTBOTTOMRIGHT)
            szFloat.cy = rbar.Height() + ptcursor.y - rframe.bottom;
    }
    else
        ((dwmode & LM_LENGTHY) ? szFloat.cy : szFloat.cx) = nlength;

    szFloat.cx = max(szFloat.cx, MinFloat.cx);
    szFloat.cy = max(szFloat.cy, MinFloat.cy);

    return szFloat;
}

BOOL CDockBarControl::HasGripper()
{
	return !IsFloating();
}

void CDockBarControl::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	if(ShowCloseBtn && nHitTest == HTCLOSE)
		m_pDockSite->ShowControlBar(this, FALSE, FALSE); // hide
	CControlBar::OnNcLButtonUp(nHitTest, point);
}

void CDockBarControl::ShowBar(BOOL bshow)
{
	if(bshow == IsVisible())
		return;
	m_pDockSite->ShowControlBar(this, bshow, FALSE);
}

void CDockBarControl::SetRegSection(LPCTSTR pszsection)
{
	ASSERT(pszsection);
	RegSection.Format("%s-EA_CDB-%d", pszsection, GetDlgCtrlID());
}

CString & CDockBarControl::GetRegSection()
{
	return RegSection;
}

// Warning: one must call from CMainFrame's OnDestroy() or DestroyWindow()!
void CDockBarControl::SaveState()
{
	VERIFY(RegSection.IsEmpty() == FALSE);
	ASSERT_VALID(this);
	ASSERT(GetSafeHwnd());

	CWinApp *papp = AfxGetApp();

	//Horizontal position
	papp->WriteProfileInt(RegSection, "szHorzcx", szHorz.cx);
	papp->WriteProfileInt(RegSection, "szHorzcy", szHorz.cy);

	//Vertical position
	papp->WriteProfileInt(RegSection, "szVertcx", szVert.cx);
	papp->WriteProfileInt(RegSection, "szVertcy", szVert.cy);

	//Floating position
	papp->WriteProfileInt(RegSection, "szFloatcx", szFloat.cx);
	papp->WriteProfileInt(RegSection, "szFloatcy", szFloat.cy);
}


void CDockBarControl::LoadState()
{
	VERIFY(RegSection.IsEmpty() == FALSE);
	ASSERT_VALID(this);
	ASSERT(GetSafeHwnd());
	
	CWinApp *papp = AfxGetApp();

	//Horizontal position
	szHorz.cx = max(MinHorz.cx, (int) papp->GetProfileInt(RegSection, "szHorzcx", szHorz.cx));
	szHorz.cy = max(MinHorz.cy, (int) papp->GetProfileInt(RegSection, "szHorzcy", szHorz.cy));

	//Vertical position
	szVert.cx = max(MinVert.cx, (int) papp->GetProfileInt(RegSection, "szVertcx", szVert.cx));
	szVert.cy = max(MinVert.cy, (int) papp->GetProfileInt(RegSection, "szVertcy", szVert.cy));

	//Floating position
	szFloat.cx = max(MinFloat.cx, (int) papp->GetProfileInt(RegSection, "szFloatcx", szFloat.cx));
	szFloat.cy = max(MinFloat.cy, (int) papp->GetProfileInt(RegSection, "szFloatcy", szFloat.cy));
}

void CDockBarControl::GlobalLoadSaveState(CFrameWnd* pframe, BOOL load, LPCTSTR pszregsection)
{
	ASSERT_VALID(pframe);
    POSITION pos = pframe->m_listControlBars.GetHeadPosition();
    while(pos != NULL)
    {
        CDockBarControl* pbar = (CDockBarControl*) pframe->m_listControlBars.GetNext(pos);
        ASSERT(pbar != NULL);
        if(pbar->IsKindOf(RUNTIME_CLASS(CDockBarControl)))
		{
			if(pszregsection)
				pbar->SetRegSection(pszregsection);
			if(!pbar->GetRegSection().IsEmpty())
				if(load)
					pbar->LoadState();
				else 
					pbar->SaveState();
		}
    }
}

void CDockBarControl::InitDefaultValues()
{
	bTracking = FALSE;
    bKeepSize = FALSE;
    bParentSizing = FALSE;
    cxEdge = 5;
    bDragShowContent = FALSE;
    DockBarID = 0;
    Style = 0;
	ClosePushed = FALSE;
	SetGripperType();
	ShowCloseBtn = FALSE;
	InitDefaultSizes();
}

// Just In case, visual studio is not showing this MEMBER function,
// I've override it to sure that I can see this function and remember...
BOOL CDockBarControl::IsVisible()
{
	return CControlBar::IsVisible();
}

void CDockBarControl::InitDefaultSizes()
{
    MinHorz = CSize(33, 32);
    MinVert = CSize(33, 32);
    MinFloat = CSize(37, 32);
    szHorz = CSize(200, 200);
    szVert = CSize(200, 200);
    szFloat = CSize(200, 200);
	GripperYSize = 12;
}


BOOL CDockBarControl::PreCreateWindow(CREATESTRUCT &cs)
{
	return CControlBar::PreCreateWindow(cs);
}

void CDockBarControl::SetGripperType(BYTE grippertype)
{
	GripperType = grippertype;
}

void CDockBarControl::ShowCloseButton(BOOL show)
{
	ShowCloseBtn = show;
	if(IsWindow(GetSafeHwnd()))
		SendMessage(WM_NCPAINT);
}
