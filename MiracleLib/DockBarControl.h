/***************************************************************************
	CDockBarControl: Docking able window control class
	© 2003 Erdinc AKAN, AIT Ltd.
	
	File Name		: DockBarControl.h
	Version			: 1.0
	Release Date	: 10/10/2003
	Contact			: erakan_AT_softhome.net
	Mobile			: +90535 932 SAKARYA SAKARYA


 ***************************************************************************/

#if !defined(AFX_DOCKBARCONTROL_H__EB51C21E_D8B8_409B_BB20_816B5FB12A60__INCLUDED_)
#define AFX_DOCKBARCONTROL_H__EB51C21E_D8B8_409B_BB20_816B5FB12A60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxpriv.h>    // for CDockContext
#include <afxtempl.h>   // for CTypedPtrArray

// Control Bar Styles
#define DBC_EDGELEFT       0x00000001
#define DBC_EDGERIGHT      0x00000002
#define DBC_EDGETOP        0x00000004
#define DBC_EDGEBOTTOM     0x00000008
#define DBC_EDGEALL        0x0000000F
#define DBC_SHOWEDGES      0x00000010
#define DBC_SIZECHILD      0x00000020

// Gripper Styles
#define DBC_GRIPPER_DOTS			0x00001
#define DBC_GRIPPER_LINE			0x00002
#define DBC_GRIPPER_DOUBLE_LINES	0x00004

// Size and space of points in the gripper
#define GRIPPERWH 2
#define GRIPPERSPACE 3


class CCastDockBar: public CDockBar
{
	friend class CDockBarControl;
};

class CDBContext : public CDockContext
{
public:
	virtual void StartDrag(CPoint pt);
	CDBContext(CControlBar* pBar) : CDockContext(pBar) {} // Construction.
};

class CDockBarControl;
typedef CTypedPtrArray <CPtrArray, CDockBarControl*> CDBCArray;

class CDockBarControl : public CControlBar  
{
	DECLARE_DYNAMIC(CDockBarControl);
public:
	CDockBarControl() {}
	virtual ~CDockBarControl() {}
	BOOL Create(LPCTSTR wndname, CWnd *powner, UINT nid, DWORD style = WS_CHILD | WS_VISIBLE | CBRS_TOP);

public:
	BOOL IsSideTracking() const;
	BOOL IsVertDocked() const;
	BOOL IsHorzDocked() const;
	BOOL IsFloating() const;
	DWORD GetDBCStyle();

public:
	void ShowCloseButton(BOOL show = TRUE);
	void SetGripperType(BYTE grippertype = DBC_GRIPPER_DOTS);
	BOOL IsVisible();
	CString &GetRegSection();
	void GlobalLoadSaveState(CFrameWnd* pframe, BOOL load = TRUE, LPCTSTR pszregsection = NULL);
	void SaveState();
	void LoadState();
	void SetRegSection(LPCTSTR pszkey);
	void ShowBar(BOOL bshow = TRUE);
	void SetDBCStyle(DWORD style);

	// Overridables
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);
	virtual CSize CalcFixedLayout(BOOL bstretch, BOOL bhorz);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	UINT GetEdgeHTCode(int edge);

	BOOL HasGripper();
	BOOL GetEdgeRect(CRect rcwnd, UINT nhittest, CRect& rcedge);
	BOOL NegotiateSpace(int nlengthtotal, BOOL bhorz);
	
	void GetRowSizingBars(CDBCArray& arrbars, int& nthis);
	void GetRowSizingBars(CDBCArray &arrbars);
	void AlignControlBars();
	void GetRowInfo(int& nfirst, int& nlast, int& nthis);

	// Minimum values for horz, vertical and floating wnd.
	CSize MinHorz;
	CSize MinVert;
	CSize MinFloat;
	//Values for current horz, vertical and floating wnd.
	CSize szHorz;
	CSize szVert;
	CSize szFloat;
	
	DWORD Style;
	DWORD DockStyle;

	CRect CloseBtn;

    int nTrackPosMin;
    int nTrackPosMax;
    int nTrackPosOld;
    int nTrackEdgeOfs;
	int cxEdge;
	int GripperYSize;

	UINT htEdge;
	UINT DockBarID;
	BYTE GripperType;

	BOOL bTracking;
	BOOL bParentSizing;
	BOOL bKeepSize;
	BOOL bDragShowContent;
	BOOL ClosePushed;
	BOOL ShowCloseBtn;

	CString RegSection;

	// MFC Message Handlers.
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT cs);
	afx_msg void OnPaint() {CPaintDC dc(this); } // Overriden to skip border painting
	afx_msg void OnNcCalcSize(BOOL bcalcvalidrects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose() {} // Overriden to handle wrongfully close.
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);

	// Overridables
	virtual void InitDefaultValues();
	virtual void InitDefaultSizes();
	virtual void StartTracking(UINT nhittest, CPoint point);
	virtual void OnTrackUpdateSize(CPoint& point);
	virtual void OnTrackInvertTracker();
	virtual void StopTracking();
	virtual void NcCalcClient(LPRECT prc, UINT dockbarid);
	virtual void NcPaintGripper(CDC* pdc, CRect rcclient);
	virtual void DrawGripper(CDC *pdc, CRect * pr);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DOCKBARCONTROL_H__EB51C21E_D8B8_409B_BB20_816B5FB12A60__INCLUDED_)
