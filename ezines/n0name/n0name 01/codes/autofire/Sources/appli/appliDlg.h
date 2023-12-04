// appliDlg.h : header file
//

#if !defined(AFX_APPLIDLG_H__98C39168_E832_448F_9322_4DDAAD263F48__INCLUDED_)
#define AFX_APPLIDLG_H__98C39168_E832_448F_9322_4DDAAD263F48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAppliDlg dialog

class CAppliDlg : public CDialog
{
// Construction
public:
	CAppliDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAppliDlg)
	enum { IDD = IDD_APPLI_DIALOG };
	CString	m_emplacement;
	CString	m_fullname;
	CString	m_shortname;
	CString	m_write;
	CString	m_read;
	CString	m_dos;
	CString	m_ioctrl;
	int		m_iotype;
	int		m_startupmode;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppliDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAppliDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnEnregistrement();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCancelMode();
	afx_msg void OnSuppression();
	afx_msg void OnLancement();
	afx_msg void OnArret();
	afx_msg void OnWrite();
	afx_msg void OnRead();
	afx_msg void OnEnvoi();
	afx_msg void OnStatus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPLIDLG_H__98C39168_E832_448F_9322_4DDAAD263F48__INCLUDED_)
