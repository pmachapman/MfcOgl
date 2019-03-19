# MFCOGL a Generic MFC OpenGL Code Sample

The information in this article applies to:

* Microsoft Win32 Application Programming Interface (API), used with:
   * Microsoft Windows NT, versions 3.5, 3.51
   * Microsoft Windows 95
   * Microsoft Visual C++, 32-bit Editions, versions 2.0, 2.1

---

## Summary

Microsoft Windows NT's OpenGL can be used with the Microsoft Foundation Class (MFC) library. This article gives you the steps to follow to enable MFC applications to use OpenGL.

The companion sample (MFCOGL) is a generic sample that demonstrates using OpenGL with MFC. 

## More Information

The following file is available for download from the Microsoft Software Library:

 * [MFCOGL.EXE](https://github.com/pmachapman/MfcOgl/releases/download/1.0/MFCOGL.EXE)

For more information about downloading files from the Microsoft Software Library, please see the following article in the Microsoft Knowledge Base:

 * [Q119591](https://jeffpar.github.io/kbarchive/kb/119/Q119591/) How to Obtain Microsoft Support Files from Online Services
 
 ## Step-by-Step Example to Use OpenGL in MFC Application
 
1. Include the necessary header files to use OpenGL including: - "gl\gl.h" for all core OpenGL library functions. These functions have the "gl" prefix such as **glBegin()**. - "gl\glu.h" for all OpenGL utility library functions. These functions have the "glu" prefix such as **gluLookAt()**. - "gl\glaux.h" for all Windows NT OpenGL auxiliary library functions. These functions have the "aux" prefix such as **auxSphere()**. You don't need to add a header file for functions with the "wgl" prefix.
Add necessary library modules to the link project settings. These library modules include OPENGL32.LIB, GLU32.LIB, and GLAUX.LIB.

2. Add implementations for **OnPaletteChanged()** and **OnQueryNewPalette()** in **CMainFrame** class for palette-aware applications.

```C++
   void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd)
   {
       CFrameWnd::OnPaletteChanged(pFocusWnd);

       if (pFocusWnd != this)
           OnQueryNewPalette();

   }

   BOOL CMainFrame::OnQueryNewPalette()
   {
       WORD   i;
       CPalette   *pOldPal;
       CMfcOglView *pView = (CMfcOglView *)GetActiveView();
       CClientDC   dc(pView);


       pOldPal = dc.SelectPalette(&pView->m_cPalette, FALSE);
       i = dc.RealizePalette();
       dc.SelectPalette(pOldPal, FALSE);

       if (i > 0)
           InvalidateRect(NULL);

       return CFrameWnd::OnQueryNewPalette();
   } 
```

3. Use the one or more of the following classes derived from CWnd, including view classes, that will use OpenGL for rendering onto:

 - Implement **PreCreateWindow()** and add WS_CLIPSIBLINGS and WS_CLIPCHILDREN to the windows styles:
```C++
   BOOL CMfcOglView::PreCreateWindow(CREATESTRUCT& cs)
   {
       cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

       return CView::PreCreateWindow(cs);
   } 
```
 - Implement **OnCreate()** to initialize a rendering context and make it current. Also, initialize any OpenGL states here:

```C++
   int CMfcOglView::OnCreate(LPCREATESTRUCT lpCreateStruct)
   {
       if (CView::OnCreate(lpCreateStruct) == -1)
           return -1;

       Init(); // initialize OpenGL

       return 0;
   } 
```

 - Implement **OnSize()** if the window is sizeable:

```C++
   void CMfcOglView::OnSize(UINT nType, int cx, int cy)
   {
       CView::OnSize(nType, cx, cy);

       if (cy > 0)
       {
           glViewport(0, 0, cx, cy);

           if ((m_oldRect.right > cx) || (m_oldRect.bottom > cy))
               RedrawWindow();

           m_oldRect.right = cx;
           m_oldRect.bottom = cy;

           glMatrixMode(GL_PROJECTION);
           glLoadIdentity();
           gluPerspective(45.0f, (GLdouble)cx / cy, 3.0f, 7.0f);
           glMatrixMode(GL_MODELVIEW);
       }
   } 
```

 - Implement OpenGL rendering code. This can be done in **OnDraw()** or other application-specific places such as **OnTimer()**.
   
 - Implement clean-up code, which is typically done in **OnDestroy()**:

```C++
   void CMfcOglView::OnDestroy()
   {
       HGLRC   hrc;

       if (m_nTimerID)
           KillTimer(m_nTimerID);

       hrc = ::wglGetCurrentContext();

       ::wglMakeCurrent(NULL,  NULL);

       if (hrc)
        ::wglDeleteContext(hrc);

       CPalette    palDefault;

       // Select our palette out of the dc
       palDefault.CreateStockObject(DEFAULT_PALETTE);
       m_pDC->SelectPalette(&palDefault, FALSE);

       if (m_pDC)
           delete m_pDC;

      CView::OnDestroy();
   }
```
