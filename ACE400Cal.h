// ACE400Cal.h : main header file for the ACE400Cal application
//

#if !defined(AFX_ACE400Cal_H__882AA407_5121_4650_8BB4_1927849024C9__INCLUDED_)
#define AFX_ACE400Cal_H__882AA407_5121_4650_8BB4_1927849024C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "Globals.h"
#include "MacroMotion.h"

#include "DlgNumberKeyPad.h"
#include "System.h"
#include "Model.h"

#include "Error.h"
#include "MainDialog.h"


//*// 
#define WDM		TRUE
#include "CDIO.h"
#pragma comment(lib, "CDIO.lib")
/*///CONTEC ¹öÀü
#define PC98	TRUE
#include "APIDIO.h"
#pragma comment(lib, "APIDIO.lib")
//*/

/////////////////////////////////////////////////////////////////////////////
// CACE400CalApp:
// See ACE400Cal.cpp for the implementation of this class
//

class CACE400CalApp : public CWinApp
{
public:
	CACE400CalApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACE400CalApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CACE400CalApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACE400Cal_H__882AA407_5121_4650_8BB4_1927849024C9__INCLUDED_)
