// MfcGldoc.h : interface of the CMfcOglDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CMfcOglDoc : public CDocument
{
protected: // create from serialization only
    CMfcOglDoc();
    DECLARE_DYNCREATE(CMfcOglDoc)

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMfcOglDoc)
    public:
    virtual BOOL OnNewDocument();
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CMfcOglDoc();
    virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CMfcOglDoc)
    	// NOTE - the ClassWizard will add and remove member functions here.
    	//    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
