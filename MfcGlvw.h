// MfcGlvw.h : interface of the CMfcOglView class
//
/////////////////////////////////////////////////////////////////////////////

class CMfcOglView : public CView
{
private:
    UINT        m_nTimerID;

protected: // create from serialization only
    CMfcOglView();
    DECLARE_DYNCREATE(CMfcOglView)

// Attributes
public:
    CPalette    m_cPalette;
	CRect		m_oldRect;
	float		m_fRadius;
    CClientDC   *m_pDC;

	CMfcOglDoc* GetDocument();

// Operations
public:
    void Init();
    void CreateRGBPalette();
    BOOL bSetupPixelFormat();
    unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift);
    void DrawScene(void);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMfcOglView)
    public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CMfcOglView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CMfcOglView)
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MfcGlvw.cpp
inline CMfcOglDoc* CMfcOglView::GetDocument()
    { return (CMfcOglDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
