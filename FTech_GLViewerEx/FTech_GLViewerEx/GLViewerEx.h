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
	CImage	m_ImgSrc;		// 이미지.
	CMenu	*m_pMenu;		//마우스 우 클릭 메뉴.
	CMenu	*m_pSubPlt;		//마우스 우 클릭 Plot Mode 세부 메뉴.
	CMenu	*m_pSubRes;		//마우스 우 클릭 해상도 세부 메뉴.
	CMenu	*m_pSubClr;		//마우스 우 클릭 배경 색상 세부 메뉴.

	float	m_fLastX;		// 마지막 마우스 X 좌표.
	float	m_fLastY;		// 마지막 마우스 Y 좌표.
	float	m_fPosX;		// 화면 이동 X 좌표.
	float	m_fPosY;		// 화면 이동 Y 좌표.
	float	m_fZoom;		// 카메라 뷰의 Z 축 변수.
	float	m_fRotX;		// 화면 X 축 회전 변수.
	float	m_fRotY;		// 화면 Y 축 회전 변수.
	
	float	m_fClrBkg;		// 배경 색상.
	int		m_nRes;			// Resolution 단계.
	int		m_nDrawMode;	// Plot Mode 종류. (Line / Quads / Triangles)
	int		m_nFillMode;	// 면을 채우는 모드. (Line / Fill)
	bool	m_bTrueColor;	// True Color 또는 임의의 Color 이미지로 표현 할 것인지 선택 변수.
	bool	m_bFillMode;	// Plot Mode (Quads / Triangles)에서 면에 색상을 채울것인지 확인 용.
	bool	m_bKeyCtrl;		// Ctrl Key가 눌렸는지 확인 용.
	bool	m_bZooming;		// Zooming 중에는 RButtonUp이 되었을 때 Popup 메뉴를 막기 위한 용도.
	bool	m_bPopup;		// Popup 메뉴 LButton이 클릭 되면 rotation 되므로 막기 위한 용도.
	bool	m_bNew;			// 로딩 후 첫 호출인지 확인.	

private :
	void RegisterWindowClass(void);
	void CheckMenuItem(int nID, bool bPlt, bool bRes, bool bClr); //Mouse Right 버튼 메뉴.
	void OnCreateMousePopupMenu(); //Popup menu 생성.
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


