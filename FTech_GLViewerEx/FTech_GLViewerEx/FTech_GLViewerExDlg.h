
// FTech_GLViewerExDlg.h : header file
//

#pragma once

#include "GLViewerEx.h"

// CFTech_GLViewerExDlg dialog
class CFTech_GLViewerExDlg : public CDialogEx
{
// Construction
public:
	CFTech_GLViewerExDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FTech_GLViewerEx_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private :
	CGLViewerEx m_wndGLView;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
