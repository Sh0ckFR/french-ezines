// appli.h : main header file for the APPLI application
//

#if !defined(AFX_APPLI_H__C88B4F53_902F_45EF_9F03_A84DD75F80A0__INCLUDED_)
#define AFX_APPLI_H__C88B4F53_902F_45EF_9F03_A84DD75F80A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAppliApp:
// See appli.cpp for the implementation of this class
//

class CAppliApp : public CWinApp
{
public:
	CAppliApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppliApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAppliApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPLI_H__C88B4F53_902F_45EF_9F03_A84DD75F80A0__INCLUDED_)
