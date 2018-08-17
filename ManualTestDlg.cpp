// ManualTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ACE400Cal.h"
#include "ManualTestDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualTestDlg dialog

CManualTestDlg::CManualTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManualTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualTestDlg)
	m_nLV1 = 0;
	m_nLV2 = 0;
	m_nHV1 = 0;
	m_nHV2 = 0;
	m_nI = 0;
	m_nBPin = 0;
	m_nCPin = 0;
	m_nBBox = 0;
	m_nCBox = 0;
	m_nBPCBEnable = 0;
	m_nCPCBEnable = 0;
	m_nBCPLD = 0;
	m_nCCPLD = 0;
	m_nBPCB0 = FALSE;
	m_nBPCB1 = FALSE;
	m_nBPCB2 = FALSE;
	m_nBPCB3 = FALSE;
	m_nBPin0 = FALSE;
	m_nBPin1 = FALSE;
	m_nBPin2 = FALSE;
	m_nBPin3 = FALSE;
	m_nBPin4 = FALSE;
	m_nBPin5 = FALSE;
	m_nCPCB0 = FALSE;
	m_nCPCB1 = FALSE;
	m_nCPCB2 = FALSE;
	m_nCPCB3 = FALSE;
	m_nCPin0 = FALSE;
	m_nCPin1 = FALSE;
	m_nCPin2 = FALSE;
	m_nCPin3 = FALSE;
	m_nCPin4 = FALSE;
	m_nCPin5 = FALSE;
	m_nSparkOn = FALSE;
	m_nSpark = 0;
	m_bIHighGainMode = FALSE;
	m_bDCOut = FALSE;
	m_ptBTR = 1;
	m_ptBRY = 1;
	m_ptCTR = 1;
	m_ptCRY = 1;
	m_bVBGuardOn = FALSE;
	m_bVCGuardOn = FALSE;
	m_nBCCV = 0;
	m_nCCCV = 0;
	m_nBCVC = 0;
	m_nCCVC = 0;
	m_bPulse = FALSE;
	m_nADC_B = -1;
	m_nADC_A = -1;
	m_nHVOGain = -1;
	m_n4WGainMode = -1;
	m_nICC = -1;		//son 170529
	m_nVB = -1;
	m_nPulseFreq = -1;
	m_nPulseMea = -1;
	m_nPulseVolt = -1;
	m_adcHV1 = 0;
	m_adcHV2 = 0;
	m_adcLV1 = 0;
	m_adcLV2 = 0;
	m_nIRng = -1;
	m_adcSpark = 0;
	m_nHLVC = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CManualTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualTestDlg)
	DDX_Control(pDX, IDC_LIST_MSG, m_listMsg);
	DDX_Control(pDX, IDC_LIST_BPOINT, m_listBPoint);
	DDX_Control(pDX, IDC_LIST_CPOINT, m_listCPoint);
	DDX_Text(pDX, IDC_EDIT_LV1, m_nLV1);
	DDX_Text(pDX, IDC_EDIT_LV2, m_nLV2);
	DDX_Text(pDX, IDC_EDIT_HV1, m_nHV1);
	DDX_Text(pDX, IDC_EDIT_HV2, m_nHV2);
	DDX_Text(pDX, IDC_EDIT_I, m_nI);
	DDX_Text(pDX, IDC_EDIT_BPIN, m_nBPin);
	DDX_Text(pDX, IDC_EDIT_CPIN, m_nCPin);
	DDX_Radio(pDX, IDC_RADIO_BBOXA, m_nBBox);
	DDX_Radio(pDX, IDC_RADIO_CBOXA, m_nCBox);
	DDX_Radio(pDX, IDC_RADIO_BPCB_ENABLE0, m_nBPCBEnable);
	DDX_Radio(pDX, IDC_RADIO_CPCB_ENABLE0, m_nCPCBEnable);
	DDX_Radio(pDX, IDC_RADIO_BCPLD0, m_nBCPLD);
	DDX_Radio(pDX, IDC_RADIO_CCPLD0, m_nCCPLD);
	DDX_Check(pDX, IDC_CHECK_BPCB0, m_nBPCB0);
	DDX_Check(pDX, IDC_CHECK_BPCB1, m_nBPCB1);
	DDX_Check(pDX, IDC_CHECK_BPCB2, m_nBPCB2);
	DDX_Check(pDX, IDC_CHECK_BPCB3, m_nBPCB3);
	DDX_Check(pDX, IDC_CHECK_BPIN0, m_nBPin0);
	DDX_Check(pDX, IDC_CHECK_BPIN1, m_nBPin1);
	DDX_Check(pDX, IDC_CHECK_BPIN2, m_nBPin2);
	DDX_Check(pDX, IDC_CHECK_BPIN3, m_nBPin3);
	DDX_Check(pDX, IDC_CHECK_BPIN4, m_nBPin4);
	DDX_Check(pDX, IDC_CHECK_BPIN5, m_nBPin5);
	DDX_Check(pDX, IDC_CHECK_CPCB0, m_nCPCB0);
	DDX_Check(pDX, IDC_CHECK_CPCB1, m_nCPCB1);
	DDX_Check(pDX, IDC_CHECK_CPCB2, m_nCPCB2);
	DDX_Check(pDX, IDC_CHECK_CPCB3, m_nCPCB3);
	DDX_Check(pDX, IDC_CHECK_CPIN0, m_nCPin0);
	DDX_Check(pDX, IDC_CHECK_CPIN1, m_nCPin1);
	DDX_Check(pDX, IDC_CHECK_CPIN2, m_nCPin2);
	DDX_Check(pDX, IDC_CHECK_CPIN3, m_nCPin3);
	DDX_Check(pDX, IDC_CHECK_CPIN4, m_nCPin4);
	DDX_Check(pDX, IDC_CHECK_CPIN5, m_nCPin5);
	DDX_Control(pDX, IDC_LABEL_I_AMP, m_lbIamp);
	DDX_Control(pDX, IDC_LABEL_V_VOLT, m_lbVvolt);
	DDX_Control(pDX, IDC_LABEL_R_OHM, m_lbRohm);
	DDX_Check(pDX, IDC_CHECK_SPARK_ON, m_nSparkOn);
	DDX_Text(pDX, IDC_EDIT_SPARK, m_nSpark);
	DDX_Check(pDX, IDC_CHECK_I_HIGH_GAIN, m_bIHighGainMode);
	DDX_Check(pDX, IDC_CHECK_DC_OUT, m_bDCOut);
	DDX_Text(pDX, IDC_EDIT_BPOINT_TR, m_ptBTR);
	DDX_Text(pDX, IDC_EDIT_BPOINT_RY, m_ptBRY);
	DDX_Text(pDX, IDC_EDIT_CPOINT_TR, m_ptCTR);
	DDX_Text(pDX, IDC_EDIT_CPOINT_RY, m_ptCRY);
	DDX_Check(pDX, IDC_CHECK_VB_GUARD_ON, m_bVBGuardOn);
	DDX_Check(pDX, IDC_CHECK_VC_GUARD_ON, m_bVCGuardOn);
	DDX_Text(pDX, IDC_EDIT_BPOINT_CC_V, m_nBCCV);
	DDX_Text(pDX, IDC_EDIT_CPOINT_CC_V, m_nCCCV);
	DDX_Radio(pDX, IDC_RADIO_BPOINT_CV, m_nBCVC);
	DDX_Radio(pDX, IDC_RADIO_CPOINT_CV, m_nCCVC);
	DDX_Check(pDX, IDC_CHECK_PULSE, m_bPulse);
	DDX_Radio(pDX, IDC_RADIO_VCFFT, m_nADC_B);
	DDX_Radio(pDX, IDC_RADIO_PFT_FAST, m_nADC_A);
	DDX_Radio(pDX, IDC_RADIO_HVO_GAIN1, m_nHVOGain);
	DDX_Radio(pDX, IDC_RADIO_4WGAIN_S1000, m_n4WGainMode);
	DDX_Radio(pDX, IDC_RADIO_I_CC, m_nICC);
	DDX_Radio(pDX, IDC_RADIO_LV1, m_nVB);
	DDX_Radio(pDX, IDC_RADIO_PULSE_FREQ, m_nPulseFreq);
	DDX_Radio(pDX, IDC_RADIO_PULSE_MEASURE_CAV0, m_nPulseMea);
	DDX_Radio(pDX, IDC_RADIO_PULSE_VOLT, m_nPulseVolt);
	DDX_Text(pDX, IDC_EDIT_HV1_DA, m_adcHV1);
	DDX_Text(pDX, IDC_EDIT_HV2_DA, m_adcHV2);
	DDX_Text(pDX, IDC_EDIT_LV1_DA, m_adcLV1);
	DDX_Text(pDX, IDC_EDIT_LV2_DA, m_adcLV2);
	DDX_Radio(pDX, IDC_RADIO_I_RNG0, m_nIRng);
	DDX_Text(pDX, IDC_EDIT_SPARK_DA, m_adcSpark);
	DDX_Radio(pDX, IDC_RADIO_HVC, m_nHLVC);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CManualTestDlg, CDialog)
	//{{AFX_MSG_MAP(CManualTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LV1_SET, OnButtonLv1Set)
	ON_BN_CLICKED(IDC_BUTTON_LV2_SET, OnButtonLv2Set)
	ON_BN_CLICKED(IDC_BUTTON_HV1_SET, OnButtonHv1Set)
	ON_BN_CLICKED(IDC_BUTTON_HV2_SET, OnButtonHv2Set)
	ON_EN_SETFOCUS(IDC_EDIT_LV1, OnSetfocusEditLv1)
	ON_EN_SETFOCUS(IDC_EDIT_LV2, OnSetfocusEditLv2)
	ON_EN_SETFOCUS(IDC_EDIT_HV1, OnSetfocusEditHv1)
	ON_EN_SETFOCUS(IDC_EDIT_HV2, OnSetfocusEditHv2)
	ON_EN_SETFOCUS(IDC_EDIT_I, OnSetfocusEditI)
	ON_BN_CLICKED(IDC_BUTTON_I_SET, OnButtonISet)
	ON_BN_CLICKED(IDC_RADIO_IFFT, OnRadioIfft)
	ON_BN_CLICKED(IDC_RADIO_ISFT, OnRadioIsft)
	ON_BN_CLICKED(IDC_RADIO_VBFT, OnRadioVbft)
	ON_BN_CLICKED(IDC_RADIO_VCFFT, OnRadioVcfft)
	ON_BN_CLICKED(IDC_RADIO_VCSFT, OnRadioVcsft)
	ON_LBN_DBLCLK(IDC_LIST_BPOINT, OnDblclkListBpoint)
	ON_EN_SETFOCUS(IDC_EDIT_BPIN, OnSetfocusEditBpin)
	ON_LBN_DBLCLK(IDC_LIST_CPOINT, OnDblclkListCpoint)
	ON_EN_SETFOCUS(IDC_EDIT_CPIN, OnSetfocusEditCpin)
	ON_BN_CLICKED(IDC_RADIO_LV1, OnRadioLv1)
	ON_BN_CLICKED(IDC_RADIO_LV2, OnRadioLv2)
	ON_BN_CLICKED(IDC_RADIO_HV1, OnRadioHv1)
	ON_BN_CLICKED(IDC_RADIO_HV2, OnRadioHv2)
	ON_BN_CLICKED(IDC_BUTTON_BREAK, OnButtonBreak)
	ON_BN_CLICKED(IDC_BUTTON_POINT_SET, OnButtonPointSet)
	ON_BN_CLICKED(IDC_BUTTON_BOX_RST, OnButtonBoxRst)
	ON_BN_CLICKED(IDC_BUTTON_AD_READ, OnButtonAdRead)
	ON_EN_SETFOCUS(IDC_EDIT_QM_BPOINT, OnSetfocusEditQmBpoint)
	ON_EN_SETFOCUS(IDC_EDIT_QM_CPOINT, OnSetfocusEditQmCpoint)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_VB_OUT_SET, OnButtonVbOutSet)
	ON_BN_CLICKED(IDC_BUTTON_SPARK_SET, OnButtonSparkSet)
	ON_EN_SETFOCUS(IDC_EDIT_SPARK, OnSetfocusEditSpark)
	ON_BN_CLICKED(IDC_CHECK_SPARK_ON, OnCheckSparkOn)
	ON_BN_CLICKED(IDC_RADIO_PULSE, OnRadioPulse)
	ON_BN_CLICKED(IDC_CHECK_DC_OUT, OnCheckDcOut)
	ON_BN_CLICKED(IDC_RADIO_PFT_FAST, OnRadioPftFast)
	ON_BN_CLICKED(IDC_RADIO_PFT_SLOW, OnRadioPftSlow)
	ON_BN_CLICKED(IDC_RADIO_PULSE_VOLT, OnRadioPulseVolt)
	ON_BN_CLICKED(IDC_RADIO_PULSE_VOLT1, OnRadioPulseVolt1)
	ON_BN_CLICKED(IDC_RADIO_PULSE_VOLT2, OnRadioPulseVolt2)
	ON_BN_CLICKED(IDC_RADIO_PULSE_VOLT3, OnRadioPulseVolt3)
	ON_BN_CLICKED(IDC_RADIO_PULSE_FREQ, OnRadioPulseFreq)
	ON_BN_CLICKED(IDC_RADIO_PULSE_FREQ1, OnRadioPulseFreq1)
	ON_BN_CLICKED(IDC_RADIO_PULSE_FREQ2, OnRadioPulseFreq2)
	ON_BN_CLICKED(IDC_RADIO_PULSE_FREQ3, OnRadioPulseFreq3)
	ON_BN_CLICKED(IDC_RADIO_PULSE_MEASURE_CAV0, OnRadioPulseMeasureCav0)
	ON_BN_CLICKED(IDC_RADIO_PULSE_MEASURE_CAV1, OnRadioPulseMeasureCav1)
	ON_BN_CLICKED(IDC_RADIO_PULSE_MEASURE_CAV2, OnRadioPulseMeasureCav2)
	ON_BN_CLICKED(IDC_RADIO_PULSE_MEASURE_CAV3, OnRadioPulseMeasureCav3)
	ON_BN_CLICKED(IDC_RADIO_I_RNG0, OnRadioIRng0)
	ON_BN_CLICKED(IDC_RADIO_I_RNG1, OnRadioIRng1)
	ON_BN_CLICKED(IDC_RADIO_I_RNG2, OnRadioIRng2)
	ON_BN_CLICKED(IDC_RADIO_I_RNG3, OnRadioIRng3)
	ON_BN_CLICKED(IDC_RADIO_I_RNG4, OnRadioIRng4)
	ON_BN_CLICKED(IDC_RADIO_I_RNG6, OnRadioIRng6)
	ON_BN_CLICKED(IDC_RADIO_I_CC, OnRadioICc)
	ON_BN_CLICKED(IDC_RADIO_I_CV, OnRadioICv)
	ON_BN_CLICKED(IDC_CHECK_I_HIGH_GAIN, OnCheckIHighGain)
	ON_BN_CLICKED(IDC_RADIO_HVO_GAIN1, OnRadioHvoGain1)
	ON_BN_CLICKED(IDC_RADIO_HVO_GAIN2, OnRadioHvoGain2)
	ON_BN_CLICKED(IDC_RADIO_HVO_GAIN3, OnRadioHvoGain3)
	ON_BN_CLICKED(IDC_RADIO_HVO_GAIN4, OnRadioHvoGain4)
	ON_BN_CLICKED(IDC_RADIO_4WGAIN_S1000, OnRadio4WGain_Sx1000)
	ON_BN_CLICKED(IDC_RADIO_4WGAIN_F10, OnRadio4WGain_Fx10)
	ON_BN_CLICKED(IDC_RADIO_4WGAIN_F100, OnRadio4WGain_Fx100)
	ON_BN_CLICKED(IDC_RADIO_4WGAIN_F1000, OnRadio4WGain_Fx1000)
	ON_BN_CLICKED(IDC_BUTTON_BPOINT_TR_SET, OnButtonBpointTrSet)
	ON_BN_CLICKED(IDC_BUTTON_BPOINT_TR_RST, OnButtonBpointTrRst)
	ON_BN_CLICKED(IDC_BUTTON_BPOINT_RY_SET, OnButtonBpointRySet)
	ON_BN_CLICKED(IDC_BUTTON_BPOINT_RY_RST, OnButtonBpointRyRst)
	ON_EN_SETFOCUS(IDC_EDIT_BPOINT_TR, OnSetfocusEditBpointTr)
	ON_EN_SETFOCUS(IDC_EDIT_BPOINT_RY, OnSetfocusEditBpointRy)
	ON_EN_SETFOCUS(IDC_EDIT_CPOINT_TR, OnSetfocusEditCpointTr)
	ON_EN_SETFOCUS(IDC_EDIT_CPOINT_RY, OnSetfocusEditCpointRy)
	ON_BN_CLICKED(IDC_BUTTON_CPOINT_TR_SET, OnButtonCpointTrSet)
	ON_BN_CLICKED(IDC_BUTTON_CPOINT_TR_RST, OnButtonCpointTrRst)
	ON_BN_CLICKED(IDC_BUTTON_CPOINT_RY_SET, OnButtonCpointRySet)
	ON_BN_CLICKED(IDC_BUTTON_CPOINT_RY_RST, OnButtonCpointRyRst)
	ON_BN_CLICKED(IDC_RADIO_BCPLD0, OnRadioBcpld0)
	ON_BN_CLICKED(IDC_RADIO_BCPLD1, OnRadioBcpld1)
	ON_BN_CLICKED(IDC_RADIO_BCPLD2, OnRadioBcpld2)
	ON_BN_CLICKED(IDC_RADIO_BCPLD3, OnRadioBcpld3)
	ON_BN_CLICKED(IDC_RADIO_CCPLD0, OnRadioCcpld0)
	ON_BN_CLICKED(IDC_RADIO_CCPLD1, OnRadioCcpld1)
	ON_BN_CLICKED(IDC_RADIO_CCPLD2, OnRadioCcpld2)
	ON_BN_CLICKED(IDC_RADIO_CCPLD3, OnRadioCcpld3)
	ON_EN_SETFOCUS(IDC_EDIT_BPOINT_CC_V, OnSetfocusEditBpointCcV)
	ON_EN_SETFOCUS(IDC_EDIT_CPOINT_CC_V, OnSetfocusEditCpointCcV)
	ON_BN_CLICKED(IDC_RADIO_BPOINT_CV, OnRadioBpointCv)
	ON_BN_CLICKED(IDC_RADIO_BPOINT_CC, OnRadioBpointCc)
	ON_BN_CLICKED(IDC_RADIO_CPOINT_CV, OnRadioCpointCv)
	ON_BN_CLICKED(IDC_RADIO_CPOINT_CC, OnRadioCpointCc)
	ON_BN_CLICKED(IDC_CHECK_VB_GUARD_ON, OnCheckVbGuardOn)
	ON_BN_CLICKED(IDC_CHECK_VC_GUARD_ON, OnCheckVcGuardOn)
	ON_BN_CLICKED(IDC_BUTTON_BPOINT_CCV_SET, OnButtonBpointCcvSet)
	ON_BN_CLICKED(IDC_BUTTON_CPOINT_CCV_SET, OnButtonCpointCcvSet)
	ON_BN_CLICKED(IDC_CHECK_PULSE, OnCheckPulse)
	ON_BN_CLICKED(IDC_RADIO_HVOFT, OnRadioHvoft)
	ON_BN_CLICKED(IDC_RADIO_FWFT, OnRadioFwft)
	ON_BN_CLICKED(IDC_BUTTON_INIT, OnButtonInit)
	ON_BN_CLICKED(IDC_RADIO_HVC, OnRadioHvc)
	ON_BN_CLICKED(IDC_RADIO_LVC, OnRadioLvc)
	ON_BN_CLICKED(IDC_BUTTON_VB_RST_ALL, OnButtonVbRstAll)
	ON_BN_CLICKED(IDC_RADIO_PULSE_FREQ, OnRadioPulseFreq)
	ON_BN_CLICKED(IDC_BUTTON_VC_RST_ALL, OnButtonVcRstAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CManualTestDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CManualTestDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualTestDlg message handlers

BOOL CManualTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
//TRACE("%s:%d\n", __FILE__, __LINE__);

	// To do 부분
	UINT dwRet = MM.m_pMicroM->InitMotion();
	if (dwRet == FALSE) 
	{	
		ErrMsg(MB_OK, FALSE); // Err를 MsgBox만 출력,  Trace는 미리 출력했음
		ERR.Reset();  
	}	
	TRACE("OnInitDialog(): MM.m_pMicroM->InitMotion %s\n\n", (dwRet == FALSE)? "Error" : "OK");
	
	// 출력상태 초기화
	dwRet = MM.Break();	
	if (dwRet == FALSE) 
	{	ErrMsg(); ERR.Reset();  }	
	TRACE("OnInitDialog(): MM.Break %s\n\n", (dwRet == FALSE)? "Error" : "OK");
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManualTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CManualTestDlg::OnPaint() 
{
	// 만약 윈도우가 minimize 돼 있다면 
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CManualTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// MFC Function: 컨트롤의 색상 바꾸기
HBRUSH CManualTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
//	if (pWnd->GetDlgCtrlID() == IDC_STATIC_MAIN_MODEL)
//	{
//		pDC->SetTextColor(YELLOW);
//		pDC->SetBkColor(SPRINGGREEN);
//		pDC->SetBkMode(TRANSPARENT);
//	} 

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

// MFC Function: 툴팁과 같이 컨트롤이나 버튼이 메시지를 받기 전에 처리해야 할 일을 수행
BOOL CManualTestDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_KEYDOWN) 
		&& (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)) {
		return TRUE;
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}

// MFC Function: 
void CManualTestDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow == TRUE){
		InitMember();
		InitView();		// 폰트 사이즈 조정

		// 200ms 주기로 현재 Dialogue의 상태에 맞춰 UI를 갱신하기 위한 타이머 설정
		SetTimer(0, 200,  NULL);	
	}
	else{
		KillTimer(0);
	}	
}

void CManualTestDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 0)
	{
		CalcPoint();
		RefreshView();	// 다이얼로그의 컨트롤들에게 최신값을 반영한다.
	}

	CDialog::OnTimer(nIDEvent);
}


void CManualTestDlg::InitMember()
{
	// 컨트롤 관련 멤버 변수
	m_nLV1 = 0;
	m_nLV2 = 0;
	m_nHV1 = 0;
	m_nHV2 = 0;
	m_nI = 0;
	m_nBPin = 0;
	m_nCPin = 0;
	m_nBBox = 0;
	m_nCBox = 0;
	m_nBPCBEnable = 0;
	m_nCPCBEnable = 0;
	m_nBCPLD = 0;
	m_nCCPLD = 0;
	m_nBPCB0 = FALSE;
	m_nBPCB1 = FALSE;
	m_nBPCB2 = FALSE;
	m_nBPCB3 = FALSE;
	m_nBPin0 = FALSE;
	m_nBPin1 = FALSE;
	m_nBPin2 = FALSE;
	m_nBPin3 = FALSE;
	m_nBPin4 = FALSE;
	m_nBPin5 = FALSE;
	m_nCPCB0 = FALSE;
	m_nCPCB1 = FALSE;
	m_nCPCB2 = FALSE;
	m_nCPCB3 = FALSE;
	m_nCPin0 = FALSE;
	m_nCPin1 = FALSE;
	m_nCPin2 = FALSE;
	m_nCPin3 = FALSE;
	m_nCPin4 = FALSE;
	m_nCPin5 = FALSE;
	m_nSparkOn = FALSE;
	m_nSpark = 0;
	m_bIHighGainMode = FALSE;
	m_bDCOut = FALSE;
	m_ptBTR = 1;
	m_ptBRY = 1;
	m_ptCTR = 1;
	m_ptCRY = 1;
	m_bVBGuardOn = FALSE;
	m_bVCGuardOn = FALSE;
	m_nBCCV = 0;
	m_nCCCV = 0;
	m_nBCVC = 0;
	m_nCCVC = 0;
	m_bPulse = FALSE;
	m_nADC_B = -1;
	m_nADC_A = -1;
	m_nHLVC = -1;
	m_nHVOGain = -1;
	m_n4WGainMode = -1;
	//m_nICC = 0;
	m_nICC = -1;	//son 170529
	m_nVB = -1;		
	m_nPulseFreq = -1;
	m_nPulseMea = -1;
	m_nPulseVolt = -1;
	m_adcHV1 = 0;
	m_adcHV2 = 0;
	m_adcLV1 = 0;
	m_adcLV2 = 0;
	m_adcSpark = 0;
	m_nIRng = -1;
	m_listBPoint.ResetContent();
	m_listCPoint.ResetContent();

	// 컨트롤 관련 멤버변수가 아님..
	m_bBTRY = m_bCTRY = TR;				// Init Gate Type 
	m_nBPCB = m_nCPCB = 0;
	m_nIRange = -1;
	m_nIadvalue = m_nVadvalue = 0;
	m_dVCvolt = m_dIamp = m_dRohm = 0;

	UpdateData(FALSE);
}

// Quick Menu의 폰트 사이즈를 키운다.
void CManualTestDlg::InitView()
{
	CBrush brush = brush.CreateStockObject(HOLLOW_BRUSH);
	CFont *pFont300 = new CFont();
	CFont *pFont240 = new CFont();
	CFont *pFont200 = new CFont();
	CFont *pFont100 = new CFont();
	
	pFont300->CreatePointFont(300, _T("Arial"));
	pFont240->CreatePointFont(240, _T("Arial"));
	pFont200->CreatePointFont(200, _T("Arial"));
	pFont100->CreatePointFont(100, _T("Arial"));

	GetDlgItem(IDC_STATIC_I)->SetFont(pFont240, TRUE);	
	GetDlgItem(IDC_STATIC_V)->SetFont(pFont240, TRUE);	
	GetDlgItem(IDC_STATIC_R)->SetFont(pFont240, TRUE);	
	
	GetDlgItem(IDC_STATIC_QM_I_UNIT)->SetFont(pFont100, TRUE);	
	GetDlgItem(IDC_STATIC_QM_V_UNIT)->SetFont(pFont100, TRUE);	
	GetDlgItem(IDC_STATIC_QM_R_UNIT)->SetFont(pFont100, TRUE);	
	
	GetDlgItem(IDC_STATIC_QM_I_ADVALUE)->SetFont(pFont200, TRUE);	
	GetDlgItem(IDC_STATIC_QM_V_ADVALUE)->SetFont(pFont200, TRUE);

	GetDlgItem(IDC_EDIT_QM_BPOINT)->SetFont(pFont200, TRUE);	
	GetDlgItem(IDC_EDIT_QM_CPOINT)->SetFont(pFont200, TRUE);

	delete pFont300;
	delete pFont240;
	delete pFont200;
	delete pFont100;
}

void CManualTestDlg::RefreshView(CWnd *pWnd)
{
	UpdateData(TRUE);
	if(pWnd != NULL) pWnd->EnableWindow(TRUE);

	CString strTemp;

	
	// ADC A filter > Current의 단위를 range에 맞게 변경
	switch(m_nIRange)
	{
	case I_RANGE0:
	case I_RANGE1:
	case I_RANGE2:
		GetDlgItem(IDC_STATIC_I_UNIT)->SetWindowText("(mA)");
		GetDlgItem(IDC_STATIC_I_UNIT2)->SetWindowText("(mA)");
		break;

	case I_RANGE3:
	case I_RANGE4:
	default:
		GetDlgItem(IDC_STATIC_I_UNIT)->SetWindowText("(uA)");
		GetDlgItem(IDC_STATIC_I_UNIT2)->SetWindowText("(uA)");
		break;
/*
	case I_RANGE6:
		GetDlgItem(IDC_STATIC_I_UNIT)->SetWindowText("(ADC)");
		GetDlgItem2(IDC_STATIC_I_UNIT)->SetWindowText("(ADC)");
*/
		break;
	}
		
	// Quick Menu > I, V, R의 단위를 range에 맞게 변경
	switch(m_nIRange)
	{
	case I_RANGE0:	
	case I_RANGE1:
	case I_RANGE2:	
		GetDlgItem(IDC_STATIC_QM_I_UNIT)->SetWindowText("mA");
		GetDlgItem(IDC_STATIC_QM_V_UNIT)->SetWindowText("V");
		if(m_nADC_B == ADCB_4WIRE)	
			GetDlgItem(IDC_STATIC_QM_R_UNIT)->SetWindowText("ohm");
		else
			GetDlgItem(IDC_STATIC_QM_R_UNIT)->SetWindowText("K-ohm");
		break;

	case I_RANGE3:
	case I_RANGE4:
	default:
		GetDlgItem(IDC_STATIC_QM_I_UNIT)->SetWindowText("uA");
		GetDlgItem(IDC_STATIC_QM_V_UNIT)->SetWindowText("V");
		if(m_nADC_B == ADCB_4WIRE)	
			GetDlgItem(IDC_STATIC_QM_R_UNIT)->SetWindowText("K-ohm");
		else
			GetDlgItem(IDC_STATIC_QM_R_UNIT)->SetWindowText("M-ohm");
		break;

/*
	case I_RANGE6:
	default:
		GetDlgItem(IDC_STATIC_QM_I_UNIT)->SetWindowText("uA");
		GetDlgItem(IDC_STATIC_QM_V_UNIT)->SetWindowText("V");
		GetDlgItem(IDC_STATIC_QM_R_UNIT)->SetWindowText("M-ohm");
*/
	}


//	strTemp.Format("%d%d%d%d", m_nBPin3, m_nBPin2, m_nBPin1, m_nBPin0);
//	GetDlgItem(IDC_STATIC_BPOIINT0B)->SetWindowText(strTemp);
//
//	strTemp.Format("%d%d%d%d", m_nBPCB0, m_nBCPLD-m_bBTRY*2, m_nBPin5, m_nBPin4);// m_bBTRY*2 버그
//	GetDlgItem(IDC_STATIC_BPOIINT1B)->SetWindowText(strTemp);
//
//	strTemp.Format("%d%d%d%d", m_nBPCBEnable, m_nBPCB3, m_nBPCB2, m_nBPCB1);
//	GetDlgItem(IDC_STATIC_BPOIINT2B)->SetWindowText(strTemp);
//
//	strTemp.Format("%d%d%d%d", 0, (m_nBBox>>2) & 1, (m_nBBox>>1) & 1, m_nBBox & 1);
//	GetDlgItem(IDC_STATIC_BPOIINT3B)->SetWindowText(strTemp);
//	
//	strTemp.Format("%d%d%d%d", m_nCPin3, m_nCPin2, m_nCPin1, m_nCPin0);
//	GetDlgItem(IDC_STATIC_CPOIINT0B)->SetWindowText(strTemp);
//
//	strTemp.Format("%d%d%d%d", m_nCPCB0, m_nCCPLD-m_bCTRY*2, m_nCPin5, m_nCPin4);
//	GetDlgItem(IDC_STATIC_CPOIINT1B)->SetWindowText(strTemp);
//
//	strTemp.Format("%d%d%d%d", m_nCPCBEnable, m_nCPCB3, m_nCPCB2, m_nCPCB1);
//	GetDlgItem(IDC_STATIC_CPOIINT2B)->SetWindowText(strTemp);
//
//	strTemp.Format("%d%d%d%d", 0, (m_nCBox>>2) & 1, (m_nCBox>>1) & 1, m_nCBox & 1);
//	GetDlgItem(IDC_STATIC_CPOIINT3B)->SetWindowText(strTemp);

	strTemp.Format("%d", m_nIadvalue);
	GetDlgItem(IDC_STATIC_QM_I_ADVALUE)->SetWindowText(strTemp);
	strTemp.Format("%d", m_nVadvalue);
	GetDlgItem(IDC_STATIC_QM_V_ADVALUE)->SetWindowText(strTemp);

	// Pulse 모드 시 전류값 사용 안 함
	if(m_nADC_A == ADCA_PFAST || m_nADC_A == ADCA_PSLOW)	// 0:PULSE FAST,  1:PULSE SLOW
	{
		m_lbIamp.SetCaption("0.000");
		m_lbIamp.EnableWindow(FALSE);
	}
	else
	{
		// I High Gain 체크시에는 전류 값을 1/10 로 UI에 출력
		if(m_bIHighGainMode == TRUE) 	
			strTemp.Format("%.3f", m_dIamp/10);
		else							
			strTemp.Format("%.3f", m_dIamp);

		m_lbIamp.SetCaption(strTemp);
		m_lbIamp.EnableWindow(TRUE);
	}

	// 2018.01.30 : HV Open일 때에도 Cal에선 전압 사용. VB는 미사용.   ACE400 기준으로 dR = dVc / dI
	//              Cal과 동일하게 맞추기 위해 ADCB_HVOPEN 케이스의 전압미사용처리 코드 코멘트처리함.
/*	
	// HV OPEN 모드 시 전압값 사용 안 함
	if(m_nADC_B == ADCB_HVOPEN)					// HVOFT (HV OPEN)
	{
		m_lbVvolt.SetCaption("0.000");
		m_lbVvolt.EnableWindow(FALSE);
	}
	else
*/
	{
		strTemp.Format("%.3f", m_dVCvolt);
		m_lbVvolt.SetCaption(strTemp);
		m_lbVvolt.EnableWindow(TRUE);
	}

	// 4 Wire 측정시 
	if(m_nADC_B == ADCB_4WIRE)	
	{
		((CWnd*)GetDlgItem(IDC_STATIC_V))->SetWindowText("4W");
	}
	else // 일반 2 Wire, HV OPEN 측정시
	{
		((CWnd*)GetDlgItem(IDC_STATIC_V))->SetWindowText("VC");
	}


	// VB 값을 해당 VbSrc의 VB창에서 read해 온다.
	strTemp="0.0";
	switch(m_nVB)
	{
	case VB_LV1:
		GetDlgItem(IDC_STATIC_LV1_MNT)->GetWindowText(strTemp);
		break;

	case VB_LV2:
		GetDlgItem(IDC_STATIC_LV2_MNT)->GetWindowText(strTemp);
		break;

	case VB_HV1:
		GetDlgItem(IDC_STATIC_HV1_MNT)->GetWindowText(strTemp);
		break;

	case VB_HV2:
		GetDlgItem(IDC_STATIC_HV2_MNT)->GetWindowText(strTemp);
		break;

//	case VB_PULSE:
//		GetDlgItem()->GetWindowText(strTemp);
//		break;
	}
	double dVBVolt = atof(strTemp);

#ifdef __NEW_CAL__
	// 2017.11.22 김영필 부장 confirm. 
	// VB read 값은 4W, Open, HvOpen, Short 모두 계산식에서 사용할 필요 없음.
	if (m_dVCvolt != 0 &&  m_dIamp != 0) 	
#else

		// 4 Wire 라면 VC 값이 0이거나 I값이 0일때(Divide by Zero)  계산 안함
	if( ( m_nADC_B == ADCB_4WIRE && (m_dVCvolt != 0 &&  m_dIamp != 0) ) ||	

		// 2 Wire 라면 VB 값이 0이거나 I값이 0일때(Divide by Zero)  계산 안함
		( m_nADC_B != ADCB_4WIRE && (dVBVolt !=0 && m_dIamp != 0) ) )
#endif
	{

#ifdef __NEW_CAL__
		// 2017.11.22 김영필 부장 confirm. 
		// 1. VB read 값은 4W, Open, HvOpen, Short 모두 계산식에서 사용할 필요 없음.
		// 2. ADC_B 필터가 HVO, 4W인 경우는 I 값을 고정해서 사용. read값 사용하지 마라.
		
		if (m_nADC_B == ADCB_HVOPEN)	
			m_dRohm = (double)(m_dVCvolt/m_nI);			// R =   VC / I (고정값)

		else if (m_nADC_B == ADCB_4WIRE)	
			m_dRohm = (double)((m_dVCvolt/m_nI) *10);	// R =   VC / I (고정값) *10

		else
			m_dRohm = (double)(m_dVCvolt/m_dIamp);		// R =   VC / I (Read값)

#else

		// HV Open, 4W 측정시  (HW에서 VC 에 이미 VB-VC한 값을 주고 있음) 
		if (m_nADC_B == ADCB_HVOPEN)
			m_dRohm = (double)(m_dVCvolt/m_dIamp);			// R =   VC / I 

		else if (m_nADC_B == ADCB_4WIRE)
			m_dRohm = (double)((m_dVCvolt/m_dIamp) * 10);	// R =   VC / I  * 10

		// ADCB_VCFAST: LV 2W, LV Short
		else
			//  R = (설정전압:VB - 측정전압:VC) / 측정전류:I
			m_dRohm = (double)(dVBVolt - m_dVCvolt)/m_dIamp;		

#endif


		while(m_dRohm>0 && m_dRohm<1)
		{ 
			// 소수점 표시 갯수를 줄이기 위해 숫자는 *1000하고 대신 단위는 1/1000 한다.
			m_dRohm *= 1000;

			((CWnd*)GetDlgItem(IDC_STATIC_QM_R_UNIT))->GetWindowText(strTemp);
			if(strTemp == "M-ohm") ((CWnd*)GetDlgItem(IDC_STATIC_QM_R_UNIT))->SetWindowText("K-ohm");
			else if(strTemp == "K-ohm") ((CWnd*)GetDlgItem(IDC_STATIC_QM_R_UNIT))->SetWindowText(  "ohm");
			else if(strTemp ==   "ohm") ((CWnd*)GetDlgItem(IDC_STATIC_QM_R_UNIT))->SetWindowText("m-ohm");
			else if(strTemp == "m-ohm") ((CWnd*)GetDlgItem(IDC_STATIC_QM_R_UNIT))->SetWindowText("u-ohm");
			else if(strTemp == "u-ohm") ((CWnd*)GetDlgItem(IDC_STATIC_QM_R_UNIT))->SetWindowText("n-ohm");
		}

		while(m_dRohm>=1000) 
		{ 
			// 1000 이상 숫자가 출력되는 것을 피하기 위해 숫자는 1/1000 하고 단위를 *1000 한다.
			m_dRohm /= 1000;

			((CWnd*)GetDlgItem(IDC_STATIC_QM_R_UNIT))->GetWindowText(strTemp);
			if(strTemp == "M-ohm") ((CWnd*)GetDlgItem(IDC_STATIC_QM_R_UNIT))->SetWindowText("G-ohm");
			else if(strTemp == "K-ohm") ((CWnd*)GetDlgItem(IDC_STATIC_QM_R_UNIT))->SetWindowText("M-ohm");
			else if(strTemp ==   "ohm") ((CWnd*)GetDlgItem(IDC_STATIC_QM_R_UNIT))->SetWindowText("K-ohm");
			else if(strTemp == "m-ohm") ((CWnd*)GetDlgItem(IDC_STATIC_QM_R_UNIT))->SetWindowText(  "ohm");
			else if(strTemp == "u-ohm") ((CWnd*)GetDlgItem(IDC_STATIC_QM_R_UNIT))->SetWindowText("m-ohm");
		}
	}
	else 
		m_dRohm = 0;
	

	strTemp.Format("%.3f", m_dRohm);
	m_lbRohm.SetCaption(strTemp);

	strTemp.Format("%d", m_ptBTR);
	GetDlgItem(IDC_EDIT_QM_BPOINT)->SetWindowText(strTemp);

	strTemp.Format("%d", m_ptCTR);
	GetDlgItem(IDC_EDIT_QM_CPOINT)->SetWindowText(strTemp);


	// Disable,Enable Window
	if(m_nSparkOn)
	{
		GetDlgItem(IDC_EDIT_SPARK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SPARK_SET)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_SPARK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SPARK_DA)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SPARK_SET)->EnableWindow(FALSE);
	}

	if(m_bPulse) 
	{
		GetDlgItem(IDC_RADIO_PULSE_FREQ)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PULSE_FREQ1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PULSE_FREQ2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PULSE_FREQ3)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PULSE_VOLT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PULSE_VOLT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PULSE_VOLT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PULSE_VOLT3)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_RADIO_PULSE_FREQ)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PULSE_FREQ1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PULSE_FREQ2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PULSE_FREQ3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PULSE_VOLT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PULSE_VOLT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PULSE_VOLT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PULSE_VOLT3)->EnableWindow(FALSE);
	}

	if(m_bBTRY == TR)
	{
		GetDlgItem(IDC_STATIC_BTR)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_BRY)->EnableWindow(FALSE);
	}
	else if(m_bBTRY == RY)
	{
		GetDlgItem(IDC_STATIC_BTR)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_BRY)->EnableWindow(TRUE);
	}

	if(m_bCTRY == TR)
	{
		GetDlgItem(IDC_STATIC_CTR)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_CRY)->EnableWindow(FALSE);
	}
	else if(m_bCTRY == RY)
	{
		GetDlgItem(IDC_STATIC_CTR)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_CRY)->EnableWindow(TRUE);
	}

	switch(m_nADC_A)
	{
	case ADCA_PFAST:		// PULSE FAST
	case ADCA_PSLOW:		// PUSE SLOW
		GetDlgItem(IDC_RADIO_PULSE_MEASURE_CAV0)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PULSE_MEASURE_CAV1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PULSE_MEASURE_CAV2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PULSE_MEASURE_CAV3)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_I_RNG0)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_I_RNG1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_I_RNG2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_I_RNG3)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_I_RNG4)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_I_RNG6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_I)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_I_ADC)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_I_MNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_I_MNT_AD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_I_SET)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_I_CC)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_I_CV)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_I_HIGH_GAIN)->EnableWindow(TRUE);
		break;

	case ADCA_IFAST:		// IFFT	(I Fast)
	case ADCA_ISLOW:		// ISFT (I Slow)
		GetDlgItem(IDC_RADIO_PULSE_MEASURE_CAV0)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PULSE_MEASURE_CAV1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PULSE_MEASURE_CAV2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PULSE_MEASURE_CAV3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_I_RNG0)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_I_RNG1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_I_RNG2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_I_RNG3)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_I_RNG4)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_I_RNG6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_I)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_I_ADC)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_I_MNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_I_MNT_AD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_I_SET)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_I_CC)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_I_CV)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_I_HIGH_GAIN)->EnableWindow(TRUE);
		break;

	case ADCA_VB:		// VBFT
	default:
		GetDlgItem(IDC_RADIO_PULSE_MEASURE_CAV0)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PULSE_MEASURE_CAV1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PULSE_MEASURE_CAV2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PULSE_MEASURE_CAV3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_I_RNG0)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_I_RNG1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_I_RNG2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_I_RNG3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_I_RNG4)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_I_RNG6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_I)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_I_ADC)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_I_MNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_I_MNT_AD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_I_SET)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_I_CC)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_I_CV)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_I_HIGH_GAIN)->EnableWindow(FALSE);
		break;
	}
	
	switch(m_nADC_B)
	{
	case ADCB_VCFAST:		// VCFFT: VC FAST
//	case ADCB_VCSLOW:		// VCSFT: VC SLOW 	//170529 VC Slow Filter는 현재 쓰지 않음 (신부성 차장 confirm)
		GetDlgItem(IDC_RADIO_HVC)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_LVC)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_HVO_GAIN1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HVO_GAIN2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HVO_GAIN3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HVO_GAIN4)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_4WGAIN_S1000)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_4WGAIN_F10)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_4WGAIN_F100)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_4WGAIN_F1000)->EnableWindow(FALSE);
		break;
		
	case ADCB_HVOPEN:		// HVOFT (HV OPEN)
		GetDlgItem(IDC_RADIO_HVC)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_LVC)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HVO_GAIN1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_HVO_GAIN2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_HVO_GAIN3)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_HVO_GAIN4)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_4WGAIN_S1000)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_4WGAIN_F10)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_4WGAIN_F100)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_4WGAIN_F1000)->EnableWindow(FALSE);
		break;

	case ADCB_4WIRE:		// FWFT (4 Wire)
		GetDlgItem(IDC_RADIO_HVC)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_LVC)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HVO_GAIN1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HVO_GAIN2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HVO_GAIN3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HVO_GAIN4)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_4WGAIN_S1000)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_4WGAIN_F10)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_4WGAIN_F100)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_4WGAIN_F1000)->EnableWindow(TRUE);
		break;

	default:
		GetDlgItem(IDC_RADIO_HVC)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_LVC)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HVO_GAIN1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HVO_GAIN2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HVO_GAIN3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HVO_GAIN4)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_4WGAIN_S1000)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_4WGAIN_F10)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_4WGAIN_F100)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_4WGAIN_F1000)->EnableWindow(FALSE);
		break;
	}

	BOOL bAble=FALSE;
	if(m_bVBGuardOn) bAble=TRUE;
	GetDlgItem(IDC_RADIO_BPOINT_CV)->EnableWindow(bAble);
	GetDlgItem(IDC_RADIO_BPOINT_CC)->EnableWindow(bAble);

	bAble=FALSE;
	if(m_bVBGuardOn & m_nBCVC)  bAble=TRUE;
	GetDlgItem(IDC_EDIT_BPOINT_CC_V)->EnableWindow(bAble);
	GetDlgItem(IDC_BUTTON_BPOINT_CCV_SET)->EnableWindow(bAble);
	
	bAble=FALSE;
	if(m_bVCGuardOn) bAble=TRUE;
	GetDlgItem(IDC_RADIO_CPOINT_CV)->EnableWindow(bAble);
	GetDlgItem(IDC_RADIO_CPOINT_CC)->EnableWindow(bAble);

	bAble=FALSE;
	if(m_bVCGuardOn & m_nCCVC)  bAble=TRUE;
	GetDlgItem(IDC_EDIT_CPOINT_CC_V)->EnableWindow(bAble);
	GetDlgItem(IDC_BUTTON_CPOINT_CCV_SET)->EnableWindow(bAble);

	UpdateData(FALSE);
}


int CManualTestDlg::SetVb_GetVb(UINT module_id, UINT in_value, double& dAdVolt)
{
	int	 dwRet;
	UINT dwVbHL; 	
	CString strModule, strVbHL;

	switch(module_id)	
	{
		case LV1:  case LV2:
			strModule = (module_id == LV1) ? "LV1" : "LV2"; 
			dwVbHL = LVB;
			strVbHL = "LVB";
			break;

		case HV1:  case HV2:
			strModule = (module_id == HV1) ? "HV1" : "HV2"; 
			dwVbHL = HVB;
			strVbHL = "HVB";
			break;

		default:
			TRACE("_SetVB..(): module_id(=%d) range(0~3) Over! \n", module_id);
			return FALSE;
	}

	// Volt 값을 DA 값으로 변경
	UNIT_CONVERT(in_value, VOLT_TO_DA);	// ex) in_value : 5 -> 15000  

	// sSET_VB:  DA 값을 IO카드로 보낸다.
	if(dwRet = MM.Set(module_id, in_value) == FALSE)    // sSET_VB   : ex) MM.Set(LV1, 15000);
	{
#ifndef __NO_LIST__
		CString ErrStr, ErrTitle="???", ErrState="Logic_err?", ErrSol;
		int err = ERR.Get();
		ERR.GetMsg(err, ErrTitle, ErrState, ErrSol);

		ErrStr.Format("OnButton%sSet(): sSet_VB(%s) err=%d %s : %s", 
				strModule, strModule, err, ErrTitle, ErrState);
		if (err != NO_ERR)
			m_listMsg.InsertString(0, ErrStr);
		TRACE("%s\n", ErrStr);
#endif
	}

	// sSET_VB_HL:  MM.Get(V)하기 전에 VB를 선택한다.    (ex: LVB)
	if (dwRet = MM.Set(dwVbHL, module_id) == FALSE)	    // sSET_VB_HL: ex) MM.Set(LVB, LV1);
	{
#ifndef __NO_LIST__
		CString ErrStr, ErrTitle="???", ErrState="Logic_err?", ErrSol;
		int err = ERR.Get();
		ERR.GetMsg(err, ErrTitle, ErrState, ErrSol);

		ErrStr.Format("OnButton%sSet(): MM.Set(%s, %s) err=%d %s : %s", 
				strModule, strVbHL, strModule, err, ErrTitle, ErrState);
		if (err != NO_ERR)
			m_listMsg.InsertString(0, ErrStr);
		TRACE("%s\n", ErrStr);
#endif
	}

	

	// sSET_ADC_A:  MM.Get(V)하기 전에 ADCA Filter를 VBFT로 선택한다. 
	if (dwRet = MM.Set(VBFT) == FALSE)			    // sSET_ADC_A: 
	{
#ifndef __NO_LIST__
		CString ErrStr, ErrTitle="???", ErrState="Logic_err?", ErrSol;
		int err = ERR.Get();
		ERR.GetMsg(err, ErrTitle, ErrState, ErrSol);

		ErrStr.Format("OnButton%sSet(): MM.Set(VBFT) err=%d %s : %s", 
				strModule, err, ErrTitle, ErrState);
		if (err != NO_ERR)
			m_listMsg.InsertString(0, ErrStr);
		TRACE("%s\n", ErrStr);
#endif
	}
	m_nADC_A = ADCA_VB;		// UI상의 ADC A filter를 'VB'(VBFT) 로 설정한다.

	Delay(5, msec);


	//------------------------------------
	// sGET_VI:  ADC A 전압 설정값(AD)을 read한다.
	int err=NO_ERR;
	UINT dwVal[5], dwMin=0, dwMax=0, dwSum=0;
	dAdVolt = -1;

	for (int i=0; i < 5; i++)		// 5회 read하여 평균을 낸다.
	{
		if((dwVal[i] = (UINT)MM.Get(V)) == (UINT)(-1))			// sGET_VI:  read from IO Card
		{
#ifndef __NO_LIST__
			CString ErrStr, ErrTitle="???", ErrState="Logic_err?", ErrSol;
			err = ERR.Get();
			ERR.GetMsg(err, ErrTitle, ErrState, ErrSol);

			ErrStr.Format("OnButton%sSet(): MM.Get(V) dwVal[%d]=%u err=%d %s : %s", 
				strModule, i, dwVal[i], err, ErrTitle, ErrState);

			if (err != NO_ERR)
				m_listMsg.InsertString(0, ErrStr);
			TRACE("%s\n", ErrStr);
#endif
			break;
		}

		if (i == 0)
		{
			dwMax = dwVal[i];
			dwMin = dwVal[i];
		}
		else //min, max 값을 골라낸다.
		{
			if (dwMax < dwVal[i])		// 최대값보다 현재값이 크면 최대값 변경
				dwMax = dwVal[i];
			if (dwMin > dwVal[i])		// 최소값보다 현재값이 작으면 최소값 변경
				dwMin = dwVal[i];
		}
		dwSum += dwVal[i];	
	}
	if (err == NO_ERR)
		dAdVolt = (dwSum - dwMax - dwMin) /3;  // min, max 제외하고 3회로 나눈 평균값

	return TRUE;
}

void CManualTestDlg::OnButtonLv1Set() 
{
	UpdateData(TRUE);	// Edit 컨트롤의 입력값을 Value형 멤버변수로 전송
	UINT module_id = LV1;
	UINT in_value = m_nLV1;		// ex) 5
	double dAdVolt = -1, dVolt = -1, dAdVolt2;

	int dwRet = SetVb_GetVb(module_id, in_value, dAdVolt);
	if(dAdVolt != (double)(-1))	// MM.Get(V) 성공이면
	{
		dAdVolt2 = dAdVolt;	  // +0.5 되어서 나오는 현상을 막기위해 카피해서 사용.
		dVolt = UNIT_CONVERT(dAdVolt2, AD_TO_LVOLT);
	}

#ifndef __NO_LIST__
	CString str;
	str.Format("..Lv1Set(): _SetVB..(module=%d, val=%d) %s! dAdVolt=%.3f, dVolt=%.3f\n\n", 
			module_id, in_value, (dwRet == FALSE) ? "Error": "OK", 
			dAdVolt, dVolt );
	m_listMsg.InsertString(0, str);
	TRACE("%s\n", str);
#endif
	if (dwRet == FALSE)
		return;

	CString strVolt;
	strVolt.Format("%.3f", dAdVolt);
	GetDlgItem(IDC_STATIC_LV1_MNT_AD)->SetWindowText(strVolt);
	strVolt.Format("%.3f", dVolt);	
	GetDlgItem(IDC_STATIC_LV1_MNT)->SetWindowText(strVolt);

//	UpdateData(FALSE);		// Value형 멤버변수의 값을 컨트롤에 반영

}

void CManualTestDlg::OnButtonLv2Set() 
{
	UpdateData(TRUE);
	UINT module_id = LV2;
	UINT in_value = m_nLV2;
	double dAdVolt = -1, dVolt = -1, dAdVolt2;

	int dwRet = SetVb_GetVb(module_id, in_value, dAdVolt);
	if(dAdVolt != (double)(-1))	// MM.Get(V) 성공이면
	{
		dAdVolt2 = dAdVolt;
		dVolt = UNIT_CONVERT(dAdVolt2, AD_TO_LVOLT);
	}

#ifndef __NO_LIST__
	CString str;
	str.Format("..Lv2Set(): _SetVB..(module=%d, val=%d) %s! dAdVolt=%.3f, dVolt=%.3f\n\n", 
			module_id, in_value, (dwRet == FALSE) ?  "Error": "OK" , 
			dAdVolt, dVolt );
	m_listMsg.InsertString(0, str);
	TRACE("%s\n", str);
#endif
	if (dwRet == FALSE)
		return;

	CString strVolt;
	strVolt.Format("%.3f", dAdVolt);
	GetDlgItem(IDC_STATIC_LV2_MNT_AD)->SetWindowText(strVolt);
	strVolt.Format("%.3f", dVolt);
	GetDlgItem(IDC_STATIC_LV2_MNT)->SetWindowText(strVolt);
//	UpdateData(FALSE);

}

void CManualTestDlg::OnButtonHv1Set() 
{
	UpdateData(TRUE);
	UINT module_id = HV1;
	UINT in_value = m_nHV1;
	double dAdVolt = -1, dVolt = -1, dAdVolt2;

	int dwRet = SetVb_GetVb(module_id, in_value, dAdVolt);
	if(dAdVolt != (double)(-1))	// MM.Get(V) 성공이면
	{
		dAdVolt2 = dAdVolt;
		dVolt = UNIT_CONVERT(dAdVolt2, AD_TO_HVOLT);
	}

#ifndef __NO_LIST__
	CString str;
	str.Format("..Hv1Set(): _SetVB..(module=%d, val=%d) %s! dAdVolt=%.3f, dVolt=%.3f\n\n", 
			module_id, in_value, (dwRet == FALSE) ?  "Error" : "OK" , 
			dAdVolt, dVolt );
	m_listMsg.InsertString(0, str);
	TRACE("%s\n", str);
#endif
	if (dwRet == FALSE)
		return;

	CString strVolt;
	strVolt.Format("%.3f", dAdVolt);
	GetDlgItem(IDC_STATIC_HV1_MNT_AD)->SetWindowText(strVolt);
	strVolt.Format("%.3f", dVolt);
	GetDlgItem(IDC_STATIC_HV1_MNT)->SetWindowText(strVolt);

//	UpdateData(FALSE);

}

void CManualTestDlg::OnButtonHv2Set() 
{
	UpdateData(TRUE);
	UINT module_id = HV2;
	UINT in_value = m_nHV2;
	double dAdVolt = -1, dVolt = -1, dAdVolt2;

	int dwRet = SetVb_GetVb(module_id, in_value, dAdVolt);
	if(dAdVolt != (double)(-1))	// MM.Get(V) 성공이면
	{
		dAdVolt2 = dAdVolt;
		dVolt = UNIT_CONVERT(dAdVolt2, AD_TO_HVOLT);
	}

#ifndef __NO_LIST__
	CString str;
	str.Format("..Hv2Set(): _SetVB..(module=%d, val=%d) %s! dAdVolt=%.3f, dVolt=%.3f\n\n", 
			module_id, in_value, (dwRet == FALSE) ?  "Error": "OK" , 
			dAdVolt, dVolt );
	m_listMsg.InsertString(0, str);
	TRACE("%s\n", str);
#endif
	if (dwRet == FALSE)
		return;

	CString strVolt;
	strVolt.Format("%.3f", dAdVolt);
	GetDlgItem(IDC_STATIC_HV2_MNT_AD)->SetWindowText(strVolt);
	strVolt.Format("%.3f", dVolt);
	GetDlgItem(IDC_STATIC_HV2_MNT)->SetWindowText(strVolt);

//	UpdateData(FALSE);	//son170526 : LV1 Set 설정후에 I Filter 설정하면 죽는 문제 관련해서 삭제함

}

void CManualTestDlg::OnCheckPulse() 
{
	/*
	UpdateData(TRUE);
	if(m_bPulse) MM.m_pMicroM->On(VB_AC_MODE);
	else		 MM.m_pMicroM->Off(VB_AC_MODE);
	*/
}

void CManualTestDlg::OnRadioPulseFreq() 
{
	//MM.Set(PULSE, 0, -1);
}

void CManualTestDlg::OnRadioPulseFreq1() 
{
	//MM.Set(PULSE, 1, -1);
}

void CManualTestDlg::OnRadioPulseFreq2() 
{
	//MM.Set(PULSE, 2, -1);
}

void CManualTestDlg::OnRadioPulseFreq3() 
{
	//MM.Set(PULSE, 3, -1);
}

void CManualTestDlg::OnRadioPulseVolt() 
{
	//MM.Set(PULSE, -1, 0);
}

void CManualTestDlg::OnRadioPulseVolt1() 
{
	//MM.Set(PULSE, -1, 1);
}

void CManualTestDlg::OnRadioPulseVolt2() 
{
	//MM.Set(PULSE, -1, 2);
}

void CManualTestDlg::OnRadioPulseVolt3() 
{
	//MM.Set(PULSE, -1, 3);
}

void CManualTestDlg::OnButtonVbOutSet() 
{
	UpdateData(TRUE);
	UINT module_id = VB_OUT;
	CString 	strTemp;

	//MM.m_pMicroM->On(VB_OUT_CHK);  // VB_OUT_CHK ON 
	//MM.m_pMicroM->On(VB_SEN_SEL);  // 회로보호를 위해 HV로 Set (0: LV,  1: HV)
	int dwRet = MM.Set(VB_OUT);    // sSET_VB_OUT: 
	strTemp.Format("..VbOutSet(): MM.Set(VB_OUT) %s\n\n", (dwRet == FALSE)? "Error" : "OK");	
	if (dwRet == FALSE)
	{
		ERR.Set( ERR.Get(), strTemp);
#ifndef __NO_LIST__
		m_listMsg.InsertString(0, strTemp);
#endif
		ErrMsg(); ERR.Reset(); 
		return;
	}
	TRACE("%s\n", strTemp);

	// MM.Get(V)하기 전에 Filter를 VBFT로 선택한다. 
	dwRet = MM.Set(VBFT);    // sSET_ADC_A: 
#ifndef __NO_LIST__
	{
		CString ErrStr, ErrTitle="???", ErrState="Logic_err?", ErrSol;
		int err = ERR.Get();
		ERR.GetMsg(err, ErrTitle, ErrState, ErrSol);

		ErrStr.Format("..VbOutSet(): MM.Set(VBFT) err=%d %s : %s", err, ErrTitle, ErrState);
		TRACE("%s\n\n", ErrStr);
		if (err != NO_ERR)		// if (dwRet == FALSE) 와 같아야 함.
			m_listMsg.InsertString(0, ErrStr);
	}
#endif
	if (dwRet == FALSE)
		return;

	m_nADC_A = ADCA_VB;		// UI상의 ADC A filter를 'VB'(VBFT) 로 설정한다.
	Delay(5, msec);

	double dVolt = -1;
	UINT dwAdVolt, dwAdVolt2;
	dwAdVolt = (UINT)MM.Get(V);		// sGET_VI:  read from IO Card
	if(dwAdVolt != (UINT)(-1))	// MM.Get(V) 성공이면
	{
		dwAdVolt2 = dwAdVolt;
		dVolt = UNIT_CONVERT(dwAdVolt2, AD_TO_HVOLT);	// VB_OUT_CHECK는 HV 임.
	}
#ifndef __NO_LIST__
	{
		CString ErrStr, ErrTitle="???", ErrState="Logic_err?", ErrSol;
		int err = ERR.Get();
		ERR.GetMsg(err, ErrTitle, ErrState, ErrSol);

		ErrStr.Format("..VbOutSet(): MM.Get(V) dVolt=%.3f(AD=%d) err=%d %s : %s", 
			dVolt, dwAdVolt, err, ErrTitle, ErrState);
		if (err != NO_ERR)
			m_listMsg.InsertString(0, ErrStr);
		TRACE("%s\n\n", ErrStr);
	}
#endif

	CString sTemp;
	sTemp.Format("%.3f", dVolt);
	GetDlgItem(IDC_STATIC_VB_OUT_MNT)->SetWindowText(sTemp);
}

void CManualTestDlg::OnButtonSparkSet() 
{
	UpdateData(TRUE);
	UINT module_id = SPARK;
	UINT in_value = m_nSpark, out_value = 0;

	UNIT_CONVERT(in_value, VOLT_TO_DA_SPARK);
	MM.Set(module_id, in_value);
}

void CManualTestDlg::OnCheckSparkOn() 
{
	UpdateData(TRUE);
	/*
	if(m_nSparkOn) MM.m_pMicroM->On(_SPARK_RST);
	else		   MM.m_pMicroM->Off(_SPARK_RST);
	*/
}

void CManualTestDlg::OnSetfocusEditLv1() 
{
	UpdateData(TRUE);
	if(m_bNKeyPadOpen == TRUE) return;
	m_bNKeyPadOpen = TRUE;

	if(NKEYPAD.SetValue((int)m_nLV1, 0, MAX_LV) == FALSE) return;
	NKEYPAD.DoModal();
	m_nLV1 = (int)NKEYPAD.GetValue();

	m_adcLV1 = m_nLV1;
	UNIT_CONVERT(m_adcLV1, VOLT_TO_DA);

	m_bNKeyPadOpen = FALSE;
	SetFocus( );
	UpdateData(FALSE);
}

void CManualTestDlg::OnSetfocusEditLv2() 
{
	UpdateData(TRUE);
	if(m_bNKeyPadOpen == TRUE) return;
	m_bNKeyPadOpen = TRUE;

	if(NKEYPAD.SetValue((int)m_nLV2, 0, MAX_LV) == FALSE) return;
	NKEYPAD.DoModal();
	m_nLV2 = (int)NKEYPAD.GetValue();

	m_adcLV2 = m_nLV2;
	UNIT_CONVERT(m_adcLV2, VOLT_TO_DA);

	m_bNKeyPadOpen = FALSE;
	SetFocus( );
	UpdateData(FALSE);	
}

void CManualTestDlg::OnSetfocusEditHv1() 
{
	UpdateData(TRUE);
	if(m_bNKeyPadOpen == TRUE) return;
	m_bNKeyPadOpen = TRUE;

	if(NKEYPAD.SetValue((int)m_nHV1, (MAX_LV+1), MAX_HV) == FALSE) return;
	NKEYPAD.DoModal();
	m_nHV1 = (int)NKEYPAD.GetValue();

	m_adcHV1 = m_nHV1;
	UNIT_CONVERT(m_adcHV1, VOLT_TO_DA);

	m_bNKeyPadOpen = FALSE;
	SetFocus( );
	UpdateData(FALSE);	
}

void CManualTestDlg::OnSetfocusEditHv2() 
{
	UpdateData(TRUE);
	if(m_bNKeyPadOpen == TRUE) return;
	m_bNKeyPadOpen = TRUE;

	if(NKEYPAD.SetValue((int)m_nHV2, (MAX_LV+1), MAX_HV) == FALSE) return;
	NKEYPAD.DoModal();
	m_nHV2 = (int)NKEYPAD.GetValue();

	m_adcHV2 = m_nHV2;
	UNIT_CONVERT(m_adcHV2, VOLT_TO_DA);

	m_bNKeyPadOpen = FALSE;
	SetFocus( );
	UpdateData(FALSE);		
}

void CManualTestDlg::OnSetfocusEditSpark() 
{
	UpdateData(TRUE);
	if(m_bNKeyPadOpen == TRUE) return;
	m_bNKeyPadOpen = TRUE;

	//son170508 range(0~0 -> 1~100)으로 수정
	if(NKEYPAD.SetValue((int)m_nSpark, 1, 100) == FALSE) return; 
	NKEYPAD.DoModal();
	m_nSpark = (int)NKEYPAD.GetValue();

	m_adcSpark = m_nSpark;
	UNIT_CONVERT(m_adcSpark, VOLT_TO_DA_SPARK);

	m_bNKeyPadOpen = FALSE;
	SetFocus( );
	UpdateData(FALSE);
}

void CManualTestDlg::OnSetfocusEditI() 
{
	UpdateData(TRUE);
	if(m_bNKeyPadOpen == TRUE) return;
	m_bNKeyPadOpen = TRUE;

/*
	//10270622  신부성차장 요청:  Range 강제설정은 위험할 수 있으므로 삭제
	switch(m_nIRange)
	{
		case I_RANGE0: case I_RANGE1: case I_RANGE2:
		case I_RANGE3: case I_RANGE4:
			break;
		default:		
			// I_Range 설정이 미리 안 돼 있으면 range_0으로 설정
			CString str;
			str.Format("..EditI(): m_nIRange(=%d) range(%d~%d) Error. Setting it for I_RANGE0(=%d)", 
					m_nIRange, I_RANGE0, I_RANGE4, I_RANGE0);
			TRACE("%s\n", str);
			m_listMsg.InsertString(0, str);

			m_nIRng = 0;	   	// UI를  IDC_RADIO_I_RNG0가 체크된 상태로 변경
			m_nIRange = I_RANGE0;	
			MM.Set(I, -1, m_nIRange);		// sSET_I
			UpdateData(FALSE);
			break;
	}
*/


	switch(m_nIRange)
	{
		case I_RANGE0://mA
	//		if(NKEYPAD.SetValue((int)m_nI, 20, 100) == FALSE) return;
			if(NKEYPAD.SetValue((int)m_nI, 1, 200) == FALSE) return;
			break;
	
		case I_RANGE1://mA
	//		if(NKEYPAD.SetValue((int)m_nI, 10, 20) == FALSE) return;
			if(NKEYPAD.SetValue((int)m_nI, 1, 20) == FALSE) return;
			break;
	
		case I_RANGE2://mA
	//		if(NKEYPAD.SetValue((int)m_nI, 2, 10) == FALSE) return;
			if(NKEYPAD.SetValue((int)m_nI, 1, 10) == FALSE) return;
			break;
	
		case I_RANGE3://uA
	//		if(NKEYPAD.SetValue((int)m_nI, 200, 2000) == FALSE) return;
			if(NKEYPAD.SetValue((int)m_nI, 1, 2000) == FALSE) return;
			break;
	
		case I_RANGE4://uA
	//		if(NKEYPAD.SetValue((int)m_nI, 20, 200) == FALSE) return;
			if(NKEYPAD.SetValue((int)m_nI, 1, 200) == FALSE) return;
			break;
	
	/*
		case I_RANGE6:
		default://DA
			if(NKEYPAD.SetValue((int)m_nI, 0, 4095) == FALSE) return;
			break;
	*/
	}

	NKEYPAD.DoModal();
	m_nI = (int)NKEYPAD.GetValue();

	m_bNKeyPadOpen = FALSE;
	SetFocus( );
	UpdateData(FALSE);
}

void CManualTestDlg::OnButtonISet() 
{
	UpdateData(TRUE);
	UINT I_value = m_nI;


	switch(m_nIRange)
	{
		case I_RANGE0://mA
		case I_RANGE1://mA
		case I_RANGE2://mA
			I_value *= 1000;	// uA 단위로 통일
		case I_RANGE3://uA
		case I_RANGE4://uA
			UNIT_CONVERT(I_value, I0_TO_DA+m_nIRange-I_RANGE0);// uA -> DA
			break;

		//case I_RANGE6:
		default://DA		
			CString str;
#if 0
			//10270622  신부성차장 요청:  Range 강제설정은 위험할 수 있으므로 삭제하고 에러 출력으로 수정
			// I_Range 설정이 미리 안 돼 있으면 range_0으로 강제 설정
			str.Format("..ISet(): m_nIRange(=%d) range(%d~%d) Error. Setting it for I_RANGE0(=%d)", 
					m_nIRange, I_RANGE0, I_RANGE4, I_RANGE0);
			TRACE("%s\n", str);
			m_listMsg.InsertString(0, str);

			m_nIRng = 0;	   	// UI에 IDC_RADIO_I_RNG0가 체크된 상태
			UpdateData(FALSE);	// m_nIRng 값을 UI에 반영

			m_nIRange = I_RANGE0;	
			MM.Set(I, -1, m_nIRange);		// sSET_I Range

			// case I_RANGE0 :  수행
			I_value *= 1000;	// uA 단위로 통일
			UNIT_CONVERT(I_value, I0_TO_DA+m_nIRange-I_RANGE0);// uA -> DA
			break;
#else
			str.Format("..ISet(): Check the I Range, first! current m_nIRange(=%d)", m_nIRange);
			TRACE("%s\n", str);
			m_listMsg.InsertString(0, str);
			return;
#endif
	}

	CString strTemp;
	strTemp.Format("%d", I_value);
	GetDlgItem(IDC_STATIC_I_ADC)->SetWindowText(strTemp);

	UINT dwRet = MM.Set(I, I_value);		// sSET_I
#ifndef __NO_LIST__
	{
		CString ErrStr, ErrTitle="???", ErrState="Logic_err?", ErrSol;
		int err = ERR.Get();
		ERR.GetMsg(err, ErrTitle, ErrState, ErrSol);

		ErrStr.Format("..ISet(): sSET_I err=%d %s : %s", 
				err, ErrTitle, ErrState);
		if (err != NO_ERR)
			m_listMsg.InsertString(0, ErrStr);
		TRACE("%s\n\n", ErrStr);
	}
#endif

	// I Get 을 위한 사전 Filter Set -----
	// I Filter가 slow이거나 fast인 경우가 아니라면 I Fast로 필터를 설정한다.
	if (m_nADC_A != ADCA_IFAST && m_nADC_A != ADCA_ISLOW) // 3:  IFFT	(I Fast) 4:ISFT (I Slow)
	{
		MM.Set(IFFT);			// sSET_ADC_A: I_FAST_CMD
		m_nADC_A = ADCA_IFAST;	// UI상의 ADC A filter를 'I Fast'(IFFT) 로 설정한다.
		UpdateData(FALSE);		// m_nADC_A 값을 UI에 반영
	}

	Delay(5, msec);

	double d_I = -1;
	UINT dwAD_I = -1, dwAd_I2;
	dwAD_I = (UINT)MM.Get(I);		// sGET_VI:  read from IO Card
	if(dwAD_I != (UINT)(-1))	// 전송 성공이면  
	{
		dwAd_I2 = dwAD_I;	// UNIT_CONVERT시에 value값이 변형이 올 수 있으므로 카피해서 사용
		d_I = UNIT_CONVERT(dwAd_I2, AD_TO_I0+m_nIRange-I_RANGE0);// DA -> uA 
/*
 		// 2017.11.26 UNIT_CONVERT에서 1/1000 계산해서 리턴하는 것으로 원상복구
		// 하기 코드는 당분간 코멘트상태로 유지하되, 불필요한 것이 확실해지면 지운다.
		switch(m_nIRange)
		{
			case I_RANGE0://mA
			case I_RANGE1://mA
			case I_RANGE2://mA
				d_I *= 1/(double)1000.;	// mA 단위로 변환 
			case I_RANGE3://uA
			case I_RANGE4://uA
				break;
		}
*/
	}
#ifndef __NO_LIST__
	{
		CString ErrStr, ErrTitle="???", ErrState="Logic_err?", ErrSol;
		int err = ERR.Get();
		ERR.GetMsg(err, ErrTitle, ErrState, ErrSol);

		ErrStr.Format("..ISet(): MM.Get(I) d_I=%.3f(AD=%d) err=%d %s : %s", 
			d_I, dwAD_I, err, ErrTitle, ErrState);
		if (err != NO_ERR)
			m_listMsg.InsertString(0, ErrStr);
		TRACE("%s\n\n", ErrStr);
	}
#endif

	m_dIamp = d_I;			// m_lbIamp Label UI에도 반영.
	m_nIadvalue = dwAD_I;

	CString sTemp;
	sTemp.Format("%.3f", d_I); 	
	GetDlgItem(IDC_STATIC_I_MNT)->SetWindowText(sTemp);
	sTemp.Format("%d", dwAD_I); 
	GetDlgItem(IDC_STATIC_I_MNT_AD)->SetWindowText(sTemp);
}

void CManualTestDlg::OnRadioICc() 
{
	//MM.m_pMicroM->Off(CC_CV_MODE);
	MM.Set(CC_CV, _OFF);	// 0: CC, 1: CV
	TRACE("OnRadioICc(): CC_CV_MODE Off(CC)\n\n");
}

void CManualTestDlg::OnRadioICv() 
{
	//MM.m_pMicroM->On(CC_CV_MODE);
	MM.Set(CC_CV, _ON);		// 0: CC, 1: CV
	TRACE("OnRadioICv(): CC_CV_MODE On(CV)\n\n");
}

void CManualTestDlg::OnCheckIHighGain() 
{
	
	UpdateData(TRUE);
	if(m_bIHighGainMode) 
		//MM.m_pMicroM->On(I_GAIN_CMD);	// 1: x 10
		MM.Set(I_GAIN, _ON);			// 1: x 10
	else				
		//MM.m_pMicroM->Off(I_GAIN_CMD);	// 0: x 1
		MM.Set(I_GAIN, _OFF);				// 0: x 1
	
	TRACE("OnCheckIHighGain(): I_GAIN_CMD %s\n\n", (m_bIHighGainMode) ? "ON":"OFF");
}

void CManualTestDlg::OnRadioPftFast() 
{
	//MM.Set(PFFT);	// sSET_ADC_A: PULSE_FILTER_SET(FAST)
}

void CManualTestDlg::OnRadioPftSlow() 
{
	//MM.Set(PSFT);	// sSET_ADC_A:  PULSE_FILTER_SET(SLOW)
}

void CManualTestDlg::OnRadioVbft() 
{
	UINT dwRet = MM.Set(VBFT);    // sSET_ADC_A: ADC_VB_SEL 
	TRACE("OnRadioVbft(): MM.Set(VB SEL Filter) : %s\n\n", 
			(dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadioIfft() 
{
	// I_GAIN_CMD 는 평소에 OFF 여야 하므로 
	// m_bIHighGainMode가 초기화 됐는지 체크하여 I_GAIN_CMD OFF를 수행
	UpdateData(TRUE);
	if (m_bIHighGainMode == FALSE) 
		//MM.m_pMicroM->Off(I_GAIN_CMD);	// 0: x 1
		MM.Set(I_GAIN, _OFF);				// 0: x 1

	UINT dwRet = MM.Set(IFFT);	// sSET_ADC_A: ADC_I_FAST_SEL
	TRACE("OnRadioIfft(): MM.Set(I_FAST Filter) %s.\n\n", 
			(dwRet == FALSE)? "Error" : "OK");

}

void CManualTestDlg::OnRadioIsft() 
{

	// I_GAIN_CMD 는 평소에 OFF 여야 하므로 
	// m_bIHighGainMode가 초기화 됐는지 체크하여 I_GAIN_CMD OFF를 수행
	UpdateData(TRUE);
	if (m_bIHighGainMode == FALSE) 
		//MM.m_pMicroM->Off(I_GAIN_CMD);	// 0: x 1
		MM.Set(I_GAIN, _OFF);				// 0: x 1

	UINT dwRet = MM.Set(ISFT);	// sSET_ADC_A: ADC_I_SLOW_SEL
	TRACE("OnRadioIsft(): MM.Set(I_SLOW Filter) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");

}

void CManualTestDlg::OnRadioPulseMeasureCav0() 
{
	//MM.Set(PULSE, -1, -1, 0);	
}

void CManualTestDlg::OnRadioPulseMeasureCav1() 
{
	//MM.Set(PULSE, -1, -1, 1);
}

void CManualTestDlg::OnRadioPulseMeasureCav2() 
{
	//MM.Set(PULSE, -1, -1, 2);
}

void CManualTestDlg::OnRadioPulseMeasureCav3() 
{
	//MM.Set(PULSE, -1, -1, 3);
}

void CManualTestDlg::OnRadioIRng0() 
{
	m_nI = 0;		    // 이전에 선택된 range의 I 값이 있을 경우를 대비해 초기화
						// 초기화 하지 않으면 range error를 유발함.
	CString str;
	str.Format("%d", m_nI);		// 0 값을 Edit에 반영
	GetDlgItem(IDC_EDIT_I)->SetWindowText(str);
	//m_nIRng = 0;	   	// IDC_RADIO_I_RNG0가 체크된 상태를 유지	

	m_nIRange = I_RANGE0;
	UINT dwRet = MM.Set(I, -1, m_nIRange);		// sSET_I

	TRACE("OnRadioIRng0(): MM.Set(I, -1, range_%d) %s.\n\n", m_nIRange,
			(dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadioIRng1() 
{
	m_nI = 0;		   	// 이전에 선택된 range의 I 값이 있을 경우를 대비해 초기화
	//m_nIRng = 1;		// IDC_RADIO_I_RNG1이 체크된 상태를 유지
	
	CString str;
	str.Format("%d", m_nI);
	GetDlgItem(IDC_EDIT_I)->SetWindowText(str);

	m_nIRange = I_RANGE1;
	UINT dwRet = MM.Set(I, -1, m_nIRange);		// sSET_I

	TRACE("OnRadioIRng1(): MM.Set(I, -1, range_%d) %s.\n\n", m_nIRange,
			(dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadioIRng2() 
{
	m_nI = 0;		   	// 이전에 선택된 range의 I 값이 있을 경우를 대비해 초기화
	//m_nIRng = 2;		// IDC_RADIO_I_RNG2이 체크된 상태를 유지
	
	CString str;
	str.Format("%d", m_nI);
	GetDlgItem(IDC_EDIT_I)->SetWindowText(str);

	m_nIRange = I_RANGE2;
	UINT dwRet = MM.Set(I, -1, m_nIRange);		// sSET_I

	TRACE("OnRadioIRng2(): MM.Set(I, -1, range_%d) %s.\n\n", m_nIRange,
			(dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadioIRng3() 
{
	m_nI = 0;		   	// 이전에 선택된 range의 I 값이 있을 경우를 대비해 초기화
	//m_nIRng = 3;		// IDC_RADIO_I_RNG3이 체크된 상태를 유지
	
	CString str;
	str.Format("%d", m_nI);
	GetDlgItem(IDC_EDIT_I)->SetWindowText(str);

	m_nIRange = I_RANGE3;
	UINT dwRet = MM.Set(I, -1, m_nIRange);		// sSET_I

	TRACE("OnRadioIRng3(): MM.Set(I, -1, range_%d) %s.\n\n", m_nIRange,
			(dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadioIRng4() 
{
	m_nI = 0;		   	// 이전에 선택된 range의 I 값이 있을 경우를 대비해 초기화
	//m_nIRng = 4;		// IDC_RADIO_I_RNG4가 체크된 상태를 유지
	
	CString str;
	str.Format("%d", m_nI);
	GetDlgItem(IDC_EDIT_I)->SetWindowText(str);

	m_nIRange = I_RANGE4;
	UINT dwRet = MM.Set(I, -1, m_nIRange);		// sSET_I

	TRACE("OnRadioIRng4(): MM.Set(I, -1, range_%d) %s.\n\n", m_nIRange,
			(dwRet == FALSE)? "Error" : "OK");
}


void CManualTestDlg::OnRadioIRng6() 
{
	m_nI = 0;		   	// 이전에 선택된 range의 I 값이 있을 경우를 대비해 초기화

	CString str;
	str.Format("%d", m_nI);
	GetDlgItem(IDC_EDIT_I)->SetWindowText(str);

	m_nIRange = I_RANGE6;
	UINT dwRet = MM.Set(I, -1, m_nIRange);		// sSET_I

	TRACE("OnRadioIRng6(): MM.Set(I, -1, range_%d) %s.\n\n", m_nIRange,
			(dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadioVcfft() 
{
	// VC LV, HV 는 평소에 OFF 여야 하므로 
	// m_nHLVC가 초기화 됐는지 체크하여 OFF를 수행 한다.
	UpdateData(TRUE);
	if (m_nHLVC == -1) 
		MM.Set(HVC, FALSE);	// HVC, LVC 의 FALSE 동작이 같으므로 1회 수행

	UINT dwRet = MM.Set(VCFFT);					// sSET_ADC_B
	TRACE("OnRadioVcfft(): MM.Set(VC FAST Filter) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");
	
}

void CManualTestDlg::OnRadioVcsft() 
{
	//170529 VC Slow Filter는 현재 쓰지 않음 (신부성 차장 confirm)
	//MM.Set(VCSFT);					// sSET_ADC_B
}

void CManualTestDlg::OnRadioHvoft() 
{
	UpdateData(TRUE);
	// HV Open 선택시 VC Filter 관련 Sub 항목도 초기화
	m_nHLVC = -1;
	MM.Set(HVC, FALSE);	// False 는 LV, HV 동일하므로 (HVC, FALSE)로 수행

	UINT dwRet = MM.Set(HVOFT);					// sSET_ADC_B
	TRACE("OnRadioHvoft(): MM.Set(HV OPEN Filter) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");

	UpdateData(FALSE);
}

void CManualTestDlg::OnRadioFwft() 
{
	UpdateData(TRUE);
	// 4 Wire 선택시엔 VC Filter 관련 Sub 항목도 초기화
	m_nHLVC = -1;
	MM.Set(HVC, FALSE);	// False 는 LV, HV 동일하므로 (HVC, FALSE)로 수행

	UINT dwRet = MM.Set(FWFT);					// sSET_ADC_B
	TRACE("OnRadioFwft(): MM.Set(4 WIRE Filter) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");

	UpdateData(FALSE);
}




void CManualTestDlg::OnRadioHvc() 
{
	// TODO: Add your control notification handler code here
	int prev_nHLVC = m_nHLVC;	
	
	UpdateData(TRUE);	// 현재의 라디오 체크상태를 변수에 반영

	if (prev_nHLVC == VCFFT_HVC)	// VC Fileter HV가 선택된 상태에서 재선택시 
	{
		// HV의 체크상태를 끈다.
		((CButton*)GetDlgItem(IDC_RADIO_HVC))->SetCheck(FALSE);
		m_nHLVC = -1;
		UINT dwRet = MM.Set(HVC, FALSE);					

		TRACE("OnRadioHvc(): m_nHLVC changed 0(HVC)-> %d, MM.Set(HVC, FALSE) %s.\n\n", 
				m_nHLVC, (dwRet == FALSE)? "Error" : "OK" );
		return;
	}
	
	UINT dwRet = MM.Set(HVC, TRUE);	// HVC, TRUE
	TRACE("OnRadioHvc(): Current m_nHLVC=%d, MM.Set(HVC, TRUE) %s.\n\n", 
				m_nHLVC, (dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadioLvc() 
{
	// TODO: Add your control notification handler code here
	
	int prev_nHLVC = m_nHLVC;	

	UpdateData(TRUE);	// 현재의 라디오 체크상태를 변수에 반영

	if (prev_nHLVC == VCFFT_LVC)	// VC Filter LV가 선택된 상태에서 재선택시 
	{
		// LV의 체크상태를 끈다.
		((CButton*)GetDlgItem(IDC_RADIO_LVC))->SetCheck(FALSE);
		m_nHLVC = -1;
		UINT dwRet = MM.Set(LVC, FALSE);					

		TRACE("OnRadioLvc(): m_nHLVC changed 1(LVC)-> %d, MM.Set(LVC, FALSE) %s.\n\n", 
				m_nHLVC, (dwRet == FALSE)? "Error" : "OK"	);
		return;
	}

	UINT dwRet = MM.Set(LVC, TRUE);	// LVC, TRUE
	TRACE("OnRadioLvc(): Current m_nHLVC=%d, MM.Set(LVC, TRUE) %s.\n\n", 
				m_nHLVC, (dwRet == FALSE)? "Error" : "OK"	);
	
}

void CManualTestDlg::OnRadioHvoGain1() 		// sSET_HVO
{
	UINT dwRet = MM.Set(HVO, 1);
	TRACE("OnRadioHvoGain1(): MM.Set(HVO, 1) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadioHvoGain2() 
{
	UINT dwRet = MM.Set(HVO, 2);
	TRACE("OnRadioHvoGain2(): MM.Set(HVO, 2) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadioHvoGain3() 
{
	UINT dwRet = MM.Set(HVO, 3);
	TRACE("OnRadioHvoGain3(): MM.Set(HVO, 3) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadioHvoGain4() 
{
	UINT dwRet = MM.Set(HVO, 4);
	TRACE("OnRadioHvoGain4(): MM.Set(HVO, 4) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadio4WGain_Sx1000() 		// sSET_FW_GAIN
{
	UINT dwRet = MM.Set(FW_GAIN, 1);// Slow Gain x1000
	TRACE("OnRadio4WGain_Sx1000(): MM.Set(FW_GAIN, 1) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadio4WGain_Fx10() 
{
	UINT dwRet = MM.Set(FW_GAIN, 2);// Fast Gain x10
	TRACE("OnRadio4WGain_Fx10(): MM.Set(FW_GAIN, 2) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadio4WGain_Fx100() 
{
	UINT dwRet = MM.Set(FW_GAIN, 3);// Fast Gain x100
	TRACE("OnRadio4WGain_Fx100(): MM.Set(FW_GAIN, 3) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadio4WGain_Fx1000() 
{
	UINT dwRet = MM.Set(FW_GAIN, 4);// Fast Gain x1000
	TRACE("OnRadio4WGain_Fx1000(): MM.Set(FW_GAIN, 4) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");
}


// 각 컨트롤들의 상태값으로 BPoint(m_ptBTR,m_ptBRY)와 CPoint(m_ptCTR, m_ptCRY)의 값을 계산한다.
void CManualTestDlg::CalcPoint()
{
	/*
	UpdateData(TRUE);
	UINT point=0, box_no=0, PCBenable=0, PCB_no=0, CPLD_no=0, pin_no=0;

	// MAX_BOX = 4, MAX_PCB_ENABLE= 2, MAX_PCB = 16, MAX_CPLD = 2, MAX_PIN = 64
	// 최대 표현 가능 pin => 16384개
	// Enable=0이면 PCB(Card) 갯수는 16개, Enable=1이면 PCB(Card)갯수가 32개가 된다.
	// CPLD=0이면 pin 수가 64개, CPLD=1이면 pin수는 128개가 된다.

	m_nBPCB = ((m_nBPCB3 &1)<<3) + ((m_nBPCB2 &1)<<2) + ((m_nBPCB1 &1)<<1) + (m_nBPCB0 &1);
	m_nBPin = ((m_nBPin5 &1)<<5) + ((m_nBPin4 &1)<<4) + ((m_nBPin3 &1)<<3) + ((m_nBPin2 &1)<<2) + ((m_nBPin1 &1)<<1) + (m_nBPin0 &1);
	box_no = m_nBBox;					// 0 ~ 3
	PCBenable = m_nBPCBEnable;			// 0 ~ 1
	PCB_no = m_nBPCB;					// 0 ~ 15
	CPLD_no = ldiv(m_nBCPLD, 2).rem;	// mnBCPLD /2 의 나머지 => 0 ~ 1
	pin_no = m_nBPin;					// 0 ~ 63

	point = box_no * 4096 + PCBenable * 2048 + PCB_no * 128 + CPLD_no * 64 + pin_no;
	if(m_bBTRY == TR)		m_ptBTR = point;
	else if(m_bBTRY == RY)	m_ptBRY = point;

	m_nCPCB = ((m_nCPCB3 &1)<<3) + ((m_nCPCB2 &1)<<2) + ((m_nCPCB1 &1)<<1) + (m_nCPCB0 &1);
	m_nCPin = ((m_nCPin5 &1)<<5) + ((m_nCPin4 &1)<<4) + ((m_nCPin3 &1)<<3) + ((m_nCPin2 &1)<<2) + ((m_nCPin1 &1)<<1) + (m_nCPin0 &1);
	box_no = m_nCBox;
	PCBenable = m_nCPCBEnable;
	PCB_no = m_nCPCB;
	CPLD_no = ldiv(m_nCCPLD, 2).rem;
	pin_no = m_nCPin;

	point = box_no * 4096 + PCBenable * 2048 + PCB_no * 128 + CPLD_no * 64 + pin_no;
	if(m_bCTRY == TR)		m_ptCTR = point;
	else if(m_bCTRY == RY)	m_ptCRY = point;

	UpdateData(FALSE);
	*/
	return;
}

// LIST_BPOINT 컨트롤에서 선택된 Bpoint 값대로 개별 컨트롤의 상태를 설정한다.
void CManualTestDlg::OnDblclkListBpoint() 
{
	BOOL bTRY = TR;
	CString strTemp;
	int index = m_listBPoint.GetCurSel();
	if(index < 0) return;
	m_listBPoint.GetText(index, strTemp);
	int nBPoint = atoi(strTemp);
	if(strTemp.Find("RY", 0) >=0 ) bTRY = RY;
	m_bBTRY = bTRY;

	// +---+---+----+----+------+
	// |BOX|ENA| PCB|CPLD|   PIN|
	// |   |BLE|    |    |      |
	// |432|  1|0987|   6|543210|
	// +---+---+----+----+------+
	// |111|  1|1111|   1|111111|
	// +---+---+----+----+------+

	// 개별 버튼상태 계산
	m_nBBox = nBPoint>>12;				
	m_nBPCBEnable = (nBPoint>>11) & 1;	
	m_nBPCB0 = (nBPoint>>7) & 1;		
	m_nBPCB1 = (nBPoint>>8) & 1;		
	m_nBPCB2 = (nBPoint>>9) & 1;
	m_nBPCB3 = (nBPoint>>10) & 1;
	m_nBCPLD = ((nBPoint>>6) & 1) + bTRY*2;
	m_nBPin0 = (nBPoint>>0) & 1;
	m_nBPin1 = (nBPoint>>1) & 1;
	m_nBPin2 = (nBPoint>>2) & 1;
	m_nBPin3 = (nBPoint>>3) & 1;
	m_nBPin4 = (nBPoint>>4) & 1;
	m_nBPin5 = (nBPoint>>5) & 1;

	SetFocus( );
	UpdateData(FALSE);
}

// 현재 EDIT_BPIN 의 값대로  Pin 컨트롤의 상태를 설정한다.
void CManualTestDlg::OnSetfocusEditBpin() 
{
	
	UpdateData(TRUE);
	if(m_bNKeyPadOpen == TRUE) return;
	m_bNKeyPadOpen = TRUE;

	if(NKEYPAD.SetValue((int)m_nBPin, 0, 63) == FALSE) return;
	NKEYPAD.DoModal();
	int nBPin = (int)NKEYPAD.GetValue();

	// 개별 버튼상태 계산
	m_nBPin0 = (nBPin>>0) & 1;
	m_nBPin1 = (nBPin>>1) & 1;
	m_nBPin2 = (nBPin>>2) & 1;
	m_nBPin3 = (nBPin>>3) & 1;
	m_nBPin4 = (nBPin>>4) & 1;
	m_nBPin5 = (nBPin>>5) & 1;

	m_bNKeyPadOpen = FALSE;
	SetFocus( );
	UpdateData(FALSE);

}

// 현재 BPoint 값대로 VB Source Point 영역의 TR 관련 컨트롤 상태를 설정한다.
void CManualTestDlg::OnSetfocusEditBpointTr() 
{
	UpdateData(TRUE);
	if(m_bNKeyPadOpen == TRUE) return;
	m_bNKeyPadOpen = TRUE;

	// 20170626: 신부성 차장 요청. Switch Card pin 번호를 1부터 시작하도록 변경
	//if(NKEYPAD.SetValue((int)m_ptBTR, 0, 32767) == FALSE) return;
	
	if(NKEYPAD.SetValue((int)m_ptBTR, 1, MAX_MODEL_PIN) == FALSE) return;
	NKEYPAD.DoModal();
	int nBPoint = (int)NKEYPAD.GetValue();
	m_ptBTR = nBPoint;

	// 개별 버튼상태 계산
	m_nBBox = nBPoint>>12;
	m_nBPCBEnable = (nBPoint>>11) & 1;
	m_nBPCB0 = (nBPoint>>7) & 1;
	m_nBPCB1 = (nBPoint>>8) & 1;
	m_nBPCB2 = (nBPoint>>9) & 1;
	m_nBPCB3 = (nBPoint>>10) & 1;
	m_nBCPLD = ((nBPoint>>6) & 1) + TR*2;
	m_nBPin0 = (nBPoint>>0) & 1;
	m_nBPin1 = (nBPoint>>1) & 1;
	m_nBPin2 = (nBPoint>>2) & 1;
	m_nBPin3 = (nBPoint>>3) & 1;
	m_nBPin4 = (nBPoint>>4) & 1;
	m_nBPin5 = (nBPoint>>5) & 1;

	m_bBTRY = TR;
	m_bNKeyPadOpen = FALSE;
	SetFocus( );
	UpdateData(FALSE);	
}

// 현재 BPoint 값대로 VB Source Point 영역의 RY 관련 컨트롤 상태를 설정한다.
void CManualTestDlg::OnSetfocusEditBpointRy() 
{
	UpdateData(TRUE);
	if(m_bNKeyPadOpen == TRUE) return;
	m_bNKeyPadOpen = TRUE;

	// 20170626: 신부성 차장 요청. Switch Card pin 번호를 1부터 시작하도록 변경
	//if(NKEYPAD.SetValue((int)m_ptBRY, 0, 32767) == FALSE) return;
	if(NKEYPAD.SetValue((int)m_ptBRY, 1, MAX_MODEL_PIN) == FALSE) return;
	NKEYPAD.DoModal();
	int nBPoint = (int)NKEYPAD.GetValue();
	m_ptBRY = nBPoint;

	// 개별 버튼상태 계산
	m_nBBox = nBPoint>>12;
	m_nBPCBEnable = (nBPoint>>11) & 1;
	m_nBPCB0 = (nBPoint>>7) & 1;
	m_nBPCB1 = (nBPoint>>8) & 1;
	m_nBPCB2 = (nBPoint>>9) & 1;
	m_nBPCB3 = (nBPoint>>10) & 1;
	m_nBCPLD = ((nBPoint>>6) & 1) + RY*2;
	m_nBPin0 = (nBPoint>>0) & 1;
	m_nBPin1 = (nBPoint>>1) & 1;
	m_nBPin2 = (nBPoint>>2) & 1;
	m_nBPin3 = (nBPoint>>3) & 1;
	m_nBPin4 = (nBPoint>>4) & 1;
	m_nBPin5 = (nBPoint>>5) & 1;

	m_bBTRY = RY;
	m_bNKeyPadOpen = FALSE;
	SetFocus( );
	UpdateData(FALSE);	
}

// TR EDIT 창의 값을 LIST_BPOINT 리스트에 추가하고 BPoint Set을 수행한다.
void CManualTestDlg::OnButtonBpointTrSet() 
{
	
	UpdateData(TRUE);

	UINT point_no=m_ptBTR;

	// 20170626: 신부성 차장 요청. Switch Card pin 번호를 1부터 시작하도록 변경
	// Switch Card로 전송 직전에 -1해서 HW에서는 0부터 처리한다.
	UINT dwRet = MM.Set(BPOINT, point_no-1, TR);
	TRACE("..BpointTrSet(): MM.Set(BPOINT, point_%d, TR) %s.\n\n",
			point_no,
			(dwRet == FALSE)? "Error" : "OK");

	CString strTemp;
	strTemp.Format("%d TR", point_no);
	int index = m_listBPoint.FindString(-1, strTemp);
	if(index == LB_ERR)	
		m_listBPoint.InsertString(0, strTemp);
	
}

// TR EDIT 창의 값을 LIST_BPOINT 리스트에서 제거하고 BPoint Reset을 수행한다.
void CManualTestDlg::OnButtonBpointTrRst() 
{
	
	UpdateData(TRUE);

	UINT point_no=m_ptBTR;

	// 20170626: 신부성 차장 요청. Switch Card pin 번호를 1부터 시작하도록 변경
	// Switch Card로 전송 직전에 -1해서 HW에서는 0부터 처리한다.
	UINT dwRet = MM.Rst(BPOINT, point_no-1, TR);
	TRACE("..BpointTrRst(): MM.Rst(BPOINT, point_%d, TR) %s.\n\n",
			point_no,
			(dwRet == FALSE)? "Error" : "OK");

	CString strTemp;
	strTemp.Format("%d TR", point_no);
	int index = m_listBPoint.FindString(-1, strTemp);
	if(index != LB_ERR)	
		m_listBPoint.DeleteString(index);
	
}

// RY EDIT 창의 값을 LIST_BPOINT 리스트에 추가한다.
void CManualTestDlg::OnButtonBpointRySet() 
{
	
	UpdateData(TRUE);
	UINT point_no=m_ptBRY;

	// 20170626: 신부성 차장 요청. Switch Card pin 번호를 1부터 시작하도록 변경
	// Switch Card로 전송 직전에 -1해서 HW에서는 0부터 처리한다.
	UINT dwRet = MM.Set(BPOINT, point_no-1, RY);
	TRACE("..BpointRySet(): MM.Set(BPOINT, point_%d, RY) %s.\n\n",
			point_no,
			(dwRet == FALSE)? "Error" : "OK");

	CString strTemp;
	strTemp.Format("%d RY", point_no);
	int index = m_listBPoint.FindString(-1, strTemp);
	if(index == LB_ERR)	
		m_listBPoint.InsertString(0, strTemp);
	
}

// RY EDIT 창의 값을 LIST_BPOINT 리스트에서 제거한다.
void CManualTestDlg::OnButtonBpointRyRst() 
{
	
	UpdateData(TRUE);
	UINT point_no=m_ptBRY;

	// 20170626: 신부성 차장 요청. Switch Card pin 번호를 1부터 시작하도록 변경
	// Switch Card로 전송 직전에 -1해서 HW에서는 0부터 처리한다.
	UINT dwRet = MM.Rst(BPOINT, point_no-1, RY);
	TRACE("..BpointRyRst(): MM.Rst(BPOINT, point_%d, RY) %s.\n\n",
			point_no,
			(dwRet == FALSE)? "Error" : "OK");

	CString strTemp;
	strTemp.Format("%d RY", point_no);
	int index = m_listBPoint.FindString(-1, strTemp);
	if(index != LB_ERR)	
		m_listBPoint.DeleteString(index);

}

// LIST_CPOINT 컨트롤에서 선택된 Cpoint 값대로 개별 컨트롤의 상태를 설정한다.
void CManualTestDlg::OnDblclkListCpoint() 
{
	BOOL bTRY = TR;
	CString strTemp;
	int index = m_listCPoint.GetCurSel();
	if(index < 0) return;
	m_listCPoint.GetText(index, strTemp);
	int nCPoint = atoi(strTemp);
	if(strTemp.Find("RY", 0) >=0 ) bTRY = RY;
	m_bCTRY = bTRY;

	// 개별 버튼상태 계산
	m_nCBox = nCPoint>>12;
	m_nCPCBEnable = (nCPoint>>11) & 1;
	m_nCPCB0 = (nCPoint>>7) & 1;
	m_nCPCB1 = (nCPoint>>8) & 1;
	m_nCPCB2 = (nCPoint>>9) & 1;
	m_nCPCB3 = (nCPoint>>10) & 1;
	m_nCCPLD = ((nCPoint>>6) & 1) + bTRY*2;
	m_nCPin0 = (nCPoint>>0) & 1;
	m_nCPin1 = (nCPoint>>1) & 1;
	m_nCPin2 = (nCPoint>>2) & 1;
	m_nCPin3 = (nCPoint>>3) & 1;
	m_nCPin4 = (nCPoint>>4) & 1;
	m_nCPin5 = (nCPoint>>5) & 1;

	SetFocus( );
	UpdateData(FALSE);
}

// 현재 EDIT_CPIN 의 값대로  Pin 컨트롤의 상태를 설정한다.
void CManualTestDlg::OnSetfocusEditCpin() 
{
	
	UpdateData(TRUE);
	if(m_bNKeyPadOpen == TRUE) return;
	m_bNKeyPadOpen = TRUE;

	if(NKEYPAD.SetValue((int)m_nCPin, 0, 63) == FALSE) return;
	NKEYPAD.DoModal();
	int nCPin = (int)NKEYPAD.GetValue();

	// 개별 버튼상태 계산
	m_nCPin0 = (nCPin>>0) & 1;
	m_nCPin1 = (nCPin>>1) & 1;
	m_nCPin2 = (nCPin>>2) & 1;
	m_nCPin3 = (nCPin>>3) & 1;
	m_nCPin4 = (nCPin>>4) & 1;
	m_nCPin5 = (nCPin>>5) & 1;

	m_bNKeyPadOpen = FALSE;
	SetFocus( );
	UpdateData(FALSE);
	
}

// 현재 CPoint 값대로 VC Sink Point 영역의 TR 관련 컨트롤 상태를 설정한다.
void CManualTestDlg::OnSetfocusEditCpointTr() 
{
	
	UpdateData(TRUE);
	if(m_bNKeyPadOpen == TRUE) return;
	m_bNKeyPadOpen = TRUE;

	// 20170626: 신부성 차장 요청. Switch Card pin 번호를 1부터 시작하도록 변경
	//if(NKEYPAD.SetValue((int)m_ptCTR, 0, 32767) == FALSE) return;
	if(NKEYPAD.SetValue((int)m_ptCTR, 1, MAX_MODEL_PIN) == FALSE) return;
	NKEYPAD.DoModal();
	int nCPoint = (int)NKEYPAD.GetValue();
	m_ptCTR = nCPoint;

	// 개별 버튼상태 계산
	m_nCBox = nCPoint>>12;
	m_nCPCBEnable = (nCPoint>>11) & 1;
	m_nCPCB0 = (nCPoint>>7) & 1;
	m_nCPCB1 = (nCPoint>>8) & 1;
	m_nCPCB2 = (nCPoint>>9) & 1;
	m_nCPCB3 = (nCPoint>>10) & 1;
	m_nCCPLD = ((nCPoint>>6) & 1) + TR*2;
	m_nCPin0 = (nCPoint>>0) & 1;
	m_nCPin1 = (nCPoint>>1) & 1;
	m_nCPin2 = (nCPoint>>2) & 1;
	m_nCPin3 = (nCPoint>>3) & 1;
	m_nCPin4 = (nCPoint>>4) & 1;
	m_nCPin5 = (nCPoint>>5) & 1;
	
	m_bCTRY = TR;
	m_bNKeyPadOpen = FALSE;
	SetFocus( );
	UpdateData(FALSE);	
	
}

// 현재 CPoint 값대로 VC Sink Point 영역의 RY 관련 컨트롤 상태를 설정한다.
void CManualTestDlg::OnSetfocusEditCpointRy() 
{

	UpdateData(TRUE);
	if(m_bNKeyPadOpen == TRUE) return;
	m_bNKeyPadOpen = TRUE;

	// 20170626: 신부성 차장 요청. Switch Card pin 번호를 1부터 시작하도록 변경
	//if(NKEYPAD.SetValue((int)m_ptCRY, 0, 32767) == FALSE) return;
	if(NKEYPAD.SetValue((int)m_ptCRY, 1, MAX_MODEL_PIN) == FALSE) return;
	NKEYPAD.DoModal();
	int nCPoint = (int)NKEYPAD.GetValue();
	m_ptCRY = nCPoint;

	// 개별 버튼상태 계산
	m_nCBox = nCPoint>>12;
	m_nCPCBEnable = (nCPoint>>11) & 1;
	m_nCPCB0 = (nCPoint>>7) & 1;
	m_nCPCB1 = (nCPoint>>8) & 1;
	m_nCPCB2 = (nCPoint>>9) & 1;
	m_nCPCB3 = (nCPoint>>10) & 1;
	m_nCCPLD = ((nCPoint>>6) & 1) + RY*2;
	m_nCPin0 = (nCPoint>>0) & 1;
	m_nCPin1 = (nCPoint>>1) & 1;
	m_nCPin2 = (nCPoint>>2) & 1;
	m_nCPin3 = (nCPoint>>3) & 1;
	m_nCPin4 = (nCPoint>>4) & 1;
	m_nCPin5 = (nCPoint>>5) & 1;
	
	m_bCTRY = RY;
	m_bNKeyPadOpen = FALSE;
	SetFocus( );
	UpdateData(FALSE);	
	
}

// TR EDIT 창의 값을 LIST_CPOINT 리스트에 추가한다.
void CManualTestDlg::OnButtonCpointTrSet() 
{
	UpdateData(TRUE);

	UINT point_no=m_ptCTR; 

	// 20170626: 신부성 차장 요청. Switch Card pin 번호를 1부터 시작하도록 변경
	// Switch Card로 전송 직전에 -1해서 HW에서는 0부터 처리한다.
	UINT dwRet = MM.Set(CPOINT, point_no-1, TR);
	TRACE("..CpointTrSet(): MM.Set(CPOINT, point_%d, TR) %s.\n\n",
			point_no,
			(dwRet == FALSE)? "Error" : "OK");

	CString strTemp;
	strTemp.Format("%d TR", point_no);
	if(m_listCPoint.FindString(-1, strTemp) == LB_ERR)	m_listCPoint.InsertString(0, strTemp);
}

// TR EDIT 창의 값을 LIST_CPOINT 리스트에서 제거한다.
void CManualTestDlg::OnButtonCpointTrRst() 
{
	UpdateData(TRUE);

	UINT point_no=m_ptCTR;

	// 20170626: 신부성 차장 요청. Switch Card pin 번호를 1부터 시작하도록 변경
	// Switch Card로 전송 직전에 -1해서 HW에서는 0부터 처리한다.
	UINT dwRet = MM.Rst(CPOINT, point_no-1, TR);
	TRACE("..CpointTrRst(): MM.Rst(CPOINT, point_%d, TR) %s.\n\n",
			point_no,
			(dwRet == FALSE)? "Error" : "OK");

	CString strTemp;
	strTemp.Format("%d TR", point_no);
	int index = m_listCPoint.FindString(-1, strTemp);
	if(index != LB_ERR)	
		m_listCPoint.DeleteString(index);
}

// RY EDIT 창의 값을 LIST_CPOINT 리스트에 추가한다.
void CManualTestDlg::OnButtonCpointRySet() 
{
	
	UpdateData(TRUE);

	UINT point_no=m_ptCRY;

	// 20170626: 신부성 차장 요청. Switch Card pin 번호를 1부터 시작하도록 변경
	// Switch Card로 전송 직전에 -1해서 HW에서는 0부터 처리한다.
	UINT dwRet = MM.Set(CPOINT, point_no-1, RY);
	TRACE("..CpointRySet(): MM.Set(CPOINT, point_%d, RY) %s.\n\n",
			point_no,
			(dwRet == FALSE)? "Error" : "OK");

	CString strTemp;
	strTemp.Format("%d RY", point_no);
	if(m_listCPoint.FindString(-1, strTemp) == LB_ERR)	
		m_listCPoint.InsertString(0, strTemp);	
	
}

// RY EDIT 창의 값을 LIST_CPOINT 리스트에서 제거한다.
void CManualTestDlg::OnButtonCpointRyRst() 
{
	
	UpdateData(TRUE);

	UINT point_no=m_ptCRY;

	// 20170626: 신부성 차장 요청. Switch Card pin 번호를 1부터 시작하도록 변경
	// Switch Card로 전송 직전에 -1해서 HW에서는 0부터 처리한다.
	UINT dwRet = MM.Rst(CPOINT, point_no-1, RY);
	TRACE("..CpointRyRst(): MM.Rst(CPOINT, point_%d, RY) %s.\n\n",
			point_no,
			(dwRet == FALSE)? "Error" : "OK");

	CString strTemp;
	strTemp.Format("%d RY", point_no);
	int index = m_listCPoint.FindString(-1, strTemp);
	if(index != LB_ERR)	m_listCPoint.DeleteString(index);
	
}

void CManualTestDlg::OnRadioLv1() 
{
	// LV apply인데 HV 필터로 잘못 설정돼 있다면 에러 출력. HW 손상은 아니지만 잘못된 결과 나올 수  
	if (m_nHLVC == VCFFT_HVC) 
	{
		CString strTemp("");
		strTemp.Format("OnRadioLv1(): m_nHLVC=%d(HVC), Stop LV VB Apply !\n", m_nHLVC);
		m_listMsg.InsertString(0, strTemp);
		ERR.Set(MAN_ERR_TEST_CONDEITION, strTemp);
		ErrMsg(); ERR.Reset();
		m_nVB = -1;		// 버튼을 안 누른상태로 원복
		//((CButton*)GetDlgItem(IDC_RADIO_LV1))->SetCheck(0);	
		UpdateData(FALSE);
		return;
	}

	UINT dwRet = MM.Apply(LV1);
	TRACE("OnRadioLv1(): MM.Apply %s\n\n", (dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadioLv2() 
{
	if (m_nHLVC == VCFFT_HVC) 
	{
		CString strTemp("");
		strTemp.Format("OnRadioLv2(): m_nHLVC=%d(HVC), Stop LV VB Apply!\n", m_nHLVC);
		m_listMsg.InsertString(0, strTemp);
		ERR.Set(MAN_ERR_TEST_CONDEITION, strTemp);
		ErrMsg(); ERR.Reset();
		m_nVB = -1;		// 버튼을 안 누른상태로 원복
		//((CButton*)GetDlgItem(IDC_RADIO_LV2)->SetCheck(0);	
		UpdateData(FALSE);
		return;
	}
	UINT dwRet = MM.Apply(LV2);
	TRACE("OnRadioLv2(): MM.Apply %s\n\n", (dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadioHv1() 
{
	CString strTemp("");
	if (m_nADC_B == ADCB_4WIRE)
	{
		strTemp.Format("OnRadioHv1(): ADC B Filter = %d(4W), Stop HV VB Apply for HW Proect!\n", m_nADC_B);
		ERR.Set(MAN_ERR_TEST_CONDEITION, strTemp);
		ErrMsg(); ERR.Reset();
		m_nVB = -1;		// 버튼을 안 누른상태로 원복
		//((CButton*)GetDlgItem(IDC_RADIO_HV1)->SetCheck(0);	
		UpdateData(FALSE);
		return;
	}

	// HV apply인데 LV 필터로 잘못 설정돼 있다면 HW 손상될 수 있음.
	if (m_nHLVC == VCFFT_LVC) 
	{
		strTemp.Format("OnRadioHv1(): m_nHLVC=%d(LVC), Stop HV VB Apply for HW Protect!\n", m_nHLVC);
		m_listMsg.InsertString(0, strTemp);
		ERR.Set(MAN_ERR_TEST_CONDEITION, strTemp);
		ErrMsg(); ERR.Reset();
		m_nVB = -1;		// 버튼을 안 누른상태로 원복
		//((CButton*)GetDlgItem(IDC_RADIO_HV1)->SetCheck(0);	
		UpdateData(FALSE);
		return;
	}


	UINT dwRet = MM.Apply(HV1);
	TRACE("OnRadioHv1(): MM.Apply %s\n\n", (dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadioHv2() 
{
	CString strTemp("");
	if (m_nADC_B == ADCB_4WIRE)
	{
		strTemp.Format("OnRadioHv2(): ADC B Filter = %d(4W), Stop HV VB Apply for HW Proect!\n", m_nADC_B);
		ERR.Set(MAN_ERR_TEST_CONDEITION, strTemp);
		ErrMsg(); ERR.Reset();
		m_nVB = -1;		// 버튼을 안 누른상태로 원복
		//((CButton*)GetDlgItem(IDC_RADIO_HV2)->SetCheck(0);	
		UpdateData(FALSE);
		return;
	}

	// HV apply인데 LV 필터로 잘못 설정돼 있다면 HW 손상될 수 있음.
	if (m_nHLVC == VCFFT_LVC) 
	{
		strTemp.Format("OnRadioHv2(): m_nHLVC=%d(LVC), Stop HV VB Apply for HW Protect!\n", m_nHLVC);
		m_listMsg.InsertString(0, strTemp);
		ERR.Set(MAN_ERR_TEST_CONDEITION, strTemp);
		ErrMsg(); ERR.Reset();
		m_nVB = -1;		// 버튼을 안 누른상태로 원복
		//((CButton*)GetDlgItem(IDC_RADIO_HV2)->SetCheck(0);	
		UpdateData(FALSE);
		return;
	}

	UINT dwRet = MM.Apply(HV2);
	TRACE("OnRadioHv2(): MM.Apply %s\n\n", (dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnRadioPulse() 
{
	//UINT dwRet = MM.Apply(PULSE);	
	//TRACE("OnRadioPulse(): MM.Apply %s\n\n", (dwRet == FALSE)? "Error" : "OK");
}

void CManualTestDlg::OnCheckDcOut() 
{
//	UpdateData(TRUE);
//	if(m_bDCOut) MM.m_pMicroM->On(DC_RELAY_OUT);
//	else		 MM.m_pMicroM->Off(DC_RELAY_OUT);

}

void CManualTestDlg::OnButtonBreak() 
{
	// 전원 차단
//	MM.Break();
//	InitMember();

	UINT dwRet = MM.Break(VB_ONLY);
	if (dwRet == FALSE) 
	{	ErrMsg(MB_OK, FALSE); ERR.Reset();  }	// bTraceOn=FALSE, 에러를 Trace 빼고 메시지박스만 출력
	TRACE("..Break(): MM.Break(VB_ONLY) %s\n\n", (dwRet == FALSE)? "Error" : "OK");

	// VB Apply의 모든 버튼을 unchecked 상태로 초기화한다.
	((CButton*)GetDlgItem(IDC_RADIO_LV1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_LV2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_HV1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_HV2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_PULSE))->SetCheck(0);

	GetDlgItem(IDC_STATIC_LV1_MNT)->SetWindowText("0.0");
	GetDlgItem(IDC_STATIC_LV2_MNT)->SetWindowText("0.0");
	GetDlgItem(IDC_STATIC_HV1_MNT)->SetWindowText("0.0");
	GetDlgItem(IDC_STATIC_HV2_MNT)->SetWindowText("0.0");

	GetDlgItem(IDC_STATIC_LV1_MNT_AD)->SetWindowText("0.0");
	GetDlgItem(IDC_STATIC_LV2_MNT_AD)->SetWindowText("0.0");
	GetDlgItem(IDC_STATIC_HV1_MNT_AD)->SetWindowText("0.0");
	GetDlgItem(IDC_STATIC_HV2_MNT_AD)->SetWindowText("0.0");
}

void CManualTestDlg::OnSetfocusEditQmBpoint() 
{
	OnSetfocusEditBpointTr();	
}

void CManualTestDlg::OnSetfocusEditQmCpoint() 
{
	OnSetfocusEditCpointTr();	
}

void CManualTestDlg::OnButtonPointSet() 
{
	
	GetDlgItem(IDC_BUTTON_POINT_SET)->EnableWindow(FALSE);

	OnButtonBpointTrSet();
	OnButtonCpointTrSet();

	GetDlgItem(IDC_BUTTON_POINT_SET)->EnableWindow(TRUE);
	
}

void CManualTestDlg::OnButtonBoxRst() 
{
	UINT dwRet;	
	GetDlgItem(IDC_BUTTON_BOX_RST)->EnableWindow(FALSE);

	dwRet = MM.RstPointAll(PT_VB);		// VB Point All Reset
	TRACE("OnButtonBoxRst(): MM.RstPointAll(PT_VB) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");

	dwRet = MM.RstPointAll (PT_VC);		// VC point All Reset
	TRACE("OnButtonBoxRst(): MM.RstPointAll(PT_VC) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");

	m_listBPoint.ResetContent();
	m_listCPoint.ResetContent();
	GetDlgItem(IDC_BUTTON_BOX_RST)->EnableWindow(TRUE);
	
}

void CManualTestDlg::OnButtonAdRead() 
{
	UpdateData(TRUE);
	CString str;
	//UINT out_value=0;


	//--------------------------------
	// I Get을 위한 ADC A 필터 체크
	// ADC A Filter가 I Slow 이거나 I Fast가 아니면 AD Read를 수행하지 않는다.
	if (m_nADC_A != ADCA_IFAST && m_nADC_A != ADCA_ISLOW) 
	{
		str.Format("..AdRead(): Check the ADC-A filter, first !");
		m_listMsg.InsertString(0, str); 
		TRACE("%s\n", str);
		return; 
	}

	// I Range 체크
	switch(m_nIRange)
	{
		case I_RANGE0: case I_RANGE1: case I_RANGE2: case I_RANGE3: case I_RANGE4:
			break;
		default:
			str.Format("..AdRead(): Check the I Range! current m_nIRange(=%d)", m_nIRange);
			m_listMsg.InsertString(0, str);
			TRACE("%s\n", str);
			return;
	}

	//-----------------------------
	// VC Get을 위한 ADC B 필터 체크
	if (m_nADC_B != ADCB_VCFAST && m_nADC_B != ADCB_HVOPEN && m_nADC_B != ADCB_4WIRE)
	{
	/*
		//20170622: 신부성 차장 요청. 강제설정은 가급적 하지 않도록 아래 코드 삭제
		
		// VC Fast Filter 자동 선택 
		MM.Set(HVC, FALSE);	// False 는 LV, HV 동일하므로 (HVC, FALSE)로 수행
		MM.Set(VCFFT);
		m_nADC_B = ADCB_VCFAST; // UI 상의 ADC B Filter를 'VC Fast' 로 설정한다. 
	*/

		str.Format("..AdRead(): Check the ADC-B filter !");
		m_listMsg.InsertString(0, str); 
		TRACE("%s\n", str);
		return; // AD Read를 수행하지 않고 리턴
	}

	//--------------------------
	// VB Apply 체크
	
	// VB 가 인가되어 있지 않으면 AD Read를 수행하지 않는다.
	switch(m_nVB)
	{
		case VB_LV1: case VB_LV2:
		case VB_HV1: case VB_HV2:
		//case VB_PULSE:  	// TBD..
			break;
		default:
			str.Format("..AdRead(): Check the VB apply !");
			m_listMsg.InsertString(0, str);
			TRACE("%s\n", str);
			GetDlgItem(IDC_BUTTON_AD_READ)->EnableWindow(TRUE);
			return;
	}

	if (m_nADC_B == ADCB_VCFAST)
	{
		switch(m_nVB)
		{
		case VB_LV1: case VB_LV2:		// LV VC Fast Filter를 선택
		/*
			//20170622: 신부성 차장 요청. 강제설정은 가급적 하지 않도록 아래 코드 삭제하고 에러출력으로 대체
			// VB Fast LV/HV 자동 설정 : 
			MM.Set(LVC, TRUE);
			m_nHLVC =  VCFFT_LVC; // VB apply가 LV면 UI 상의 HV/LV radio를 'LV'로 설정
			UpdateData(FALSE);
		*/
			if (m_nHLVC == VCFFT_HVC) // LV apply인데 HV 필터로 잘못 설정돼 있다면
			{
				str.Format("..AdRead(): Check the ADC_B Filter-> VC Fast-> HV/LV Error!");
				m_listMsg.InsertString(0, str);
				TRACE("%s\n", str);
				GetDlgItem(IDC_BUTTON_AD_READ)->EnableWindow(TRUE);
				return;
			}
			break;

		case VB_HV1: case VB_HV2:		// HV VC Fast Filter를 선택
		/*
			//20170622: 신부성 차장 요청. 강제설정은 가급적 하지 않도록 아래 코드 삭제하고 에러출력으로 대체
			MM.Set(HVC, TRUE);
			m_nHLVC =  VCFFT_HVC; // VB apply가 HV면 UI 상의 HV/LV radiio를 'HV'로 설정
			UpdateData(FALSE);
		*/
			if (m_nHLVC == VCFFT_LVC) // HV apply인데 LV 필터로 잘못 설정돼 있다면
			{
				str.Format("..AdRead(): Check the ADC_B Filter-> VC Fast-> HV/LV Error!");
				m_listMsg.InsertString(0, str);
				TRACE("%s\n", str);
				GetDlgItem(IDC_BUTTON_AD_READ)->EnableWindow(TRUE);
				return;
			}
			break;
		}

	}
	
	GetDlgItem(IDC_BUTTON_AD_READ)->EnableWindow(FALSE);
	m_listMsg.ResetContent();




	//----------------
	// A/D 값 읽기 
	// : ADC_A(측정 I) 와 ADC_B(측정 V: VC) 값을 모두 read한다.
	UINT temp = MM.Get(VI);		
	if(temp == (UINT)(-1))			// sGET_VI:  read from IO Card
	{
		str.Format("..AdRead(): MM.Get(VI) Fail!");
		m_listMsg.InsertString(0, str);
		TRACE("%s\n", str);
		GetDlgItem(IDC_BUTTON_AD_READ)->EnableWindow(TRUE);
		return;
	}

	UINT nIadvalue = temp     & 0xffff;
	UINT nVadvalue = temp>>16 & 0xffff;
	m_nIadvalue = nIadvalue;
	m_nVadvalue = nVadvalue;
	m_dIamp = UNIT_CONVERT(nIadvalue, AD_TO_I0 + m_nIRange-I_RANGE0);	// mA
/*
 	// 2017.11.26 UNIT_CONVERT에서 1/1000 계산해서 리턴하는 것으로 원상복구
	// 하기 코드는 당분간 코멘트상태로 유지하되, 불필요한 것이 확실해지면 지운다.
	switch(m_nIRange)
	{
		case I_RANGE0://mA
		case I_RANGE1://mA
		case I_RANGE2://mA
			m_dIamp *= 1/(double)1000.;	// mA 단위로 변환 
		case I_RANGE3://uA
		case I_RANGE4://uA
			break;
	}
*/


	//-------------------------------------
	// VB Apply Type에 따라 Volt(VC) 값을 변환 
	

	//-------------------------------
	// 4W 인 경우 Volt(VC) 값을 변환
	if (m_nADC_B == ADCB_4WIRE)
		m_dVCvolt = UNIT_CONVERT(nVadvalue, AD_TO_4W_VOLT);

	else // ADCB_VCFAT이거나, ADCB_HVOPEN 인경우
	//if (m_nADC_B == ADCB_VCFAST)
	{
		switch(m_nVB)
		{
		case VB_LV1: case VB_LV2:		// LV
			m_dVCvolt = UNIT_CONVERT(nVadvalue, AD_TO_LVOLT);
			break;

		case VB_HV1: case VB_HV2:		// HV
			m_dVCvolt = UNIT_CONVERT(nVadvalue, AD_TO_HVOLT);
			break;

		default:
			m_dVCvolt = 0;
		}
	}



	GetDlgItem(IDC_BUTTON_AD_READ)->EnableWindow(TRUE);
	TRACE("..AdRead(): m_nIadval=%d, m_nVadval=%d, m_dVCvolt=%.3f, m_dIamp=%.3f\n\n",
						m_nIadvalue, m_nVadvalue, m_dVCvolt, m_dIamp);
}

void CManualTestDlg::OnRadioBcpld0() 
{
	m_bBTRY = TR;
}

void CManualTestDlg::OnRadioBcpld1() 
{
	m_bBTRY = TR;
}

void CManualTestDlg::OnRadioBcpld2() 
{
	m_bBTRY = RY;
}

void CManualTestDlg::OnRadioBcpld3() 
{
	m_bBTRY = RY;
}

void CManualTestDlg::OnRadioCcpld0() 
{
	m_bCTRY = TR;
}

void CManualTestDlg::OnRadioCcpld1() 
{
	m_bCTRY = TR;
}

void CManualTestDlg::OnRadioCcpld2() 
{
	m_bCTRY = RY;
}

void CManualTestDlg::OnRadioCcpld3() 
{
	m_bCTRY = RY;
}

void CManualTestDlg::OnCheckVbGuardOn() 
{
	/*
	UpdateData(TRUE);
	if(m_bVBGuardOn) MM.m_pMicroM->On(VB_GUARD_ON);
	else			 MM.m_pMicroM->Off(VB_GUARD_ON);
	*/
}

void CManualTestDlg::OnRadioBpointCv() 
{
	//MM.m_pMicroM->Off(VB_GUARD_SEL);
}

void CManualTestDlg::OnRadioBpointCc() 
{
	//MM.m_pMicroM->On(VB_GUARD_SEL);
}

void CManualTestDlg::OnSetfocusEditBpointCcV() 
{
	/*
	UpdateData(TRUE);
	if(m_bNKeyPadOpen == TRUE) return;
	m_bNKeyPadOpen = TRUE;

	if(NKEYPAD.SetValue((int)m_nBCCV, 0, 20) == FALSE) return;
	NKEYPAD.DoModal();
	m_nBCCV = (int)NKEYPAD.GetValue();

	m_bNKeyPadOpen = FALSE;
	SetFocus( );
	UpdateData(FALSE);	
	*/
}

void CManualTestDlg::OnButtonBpointCcvSet() 
{
	/*
	UpdateData(TRUE);
	UINT module_id = LV1;
	UINT in_value = m_nBCCV, out_value = 0;

	UNIT_CONVERT(in_value, VOLT_TO_DA);
	MM.Set(module_id, in_value);

	MM.Set(LVB, module_id);
	MM.Set(VBFT);

	CString sTemp;
	UINT temp = MM.Get(V);
	sTemp.Format("%.3f", UNIT_CONVERT(temp, AD_TO_LVOLT));
	GetDlgItem(IDC_STATIC_LV1_MNT)->SetWindowText(sTemp);
	*/
}

void CManualTestDlg::OnCheckVcGuardOn() 
{
	/*
	UpdateData(TRUE);
	if(m_bVCGuardOn) MM.m_pMicroM->On(VC_GUARD_ON);
	else			 MM.m_pMicroM->Off(VC_GUARD_ON);

	RefreshView();
	*/
}

void CManualTestDlg::OnRadioCpointCv() 
{
//	MM.m_pMicroM->Off(VC_GUARD_SEL);	
}

void CManualTestDlg::OnRadioCpointCc() 
{
//	MM.m_pMicroM->On(VC_GUARD_SEL);
}

void CManualTestDlg::OnSetfocusEditCpointCcV() 
{
	/*
	UpdateData(TRUE);
	if(m_bNKeyPadOpen == TRUE) return;
	m_bNKeyPadOpen = TRUE;

	if(NKEYPAD.SetValue((int)m_nCCCV, 0, 20) == FALSE) return;
	NKEYPAD.DoModal();
	m_nCCCV = (int)NKEYPAD.GetValue();

	m_bNKeyPadOpen = FALSE;
	SetFocus( );
	UpdateData(FALSE);	
	*/
}

void CManualTestDlg::OnButtonCpointCcvSet() 
{
	/*
	UpdateData(TRUE);
	UINT module_id = LV1;
	UINT in_value = m_nCCCV, out_value = 0;

	UNIT_CONVERT(in_value, VOLT_TO_DA);
	MM.Set(module_id, in_value);

	MM.Set(LVB, module_id);
	MM.Set(VBFT);

	CString sTemp;
	UINT temp = MM.Get(V);
	sTemp.Format("%.3f", UNIT_CONVERT(temp, AD_TO_LVOLT));
	GetDlgItem(IDC_STATIC_LV1_MNT)->SetWindowText(sTemp);
	*/
}

BOOL CManualTestDlg::DestroyWindow() 
{
	UINT dwRet = MM.Break();
	if (dwRet == FALSE) 
	{	ErrMsg(); ERR.Reset();  }	
	TRACE("DestroyWindow(): MM.Break() %s\n\n", (dwRet == FALSE)? "Error" : "OK");

	//son
	dwRet = MM.m_pMicroM->CloseMotion();
	if (dwRet == FALSE) 
	{	ErrMsg(MB_OK,FALSE); ERR.Reset();  }	// Err를 MsgBox만 출력,  Trace는 미리 출력했음
	TRACE("DestroyWindow():  MM.m_pMicroM->CloseMotion() %s\n\n", (dwRet == FALSE)? "Error" : "OK");
	
	return CDialog::DestroyWindow();
}

void CManualTestDlg::OnButtonInit() 
{
	UINT dwRet = MM.Break();
	if (dwRet == FALSE) 
	{	ErrMsg(MB_OK, FALSE); ERR.Reset();  }	// bTraceOn=FALSE, 에러를 Trace 빼고 메시지박스만 출력
	TRACE("OnButtonInit(): MM.Break %s\n\n", (dwRet == FALSE)? "Error" : "OK");

	InitMember();
}



void CManualTestDlg::OnButtonVbRstAll() 
{
	// TODO: Add your control notification handler code here
	UINT dwRet;	
	//GetDlgItem(IDC_BUTTON_BOX_RST)->EnableWindow(FALSE);

	dwRet = MM.RstPointAll(PT_VB);		// VB Point All Reset
	TRACE("OnButtonVbRstAll(): MM.RstPointAll(PT_VB) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");

	m_listBPoint.ResetContent();
	//GetDlgItem(IDC_BUTTON_BOX_RST)->EnableWindow(TRUE);
	
}

void CManualTestDlg::OnButtonVcRstAll() 
{
	// TODO: Add your control notification handler code here
	
	UINT dwRet;	
	//GetDlgItem(IDC_BUTTON_BOX_RST)->EnableWindow(FALSE);

	dwRet = MM.RstPointAll (PT_VC);		// VC point All Reset
	TRACE("OnButtonVcRstAll(): MM.RstPointAll(PT_VC) %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");

	m_listCPoint.ResetContent();
	//GetDlgItem(IDC_BUTTON_BOX_RST)->EnableWindow(TRUE);
}
