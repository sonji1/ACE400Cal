#if !defined(AFX_MAINDIALOG_H__1CD34A10_F329_4723_A436_131F180AF166__INCLUDED_)
#define AFX_MAINDIALOG_H__1CD34A10_F329_4723_A436_131F180AF166__INCLUDED_

#include "ConfigDialog.h"	// Added by ClassView
#include "CalibDialog.h"	// Added by ClassView
#include "ManualTestDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainDialog dialog

class CMainDialog : public CDialog
{
// Construction
public:
	CConfigDialog		m_ConfigDlg;
	CManualTestDlg		m_ManualTestDlg;
	CCalibDialog		m_CalibDlg;
	CMainDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainDialog)
	enum { IDD = IDD_MAIN_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDialog)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonCalibration();
	afx_msg void OnButtonManualTest();
	afx_msg void OnBUTTONConfig();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDIALOG_H__1CD34A10_F329_4723_A436_131F180AF166__INCLUDED_)
