// CalibDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ACE400Cal.h"
#include "CalibDialog.h"
#include "System.h"
#include <stdlib.h> // qsort()


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalibDialog dialog


#define INIT_MEAS_COUNT		3
CCalibDialog::CCalibDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCalibDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalibDialog)
	//}}AFX_DATA_INIT
	
	
	m_nMeasCount = INIT_MEAS_COUNT;
	m_nHLVC = -1;
	m_bIsPinLoaded = FALSE;
	m_bFlagStopMeas = FALSE;
	m_bFlagStopAutoCal = FALSE;

	// 2018.02.21 â �ٲ� ������ m_nCalMode=5�� �ٲ�� ���� �����ؼ� �� �ʱ�ȭ�� InitMember()���� �����ڷ� �ű�. 
	m_nCalMode = 5;	// ���� �׽�Ʈ ���� ex) 5= "6 Open 10 V"
}


void CCalibDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalibDialog)
	DDX_Control(pDX, IDC_COMBO_CALIB_MODE, m_comboCalMode);
	DDX_Control(pDX, IDC_GRID_CAL_RESULT, m_gridCalResult);
	DDX_Text(pDX, IDC_EDIT_MEAS_CNT, m_nMeasCount);
	DDV_MinMaxUInt(pDX, m_nMeasCount, 1, MAX_MEAS_COUNT);
	DDX_Control(pDX, IDC_GRID_CONDITION, m_gridCalModeCondition);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalibDialog, CDialog)
	//{{AFX_MSG_MAP(CCalibDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_SAVE_CONDITION, OnButtonSaveCondition)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_REF, OnButtonLoadRef)
	ON_BN_CLICKED(IDC_BUTTON_START_MEAS, OnButtonStartMeas)
	ON_BN_CLICKED(IDC_BUTTON_CHG_REF, OnButtonChgRef)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_REF, OnButtonSaveRef)
	ON_BN_CLICKED(IDC_BUTTON_CLR_SCREEN, OnButtonClrScreen)
	ON_BN_CLICKED(IDC_BUTTON_STOP_MEAS, OnButtonStopMeas)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_RESULT_FILE, OnButtonViewResultFile)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_CALIB, OnButtonAutoCalib)
	ON_CBN_SELCHANGE(IDC_COMBO_CALIB_MODE, OnSelchangeComboCalibMode)
	ON_EN_CHANGE(IDC_EDIT_MEAS_CNT, OnChangeEditMeasCnt)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_CONDITION_FILE, OnButtonViewConditionFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CCalibDialog, CDialog)
    //{{AFX_EVENTSINK_MAP(CCalibDialog)
	ON_EVENT(CCalibDialog, IDC_GRID_CONDITION, -601 /* DblClick */, OnDblClickGridCondition, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCalibDialog message handlers

BOOL CCalibDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCalibDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(bShow == TRUE){
		if (InitMember() == FALSE)
			return;
		InitView();		// ��Ʈ ������ ���� ��
	}

	
}

BOOL CCalibDialog::InitMember()
{

	// UI ��Ʈ�� ���� Value�� ��� ���� �ʱ�ȭ
	m_nMeasCount = INIT_MEAS_COUNT;
	m_nHLVC = -1;
	m_bIsPinLoaded = FALSE;
	m_bFlagStopMeas = FALSE;
	m_bFlagStopAutoCal = FALSE;

	//-------------------------
	// ������ ���� �ҷ�����
	
	g_sFile.Init();
	g_sSystem.InitMember();
	g_sModel.InitMember();

	int ret;
	// "C:\ACE400\SETUP\FilePathJig.ini" ���� BDL ���� path�� �ε��Ѵ�.
	ret = Read_BdlFilePath();
	if (ret == FALSE) {	ErrMsg(); ERR.Reset(); return FALSE; }
	
	// "C:\\ACE400\\SETUP\\SysSet01.ini"���� g_sSystem.m_nSystemType ������ (QD, SD, DD, S100_1, S100_2) �ε�
	ret = g_sSystem.Load_SysInfoData();
	if (ret == FALSE) {	ErrMsg(); ERR.Reset(); return FALSE; }

	// g_sSystem.m_saRJigData ������ �ε�
	//for "C:\\ACE400\\BDL\\QC\\RJigReadType.TXT", C:\\ACE400\\BDL\\QC\\RJigReadType_S100.TXT"
	ret = g_sSystem.Load_RJigData();
	if (ret == FALSE) { ErrMsg(); ERR.Reset(); return FALSE; }

	// CalMode ComboBox�� TestCondition Text List�� �߰��Ѵ�. 
	for (int i=0; i < MAX_TEST_MODE; i++)
	{
		//Test not Use�� combo�� ǥ���ϸ�  m_nCalMode�� �״�� m_gridCalModeCondition[]�� index�� 
		//����� �� ����.  ���� �� ������ �ذ��ϰ� �Ʒ� �ڵ带 Ǯ� ����� ��. 
		//if (g_saTestCondition[i].nTest != TEST_NOT_USE)
			m_comboCalMode.InsertString(-1, g_saTestCondition[i].szCalModeName);
	}

	// 2018.02.21 â �ٲ� ������ m_nCalMode=5�� �ٲ�� ���� �����ؼ� �� �ʱ�ȭ�� �����ڷ� �ű�. 
	//m_nCalMode = 5;	// ���� �׽�Ʈ ���� ex) 5= "6 Open 10 V"
	m_comboCalMode.SetCurSel(m_nCalMode);	
	

	// "C:\ACE400\SETUP\\SysSet21_ACE400.ini"���� g_saTestCondition[MAX_TEST_MODE] ������ �ε�
	ret = g_sSystem.Load_TestCondtionData();
	if (ret == FALSE) { ErrMsg(); ERR.Reset(); return FALSE; }

	// "C:\ACE400\SETUP\\SysSet21_4W.ini"���� g_sSystem.m_na4W_Range[MAX_TEST_MODE] ������ �ε�
	ret = g_sSystem.Load_4W_RangeData();
	if (ret == FALSE) { ErrMsg(); ERR.Reset(); return FALSE; }

	ret = g_sModel.Load_PinConFile();
	if (ret == FALSE) { ErrMsg(); ERR.Reset(); return FALSE; }

	ret = g_sModel.Load_PinSetupData();
	if (ret == FALSE) { ErrMsg(); ERR.Reset(); return FALSE; }


	// 2018.02.19  Bad �׸� ������ .csv ���Ϸ� ������ ���� ������ ���� ����  
	FILE 	*fpBad;
	char  	fNameBad[200]; 
	CString	strTemp;
	
	// ������ ������ ������ ��� clear�� ���� �ϴ� close�Ѵ�.  
	// ���� ����� calMode���� Meas_CalcR_FileOut() ȣ��ÿ� ������.
	strTemp.Format("%s\\001LOGRCalBad.csv",g_sFile.MeasureDir );	
	::ZeroMemory(&fNameBad, sizeof(fNameBad));
	strcat( fNameBad ,  strTemp);		
	fpBad = fopen(fNameBad,"wt");	// "wt" : ������ �����ϴ� ���� ������ �ִٸ�  destroy�ϰ� open
	if (fpBad == NULL)
	{ 	ERR.Set(FLAG_FILE_CANNOT_OPEN, fNameBad); return FALSE; }
	fclose(fpBad);

	UpdateData(FALSE);

	return TRUE;
}

void CCalibDialog::InitView()
{
	UpdateData(TRUE);

	// �׸��� ����
	int i;
	m_gridCalModeCondition.SetCols(NUM_CAL_CONDITION_COL);	// 16 �÷�
	m_gridCalModeCondition.SetRows(2);
	int iColWidth1[] = {1400, 700, 750, 750, 750, 750, 1100, 750, 750, 750, 750, 750, 750, 750, 750, 750  };
	for (i=0; i<NUM_CAL_CONDITION_COL; i++) {
		m_gridCalModeCondition.SetColWidth(i, iColWidth1[i]);

		//if(i != 0) 
		{
			m_gridCalModeCondition.SetFixedAlignment(i, 3);// 3 = H Center
			m_gridCalModeCondition.SetColAlignment(i, 7);// 4 = V Center
		}
	}

	m_gridCalResult.SetCols(NUM_CAL_RESULT_COL);	// 22 +60(per Meas) �÷�
	m_gridCalResult.SetRows(MAX_2W_RJIG+1);			// ������� 97�� �ʿ�
					//   0    1    2     3       4       5       6       7        8       9       10     
					//  No   Use  Valid VbTrPin VbRyPin VcTrPin VcRyPin OrgR  RefR(Avg) RefR:VC  Ref R:I
	int iColWidth2[] = {400, 600, 800,  800,    800,    800,     800,   1200,   1100,    900,    900, 

					//   11     12     13        14       15         16         17          18          19          20     21   
					//  ��ĭ  R(Avg) MaxRDifRt MinRDifRt R(Avg):VB  R(Avg):VC  R(Avg):I  R(Avg):AdVC  R(Avg):AdI   MaxR   MinR  
						200,   1100,  1200,      1200,    900,       900,       900,        1100,       1100,      1100,  1100, 

					//  22     23    24    25      26    27
					//  ��ĭ  01:R  01:VC  01:I  01:ADVC  01:ADI
     					200,  1100,  900,  900,    900,  900,
     					200,  1100,  900,  900,    900,  900,
     					200,  1100,  900,  900,    900,  900,
     					200,  1100,  900,  900,    900,  900,
     					200,  1100,  900,  900,    900,  900,
     					200,  1100,  900,  900,    900,  900,
     					200,  1100,  900,  900,    900,  900,
     					200,  1100,  900,  900,    900,  900,
     					200,  1100,  900,  900,    900,  900,
     					200,  1100,  900,  900,    900,  900 };
    				//  76     77    78    79      80    81
    				
	for (i = 0; i < NUM_CAL_RESULT_COL; i++) {
		m_gridCalResult.SetFixedAlignment(i, 3);// 3 = H Center
		//if (i < 20)
			m_gridCalResult.SetColWidth(i, iColWidth2[i]);
		if(i != 0) 
			m_gridCalResult.SetColAlignment(i, 7);// 4 = V Center
	}
	

	// Cal Mode Condition: Set Grid Header
	m_gridCalModeCondition.SetTextMatrix(0, 0, "Mode");
	m_gridCalModeCondition.SetTextMatrix(0, 1, "CC");
	m_gridCalModeCondition.SetTextMatrix(0, 2, "VBSrc");
	m_gridCalModeCondition.SetTextMatrix(0, 3, "VSet");
	m_gridCalModeCondition.SetTextMatrix(0, 4, "IRng");
	m_gridCalModeCondition.SetTextMatrix(0, 5, "ISet*10");
	m_gridCalModeCondition.SetTextMatrix(0, 6, "Delay(usec)");
	m_gridCalModeCondition.SetTextMatrix(0, 7, "AdCount");
	m_gridCalModeCondition.SetTextMatrix(0, 8, "Filter");
	m_gridCalModeCondition.SetTextMatrix(0, 9, "IFilter");
	m_gridCalModeCondition.SetTextMatrix(0, 10, "HVOfst");
	m_gridCalModeCondition.SetTextMatrix(0, 11, "HVGain");
	m_gridCalModeCondition.SetTextMatrix(0, 12, "IGain");
	m_gridCalModeCondition.SetTextMatrix(0, 13, "4WGain");
	m_gridCalModeCondition.SetTextMatrix(0, 14, "sCount");
	m_gridCalModeCondition.SetTextMatrix(0, 15, "nTest");
	UpdateData(FALSE);

	// �ش� calMode�� TestCondition ������  m_gridCalModeCondition�� display�Ѵ�.
	SetGrid_TestCondition(m_nCalMode);	


	// Cal Result: Set Grid Header
	m_gridCalResult.SetTextMatrix(0, 0, "No");
	m_gridCalResult.SetTextMatrix(0, 1, "Use=1");
	m_gridCalResult.SetTextMatrix(0, 2, "Valid=1");

	m_gridCalResult.SetTextMatrix(0, 3, "VbTrPin");
	m_gridCalResult.SetTextMatrix(0, 4, "VbRyPin");
	m_gridCalResult.SetTextMatrix(0, 5, "VcTrPin");
	m_gridCalResult.SetTextMatrix(0, 6, "VcRyPin");
	m_gridCalResult.SetTextMatrix(0, 7, "Org R");

	m_gridCalResult.SetTextMatrix(0, 8, "Ref R(Avg)");
	m_gridCalResult.SetTextMatrix(0, 9, "Ref R:VC");
	m_gridCalResult.SetTextMatrix(0, 10, "Ref R:I");		// 11�� �� �÷�

	m_gridCalResult.SetTextMatrix(0, 12, "R(Avg)");
	m_gridCalResult.SetTextMatrix(0, 13, "MaxRDifRt(%)");
	m_gridCalResult.SetTextMatrix(0, 14, "MinRDifRt(%)");
	m_gridCalResult.SetTextMatrix(0, 15, "R(Avg):VB");
	m_gridCalResult.SetTextMatrix(0, 16, "R(Avg):VC");
	m_gridCalResult.SetTextMatrix(0, 17, "R(Avg):I");
	m_gridCalResult.SetTextMatrix(0, 18, "R(Avg):AD VC");
	m_gridCalResult.SetTextMatrix(0, 19, "R(Avg):AD I");
	m_gridCalResult.SetTextMatrix(0, 20, "MaxR");
	m_gridCalResult.SetTextMatrix(0, 21, "MinR");

	CString strTemp;
	for(i=0; i < MAX_MEAS_COUNT; i++)
	{
		// Col22 +(i*6)�� �� �÷�
		// 22 + (i*6) +j
		// i=0	 22, 23, 24, 25, 26, 27
		// i=1	 28, 29, 30, 31, 32, 33
		// i=2	 34, 35, 36, 37, 38, 39
		// i=9   76, 77, 78, 79, 80, 81
		strTemp.Format("%02d: R", i+1);
		m_gridCalResult.SetTextMatrix(0, 22+(i*6 +1), strTemp);

		strTemp.Format("%02d: VC", i+1);
		m_gridCalResult.SetTextMatrix(0, 22+(i*6 +2), strTemp);

		strTemp.Format("%02d: I", i+1);
		m_gridCalResult.SetTextMatrix(0, 22+(i*6 +3), strTemp);

		strTemp.Format("%02d: ADVC", i+1);
		m_gridCalResult.SetTextMatrix(0, 22+(i*6 +4), strTemp);

		strTemp.Format("%02d: ADI", i+1);
		m_gridCalResult.SetTextMatrix(0, 22+(i*6 +5), strTemp);
	}


	// Cal Result Grid Screen�� �ʱ�ȭ�Ѵ�.
	//ClearGrid_CalResult();

	UpdateData(FALSE);
}

//---------------------------------------------------
// 	m_comboCalMode & m_gridCalModeCondition ���� �Լ�
//---------------------------------------------------


void CCalibDialog::OnSelchangeComboCalibMode() 
{
	// TODO: Add your control notification handler code here
	m_nCalMode = m_comboCalMode.GetCurSel();

	// �ش� calMode�� TestCondition ������  m_gridCalModeCondition�� display�Ѵ�.
	SetGrid_TestCondition(m_nCalMode);	

	// mode�� change �Ǿ����� ���� �ش� pin ���� �ε� �������Ƿ� FALSE
	m_bIsPinLoaded = FALSE;
}


void CCalibDialog::SetGrid_TestCondition(int nCalMode)	
{
	UpdateData(TRUE);

	CString strTemp("");

	if (nCalMode < 0  || nCalMode >= MAX_TEST_MODE)
	{
		strTemp.Format("SetGrid_TestCondition: nCalMode=%d (0 <= nCalMode < %d)", nCalMode, MAX_TEST_MODE);
		ERR.Set(RANGE_OVER, strTemp);
		ErrMsg(); ERR.Reset(); 
		return;
	}

	m_gridCalModeCondition.SetTextMatrix(1, 0,  g_saTestCondition[nCalMode].szCalModeName);

	strTemp.Format("%d", g_saTestCondition[nCalMode].nCC);
	m_gridCalModeCondition.SetTextMatrix(1, 1,  strTemp);

	strTemp.Format("%d", g_saTestCondition[nCalMode].nVBSrc);
	m_gridCalModeCondition.SetTextMatrix(1, 2,  strTemp); 

	strTemp.Format("%d", g_saTestCondition[nCalMode].nVSet);
	m_gridCalModeCondition.SetTextMatrix(1, 3,  strTemp);

	strTemp.Format("%d", g_saTestCondition[nCalMode].nIRng);
	m_gridCalModeCondition.SetTextMatrix(1, 4,  strTemp);

	strTemp.Format("%d", g_saTestCondition[nCalMode].nISet);
	m_gridCalModeCondition.SetTextMatrix(1, 5,  strTemp);

	strTemp.Format("%d", g_saTestCondition[nCalMode].nDelay);
	m_gridCalModeCondition.SetTextMatrix(1, 6,  strTemp);

	strTemp.Format("%d", g_saTestCondition[nCalMode].nAdCnt);
	m_gridCalModeCondition.SetTextMatrix(1, 7,  strTemp);

	strTemp.Format("%d", g_saTestCondition[nCalMode].nFilter);
	m_gridCalModeCondition.SetTextMatrix(1, 8,  strTemp);

	strTemp.Format("%d", g_saTestCondition[nCalMode].nIFilter);
	m_gridCalModeCondition.SetTextMatrix(1, 9,  strTemp);

	strTemp.Format("%d", g_saTestCondition[nCalMode].nHvOffset);
	m_gridCalModeCondition.SetTextMatrix(1, 10, strTemp);

	strTemp.Format("%d", g_saTestCondition[nCalMode].nHVGain);
	m_gridCalModeCondition.SetTextMatrix(1, 11, strTemp);

	strTemp.Format("%d", g_saTestCondition[nCalMode].nIGain);
	m_gridCalModeCondition.SetTextMatrix(1, 12, strTemp);

	strTemp.Format("%d", g_saTestCondition[nCalMode].n4WGain);
	m_gridCalModeCondition.SetTextMatrix(1, 13, strTemp);

	strTemp.Format("%d", g_saTestCondition[nCalMode].sCount);
	m_gridCalModeCondition.SetTextMatrix(1, 14, strTemp);

	strTemp.Format("%d", g_saTestCondition[nCalMode].nTest);
	m_gridCalModeCondition.SetTextMatrix(1, 15, strTemp);

	UpdateData(FALSE);
}



// CalMode Condtion �׸��� ��Ʈ���� ����Ŭ������ �� ó���ϴ� ��ƾ
// CalModeCondition �׸����� �׸��� �����Ѵ�. 
void CCalibDialog::OnDblClickGridCondition() 
{
	// TODO: Add your control notification handler code here

/*
	
	CEditGridDlg dlg; 
	// ������ x, y ��ǥ�� ��´�. 
	int x, y; 
	x = m_gridCalModeCondition.GetColSel(); 
	y = m_gridCalModeCondition.GetRowSel(); 
	dlg.m_editStr = m_gridCalModeCondition.GetTextMatrix(y, x); 
	if ( dlg.DoModal() == IDOK) 
	{ 
		m_pGrid.SetText(dlg.m_editStr); 
	} 
*/

	//-----------------------------------------
	// �ش� �÷��� data�� �����´�. 
	int nRow, nCol; 
	CString strName, strTemp; 
	nCol = m_gridCalModeCondition.GetColSel(); 
	nRow = m_gridCalModeCondition.GetRowSel(); 
	if (nCol == 0 )	// ù��° �÷��� �����Ұ�.  CalMode Name�� ������ �� ����.
		return;		// ù��° �÷��� Ŭ���� �����ϹǷ� ������ ���� �ʴ� ������ ����. ����ó�� ���� �ʰ� ����.

	if (nCol <= 0 || nCol >= NUM_CAL_CONDITION_COL) // Col Range Error
	{
		strTemp.Format("OnDblClickGridCondition: nCol=%d (0 < nCol < %d)", nCol, NUM_CAL_CONDITION_COL);
		ERR.Set(RANGE_OVER, strTemp);
		ErrMsg(); ERR.Reset(); 
		return;
	}
	


	//-----------------------------------------
	// ���̾�α׸� ����� �Է��� �޴´�.
	//
	
	// �ش� �÷� �׸���  Range Value Ȯ��  
	int nMinVal = 0;	
	int nMaxVal = 10000;	

	nMinVal = g_saTestConditionRange[nCol].nMinVal;
	nMaxVal = g_saTestConditionRange[nCol].nMaxVal;

	// �ش� �÷��� ���� ��°��� �����ͼ� �Է� ���̾�α׿� int�� �����Ѵ�.
	strName = m_gridCalModeCondition.GetTextMatrix(nRow, nCol);
	int nVal = atoi(strName);
	if(NKEYPAD.SetValue((int)nVal, nMinVal, nMaxVal) == FALSE) 
		return;

	NKEYPAD.DoModal();
	nVal = (int)NKEYPAD.GetValue();


	//-----------------------------------------------
	// ���̾�α׿��� �Է¹��� ���� �ش� �÷��� ����Ѵ�.
	strName.Format("%d", nVal);
	m_gridCalModeCondition.SetTextMatrix(nRow, nCol, strName);


	//-----------------------------------------
	// �ش� TestCondition ���� ������ �ݿ��Ѵ�.
	// UI <-> ���� TestCondition data ������ ������ ��ġ ��Ŵ.
	switch(nCol)
	{
		case 1:	 g_saTestCondition[m_nCalMode].nCC = nVal;
				 break;
		case 2:	 g_saTestCondition[m_nCalMode].nVBSrc = nVal;
				 break;
		case 3:	 g_saTestCondition[m_nCalMode].nVSet = nVal;
				 break;
		case 4:	 g_saTestCondition[m_nCalMode].nIRng = nVal;
				 break;
		case 5:	 g_saTestCondition[m_nCalMode].nISet = nVal;
				 break;
		case 6:	 g_saTestCondition[m_nCalMode].nDelay = nVal;
				 break;
		case 7:	 g_saTestCondition[m_nCalMode].nAdCnt = nVal;
				 break;
		case 8:	 g_saTestCondition[m_nCalMode].nFilter = nVal;
				 break;
		case 9:	 g_saTestCondition[m_nCalMode].nIFilter = nVal;
				 break;
		case 10: g_saTestCondition[m_nCalMode].nHvOffset = nVal;
				 break;
		case 11: g_saTestCondition[m_nCalMode].nHVGain = nVal;
				 break;
		case 12: g_saTestCondition[m_nCalMode].nIGain = nVal;
				 break;
		case 13: g_saTestCondition[m_nCalMode].n4WGain = nVal;
				 break;
		case 14: g_saTestCondition[m_nCalMode].sCount = nVal;
				 break;
		case 15: g_saTestCondition[m_nCalMode].nTest = nVal;
				 break;
		default:
			break;
	}

}


//-------------------------------------
// ButtonSaveCondition ���� �Լ�
//------------------------------------
// ���� TestCondition <-> �ܺ� CalConditon File(C:\ACE400\SETUP\\SysSet21_ACE400.ini) ���� Sync��
// 'Save Condtion' ��ư Ŭ���ÿ� �����. 
void CCalibDialog::OnButtonSaveCondition() 
{
	// TODO: Add your control notification handler code here
	BOOL bRet = g_sSystem.Save_TestCondtionData();
	if (bRet == FALSE) {	ErrMsg(); ERR.Reset(); return; }
	
}

//-------------------------------------
// ButtonViewConditionFile ���� �Լ�
//------------------------------------
void CCalibDialog::OnButtonViewConditionFile() 
{
	// TODO: Add your control notification handler code here
	
	//CString strTemp;
    //strTemp.Format("%s" , g_sFile.ACE400_TestConditionPath);		// "C:\ACE400\SETUP\\SysSet21_ACE400.ini"

	::ShellExecute(NULL,"open","notepad.EXE",g_sFile.ACE400_TestConditionPath,"NULL",SW_SHOWNORMAL);	

}

//--------------------------------------------
// RJigData �ʱ�ȭ & ButtonLoadRef ���� �Լ�
//--------------------------------------------


void CCalibDialog::OnButtonLoadRef() 
{
	// TODO: Add your control notification handler code here
	
	// Cal ���� data�� �ʱ�ȭ�Ѵ�.    RJigData�� �ʱ�ȭ�ϸ� �� ��.
	g_sSystem.InitCalData();
	
	// Cal Result Grid Screen�� �ʱ�ȭ�Ѵ�.
	ClearGrid_CalResult();

	// �ش� calMode�� RjigData, Ref R ���� m_gridCalResult�� display�Ѵ�.
	SetGrid_RJigData(m_nCalMode);	

	// ���� SetGrid_RJigData(m_nCalMode)�� SetGrid_PrevCalData(m_nCalMode)���� ���� ȣ���� ��� 
	// m_saCalData[nCalMode][i].dOrgR���� Ȯ���� �� �ִ�.

	// �ش� calMode�� ���� Ref cal data �� Grid�� �ε��Ѵ�.
	SetGrid_PrevCalData(m_nCalMode);	

	
	// mode�� change �Ǿ��� �ش� pin ������ �ε� �Ǿ����Ƿ� TRUE�� ����
	m_bIsPinLoaded = TRUE;
}

// �ش� calMode�� RjigData, Ref R ���� m_gridCalResult�� display�Ѵ�.
void CCalibDialog::SetGrid_RJigData(int nCalMode)	
{
	UpdateData(TRUE);

	sRJIG_DATA	*p_saRJigData;
	CString strTemp("");
	int RJigIdx;

	if (nCalMode < 0  || nCalMode >= MAX_TEST_MODE)
	{
		strTemp.Format("SetGrid_RJigData: nCalMode=%d (0 <= nCalMode < %d)", nCalMode, MAX_TEST_MODE);
		ERR.Set(RANGE_OVER, strTemp);
		ErrMsg(); ERR.Reset(); 
		return;
	}

	// RJigData s100���� ���(1), �ƴѰ�� (0)
	if(	g_sSystem.m_nSystemType == S100_1 || g_sSystem.m_nSystemType == S100_2) 	// 3 or 4
		g_sSystem.m_nRJigFileNum = 1;
	else
		g_sSystem.m_nRJigFileNum = 0;

	int		nRJigStart, nRJigEnd;
	g_sSystem.Get_RJigPinRange(nCalMode, nRJigStart, nRJigEnd);		// nRJigStart, nRJigEnd Reference Value

	switch(g_saTestCondition[nCalMode].nTest)
	{
		case TEST_OPEN: case TEST_SHORT:
			p_saRJigData = (sRJIG_DATA*)g_sSystem.m_saRJigData_2W[g_sSystem.m_nRJigFileNum];

			//for (RJigIdx = 0; RJigIdx < MAX_2W_RJIG; RJigIdx++)
			for (RJigIdx = nRJigStart; RJigIdx <= nRJigEnd; RJigIdx++)
			{
				strTemp.Format("%d", p_saRJigData[RJigIdx].nNo);			// Col0
				m_gridCalResult.SetTextMatrix(RJigIdx+1, 0,  strTemp);

				strTemp.Format("%d", p_saRJigData[RJigIdx].nVbTrPin);		// Col3
				m_gridCalResult.SetTextMatrix(RJigIdx+1, 3,  strTemp); 

				strTemp.Format("%d", p_saRJigData[RJigIdx].nVcTrPin);		// Col5
				m_gridCalResult.SetTextMatrix(RJigIdx+1, 5,  strTemp);

				// dOrgR�� disply�� ���⼭ ���ϰ�  SetGrid_PrevCalData()�� �� �� �ֱ�� �Ѵ�.
					
				// p_saRJigData[RJigIdx].dOrgR�� CalData display�� (SetGrid_PrevCalData)�� ǥ���� �ֱ� ���� 
				// ���� ������ CalData�� .OrgR ���� �ݿ��� �д�.  ���� CalData file Save ���� ���.
				g_sSystem.m_saCalData[nCalMode][RJigIdx].dOrgR = p_saRJigData[RJigIdx].dOrgR;
			}
			break;

		case TEST_4W:
			p_saRJigData = (sRJIG_DATA*)g_sSystem.m_saRJigData_4W[g_sSystem.m_nRJigFileNum];

			//for (RJigIdx = 0; RJigIdx < MAX_4W_RJIG; RJigIdx++)
			for (RJigIdx = nRJigStart; RJigIdx <= nRJigEnd; RJigIdx++)
			{
				strTemp.Format("%d", p_saRJigData[RJigIdx].nNo);
				m_gridCalResult.SetTextMatrix(RJigIdx+1, 0,  strTemp);		// Col0

				strTemp.Format("%d", p_saRJigData[RJigIdx].nVbTrPin);
				m_gridCalResult.SetTextMatrix(RJigIdx+1, 3,  strTemp); 		// Col3

				strTemp.Format("%d", p_saRJigData[RJigIdx].nVbRyPin);		
				m_gridCalResult.SetTextMatrix(RJigIdx+1, 4,  strTemp); 		// Col4

				strTemp.Format("%d", p_saRJigData[RJigIdx].nVcTrPin);
				m_gridCalResult.SetTextMatrix(RJigIdx+1, 5,  strTemp);		// Col5

				strTemp.Format("%d", p_saRJigData[RJigIdx].nVcRyPin);
				m_gridCalResult.SetTextMatrix(RJigIdx+1, 6,  strTemp);		// Col6

				// dOrgR�� disply�� ���⼭ ���ϰ�  SetGrid_PrevCalData()�� �� �� �ֱ�� �Ѵ�.

				// p_saRJigData[RJigIdx].dOrgR�� CalData display�� (SetGrid_PrevCalData )�� ǥ���� �ֱ� ���� 
				// ���� ������ CalData�� .OrgR ���� �ݿ��� �д�.  ���� CalData file Save ���� ���.
				g_sSystem.m_saCalData[nCalMode][RJigIdx].dOrgR = p_saRJigData[RJigIdx].dOrgR;
			}
			break;


		case TEST_NOT_USE:			// TEST_NOT_USE�� ���� ������� ��� Grid�� �ε� ����
			break;

		default:
			strTemp.Format("SetGrid_RJigData() Fail!: nCalMode=%d nTest=%d, Invalid nTest Value.", 
							nCalMode, g_saTestCondition[nCalMode].nTest);
			ERR.Set(RANGE_OVER, strTemp);
			ErrMsg(); ERR.Reset(); 
			break;

	}


	UpdateData(FALSE);
}

// �ش� calMode�� ���� Ref cal data �� Grid�� �ε��Ѵ�.
void CCalibDialog::SetGrid_PrevCalData(int nCalMode)	
{
	
	// ("C:\\ACE400\\SETUP\\CAL\\CALR%02dB.INI",mode  ) ����  m_saCalData �������� ���� Ref �� �ε�
	// nCalMode�� range check�� ���⿡�� �����ϹǷ� SetGrid_PrevCalData()�� range check�� ���� ����
	if (g_sSystem.Load_PrevCalData(nCalMode) == FALSE)
	{ 	ErrMsg(); ERR.Reset(); return; }


	// �ε��� previous RefCalData�� CalResult Grid�� ����Ѵ�.
	SetGrid_RefCalData(nCalMode);

}


void CCalibDialog::SetGrid_RefCalData(int nCalMode)	
{
	UpdateData(TRUE);

	CString strTemp("");
	int i;

	int		nRJigStart, nRJigEnd;
	g_sSystem.Get_RJigPinRange(nCalMode, nRJigStart, nRJigEnd);		// nRJigStart, nRJigEnd Reference Value

	switch (g_saTestCondition[nCalMode].nTest)
	{
		case TEST_OPEN: case TEST_SHORT: case TEST_4W:		
			//for (i = 0; i < MAX_CAL_DATA; i++)
			for (i = nRJigStart; i <= nRJigEnd; i++)
			{
				strTemp.Format("%d", g_sSystem.m_saCalData[nCalMode][i].nUse);			// Col1
				m_gridCalResult.SetTextMatrix(i+1, 1,  strTemp);

				strTemp.Format("%.1f", g_sSystem.m_saCalData[nCalMode][i].dOrgR);		// Col7
				m_gridCalResult.SetTextMatrix(i+1, 7,  strTemp);

				strTemp.Format("%.2f", g_sSystem.m_saCalData[nCalMode][i].dRefR_Avg);	// Col8
				m_gridCalResult.SetTextMatrix(i+1, 8,  strTemp); 

				strTemp.Format("%.2f", g_sSystem.m_saCalData[nCalMode][i].dRefVcVal);	// Col9
				m_gridCalResult.SetTextMatrix(i+1, 9,  strTemp); 

				strTemp.Format("%.2f", g_sSystem.m_saCalData[nCalMode][i].dRefIVal);	// Col10
				m_gridCalResult.SetTextMatrix(i+1, 10,  strTemp);

			}
			break;

		case TEST_NOT_USE :			// TEST_NOT_USE�� ���� ���� ��� ��� Grid�� �ε� ����
			return;

		default:	
			strTemp.Format("SetGrid_RefCalData() Fail!: nCalMode=%d nTest=%d, Invalid nTest Value.", 
							nCalMode, g_saTestCondition[nCalMode].nTest);
			ERR.Set(RANGE_OVER, strTemp);
			ErrMsg(); ERR.Reset(); 
			return;
	}
	UpdateData(FALSE);
}


//---------------------------------
// ButtonStartMeas ���� �Լ�
//---------------------------------
void CCalibDialog::OnButtonStartMeas() 
{
	// TODO: Add your control notification handler code here
	
	CString	strTemp, strTemp2;
	UINT dwRet;
	

	if (m_bIsPinLoaded == FALSE)	// RJigPinData�� �ε��� �Ǿ����� üũ
	{
		strTemp.Format("Cannot Start Measure: CalMode=%d+1. Push 'Load Refference' Button please.", m_nCalMode);
		ERR.Set(USER_ERR, strTemp);
		ErrMsg(); ERR.Reset(); 
		return;
	}
	
    //---------------
	// RANGE CHECK
	
	//���� calMode�� TestCondtion���� nTest Type�� üũ�Ѵ�.
	switch (g_saTestCondition[m_nCalMode].nTest)
	{
		case TEST_OPEN: case TEST_SHORT: case TEST_4W:		
			break;

		case TEST_NOT_USE :			// TEST_NOT_USE �Ǵ� �ٸ� ���� ��쿡�� ������ �������� �ʴ´�.
		default:	
			strTemp.Format("Cannot Start Measure: CalMode=%d+1 nTest=%d(%s).", 
							m_nCalMode, g_saTestCondition[m_nCalMode].nTest, 
							(g_saTestCondition[m_nCalMode].nTest == TEST_NOT_USE) ? "TEST_NOT_USE" : "Invalid Value");
			ERR.Set(RANGE_OVER, strTemp);
			ErrMsg(); ERR.Reset(); 
			return;
	}
	
	// TestCondition�� range check��  InitMember()���� g_sSystem.Load_TestCondtionData() �Ҷ� 
	// ��� ���̽��� ���� �̸� �����Ͽ����� ���⿡���� ������������ �ѹ� �� üũ
	// �׿� Test Condtion  ������ ���� üũ�� ����.
	if( Meas_CheckRange_TestCondition() == FALSE)
		return;



	__int64	startTime, endTime ;
	m_bFlagStopMeas = FALSE; 
	startTime = GetMicroSecond();		// Calibration Time ������.

    //---------------
	// INITAL SET

	// Meas �����߿� ���Ǵ� ������ �ʱ�ȭ
	m_nHLVC = -1;

    // CModeSub71::AOnPoweroff2() �� �ش���.
    // MM.Break(VB_ONLY): bit52(HV_RELAY_IN_IO_0) ~ bit55(LV_RELAY_IN_IO_1) VB Apply ���� ��Ʈ�� ��� �ʱ�ȭ
	// MM.Break(VB_ALL) : bit00 ~ bit127 OUTBIT ���� ���θ� OFF �� �Ϻ� ������ ON
    // MM.Break(ALL_OUT) ����ÿ� ���������� ����  MM.Break(VB_ONLY)�� ������.
	dwRet = MM.Break(ALL_OUT);					// MM.Break()���� �̸� Trace�� �����. return�� ���� ������ ����..
	strTemp.Format("Cal::..StartMeas(m_nCalMode=%d): MM.Break(ALL_OUT) %s\n", m_nCalMode, (dwRet == FALSE)? "Error" : "OK");
	if (dwRet == FALSE) {	
		strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
		ERR.Set(ERR.Get(), strTemp2); 
		ErrMsg(MB_OK, FALSE); ERR.Reset();  // bTraceOn=FALSE, ������ Trace ���� �޽����ڽ��� ���
	}		
	MyTrace(PRT_LEVEL1, "%s\n", strTemp);
      
    Delay(3, msec);		// 3ms


	//--------------------
 	// Set VB
	if (Meas_SetVb() == FALSE) 
		return;

    Delay(100, msec);	// 100ms

	
	//--------------------
 	// Read VB
	if (Meas_ReadVb() == FALSE)		// ���⿡�� ���� VB���� Ȯ��  (Vb OutCheck �ƴ� ����)
		return;
	
	//--------------------------------------
	// ���-> ���� ��ȯ��  VB Out Check
	
	// ��� -> ���нÿ� ���� �׽�Ʈ�� �������� Vb ���� ���������� ������ ���� �� �����Ƿ�
	// VB_OUT_CHECK�� �ɰ� ���� ���� �������Ϸ� ������ �� ���� ��ٷ��� �Ѵ�.
	
	// VB OutCheck�� �ɾ VbRead�� �غ��� Set������ ������ ������������ VbOutCheck�� �ݺ��Ѵ�.
	// �̻������� �ڵ忡���� Open300V -> 4W 10V�� �Ѿ�� üũ�ϱ� ���� �ϱ� 4W�� ������ �����ϰ� �ڵ��� ��.
	//if (g_saTestCondition[m_nCalMode].nTest == TEST_4W)
	
	// ���-> ���� ��ȯ�ÿ��� ����. ���� -> ��� ��ȯ�ÿ��� ������ ����.
	if (g_saTestCondition[m_nCalMode].nVBSrc == LV1 || g_saTestCondition[m_nCalMode].nVBSrc == LV2)
	{
		// �׽�Ʈ�ʿ�1: Open-> 4W�� �Ѿ�� �������� �ϱ� �ڵ尡 �ݵ�� ������ �Ǿ�� ��.
		// ���� H/W �׽�Ʈ�ÿ� �ϱ� �ڵ� ����� ����Ǵ��� ���� �׽�Ʈ �ʿ���.
		
		if (Meas_VbOutCheck() == FALSE)		
			return;

	}


	//�׽�Ʈ�ʿ�2: Vb Out Check�� 'VBSet  ����  VBRead' ������ �ϴ°� ������ ���� �����ؼ� HW ���� �׽�Ʈ �ʿ�.

    //-------------------------
    // LVC/HVC ���� 
    if (Meas_SetLvcHvc() == FALSE)
    	return;


	
	//-------------------------------------------
	// ADC_B Filter ����,  
	// FT_B_HV_OPEN(3)�� ��쿡�� HV Open Gain�� ����
    if (Meas_SetAdcBFilter() == FALSE)
    	return;

	//----------------------
	// ADC_A Filter ����
	// FT_A_I_FAST �Ǵ� FT_A_I_SLOW �� ��� I High Gain�� ����
    if (Meas_SetAdcAFilter() == FALSE)
    	return;


	//----------------------
    // 4W Gain ����
	if (Meas_Set4WGain() == FALSE)
		return;
    
    
	//----------------------
	// 1. Set I Range
	if (Meas_SetIRange() == FALSE)
		return;

	//----------------------
	// 2. Set I VALUE
	if (Meas_SetIValue() == FALSE)
		return;

	//------------------------------------------------
	// 3. VB Apply for connected LV1, LV2,  HV1, HV2
	if (Meas_VbRelayOn() == FALSE)
		return;

	if (g_saTestCondition[m_nCalMode].nTest != TEST_4W)
	    Delay(200, msec);		// 200ms

	// Setup Time ���
	endTime = GetMicroSecond(); 	
	g_sSystem.m_dCalTime[0] = (endTime - startTime) / 1000.; 	// usec-> msec�� ��ȯ
	startTime = endTime;			// R�� ����Time ���� 


	//--------------------------
	// 4W�� �ƴ� ��� R �� ����
	if (g_saTestCondition[m_nCalMode].nTest != TEST_4W)
		if (Meas_CalcR_2W() == FALSE)
			return;


	//--------------------------
	// 4W�� ��� R �� ����
	if (g_saTestCondition[m_nCalMode].nTest == TEST_4W)
		if (Meas_CalcR_4W() == FALSE)
			return;


	// R�� ���� Time ���
	endTime = GetMicroSecond(); 	
	g_sSystem.m_dCalTime[1] = (endTime - startTime) / 1000.;	// usec-> msec�� ��ȯ 

	//---------------------------
	// CalResult ���� ��� 
	Meas_CalcR_FileOut();



	//-----------------------------------------------------------
	// Calibration Test ������ �ܰ迡�� ��ü ��Ʈ�� �ٽ� �ʱ�ȭ 
	dwRet = MM.Break(ALL_OUT);					// MM.Break()���� �̸� Trace�� �����. return�� ���� ������ ����..
	strTemp.Format("Cal::..StartMeas(): Test End. MM.Break(ALL_OUT) %s\n", (dwRet == FALSE)? "Error" : "OK");
	if (dwRet == FALSE) {	
		strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
		ERR.Set(ERR.Get(), strTemp2); 
		ErrMsg(MB_OK, FALSE); ERR.Reset();  // bTraceOn=FALSE, ������ Trace ���� �޽����ڽ��� ���
	}		
	MyTrace(PRT_LEVEL1, "%s\n", strTemp);
}


BOOL CCalibDialog::Meas_CheckRange_TestCondition() 
{
	// ���� calMode, TestCondtion�� Range�� üũ�Ѵ�.
	
	if (g_sSystem.Check_TestCondtionRange(1, g_saTestCondition[m_nCalMode].nCC) == FALSE)
	{	ErrMsg(); ERR.Reset(); return FALSE;  }
	
	if (g_sSystem.Check_TestCondtionRange(2, g_saTestCondition[m_nCalMode].nVBSrc) == FALSE)
	{	ErrMsg(); ERR.Reset(); return FALSE;  }

	if (g_sSystem.Check_TestCondtionRange(3, g_saTestCondition[m_nCalMode].nVSet) == FALSE)
	{	ErrMsg(); ERR.Reset(); return FALSE;  }

	if (g_sSystem.Check_TestCondtionRange(4, g_saTestCondition[m_nCalMode].nIRng) == FALSE)
	{	ErrMsg(); ERR.Reset(); return FALSE;  }

	if (g_sSystem.Check_TestCondtionRange(5, g_saTestCondition[m_nCalMode].nISet) == FALSE)
	{	ErrMsg(); ERR.Reset(); return FALSE;  }

	if (g_sSystem.Check_TestCondtionRange(6, g_saTestCondition[m_nCalMode].nDelay) == FALSE)
	{	ErrMsg(); ERR.Reset(); return FALSE;  }

	if (g_sSystem.Check_TestCondtionRange(7, g_saTestCondition[m_nCalMode].nAdCnt) == FALSE)
	{	ErrMsg(); ERR.Reset(); return FALSE;  }

	if (g_sSystem.Check_TestCondtionRange(8, g_saTestCondition[m_nCalMode].nFilter) == FALSE)
	{	ErrMsg(); ERR.Reset(); return FALSE;  }

	if (g_sSystem.Check_TestCondtionRange(9, g_saTestCondition[m_nCalMode].nIFilter) == FALSE)
	{	ErrMsg(); ERR.Reset(); return FALSE;  }

	if (g_sSystem.Check_TestCondtionRange(10, g_saTestCondition[m_nCalMode].nHvOffset) == FALSE)
	{	ErrMsg(); ERR.Reset(); return FALSE;  }

	if (g_sSystem.Check_TestCondtionRange(11, g_saTestCondition[m_nCalMode].nHVGain) == FALSE)
	{	ErrMsg(); ERR.Reset(); return FALSE;  }

	if (g_sSystem.Check_TestCondtionRange(12, g_saTestCondition[m_nCalMode].nIGain) == FALSE)
	{	ErrMsg(); ERR.Reset(); return FALSE;  }

	if (g_sSystem.Check_TestCondtionRange(13, g_saTestCondition[m_nCalMode].n4WGain) == FALSE)
	{	ErrMsg(); ERR.Reset(); return FALSE;  }

	if (g_sSystem.Check_TestCondtionRange(14, g_saTestCondition[m_nCalMode].sCount) == FALSE)
	{	ErrMsg(); ERR.Reset(); return FALSE;  }

	// Check_TestCondtionRange(15, g_saTestCondition[m_nCalMode].nTest)�� ������ �̸� ������. skip
	

	//----------------------------------
	// Test Condition ���� üũ
	
	// CalMode�� Range�� 171~175�ε� nTest�� 4W�� �ƴϸ� ���� ���
	CString strTemp("");	
	if (g_sSystem.Is4WTestCase(m_nCalMode) == TRUE) 
	{
		if (g_saTestCondition[m_nCalMode].nTest	!= TEST_4W)
		{
			strTemp.Format("Meas_CheckRange_TestCondition(): Test Condition Error. m_nCalMode=%d, But nTest=%d. nTest Should be %d!.\n",
							m_nCalMode, g_saTestCondition[m_nCalMode].nTest, TEST_4W);
			ERR.Set(CAL_CHECK_TEST_CONDITION, strTemp);
			ErrMsg(); ERR.Reset();  
			return FALSE;
		}

	}

	// CalMode�� Range�� 171~175�� �ƴѵ�  nTest�� 4W�̸� ���� ���
	else 
	{
		if (g_saTestCondition[m_nCalMode].nTest	== TEST_4W)
		{
			strTemp.Format("Meas_CheckRange_TestCondition(): Test Condition Error. m_nCalMode=%d, nTest=%d(4W). nTest Can't be %d!.\n",
							m_nCalMode, g_saTestCondition[m_nCalMode].nTest, TEST_4W);
			ERR.Set(CAL_CHECK_TEST_CONDITION, strTemp);
			ErrMsg(); ERR.Reset();  
			return FALSE;
		}
	}

	// nTest�� 4W�ε�, nFilter�� 4W�� �ƴϸ� ���� ���
	if (g_saTestCondition[m_nCalMode].nTest	== TEST_4W)
	{
		if( g_saTestCondition[m_nCalMode].nFilter != FT_B_4WIRE)
		{
			strTemp.Format("Meas_CheckRange_TestCondition(): Test Condition Error. nTest=%d(4W). nFilter=%d(Not 4Wire)!.\n",
							m_nCalMode, g_saTestCondition[m_nCalMode].nTest, g_saTestCondition[m_nCalMode].nFilter);
			ERR.Set(CAL_CHECK_TEST_CONDITION, strTemp);
			ErrMsg(); ERR.Reset();  
			return FALSE;
		}
	}

	// nTest�� 4W�� �ƴѵ�, nFilter�� 4W �� ���� ���
	if (g_saTestCondition[m_nCalMode].nTest	!= TEST_4W)
	{
		if( g_saTestCondition[m_nCalMode].nFilter == FT_B_4WIRE)
		{
			strTemp.Format("Meas_CheckRange_TestCondition(): Test Condition Error. nTest=%d(No 4W). nFilter=%d(4Wire)!.\n",
							m_nCalMode, g_saTestCondition[m_nCalMode].nTest, g_saTestCondition[m_nCalMode].nFilter);
			ERR.Set(CAL_CHECK_TEST_CONDITION, strTemp);
			ErrMsg(); ERR.Reset();  
			return FALSE;
		}
	}


	return TRUE;	
}


// Set VB  
// V SET ON    LV1, LV2, HV1, HV2
// if( nVRel==1) Sub71.AOnVSetMulti1(nVSet,0,0,0) for LV1 �� �ش�. 
// UI ��ºκ� �����ϰ� AVBRV1S()�� ������.
BOOL CCalibDialog::Meas_SetVb() 
{
	CString strTemp, strTemp2, strVSrc;
	UINT 	dwRet = TRUE; 
	UINT	dwVbSrc, dwVSetVal, dwVbHL;
	
	// VB source(nVRel) Col2  // 1: C_LV1, 2: C_LV2, 3: C_HV1, 4: C_HV2
	switch(g_saTestCondition[m_nCalMode].nVBSrc)
	{
		case C_LV1: dwVbSrc = LV1;	dwVbHL = LVB; 	strVSrc = "LV1"; break;
		case C_LV2: dwVbSrc = LV2;	dwVbHL = LVB; 	strVSrc = "LV2"; break;
		case C_HV1: dwVbSrc = HV1;	dwVbHL = HVB; 	strVSrc = "HV1"; break;
		case C_HV2: dwVbSrc = HV2;	dwVbHL = HVB; 	strVSrc = "HV2"; break;
		default: break;		// default case�� ������ range check�� �Ͽ����Ƿ� �������� �������� ����.
	}

	dwVSetVal = g_saTestCondition[m_nCalMode].nVSet;	// Voltage Set		Col3  // 1~ 300

	// Volt ���� DA ������ ����  (col3 Volatage Set  1~300 )
	UNIT_CONVERT(dwVSetVal, VOLT_TO_DA);	//  ex) 5 -> 15000  

	// sSET_VB:   DA ���� IOī��� ������.		
	//             Sub71.AOnVBSet1(1,av) �� ����
	dwRet = MM.Set(dwVbSrc, dwVSetVal);
	strTemp.Format("Meas_SetVb(): MM.Set(%s, %d) %s\n", 
			strVSrc, dwVSetVal, (dwRet == FALSE)? "Error" : "OK");
	if (dwRet == FALSE) {
		strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
		ERR.Set(ERR.Get(), strTemp2); 
		ErrMsg(MB_OK, FALSE); ERR.Reset();  
	}
	MyTrace(PRT_LEVEL1, "%s\n", strTemp);

	return (BOOL)dwRet;
}

#define VB_READ_COUNT		5			// �� ���� 3 �̻��̾�� device by zero���� ������ �� ����
#define VB_READ_RETRY_COUNT		10			
// ADCA ���� VB�� read.  
// Sub71.AOnReadVB1(1) �� ����  ;// VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 
BOOL CCalibDialog::Meas_ReadVb() 
{
	CString strTemp, strVSrc;
	UINT 	dwRet = TRUE;
	UINT	dwVbSrc, dwVbHL;

	// 4Wire�� ������ VB read ���� �ʴ´�. nVSet ���� �׳� �Ҵ� 
	if (g_saTestCondition[m_nCalMode].nTest == TEST_4W)
	{
		UINT	dwVSetVal;
		g_sSystem.m_dVb[m_nCalMode] = g_saTestCondition[m_nCalMode].nVSet;

		dwVSetVal = g_saTestCondition[m_nCalMode].nVSet;	// Voltage Set		Col3  // 1~ 300
		UNIT_CONVERT(dwVSetVal, VOLT_TO_DA);	// Volt ���� DA ������ ����  

		g_sSystem.m_dAdVb[m_nCalMode] = dwVSetVal;
		MyTrace(PRT_LEVEL1, "Meas_ReadVb(): 4W, Use nVet for VB. dAdVb= %.3f, dVb=%.3f\n\n", 
				g_sSystem.m_dAdVb[m_nCalMode], g_sSystem.m_dVb[m_nCalMode] );
		return TRUE;
	}

	
	// VB source(nVRel) Col2  // 1: C_LV1, 2: C_LV2, 3: C_HV1, 4: C_HV2
	switch(g_saTestCondition[m_nCalMode].nVBSrc)
	{
		case C_LV1: dwVbSrc = LV1;	dwVbHL = LVB; 	strVSrc = "LV1"; break;
		case C_LV2: dwVbSrc = LV2;	dwVbHL = LVB; 	strVSrc = "LV2"; break;
		case C_HV1: dwVbSrc = HV1;	dwVbHL = HVB; 	strVSrc = "HV1"; break;
		case C_HV2: dwVbSrc = HV2;	dwVbHL = HVB; 	strVSrc = "HV2"; break;
		default: break;		// default case�� ������ range check�� �Ͽ����Ƿ� �������� �������� ����.
	}
 	
	// sSET_VB_HL:  MM.Get(V)�ϱ� ���� VB�� �����Ѵ�.    (ex: LVB)
	dwRet = MM.Set(dwVbHL, dwVbSrc);	    // sSET_VB_HL: ex) MM.Set(LVB, LV1);
	strTemp.Format("Meas_ReadVb(): MM.Set(%s, %s) %s\n", 
			(dwVbHL == LVB) ? "LVB":"HVB", strVSrc, (dwRet == FALSE)? "Error" : "OK");
	if (dwRet == FALSE)	{
		ERR.Set(ERR.Get(), strTemp);
		ErrMsg(); ERR.Reset(); 
		return (BOOL)dwRet;
	}
	MyTrace(PRT_LEVEL1, "%s\n", strTemp);
	Delay(5, msec);
	

	// sSET_ADC_A:  MM.Get(V)�ϱ� ���� ADCA Filter�� VBFT�� �����Ѵ�. 
	dwRet = MM.Set(VBFT);			 	// sSET_ADC_A: 
	strTemp.Format("Meas_ReadVb(): MM.Set(VBFT) %s\n", (dwRet == FALSE)? "Error" : "OK");
	if (dwRet == FALSE) {
		ERR.Set(ERR.Get(), strTemp);
		ErrMsg(); ERR.Reset(); 
		return (BOOL)dwRet;
	}
	MyTrace(PRT_LEVEL1, "%s\n", strTemp);
	//Delay(5, msec);

	// sGET_VI:  ADC A ���� ������(AD)�� read�Ѵ�.
	UINT 	dwVal[VB_READ_COUNT], dwMin=0, dwMax=0, dwSum=0;
	double 	dAdVb = -1;


	for (int retry=0; retry < VB_READ_RETRY_COUNT; retry++)
	{
		dwRet = TRUE;	//  retry������ ���� TRUE�� �缳��.

		dAdVb = -1;
		dwMin = 0; dwMax = 0; dwSum = 0;
		::FillMemory(dwVal, sizeof(dwVal), -1);

		for (int i=0; i < VB_READ_COUNT; i++)		// 5ȸ read�Ͽ� ����� ����.
		{
			// check return value
			dwVal[i] = (UINT)MM.Get(V);	// sGET_VI:  read from IO Card
			if( dwVal[i] == (UINT)(-1) ) 	
			{
				strTemp.Format("MM.Get(V) dwVal[%d]=%u ERROR.", i, dwVal[i]);
				ERR.Set(ERR.Get(), strTemp);
				ErrMsg(); 	ERR.Reset();
				dwRet = FALSE;
				break;		// ���� for�� break.
			}
		
			if (i == 0) {
				dwMax = dwVal[i];
				dwMin = dwVal[i];
			}
			else //min, max ���� ��󳽴�.
			{
				if (dwMax < dwVal[i])		// �ִ밪���� ���簪�� ũ�� �ִ밪 ����
					dwMax = dwVal[i];
				if (dwMin > dwVal[i])		// �ּҰ����� ���簪�� ������ �ּҰ� ����
					dwMin = dwVal[i];
			}
			dwSum += dwVal[i];	
		}
	
		if (dwRet == FALSE) 	// ���� for�� break��� . MM.Get(V) ���� ��Ȳ.
		{
			MyTrace(PRT_LEVEL1, "Meas_ReadVb() retry=%d: MM.Get(V) %s: \n\n", retry, (dwRet == FALSE)? "ERROR" : "OK" );
	
			continue;		// ���� retry�� ����
		}

		// ���� for�� break�Ȱ� �ƴ϶��. ��, ���� read ��Ȳ�̸� ��հ� ��� ���� 
		dAdVb = (dwSum - dwMax - dwMin) / (VB_READ_COUNT -2);  // min, max �����ϰ� 3ȸ�� ���� ��հ�
		g_sSystem.m_dAdVb[m_nCalMode] = dAdVb;

		double dAdVb2 = dAdVb;
		switch(g_saTestCondition[m_nCalMode].nVBSrc)
		{
			case C_LV1: case C_LV2: 
				g_sSystem.m_dVb[m_nCalMode] = UNIT_CONVERT(dAdVb2, AD_TO_LVOLT);	
				break;
			case C_HV1: case C_HV2: 
				g_sSystem.m_dVb[m_nCalMode] = UNIT_CONVERT(dAdVb2, AD_TO_HVOLT);	
				break;
			default: // default case�� �̹� range check�� �Ͽ����Ƿ� �������� �������� ����.
				break;		
		}

		// Ad��հ��� ���� VB Val Check �ؼ� �̻��� ������(ret=FALSE) retry. (max 3ȸ)
		if (Meas_CheckRange_VbVal(dAdVb) == FALSE)
		{

//PC Test �� ������ ���� ����� ������ �����Ƿ� retry error ó���� skip
#ifndef __PC_TEST__
			dwRet = FALSE;

			if ((retry + 1) == VB_READ_RETRY_COUNT) 	// 3ȸ vb read retry ��� ���н� �޽����ڽ� ���. 
				ErrMsg(MB_OK, FALSE); 	
			ERR.Reset();	// �޽����ڽ� ��� ���ϴ��� ������ ���� Set�� ���� ERR.Reset()

			Delay(100, msec);	// 100ms ���� 
			MyTrace(PRT_LEVEL1, "Meas_ReadVb() retry=%d: MM.Get(V) %s: dAdVb= %.3f, dVb= %.3f \n\n", 
						retry, (dwRet == FALSE)? "ERROR" : "OK", 
						g_sSystem.m_dAdVb[m_nCalMode], g_sSystem.m_dVb[m_nCalMode]);

			continue;		// ���� retry�� ����	
#endif
		}

		// VB Val Check �ؼ� �̻� ������ retry ����. 
		MyTrace(PRT_LEVEL1, "Meas_ReadVb() retry=%d: MM.Get(V) %s: dAdVb= %.3f, dVb= %.3f \n\n", 
					retry, (dwRet == FALSE)? "ERROR" : "OK", 
					g_sSystem.m_dAdVb[m_nCalMode], g_sSystem.m_dVb[m_nCalMode]);
		//break;
		return (BOOL)dwRet;		// �ܺ� for�� Ż��. dwRet�� ���⼭ ������ true��.

	}	

	return (BOOL)dwRet;
}

BOOL CCalibDialog::Meas_CheckRange_VbVal(double dAdVb_ReadVal) 
{
	CString	strTemp;
	UINT 	dwVSetVal;
	double	dVbGap, dVbHigh, dVbLow;


	//-------------------------------------------------------------------
	// check1: Vb AD Read���� 60000(NonAd ���� 300 V)�̻��̸� ����
	
	// 2018.01.24 �źμ� �����ǰ�,  250V ���� ū Calibration case�� ������� �� ��. 
	//            300V ���� ū ���̽��� üũ�� 250V �̻��� ������� ������ check1�� üũ�� �ʿ䵵 ����. 
	//            151, 152 ��忡�� 300V VB Set�� �ϸ� check1 �ڵ忡 �ݵ�� �ɷ��� ������ �� �ǰ� ��.
	// 2018.01.26 �迵ö����. 
	//            300V���� Ŭ ���� ����. check1 ���ʿ�. ���� ��
/* 	
	double dAdVb2 = dAdVb_ReadVal, dVb= 0;
	switch(g_saTestCondition[m_nCalMode].nVBSrc)
	{
		case C_LV1: case C_LV2: 
			dVb = UNIT_CONVERT(dAdVb2, AD_TO_LVOLT);	
			break;
		case C_HV1: case C_HV2: 
			dVb = UNIT_CONVERT(dAdVb2, AD_TO_HVOLT);	
			break;
		default: // default case�� �̹� range check�� �Ͽ����Ƿ� �������� �������� ����.
			break;		
	}

	// 2018.01.24 HV �϶����� �Ʒ� �ڵ尡 ȿ�� �ִ�. LV�� �� üũ�� �ɸ��� ����.
	//            LV�� ������ dVb 20V����  ũ����  20.1 �̷����̹Ƿ� 300V���ٴ� Ŭ �� ����. 
	
	if (dVb > MAX_HV)		// > 300 �̸� 
	{
		strTemp.Format("Meas_CheckRange_VbVal: dAdVb_ReadVal=%.2f, dVb=%.2f: Range(dVb <= %d) Over! Check HW Power please!.", 
				dAdVb_ReadVal, dVb, MAX_HV );
		ERR.Set(CAL_CHECK_HW, strTemp);
		MyTrace(PRT_LEVEL1, "%s\n", strTemp);
		return FALSE;	
	}
*/

	//-------------------------------------------------------------------
	// check2: Vb AD Read���� ����(+5%) ����(-5%) �� ����� ����
	//         ���������� ����(+10%) ����(-10%)
	
	// VB Read ���� AD �����̹Ƿ� AD ���� ���¿��� ���Ѵ�.
	dwVSetVal = g_saTestCondition[m_nCalMode].nVSet;	// Voltage Set		Col3  // 1~ 300
	UNIT_CONVERT(dwVSetVal, VOLT_TO_DA);	//  ex) 5 -> 15000  (for LV)

	if(g_saTestCondition[m_nCalMode].nVSet < 5){	 // ���������϶� 
		// ex) nVSet:1V, dwVSetVal: 3000    dVbGap = 3000 *0.10 = 300 
		dVbGap=dwVSetVal*(10*0.01);	// 10%
	}
	else{
		// ex) nVSet:10V, dwVSetVal: 30000   dVbGap = 30000 * 0.05 = 1500 
		dVbGap=dwVSetVal*(5*0.01);	// 5%
	}
	dVbLow = dwVSetVal-dVbGap;		//son VB Read ���Ѱ�   ex) dwVSetVal:30000,  dVbLow: 28500 
	dVbHigh= dwVSetVal+dVbGap;		//son VB Read ���Ѱ�   ex) dwVSetVal:30000,  dVbHigh: 31500 

	//son VB read ���� �����̳� ���Ѱ��� ���� �ʴ� ���
	if( dAdVb_ReadVal < dVbLow || dAdVb_ReadVal > dVbHigh)
	{
		strTemp.Format("Meas_CheckRange_VbVal: dAdVb_ReadVal=%.2f: Range(%.2f <= dAdVb_ReadVal <= %.2f) Over! Check HW please!.", 
				dAdVb_ReadVal, dVbLow, dVbHigh );
		ERR.Set(CAL_CHECK_HW, strTemp);
		MyTrace(PRT_LEVEL1, "%s\n", strTemp);
		return FALSE;	
	}

	return TRUE;
}

// ���-> ���� ��ȯ��  VB Out Check
// ������ 200V Openó�� ����� �����ߴٰ� ���ڱ� 5V 4W ���� ������ �����ϸ� ������ ��.
// ������ �����ߴ� ��л��°� ������ ������ ��ٸ��� ���� ���.
BOOL CCalibDialog::Meas_VbOutCheck() 
{
	CString strTemp, strTemp2;
	int		dwRet = TRUE;

	// VB_OUT_CHK On  
	dwRet = MM.Set(VB_OUT);    // sSET_VB_OUT: 
	if (dwRet == FALSE)
	{
		strTemp.Format("Meas_VbOutCheck(): MM.Set(VB_OUT) ERROR.");
		ERR.Set( ERR.Get(), strTemp);
		ErrMsg(); ERR.Reset(); 
		return (BOOL)dwRet;
	}
	MyTrace(PRT_LEVEL1, "Meas_VbOutCheck(): MM.Set(VB_OUT) %s\n", (dwRet == FALSE)? "Error" : "OK");

	// MM.Get(V)�ϱ� ���� Filter�� VBFT�� �����Ѵ�. 
	dwRet = MM.Set(VBFT);    // sSET_ADC_A: 
	if (dwRet == FALSE)
	{
		strTemp.Format("Meas_VbOutCheck(): MM.Set(VBFT) ERROR.");
		ERR.Set( ERR.Get(), strTemp);
		ErrMsg(); ERR.Reset(); 
		return (BOOL)dwRet;
	}
	MyTrace(PRT_LEVEL1, "Meas_VbOutCheck(): MM.Set(VBFT) %s\n", (dwRet == FALSE)? "Error" : "OK");

	double dVb = -1;
	UINT dwAdVb, dwAdVb2;
	for (int i=0; i < 300; i++)
	{

		// VB Read: VB OUT �� Ȯ��
		dwAdVb = (UINT)MM.Get(V);		// sGET_VI:  read from IO Card
		if(dwAdVb == (UINT)(-1))	// MM.Get(V) ���� 
		{
			strTemp.Format("Meas_VbOutCheck(): MM.Get(V) ERROR: dwAdVb=%u\n ", dwAdVb);
			strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
			ERR.Set(ERR.Get(), strTemp2);
			ErrMsg(); 	ERR.Reset();
			dwRet = FALSE;
			continue;
		}
		dwRet = TRUE;

		dwAdVb2 = dwAdVb;
		dVb   = UNIT_CONVERT(dwAdVb2, AD_TO_HVOLT);
		MyTrace(PRT_LEVEL2, "Meas_VbOutCheck(): MM.Get(V) OK: dwAdVb=%u, dVb=%.3f, nVSet=%d\n ", 
				dwAdVb, dVb, g_saTestCondition[m_nCalMode].nVSet);
		if(i<=20) 
			Delay(1,  msec);	//1ms	=> 1ms* 20 = 20ms
		else if(i<=60) 
			Delay(5,  msec);	//5ms	=> 5ms* 40 = 200 ms
		else 
			Delay(10, msec);	//10ms	=> 10ms * 240 = 2400ms

		// VB Out Check �� ��(dVb) �� nVSet ������  ũ�ٸ� ������ ������ ��� VB Out�� üũ�Ѵ�.
		//   ��, dVb�� nVSet���� �۰ų� ���ٸ� ������ �ߴ��Ѵ�. 
		if (dVb <= g_saTestCondition[m_nCalMode].nVSet)
		{
			MyTrace(PRT_LEVEL1, "Meas_VbOutCheck(): dwAdVb=%u, (dVb(%.3f) <= nVSet(%d))? is TRUE(OK). Break VbOutCheck. \n", 
					dwAdVb, dVb, g_saTestCondition[m_nCalMode].nVSet);
			break;
		}
	}

	//---------------------------------
	// ���� ���� ���󺹱�
	// Meas_VbOutCheck() ���� ���Ŀ� ������ VB_HL ���¸� ���󺹱��Ѵ�.  (Read_Vb() �ڵ忡�� ������)
	
/*
	switch(g_saTestCondition[m_nCalMode].nVBSrc)
	{
		case C_LV1: case C_LV2:
			MM.m_pMicroM->Off(VB_OUT_CHK);   // VB_OUT_CHK(122) Off 
			MM.m_pMicroM->Off(VB_SEN_SEL);   // VB_SEN_SEL(42) 0: LV
			break;

		case C_HV1: case C_HV2:
			MM.m_pMicroM->Off(VB_OUT_CHK);   // VB_OUT_CHK(122) Off 
			MM.m_pMicroM->On(VB_SEN_SEL);    // VB_SEN_SEL(42) 1: HV
			break;
	}
*/

	CString strHLVB, strVSrc;
	switch(g_saTestCondition[m_nCalMode].nVBSrc)
	{
		case C_LV1: 
			dwRet = MM.Set(LVB, LV1);	strHLVB = "LVB";  strVSrc = "LV1";  
			break;
		case C_LV2:
			dwRet = MM.Set(LVB, LV2);	strHLVB = "LVB";  strVSrc = "LV2";    
			break;
		case C_HV1: 
			dwRet = MM.Set(HVB, HV1);	strHLVB = "HVB";  strVSrc = "HV1";    
			break;
		case C_HV2:
			dwRet = MM.Set(HVB, HV2);	strHLVB = "HVB";  strVSrc = "HV2";    
			break;
	}
	strTemp.Format("Meas_VbOutCheck(): MM.Set(%s, %s) %s\n", 
			strHLVB, strVSrc, (dwRet == FALSE)? "Error" : "OK");
	if (dwRet == FALSE)	{
		ERR.Set(ERR.Get(), strTemp);
		ErrMsg(); ERR.Reset(); 
		return (BOOL)dwRet;
	}
	MyTrace(PRT_LEVEL1, "%s\n", strTemp);

	// CModeSub71::AOn_VB_CheckOut3_Stop()�� �ٸ� �κ�!
	// �� �������� Ixud_WriteDOBit(0,10,2,0) ��, ADC_VB_SEL OFF �� ������ �ϴµ�, VBFT�� OFF �ϴ� �����̴�. 
	// Meas_VbOutCheck() �Ϸ� ���Ŀ� Meas_SetAdcAFilter()�� �ϸ鼭 �ڿ������� ADC_VB_SEL OFF�� �ǹǷ�
	// ACE400 original �ڵ������� ���⿡ ���� ���� �ʾ���.

	// I ���� 300 �̻��̸� Vb OutCheck ����� ���� �����Ƿ� ������ ����.
	if (i >= 300) {
		strTemp.Format("Meas_VbOutCheck() Error! i=%d, nVSet=%d > dVb=%d! 2620 mSec OverTime.\n", 
											i, g_saTestCondition[m_nCalMode].nVSet, dVb );  
		ERR.Set(OT_VB_OUTCHECK_ERR, strTemp);
		ErrMsg(); 	ERR.Reset();
		return FALSE;
	}	

	return (BOOL)dwRet;
}

BOOL CCalibDialog::Meas_SetLvcHvc() 
{
	UINT 	dwRet = TRUE; 
	CString	strTemp, strTemp2;

	switch(g_saTestCondition[m_nCalMode].nFilter)
	{
		case FT_B_VC_FAST: 	
		{
			// VB source(nVRel) Col2  // 1: C_LV1, 2: C_LV2, 3: C_HV1, 4: C_HV2
			switch(g_saTestCondition[m_nCalMode].nVBSrc)
			{
				case C_LV1: case C_LV2: // LVC Set
					dwRet = MM.Set(LVC, TRUE);	// LVC, TRUE
					strTemp.Format("Meas_SetLvcHvc(): MM.Set(LVC, TRUE) %s.\n", (dwRet == FALSE)? "Error" : "OK"	);
					if (dwRet == FALSE) {	
						strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
						ERR.Set(ERR.Get(), strTemp2); 
						ErrMsg(MB_OK, FALSE); ERR.Reset();  
					}
					MyTrace(PRT_LEVEL1, "%s\n", strTemp);
					m_nHLVC = VCFFT_LVC;
					break;

				case C_HV1: case C_HV2: // HVC Set
					dwRet = MM.Set(HVC, TRUE);	// HVC, TRUE
					strTemp.Format("Meas_SetLvcHvc(): MM.Set(HVC, TRUE) %s.\n", (dwRet == FALSE)? "Error" : "OK"	);
					if (dwRet == FALSE) {	
						strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
						ERR.Set(ERR.Get(), strTemp2); 
						ErrMsg(MB_OK, FALSE); ERR.Reset();  // bTraceOn=FALSE, ������ Trace ���� �޽����ڽ��� ���
					}		
					MyTrace(PRT_LEVEL1, "%s\n", strTemp);
					m_nHLVC = VCFFT_HVC;
					break;

				default: // default case�� �̹� range check�� �Ͽ����Ƿ� �������� �������� ����.
					break;		
			}
		}
			break;

		case FT_B_VC_SLOW:		// �̻��
			break;
		
		// LVC, HVC  ��� OFF
		case FT_B_HV_OPEN:	case FT_B_4WIRE:
		{
			dwRet = MM.Set(HVC, FALSE);	// HVC LVC�� ��� FALSE ó�� 
			strTemp.Format("Meas_SetLvcHvc(): MM.Set(LVC_HVC, FALSE) %s.\n", (dwRet == FALSE)? "Error" : "OK"	);
			if (dwRet == FALSE) {	
				strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
				ERR.Set(ERR.Get(), strTemp2); 
				ErrMsg(MB_OK, FALSE); ERR.Reset();  
			}		// bTraceOn=FALSE, ������ Trace ���� �޽����ڽ��� ���
			MyTrace(PRT_LEVEL1, "%s\n", strTemp);
		}
			break;

		default: 
			break;		
	}
	
	return dwRet;
}

BOOL CCalibDialog::Meas_SetAdcBFilter() 
{
	UINT	dwRet = TRUE;
	CString	strTemp, strTemp2;

	switch(g_saTestCondition[m_nCalMode].nFilter)
	{
		case FT_B_VC_FAST: 	
			dwRet = MM.Set(VCFFT);					// sSET_ADC_B
			strTemp.Format("Meas_SetAdcBFilter(): MM.Set(VC_FAST Filter) %s.\n", (dwRet == FALSE)? "Error" : "OK");
			if (dwRet == FALSE) {	
				strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
				ERR.Set(ERR.Get(), strTemp2); 
				ErrMsg(MB_OK, FALSE); ERR.Reset();  
			}		
			MyTrace(PRT_LEVEL1, "%s\n", strTemp);
			break;

		case FT_B_VC_SLOW:		// �̻��
			break;

		case FT_B_HV_OPEN:	
			// HV Gain ���� ���� -----------------
			if (g_saTestCondition[m_nCalMode].nHVGain == 0)
			{
				CString strTemp("");
				strTemp.Format("Meas_SetAdcBFilter(): MM.Set(HVO, %d) nHVGain=0 Error!\n if(nFilter=3) nHvGain Can't be 0, Check TestCondtion!.\n", 
							g_saTestCondition[m_nCalMode].nHVGain);			
				ERR.Set(CAL_CHECK_TEST_CONDITION, strTemp);
				ErrMsg(); ERR.Reset();  
			}
			else
			{
				// sSET_HVO : HV Gain
				dwRet = MM.Set(HVO, g_saTestCondition[m_nCalMode].nHVGain);	
				strTemp.Format("Meas_SetAdcBFilter(): MM.Set(HVO, %d: nHVGain=%d) %s.\n", 
						g_saTestCondition[m_nCalMode].nHVGain, g_saTestCondition[m_nCalMode].nHVGain,
						(dwRet == FALSE)? "Error" : "OK");
				if (dwRet == FALSE) {	
					strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
					ERR.Set(ERR.Get(), strTemp2); 
					ErrMsg(MB_OK, FALSE); ERR.Reset();  
				}		
				MyTrace(PRT_LEVEL1, "%s\n", strTemp);
			}

			// HV Open AdcB Filter ���� ----------
			dwRet = MM.Set(HVOFT);					// sSET_ADC_B
			strTemp.Format("Meas_SetAdcBFilter(): MM.Set(ADCB HV OPEN Filter) %s.\n", (dwRet == FALSE)? "Error" : "OK");
			if (dwRet == FALSE) {	
				strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
				ERR.Set(ERR.Get(), strTemp2); 
				ErrMsg(MB_OK, FALSE); ERR.Reset();  
			}		
			MyTrace(PRT_LEVEL1, "%s\n", strTemp);

			break;
		case FT_B_4WIRE:
			strTemp.Format("Meas_SetAdcBFilter(): MM.Set(ADCB 4 WIRE Filter) %s.\n", (dwRet == FALSE)? "Error" : "OK");
			dwRet = MM.Set(FWFT);					// sSET_ADC_B
			if (dwRet == FALSE) {	
				strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
				ERR.Set(ERR.Get(), strTemp2); 
				ErrMsg(MB_OK, FALSE); ERR.Reset();  
			}		
			MyTrace(PRT_LEVEL1, "%s\n", strTemp);
			break;
	}


	return dwRet;
}

BOOL CCalibDialog::Meas_SetAdcAFilter() 
{
	UINT	dwRet = TRUE;
	CString strTemp("");
	CString strTemp2("");

	// FT_A_I_FAST �Ǵ� FT_A_I_SLOW �� ��� I High Gain ����.
	switch (g_saTestCondition[m_nCalMode].nIFilter)
	{
		case FT_A_I_FAST: 	
		case FT_A_I_SLOW:		
			// I HIGH GAIN ON  :    Short ������ ����
			if (g_saTestCondition[m_nCalMode].nIGain == I_HIGH_GAIN_ON)		// 2
			{
				//dwRet = MM.m_pMicroM->On(I_GAIN_CMD);
				dwRet = MM.Set(I_GAIN, _ON);
				strTemp.Format("Meas_SetAdcAFilter(): On(I_GAIN_CMD): I High Gain On. %s.\n", (dwRet == FALSE)? "Error" : "OK");
				if (dwRet == FALSE) {	
					strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
					ERR.Set(ERR.Get(), strTemp2); 
					ErrMsg(MB_OK, FALSE); ERR.Reset();  // bTraceOn=FALSE, ������ Trace ���� �޽����ڽ��� ���
				}		
				MyTrace(PRT_LEVEL1, "%s\n", strTemp);

			}
			// I HIGH GAIN OFF
			else
			{
				//dwRet = MM.m_pMicroM->Off(I_GAIN_CMD);
				dwRet = MM.Set(I_GAIN, _OFF);
				strTemp.Format("Meas_SetAdcAFilter(): Off(I_GAIN_CMD): I High Gain Off. %s.\n", (dwRet == FALSE)? "Error" : "OK");
				if (dwRet == FALSE) {	
					strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
					ERR.Set(ERR.Get(), strTemp2); 
					ErrMsg(MB_OK, FALSE); ERR.Reset();  
				}		
				MyTrace(PRT_LEVEL1, "%s\n", strTemp);
			}
			break;
	}

	// ADC A Filter ����
	switch (g_saTestCondition[m_nCalMode].nIFilter)
	{
		case FT_A_I_FAST: 	
			dwRet = MM.Set(IFFT);					// sSET_ADC_B
			strTemp.Format("Meas_SetAdcAFilter(): MM.Set(ADCA I_FAST Filter) %s.\n", (dwRet == FALSE)? "Error" : "OK");
			if (dwRet == FALSE) {	
				strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
				ERR.Set(ERR.Get(), strTemp2); 
				ErrMsg(MB_OK, FALSE); ERR.Reset();  
			}		// bTraceOn=FALSE, ������ Trace ���� �޽����ڽ��� ���
			MyTrace(PRT_LEVEL1, "%s\n", strTemp);
			break;

		case FT_A_I_SLOW:		
			dwRet = MM.Set(ISFT);					// sSET_ADC_B
			strTemp.Format("Meas_SetAdcAFilter(): MM.Set(ADCA I_SLOW Filter) %s.\n", (dwRet == FALSE)? "Error" : "OK");
			if (dwRet == FALSE) {	
				strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
				ERR.Set(ERR.Get(), strTemp2); 
				ErrMsg(MB_OK, FALSE); ERR.Reset();  
			}		
			MyTrace(PRT_LEVEL1, "%s\n", strTemp);
			break;

		case FT_A_VB:	
			dwRet = MM.Set(VBFT);					// sSET_ADC_B
			strTemp.Format("Meas_SetAdcAFilter(): MM.Set(ADCA VB Filter) : %s\n", (dwRet == FALSE)? "Error" : "OK");
			if (dwRet == FALSE) {	
				strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
				ERR.Set(ERR.Get(), strTemp2); 
				ErrMsg(MB_OK, FALSE); ERR.Reset();  
			}		
			MyTrace(PRT_LEVEL1, "%s\n", strTemp);
			break;
	}


	return dwRet;
}

BOOL CCalibDialog::Meas_Set4WGain() 
{
	UINT	dwRet = TRUE;
	CString strTemp("");
	CString strTemp2("");

	// 4W�� �ƴ� ���(Open, Short)  ---------
	if (g_saTestCondition[m_nCalMode].nTest != TEST_4W)
	{
		// 4W Gain 1�� ����
		dwRet = MM.Set(FW_GAIN, F4W_GAIN_Sx1000);		
		strTemp.Format("Meas_Set4WGain(): MM.Set(FW_GAIN, %d(F4W_GAIN_Sx1000)) %s.\n",
				F4W_GAIN_Sx1000, (dwRet == FALSE)? "Error" : "OK");
		if (dwRet == FALSE)
		{	
			strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
			ERR.Set(ERR.Get(), strTemp2); 
			ErrMsg(MB_OK, FALSE); ERR.Reset();  
		}		
		MyTrace(PRT_LEVEL1, "%s\n", strTemp);
		return dwRet;
	}

	// 4W �� ���  --------
	switch (g_saTestCondition[m_nCalMode].n4WGain)
	{
		case F4W_GAIN_Sx1000:	// 1
		case F4W_GAIN_Fx10:		// 2
		case F4W_GAIN_Fx100:	// 3
		case F4W_GAIN_Fx1000:	// 4
			dwRet = MM.Set(FW_GAIN, g_saTestCondition[m_nCalMode].n4WGain);		
			strTemp.Format("Meas_Set4WGain(): MM.Set(FW_GAIN, %d(%s)) %s.\n",
				g_saTestCondition[m_nCalMode].n4WGain,
				(g_saTestCondition[m_nCalMode].n4WGain == F4W_GAIN_Sx1000) ? "Sx1000" : 
				(g_saTestCondition[m_nCalMode].n4WGain == F4W_GAIN_Fx10) ? "Fx10" : 
				(g_saTestCondition[m_nCalMode].n4WGain == F4W_GAIN_Fx100) ? "Fx100" : 
				(g_saTestCondition[m_nCalMode].n4WGain == F4W_GAIN_Fx1000) ? "Fx1000" : "Invalid 4W Gain Val",
				(dwRet == FALSE)? "Error" : "OK");
			if (dwRet == FALSE)
			{	
				strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
				ERR.Set(ERR.Get(), strTemp2); 
				ErrMsg(MB_OK, FALSE); ERR.Reset();  
			}		
			MyTrace(PRT_LEVEL1, "%s\n", strTemp);
			break;

		case F4W_GAIN_NOT_USE:	// 0
			CString strTemp("");
			strTemp.Format("Meas_Set4WGain(): n4wGain=0, 4wGain Can't be 0 in 4WTest. Check Testcondition!"); 
			ERR.Set(CAL_CHECK_TEST_CONDITION, strTemp);
			ErrMsg(); ERR.Reset();
			break;
	}

	return dwRet;
}






BOOL CCalibDialog::Meas_SetIRange() 
{
	UINT 	dwRet = TRUE;
	CString strTemp("");
	CString strTemp2("");	// ErrMsg ��¿�

	//---------------------------
	// 1. I Range ���� 
	dwRet = MM.Set(I, -1, g_saTestCondition[m_nCalMode].nIRng - 1);	
	strTemp.Format("Meas_SetIRange():Set nIRng=%d => MM.Set(I, -1, I_RANGE%d) %s\n", 
			g_saTestCondition[m_nCalMode].nIRng, (g_saTestCondition[m_nCalMode].nIRng - 1),
			(dwRet == FALSE)? "Error" : "OK");
	if (dwRet == FALSE) {	
		strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
		ERR.Set(ERR.Get(), strTemp2); 
		ErrMsg(MB_OK, FALSE); ERR.Reset();  
	}		
	MyTrace(PRT_LEVEL1, "%s\n", strTemp);

	return dwRet;
}


BOOL CCalibDialog::Meas_SetIValue() 
{
	UINT 	dwRet = TRUE;
	CString strTemp("");
	CString strTemp2("");	// ErrMsg ��¿�

	//---------------------------
	// 2. Set I Value 

	// CC ������ �Ѵ�.  ���� CV ���� �����Ƿ� ���� CV �� ������ ������ ����ϰ� ����.
	switch (g_saTestCondition[m_nCalMode].nCC)
	{
		case C_CC:
			//dwRet = MM.m_pMicroM->Off(CC_CV_MODE);
			dwRet = MM.Set(CC_CV, _OFF);	// 0: CC, 1: CV
			MyTrace(PRT_LEVEL1, "Meas_SetIValue(): CC_CV_MODE Off(CC) %s\n\n", (dwRet == FALSE)? "Error" : "OK");
			break;

		case C_CV:
			strTemp.Format("Meas_SetIValue(): calMode=%d, nCC=%d(CV). Check TestCondtion File!\n", 
					m_nCalMode, g_saTestCondition[m_nCalMode].nCC );
			ERR.Set(CAL_CHECK_TEST_CONDITION, strTemp);
			ErrMsg(); ERR.Reset();
			return FALSE;
	}

	// Set I Value
	
	// TestCondtion�� nISet ���� ������*10 ���̹Ƿ� /10�Ͽ� ����Ѵ�.
	double 	dISet = (double) (g_saTestCondition[m_nCalMode].nISet / 10); 

	// SelfTest�� ��쿡�� I ���� 0���� �ؾ� �Ѵ�.
	if (g_sSystem.IsSelfTestCase(m_nCalMode) == TRUE)
		dISet = (double)0;

	double dISetCnvted = dISet;
	switch(g_saTestCondition[m_nCalMode].nIRng)
	{
		case IR1: // 20001~ 200000 uA	 	=> *=(50000/ 200000.))  => *=0.25   *1000�ϸ� *=250	 (ACE400: *=250)
		case IR2: // 10001~ 20000  uA	  	=> *=(50000/ 20000.)	=> *=2.5	*1000�ϸ� *=2500 (ACE400: *=2500)
		case IR3: // 2001 ~ 10000  uA		=> *=(50000/ 10000.)	=> *=5		*1000�ϸ� *=5000 (ACE400: *=2500 ?? error ���� )
			dISetCnvted *= 1000;		// mA�� uA�� ��ȯ. 
		case IR4: // 201  ~ 2000   uA		=> *=(50000/ 2000.) 	=> *=25		 (ACE400: *=25)
		case IR5: // 1    ~ 200    uA		=> *=(50000/ 200.)		=> *=250	 (ACE400: *=250)
			UNIT_CONVERT(dISetCnvted, I0_TO_DA+(g_saTestCondition[m_nCalMode].nIRng - 1)-I_RANGE0);
			break;

		default:
			strTemp.Format("Meas_SetIValue(): Check the nIRng=%d, Range(%d~%d) Over!!\n", 
					g_saTestCondition[m_nCalMode].nIRng, IR1, IR5 );
			ERR.Set(RANGE_OVER, strTemp);
			ErrMsg(); ERR.Reset();
			return FALSE;
	}

	dwRet = MM.Set(I, (int)dISetCnvted);		// sSET_I
	strTemp.Format("Meas_SetIValue(): Set I Value %s. nISet/10=%.3f, Converted ISet=%u\n", 
				(dwRet == FALSE)? "Error" : "OK", dISet, (int)dISetCnvted);
	if (dwRet == FALSE) {	
		strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
		ERR.Set(ERR.Get(), strTemp2); 
		ErrMsg(MB_OK, FALSE); ERR.Reset();  // bTraceOn=FALSE, ������ Trace ���� �޽����ڽ��� ���
	}		
	MyTrace(PRT_LEVEL1, "%s\n", strTemp);


	// Read I Value
	//   : ������ I ���� �ٽ� �о Ȯ���Ѵ�.  
	//   => ����� �ʿ� ����.  ���� MM.Get(VI) ���� VC�� I ���� �ѹ��� �о Ȯ��
	
	return dwRet;
}
	
// VB Apply
BOOL CCalibDialog::Meas_VbRelayOn() 
{
	UINT 	dwRet = TRUE;
	CString strTemp("");
	CString strTemp2("");

	//---------------------------------
	// ���� üũ 

	// nVBSrc�� HV �� ��� ����üũ
	if (g_saTestCondition[m_nCalMode].nVBSrc == C_HV1 
				|| g_saTestCondition[m_nCalMode].nVBSrc == C_HV2)
	{
		// 4W Test�̸鼭 vbSrc�� HV�� �� ����.  4W�� ��� VB Apply�ÿ� ȸ�μջ�ǹǷ�  H/W Protect �ʿ�.
		// 2017.11.14�� �źμ� ���� confirm
		if (g_saTestCondition[m_nCalMode].nTest	== TEST_4W)
		{
			strTemp.Format("Meas_VbRelayOn(): Test Condition Error. nTest=%d(4W), But nVBSrc=%d(HV), Stop for HW_Protect  !.\n",
							g_saTestCondition[m_nCalMode].nTest, g_saTestCondition[m_nCalMode].nVBSrc );
			ERR.Set(CAL_CHECK_TEST_CONDITION, strTemp);
			ErrMsg(); ERR.Reset();  
			return FALSE;
		}

		// vbSrc�� HV�ε� LVC�� ������ �� �ִٸ� ���� ȸ�μջ��
		// 2017.11.14�� �źμ� ���� confirm
		if (m_nHLVC == VCFFT_LVC)
		{
			strTemp.Format("Meas_VbRelayOn(): Check SW Logic Error. m_nHLVC=%d(LVC), But nVBSrc=%d(HV)!.\n",
							m_nHLVC, g_saTestCondition[m_nCalMode].nVBSrc );
			ERR.Set(SW_LOGIC_ERR, strTemp);
			ErrMsg(); ERR.Reset();  
			return FALSE;
		}
	}
	// nVBSrc�� LV �� ��� ����üũ
	else		
	{
		// vbSrc�� LV�ε� HVC�� ������ �� �ִٸ� ���� ó��. 
		// 2017.11.14�� �źμ� ���� confirm
		if (m_nHLVC == VCFFT_HVC)
		{
			strTemp.Format("Meas_VbRelayOn(): Check SW Logic Error. m_nHLVC=%d(HVC), But nVBSrc=%d(LV)!.\n",
							m_nHLVC, g_saTestCondition[m_nCalMode].nVBSrc );
			ERR.Set(SW_LOGIC_ERR, strTemp);
			ErrMsg(); ERR.Reset();  
			return FALSE;
		}
	}

	// 4W Test�̸鼭 nVSet���� 20V �ʰ� ����̸� �ߴ�.  4W�� ��� VB Apply�ÿ� ȸ�μջ�ǹǷ�  H/W Protect �ʿ�.
	// 2017.11.15�� �źμ� ���� confirm
	if (g_saTestCondition[m_nCalMode].nTest	== TEST_4W
			&& g_saTestCondition[m_nCalMode].nVSet > MAX_LV)
	{
		strTemp.Format("Meas_VbRelayOn(): Test Condition Error. nTest=%d(4W), But nVSet=%d(> %d), Stop for HW_Protect  !.\n",
						g_saTestCondition[m_nCalMode].nTest, g_saTestCondition[m_nCalMode].nVSet, MAX_LV );
		ERR.Set(CAL_CHECK_TEST_CONDITION, strTemp);
		ErrMsg(); ERR.Reset();  
		return FALSE;
	}

	// nVset ���� 300V �ʰ��̸� �ߴ�.  for H/W Protect
	// 2017.11.15�� �迵�� ���� confirm
	if (g_saTestCondition[m_nCalMode].nVSet > MAX_HV)
	{
		strTemp.Format("Meas_VbRelayOn(): Test Condition Error. nVSet=%d(> %d), Stop for HW_Protect  !.\n",
						g_saTestCondition[m_nCalMode].nVSet, MAX_HV );
		ERR.Set(CAL_CHECK_TEST_CONDITION, strTemp);
		ErrMsg(); ERR.Reset();  
		return FALSE;
	}


	//-------------------------------------------------------
	// 3. VB  RELAY ON  for connected LV1, LV2,  HV1, HV2 

	CString strSrc;
	switch(g_saTestCondition[m_nCalMode].nVBSrc)
	{
		case C_LV1:	strSrc = "LV1"; dwRet = MM.Apply(LV1);	 	break;	
		case C_LV2: strSrc = "LV2"; dwRet = MM.Apply(LV2);	 	break;	
		case C_HV1: strSrc = "HV1"; dwRet = MM.Apply(HV1);	 	break;	
		case C_HV2: strSrc = "HV2"; dwRet = MM.Apply(HV2);	 	break;	
			break;
	}
	strTemp.Format("Meas_VbRelayOn(): MM.Apply(%s) %s\n", strSrc, (dwRet == FALSE)? "Error" : "OK");
	if (dwRet == FALSE) {	
		strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
		ERR.Set(ERR.Get(), strTemp2); 
		ErrMsg(MB_OK, FALSE); ERR.Reset();  	//  bTraceOn=FALSE, ������ Trace ���� �޽����ڽ��� ���
	}		
	MyTrace(PRT_LEVEL1, "%s\n", strTemp);

	return dwRet;
}

// VB, VC Point All Reset
BOOL CCalibDialog::Meas_VB_VC_PointAllReset() 
{
	UINT 	dwRet = TRUE;
	CString strTemp("");
	CString strTemp2("");

	// VB Point All Reset
	dwRet = MM.RstPointAll(PT_VB);
	strTemp.Format("Meas_VB_VC_PointAllReset(): MM.RstPointAll(PT_VB) %s.\n",
			(dwRet == FALSE)? "Error" : "OK");
	if (dwRet == FALSE) {	
		strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
		ERR.Set(ERR.Get(), strTemp2); 
		ErrMsg(MB_OK, FALSE); ERR.Reset();  	//  bTraceOn=FALSE, ������ Trace ���� �޽����ڽ��� ���
	}		
	MyTrace(PRT_LEVEL1, "%s\n", strTemp);

	// VC Point All Reset
	dwRet = MM.RstPointAll (PT_VC);		// VC point All Reset
	strTemp.Format("Meas_VB_VC_PointAllReset(): MM.RstPointAll(PT_VC) %s.\n",
			(dwRet == FALSE)? "Error" : "OK");
	if (dwRet == FALSE) {	
		strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
		ERR.Set(ERR.Get(), strTemp2); 
		ErrMsg(MB_OK, FALSE); ERR.Reset();  	
	}		
	MyTrace(PRT_LEVEL1, "%s\n", strTemp);
	
	return dwRet;	
}


#define	ERR_SET_VBVCPIN_PIN0	2
// ACE400 OnA_Cal_JIG_Read_151211() 4W R�� ���� �κ� ����
BOOL CCalibDialog::Meas_CalcR_4W() 
{
	UINT 	dwRet = TRUE;
	CString strTemp("");
	CString strTemp2("");


	if (g_sSystem.m_nRJigFileNum == -1)		// m_nRJigFileNum�� �ʱ�ȭ���� ���� ���.
	{
		strTemp.Format("Meas_CalcR_4W(): Not Initialized m_nRJigFileNum=%d", g_sSystem.m_nRJigFileNum);
		ERR.Set(SW_NO_INIT_VAR, strTemp);
		ErrMsg(); ERR.Reset();
		return FALSE;
	}

	// RJigFileNum: RJigData s100���� ���(1), �ƴѰ�� (0)
	sRJIG_DATA	*p_saRJigData;
	p_saRJigData = (sRJIG_DATA*)g_sSystem.m_saRJigData_4W[g_sSystem.m_nRJigFileNum];

	int		nRJigStart, nRJigEnd;
	g_sSystem.Get_RJigPinRange(m_nCalMode, nRJigStart, nRJigEnd);		// nRJigStart, nRJigEnd Reference Value

	//----------------------------------------------------------------------------------------
	//son JigData ���� (C:\ACE400\BDL\QC\RJigReadType_S100.txt)-> RJigData1[35][20]����
	//son JigPin ���� data�� ������� �����ͼ� Pin �����ϰ�  ADC R ���� read�Ͽ� R���� ���Ѵ�.
	//son �� ������ ��� JigData ������ data�� ���� �ݺ��Ͽ� �����Ѵ�. 
	//----------------------------------------------------------------------------------------

	//--------------------------------------------
	//  ��� JigData�� ���� VC, I �� ����
	
	// 2018.02.13 dumpNetCount changed for 10. by kim young pil
	// 2019.02.27 �迵�� ���� confirm
	//            2W , 4W ��� 10�� Net�� *5ȸ �����ؼ� 50���� ������ �ٽ� 0���� ����.
	int RJigIdx = 0, RJigIdx2 = 0, dumpNetCount = 10, dumpRepeat = 5;	
	for (RJigIdx2=(nRJigStart -dumpNetCount); RJigIdx2 <= nRJigEnd; RJigIdx2++)	// ACE400 i, (nStart+nRCount)�� �ش�
	{
		// 2018.02.27 �迵�ʺ��� confirm.
		// 4W ù ��ȸ(dumpNetCount)�� �����ϰ� ������. �ϳ��� ������ �����ϸ� ��ȭ�� ���������� �߻��ϹǷ�
		// dumpNetCount�� 10�̸� 0~9������ pinNet�� Read�ϰ� ��������, �ٽ� 0���� ���ư��� ����� ������ ����. 
		if (RJigIdx2 < nRJigStart)
			RJigIdx = RJigIdx2 + dumpNetCount;
		else
		{
			// 5ȸ �ݺ��̸� RJigIdx�� �ٽ� (nRJigStart - dumpNetCount)�� ����� �� 4���� �ϸ� �ȴ�.
			if (RJigIdx2 == nRJigStart && (dumpRepeat-1) > 0)
			{ 
				// RJigIdx2�� nRJigStart(��������)�� �Ǵ� ���� -(dumpNetCount+1)�� �ϰ� for�� �������� �̵��ϸ�
				// RJigIdx2++�� �Ǿ RJigIdex2�� (nRJigStart - dumpNetCount) ���·� �ٽ� ���ư���.
				RJigIdx2 = nRJigStart - (dumpNetCount +1);
				dumpRepeat--;
				continue;
			}

			RJigIdx = RJigIdx2;
		}

		// 'Stop Measure' �� ������ ��� �ߴ��ϰ� ����
		if ( m_bFlagStopMeas == TRUE)	
		{
			MyTrace(PRT_LEVEL1, "Meas_CalcR_4W() Stop!: RJigIdx=%d, 'Stop Measure' Detected.\n", RJigIdx);
			return FALSE;
		}

		/*
		// OnButtonLoadRef() �κп��� g_sSystem.InitCalData()�� ��°�� �ʱ�ȭ�ϹǷ� ���⿡���� �����Ѵ�. 2017.11.29
		// ���� ���Ŀ� LogCal data ��½ÿ� �Ʒ� �ʱ�ȭ ������ ����� �� �� �� ����.
		
		//R���� �����ϱ� ���� �ʱ�ȭ�� �д�.
		dwRet = g_sSystem.InitCal_RJigData(m_nCalMode, RJigIdx);
		if (dwRet == FALSE) {	
			strTemp.Format("%s\nMeas_CalcR_4W(): g_sSystem.InitCal_RJigData() Error.\n", ERR.m_strState);
			ERR.Set(ERR.Get(), strTemp); 
			ErrMsg(); ERR.Reset();
		}
		*/

		//-------------------
		// 4W Pin ����.
		
		// Pin �����ϱ� ���� ������ ������ pin�� Reset. 
		if (Meas_VB_VC_PointAllReset() == FALSE)
		{
			MyTrace(PRT_LEVEL1, "Meas_CalcR_4W(): Meas_VB_VC_PointAllReset() %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");
			continue;
		}

		// 4���� 4W Pin ����.
		
		// 2018.02.01 �迵�� ���� ��û���� ���� ���� ����
		// 				CPOINT RY - 1
		// 				BPOINT RY - 2
		// 				CPOINT TR - 3
		// 				BPOINT TR - 4

		// CPOINT RY - 1
		dwRet = Meas_CalcR_Set_VbVcPin(CPOINT, RY, p_saRJigData[RJigIdx].nVcRyPin);
		strTemp.Format("Meas_CalcR_4W(): RJigIdx=%d, Meas_CalcR_Set_VbVcPin(CPOINT, RY, pinNo_%d) %s.\n",
				RJigIdx, p_saRJigData[RJigIdx].nVcRyPin,
				(dwRet == FALSE)? "Error" : "OK");
		if (dwRet == FALSE) {
			strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
			ERR.Set(ERR.Get(), strTemp2); 
			ErrMsg(); ERR.Reset();
			continue;
		}
		MyTrace(PRT_LEVEL1, "%s\n", strTemp);

		// BPOINT RY - 2
		dwRet = Meas_CalcR_Set_VbVcPin(BPOINT, RY, p_saRJigData[RJigIdx].nVbRyPin);
		strTemp.Format("Meas_CalcR_4W(): RJigIdx=%d, Meas_CalcR_Set_VbVcPin(BPOINT, RY, pinNo_%d) %s.\n",
				RJigIdx, p_saRJigData[RJigIdx].nVbRyPin,
				(dwRet == FALSE)? "Error" : "OK");
		if (dwRet == FALSE) {
			strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
			ERR.Set(ERR.Get(), strTemp2); 
			ErrMsg(); ERR.Reset();
			continue;
		}
		MyTrace(PRT_LEVEL1, "%s\n", strTemp);


		// CPOINT TR - 3
		dwRet = Meas_CalcR_Set_VbVcPin(CPOINT, TR, p_saRJigData[RJigIdx].nVcTrPin);
		strTemp.Format("Meas_CalcR_4W(): RJigIdx=%d, Meas_CalcR_Set_VbVcPin(CPOINT, TR, pinNo_%d) %s.\n",
				RJigIdx, p_saRJigData[RJigIdx].nVcTrPin,
				(dwRet == FALSE)? "Error" : "OK");
		if (dwRet == FALSE) {
			strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
			ERR.Set(ERR.Get(), strTemp2); 
			ErrMsg(); ERR.Reset();
			continue;
		}
		MyTrace(PRT_LEVEL1, "%s\n", strTemp);

		// BPOINT TR - 4
		dwRet = Meas_CalcR_Set_VbVcPin(BPOINT, TR, p_saRJigData[RJigIdx].nVbTrPin);
		strTemp.Format("Meas_CalcR_4W(): RJigIdx=%d, Meas_CalcR_Set_VbVcPin(BPOINT, TR, pinNo_%d) %s.\n",
				RJigIdx, p_saRJigData[RJigIdx].nVbTrPin,
				(dwRet == FALSE)? "Error" : "OK"); 
		if (dwRet == FALSE) {
			strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
			ERR.Set(ERR.Get(), strTemp2); 
			ErrMsg(); ERR.Reset();
			continue;
		}
		MyTrace(PRT_LEVEL1, "%s\n", strTemp);


		// �ɼ��� ���� TestCondition�� ������  delay ����
		Delay(g_saTestCondition[m_nCalMode].nDelay, usec);


		//---------------------------------
		// VC, I������,  R, DiffRate ��� 
		//
		// 1. VC, I �� ���� (m_nMeasCount��ŭ �ݺ�), maxMeasR, minMeasR �� ȹ��
		// 2. avg ADC VC, avg ADC I ��� 
		// 3.1 ADC I�� mA(IR4, IR5�� uA)�� ��ȯ
		// 3.2 ADC VC�� Volt�� ��ȯ  
		// 3.3 R ���
		// 4. DiffRate ��� 
		Meas_CalcR_CalData(RJigIdx);		// 2W, 4W ���� ���
	}


	// 2018.02.05 Pin ���̽����� ��� ���Ŀ� UI�� ������� ����ϴ� ���� ����ӵ��� �ʹ� ������ ��.
	//            ���� �Ϸ��Ŀ� �Ѳ����� Display�ϵ��� ����.
	DoEvents();
	for (RJigIdx=nRJigStart; RJigIdx <= nRJigEnd; RJigIdx++)	// ACE400 i, (nStart+nRCount)�� �ش�
	{
		//---------------------------
		// ����� ȭ�� Display
		Meas_CalcR_Display(RJigIdx);

	}

	
	return dwRet;	
}

// ACE400 OnA_Cal_JIG_Read_151211() 2W R�� ���� �κ� ����
BOOL CCalibDialog::Meas_CalcR_2W() 
{
	UINT 	dwRet = TRUE;
	CString strTemp("");
	CString strTemp2("");

	if (g_sSystem.m_nRJigFileNum == -1)		// m_nRJigFileNum�� �ʱ�ȭ���� ���� ��� üũ
	{
		strTemp.Format("Meas_CalcR_2W(): Not Initialized m_nRJigFileNum=%d", g_sSystem.m_nRJigFileNum);
		ERR.Set(SW_NO_INIT_VAR, strTemp);
		ErrMsg(); ERR.Reset();
		return FALSE;
	}

	// RJigData s100���� ���(1), �ƴѰ�� (0)
	sRJIG_DATA	*p_saRJigData;
	p_saRJigData = (sRJIG_DATA*)g_sSystem.m_saRJigData_2W[g_sSystem.m_nRJigFileNum];

	int		nRJigStart, nRJigEnd;
	g_sSystem.Get_RJigPinRange(m_nCalMode, nRJigStart, nRJigEnd);		// nRJigStart, nRJigEnd Reference Value

	//----------------------------------------------------------------------------------------
	//son JigData ���� (C:\ACE400\BDL\QC\RJigReadType_S100.txt)-> RJigData1[35][20]����
	//son JigPin ���� data�� ������� �����ͼ� Pin �����ϰ�  ADC R ���� read�Ͽ� R���� ���Ѵ�.
	//son �� ������ ��� JigData ������ data�� ���� �ݺ��Ͽ� �����Ѵ�. 
	//----------------------------------------------------------------------------------------
	
	//int RJigIdx = 0;
	//for (RJigIdx=nRJigStart; RJigIdx <= nRJigEnd; RJigIdx++)	// ACE400 i�� �ش�
	
	// 2018.02.26 dumpNetCount�� 2W�� ��� 5ȸ ���� ������ ���� �߰���.  
	//            �̻������� �ڵ� ModeSer13.cpp 1390���� ����.
	// 2019.02.27 �迵�� ���� confirm
	//            2W�� 4W�� ���������� 10�� Net�� *5ȸ �����ؼ� (50���� ������.)
	int RJigIdx = 0, RJigIdx2 = 0, dumpNetCount = 10, dumpRepeat = 5;	
	for (RJigIdx2=(nRJigStart -dumpNetCount); RJigIdx2 <= nRJigEnd; RJigIdx2++)	// ACE400 i, (nStart+nRCount)�� �ش�
	{
		// 2018.02.27 �迵�ʺ��� confirm.
		// 2W�� ù ��ȸ(dumpNetCount)�� �����ϰ� ������. �ϳ��� ������ �����ϸ� ��ȭ�� ���������� �߻��ϹǷ�
		// dumpNetCount�� 10�̸� 0~9������ pinNet�� Read�ϰ� ��������, �ٽ� 0���� ���ư��� ����� ������ ����. 
		if (RJigIdx2 < nRJigStart)
			RJigIdx = RJigIdx2 + dumpNetCount;
		else
		{
			// 5ȸ �ݺ��̸� RJigIdx�� �ٽ� (nRJigStart - dumpNetCount)�� ����� �� 4���� �ϸ� �ȴ�.
			if (RJigIdx2 == nRJigStart && (dumpRepeat-1) > 0)
			{ 
				// RJigIdx2�� nRJigStart(��������)�� �Ǵ� ���� -(dumpNetCount+1)�� �ϰ� for�� �������� �̵��ϸ�
				// RJigIdx2++�� �Ǿ RJigIdex2�� (nRJigStart - dumpNetCount) ���·� �ٽ� ���ư���.
				RJigIdx2 = nRJigStart - (dumpNetCount +1);
				dumpRepeat--;
				continue;
			}

			RJigIdx = RJigIdx2;
		}

		// 'Stop Measure' �� ������ ��� �ߴ��ϰ� ����
		if ( m_bFlagStopMeas == TRUE)	
		{
			MyTrace(PRT_LEVEL1, "Meas_CalcR_2W() Stop!: RJigIdx=%d, 'Stop Measure' Detected.\n", RJigIdx);
			//ERR.Set(NO_ERR, strTemp); 
			//ErrMsg(); ERR.Reset();
			return FALSE;
		}

		/*
		// OnButtonLoadRef() �κп��� g_sSystem.InitCalData()�� ��°�� �ʱ�ȭ�ϹǷ� ���⿡���� �����Ѵ�. 2017.11.29
		// ���� ���Ŀ� LogCal data ��½ÿ� �Ʒ� �ʱ�ȭ ������ ����� �� �� �� ����.
		
		//R���� �����ϱ� ���� �ʱ�ȭ�� �д�.
		dwRet = g_sSystem.InitCal_RJigData(m_nCalMode, RJigIdx);
		if (dwRet == FALSE) {	
			strTemp.Format("%s\n Meas_CalcR_2W(): g_sSystem.InitCal_RJigData() Error.\n", ERR.m_strState);
			ERR.Set(ERR.Get(), strTemp); 
			ErrMsg(); ERR.Reset();
		}
		*/

		//-------------------
		// 2W Pin ����.
		
		// Pin �����ϱ� ���� ������ ������ pin ���� Reset. 
		if (Meas_VB_VC_PointAllReset() == FALSE)
		{
			MyTrace(PRT_LEVEL1, "Meas_CalcR_2W(): Meas_VB_VC_PointAllReset() %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");
			continue;
		}

		// 2018.02.26 Open Hi Voltage ���� �ʱ� R���� �������� �����Ǵ� ������ ��� ���� 
		//            Open Hi Voltage�� Pin ���� ������ �߰��� 300 usec ����� �Ѵ�.  
		//            ModeSer13.cpp 1040 ���� �̻������� �ڵ忡�� ������.
		if( g_saTestCondition[m_nCalMode].nVSet >= 100 ){//sylee150630  // 100V �̻��� ��
			Delay(300, usec);
		}


		// 2018.02.01 �迵�� ���� ��û���� ���� ���� ����
		// 				CPOINT RY - 1
		// 				BPOINT RY - 2
		// 				CPOINT TR - 3
		// 				BPOINT TR - 4

		// CPOINT TR - 3
		dwRet = Meas_CalcR_Set_VbVcPin(CPOINT, TR, p_saRJigData[RJigIdx].nVcTrPin);
		strTemp.Format("Meas_CalcR_2W(): RJigIdx=%d, Meas_CalcR_Set_VbVcPin(CPOINT, TR, pinNo_%d) %s.\n",
				RJigIdx, p_saRJigData[RJigIdx].nVcTrPin,
				(dwRet == FALSE)? "Error" : "OK");
		if (dwRet == FALSE) {
			strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
			ERR.Set(ERR.Get(), strTemp2); 
			ErrMsg(); ERR.Reset();
			continue;
		}
		MyTrace(PRT_LEVEL1, "%s\n", strTemp);

		
		// BPOINT TR - 4
		dwRet = Meas_CalcR_Set_VbVcPin(BPOINT, TR, p_saRJigData[RJigIdx].nVbTrPin);
		strTemp.Format("Meas_CalcR_2W(): RJigIdx=%d, Meas_CalcR_Set_VbVcPin(BPOINT, TR, pinNo_%d) %s.\n",
				RJigIdx, p_saRJigData[RJigIdx].nVbTrPin,
				(dwRet == FALSE)? "Error" : "OK");
		if (dwRet == FALSE) {
			strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
			ERR.Set(ERR.Get(), strTemp2); 
			ErrMsg(); ERR.Reset();
			continue;
		}
		MyTrace(PRT_LEVEL1, "%s\n", strTemp);


		// �ɼ��� ���� TestCondition�� ������  delay ����
		Delay(g_saTestCondition[m_nCalMode].nDelay, usec);


		//---------------------------------
		// VC, I������,  R, DiffRate ��� 
		// 1. VC, I �� ���� (m_nMeasCount��ŭ �ݺ�)
		// 2. avg ADC VC, avg ADC I, maxMeasR, minMeasR �� ȹ�� 
		// 3. ADC I�� mA(IR4, IR5�� uA)�� ��ȯ
		// 4. ADC VC�� Volt�� ��ȯ  
		// 5. R ���
		// 6. DiffRate ��� 
	    // 7. Check Invalid
		Meas_CalcR_CalData(RJigIdx);

	}

	// 2018.02.05 Pin ���̽����� ��� ���Ŀ� UI�� ������� ����ϴ� ���� ����ӵ��� �ʹ� ������ ��.
	//            ���� �Ϸ��Ŀ� �Ѳ����� Display�ϵ��� ����.
	DoEvents();
	for (RJigIdx=nRJigStart; RJigIdx <= nRJigEnd; RJigIdx++)	// ACE400 i�� �ش�
	{

		//---------------------------
		// ����� ȭ�� Display
		Meas_CalcR_Display(RJigIdx);
	}


	return dwRet;	
}


// ACE400 CModeSub71::AOnPinSet1(int Type, int No, int nMod) �����ؼ� �ۼ�.
//
// VbVcType : BPOINT, CPOINT
// gateType	: TR, RY
// pinNo    : 0~ 32767
BOOL CCalibDialog::Meas_CalcR_Set_VbVcPin(int VbVcType, int gateType, int pinNo) 
{
	CString strTemp("");
	CString strTemp2("");

	

	// VbVcType üũ
	if (VbVcType != BPOINT  && VbVcType != CPOINT)
	{
		strTemp.Format("Meas_CalcR_Set_VbVcPin: Invalid VbVcType=%d", VbVcType);
		ERR.Set(INVALID_INPUT, strTemp);
		return FALSE;
	}

	// gateType üũ
	if (gateType != TR  && gateType != RY)
	{
		strTemp.Format("Meas_CalcR_Set_VbVcPin: Invalid gateType=%d", gateType);
		ERR.Set(INVALID_INPUT, strTemp);
		return FALSE;
	}
	
	// Pin Range üũ
	if (pinNo < 0  || pinNo >= MAX_MODEL_PIN)
	{
		strTemp.Format("Meas_CalcR_Set_VbVcPin: pinNo=%d, Range(0<= pinNo < %d) Over!", pinNo, MAX_MODEL_PIN);
		ERR.Set(RANGE_OVER, strTemp);
		return FALSE;
	}

	// 0�� ���� ���Ͽ� ������ �� ������ ������ 0���� Set�� ���� ����. 
	// MM.Set(.., pinNo-1, ..)�ϸ� -1�� Set�ϰ� ��
	if (pinNo == 0)
//		return ERR_SET_VBVCPIN_PIN0;	// error�� �ƴϹǷ� 2�� �����ؼ� ȣ���� �Լ����� R�� ������ skipó�� 
//		                             ==> �׳� ���� �����ϴ� ������ ������.  ���⼺ ����
	{
		strTemp.Format("Meas_CalcR_Set_VbVcPin: pinNo=0, Skip! ");
		ERR.Set(INVALID_INPUT, strTemp);
		return FALSE;
	}
	

	//---------------------------------
	// RJig Pin# -> HW Switch Pin#
	
	int nPin = -1;
//	if (g_sModel.m_bIsConExist == FALSE)	// .CON ������ �������� �ʾƵ� nPinMap�� ����ϱ���ϰ�  commentó����.
//   	nPin = pinNo -1;	
//  else
	nPin=g_sModel.nPinMap_SWT[pinNo-1];  

	// Pin Range üũ
	if (nPin < 0  || nPin >= MAX_MODEL_PIN)
	{
		strTemp.Format("Meas_CalcR_Set_VbVcPin: pinNo=%d -> nPin=%d,  Range(0<= nPin < %d) Over!", 
				pinNo, nPin, MAX_MODEL_PIN);
		ERR.Set(RANGE_OVER, strTemp);
		return FALSE;
	}

	//---------------------------------
	// HW ���� 
	
	//UINT dwRet = MM.Set(VbVcType, pinNo-1, gateType);
	UINT dwRet = MM.Set(VbVcType, nPin, gateType);
	strTemp.Format("Meas_CalcR_Set_VbVcPin(): RJig pinNo-1=%d, MM.Set(%s, %s, nPin_%d) %s.\n",
			pinNo-1, 
			(VbVcType == BPOINT)? "BPOINT":
			(VbVcType == CPOINT)? "CPOINT": "Invalid VbVcType",
			(gateType == TR)? "TR":
			(gateType == RY)? "RY": "Invalid gateType", 
			nPin ,
			(dwRet == FALSE)? "Error" : "OK" 	);
	if (dwRet == FALSE) {	
		strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
		ERR.Set(ERR.Get(), strTemp2); 
		return FALSE;
	}		
	MyTrace(PRT_LEVEL2, "%s\n", strTemp);

	return TRUE;
}




//---------------------------------
// VC, I������,  R, DiffRate ��� 
// 1. VC, I �� ���� (m_nMeasCount��ŭ �ݺ�) , maxMeasR, minMeasR �� ȹ�� 
// 2. avg ADC VC, avg ADC I ���
// 3.1 ADC I�� mA(IR4, IR5�� uA)�� ��ȯ
// 3.2 ADC VC�� Volt�� ��ȯ  
// 3.3 R ���
// 4. DiffRate ��� 
// 5. Check Invalid
#define DIFF_RATE_LIMIT		5
void CCalibDialog::Meas_CalcR_CalData(int RJigIdx) 
{
	UINT 	dwRet = TRUE;

	//----------------------------------------------------
	// 1. VC, I �� ����  (m_nMeasCount��ŭ �ݺ�) , maxMeasR, minMeasR �� ȹ��

	double 	dSumADI= 0, dAvgADI= 0;		
	double 	dSumADVC= 0, dAvgADVC= 0;		
	double  dMinMeasR= 0, dMaxMeasR=0;
	UINT measIdx;
	for (measIdx=0; measIdx < m_nMeasCount; measIdx++)		// ACE400 j, nCount�� �ش�
	{

		// VC, I �� ���� & ���� R �� ���
		dwRet = Meas_CalcR_AdRead_VI(m_nCalMode, RJigIdx, measIdx);
		//dwRet = Meas_CalcR_AdRead_VI_Median(m_nCalMode, RJigIdx, measIdx);
		//dwRet = Meas_CalcR_AdRead_VI_CMAvg(m_nCalMode, RJigIdx, measIdx);
		if (dwRet == FALSE) {
			ErrMsg(); ERR.Reset();
			continue;
		}

		dSumADI  += g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dAdI;
		dSumADVC += g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dAdVc;
	}	

	//----------------------------------------------
	// 1.1 Meas���� R ���   
	// ���� R ���� AdI, adVC ������ ����� �� ���� VC/I �ؼ� ����Ѵ�. 
	// ���� R �ܰ迡�� �߿��� ���� AdI, AdVC���̸�, 
	// ���� I, VC, R ����  MaxR, MinR���� ã�� csv ������ ����ϱ� ���� �ΰ������� �߰���.
	//
	
	// �ݹ� ������ AdI, AdVc ���� �Է��ؼ� I, Vc, R ���� ����� ����.
	for (measIdx=0; measIdx < m_nMeasCount; measIdx++)		// ACE400 j, nCount�� �ش�
	{
		Meas_CalcR_makeR(m_nCalMode, 
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dAdI, 	// Input
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dAdVc,	// Input
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dI, 	// Output (Ref�Ƿ� pointer�� �� �ѱ�)
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dVc, 	// Output (Ref)
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dR);	// Output (Ref)
		MyTrace(PRT_LEVEL1, "..AdRead_VI(CalMode=%d,RJig=%d,Meas=%d)=> %s\n", 
						m_nCalMode, RJigIdx, measIdx, ERR.m_strState);
		ERR.Reset();
		
		if (measIdx == 0)
		{	// ����R min, max �ʱ�ȭ
			dMaxMeasR = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dR;	
			dMinMeasR = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dR;	
		}
		else // ����R min, max ���� ��󳽴�.
		{
			// �ִ밪���� ���簪�� ũ�� �ִ밪 ����
			if (dMaxMeasR < g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dR)		
				dMaxMeasR = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dR;
			// �ּҰ����� ���簪�� ������ �ּҰ� ����
			if (dMinMeasR > g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dR)		
				dMinMeasR = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dR;
		}
	}

	//-------------------------------
	// 2. avg ADC VC, avg ADC I ���
	
	// Avg
	dAvgADI  = dSumADI  / (double)m_nMeasCount;			// m_nMeasCount�� 0�� �� �� ����.
	dAvgADVC = dSumADVC / (double)m_nMeasCount;
	g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasAdI_Avg = dAvgADI;
	g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasAdVc_Avg = dAvgADVC;


	//-------------------------------
	// 3. ���� R ��� 
	double dAvgR = 9999999999.0;
	
	// ���� Average  AdI, AdVc ���� �Է¹޾Ƽ�  I, Vc, ���������� R ���� ����� ����.
	Meas_CalcR_makeR(m_nCalMode, 
					dAvgADI, dAvgADVC, 		// Input
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasI_Avg, 	// Output (Ref)
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasVc_Avg,	// Output (Ref)
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_Avg);	// Output (Ref)
	dAvgR = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_Avg;
	MyTrace(PRT_LEVEL1, "Meas_CalcR_CalData(RJig=%d)=> %s\n", RJigIdx, ERR.m_strState);
	ERR.Reset();


	//---------------------------
	// 4. DiffRate ���

	// MinR, MaxR    => // DiffRate ������. ���� R�� ���� �������.
	g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_Max = dMaxMeasR;
	g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_Min = dMinMeasR;

	// Meas DiffRate ��� (Use ������ )  
	// dMeasR_MaxDiffRate = (Max - Avg)/Avg : + ������  .���� +5%���� ũ�� ���� ǥ��(nValid=False). TBD
	// dMeasR_MinDiffRate = (Min - Avg)/Avg : - ������  .���� -5%���� ������ ���� ǥ��(nValid=False). TBD 
	double 	dMeasR_MaxDiffRate = 0, dMeasR_MinDiffRate = 0;
	if (dAvgR != 0)	 // check divide by zero
	{
		dMeasR_MaxDiffRate = ((dMaxMeasR - dAvgR) / dAvgR) * 100;		// % ��
		dMeasR_MinDiffRate = ((dMinMeasR - dAvgR) / dAvgR) * 100;		
	}
	g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_MaxDiffRate = dMeasR_MaxDiffRate;
	g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_MinDiffRate = dMeasR_MinDiffRate;


	//---------------------------
	// 5. Check Invalid

	// 4W�� mode���� üũ�ϴ� range�� ������ �����Ƿ� range �ܿ����� ������ ������ ����ó���� �ʿ䰡 ����.
	g_sSystem.m_saCalData[m_nCalMode][RJigIdx].nValid = TRUE;
	if (g_saTestCondition[m_nCalMode].nTest == TEST_4W)
	{
		// 		 n4wMinR	n4wMaxR		
		// 171   1			80			m_na4W_Range[1]  ex) 80      1~80     
		// 172   81			800			m_na4W_Range[2]  ex) 800     81~800   
		// 173   801		5000		m_na4W_Range[3]  ex) 5000    801~5000 
		// 174   5001		50000		m_na4W_Range[4]  ex) 50000   5001 ~ 50000   
		// 175   50001		500000		m_na4W_Range[5]  ex) 500000  50001 ~ 500000 

		int n4wMinR, n4wMaxR;
		if (m_nCalMode == 170)
			n4wMinR = 1;
		else
			n4wMinR = g_sSystem.m_na4W_Range[m_nCalMode - 171] + 1;
		n4wMaxR = g_sSystem.m_na4W_Range[m_nCalMode - 170];

		// ���� OrgR�� �ش����� range�� �´� �����̶�� Invalid üũ�� �Ѵ�.
		if ((n4wMinR <= g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dOrgR)
		    && (g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dOrgR <= n4wMaxR)) 
			Meas_CalcR_CheckInvalid(m_nCalMode, RJigIdx);
	}
	else
		Meas_CalcR_CheckInvalid(m_nCalMode, RJigIdx);
	

	MyTrace(PRT_LEVEL1, "Meas_CalcR_CalData(RJigIdx=%d): dMeasR_Max=%.3f, MaxDiffRate=%.3f(%s), dMeasR_Min=%.3f, MinDiffRate=%.3f(%s), Valid = %s \n",
			RJigIdx,
			g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_Max, 
			g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_MaxDiffRate, 	
			(g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_MaxDiffRate > DIFF_RATE_LIMIT)? "Bad": "Good",	// > 5%
			g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_Min, 
			g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_MinDiffRate,
			((-g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_MinDiffRate) > DIFF_RATE_LIMIT)? "Bad": "Good",// < -5%
			(g_sSystem.m_saCalData[m_nCalMode][RJigIdx].nValid == FALSE)? "Bad": "Good");


}


//---------------------------
// VC, I �� ���� & R �� ���
//---------------------------


//UINT 	ga_dwAdIVal[MAX_MEAS_COUNT][CAL_AD_BUF_SIZE];		// AD I raw data 
//UINT 	ga_dwAdVCVal[MAX_MEAS_COUNT][CAL_AD_BUF_SIZE];		// AD VC raw data 
BOOL CCalibDialog::Meas_CalcR_AdRead_VI(int nCalMode, int nRJigId, int nMeasId) 
{

	char	strTemp[500] = "";
	char	strTemp2[500] = "";
	UINT 	dwVal[CAL_AD_BUF_SIZE], dwMinI=0, dwMaxI=0, dwSumI=0, dwMinI2= 0, dwMaxI2= 0;
	UINT 	dwMinVC=0, dwMaxVC=0, dwSumVC=0, dwMinVC2=0, dwMaxVC2=0;

	int nAdCount = g_sSystem.Get_nAdCount(nCalMode);
	for (int adCntIdx=0; adCntIdx < nAdCount; adCntIdx++)		
	{

		//-------------------
		// I��, VC�� ����

		dwVal[adCntIdx] = MM.Get(VI, TRUE);		
		UINT nAdIVal  = dwVal[adCntIdx] & 0xffff;
		UINT nAdVCVal = dwVal[adCntIdx] >>16 & 0xffff;

#ifdef  __AD_RAW_DATA__
		// raw data ����.  (��¿�)
		g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasRawData[nMeasId][adCntIdx].nAdI = nAdIVal;
		g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasRawData[nMeasId][adCntIdx].nAdVc = nAdVCVal;
#endif

		// 2018.02.12  AdRead �ӵ��� �ʹ� ������ �Ʒ� ����Ʈ ���� MySnprintf�� ����. print off�̸� string �������� �ʰ� ���� 
		MySnprintf(PRT_LEVEL2, strTemp, sizeof(strTemp),
				"..AdRead_VI(CalMode=%d,RJig=%d,Meas=%d): adCntIdx=%d, MM.Get(VI) %s. dwVal[%d]=0x%08x nAdIVal=0x%04x(%d) nAdVCVal=0x%04x(%d)\n", 
				nCalMode, nRJigId, nMeasId, adCntIdx,
				(dwVal[adCntIdx] == (UINT)(-1))? "Error" : "OK", adCntIdx, dwVal[adCntIdx], nAdIVal, nAdIVal, nAdVCVal, nAdVCVal);
		if (dwVal[adCntIdx] == (UINT)(-1) ) {
			MySnprintf(PRT_LEVEL2, strTemp2, sizeof(strTemp2), 
					"%s\n%s\n", ERR.m_strState, strTemp);
			ERR.Set(ERR.Get(), strTemp2); 
			return FALSE;
		}
		MyTrace(PRT_LEVEL2, "%s\n", strTemp);


		if (adCntIdx == 0)
		{
			dwMaxI = dwMaxI2 = nAdIVal;	
			dwMinI = dwMinI2 = nAdIVal;	

			dwMaxVC = dwMaxVC2 = nAdVCVal; 	
			dwMinVC = dwMinVC2 = nAdVCVal;
		}
		else //min, max ���� ��󳽴�.
		{
			// I
			if (dwMaxI < nAdIVal)		// �ִ밪���� ���簪�� ũ�� �ִ밪 ����
			{
				dwMaxI2 = dwMaxI;
				dwMaxI = nAdIVal;
			}
			if (dwMinI > nAdIVal)		// �ּҰ����� ���簪�� ������ �ּҰ� ����
				
				dwMinI = nAdIVal;

			// VC
			if (dwMaxVC < nAdVCVal)		// �ִ밪���� ���簪�� ũ�� �ִ밪 ����
				dwMaxVC = nAdVCVal;
			if (dwMinVC > nAdVCVal)		// �ּҰ����� ���簪�� ������ �ּҰ� ����
				dwMinVC = nAdVCVal;
		}
		dwSumI += nAdIVal;	
		dwSumVC += nAdVCVal;	

	}

	// Avg ���
	double dAdI=0, dAdVc=0;
	if (nAdCount > 3) {
		dAdI  = (dwSumI - dwMaxI - dwMinI) / (nAdCount -2);  // min, max �����ϰ� (adCount-2)ȸ�� ���� ��հ�
		dAdVc = (dwSumVC - dwMaxVC - dwMinVC) / (nAdCount -2);  
	}
	else {
		dAdI = dwSumI / nAdCount;  // min max ������� adCount�� ��հ�
		dAdVc = dwSumVC / nAdCount;  
	}

	g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasData[nMeasId].dAdI = dAdI;
	g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasData[nMeasId].dAdVc = dAdVc;

	MyTrace(PRT_LEVEL1, "..AdRead_VI(CalMode=%d,RJig=%d,Meas=%d): dAdI=%.3f dAdVc=%.3f Ad Averaged.\n", 
				nCalMode, nRJigId, nMeasId,
				g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasData[nMeasId].dAdI,
				g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasData[nMeasId].dAdVc);

	return TRUE;
}

// 2018.02.21  ��հ��� �ƴ϶� �߾Ӱ�(Median)���� AdRead���� ��ǥ���� �����Ѵ�.
BOOL CCalibDialog::Meas_CalcR_AdRead_VI_Median(int nCalMode, int nRJigId, int nMeasId) 
{
	char	strTemp[500] = "";
	char	strTemp2[500] = "";

	UINT 	dwVal[CAL_AD_BUF_SIZE]; 
	UINT 	dwAdIVal[CAL_AD_BUF_SIZE];
	UINT 	dwAdVCVal[CAL_AD_BUF_SIZE];
	UINT 	dwMinVC=0, dwMaxVC=0, dwSumVC=0, dwMinVC2=0, dwMaxVC2=0;

	::ZeroMemory(&dwVal, sizeof(dwVal));
	::ZeroMemory(&dwAdIVal, sizeof(dwAdIVal));
	::ZeroMemory(&dwAdVCVal, sizeof(dwAdVCVal));
	
	int nAdCount = g_sSystem.Get_nAdCount(nCalMode);
	for (int adCntIdx=0; adCntIdx < nAdCount; adCntIdx++)		
	{

		//-------------------
		// I��, VC�� ����

		dwVal[adCntIdx] = MM.Get(VI, TRUE);		
		dwAdIVal[adCntIdx]  = dwVal[adCntIdx] & 0xffff;
		dwAdVCVal[adCntIdx] = dwVal[adCntIdx] >>16 & 0xffff;

#ifdef  __AD_RAW_DATA__
		// raw data ����.  (��¿�)
		g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasRawData[nMeasId][adCntIdx].nAdI = dwAdIVal[adCntIdx];
		g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasRawData[nMeasId][adCntIdx].nAdVc = dwAdVCVal[adCntIdx];
#endif

		// 2018.02.12  AdRead �ӵ��� �ʹ� ������ �Ʒ� ����Ʈ ���� MySnprintf�� ����. print off�̸� string �������� �ʰ� ���� 
		MySnprintf(PRT_LEVEL2, strTemp, sizeof(strTemp),
				"..AdRead_VI(CalMode=%d,RJig=%d,Meas=%d): adCntIdx=%d, MM.Get(VI) %s. dwVal[%d]=0x%08x AdIVal=0x%04x(%d) AdVCVal=0x%04x(%d)\n", 
				nCalMode, nRJigId, nMeasId, adCntIdx,
				(dwVal[adCntIdx] == (UINT)(-1))? "Error" : "OK", adCntIdx, dwVal[adCntIdx], 
				dwAdIVal[adCntIdx], dwAdIVal[adCntIdx], dwAdVCVal[adCntIdx], dwAdVCVal[adCntIdx]);
		if (dwVal[adCntIdx] == (UINT)(-1) ) {
			MySnprintf(PRT_LEVEL2, strTemp2, sizeof(strTemp2), 
					"%s\n%s\n", ERR.m_strState, strTemp);
			ERR.Set(ERR.Get(), strTemp2); 
			return FALSE;
		}
		MyTrace(PRT_LEVEL2, "%s\n", strTemp);

	}

	//---------------------------------
	// AdIVal�� AdVcVal�� Median ���  
	double dAdI=0, dAdVc=0;

	// Max 200���� �������� �ʰ� nAdCount������ŭ�� �����ؼ� ����Ѵ�.
	qsort((void *)dwAdIVal,						// Start target array
			nAdCount, 	// Array size in element
			sizeof(dwAdIVal[0]),				// Element Size in byte
			CompareDouble);						// Compare function
	//dAdI = GetMedian((int*)dwAdIVal, nAdCount);
	dAdI = GetMedian2((int*)dwAdIVal, nAdCount);

	qsort((void *)dwAdVCVal, 
			nAdCount, 	
			sizeof(dwAdVCVal[0]), 
			CompareDouble);
	//dAdVc = GetMedian((int*)dwAdVCVal, nAdCount);
	dAdVc = GetMedian2((int*)dwAdVCVal, nAdCount);

	g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasData[nMeasId].dAdI = dAdI;
	g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasData[nMeasId].dAdVc = dAdVc;


	MyTrace(PRT_LEVEL1, "..AdRead_VI_Med(CalMode=%d,RJig=%d,Meas=%d): dAdI=%.3f dAdVc=%.3f Median Ad.\n", 
				nCalMode, nRJigId, nMeasId,
				g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasData[nMeasId].dAdI,
				g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasData[nMeasId].dAdVc);


	return TRUE;
}

// 2018.02.23  �迵���̻� ��û
// ��հ��� �ƴ϶� Centered Moving Avg��(CMAvg)���� AdRead���� ��ǥ���� �����Ѵ�.
BOOL CCalibDialog::Meas_CalcR_AdRead_VI_CMAvg(int nCalMode, int nRJigId, int nMeasId) 
{
	char	strTemp[500] = "";
	char	strTemp2[500] = "";

	UINT 	dwVal[CAL_AD_BUF_SIZE]; 
	UINT 	dwAdIVal[CAL_AD_BUF_SIZE];
	UINT 	dwAdVCVal[CAL_AD_BUF_SIZE];
	double 	dAdCmIVal[CAL_AD_BUF_SIZE];
	double 	dAdCmVCVal[CAL_AD_BUF_SIZE];
	UINT 	dwMinVC=0, dwMaxVC=0, dwSumVC=0, dwMinVC2=0, dwMaxVC2=0;

	::ZeroMemory(&dwVal, sizeof(dwVal));
	::ZeroMemory(&dwAdIVal, sizeof(dwAdIVal));
	::ZeroMemory(&dwAdVCVal, sizeof(dwAdVCVal));
	::ZeroMemory(&dAdCmIVal, sizeof(dAdCmIVal));
	::ZeroMemory(&dAdCmVCVal, sizeof(dAdCmVCVal));
	

	int nAdCount = g_sSystem.Get_nAdCount(nCalMode);
	for (int adCntIdx=0; adCntIdx < nAdCount; adCntIdx++)		
	{

		//-------------------
		// I��, VC�� ����

		dwVal[adCntIdx] = MM.Get(VI, TRUE);		
		dwAdIVal[adCntIdx]  = dwVal[adCntIdx] & 0xffff;
		dwAdVCVal[adCntIdx] = dwVal[adCntIdx] >>16 & 0xffff;

#ifdef  __AD_RAW_DATA__
		// raw data ����.  (��¿�)
		g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasRawData[nMeasId][adCntIdx].nAdI = dwAdIVal[adCntIdx];
		g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasRawData[nMeasId][adCntIdx].nAdVc = dwAdVCVal[adCntIdx];
#endif

		// 2018.02.12  AdRead �ӵ��� �ʹ� ������ �Ʒ� ����Ʈ ���� MySnprintf�� ����. print off�̸� string �������� �ʰ� ���� 
		MySnprintf(PRT_LEVEL2, strTemp, sizeof(strTemp),
				"..AdRead_VI(CalMode=%d,RJig=%d,Meas=%d): adCntIdx=%d, MM.Get(VI) %s. dwVal[%d]=0x%08x AdIVal=0x%04x(%d) AdVCVal=0x%04x(%d)\n", 
				nCalMode, nRJigId, nMeasId, adCntIdx,
				(dwVal[adCntIdx] == (UINT)(-1))? "Error" : "OK", adCntIdx, dwVal[adCntIdx], 
				dwAdIVal[adCntIdx], dwAdIVal[adCntIdx], dwAdVCVal[adCntIdx], dwAdVCVal[adCntIdx]);
		if (dwVal[adCntIdx] == (UINT)(-1) ) {
			MySnprintf(PRT_LEVEL2, strTemp2, sizeof(strTemp2), 
					"%s\n%s\n", ERR.m_strState, strTemp);
			ERR.Set(ERR.Get(), strTemp2); 
			return FALSE;
		}
		MyTrace(PRT_LEVEL2, "%s\n", strTemp);

	}

	//---------------------------------------------------
	// AdIVal�� AdVcVal�� CM(Centered Moving) Avg ���  
	double dAdI=0, dAdVc=0;
	double dAdSumI = 0, dAdSumVc = 0;

	// 4���� �����ؼ� ��� 2���� ��ճ� ��(min, max ���� �߾� 2�� avg)�� ���Ѵ���(size 4�� Median ����), 
	// �װ� �ٽ� ��ճ���.  item ������ �����ؾ� �ϹǷ�  n-3ȸ ��ŭ �ݺ� (10�� item�̶�� 7ȸ loop) �Ͽ� 
	// Sum �� �Ŀ�  1/(n-3) �����Ͽ� Average�� ���.
	//
	// ex1)
	// item0   item1   item2  item3   item4   item5  item6  item7  item8  item9
	//  282     275     272    275     275     279    276    274    274   277          
	//                        citem3  citem4  citm5  citem6 citem7 citem8 citem9  
	//                         (0~3)  (1~4)   (2~5)   (3~6) (4~7)  (5~8)  (6~9)
	//                         275     275     275    275.5  275.5  275    275   <= min,max �����ϰ� center 2�� AVg
	//                                           => 7�� citem avg�� 275.1429
	// ex2)
	// item0   item1   item2  item3   item4   item5  item6  item7  item8  item9
	//  350	    349	    355	   351     351	   354	  351	 348    356	   347
	//                        citem3  citem4  citm5  citem6 citem7 citem8 citem9  
	//                         (0~3)  (1~4)   (2~5)   (3~6) (4~7)  (5~8)  (6~9)
	//                        350.5    351 	  352.5   351 	 351 	352.5  349.5 <= min,max �����ϰ� center 2�� AVg
	//                                       => 7�� citem avg�� 351.1429
	
	
	for (adCntIdx=3; adCntIdx < nAdCount; adCntIdx++)	// (nAdCount - 3) ȸ �ݺ�
	{
		UINT  dwAdIBuf[4], dwAdVcBuf[4];	// 4 : slit size
		int	  cLoc;

		cLoc = adCntIdx - 3;
		memcpy(dwAdIBuf, &dwAdIVal[cLoc], sizeof(dwAdIBuf)); 

		qsort((void *)dwAdIBuf,						// Start target array
				4, 									// Array size in element
				sizeof(dwAdIBuf[0]),				// Element Size in byte
				CompareDouble);						// Compare function
		dAdSumI += dAdCmIVal[adCntIdx] = GetMedian((int*)dwAdIBuf, 4);

		memcpy(dwAdVcBuf, &dwAdVCVal[cLoc], sizeof(dwAdVcBuf)); 
		qsort((void *)dwAdVcBuf,					// Start target array
				4, 									// Array size in element
				sizeof(dwAdVcBuf[0]),				// Element Size in byte
				CompareDouble);						// Compare function
		dAdSumVc += dAdCmVCVal[adCntIdx] = GetMedian((int*)dwAdVcBuf, 4);
	
	}

	dAdI  = dAdSumI / (nAdCount-3);
	dAdVc = dAdSumVc / (nAdCount-3);


	g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasData[nMeasId].dAdI = dAdI;
	g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasData[nMeasId].dAdVc = dAdVc;


	MyTrace(PRT_LEVEL1, "..AdRead_VI_CMAvg(CalMode=%d,RJig=%d,Meas=%d): dAdI=%.3f dAdVc=%.3f CM(Centered Moving) Avg Ad.\n", 
				nCalMode, nRJigId, nMeasId,
				g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasData[nMeasId].dAdI,
				g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasData[nMeasId].dAdVc);


	return TRUE;
}

/*
double	CCalibDialog::GetMedian(double* array, size_t arraySize) 
{
	size_t center = arraySize / 2;// ��� ������ ���ݰ� ���ϱ�
	if (arraySize % 2 == 1) { 	// ��� ������ Ȧ����
		return array[center]; 		// Ȧ�� ������ �迭������ �߰� ��Ҹ� �״�� ��ȯ
	} 
	else {
		return (array[center - 1] + array[center]) / 2.0; // ¦�� �� ��Ҵ�, �߰� �� ���� ��� ��ȯ
	}
}

// qsort ���ο��� ���Ǵ� �� �Լ�
int	CompareDouble(const void *a, const void *b) 
{
	if (*(double*)a <  *(double*)b) return -1;
	if (*(double*)a == *(double*)b) return  0;

	return 1;
}
*/
double	CCalibDialog::GetMedian(int* array, size_t arraySize) 
{
	size_t center = arraySize / 2;// ��� ������ ���ݰ� ���ϱ�
	if (arraySize % 2 == 1) { 	// ��� ������ Ȧ����
		return array[center]; 		// Ȧ�� ������ �迭������ �߰� ��Ҹ� �״�� ��ȯ
	} 
	else {
		return (double)(array[center - 1] + array[center]) / 2.0; // ¦�� �� ��Ҵ�, �߰� �� ���� ��� ��ȯ
	}
}

// qsort ���ο��� ���Ǵ� �� �Լ�
int	CompareDouble(const void *a, const void *b) 
{
	if (*(int*)a <  *(int*)b) return -1;
	if (*(int*)a == *(int*)b) return  0;

	return 1;
}

double	CCalibDialog::GetMedian2(int* array, size_t arraySize) 
{
	size_t center = arraySize / 2;// ��� ������ ���ݰ� ���ϱ�
	if (arraySize > 5)
	{
		if (arraySize % 2 == 1) { 	// ��� ������ Ȧ���� ��� 3���� ���
			return (double)(array[center -1] + array[center] +array[center +1]) / 3.0; 
		}
		else {						// ��� ������ ¦���� ��� 4���� ���
			return (double)(array[center -2] + array[center -1] + array[center] +array[center +1]) / 4.0; 
		}
	}

	if (arraySize % 2 == 1) { 		// ��� ������ Ȧ����
		return array[center]; 		// Ȧ�� ������ �迭������ �߰� ��Ҹ� �״�� ��ȯ
	} 
	else {
		return (double)(array[center - 1] + array[center]) / 2.0; // ¦�� �� ��Ҵ�, �߰� �� ���� ��� ��ȯ
	}
}

// AdI, AdVc ���� �Է¹޾Ƽ� I, Vc, R ���� ����� ����.
// Input : nCalMode, dAdI, dAdVc    
// Output: rdI, rdVc, rdR    (Reference Type)
void CCalibDialog::Meas_CalcR_makeR(int nCalMode, double dAdI, double dAdVc, 
									double& rdI, double& rdVc, double& rdR) 
{
	CString strTemp;

	//----------------------------
	// A. Short R ���

	// 2017.11.29�� �迵�� ���� confirm.
	// Short �� ���� ADC I ���� �׳� R ���װ����� ����Ѵ�. 
	// Open�� ���� �������� VC ��ȭ�� �����ؼ� R�� ��ȭ������ 
	// Short�� �������̰�, ������ ��ȭ�� �����ؾ� �Ѵ�. 
	// Short�� ���� VC���� ���ǹ��� ������ �۾Ƽ� VC/I �� ���װ���ϴ� ���� �����Ƿ� 
	// �׳� ADC I ���� ���׼����� �����ϴµ� ���.
	//if (g_sSystem.IsShortTestCase(nCalMode) == TRUE) 
	if (g_saTestCondition[nCalMode].nTest == TEST_SHORT)
	{
		rdR  = dAdI;
		rdVc = 0;
		rdI  = 0;

		// ������ �ƴ����� ȣ����ġ�� �Ʒ� string�� �ѱ�� ���� ��� ERR.m_strState�� ���.
		strTemp.Format("..makeR(dAdI:%.3f, dAdVc:%.3f): CalMode is Short! Use dAdI as rdR=%.3f, rdVc=0, rdI=0\n",
				dAdI, dAdVc, rdR);
		ERR.Set(TEST_TRACE, strTemp);
		return;
	}

	//----------------------------
	// B. R ���   
	
	// 1. ADC I ���� mA(�Ǵ� uA)�� ��ȯ. sa_MeasData[nMeasId].dI�� ����
	double dAdI2 = dAdI, dI= 0;
	dI = UNIT_CONVERT(dAdI2, AD_TO_I0+ (g_saTestCondition[nCalMode].nIRng -1) -I_RANGE0);
	rdI = dI;		// Reference dI ������ ������ ����


	// 2. ADC VC ���� Volt�� ��ȯ. dMeasVc_Avg �� ����
	double dAdVc2 = dAdVc, dVc= 0;
	if (g_saTestCondition[nCalMode].nTest == TEST_4W)		// 4W
		dVc = UNIT_CONVERT(dAdVc2, AD_TO_4W_VOLT);

	else // 2W
	{	
		switch(g_saTestCondition[nCalMode].nVBSrc)
		{
			case C_LV1: case C_LV2: 
				dVc = UNIT_CONVERT(dAdVc2, AD_TO_LVOLT);
				break;

			case C_HV1: case C_HV2: 
				dVc = UNIT_CONVERT(dAdVc2, AD_TO_HVOLT);
				break;
			default: // default case�� �̹� range check�� �Ͽ����Ƿ� �������� �������� ����.
				break;		
		}
	}
	rdVc = dVc;


	// 3. R �� ���
	
	double dR = 9999999999.0;

#ifdef __NEW_CAL__
	// 2017.11.22 �迵�� ���� confirm. 
	// 1) VB read ���� 4W, Open, HvOpen, Short ��� ���Ŀ��� ����� �ʿ� ����.
	//     VB-VC �ʿ� ���� read ���� ����üũ������ ����ض�.
	//     ���open ������  VB-VC�� ���� VC�� �ְ� ������, ���� open �� ��쿡�� �ʿ� ����. 
	//    
	// 2) ADC_B ���Ͱ� HVO, 4W�� ���� I ���� �����ؼ� ���. read�� ������� ����.
	//     I�� �����ϰ� read �ؼ� ��鸮�� �� ���θ� ����üũ�ϸ� �ȴ�.   => �� �κ��� ���� ������ �ٰ� ����. ���???
	//
	
	//if (dI > 0)		
	{
		if (dI < 0.0000001) // check divide by zero
			rdI = dI = 0.0000001;

		// 2018.01.29  �迵�ʺ���� ��û��� ���� ��, 
		//             HV_OPEN ���̽��� (mode 16) original ACE400 �� 1/5 ������ R ���� �۰� ������ �ȴ�. �̰� ������? 
		if (g_saTestCondition[nCalMode].nFilter == FT_B_HV_OPEN )
		{
			// nISet ���� �̹� range üũ�� 0�� �ƴ��� Ȯ����.   
			// nISet�� *10���̹Ƿ� ���ÿ��� /10�� �ؼ� �����ʿ�
			rdI = dI = (double)(g_saTestCondition[nCalMode].nISet/ 10);		// read�� I�� ���������� ����
			dR = dVc / dI ;		// R =   VC / I (������) 

			if (dVc > 325)		//sylee150126    AD VC���� 65535�̻��϶� ���� ó��
				dR = 9999999999.0;
		}
		else if (g_saTestCondition[nCalMode].nFilter == FT_B_4WIRE )
		{
			// nISet�� *10���̹Ƿ� ���ÿ��� /10�� �ؼ� �����ʿ�. 
			// ��, 4W�� ���� *10��.
			rdI = dI = (double)(g_saTestCondition[nCalMode].nISet/ 10);
			dR = (dVc / dI) * 10;	// R =   VC / I (������) *10
		}
		else // ADCB_VCFAST:  LV 2W, LV Short
		{

			dR = dVc / dI;			// ���װ� =   VC / I (Read��) 
		}
	}

#else
	// 2018.02.07 �̻������� �ڵ� ����
	// 			  1. ACE400 �̻��� ���� �ڵ忡�� �ϴ� 4W�ε� I Read���� +10%, -10% �����̸�  VC/ISet, I Read���� �����  VC/I ,
	//            2. ADCB ���Ͱ� 2W HVO �϶����� VC/I,  2W������ HVO�� �ƴҶ�����  (VB-VC)/ I�� �ϰ� ����.
	// 2017.03.02 �迵�� ���� ��û
	//            3. 4W, Open (Short ����) �϶�, I Read ���� +1%, -1% �����̸� VC/ISet, I Read���� ����� VC/I�� �ش޶�.
	
	double	dIHigh, dILow, dISet;
	dISet = (double)(g_saTestCondition[nCalMode].nISet / 10);

	if (dI < 0.0000001)			// check divide by zero
		rdI = dI = 0.0000001;

	if ( g_saTestCondition[nCalMode].nTest == TEST_4W)
	{
		dILow  = dISet-dISet*0.1;	// 10% ���� 
		dIHigh = dISet+dISet*0.1;	// 10% ����

		// 1. read�� I ����   10%���� < readI  < 10%����  ���� �̳���� nISet ���� I������ ����Ͽ� R�� ����Ѵ�.
		if ((dILow < dI ) &&  (dI < dIHigh)) 
			rdI = dI = dISet;

		// R =   VC / I (Read��)   : 2018.01.29 �ϴ� ACE400 ���� �������� Test  
		// 4W�� ���� VC/I�� *10��.
		dR = (dVc / dI) * 10;		// R =   VC / I ( read ��)  *10  //161207-0 x10    ray ok

	}

	else // Open, Short 
	{
		dILow  = dISet-dISet*0.01;	// 1% ���� 
		dIHigh = dISet+dISet*0.01;	// 1% ����

		if (g_saTestCondition[nCalMode].nFilter == FT_B_HV_OPEN )	// HV Open
		{
#if 1
			// 2. Open�� ��� I Read ���� +1%, -1% �����̸� VC/ISet, �ƴϸ� VC/I
			if ((g_saTestCondition[nCalMode].nTest == TEST_OPEN) 
					&& ((dILow < dI ) &&  (dI < dIHigh)))
				rdI = dI = dISet;
#endif
			// R =   VC / I (Read��)   : 2018.01.29 �ϴ� ACE400 ���� �������� Test  
			dR = dVc / dI;				// R =   VC / I ( read ��) 


			if (dVc > 325)		//sylee150126    AD VC���� 65535�̻��϶� ���� ó��
				dR = 9999999999.0;
		}

		// ADCB_VCFAST: LV Open, Short
		else	
		{
#if 1
			// 3. Open�� ��� I Read ���� +1%, -1% �����̸� (VB-VC)/ISet, �ƴϸ� (VB-VC)/I
			if ((g_saTestCondition[nCalMode].nTest == TEST_OPEN) 
					&& ((dILow < dI ) &&  (dI < dIHigh)))
				rdI = dI = dISet;
#endif
			// R =   (VB-VC) / I (Read��)   : 2018.01.22 �ϴ� ACE400 ���� �������� Test  
			dR = (g_sSystem.m_dVb[nCalMode] - dVc) / dI;			
		}
	}
#endif



	//2017.11.24      ModeSer13.cpp 1472 ���� ��ó
	// ACE400 �̻��� ���� �ڵ�.  2W�� �� ����� R ����  IR1, IR2�� *1000,   IR3, IR4, IR5�� *1000000�� ��. 
	// 4W �� ������ IR1, IR2�� *10000, IR3, IR4, IR5�� *1000000. 
	if (dR != 9999999999)
	{
		switch(g_saTestCondition[nCalMode].nIRng)
		{
			case IR1: case IR2: 
				dR *= 1000.;		// Ohm-> mOhm ������ ��ȯ 
				break;

			case IR3: case IR4: case IR5:
				dR *= 1000000.;		// Ohm-> uOhm ������ ��ȯ 
				break;
		}
	}
	rdR = dR;

	strTemp.Format("..makeR(): dAdI:%.3f(rdI:%.3f), dVb:%.3f, dAdVc:%.3f(rdVc:%.3f), rdR=%.3f\n",
				dAdI, rdI, g_sSystem.m_dVb[m_nCalMode], dAdVc, rdVc,  rdR);
	ERR.Set(TEST_TRACE, strTemp);

}


// Check Invalid
void CCalibDialog::Meas_CalcR_CheckInvalid(int nCalMode, int nRJigId) 
{

	// 1. ������ ������ ����� Invalid. 
	if ((g_sSystem.m_saCalData[nCalMode][nRJigId].dMeasR_MaxDiffRate > DIFF_RATE_LIMIT)				// > 5%
			|| ((-g_sSystem.m_saCalData[nCalMode][nRJigId].dMeasR_MinDiffRate) > DIFF_RATE_LIMIT) )	// < -5%
		g_sSystem.m_saCalData[nCalMode][nRJigId].nValid = FALSE;
	else
		g_sSystem.m_saCalData[nCalMode][nRJigId].nValid = TRUE;



	// 2. ������ �����ϸ� R�� �����ؾ���. R���� ���� nRJigId�� ���� ������ ��쿡�� Invalid. 
	int		nRJigStart, nRJigEnd;
	g_sSystem.Get_RJigPinRange(nCalMode, nRJigStart, nRJigEnd);		// nRJigStart, nRJigEnd Reference Value
	if ((nRJigId == 0)				// nRJigId=0�̸� nRJigId-1�� -1�̹Ƿ� �޸� ���� �����ϹǷ� ����
	    || (nRJigId == nRJigStart))	// ù��° �׸��� �񱳴���� �����Ƿ� ����
	    return;

	if (g_saTestCondition[nCalMode].nTest != TEST_SHORT)	// Open, 4W
	{
		// ����üũ
		if (g_sSystem.m_saCalData[nCalMode][nRJigId-1].dMeasR_Avg > g_sSystem.m_saCalData[m_nCalMode][nRJigId].dMeasR_Avg) 
			g_sSystem.m_saCalData[nCalMode][nRJigId].nValid = FALSE;
	}

	// 3. Short�� ���Ұ� �����̹Ƿ� �Ųٷ� ������ ��,  Invalidó���Ѵ�.
	else 	// SHORT
	{
		// ����üũ
		if (g_sSystem.m_saCalData[nCalMode][nRJigId-1].dMeasR_Avg < g_sSystem.m_saCalData[m_nCalMode][nRJigId].dMeasR_Avg) 
			g_sSystem.m_saCalData[nCalMode][nRJigId].nValid = FALSE;
	}


}

// Calibration CalcR ����� ȭ�� Display
void CCalibDialog::Meas_CalcR_Display(int RJigIdx) 
{
	CString strTemp;

/*
 *  2018.02.26: �� �ȵ�. ����
 *  ��ü������ ���� ���� ���ڻ���  ���������� �ٲ�⸸ �ϰ� Ư�� ���θ� ���ڻ��� ���������� ������ ���� ����.
 *  m_gridCalResult.SetForeColorFixed(RGB(200, 0, 20)) �� ���� ����, ���� ������, ���񿭸� ���ڻ��� �ٲ�.

	if (g_sSystem.m_saCalData[m_nCalMode][RJigIdx].nValid == FALSE)
		m_gridCalResult.SetForeColor(RGB(200, 0, 20));		// Bad�� ��� ���������� ����.
	else
		m_gridCalResult.SetForeColor(RGB(0, 0, 0));			// Good�� ��� ���������� ����.
	DoEvents();
	UpdateData(FALSE);
*/
	strTemp.Format("%d", g_sSystem.m_saCalData[m_nCalMode][RJigIdx].nValid);
	m_gridCalResult.SetTextMatrix(RJigIdx+1, 2,  strTemp);		//Col2

	strTemp.Format("%.2f", g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_Avg);
	m_gridCalResult.SetTextMatrix(RJigIdx+1, 12,  strTemp);		//Col12

	strTemp.Format("%.2f", g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_MaxDiffRate);
	m_gridCalResult.SetTextMatrix(RJigIdx+1, 13,  strTemp);		//Col13

	strTemp.Format("%.2f", g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_MinDiffRate);
	m_gridCalResult.SetTextMatrix(RJigIdx+1, 14,  strTemp);		//Col14

	strTemp.Format("%.2f", g_sSystem.m_dVb[m_nCalMode]);
	m_gridCalResult.SetTextMatrix(RJigIdx+1, 15,  strTemp);		//Col15

	strTemp.Format("%.2f", g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasVc_Avg);
	m_gridCalResult.SetTextMatrix(RJigIdx+1, 16,  strTemp);		//Col16

	strTemp.Format("%.2f", g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasI_Avg);
	m_gridCalResult.SetTextMatrix(RJigIdx+1, 17,  strTemp);		//Col17

	strTemp.Format("%.2f", g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasAdVc_Avg);
	m_gridCalResult.SetTextMatrix(RJigIdx+1, 18,  strTemp);		//Col18

	strTemp.Format("%.2f", g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasAdI_Avg);
	m_gridCalResult.SetTextMatrix(RJigIdx+1, 19,  strTemp);		//Col19

	strTemp.Format("%.2f", g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_Max);
	m_gridCalResult.SetTextMatrix(RJigIdx+1, 20,  strTemp);		//Col20

	strTemp.Format("%.2f", g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_Min);
	m_gridCalResult.SetTextMatrix(RJigIdx+1, 21,  strTemp);		//Col21

	for (UINT measIdx=0; measIdx < m_nMeasCount; measIdx++)		// ACE400 j, nCount�� �ش�
	{
		strTemp.Format("%.2f", g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dR);
		m_gridCalResult.SetTextMatrix(RJigIdx+1, 22 + (measIdx*6 +1),  strTemp);	// Col23, Col29, Col35, ... Col77

		strTemp.Format("%.2f", g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dVc);
		m_gridCalResult.SetTextMatrix(RJigIdx+1, 22 + (measIdx*6 +2),  strTemp);	// Col24, Col30, Col36, ... Col78

		strTemp.Format("%.2f", g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dI);
		m_gridCalResult.SetTextMatrix(RJigIdx+1, 22 + (measIdx*6 +3),  strTemp);	// Col25, Col31, Col37, ... Col79

		strTemp.Format("%.2f", g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dAdVc);
		m_gridCalResult.SetTextMatrix(RJigIdx+1, 22 + (measIdx*6 +4),  strTemp);	// Col26, Col32, Col38, ... Col80

		strTemp.Format("%.2f", g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dAdI);
		m_gridCalResult.SetTextMatrix(RJigIdx+1, 22 + (measIdx*6 +5),  strTemp);	// Col27, Col33, Col39, ... Col81
	}

}

// Calibration CalcR ����� CSV ���� ��� 
void CCalibDialog::Meas_CalcR_FileOut() 
{

	if (g_sSystem.Save_LogCalData(m_nCalMode, m_nMeasCount) == FALSE)
	{ 	ErrMsg(); ERR.Reset(); return; }

}

//---------------------------------
// OnButtonChgRef ���� �Լ�
//---------------------------------
void CCalibDialog::OnButtonChgRef() 
{
	// TODO: Add your control notification handler code here
	//
	
	// ���� ������ MeasR, MeasAdVc, MeasAdI ���� Ref ������ �����Ѵ�.
	int 	RJigIdx;
	int		nRJigStart, nRJigEnd;
	g_sSystem.Get_RJigPinRange(m_nCalMode, nRJigStart, nRJigEnd);		// nRJigStart, nRJigEnd Reference Value
	for (RJigIdx = nRJigStart; RJigIdx <= nRJigEnd; RJigIdx++)
	{
		g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dRefR_Avg = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_Avg;
		g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dRefVcVal = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasAdVc_Avg;
		g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dRefIVal  = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasI_Avg; 
	}
	
	// ������ RefCalData�� CalResult Grid�� ����Ѵ�.
	SetGrid_RefCalData(m_nCalMode);
}

//---------------------------------
// ButtonSaveRef ���� �Լ�
//---------------------------------
void CCalibDialog::OnButtonSaveRef() 
{
	// TODO: Add your control notification handler code here
	
	// ���� RefCalData��  ������ file ��ġ�� save�Ѵ�.
	if (g_sSystem.Save_CalData(m_nCalMode) == FALSE)
	{ 	ErrMsg(); ERR.Reset(); return; }
}

//---------------------------------
// ButtonClrScreen ���� �Լ�
//---------------------------------
void CCalibDialog::OnButtonClrScreen() 
{
	ClearGrid_CalResult();
}

void CCalibDialog::ClearGrid_CalResult()	
{
	// TODO: Add your control notification handler code here
	
	int i, j; 
	
	for (i = 1; i < (MAX_2W_RJIG+1); i++)	// ����� �����ϰ� clear
		for (j = 0; j < NUM_CAL_RESULT_COL; j++)
			m_gridCalResult.SetTextMatrix(i, j, "           ");
}

//---------------------------------
// ButtonStopMeas ���� �Լ�
//---------------------------------
void CCalibDialog::OnButtonStopMeas() 
{
	// TODO: Add your control notification handler code here
	
	m_bFlagStopMeas 	= TRUE;
	m_bFlagStopAutoCal 	= TRUE;
}

//---------------------------------
// ButtonViewResultFile ���� �Լ�
//---------------------------------
void CCalibDialog::OnButtonViewResultFile() 
{
	// TODO: Add your control notification handler code here
	
	CString strTemp;
	
	// 2017.11.30  Output ������ ACE400�� ��ġ�� overwrite���� �ʰ� ��ü������ �����ϱ�� ��.
	// ACE400 LogCal ���丮��� ACE400Cal/Measure ���丮 ���.
    //strTemp.Format("%s\\001LOGRCal%d.csv",g_sFile.ACE400_LogCalDataDir, (nCalMode +1) );
    strTemp.Format("%s\\001LOGRCal%d.csv",g_sFile.MeasureDir, (m_nCalMode +1) );

	::ShellExecute(NULL,"open","EXCEl.EXE",strTemp,"NULL",SW_SHOWNORMAL);	
	
}

//---------------------------------
// ButtonAutoCalib ���� �Լ�
//---------------------------------
void CCalibDialog::OnButtonAutoCalib() 
{
	m_bFlagStopAutoCal = FALSE; 
	UINT	dwRet = TRUE;

	// TODO: Add your control notification handler code here
	
	// 1. 4W Test ���� ����
	int nCalMode;
	for (nCalMode=0; nCalMode < MAX_TEST_MODE; nCalMode++)
	{
		if (g_saTestCondition[nCalMode].nTest == TEST_4W)
		{
			dwRet = Auto_Calib_Mode(nCalMode);
			if (dwRet == FALSE)		// Auto Calibration Stop Detected.
				return;
		}

	}

	// 2. Open Test ����
	for (nCalMode=0; nCalMode < MAX_TEST_MODE; nCalMode++)
	{
		if (g_saTestCondition[nCalMode].nTest == TEST_OPEN)
		{
			dwRet = Auto_Calib_Mode(nCalMode);
			if (dwRet == FALSE)		// Auto Calibration Stop Detected.
				return;
		}

	}

	// 3. Short(Leak) Test ����
	for (nCalMode=0; nCalMode < MAX_TEST_MODE; nCalMode++)
	{
		if (g_saTestCondition[nCalMode].nTest == TEST_SHORT)
		{
			dwRet = Auto_Calib_Mode(nCalMode);
			if (dwRet == FALSE)		// Auto Calibration Stop Detected.
				return;
		}
	}

	AfxMessageBox("Auto Calibration Complete.", MB_OK | MB_ICONERROR);
}

BOOL CCalibDialog::Auto_Calib_Mode(int nCalMode) 
{
	CString strTemp;

	// ���� Test ���� ���(nCalMode) ����
	m_comboCalMode.SetCurSel(nCalMode);	

	//-------------------------------
	// 1. Load TestCondition
	// �ش� nCalMode�� TestCondition �ε� 
	OnSelchangeComboCalibMode();

	//-------------------------------
	// 2. Load RJigData, PreCalData
	// �ش� nCalMode�� RJigData(Net Pin Data), PrevCalData �ε� 
	OnButtonLoadRef(); 
	DoEvents(); 

#if 0
	ERR.Set(TEST_ERR, "test");
	ErrMsg(); ERR.Reset(); 
	DoEvents(); 
#endif

	//-------------------------------
	// 3. Calibration Measure ���� 
	OnButtonStartMeas();

	// 'Stop AutoCal' ��  ������ ��� �ߴ��ϰ� ����
	DoEvents();
	if ( m_bFlagStopAutoCal == TRUE)	
	{
		strTemp.Format("OnButtonAutoCalib() Stop!: nCalMode=%d, 'Stop AutoCal' Detected.\n", nCalMode);
		ERR.Set(TEST_ERR, strTemp);
		ErrMsg(); ERR.Reset(); 
		return FALSE;
	}
	
	//-------------------------------
	// 4. Change Reference ����
	// => ���� Save Reference�� Ȯ���� �Ϸ�� ������ Ǯ� ����Ѵ�.
	//   H/W ���� �����ؼ� ����ȭ ����.
	//OnButtonChgRef(); 


	//-------------------------------
	// 5. Save Reference ����
	// => ���� Save Reference�� Ȯ���� �Ϸ�� ������ Ǯ� ����Ѵ�.
	//   H/W ���� �����ؼ� ����ȭ ����.
	// OnButtonSaveRef(); 		
	DoEvents(); 
	Delay(400, msec);

	return TRUE;
}

//-----------------------------------
// IDC_EDIT_MEAS_CNT  ���� �Լ�
//-----------------------------------
void CCalibDialog::OnChangeEditMeasCnt() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	UpdateData(FALSE);
}

