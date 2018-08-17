//{{AFX_INCLUDES()
#include "sspnctrl.h"
//}}AFX_INCLUDES
#if !defined(AFX_DLGNUMBERKEYPAD_H__43CD8555_86D8_491C_9DD0_5AB1C7BD72FB__INCLUDED_)
#define AFX_DLGNUMBERKEYPAD_H__43CD8555_86D8_491C_9DD0_5AB1C7BD72FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNumberKeyPad.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNumberKeyPad dialog

class CDlgNumberKeyPad : public CDialog
{
	BOOL	m_bFirst, m_bInteger;
	int		m_nCursorPos, m_nNumLength;
	double	m_dCalValue, m_dBackupValue;
	double	m_dNumData;
	double	m_dMaxLimitValue, m_dMinLimitValue;
	CString m_strPanelBit[12];

// Construction
public:
	CDlgNumberKeyPad(CWnd* pParent = NULL);   // standard constructor

	BOOL	m_bSignVel;

	double	GetValue() { return m_dCalValue; }
	BOOL	SetValue(int nValue, double dMinLimitValue=-100000000, double dMaxLimitValue=100000000);
	BOOL	SetValue(double dValue, double dMinLimitValue=-100000000, double dMaxLimitValue=100000000);
	void	SetLimitValue(double dMinValue, double dMaxValue);

protected:
	void	DisplayNum();
	void	SetCursorColor(int nNo, long lColor);
	void	AddBit(CString strBit);
	void	BackspaceBit();
	void	DeleteBit();

// Dialog Data
	//{{AFX_DATA(CDlgNumberKeyPad)
	enum { IDD = IDD_DIALOG_NUM_PAD };
	CSSPNCtrl	m_NumPadData;
	CSSPNCtrl	m_NumPos0;
	CSSPNCtrl	m_NumPos1;
	CSSPNCtrl	m_NumPos10;
	CSSPNCtrl	m_NumPos11;
	CSSPNCtrl	m_NumPos2;
	CSSPNCtrl	m_NumPos3;
	CSSPNCtrl	m_NumPos4;
	CSSPNCtrl	m_NumPos5;
	CSSPNCtrl	m_NumPos6;
	CSSPNCtrl	m_NumPos7;
	CSSPNCtrl	m_NumPos8;
	CSSPNCtrl	m_NumPos9;
	CSSPNCtrl	m_sMaxLimit;
	CSSPNCtrl	m_sMinLimit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNumberKeyPad)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNumberKeyPad)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonKeypad0();
	afx_msg void OnButtonBackspace();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonClr();
	afx_msg void OnButtonCursorBack();
	afx_msg void OnButtonCursorFwd();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonDot();
	afx_msg void OnButtonKeypad1();
	afx_msg void OnButtonKeypad2();
	afx_msg void OnButtonKeypad3();
	afx_msg void OnButtonKeypad4();
	afx_msg void OnButtonKeypad5();
	afx_msg void OnButtonKeypad6();
	afx_msg void OnButtonKeypad7();
	afx_msg void OnButtonKeypad8();
	afx_msg void OnButtonKeypad9();
	afx_msg void OnButtonSign();
	afx_msg void OnButtonEnter();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CDlgNumberKeyPad NKEYPAD;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNUMBERKEYPAD_H__43CD8555_86D8_491C_9DD0_5AB1C7BD72FB__INCLUDED_)
