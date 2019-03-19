// MfcOgl.h : main header file for the MFCOGL application
//

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMfcOglApp:
// See MfcOgl.cpp for the implementation of this class
//

class CMfcOglApp : public CWinApp
{
public:
    CMfcOglApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMfcOglApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CMfcOglApp)
    afx_msg void OnAppAbout();
    	// NOTE - the ClassWizard will add and remove member functions here.
    	//    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
