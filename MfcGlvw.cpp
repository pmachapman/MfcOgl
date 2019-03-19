// MfcGlvw.cpp : implementation of the CMfcOglView class
//

#include "stdafx.h"
#include "MfcOgl.h"

#include "MfcGldoc.h"
#include "MfcGlvw.h"

#include "gl\gl.h"
#include "gl\glu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

unsigned char threeto8[8] = {
    0, 0111>>1, 0222>>1, 0333>>1, 0444>>1, 0555>>1, 0666>>1, 0377
};

unsigned char twoto8[4] = {
    0, 0x55, 0xaa, 0xff
};

unsigned char oneto8[2] = {
    0, 255
};

static int defaultOverride[13] = {
    0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
};

static PALETTEENTRY defaultPalEntry[20] = {
    { 0,   0,   0,    0 },
    { 0x80,0,   0,    0 },
    { 0,   0x80,0,    0 },
    { 0x80,0x80,0,    0 },
    { 0,   0,   0x80, 0 },
    { 0x80,0,   0x80, 0 },
    { 0,   0x80,0x80, 0 },
    { 0xC0,0xC0,0xC0, 0 },

    { 192, 220, 192,  0 },
    { 166, 202, 240,  0 },
    { 255, 251, 240,  0 },
    { 160, 160, 164,  0 },

    { 0x80,0x80,0x80, 0 },
    { 0xFF,0,   0,    0 },
    { 0,   0xFF,0,    0 },
    { 0xFF,0xFF,0,    0 },
    { 0,   0,   0xFF, 0 },
    { 0xFF,0,   0xFF, 0 },
    { 0,   0xFF,0xFF, 0 },
    { 0xFF,0xFF,0xFF, 0 }
};

/////////////////////////////////////////////////////////////////////////////
// CMfcOglView

IMPLEMENT_DYNCREATE(CMfcOglView, CView)

BEGIN_MESSAGE_MAP(CMfcOglView, CView)
    //{{AFX_MSG_MAP(CMfcOglView)
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_TIMER()
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcOglView construction/destruction

CMfcOglView::CMfcOglView()
{
    // TODO: add construction code here

}

CMfcOglView::~CMfcOglView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMfcOglView drawing

void CMfcOglView::OnDraw(CDC* pDC)
{
    DrawScene();
}

/////////////////////////////////////////////////////////////////////////////
// CMfcOglView diagnostics

#ifdef _DEBUG
void CMfcOglView::AssertValid() const
{
    CView::AssertValid();
}

void CMfcOglView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CMfcOglDoc* CMfcOglView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMfcOglDoc)));
    return (CMfcOglDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMfcOglView message handlers

BOOL CMfcOglView::PreCreateWindow(CREATESTRUCT& cs) 
{
    // An OpenGL window must be created with the following flags and must not
    // include CS_PARENTDC for the class style. Refer to SetPixelFormat
    // documentation in the "Comments" section for further information.

    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	
    return CView::PreCreateWindow(cs);
}

int CMfcOglView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;
	
    Init(); // initialize OpenGL
	
    return 0;
}

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

void CMfcOglView::OnTimer(UINT nIDEvent) 
{
	DrawScene();	
	
    CView::OnTimer(nIDEvent);
}

void CMfcOglView::OnDestroy() 
{
    HGLRC   hrc;

    if (m_nTimerID)
        KillTimer(m_nTimerID);

	hrc	 = ::wglGetCurrentContext();

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

/////////////////////////////////////////////////////////////////////////////
// GL helper functions

void CMfcOglView::Init()
{
    PIXELFORMATDESCRIPTOR pfd;
    int         n;
	HGLRC		hrc;
	GLfloat     fMaxObjSize, fAspect;
	GLfloat     fNearPlane, fFarPlane;

    m_pDC = new CClientDC(this);

    ASSERT(m_pDC != NULL);

    if (!bSetupPixelFormat())
        return;

    n = ::GetPixelFormat(m_pDC->GetSafeHdc());
    ::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

    CreateRGBPalette();

    // create a rendering context and make it current
        
    hrc = wglCreateContext(m_pDC->GetSafeHdc());
    wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);

    GetClientRect(&m_oldRect);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);

    fAspect = (GLfloat)m_oldRect.right / m_oldRect.bottom;
	fNearPlane = 3.0f;
	fFarPlane = 7.0f;
	fMaxObjSize = 3.0f;
	m_fRadius = fNearPlane + fMaxObjSize / 2.0f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, fAspect, fNearPlane, fFarPlane);
    glMatrixMode(GL_MODELVIEW);
	
    m_nTimerID = SetTimer(1, 50, NULL);

    ASSERT(m_nTimerID != NULL);
}

BOOL CMfcOglView::bSetupPixelFormat()
{
    static PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW |            // support window
          PFD_SUPPORT_OPENGL |          // support OpenGL
          PFD_DOUBLEBUFFER,             // double buffered
        PFD_TYPE_RGBA,                  // RGBA type
        24,                             // 24-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
        16,                             // 16-bit z-buffer
        0,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };

    int     pixelformat;

    if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
    {
        MessageBox("ChoosePixelFormat failed");
        return FALSE;
    }

    if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
    {
        MessageBox("SetPixelFormat failed");
        return FALSE;
    }

    return TRUE;
}

unsigned char CMfcOglView::ComponentFromIndex(int i, UINT nbits, UINT shift)
{
    unsigned char val;

    val = (unsigned char) (i >> shift);

    switch (nbits)
    {
        case 1:
            val &= 0x1;
            return oneto8[val];

        case 2:
            val &= 0x3;
            return twoto8[val];

        case 3:
            val &= 0x7;
            return threeto8[val];

        default:
            return 0;
    }
}

void CMfcOglView::CreateRGBPalette()
{
    PIXELFORMATDESCRIPTOR pfd;
    LOGPALETTE  *pPal;
    WORD        n, i;

    n = ::GetPixelFormat(m_pDC->GetSafeHdc());
    ::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

    if (pfd.dwFlags & PFD_NEED_PALETTE)
    {
        n = 1 << pfd.cColorBits;

                                               
        pPal = (PLOGPALETTE) new BYTE[sizeof(LOGPALETTE) + n * sizeof(PALETTEENTRY)];

        ASSERT(pPal != NULL);

        pPal->palVersion = 0x300;
        pPal->palNumEntries = n;

        for (i=0; i<n; i++)
        {
            pPal->palPalEntry[i].peRed =
                    ComponentFromIndex(i, pfd.cRedBits, pfd.cRedShift);
            pPal->palPalEntry[i].peGreen =
                    ComponentFromIndex(i, pfd.cGreenBits, pfd.cGreenShift);
            pPal->palPalEntry[i].peBlue =
                    ComponentFromIndex(i, pfd.cBlueBits, pfd.cBlueShift);
            pPal->palPalEntry[i].peFlags = 0;
        }

        // fix up the palette to include the default GDI palette

        if ((pfd.cColorBits == 8)                           &&
            (pfd.cRedBits   == 3) && (pfd.cRedShift   == 0) &&
            (pfd.cGreenBits == 3) && (pfd.cGreenShift == 3) &&
            (pfd.cBlueBits  == 2) && (pfd.cBlueShift  == 6)
           )
        {
			for (i = 1 ; i <= 12 ; i++)
                pPal->palPalEntry[defaultOverride[i]] = defaultPalEntry[i];
        }
        

        m_cPalette.CreatePalette(pPal);
        delete pPal;

        m_pDC->SelectPalette(&m_cPalette, FALSE);
        m_pDC->RealizePalette();
    }
}

void CMfcOglView::DrawScene(void)
{
    static BOOL     bBusy = FALSE;
    static GLfloat  AngleY = 10.0f;
    static GLfloat  AngleX = 1.0f;
    static GLfloat  AngleZ = 5.0f;

    if(bBusy)
        return;

    bBusy = TRUE;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

        glTranslatef(0.0f, 0.0f, -(GLfloat)m_fRadius);
        glRotatef(AngleX, 1.0f, 0.0f, 0.0f);
        glRotatef(AngleY, 0.0f, 1.0f, 0.0f);
        glRotatef(AngleZ, 0.0f, 0.0f, 1.0f);

        AngleX += 1.0f;
        AngleY += 10.0f;
        AngleZ += 5.0f;

		glBegin(GL_QUAD_STRIP);
			glColor3f(1.0f, 0.0f, 1.0f);
			glVertex3f(-0.5f, 0.5f, 0.5f);

            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f, 0.5f);

			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0.5f, 0.5f, 0.5f);

            glColor3f(1.0f, 1.0f, 0.0f);
            glVertex3f(0.5f, -0.5f, 0.5f);

			glColor3f(0.0f, 1.0f, 1.0f);
			glVertex3f(0.5f, 0.5f, -0.5f);

            glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.5f, -0.5f, -0.5f);
		
			glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(-0.5f, 0.5f, -0.5f);

            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f,  -0.5f);

			glColor3f(1.0f, 0.0f, 1.0f);
			glVertex3f(-0.5f, 0.5f, 0.5f);

            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f, 0.5f);

		glEnd();

        glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 1.0f);
			glVertex3f(-0.5f, 0.5f, 0.5f);

			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0.5f, 0.5f, 0.5f);

			glColor3f(0.0f, 1.0f, 1.0f);
			glVertex3f(0.5f, 0.5f, -0.5f);

			glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(-0.5f, 0.5f, -0.5f);
        glEnd();

        glBegin(GL_QUADS);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f, 0.5f);

			glColor3f(1.0f, 1.0f, 0.0f);
            glVertex3f(0.5f, -0.5f, 0.5f);

			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.5f, -0.5f, -0.5f);

			glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f,  -0.5f);
        glEnd();

    glPopMatrix();

    SwapBuffers(wglGetCurrentDC());

    bBusy = FALSE;
}


