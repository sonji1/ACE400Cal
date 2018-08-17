// MainDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ACE400Cal.h"
#include "MainDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDialog dialog


CMainDialog::CMainDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
	//{{AFX_MSG_MAP(CMainDialog)
	ON_BN_CLICKED(IDC_BUTTON_Calib, OnButtonCalibration)
	ON_BN_CLICKED(IDC_BUTTON_ManualTest, OnButtonManualTest)
	ON_BN_CLICKED(IDC_BUTTON_Config, OnBUTTONConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDialog message handlers

BOOL CMainDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_CalibDlg.Create(IDD_CALIB_DIALOG, this);
	m_CalibDlg.ShowWindow(SW_SHOW);
	m_ManualTestDlg.Create(IDD_MANUAL_TEST_DIALOG, this);   
	m_ManualTestDlg.ShowWindow(SW_HIDE);

	m_ConfigDlg.Create(IDD_CONFIG_DIALOG, this);
	m_ConfigDlg.ShowWindow(SW_HIDE);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMainDialog::OnButtonCalibration() 
{
	m_ManualTestDlg.ShowWindow(SW_HIDE);
	m_ConfigDlg.ShowWindow(SW_HIDE);
	m_CalibDlg.ShowWindow(SW_SHOW);	
}

void CMainDialog::OnButtonManualTest() 
{
	m_CalibDlg.ShowWindow(SW_HIDE);
	m_ConfigDlg.ShowWindow(SW_HIDE);
	m_ManualTestDlg.ShowWindow(SW_SHOW);
	
}


void CMainDialog::OnBUTTONConfig() 
{
	m_CalibDlg.ShowWindow(SW_HIDE);
	m_ManualTestDlg.ShowWindow(SW_HIDE);
	m_ConfigDlg.ShowWindow(SW_SHOW);
	
}

BOOL CMainDialog::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	m_ManualTestDlg.DestroyWindow();

	// m_CalibDlg, m_ConfigDlg의 경우에는 DestroyWindow() function이 없으므로 생략. 
	
	return CDialog::DestroyWindow();
}


void CMainDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
