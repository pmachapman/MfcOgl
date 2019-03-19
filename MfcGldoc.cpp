// MfcGldoc.cpp : implementation of the CMfcOglDoc class
//

#include "stdafx.h"
#include "MfcOgl.h"

#include "MfcGldoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMfcOglDoc

IMPLEMENT_DYNCREATE(CMfcOglDoc, CDocument)

BEGIN_MESSAGE_MAP(CMfcOglDoc, CDocument)
	//{{AFX_MSG_MAP(CMfcOglDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcOglDoc construction/destruction

CMfcOglDoc::CMfcOglDoc()
{
	// TODO: add one-time construction code here

}

CMfcOglDoc::~CMfcOglDoc()
{
}

BOOL CMfcOglDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMfcOglDoc serialization

void CMfcOglDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
    	// TODO: add storing code here
    }
    else
    {
    	// TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CMfcOglDoc diagnostics

#ifdef _DEBUG
void CMfcOglDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CMfcOglDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMfcOglDoc commands
