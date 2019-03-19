// mainfrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MfcOgl.h"

#include "mfcgldoc.h"
#include "mfcglvw.h"
#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
    // remove this flag to remove " - Untitled" from the frame's caption

    cs.style &= ~ FWS_ADDTOTITLE;

    cs.cx = 400;
    cs.cy = 400;
	
    return CFrameWnd::PreCreateWindow(cs);
}

void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd) 
{
    CFrameWnd::OnPaletteChanged(pFocusWnd);
	
    if (pFocusWnd != this)
        OnQueryNewPalette();
	
}

BOOL CMainFrame::OnQueryNewPalette() 
{
    WORD		i;
    CPalette	*pOldPal;
    CMfcOglView *pView = (CMfcOglView *)GetActiveView();
    CClientDC	dc(pView);


    pOldPal = dc.SelectPalette(&pView->m_cPalette, FALSE);
    i = dc.RealizePalette();
    dc.SelectPalette(pOldPal, FALSE);

    if (i > 0)
        InvalidateRect(NULL);
	
    return CFrameWnd::OnQueryNewPalette();
}
