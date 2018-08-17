
//{{AFX_INCLUDES()
#include "labelcontrol.h"
//}}AFX_INCLUDES
// ManualTestDlg.h : header file
//

#if !defined(AFX_MANUAL_TEST_DLG_H__E0BF6063_6F03_40DE_A586_5DB4D3DA7AA0__INCLUDED_)
#define AFX_MANUAL_TEST_H__E0BF6063_6F03_40DE_A586_5DB4D3DA7AA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CManualTestDlg dialog

class CManualTestDlg : public CDialog
{
	BOOL m_bNKeyPadOpen;
	BOOL m_bBTRY;		// VB Gate Type
	BOOL m_bCTRY;		// VC Gate Type
	int  m_nBPCB;		// VB PCB Number
	int  m_nCPCB;		// VC PCB Number
	int  m_nIRange;
	UINT m_nIadvalue;
	UINT m_nVadvalue;

	double m_dIamp;
	double m_dVCvolt;
	double m_dRohm;

// Construction
public:
	CManualTestDlg(CWnd* pParent = NULL);	// standard constructor

	void InitMember();
	void InitView();
	void RefreshView(CWnd *pWnd = NULL);
	void CalcPoint();
	int  SetVb_GetVb(UINT module_id, UINT in_value, double& dAdVolt);

// Dialog Data
	//{{AFX_DATA(CManualTestDlg)
	enum { IDD = IDD_MANUAL_TEST_DIALOG };
	CListBox	m_listMsg;
	CListBox	m_listBPoint;
	CListBox	m_listCPoint;
	UINT	m_nLV1;
	UINT	m_nLV2;
	UINT	m_nHV1;
	UINT	m_nHV2;
	UINT	m_nI;
	UINT	m_nBPin;
	UINT	m_nCPin;
	int		m_nBBox;
	int		m_nCBox;
	int		m_nBPCBEnable;
	int		m_nCPCBEnable;
	int		m_nBCPLD;
	int		m_nCCPLD;
	BOOL	m_nBPCB0;
	BOOL	m_nBPCB1;
	BOOL	m_nBPCB2;
	BOOL	m_nBPCB3;
	BOOL	m_nBPin0;
	BOOL	m_nBPin1;
	BOOL	m_nBPin2;
	BOOL	m_nBPin3;
	BOOL	m_nBPin4;
	BOOL	m_nBPin5;
	BOOL	m_nCPCB0;
	BOOL	m_nCPCB1;
	BOOL	m_nCPCB2;
	BOOL	m_nCPCB3;
	BOOL	m_nCPin0;
	BOOL	m_nCPin1;
	BOOL	m_nCPin2;
	BOOL	m_nCPin3;
	BOOL	m_nCPin4;
	BOOL	m_nCPin5;
	CLabelControl	m_lbIamp;
	CLabelControl	m_lbVvolt;
	CLabelControl	m_lbRohm;
	BOOL	m_nSparkOn;
	UINT	m_nSpark;
	BOOL	m_bIHighGainMode;
	BOOL	m_bDCOut;
	UINT	m_ptBTR;
	UINT	m_ptBRY;
	UINT	m_ptCTR;
	UINT	m_ptCRY;
	BOOL	m_bVBGuardOn;
	BOOL	m_bVCGuardOn;
	UINT	m_nBCCV;
	UINT	m_nCCCV;
	int		m_nBCVC;
	int		m_nCCVC;
	BOOL	m_bPulse;
	int		m_nADC_B;
	int		m_nADC_A;
	int		m_nHVOGain;
	int		m_n4WGainMode;
	int		m_nICC;
	int		m_nVB;
	int		m_nPulseFreq;
	int		m_nPulseMea;
	int		m_nPulseVolt;
	UINT	m_adcHV1;
	UINT	m_adcHV2;
	UINT	m_adcLV1;
	UINT	m_adcLV2;
	int		m_nIRng;
	UINT	m_adcSpark;
	int		m_nHLVC;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualTestDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CManualTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonLv1Set();
	afx_msg void OnButtonLv2Set();
	afx_msg void OnButtonHv1Set();
	afx_msg void OnButtonHv2Set();
	afx_msg void OnSetfocusEditLv1();
	afx_msg void OnSetfocusEditLv2();
	afx_msg void OnSetfocusEditHv1();
	afx_msg void OnSetfocusEditHv2();
	afx_msg void OnSetfocusEditI();
	afx_msg void OnButtonISet();
	afx_msg void OnRadioIfft();
	afx_msg void OnRadioIsft();
	afx_msg void OnRadioVbft();
	afx_msg void OnRadioVcfft();
	afx_msg void OnRadioVcsft();
	afx_msg void OnDblclkListBpoint();
	afx_msg void OnSetfocusEditBpin();
	afx_msg void OnDblclkListCpoint();
	afx_msg void OnSetfocusEditCpin();
	afx_msg void OnRadioLv1();
	afx_msg void OnRadioLv2();
	afx_msg void OnRadioHv1();
	afx_msg void OnRadioHv2();
	afx_msg void OnButtonBreak();
	afx_msg void OnButtonPointSet();
	afx_msg void OnButtonBoxRst();
	afx_msg void OnButtonAdRead();
	afx_msg void OnSetfocusEditQmBpoint();
	afx_msg void OnSetfocusEditQmCpoint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButtonVbOutSet();
	afx_msg void OnButtonSparkSet();
	afx_msg void OnSetfocusEditSpark();
	afx_msg void OnCheckSparkOn();
	afx_msg void OnRadioPulse();
	afx_msg void OnCheckDcOut();
	afx_msg void OnRadioPftFast();
	afx_msg void OnRadioPftSlow();
	afx_msg void OnRadioPulseVolt();
	afx_msg void OnRadioPulseVolt1();
	afx_msg void OnRadioPulseVolt2();
	afx_msg void OnRadioPulseVolt3();
	afx_msg void OnRadioPulseFreq();
	afx_msg void OnRadioPulseFreq1();
	afx_msg void OnRadioPulseFreq2();
	afx_msg void OnRadioPulseFreq3();
	afx_msg void OnRadioPulseMeasureCav0();
	afx_msg void OnRadioPulseMeasureCav1();
	afx_msg void OnRadioPulseMeasureCav2();
	afx_msg void OnRadioPulseMeasureCav3();
	afx_msg void OnRadioIRng0();
	afx_msg void OnRadioIRng1();
	afx_msg void OnRadioIRng2();
	afx_msg void OnRadioIRng3();
	afx_msg void OnRadioIRng4();
	afx_msg void OnRadioIRng6();
	afx_msg void OnRadioICc();
	afx_msg void OnRadioICv();
	afx_msg void OnCheckIHighGain();
	afx_msg void OnRadioHvoGain1();
	afx_msg void OnRadioHvoGain2();
	afx_msg void OnRadioHvoGain3();
	afx_msg void OnRadioHvoGain4();
	afx_msg void OnRadio4WGain_Sx1000();
	afx_msg void OnRadio4WGain_Fx10();
	afx_msg void OnRadio4WGain_Fx100();
	afx_msg void OnRadio4WGain_Fx1000();
	afx_msg void OnButtonBpointTrSet();
	afx_msg void OnButtonBpointTrRst();
	afx_msg void OnButtonBpointRySet();
	afx_msg void OnButtonBpointRyRst();
	afx_msg void OnSetfocusEditBpointTr();
	afx_msg void OnSetfocusEditBpointRy();
	afx_msg void OnSetfocusEditCpointTr();
	afx_msg void OnSetfocusEditCpointRy();
	afx_msg void OnButtonCpointTrSet();
	afx_msg void OnButtonCpointTrRst();
	afx_msg void OnButtonCpointRySet();
	afx_msg void OnButtonCpointRyRst();
	afx_msg void OnRadioBcpld0();
	afx_msg void OnRadioBcpld1();
	afx_msg void OnRadioBcpld2();
	afx_msg void OnRadioBcpld3();
	afx_msg void OnRadioCcpld0();
	afx_msg void OnRadioCcpld1();
	afx_msg void OnRadioCcpld2();
	afx_msg void OnRadioCcpld3();
	afx_msg void OnSetfocusEditBpointCcV();
	afx_msg void OnSetfocusEditCpointCcV();
	afx_msg void OnRadioBpointCv();
	afx_msg void OnRadioBpointCc();
	afx_msg void OnRadioCpointCv();
	afx_msg void OnRadioCpointCc();
	afx_msg void OnCheckVbGuardOn();
	afx_msg void OnCheckVcGuardOn();
	afx_msg void OnButtonBpointCcvSet();
	afx_msg void OnButtonCpointCcvSet();
	afx_msg void OnCheckPulse();
	afx_msg void OnRadioHvoft();
	afx_msg void OnRadioFwft();
	afx_msg void OnButtonInit();
	afx_msg void OnRadioHvc();
	afx_msg void OnRadioLvc();
	afx_msg void OnButtonVbRstAll();
	afx_msg void OnButtonVcRstAll();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUAL_TEST_H__E0BF6063_6F03_40DE_A586_5DB4D3DA7AA0__INCLUDED_)
