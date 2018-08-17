//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_CALIBDIALOG_H__DD058E1F_0AF2_493A_A934_5995792E2E3C__INCLUDED_)
#define AFX_CALIBDIALOG_H__DD058E1F_0AF2_493A_A934_5995792E2E3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalibDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalibDialog dialog


// - Calibration
//   : ���װ��� �����ϴ� �κ�. 
//    H/W �� AD ����  ���װ����� ��ȯ�ϴ� ���� Calibration�̴�. 
//    H/W�� ������  AD ��(����, ����) �� ��ȭ��  ���� ���װ� �󸶿� match �Ǵ��ĸ� ���� ���� ���� Calibration.
//    Path, Fail�� ����. 


class CCalibDialog : public CDialog
{

// Construction
public:
	CCalibDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalibDialog)
	enum { IDD = IDD_CALIB_DIALOG };

	CComboBox	m_comboCalMode;
	CMSFlexGrid	m_gridCalResult;
	CMSFlexGrid	m_gridCalModeCondition;
	UINT		m_nMeasCount;
	//}}AFX_DATA
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalibDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalibDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButtonSaveCondition();
	afx_msg void OnButtonViewConditionFile();
	afx_msg void OnButtonLoadRef();
	afx_msg void OnButtonStartMeas();
	afx_msg void OnButtonChgRef();
	afx_msg void OnButtonSaveRef();
	afx_msg void OnButtonClrScreen();
	afx_msg void OnButtonStopMeas();
	afx_msg void OnButtonViewResultFile();
	afx_msg void OnButtonAutoCalib();
	afx_msg void OnDblClickGridCondition();
	afx_msg void OnSelchangeComboCalibMode();
	afx_msg void OnChangeEditMeasCnt();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int		m_nHLVC;			//  0:VCFFT_LVC , 1:VCFFT_HVC
	int		m_nCalMode;	
	BOOL	m_bIsPinLoaded;		//  TRUE:  Load Reffence ��ư ����Ϸ�.  
								//  FALSE: Load Reffence ��ư �̼���
	BOOL	m_bFlagStopMeas;	//  TRUE:  Stop Measure ��ưŬ����   
								//  FALSE: StartMeas �����߰��� Flag �ν��ϰ� R ��� �ߴ� ����
	BOOL	m_bFlagStopAutoCal;	//  TRUE:  Stop Measure ��ưŬ����   
								//  FALSE: AutoCal �����߰��� Flag �ν��ϰ� R ��� �ߴ� ����
									
	BOOL 	InitMember();
	void 	InitView();
	void 	SetGrid_TestCondition(int nCalMode);
	void 	SetGrid_RJigData(int nCalMode);
	void 	SetGrid_PrevCalData(int nCalMode);
	void 	SetGrid_RefCalData(int nCalMode);
	void 	ClearGrid_CalResult();

	// OnButtonStartMeas() ���� �Լ���
	BOOL 	Meas_CheckRange_TestCondition();
	BOOL 	Meas_SetVb();
	BOOL 	Meas_ReadVb();
	BOOL 	Meas_CheckRange_VbVal(double dAdVb_ReadVal);
	BOOL 	Meas_VbOutCheck();
	BOOL 	Meas_SetLvcHvc();
	BOOL 	Meas_SetAdcBFilter();
	BOOL 	Meas_SetAdcAFilter();
	BOOL 	Meas_Set4WGain();
	BOOL 	Meas_SetIRange();
	BOOL 	Meas_SetIValue();
	BOOL 	Meas_VbRelayOn();
	BOOL 	Meas_VB_VC_PointAllReset();
	BOOL 	Meas_CalcR_4W();
	BOOL 	Meas_CalcR_2W();
	BOOL 	Meas_CalcR_Set_VbVcPin(int VbVcType, int gateType, int pinNo);
	void 	Meas_CalcR_CalData(int RJigIdx);
	BOOL 	Meas_CalcR_AdRead_VI(int nCalMode, int nRJigId, int nMeasId);
	BOOL 	Meas_CalcR_AdRead_VI_Median(int nCalMode, int nRJigId, int nMeasId);
	BOOL 	Meas_CalcR_AdRead_VI_CMAvg(int nCalMode, int nRJigId, int nMeasId);
	double 	GetMedian(int* array, size_t arraySize);
	double 	GetMedian2(int* array, size_t arraySize);

	void 	Meas_CalcR_makeR(int nCalMode, double dAdI, double dAdVc, double& rdI, double& rdVc, double& rdR);
	void 	Meas_CalcR_CheckInvalid(int nCalMode, int nRJigId);

	void 	Meas_CalcR_Display(int RJigIdx);
	void 	Meas_CalcR_FileOut();
	BOOL 	Auto_Calib_Mode(int nCalMode);

};
extern int  	CompareDouble(const void *a, const void *b);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIBDIALOG_H__DD058E1F_0AF2_493A_A934_5995792E2E3C__INCLUDED_)
