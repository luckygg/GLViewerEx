// GLViewerEx.cpp : implementation file
//

#include "stdafx.h"
#include "FTech_GLViewerEx.h"
#include "GLViewerEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CGLViewerEx

IMPLEMENT_DYNAMIC(CGLViewerEx, CWnd)

CGLViewerEx::CGLViewerEx()
{
	RegisterWindowClass();

	InitValues();

	m_pMenu		= NULL;
	m_pSubPlt	= NULL;
	m_pSubRes	= NULL;
	m_pSubClr	= NULL;
}

CGLViewerEx::~CGLViewerEx()
{
	if (m_pMenu != NULL)
	{
		delete m_pMenu;
		m_pMenu = NULL;
	}

	if (m_pSubPlt != NULL)
	{
		delete m_pSubPlt;
		m_pSubPlt = NULL;
	}

	if (m_pSubRes != NULL)
	{
		delete m_pSubRes;
		m_pSubRes = NULL;
	}

	if (m_pSubClr != NULL)
	{
		delete m_pSubClr;
		m_pSubClr = NULL;
	}
}


BEGIN_MESSAGE_MAP(CGLViewerEx, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_MENU_LOAD, OnLoadImage)
	ON_COMMAND(ID_MENU_SAVE, OnSaveImage)
	ON_COMMAND_RANGE(ID_MENU_PLT_LINE, ID_MENU_PLT_TRNG, OnSetPlot)
	ON_COMMAND(ID_MENU_PLT_FILL, OnSetFillMode)
	ON_COMMAND_RANGE(ID_MENU_RES_X1, ID_MENU_RES_X003125, OnSetResolution)
	ON_COMMAND_RANGE(ID_MENU_CLR_WHT, ID_MENU_CLR_BLK, OnSetBkgColor)
	ON_COMMAND(ID_MENU_TRUECLR, OnSetTrueColor)
	ON_COMMAND(ID_MENU_INFO, OnShowInformation)
END_MESSAGE_MAP()

void CGLViewerEx::RegisterWindowClass(void)
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc      = ::DefWindowProc;
	wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
	wndcls.hInstance        = hInst;
	wndcls.hIcon            = NULL;
	wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
	wndcls.lpszMenuName     = NULL;
	wndcls.lpszClassName    = GL_VIEWER_WINCLASSNAME;

	AfxRegisterClass(&wndcls);
}

void CGLViewerEx::InitValues()
{
	m_fPosX		= 0.0f;		// X position of model in camera view
	m_fPosY		= 0.0f;		// Y position of model in camera view
	m_fZoom		= 150.0f;	// Zoom on model in camera view
	m_fRotX		= 0.0f;		// Rotation on model in camera view
	m_fRotY		= 0.0f;		// Rotation on model in camera view
	m_fClrBkg	= 0.5;
	m_nRes		= 16;
	
	m_bNew		 = true;
	m_bKeyCtrl	 = false;
	m_bZooming	 = false;
	m_bPopup	 = false;
	m_bTrueColor = true;
	
	m_nDrawMode = GL_LINE_STRIP;
	m_nFillMode = GL_LINE;
}

BOOL CGLViewerEx::OnInitControl(CWnd* pWnd)
{
	if (pWnd == NULL) return FALSE;

	CRect rect;
	GetWindowRect(&rect);
	pWnd->ScreenToClient(&rect);

	UINT id = GetDlgCtrlID();
	BOOL ret = DestroyWindow();
	if (ret == FALSE) return FALSE;

	ret = Create(NULL,NULL,WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rect, pWnd, id);

	return ret;
}

void CGLViewerEx::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	ValidateRect(NULL);

	// Clear color and depth buffer bits
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw OpenGL scene
	OnGLDraw();

	// Swap buffers
	SwapBuffers(GetDC()->GetSafeHdc());
}

void CGLViewerEx::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;

	//// Map the OpenGL coordinates.
	//glViewport(0, 0, cx, cy);

	//// Projection view
	//glMatrixMode(GL_PROJECTION);

	//glLoadIdentity();

	//glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	//// Set our current view perspective
	////gluPerspective(35.0f, (float)cx / (float)cy, 0.01f, 2000.0f);

	//// 
	////if (m_ImgSrc.IsNull() == true) return;

	////int w = m_ImgSrc.GetWidth();
	////int h = m_ImgSrc.GetHeight();

	//gluPerspective(1.0f, (float)cx / (float)cy, 0.00f, 256.0f);
	////gluPerspective(1.0f, (float)w / (float)h, 0.01f, 256.0f);
	////gluPerspective(1.0f, (float)h / (float)w, 0.01f, 2000.0f);

	//// Model view
	//glMatrixMode(GL_MODELVIEW);
}

void CGLViewerEx::OnDraw(CDC *pDC)
{
	// If the current view is perspective...
	glLoadIdentity();
	
	glTranslatef(0.0f, 0.0f, -m_fZoom);
	glTranslatef(m_fPosX, m_fPosY, 0.0f);
	glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);

	InvalidateRect(NULL, FALSE);
}

void CGLViewerEx::OnMouseMove(UINT nFlags, CPoint point)
{
	int diffX = (int)(point.x - m_fLastX);
	int diffY = (int)(point.y - m_fLastY);
	m_fLastX  = (float)point.x;
	m_fLastY  = (float)point.y;

	m_bZooming = false;
	
	// Left mouse button
	
	if ( (nFlags & MK_LBUTTON) && (m_bPopup == false) )
	{
		m_fRotX += (float)0.5f * diffY;

		if ((m_fRotX > 360.0f) || (m_fRotX < -360.0f))
		{
			m_fRotX = 0.0f;
		}

		m_fRotY += (float)0.5f * diffX;

		if ((m_fRotY > 360.0f) || (m_fRotY < -360.0f))
		{
			m_fRotY = 0.0f;
		}
	}

	// Right mouse button
	else if ( (nFlags & MK_RBUTTON) && m_bKeyCtrl)
	{
		m_fZoom -= (float)0.1f * diffY;
		m_bZooming = true;
		//m_fZScale += (m_RightDownPos.y - point.y)/(float)60.0; // 60.0 zoom rate
	}

	// Middle mouse button
	else if (nFlags & MK_MBUTTON)
	{
		m_fPosX += (float)0.005f * diffX;
		m_fPosY -= (float)0.005f * diffY;
	}

	OnDraw(NULL);

	CWnd::OnMouseMove(nFlags, point);
}

void CGLViewerEx::OnGLInit(void)
{
	// Initial Setup:
	//
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32, // bit depth
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16, // z-buffer depth
		0, 0, 0, 0, 0, 0, 0,
	};

	// Get device context only once.
	HDC hdc = GetDC()->m_hDC;

	// Pixel format.
	int nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, nPixelFormat, &pfd);

	// Create the OpenGL Rendering Context.
	HGLRC hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	// Basic Setup:
	//
	// Set color to use when clearing the background.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	// Turn on backface culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Send draw request
	OnDraw(NULL);
}

void CGLViewerEx::OnGLDraw(void)
{
	if (m_ImgSrc.IsNull() == true) return;

	int *p = (int *) m_ImgSrc.GetBits();	// Ptr to bitmap
	unsigned long nx = m_ImgSrc.GetWidth();
	unsigned long ny = m_ImgSrc.GetHeight();
	int r, r2, r3, r4, *q;			
	unsigned long i, j;
	int min, max;
	GLfloat xscale, yscale, zscale;

	min=0;
	max=255;
	xscale = ZOOM_SCALE/(GLfloat)nx;		// Size may have changed
	yscale = ZOOM_SCALE/((GLfloat)(max - min)*(GLfloat)2.0); // Normalize
	zscale = ZOOM_SCALE/(GLfloat)ny;

	// Begin with nice perspective view
	if (m_bNew) 
	{
		glClearColor(m_fClrBkg, m_fClrBkg, m_fClrBkg, 0.0); // Set def. background
		glRotatef(m_fRotX, 0.0, 1.0, 0.0);	// About y-axis
		glRotatef(m_fRotY, 1.0, 0.0, 0.0);	// About x-axis
		m_bNew = false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, m_nFillMode);

	glPushMatrix();
	glEnable(GL_DEPTH_TEST);	/* Enable hidden-surface-removal */

	glTranslatef(-((GLfloat)nx/(GLfloat)2.) * xscale, 
		-((GLfloat)(max + min)/(GLfloat)2.) * yscale, 
		((GLfloat)ny/(GLfloat)2.) * zscale);
	glScalef(xscale, yscale, zscale);
	
	switch (m_nDrawMode)
	{
	case GL_LINE_STRIP :
		for (j = 0; j < ny; j+= m_nRes) {
			p = (int *)m_ImgSrc.GetPixelAddress(0, j); // Inc. addr. for m_nRes
			glBegin(GL_LINE_STRIP);
			for (i = 0; i < nx; i+= m_nRes, p+= m_nRes) 
			{
				r = NINT(RED(*p)*0.299 + GRN(*p)*0.587 + BLU(*p)*0.114);
				MapColor(p);
				glVertex3i(i, r, -(signed)j);
			}
			glEnd();
		}
		break;

	case GL_QUADS :
		glBegin(GL_QUADS);
		for (j = 0; j < ny; j += m_nRes) {
			p = (int *)m_ImgSrc.GetPixelAddress(0, j);
			for (i = 0; i < nx; i += m_nRes, p += m_nRes) {
				r = NINT(RED(*p)*0.299 + GRN(*p)*0.587 + BLU(*p)*0.114);
				if (i + m_nRes < nx) {
					r2 = NINT(RED(*(p+m_nRes))*0.299 
						+ GRN(*(p+m_nRes))*0.587 
						+ BLU(*(p+m_nRes))*0.114);
				}
				else break;

				if (j + m_nRes < ny) {
					q = p + nx*m_nRes;
					r3 = NINT(RED(*q)*0.299 
						+ GRN(*q)*0.587 
						+ BLU(*q)*0.114);
					r4 = NINT(RED(*(q+m_nRes))*0.299
						+ GRN(*(q+m_nRes))*0.587
						+ BLU(*(q+m_nRes))*0.114);
				}
				else break;

				MapColor(p);
				glVertex3i(i, r, -(signed)j);
				MapColor(p+m_nRes);
				glVertex3i(i + m_nRes, r2, -(signed)j);
				MapColor(q+m_nRes);
				glVertex3i(i + m_nRes, r4, -(signed)(j + m_nRes));
				MapColor(q);
				glVertex3i(i, r3, -(signed)(j + m_nRes));
			}
		}
		glEnd();
		break;
	case GL_TRIANGLES :
		glBegin(GL_TRIANGLES);
		for (j = 0; j < ny; j += m_nRes) {
			p = (int *)m_ImgSrc.GetPixelAddress(0, j);
			for (i = 0; i < nx; i += m_nRes, p += m_nRes) {
				r = NINT(RED(*p)*0.299 + GRN(*p)*0.587 + BLU(*p)*0.114);
				if (i + m_nRes < nx) {
					r2 = NINT(RED(*(p+m_nRes))*0.299 
						+ GRN(*(p+m_nRes))*0.587 
						+ BLU(*(p+m_nRes))*0.114);
				}
				else break;

				if (j + m_nRes < ny) {
					q = p + nx*m_nRes;
					r3 = NINT(RED(*q)*0.299 
						+ GRN(*q)*0.587 
						+ BLU(*q)*0.114);
					r4 = NINT(RED(*(q+m_nRes))*0.299
						+ GRN(*(q+m_nRes))*0.587
						+ BLU(*(q+m_nRes))*0.114);
				}
				else break;

				MapColor(p);
				glVertex3i(i, r, -(signed)j);
				MapColor(p+m_nRes);
				glVertex3i(i + m_nRes, r2, -(signed)j);
				MapColor(q);
				glVertex3i(i, r3, -(signed)(j + m_nRes));

				MapColor(p+m_nRes);
				glVertex3i(i + m_nRes, r2, -(signed)j);
				MapColor(q);
				glVertex3i(i, r3, -(signed)(j + m_nRes));
				MapColor(q+m_nRes);
				glVertex3i(i + m_nRes, r4, -(signed)(j + m_nRes));
			}
		}
		glEnd();
		break;
	}
	
	glPopMatrix();
	glFlush();
}

void CGLViewerEx::MapColor(const int *p)
{
	if (m_bTrueColor) 
	{
		glColor3f(RED(*p)/(GLfloat)255., GRN(*p)/(GLfloat)255., BLU(*p)/(GLfloat)255.);
	}
	else 
	{
		byte r = RED(*p); // Red
		glColor3f(1.0*((float)r/256),0.9*((float)r/256)-0.2,0.5*((float)r/256)-0.4);
	}
}

void CGLViewerEx::Convert32Bit()
{
	if (m_ImgSrc.GetBPP() < 8) return;

	byte *t, r, g, b;			
	int *p, *q, *buf;
	unsigned long i, j, nx, ny;
	RGBQUAD *pRGB = new RGBQUAD[256];	// For GetDIBColorTable()

	nx = m_ImgSrc.GetWidth();
	ny = m_ImgSrc.GetHeight();
	unsigned long n = nx * ny;	// No. of pixels

	// Allocate n sized buffer for temp storage
	if (!(buf = (int *)malloc(n * sizeof(int)))) 
		return;

	switch (m_ImgSrc.GetBPP()) 
	{
	case 8:
		if (!(i = GetDIBColorTable(m_ImgSrc.GetDC(), 0, 256, pRGB))) 
		{
			m_ImgSrc.ReleaseDC();
			goto End;
		}
		
		m_ImgSrc.ReleaseDC();
		for (j = 0, q = buf; j < ny; j++) 
		{
			t = (byte *) m_ImgSrc.GetPixelAddress(0, j);	 
			for (i = 0; i < nx; i++, t++, q++) 
			{
				r = pRGB[*t].rgbRed;
				g = pRGB[*t].rgbGreen;
				b = pRGB[*t].rgbBlue;
				*q = BGR(b, g, r);	// CImage is BGR
			}
		}
		break;
	case 24:
		for (j = 0, q = buf; j < ny; j++) 
		{
			// Addr. next row (avoids 24 bit offset and bottom-up calc.)
			t = (byte *) m_ImgSrc.GetPixelAddress(0, j);	 
			for (i = 0; i < nx; i++, t++, q++) 
			{
				b = *t;			// CImage is BGR
				g = *(++t);
				r = *(++t);
				*q = BGR(b, g, r);
			}
		}
		break;
	case 32:	// Just need to make top-down
		for (j = 0, q = buf; j < ny; j++) 
		{
			// Addr. next row (avoids bottom-up calc.)
			p = (int *) m_ImgSrc.GetPixelAddress(0, j);	 
			for (i = 0; i < nx; i++, p++, q++) 
			{
				*q = *p;
			}
		}
		break;
	}

	// Start a new CImage
	m_ImgSrc.Destroy();
	if (!m_ImgSrc.Create(nx, -(int)ny, 32, 0)) 
		goto End;
	
	p = (int *) m_ImgSrc.GetBits();	// Ptr to new bitmap (top-down DIB)
	memcpy_s(p, n * sizeof(int), buf, n * sizeof(int)); // Copy buf to bitmap

End:
	free(buf);
	delete []pRGB;
}

void CGLViewerEx::OnCreateMousePopupMenu()
{
	if (m_pSubPlt != NULL) return;
	m_pSubPlt = new CMenu();
	m_pSubPlt->CreatePopupMenu();
	m_pSubPlt->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_PLT_LINE, _T("Line"));
	m_pSubPlt->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_PLT_QUAD, _T("Quads"));
	m_pSubPlt->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_PLT_TRNG, _T("Triangles"));
	m_pSubPlt->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_PLT_FILL, _T("Polygon Fill"));

	if (m_pSubRes != NULL) return;
	m_pSubRes = new CMenu();
	m_pSubRes->CreatePopupMenu();
	m_pSubRes->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_RES_X1		, _T("x1"));
	m_pSubRes->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_RES_X05	, _T("x0.5"));
	m_pSubRes->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_RES_X025	, _T("x0.25"));
	m_pSubRes->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_RES_X0125	, _T("x0.125"));
	m_pSubRes->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_RES_X00625	, _T("x0.0625"));
	m_pSubRes->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_RES_X003125, _T("x0.03125"));

	if (m_pSubClr != NULL) return;
	m_pSubClr = new CMenu();
	m_pSubClr->CreatePopupMenu();
	m_pSubClr->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_CLR_WHT, _T("White"));
	m_pSubClr->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_CLR_GRY, _T("Gray"));
	m_pSubClr->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_CLR_BLK, _T("Black"));

	if (m_pMenu != NULL) return;
	m_pMenu = new CMenu();
	m_pMenu->CreatePopupMenu();
	
	m_pMenu->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_LOAD	  , _T("Load..."));
	m_pMenu->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_SAVE	  , _T("Save..."));
	m_pMenu->AppendMenuW(MF_POPUP,(UINT)m_pSubPlt->m_hMenu, _T("Plot Mode"));
	m_pMenu->AppendMenuW(MF_POPUP,(UINT)m_pSubRes->m_hMenu, _T("Resolution"));
	m_pMenu->AppendMenuW(MF_POPUP,(UINT)m_pSubClr->m_hMenu, _T("Back Color"));
	m_pMenu->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_TRUECLR, _T("True Color"));
	m_pMenu->InsertMenuW(-1,MF_BYPOSITION, ID_MENU_INFO	  , _T("Info..."));

	m_pSubPlt->CheckMenuItem(ID_MENU_PLT_LINE, MF_CHECKED);
	m_pSubRes->CheckMenuItem(ID_MENU_RES_X003125, MF_CHECKED);
	m_pSubClr->CheckMenuItem(ID_MENU_CLR_GRY, MF_CHECKED);
	m_pMenu->CheckMenuItem(ID_MENU_TRUECLR, MF_CHECKED);

	m_pSubPlt->EnableMenuItem(ID_MENU_PLT_FILL, MF_DISABLED);
}

void CGLViewerEx::CheckMenuItem(int nID, bool bPlt, bool bRes, bool bClr)
{
	if (bPlt == true)
	{
		for (int i=ID_MENU_PLT_LINE; i<ID_MENU_PLT_TRNG+1; i++)
		{
			if (i == nID)
				m_pSubPlt->CheckMenuItem(i, MF_CHECKED);
			else
				m_pSubPlt->CheckMenuItem(i, MF_UNCHECKED);
		}
	}
	else if (bRes == true)
	{
		for (int i=ID_MENU_RES_X1; i<ID_MENU_RES_X003125+1; i++)
		{
			if (i == nID)
				m_pSubRes->CheckMenuItem(i, MF_CHECKED);
			else
				m_pSubRes->CheckMenuItem(i, MF_UNCHECKED);
		}
	}
	else if (bClr == true)
	{
		for (int i=ID_MENU_CLR_WHT; i<ID_MENU_CLR_BLK+1; i++)
		{
			if (i == nID)
				m_pSubClr->CheckMenuItem(i, MF_CHECKED);
			else
				m_pSubClr->CheckMenuItem(i, MF_UNCHECKED);
		}
	}
}

void CGLViewerEx::PreSubclassWindow()
{
	ModifyStyle( 0, SS_NOTIFY );
	OnCreateMousePopupMenu();

	OnGLInit();

	CWnd::PreSubclassWindow();
}

void CGLViewerEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_CONTROL :
		m_bKeyCtrl = true;
		break;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGLViewerEx::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_CONTROL :
		m_bKeyCtrl = false;
		break;
	}

	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CGLViewerEx::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	if (m_bZooming == false)
	{
		m_pMenu->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);
		m_bPopup = true;
	}
}

void CGLViewerEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bPopup = false;

	CWnd::OnLButtonUp(nFlags, point);
}

bool CGLViewerEx::OnLoad(CString strPath)
{
	if (strPath.IsEmpty()) return false;

	if (m_ImgSrc.IsNull() == false)
		m_ImgSrc.Destroy();

	m_ImgSrc.Load(strPath);

	Convert32Bit();

	InitValues();

	CheckMenuItem(ID_MENU_PLT_LINE, true, false, false);
	CheckMenuItem(ID_MENU_RES_X003125, false, true, false);
	CheckMenuItem(ID_MENU_CLR_GRY, false, false, true);

	m_bFillMode = false;
	m_pMenu->CheckMenuItem(ID_MENU_PLT_FILL, MF_UNCHECKED);
	m_pSubPlt->EnableMenuItem(ID_MENU_PLT_FILL, MF_DISABLED);
	m_pMenu->CheckMenuItem(ID_MENU_TRUECLR, MF_CHECKED);

	//////////////////////////////////////////////////////////////////////////
	CRect rect;
	GetClientRect(&rect);
	// Map the OpenGL coordinates.
	glViewport(0, 0, rect.Width(), rect.Height());

	// Projection view
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	int w = m_ImgSrc.GetWidth();
	int h = m_ImgSrc.GetHeight();

	gluPerspective(1.0f, (float)w / (float)h, 0.00f, 256.0f);

	// Model view
	glMatrixMode(GL_MODELVIEW);
	//////////////////////////////////////////////////////////////////////////

	Invalidate();

	return true;
}

void CGLViewerEx::OnLoadImage()
{
	CString strFilter = _T("All Files (*.*)|*.*||");

	CFileDialog FileDlg(TRUE, _T(".BMP"), NULL, 0, strFilter);

	if( FileDlg.DoModal() == IDOK )
	{
		bool ret = OnLoad(FileDlg.GetPathName());
		if (ret == false) return;

		NMHDR nmHdr;
		::ZeroMemory(&nmHdr, sizeof(NMHDR));

		nmHdr.hwndFrom = m_hWnd;
		nmHdr.idFrom = GetDlgCtrlID();
		nmHdr.code = WM_LOAD_IMAGE_MESSAGE;
		GetParent()->SendMessage(WM_NOTIFY, (LPARAM)NULL, (WPARAM)&nmHdr);
	}
}

void CGLViewerEx::OnSave(CString strPath)
{
	BITMAPFILEHEADER    BMFH;
	BITMAPINFOHEADER    BMIH;
	
	unsigned long dwQuadrupleWidth = 0;

	GLbyte *pPixelData = NULL;

	CRect rect;
	GetClientRect(&rect);           

	int nWidth = rect.Width();
	int nHeight = rect.Height();

	dwQuadrupleWidth = ( nWidth % 4 ) ? ( ( nWidth ) + ( 4 - ( nWidth % 4 ) ) ) : ( nWidth );

	BMFH.bfType  = 0x4D42;
	BMFH.bfSize  = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + ( dwQuadrupleWidth * 3 * nHeight );
	BMFH.bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );
	BMIH.biSize             = sizeof( BITMAPINFOHEADER );
	BMIH.biWidth            = nWidth;
	BMIH.biHeight           = nHeight;
	BMIH.biPlanes           = 1;
	BMIH.biBitCount         = 24;
	BMIH.biCompression      = BI_RGB;
	BMIH.biSizeImage        = dwQuadrupleWidth * 3 * nHeight;
	BMIH.biXPelsPerMeter    = 0;
	BMIH.biYPelsPerMeter    = 0;
	BMIH.biClrUsed          = 0;
	BMIH.biClrImportant     = 0;

	pPixelData = new GLbyte[ dwQuadrupleWidth * 3 * nHeight ];

	glReadPixels(0, 0, nWidth, nHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

	wchar_t* wchar_str;     
	char*    char_str;      
	int      char_str_len;  
	wchar_str = strPath.GetBuffer(strPath.GetLength());

	char_str_len = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
	char_str = new char[char_str_len];
	WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, char_str, char_str_len, 0,0);  

	FILE *outFile;
	fopen_s(&outFile, char_str, "wb");
	if( outFile == NULL )
	{
		//에러 처리

		delete []char_str;
		char_str = NULL;

		delete []pPixelData;
		pPixelData = NULL;

		return;
	}

	fwrite( &BMFH, sizeof( char ), sizeof(BITMAPFILEHEADER), outFile );
	fwrite( &BMIH, sizeof( char ), sizeof(BITMAPINFOHEADER), outFile );
	fwrite( pPixelData, sizeof( unsigned char ), BMIH.biSizeImage, outFile );

	fclose( outFile );

	delete []char_str;
	char_str = NULL;

	delete []pPixelData;
	pPixelData = NULL;
}

void CGLViewerEx::OnSaveImage()
{
	if (m_ImgSrc.IsNull()) return;

	CString strFilter = _T("JPG Files (*.jpg)|*.jpg||");

	CFileDialog FileDlg(FALSE, _T(".jpg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	if( FileDlg.DoModal() == IDOK )
	{
		OnSave(FileDlg.GetPathName());
	}
}

void CGLViewerEx::OnSetPlot(UINT wParam)
{
	int nMenuID = (int)wParam;
	switch(nMenuID)
	{
	case ID_MENU_PLT_LINE:
		m_nDrawMode = GL_LINE_STRIP;
		m_nFillMode = GL_LINE;
		m_bFillMode = false;
		m_pSubPlt->CheckMenuItem(ID_MENU_PLT_FILL, MF_UNCHECKED);
		m_pSubPlt->EnableMenuItem(ID_MENU_PLT_FILL, MF_DISABLED);
		break;
	case ID_MENU_PLT_QUAD:
		m_nDrawMode = GL_QUADS;
		m_pSubPlt->EnableMenuItem(ID_MENU_PLT_FILL, MF_ENABLED);
		break;
	case ID_MENU_PLT_TRNG:
		m_nDrawMode = GL_TRIANGLES;
		m_pSubPlt->EnableMenuItem(ID_MENU_PLT_FILL, MF_ENABLED);
		break;
	}

	CheckMenuItem(nMenuID,true,false,false);
	m_bPopup = false;
	Invalidate();
}

void CGLViewerEx::OnSetResolution(UINT wParam)
{
	int nMenuID = (int)wParam;
	switch(nMenuID)
	{
	case ID_MENU_RES_X1:
		m_nRes = 1;
		break;
	case ID_MENU_RES_X05:
		m_nRes = 2;
		break;
	case ID_MENU_RES_X025:
		m_nRes = 4;
		break;
	case ID_MENU_RES_X0125:
		m_nRes = 8;
		break;
	case ID_MENU_RES_X00625:
		m_nRes = 16;
		break;
	case ID_MENU_RES_X003125:
		m_nRes = 32;
		break;
	}

	CheckMenuItem(nMenuID,false,true,false);
	m_bPopup = false;
	Invalidate();
}

void CGLViewerEx::OnSetBkgColor(UINT wParam)
{
	int nMenuID = (int)wParam;
	switch(nMenuID)
	{
	case ID_MENU_CLR_WHT:
		m_fClrBkg = 1.0;
		break;
	case ID_MENU_CLR_GRY:
		m_fClrBkg = 0.5;
		break;
	case ID_MENU_CLR_BLK:
		m_fClrBkg = 0.0;
		break;
	}

	CheckMenuItem(nMenuID,false,false,true);
	m_bPopup = false;
	m_bNew = true;

	Invalidate();
}

void CGLViewerEx::OnSetFillMode()
{
	if (m_bFillMode == true)
	{
		m_bFillMode = false;
		m_nFillMode = GL_LINE;
		m_pMenu->CheckMenuItem(ID_MENU_PLT_FILL, MF_UNCHECKED);
	}
	else
	{
		m_bFillMode = true;
		m_nFillMode = GL_FILL;
		m_pMenu->CheckMenuItem(ID_MENU_PLT_FILL, MF_CHECKED);
	}

	m_bPopup = false;
	Invalidate();
}

void CGLViewerEx::OnSetTrueColor()
{
	if (m_bTrueColor == true)
	{
		m_bTrueColor = false;
		m_pMenu->CheckMenuItem(ID_MENU_TRUECLR, MF_UNCHECKED);
	}
	else
	{
		m_bTrueColor = true;
		m_pMenu->CheckMenuItem(ID_MENU_TRUECLR, MF_CHECKED);
	}

	m_bPopup = false;
	Invalidate();
}

void CGLViewerEx::OnShowInformation()
{
	CString strText=_T("");

	strText = _T("How to use...\r\n\
L-Button Moving : Rotation\r\n\
Ctrl+R-Button Moving : Zooming\r\n\
Wheel Button Moving: Moving\r\n\
R-Button : Popup Menu");
	MessageBox(strText,_T("GLViewerEx Information"), MB_ICONASTERISK);
}