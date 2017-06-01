//----------------------------------------------------------
// GLViewerEx Class
//----------------------------------------------------------
// Programmed by William Kim
//----------------------------------------------------------
// Last Update : 2017-06-01 18:42
// Modified by William Kim
//----------------------------------------------------------
#pragma once

#ifdef _WIN64
	#include "./include/freeglut.h"
	#pragma comment (lib,"freeglut.lib")
#else
	#include <gl/gl.h>
	#include <gl/glu.h>
	#include <gl/glut.h>
	#pragma comment (lib,"glut32")
#endif

#define WM_LOAD_IMAGE_MESSAGE	WM_USER+2
#define GL_VIEWER_WINCLASSNAME L"GL_VIEWEREX_CLASS"

#define USER_ID				WM_USER+200
//----- Popup Menu -----//

// Load Image
#define ID_MENU_LOAD		USER_ID+1
// Save Image
#define ID_MENU_SAVE		USER_ID+2
// Plot Mode
#define ID_MENU_PLT_LINE	USER_ID+10
#define ID_MENU_PLT_QUAD	USER_ID+11
#define ID_MENU_PLT_TRNG	USER_ID+12
#define ID_MENU_PLT_FILL	USER_ID+13
// Plot Resolution
#define ID_MENU_RES_X1		USER_ID+20
#define ID_MENU_RES_X05		USER_ID+21
#define ID_MENU_RES_X025	USER_ID+22
#define ID_MENU_RES_X0125	USER_ID+23
#define ID_MENU_RES_X00625	USER_ID+24
#define ID_MENU_RES_X003125	USER_ID+25
// Background Color
#define ID_MENU_CLR_WHT		USER_ID+30
#define ID_MENU_CLR_GRY		USER_ID+31
#define ID_MENU_CLR_BLK		USER_ID+32
// True Color
#define ID_MENU_TRUECLR		USER_ID+40
// Information
#define ID_MENU_INFO		USER_ID+50
//----------------------//

#define ZOOM_SCALE 1
#include <atlimage.h>		// CImage class
#include <comdef.h>			// _com_error Class

// Macros
#define NINT(f)   ((f >= 0) ? (int)(f + .5) : (int)(f - .5))
#define THRESH(d) ((d > 255) ? 255 : ((d < 0) ? 0 : d))

// CImage is BGR so defs of red and blue are switched from GetXValue() defs
#define RED(rgb)	(LOBYTE((rgb) >> 16))
#define GRN(rgb)	(LOBYTE(((WORD)(rgb)) >> 8))
#define BLU(rgb)	(LOBYTE(rgb))
#define BGR(b,g,r)	RGB(b,g,r)

// CGLViewerEx

class CGLViewerEx : public CWnd
{
	DECLARE_DYNAMIC(CGLViewerEx)

public:
	CGLViewerEx();
	virtual ~CGLViewerEx();

public:
	BOOL OnInitControl(CWnd* pWnd);

private:
	CImage	m_ImgSrc;		// �̹���.
	CMenu	*m_pMenu;		//���콺 �� Ŭ�� �޴�.
	CMenu	*m_pSubPlt;		//���콺 �� Ŭ�� Plot Mode ���� �޴�.
	CMenu	*m_pSubRes;		//���콺 �� Ŭ�� �ػ� ���� �޴�.
	CMenu	*m_pSubClr;		//���콺 �� Ŭ�� ��� ���� ���� �޴�.

	float	m_fLastX;		// ������ ���콺 X ��ǥ.
	float	m_fLastY;		// ������ ���콺 Y ��ǥ.
	float	m_fPosX;		// ȭ�� �̵� X ��ǥ.
	float	m_fPosY;		// ȭ�� �̵� Y ��ǥ.
	float	m_fZoom;		// ī�޶� ���� Z �� ����.
	float	m_fRotX;		// ȭ�� X �� ȸ�� ����.
	float	m_fRotY;		// ȭ�� Y �� ȸ�� ����.
	
	float	m_fClrBkg;		// ��� ����.
	int		m_nRes;			// Resolution �ܰ�.
	int		m_nDrawMode;	// Plot Mode ����. (Line / Quads / Triangles)
	int		m_nFillMode;	// ���� ä��� ���. (Line / Fill)
	bool	m_bTrueColor;	// True Color �Ǵ� ������ Color �̹����� ǥ�� �� ������ ���� ����.
	bool	m_bFillMode;	// Plot Mode (Quads / Triangles)���� �鿡 ������ ä������� Ȯ�� ��.
	bool	m_bKeyCtrl;		// Ctrl Key�� ���ȴ��� Ȯ�� ��.
	bool	m_bZooming;		// Zooming �߿��� RButtonUp�� �Ǿ��� �� Popup �޴��� ���� ���� �뵵.
	bool	m_bPopup;		// Popup �޴� LButton�� Ŭ�� �Ǹ� rotation �ǹǷ� ���� ���� �뵵.
	bool	m_bNew;			// �ε� �� ù ȣ������ Ȯ��.	

private :
	void RegisterWindowClass(void);
	void CheckMenuItem(int nID, bool bPlt, bool bRes, bool bClr); //Mouse Right ��ư �޴�.
	void OnCreateMousePopupMenu(); //Popup menu ����.
	void MapColor(const int *p);
	void Convert32Bit();
	bool OnLoad(CString strPath);
	void OnSave(CString strPath);
	void OnGLDraw();
	void OnGLInit();
	void InitValues();

private :
	virtual void PreSubclassWindow();

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg	void OnDraw(CDC *pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetPlot(UINT wParam);
	afx_msg void OnSetResolution(UINT wParam);
	afx_msg void OnSetBkgColor(UINT wParam);
	afx_msg void OnSetTrueColor();
	afx_msg void OnShowInformation();
	afx_msg void OnLoadImage();
	afx_msg void OnSaveImage();
	afx_msg void OnSetFillMode();

	DECLARE_MESSAGE_MAP()
};


