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

	// 2018.02.21 창 바뀔 때마다 m_nCalMode=5로 바뀌는 문제 관련해서 이 초기화를 InitMember()에서 생성자로 옮김. 
	m_nCalMode = 5;	// 현재 테스트 전압 ex) 5= "6 Open 10 V"
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
		InitView();		// 폰트 사이즈 조정 등
	}

	
}

BOOL CCalibDialog::InitMember()
{

	// UI 컨트롤 관련 Value형 멤버 변수 초기화
	m_nMeasCount = INIT_MEAS_COUNT;
	m_nHLVC = -1;
	m_bIsPinLoaded = FALSE;
	m_bFlagStopMeas = FALSE;
	m_bFlagStopAutoCal = FALSE;

	//-------------------------
	// 데이터 파일 불러오기
	
	g_sFile.Init();
	g_sSystem.InitMember();
	g_sModel.InitMember();

	int ret;
	// "C:\ACE400\SETUP\FilePathJig.ini" 에서 BDL 파일 path를 로드한다.
	ret = Read_BdlFilePath();
	if (ret == FALSE) {	ErrMsg(); ERR.Reset(); return FALSE; }
	
	// "C:\\ACE400\\SETUP\\SysSet01.ini"에서 g_sSystem.m_nSystemType 데이터 (QD, SD, DD, S100_1, S100_2) 로드
	ret = g_sSystem.Load_SysInfoData();
	if (ret == FALSE) {	ErrMsg(); ERR.Reset(); return FALSE; }

	// g_sSystem.m_saRJigData 데이터 로드
	//for "C:\\ACE400\\BDL\\QC\\RJigReadType.TXT", C:\\ACE400\\BDL\\QC\\RJigReadType_S100.TXT"
	ret = g_sSystem.Load_RJigData();
	if (ret == FALSE) { ErrMsg(); ERR.Reset(); return FALSE; }

	// CalMode ComboBox에 TestCondition Text List를 추가한다. 
	for (int i=0; i < MAX_TEST_MODE; i++)
	{
		//Test not Use만 combo에 표시하면  m_nCalMode를 그대로 m_gridCalModeCondition[]의 index로 
		//사용할 수 없다.  추후 이 문제를 해결하고 아래 코드를 풀어서 사용할 것. 
		//if (g_saTestCondition[i].nTest != TEST_NOT_USE)
			m_comboCalMode.InsertString(-1, g_saTestCondition[i].szCalModeName);
	}

	// 2018.02.21 창 바뀔 때마다 m_nCalMode=5로 바뀌는 문제 관련해서 이 초기화를 생성자로 옮김. 
	//m_nCalMode = 5;	// 현재 테스트 전압 ex) 5= "6 Open 10 V"
	m_comboCalMode.SetCurSel(m_nCalMode);	
	

	// "C:\ACE400\SETUP\\SysSet21_ACE400.ini"에서 g_saTestCondition[MAX_TEST_MODE] 데이터 로드
	ret = g_sSystem.Load_TestCondtionData();
	if (ret == FALSE) { ErrMsg(); ERR.Reset(); return FALSE; }

	// "C:\ACE400\SETUP\\SysSet21_4W.ini"에서 g_sSystem.m_na4W_Range[MAX_TEST_MODE] 데이터 로드
	ret = g_sSystem.Load_4W_RangeData();
	if (ret == FALSE) { ErrMsg(); ERR.Reset(); return FALSE; }

	ret = g_sModel.Load_PinConFile();
	if (ret == FALSE) { ErrMsg(); ERR.Reset(); return FALSE; }

	ret = g_sModel.Load_PinSetupData();
	if (ret == FALSE) { ErrMsg(); ERR.Reset(); return FALSE; }


	// 2018.02.19  Bad 항목만 별도의 .csv 파일로 모으기 위해 파일을 최초 생성  
	FILE 	*fpBad;
	char  	fNameBad[200]; 
	CString	strTemp;
	
	// 이전에 존재한 내용을 모두 clear한 다음 일단 close한다.  
	// 실제 출력은 calMode별로 Meas_CalcR_FileOut() 호출시에 수행함.
	strTemp.Format("%s\\001LOGRCalBad.csv",g_sFile.MeasureDir );	
	::ZeroMemory(&fNameBad, sizeof(fNameBad));
	strcat( fNameBad ,  strTemp);		
	fpBad = fopen(fNameBad,"wt");	// "wt" : 이전에 존재하는 파일 내용이 있다면  destroy하고 open
	if (fpBad == NULL)
	{ 	ERR.Set(FLAG_FILE_CANNOT_OPEN, fNameBad); return FALSE; }
	fclose(fpBad);

	UpdateData(FALSE);

	return TRUE;
}

void CCalibDialog::InitView()
{
	UpdateData(TRUE);

	// 그리드 설정
	int i;
	m_gridCalModeCondition.SetCols(NUM_CAL_CONDITION_COL);	// 16 컬럼
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

	m_gridCalResult.SetCols(NUM_CAL_RESULT_COL);	// 22 +60(per Meas) 컬럼
	m_gridCalResult.SetRows(MAX_2W_RJIG+1);			// 헤더까지 97줄 필요
					//   0    1    2     3       4       5       6       7        8       9       10     
					//  No   Use  Valid VbTrPin VbRyPin VcTrPin VcRyPin OrgR  RefR(Avg) RefR:VC  Ref R:I
	int iColWidth2[] = {400, 600, 800,  800,    800,    800,     800,   1200,   1100,    900,    900, 

					//   11     12     13        14       15         16         17          18          19          20     21   
					//  빈칸  R(Avg) MaxRDifRt MinRDifRt R(Avg):VB  R(Avg):VC  R(Avg):I  R(Avg):AdVC  R(Avg):AdI   MaxR   MinR  
						200,   1100,  1200,      1200,    900,       900,       900,        1100,       1100,      1100,  1100, 

					//  22     23    24    25      26    27
					//  빈칸  01:R  01:VC  01:I  01:ADVC  01:ADI
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

	// 해당 calMode의 TestCondition 정보를  m_gridCalModeCondition에 display한다.
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
	m_gridCalResult.SetTextMatrix(0, 10, "Ref R:I");		// 11은 빈 컬럼

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
		// Col22 +(i*6)은 빈 컬럼
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


	// Cal Result Grid Screen을 초기화한다.
	//ClearGrid_CalResult();

	UpdateData(FALSE);
}

//---------------------------------------------------
// 	m_comboCalMode & m_gridCalModeCondition 관련 함수
//---------------------------------------------------


void CCalibDialog::OnSelchangeComboCalibMode() 
{
	// TODO: Add your control notification handler code here
	m_nCalMode = m_comboCalMode.GetCurSel();

	// 해당 calMode의 TestCondition 정보를  m_gridCalModeCondition에 display한다.
	SetGrid_TestCondition(m_nCalMode);	

	// mode는 change 되었으나 아직 해당 pin 정보 로딩 안했으므로 FALSE
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



// CalMode Condtion 그리드 컨트롤을 더블클릭했을 때 처리하는 루틴
// CalModeCondition 그리드의 항목을 수정한다. 
void CCalibDialog::OnDblClickGridCondition() 
{
	// TODO: Add your control notification handler code here

/*
	
	CEditGridDlg dlg; 
	// 현재의 x, y 좌표를 얻는다. 
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
	// 해당 컬럼의 data를 가져온다. 
	int nRow, nCol; 
	CString strName, strTemp; 
	nCol = m_gridCalModeCondition.GetColSel(); 
	nRow = m_gridCalModeCondition.GetRowSel(); 
	if (nCol == 0 )	// 첫번째 컬럼은 수정불가.  CalMode Name은 수정할 수 없다.
		return;		// 첫번째 컬럼은 클릭이 가능하므로 반응을 하지 않는 것으로 대응. 에러처리 하지 않고 종료.

	if (nCol <= 0 || nCol >= NUM_CAL_CONDITION_COL) // Col Range Error
	{
		strTemp.Format("OnDblClickGridCondition: nCol=%d (0 < nCol < %d)", nCol, NUM_CAL_CONDITION_COL);
		ERR.Set(RANGE_OVER, strTemp);
		ErrMsg(); ERR.Reset(); 
		return;
	}
	


	//-----------------------------------------
	// 다이얼로그를 띄워서 입력을 받는다.
	//
	
	// 해당 컬럼 항목의  Range Value 확보  
	int nMinVal = 0;	
	int nMaxVal = 10000;	

	nMinVal = g_saTestConditionRange[nCol].nMinVal;
	nMaxVal = g_saTestConditionRange[nCol].nMaxVal;

	// 해당 컬럼의 현재 출력값을 가져와서 입력 다이얼로그에 int로 전달한다.
	strName = m_gridCalModeCondition.GetTextMatrix(nRow, nCol);
	int nVal = atoi(strName);
	if(NKEYPAD.SetValue((int)nVal, nMinVal, nMaxVal) == FALSE) 
		return;

	NKEYPAD.DoModal();
	nVal = (int)NKEYPAD.GetValue();


	//-----------------------------------------------
	// 다이얼로그에서 입력받은 값을 해당 컬럼에 출력한다.
	strName.Format("%d", nVal);
	m_gridCalModeCondition.SetTextMatrix(nRow, nCol, strName);


	//-----------------------------------------
	// 해당 TestCondition 내부 정보에 반영한다.
	// UI <-> 내부 TestCondition data 사이의 정보를 일치 시킴.
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
// ButtonSaveCondition 관련 함수
//------------------------------------
// 내부 TestCondition <-> 외부 CalConditon File(C:\ACE400\SETUP\\SysSet21_ACE400.ini) 과의 Sync는
// 'Save Condtion' 버튼 클릭시에 맞춘다. 
void CCalibDialog::OnButtonSaveCondition() 
{
	// TODO: Add your control notification handler code here
	BOOL bRet = g_sSystem.Save_TestCondtionData();
	if (bRet == FALSE) {	ErrMsg(); ERR.Reset(); return; }
	
}

//-------------------------------------
// ButtonViewConditionFile 관련 함수
//------------------------------------
void CCalibDialog::OnButtonViewConditionFile() 
{
	// TODO: Add your control notification handler code here
	
	//CString strTemp;
    //strTemp.Format("%s" , g_sFile.ACE400_TestConditionPath);		// "C:\ACE400\SETUP\\SysSet21_ACE400.ini"

	::ShellExecute(NULL,"open","notepad.EXE",g_sFile.ACE400_TestConditionPath,"NULL",SW_SHOWNORMAL);	

}

//--------------------------------------------
// RJigData 초기화 & ButtonLoadRef 관련 함수
//--------------------------------------------


void CCalibDialog::OnButtonLoadRef() 
{
	// TODO: Add your control notification handler code here
	
	// Cal 관련 data만 초기화한다.    RJigData는 초기화하면 안 됨.
	g_sSystem.InitCalData();
	
	// Cal Result Grid Screen을 초기화한다.
	ClearGrid_CalResult();

	// 해당 calMode의 RjigData, Ref R 값을 m_gridCalResult에 display한다.
	SetGrid_RJigData(m_nCalMode);	

	// 주의 SetGrid_RJigData(m_nCalMode)를 SetGrid_PrevCalData(m_nCalMode)보다 먼저 호출해 줘야 
	// m_saCalData[nCalMode][i].dOrgR값을 확보할 수 있다.

	// 해당 calMode의 이전 Ref cal data 를 Grid에 로드한다.
	SetGrid_PrevCalData(m_nCalMode);	

	
	// mode는 change 되었고 해당 pin 정보도 로딩 되었으므로 TRUE로 변경
	m_bIsPinLoaded = TRUE;
}

// 해당 calMode의 RjigData, Ref R 값을 m_gridCalResult에 display한다.
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

	// RJigData s100용인 경우(1), 아닌경우 (0)
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

				// dOrgR의 disply는 여기서 안하고  SetGrid_PrevCalData()할 때 해 주기로 한다.
					
				// p_saRJigData[RJigIdx].dOrgR을 CalData display시 (SetGrid_PrevCalData)에 표기해 주기 위해 
				// 지금 시점에 CalData의 .OrgR 값에 반영해 둔다.  추후 CalData file Save 에도 사용.
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

				// dOrgR의 disply는 여기서 안하고  SetGrid_PrevCalData()할 때 해 주기로 한다.

				// p_saRJigData[RJigIdx].dOrgR을 CalData display시 (SetGrid_PrevCalData )에 표기해 주기 위해 
				// 지금 시점에 CalData의 .OrgR 값에 반영해 둔다.  추후 CalData file Save 에도 사용.
				g_sSystem.m_saCalData[nCalMode][RJigIdx].dOrgR = p_saRJigData[RJigIdx].dOrgR;
			}
			break;


		case TEST_NOT_USE:			// TEST_NOT_USE인 경우는 에러출력 대신 Grid에 로드 안함
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

// 해당 calMode의 이전 Ref cal data 를 Grid에 로드한다.
void CCalibDialog::SetGrid_PrevCalData(int nCalMode)	
{
	
	// ("C:\\ACE400\\SETUP\\CAL\\CALR%02dB.INI",mode  ) 에서  m_saCalData 데이터의 이전 Ref 값 로드
	// nCalMode의 range check도 여기에서 수행하므로 SetGrid_PrevCalData()의 range check는 하지 않음
	if (g_sSystem.Load_PrevCalData(nCalMode) == FALSE)
	{ 	ErrMsg(); ERR.Reset(); return; }


	// 로드한 previous RefCalData를 CalResult Grid에 출력한다.
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

		case TEST_NOT_USE :			// TEST_NOT_USE인 경우는 에러 출력 대신 Grid에 로드 안함
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
// ButtonStartMeas 관련 함수
//---------------------------------
void CCalibDialog::OnButtonStartMeas() 
{
	// TODO: Add your control notification handler code here
	
	CString	strTemp, strTemp2;
	UINT dwRet;
	

	if (m_bIsPinLoaded == FALSE)	// RJigPinData가 로딩이 되었는지 체크
	{
		strTemp.Format("Cannot Start Measure: CalMode=%d+1. Push 'Load Refference' Button please.", m_nCalMode);
		ERR.Set(USER_ERR, strTemp);
		ErrMsg(); ERR.Reset(); 
		return;
	}
	
    //---------------
	// RANGE CHECK
	
	//현재 calMode의 TestCondtion에서 nTest Type을 체크한다.
	switch (g_saTestCondition[m_nCalMode].nTest)
	{
		case TEST_OPEN: case TEST_SHORT: case TEST_4W:		
			break;

		case TEST_NOT_USE :			// TEST_NOT_USE 또는 다른 값일 경우에는 측정을 수행하지 않는다.
		default:	
			strTemp.Format("Cannot Start Measure: CalMode=%d+1 nTest=%d(%s).", 
							m_nCalMode, g_saTestCondition[m_nCalMode].nTest, 
							(g_saTestCondition[m_nCalMode].nTest == TEST_NOT_USE) ? "TEST_NOT_USE" : "Invalid Value");
			ERR.Set(RANGE_OVER, strTemp);
			ErrMsg(); ERR.Reset(); 
			return;
	}
	
	// TestCondition의 range check는  InitMember()에서 g_sSystem.Load_TestCondtionData() 할때 
	// 모든 케이스에 대해 미리 수행하였으나 여기에서도 예방차원에서 한번 더 체크
	// 그외 Test Condtion  에러에 대한 체크도 수행.
	if( Meas_CheckRange_TestCondition() == FALSE)
		return;



	__int64	startTime, endTime ;
	m_bFlagStopMeas = FALSE; 
	startTime = GetMicroSecond();		// Calibration Time 측정용.

    //---------------
	// INITAL SET

	// Meas 진행중에 사용되는 변수값 초기화
	m_nHLVC = -1;

    // CModeSub71::AOnPoweroff2() 에 해당함.
    // MM.Break(VB_ONLY): bit52(HV_RELAY_IN_IO_0) ~ bit55(LV_RELAY_IN_IO_1) VB Apply 관련 비트를 모두 초기화
	// MM.Break(VB_ALL) : bit00 ~ bit127 OUTBIT 영역 전부를 OFF 후 일부 영역만 ON
    // MM.Break(ALL_OUT) 수행시에 내부적으로 먼저  MM.Break(VB_ONLY)를 수행함.
	dwRet = MM.Break(ALL_OUT);					// MM.Break()에서 미리 Trace를 출력함. return값 강제 조정을 위해..
	strTemp.Format("Cal::..StartMeas(m_nCalMode=%d): MM.Break(ALL_OUT) %s\n", m_nCalMode, (dwRet == FALSE)? "Error" : "OK");
	if (dwRet == FALSE) {	
		strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
		ERR.Set(ERR.Get(), strTemp2); 
		ErrMsg(MB_OK, FALSE); ERR.Reset();  // bTraceOn=FALSE, 에러를 Trace 빼고 메시지박스만 출력
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
	if (Meas_ReadVb() == FALSE)		// 여기에서 실제 VB값을 확보  (Vb OutCheck 아닌 상태)
		return;
	
	//--------------------------------------
	// 고압-> 저압 전환시  VB Out Check
	
	// 고압 -> 저압시에 이전 테스트의 영향으로 Vb 값이 정상적으로 읽히지 않을 수 있으므로
	// VB_OUT_CHECK를 걸고 현재 기준 전압이하로 떨어질 때 까지 기다려야 한다.
	
	// VB OutCheck를 걸어서 VbRead를 해보고 Set값보다 높으면 떨어질때까지 VbOutCheck를 반복한다.
	// 이상윤부장 코드에서는 Open300V -> 4W 10V로 넘어갈때 체크하기 위해 하기 4W일 때에만 수행하게 코딩해 둠.
	//if (g_saTestCondition[m_nCalMode].nTest == TEST_4W)
	
	// 고압-> 저압 전환시에만 수행. 저압 -> 고압 전환시에는 문제가 없음.
	if (g_saTestCondition[m_nCalMode].nVBSrc == LV1 || g_saTestCondition[m_nCalMode].nVBSrc == LV2)
	{
		// 테스트필요1: Open-> 4W로 넘어가는 시점에는 하기 코드가 반드시 수행이 되어야 함.
		// 추후 H/W 테스트시에 하기 코드 제대로 수행되는지 정밀 테스트 필요함.
		
		if (Meas_VbOutCheck() == FALSE)		
			return;

	}


	//테스트필요2: Vb Out Check를 'VBSet  이후  VBRead' 이전에 하는게 나을지 추후 변경해서 HW 연동 테스트 필요.

    //-------------------------
    // LVC/HVC 설정 
    if (Meas_SetLvcHvc() == FALSE)
    	return;


	
	//-------------------------------------------
	// ADC_B Filter 설정,  
	// FT_B_HV_OPEN(3)일 경우에는 HV Open Gain도 설정
    if (Meas_SetAdcBFilter() == FALSE)
    	return;

	//----------------------
	// ADC_A Filter 설정
	// FT_A_I_FAST 또는 FT_A_I_SLOW 인 경우 I High Gain도 설정
    if (Meas_SetAdcAFilter() == FALSE)
    	return;


	//----------------------
    // 4W Gain 설정
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

	// Setup Time 계산
	endTime = GetMicroSecond(); 	
	g_sSystem.m_dCalTime[0] = (endTime - startTime) / 1000.; 	// usec-> msec로 전환
	startTime = endTime;			// R값 측정Time 시작 


	//--------------------------
	// 4W가 아닌 경우 R 값 측정
	if (g_saTestCondition[m_nCalMode].nTest != TEST_4W)
		if (Meas_CalcR_2W() == FALSE)
			return;


	//--------------------------
	// 4W인 경우 R 값 측정
	if (g_saTestCondition[m_nCalMode].nTest == TEST_4W)
		if (Meas_CalcR_4W() == FALSE)
			return;


	// R값 측정 Time 계산
	endTime = GetMicroSecond(); 	
	g_sSystem.m_dCalTime[1] = (endTime - startTime) / 1000.;	// usec-> msec로 전환 

	//---------------------------
	// CalResult 파일 출력 
	Meas_CalcR_FileOut();



	//-----------------------------------------------------------
	// Calibration Test 마지막 단계에서 전체 비트를 다시 초기화 
	dwRet = MM.Break(ALL_OUT);					// MM.Break()에서 미리 Trace를 출력함. return값 강제 조정을 위해..
	strTemp.Format("Cal::..StartMeas(): Test End. MM.Break(ALL_OUT) %s\n", (dwRet == FALSE)? "Error" : "OK");
	if (dwRet == FALSE) {	
		strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
		ERR.Set(ERR.Get(), strTemp2); 
		ErrMsg(MB_OK, FALSE); ERR.Reset();  // bTraceOn=FALSE, 에러를 Trace 빼고 메시지박스만 출력
	}		
	MyTrace(PRT_LEVEL1, "%s\n", strTemp);
}


BOOL CCalibDialog::Meas_CheckRange_TestCondition() 
{
	// 현재 calMode, TestCondtion의 Range를 체크한다.
	
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

	// Check_TestCondtionRange(15, g_saTestCondition[m_nCalMode].nTest)는 위에서 미리 수행함. skip
	

	//----------------------------------
	// Test Condition 에러 체크
	
	// CalMode의 Range가 171~175인데 nTest가 4W가 아니면 에러 출력
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

	// CalMode의 Range가 171~175가 아닌데  nTest가 4W이면 에러 출력
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

	// nTest는 4W인데, nFilter가 4W가 아니면 에러 출력
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

	// nTest는 4W가 아닌데, nFilter가 4W 면 에러 출력
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
// if( nVRel==1) Sub71.AOnVSetMulti1(nVSet,0,0,0) for LV1 에 해당. 
// UI 출력부분 제외하고 AVBRV1S()와 동일함.
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
		default: break;		// default case는 위에서 range check를 하였으므로 논리적으로 존재하지 않음.
	}

	dwVSetVal = g_saTestCondition[m_nCalMode].nVSet;	// Voltage Set		Col3  // 1~ 300

	// Volt 값을 DA 값으로 변경  (col3 Volatage Set  1~300 )
	UNIT_CONVERT(dwVSetVal, VOLT_TO_DA);	//  ex) 5 -> 15000  

	// sSET_VB:   DA 값을 IO카드로 보낸다.		
	//             Sub71.AOnVBSet1(1,av) 과 동일
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

#define VB_READ_COUNT		5			// 이 값은 3 이상이어야 device by zero등의 문제가 안 생김
#define VB_READ_RETRY_COUNT		10			
// ADCA 에서 VB값 read.  
// Sub71.AOnReadVB1(1) 과 동일  ;// VB READING ,   nMode2     1=LV1,  2=LV2,  3=HV1, 4=HV2; 
BOOL CCalibDialog::Meas_ReadVb() 
{
	CString strTemp, strVSrc;
	UINT 	dwRet = TRUE;
	UINT	dwVbSrc, dwVbHL;

	// 4Wire일 때에는 VB read 하지 않는다. nVSet 값을 그냥 할당 
	if (g_saTestCondition[m_nCalMode].nTest == TEST_4W)
	{
		UINT	dwVSetVal;
		g_sSystem.m_dVb[m_nCalMode] = g_saTestCondition[m_nCalMode].nVSet;

		dwVSetVal = g_saTestCondition[m_nCalMode].nVSet;	// Voltage Set		Col3  // 1~ 300
		UNIT_CONVERT(dwVSetVal, VOLT_TO_DA);	// Volt 값을 DA 값으로 변경  

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
		default: break;		// default case는 위에서 range check를 하였으므로 논리적으로 존재하지 않음.
	}
 	
	// sSET_VB_HL:  MM.Get(V)하기 전에 VB를 선택한다.    (ex: LVB)
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
	

	// sSET_ADC_A:  MM.Get(V)하기 전에 ADCA Filter를 VBFT로 선택한다. 
	dwRet = MM.Set(VBFT);			 	// sSET_ADC_A: 
	strTemp.Format("Meas_ReadVb(): MM.Set(VBFT) %s\n", (dwRet == FALSE)? "Error" : "OK");
	if (dwRet == FALSE) {
		ERR.Set(ERR.Get(), strTemp);
		ErrMsg(); ERR.Reset(); 
		return (BOOL)dwRet;
	}
	MyTrace(PRT_LEVEL1, "%s\n", strTemp);
	//Delay(5, msec);

	// sGET_VI:  ADC A 전압 설정값(AD)을 read한다.
	UINT 	dwVal[VB_READ_COUNT], dwMin=0, dwMax=0, dwSum=0;
	double 	dAdVb = -1;


	for (int retry=0; retry < VB_READ_RETRY_COUNT; retry++)
	{
		dwRet = TRUE;	//  retry진행을 위해 TRUE로 재설정.

		dAdVb = -1;
		dwMin = 0; dwMax = 0; dwSum = 0;
		::FillMemory(dwVal, sizeof(dwVal), -1);

		for (int i=0; i < VB_READ_COUNT; i++)		// 5회 read하여 평균을 낸다.
		{
			// check return value
			dwVal[i] = (UINT)MM.Get(V);	// sGET_VI:  read from IO Card
			if( dwVal[i] == (UINT)(-1) ) 	
			{
				strTemp.Format("MM.Get(V) dwVal[%d]=%u ERROR.", i, dwVal[i]);
				ERR.Set(ERR.Get(), strTemp);
				ErrMsg(); 	ERR.Reset();
				dwRet = FALSE;
				break;		// 내부 for문 break.
			}
		
			if (i == 0) {
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
	
		if (dwRet == FALSE) 	// 내부 for문 break라면 . MM.Get(V) 실패 상황.
		{
			MyTrace(PRT_LEVEL1, "Meas_ReadVb() retry=%d: MM.Get(V) %s: \n\n", retry, (dwRet == FALSE)? "ERROR" : "OK" );
	
			continue;		// 다음 retry로 점프
		}

		// 내부 for문 break된게 아니라면. 즉, 정상 read 상황이면 평균값 계산 진행 
		dAdVb = (dwSum - dwMax - dwMin) / (VB_READ_COUNT -2);  // min, max 제외하고 3회로 나눈 평균값
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
			default: // default case는 이미 range check를 하였으므로 논리적으로 존재하지 않음.
				break;		
		}

		// Ad평균값에 대해 VB Val Check 해서 이상이 있으면(ret=FALSE) retry. (max 3회)
		if (Meas_CheckRange_VbVal(dAdVb) == FALSE)
		{

//PC Test 할 때에는 값이 제대로 읽히지 않으므로 retry error 처리는 skip
#ifndef __PC_TEST__
			dwRet = FALSE;

			if ((retry + 1) == VB_READ_RETRY_COUNT) 	// 3회 vb read retry 모두 실패시 메시지박스 출력. 
				ErrMsg(MB_OK, FALSE); 	
			ERR.Reset();	// 메시지박스 출력 안하더라도 다음번 에러 Set을 위해 ERR.Reset()

			Delay(100, msec);	// 100ms 쉬고 
			MyTrace(PRT_LEVEL1, "Meas_ReadVb() retry=%d: MM.Get(V) %s: dAdVb= %.3f, dVb= %.3f \n\n", 
						retry, (dwRet == FALSE)? "ERROR" : "OK", 
						g_sSystem.m_dAdVb[m_nCalMode], g_sSystem.m_dVb[m_nCalMode]);

			continue;		// 다음 retry로 점프	
#endif
		}

		// VB Val Check 해서 이상 없으면 retry 종료. 
		MyTrace(PRT_LEVEL1, "Meas_ReadVb() retry=%d: MM.Get(V) %s: dAdVb= %.3f, dVb= %.3f \n\n", 
					retry, (dwRet == FALSE)? "ERROR" : "OK", 
					g_sSystem.m_dAdVb[m_nCalMode], g_sSystem.m_dVb[m_nCalMode]);
		//break;
		return (BOOL)dwRet;		// 외부 for문 탈출. dwRet는 여기서 무조건 true임.

	}	

	return (BOOL)dwRet;
}

BOOL CCalibDialog::Meas_CheckRange_VbVal(double dAdVb_ReadVal) 
{
	CString	strTemp;
	UINT 	dwVSetVal;
	double	dVbGap, dVbHigh, dVbLow;


	//-------------------------------------------------------------------
	// check1: Vb AD Read값이 60000(NonAd 기준 300 V)이상이면 에러
	
	// 2018.01.24 신부성 차장의견,  250V 보다 큰 Calibration case는 사용하지 말 것. 
	//            300V 보다 큰 케이스의 체크도 250V 이상을 사용하지 않으면 check1은 체크할 필요도 없다. 
	//            151, 152 모드에서 300V VB Set을 하면 check1 코드에 반드시 걸려서 수행이 안 되게 됨.
	// 2018.01.26 김영철부장. 
	//            300V보다 클 수도 있음. check1 불필요. 막을 것
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
		default: // default case는 이미 range check를 하였으므로 논리적으로 존재하지 않음.
			break;		
	}

	// 2018.01.24 HV 일때에만 아래 코드가 효과 있다. LV는 이 체크에 걸리지 않음.
	//            LV일 때에는 dVb 20V보다  크더라도  20.1 이런식이므로 300V보다는 클 수 없다. 
	
	if (dVb > MAX_HV)		// > 300 이면 
	{
		strTemp.Format("Meas_CheckRange_VbVal: dAdVb_ReadVal=%.2f, dVb=%.2f: Range(dVb <= %d) Over! Check HW Power please!.", 
				dAdVb_ReadVal, dVb, MAX_HV );
		ERR.Set(CAL_CHECK_HW, strTemp);
		MyTrace(PRT_LEVEL1, "%s\n", strTemp);
		return FALSE;	
	}
*/

	//-------------------------------------------------------------------
	// check2: Vb AD Read값이 상한(+5%) 하한(-5%) 를 벗어날때 에러
	//         초저전압은 상한(+10%) 하한(-10%)
	
	// VB Read 값이 AD 기준이므로 AD 기준 상태에서 비교한다.
	dwVSetVal = g_saTestCondition[m_nCalMode].nVSet;	// Voltage Set		Col3  // 1~ 300
	UNIT_CONVERT(dwVSetVal, VOLT_TO_DA);	//  ex) 5 -> 15000  (for LV)

	if(g_saTestCondition[m_nCalMode].nVSet < 5){	 // 초저전압일때 
		// ex) nVSet:1V, dwVSetVal: 3000    dVbGap = 3000 *0.10 = 300 
		dVbGap=dwVSetVal*(10*0.01);	// 10%
	}
	else{
		// ex) nVSet:10V, dwVSetVal: 30000   dVbGap = 30000 * 0.05 = 1500 
		dVbGap=dwVSetVal*(5*0.01);	// 5%
	}
	dVbLow = dwVSetVal-dVbGap;		//son VB Read 하한값   ex) dwVSetVal:30000,  dVbLow: 28500 
	dVbHigh= dwVSetVal+dVbGap;		//son VB Read 상한값   ex) dwVSetVal:30000,  dVbHigh: 31500 

	//son VB read 값이 상한이나 하한값에 들지 않는 경우
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

// 고압-> 저압 전환시  VB Out Check
// 이전에 200V Open처럼 고압을 설정했다가 갑자기 5V 4W 같은 저압을 설정하면 문제가 됨.
// 이전에 설정했던 고압상태가 떨어질 때까지 기다리기 위한 기능.
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

	// MM.Get(V)하기 전에 Filter를 VBFT로 선택한다. 
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

		// VB Read: VB OUT 값 확인
		dwAdVb = (UINT)MM.Get(V);		// sGET_VI:  read from IO Card
		if(dwAdVb == (UINT)(-1))	// MM.Get(V) 실패 
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

		// VB Out Check 한 값(dVb) 이 nVSet 값보다  크다면 떨어질 때까지 계속 VB Out을 체크한다.
		//   즉, dVb가 nVSet보다 작거나 같다면 루프를 중단한다. 
		if (dVb <= g_saTestCondition[m_nCalMode].nVSet)
		{
			MyTrace(PRT_LEVEL1, "Meas_VbOutCheck(): dwAdVb=%u, (dVb(%.3f) <= nVSet(%d))? is TRUE(OK). Break VbOutCheck. \n", 
					dwAdVb, dVb, g_saTestCondition[m_nCalMode].nVSet);
			break;
		}
	}

	//---------------------------------
	// 이전 상태 원상복구
	// Meas_VbOutCheck() 수행 이후에 무너진 VB_HL 상태를 원상복구한다.  (Read_Vb() 코드에서 가져옴)
	
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

	// CModeSub71::AOn_VB_CheckOut3_Stop()과 다른 부분!
	// 이 시점에서 Ixud_WriteDOBit(0,10,2,0) 즉, ADC_VB_SEL OFF 도 수행을 하는데, VBFT를 OFF 하는 과정이다. 
	// Meas_VbOutCheck() 완료 이후에 Meas_SetAdcAFilter()를 하면서 자연스럽게 ADC_VB_SEL OFF가 되므로
	// ACE400 original 코드이지만 여기에 굳이 넣지 않았음.

	// I 값이 300 이상이면 Vb OutCheck 결과가 좋지 않으므로 에러를 리턴.
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
						ErrMsg(MB_OK, FALSE); ERR.Reset();  // bTraceOn=FALSE, 에러를 Trace 빼고 메시지박스만 출력
					}		
					MyTrace(PRT_LEVEL1, "%s\n", strTemp);
					m_nHLVC = VCFFT_HVC;
					break;

				default: // default case는 이미 range check를 하였으므로 논리적으로 존재하지 않음.
					break;		
			}
		}
			break;

		case FT_B_VC_SLOW:		// 미사용
			break;
		
		// LVC, HVC  모두 OFF
		case FT_B_HV_OPEN:	case FT_B_4WIRE:
		{
			dwRet = MM.Set(HVC, FALSE);	// HVC LVC를 모두 FALSE 처리 
			strTemp.Format("Meas_SetLvcHvc(): MM.Set(LVC_HVC, FALSE) %s.\n", (dwRet == FALSE)? "Error" : "OK"	);
			if (dwRet == FALSE) {	
				strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
				ERR.Set(ERR.Get(), strTemp2); 
				ErrMsg(MB_OK, FALSE); ERR.Reset();  
			}		// bTraceOn=FALSE, 에러를 Trace 빼고 메시지박스만 출력
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

		case FT_B_VC_SLOW:		// 미사용
			break;

		case FT_B_HV_OPEN:	
			// HV Gain 먼저 설정 -----------------
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

			// HV Open AdcB Filter 설정 ----------
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

	// FT_A_I_FAST 또는 FT_A_I_SLOW 인 경우 I High Gain 설정.
	switch (g_saTestCondition[m_nCalMode].nIFilter)
	{
		case FT_A_I_FAST: 	
		case FT_A_I_SLOW:		
			// I HIGH GAIN ON  :    Short 측정을 위함
			if (g_saTestCondition[m_nCalMode].nIGain == I_HIGH_GAIN_ON)		// 2
			{
				//dwRet = MM.m_pMicroM->On(I_GAIN_CMD);
				dwRet = MM.Set(I_GAIN, _ON);
				strTemp.Format("Meas_SetAdcAFilter(): On(I_GAIN_CMD): I High Gain On. %s.\n", (dwRet == FALSE)? "Error" : "OK");
				if (dwRet == FALSE) {	
					strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
					ERR.Set(ERR.Get(), strTemp2); 
					ErrMsg(MB_OK, FALSE); ERR.Reset();  // bTraceOn=FALSE, 에러를 Trace 빼고 메시지박스만 출력
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

	// ADC A Filter 설정
	switch (g_saTestCondition[m_nCalMode].nIFilter)
	{
		case FT_A_I_FAST: 	
			dwRet = MM.Set(IFFT);					// sSET_ADC_B
			strTemp.Format("Meas_SetAdcAFilter(): MM.Set(ADCA I_FAST Filter) %s.\n", (dwRet == FALSE)? "Error" : "OK");
			if (dwRet == FALSE) {	
				strTemp2.Format("%s\n%s\n", ERR.m_strState, strTemp);
				ERR.Set(ERR.Get(), strTemp2); 
				ErrMsg(MB_OK, FALSE); ERR.Reset();  
			}		// bTraceOn=FALSE, 에러를 Trace 빼고 메시지박스만 출력
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

	// 4W가 아닌 경우(Open, Short)  ---------
	if (g_saTestCondition[m_nCalMode].nTest != TEST_4W)
	{
		// 4W Gain 1로 설정
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

	// 4W 인 경우  --------
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
	CString strTemp2("");	// ErrMsg 출력용

	//---------------------------
	// 1. I Range 설정 
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
	CString strTemp2("");	// ErrMsg 출력용

	//---------------------------
	// 2. Set I Value 

	// CC 설정을 한다.  현재 CV 모드는 없으므로 값이 CV 일 때에는 에러를 출력하고 리턴.
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
	
	// TestCondtion의 nISet 값은 실제값*10 값이므로 /10하여 사용한다.
	double 	dISet = (double) (g_saTestCondition[m_nCalMode].nISet / 10); 

	// SelfTest의 경우에는 I 값을 0으로 해야 한다.
	if (g_sSystem.IsSelfTestCase(m_nCalMode) == TRUE)
		dISet = (double)0;

	double dISetCnvted = dISet;
	switch(g_saTestCondition[m_nCalMode].nIRng)
	{
		case IR1: // 20001~ 200000 uA	 	=> *=(50000/ 200000.))  => *=0.25   *1000하면 *=250	 (ACE400: *=250)
		case IR2: // 10001~ 20000  uA	  	=> *=(50000/ 20000.)	=> *=2.5	*1000하면 *=2500 (ACE400: *=2500)
		case IR3: // 2001 ~ 10000  uA		=> *=(50000/ 10000.)	=> *=5		*1000하면 *=5000 (ACE400: *=2500 ?? error 추정 )
			dISetCnvted *= 1000;		// mA를 uA로 전환. 
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
		ErrMsg(MB_OK, FALSE); ERR.Reset();  // bTraceOn=FALSE, 에러를 Trace 빼고 메시지박스만 출력
	}		
	MyTrace(PRT_LEVEL1, "%s\n", strTemp);


	// Read I Value
	//   : 설정한 I 값을 다시 읽어서 확인한다.  
	//   => 현재는 필요 없음.  추후 MM.Get(VI) 에서 VC와 I 값을 한번에 읽어서 확인
	
	return dwRet;
}
	
// VB Apply
BOOL CCalibDialog::Meas_VbRelayOn() 
{
	UINT 	dwRet = TRUE;
	CString strTemp("");
	CString strTemp2("");

	//---------------------------------
	// 사전 체크 

	// nVBSrc가 HV 인 경우 에러체크
	if (g_saTestCondition[m_nCalMode].nVBSrc == C_HV1 
				|| g_saTestCondition[m_nCalMode].nVBSrc == C_HV2)
	{
		// 4W Test이면서 vbSrc가 HV일 수 없음.  4W에 고압 VB Apply시에 회로손상되므로  H/W Protect 필요.
		// 2017.11.14일 신부성 차장 confirm
		if (g_saTestCondition[m_nCalMode].nTest	== TEST_4W)
		{
			strTemp.Format("Meas_VbRelayOn(): Test Condition Error. nTest=%d(4W), But nVBSrc=%d(HV), Stop for HW_Protect  !.\n",
							g_saTestCondition[m_nCalMode].nTest, g_saTestCondition[m_nCalMode].nVBSrc );
			ERR.Set(CAL_CHECK_TEST_CONDITION, strTemp);
			ErrMsg(); ERR.Reset();  
			return FALSE;
		}

		// vbSrc가 HV인데 LVC가 설정이 돼 있다면 역시 회로손상됨
		// 2017.11.14일 신부성 차장 confirm
		if (m_nHLVC == VCFFT_LVC)
		{
			strTemp.Format("Meas_VbRelayOn(): Check SW Logic Error. m_nHLVC=%d(LVC), But nVBSrc=%d(HV)!.\n",
							m_nHLVC, g_saTestCondition[m_nCalMode].nVBSrc );
			ERR.Set(SW_LOGIC_ERR, strTemp);
			ErrMsg(); ERR.Reset();  
			return FALSE;
		}
	}
	// nVBSrc가 LV 인 경우 에러체크
	else		
	{
		// vbSrc가 LV인데 HVC가 설정이 돼 있다면 오류 처리. 
		// 2017.11.14일 신부성 차장 confirm
		if (m_nHLVC == VCFFT_HVC)
		{
			strTemp.Format("Meas_VbRelayOn(): Check SW Logic Error. m_nHLVC=%d(HVC), But nVBSrc=%d(LV)!.\n",
							m_nHLVC, g_saTestCondition[m_nCalMode].nVBSrc );
			ERR.Set(SW_LOGIC_ERR, strTemp);
			ErrMsg(); ERR.Reset();  
			return FALSE;
		}
	}

	// 4W Test이면서 nVSet값이 20V 초과 고압이면 중단.  4W에 고압 VB Apply시에 회로손상되므로  H/W Protect 필요.
	// 2017.11.15일 신부성 차장 confirm
	if (g_saTestCondition[m_nCalMode].nTest	== TEST_4W
			&& g_saTestCondition[m_nCalMode].nVSet > MAX_LV)
	{
		strTemp.Format("Meas_VbRelayOn(): Test Condition Error. nTest=%d(4W), But nVSet=%d(> %d), Stop for HW_Protect  !.\n",
						g_saTestCondition[m_nCalMode].nTest, g_saTestCondition[m_nCalMode].nVSet, MAX_LV );
		ERR.Set(CAL_CHECK_TEST_CONDITION, strTemp);
		ErrMsg(); ERR.Reset();  
		return FALSE;
	}

	// nVset 값이 300V 초과이면 중단.  for H/W Protect
	// 2017.11.15일 김영필 부장 confirm
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
		ErrMsg(MB_OK, FALSE); ERR.Reset();  	//  bTraceOn=FALSE, 에러를 Trace 빼고 메시지박스만 출력
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
		ErrMsg(MB_OK, FALSE); ERR.Reset();  	//  bTraceOn=FALSE, 에러를 Trace 빼고 메시지박스만 출력
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
// ACE400 OnA_Cal_JIG_Read_151211() 4W R값 측정 부분 참고
BOOL CCalibDialog::Meas_CalcR_4W() 
{
	UINT 	dwRet = TRUE;
	CString strTemp("");
	CString strTemp2("");


	if (g_sSystem.m_nRJigFileNum == -1)		// m_nRJigFileNum이 초기화되지 않은 경우.
	{
		strTemp.Format("Meas_CalcR_4W(): Not Initialized m_nRJigFileNum=%d", g_sSystem.m_nRJigFileNum);
		ERR.Set(SW_NO_INIT_VAR, strTemp);
		ErrMsg(); ERR.Reset();
		return FALSE;
	}

	// RJigFileNum: RJigData s100용인 경우(1), 아닌경우 (0)
	sRJIG_DATA	*p_saRJigData;
	p_saRJigData = (sRJIG_DATA*)g_sSystem.m_saRJigData_4W[g_sSystem.m_nRJigFileNum];

	int		nRJigStart, nRJigEnd;
	g_sSystem.Get_RJigPinRange(m_nCalMode, nRJigStart, nRJigEnd);		// nRJigStart, nRJigEnd Reference Value

	//----------------------------------------------------------------------------------------
	//son JigData 파일 (C:\ACE400\BDL\QC\RJigReadType_S100.txt)-> RJigData1[35][20]에서
	//son JigPin 설정 data를 순서대로 가져와서 Pin 설정하고  ADC R 값을 read하여 R값과 비교한다.
	//son 이 과정을 모든 JigData 파일의 data에 대해 반복하여 수행한다. 
	//----------------------------------------------------------------------------------------

	//--------------------------------------------
	//  모든 JigData에 대해 VC, I 값 측정
	
	// 2018.02.13 dumpNetCount changed for 10. by kim young pil
	// 2019.02.27 김영필 부장 confirm
	//            2W , 4W 모두 10개 Net를 *5회 측정해서 50번을 버리고 다시 0부터 시작.
	int RJigIdx = 0, RJigIdx2 = 0, dumpNetCount = 10, dumpRepeat = 5;	
	for (RJigIdx2=(nRJigStart -dumpNetCount); RJigIdx2 <= nRJigEnd; RJigIdx2++)	// ACE400 i, (nStart+nRCount)에 해당
	{
		// 2018.02.27 김영필부장 confirm.
		// 4W 첫 몇회(dumpNetCount)는 측정하고 버린다. 하나만 여러번 측정하면 열화가 집중적으로 발생하므로
		// dumpNetCount가 10이면 0~9까지의 pinNet를 Read하고 버린다음, 다시 0으로 돌아가서 제대로 측정을 시작. 
		if (RJigIdx2 < nRJigStart)
			RJigIdx = RJigIdx2 + dumpNetCount;
		else
		{
			// 5회 반복이면 RJigIdx를 다시 (nRJigStart - dumpNetCount)로 만드는 건 4번만 하면 된다.
			if (RJigIdx2 == nRJigStart && (dumpRepeat-1) > 0)
			{ 
				// RJigIdx2가 nRJigStart(시작지점)이 되는 순간 -(dumpNetCount+1)을 하고 for문 시작으로 이동하면
				// RJigIdx2++이 되어서 RJigIdex2는 (nRJigStart - dumpNetCount) 상태로 다시 돌아간다.
				RJigIdx2 = nRJigStart - (dumpNetCount +1);
				dumpRepeat--;
				continue;
			}

			RJigIdx = RJigIdx2;
		}

		// 'Stop Measure' 가 감지된 경우 중단하고 리턴
		if ( m_bFlagStopMeas == TRUE)	
		{
			MyTrace(PRT_LEVEL1, "Meas_CalcR_4W() Stop!: RJigIdx=%d, 'Stop Measure' Detected.\n", RJigIdx);
			return FALSE;
		}

		/*
		// OnButtonLoadRef() 부분에서 g_sSystem.InitCalData()로 통째로 초기화하므로 여기에서는 삭제한다. 2017.11.29
		// 측정 이후에 LogCal data 출력시에 아래 초기화 때문에 출력이 안 될 수 있음.
		
		//R값을 측정하기 전에 초기화해 둔다.
		dwRet = g_sSystem.InitCal_RJigData(m_nCalMode, RJigIdx);
		if (dwRet == FALSE) {	
			strTemp.Format("%s\nMeas_CalcR_4W(): g_sSystem.InitCal_RJigData() Error.\n", ERR.m_strState);
			ERR.Set(ERR.Get(), strTemp); 
			ErrMsg(); ERR.Reset();
		}
		*/

		//-------------------
		// 4W Pin 설정.
		
		// Pin 설정하기 전에 이전에 설정된 pin을 Reset. 
		if (Meas_VB_VC_PointAllReset() == FALSE)
		{
			MyTrace(PRT_LEVEL1, "Meas_CalcR_4W(): Meas_VB_VC_PointAllReset() %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");
			continue;
		}

		// 4개의 4W Pin 설정.
		
		// 2018.02.01 김영필 부장 요청으로 설정 순서 조정
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


		// 핀설정 이후 TestCondition에 지정된  delay 수행
		Delay(g_saTestCondition[m_nCalMode].nDelay, usec);


		//---------------------------------
		// VC, I값측정,  R, DiffRate 계산 
		//
		// 1. VC, I 값 측정 (m_nMeasCount만큼 반복), maxMeasR, minMeasR 값 획득
		// 2. avg ADC VC, avg ADC I 계산 
		// 3.1 ADC I를 mA(IR4, IR5는 uA)로 변환
		// 3.2 ADC VC를 Volt로 변환  
		// 3.3 R 계산
		// 4. DiffRate 계산 
		Meas_CalcR_CalData(RJigIdx);		// 2W, 4W 동일 사용
	}


	// 2018.02.05 Pin 케이스마다 계산 직후에 UI에 결과값을 출력하는 것은 진행속도를 너무 느리게 함.
	//            측정 완료후에 한꺼번에 Display하도록 수정.
	DoEvents();
	for (RJigIdx=nRJigStart; RJigIdx <= nRJigEnd; RJigIdx++)	// ACE400 i, (nStart+nRCount)에 해당
	{
		//---------------------------
		// 결과값 화면 Display
		Meas_CalcR_Display(RJigIdx);

	}

	
	return dwRet;	
}

// ACE400 OnA_Cal_JIG_Read_151211() 2W R값 측정 부분 참고
BOOL CCalibDialog::Meas_CalcR_2W() 
{
	UINT 	dwRet = TRUE;
	CString strTemp("");
	CString strTemp2("");

	if (g_sSystem.m_nRJigFileNum == -1)		// m_nRJigFileNum이 초기화되지 않은 경우 체크
	{
		strTemp.Format("Meas_CalcR_2W(): Not Initialized m_nRJigFileNum=%d", g_sSystem.m_nRJigFileNum);
		ERR.Set(SW_NO_INIT_VAR, strTemp);
		ErrMsg(); ERR.Reset();
		return FALSE;
	}

	// RJigData s100용인 경우(1), 아닌경우 (0)
	sRJIG_DATA	*p_saRJigData;
	p_saRJigData = (sRJIG_DATA*)g_sSystem.m_saRJigData_2W[g_sSystem.m_nRJigFileNum];

	int		nRJigStart, nRJigEnd;
	g_sSystem.Get_RJigPinRange(m_nCalMode, nRJigStart, nRJigEnd);		// nRJigStart, nRJigEnd Reference Value

	//----------------------------------------------------------------------------------------
	//son JigData 파일 (C:\ACE400\BDL\QC\RJigReadType_S100.txt)-> RJigData1[35][20]에서
	//son JigPin 설정 data를 순서대로 가져와서 Pin 설정하고  ADC R 값을 read하여 R값과 비교한다.
	//son 이 과정을 모든 JigData 파일의 data에 대해 반복하여 수행한다. 
	//----------------------------------------------------------------------------------------
	
	//int RJigIdx = 0;
	//for (RJigIdx=nRJigStart; RJigIdx <= nRJigEnd; RJigIdx++)	// ACE400 i에 해당
	
	// 2018.02.26 dumpNetCount를 2W의 경우 5회 정보 버리기 위해 추가함.  
	//            이상윤부장 코드 ModeSer13.cpp 1390라인 참고.
	// 2019.02.27 김영필 부장 confirm
	//            2W도 4W와 마찬가지로 10개 Net를 *5회 측정해서 (50번을 버린다.)
	int RJigIdx = 0, RJigIdx2 = 0, dumpNetCount = 10, dumpRepeat = 5;	
	for (RJigIdx2=(nRJigStart -dumpNetCount); RJigIdx2 <= nRJigEnd; RJigIdx2++)	// ACE400 i, (nStart+nRCount)에 해당
	{
		// 2018.02.27 김영필부장 confirm.
		// 2W도 첫 몇회(dumpNetCount)는 측정하고 버린다. 하나만 여러번 측정하면 열화가 집중적으로 발생하므로
		// dumpNetCount가 10이면 0~9까지의 pinNet를 Read하고 버린다음, 다시 0으로 돌아가서 제대로 측정을 시작. 
		if (RJigIdx2 < nRJigStart)
			RJigIdx = RJigIdx2 + dumpNetCount;
		else
		{
			// 5회 반복이면 RJigIdx를 다시 (nRJigStart - dumpNetCount)로 만드는 건 4번만 하면 된다.
			if (RJigIdx2 == nRJigStart && (dumpRepeat-1) > 0)
			{ 
				// RJigIdx2가 nRJigStart(시작지점)이 되는 순간 -(dumpNetCount+1)을 하고 for문 시작으로 이동하면
				// RJigIdx2++이 되어서 RJigIdex2는 (nRJigStart - dumpNetCount) 상태로 다시 돌아간다.
				RJigIdx2 = nRJigStart - (dumpNetCount +1);
				dumpRepeat--;
				continue;
			}

			RJigIdx = RJigIdx2;
		}

		// 'Stop Measure' 가 감지된 경우 중단하고 리턴
		if ( m_bFlagStopMeas == TRUE)	
		{
			MyTrace(PRT_LEVEL1, "Meas_CalcR_2W() Stop!: RJigIdx=%d, 'Stop Measure' Detected.\n", RJigIdx);
			//ERR.Set(NO_ERR, strTemp); 
			//ErrMsg(); ERR.Reset();
			return FALSE;
		}

		/*
		// OnButtonLoadRef() 부분에서 g_sSystem.InitCalData()로 통째로 초기화하므로 여기에서는 삭제한다. 2017.11.29
		// 측정 이후에 LogCal data 출력시에 아래 초기화 때문에 출력이 안 될 수 있음.
		
		//R값을 측정하기 전에 초기화해 둔다.
		dwRet = g_sSystem.InitCal_RJigData(m_nCalMode, RJigIdx);
		if (dwRet == FALSE) {	
			strTemp.Format("%s\n Meas_CalcR_2W(): g_sSystem.InitCal_RJigData() Error.\n", ERR.m_strState);
			ERR.Set(ERR.Get(), strTemp); 
			ErrMsg(); ERR.Reset();
		}
		*/

		//-------------------
		// 2W Pin 설정.
		
		// Pin 설정하기 전에 이전에 설정된 pin 값을 Reset. 
		if (Meas_VB_VC_PointAllReset() == FALSE)
		{
			MyTrace(PRT_LEVEL1, "Meas_CalcR_2W(): Meas_VB_VC_PointAllReset() %s.\n\n",
			(dwRet == FALSE)? "Error" : "OK");
			continue;
		}

		// 2018.02.26 Open Hi Voltage 에서 초기 R값이 하향으로 측정되는 경향을 잡기 위해 
		//            Open Hi Voltage는 Pin 설정 이전에 추가로 300 usec 쉬기로 한다.  
		//            ModeSer13.cpp 1040 라인 이상윤부장 코드에서 가져옴.
		if( g_saTestCondition[m_nCalMode].nVSet >= 100 ){//sylee150630  // 100V 이상일 때
			Delay(300, usec);
		}


		// 2018.02.01 김영필 부장 요청으로 설정 순서 조정
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


		// 핀설정 이후 TestCondition에 지정된  delay 수행
		Delay(g_saTestCondition[m_nCalMode].nDelay, usec);


		//---------------------------------
		// VC, I값측정,  R, DiffRate 계산 
		// 1. VC, I 값 측정 (m_nMeasCount만큼 반복)
		// 2. avg ADC VC, avg ADC I, maxMeasR, minMeasR 값 획득 
		// 3. ADC I를 mA(IR4, IR5는 uA)로 변환
		// 4. ADC VC를 Volt로 변환  
		// 5. R 계산
		// 6. DiffRate 계산 
	    // 7. Check Invalid
		Meas_CalcR_CalData(RJigIdx);

	}

	// 2018.02.05 Pin 케이스마다 계산 직후에 UI에 결과값을 출력하는 것은 진행속도를 너무 느리게 함.
	//            측정 완료후에 한꺼번에 Display하도록 수정.
	DoEvents();
	for (RJigIdx=nRJigStart; RJigIdx <= nRJigEnd; RJigIdx++)	// ACE400 i에 해당
	{

		//---------------------------
		// 결과값 화면 Display
		Meas_CalcR_Display(RJigIdx);
	}


	return dwRet;	
}


// ACE400 CModeSub71::AOnPinSet1(int Type, int No, int nMod) 참고해서 작성.
//
// VbVcType : BPOINT, CPOINT
// gateType	: TR, RY
// pinNo    : 0~ 32767
BOOL CCalibDialog::Meas_CalcR_Set_VbVcPin(int VbVcType, int gateType, int pinNo) 
{
	CString strTemp("");
	CString strTemp2("");

	

	// VbVcType 체크
	if (VbVcType != BPOINT  && VbVcType != CPOINT)
	{
		strTemp.Format("Meas_CalcR_Set_VbVcPin: Invalid VbVcType=%d", VbVcType);
		ERR.Set(INVALID_INPUT, strTemp);
		return FALSE;
	}

	// gateType 체크
	if (gateType != TR  && gateType != RY)
	{
		strTemp.Format("Meas_CalcR_Set_VbVcPin: Invalid gateType=%d", gateType);
		ERR.Set(INVALID_INPUT, strTemp);
		return FALSE;
	}
	
	// Pin Range 체크
	if (pinNo < 0  || pinNo >= MAX_MODEL_PIN)
	{
		strTemp.Format("Meas_CalcR_Set_VbVcPin: pinNo=%d, Range(0<= pinNo < %d) Over!", pinNo, MAX_MODEL_PIN);
		ERR.Set(RANGE_OVER, strTemp);
		return FALSE;
	}

	// 0번 핀이 파일에 존재할 수 있지만 실제로 0번을 Set할 수는 없다. 
	// MM.Set(.., pinNo-1, ..)하면 -1을 Set하게 됨
	if (pinNo == 0)
//		return ERR_SET_VBVCPIN_PIN0;	// error는 아니므로 2를 리턴해서 호출한 함수에서 R값 측정을 skip처리 
//		                             ==> 그냥 에러 리턴하는 것으로 수정함.  복잡성 제거
	{
		strTemp.Format("Meas_CalcR_Set_VbVcPin: pinNo=0, Skip! ");
		ERR.Set(INVALID_INPUT, strTemp);
		return FALSE;
	}
	

	//---------------------------------
	// RJig Pin# -> HW Switch Pin#
	
	int nPin = -1;
//	if (g_sModel.m_bIsConExist == FALSE)	// .CON 파일이 존재하지 않아도 nPinMap은 사용하기로하고  comment처리함.
//   	nPin = pinNo -1;	
//  else
	nPin=g_sModel.nPinMap_SWT[pinNo-1];  

	// Pin Range 체크
	if (nPin < 0  || nPin >= MAX_MODEL_PIN)
	{
		strTemp.Format("Meas_CalcR_Set_VbVcPin: pinNo=%d -> nPin=%d,  Range(0<= nPin < %d) Over!", 
				pinNo, nPin, MAX_MODEL_PIN);
		ERR.Set(RANGE_OVER, strTemp);
		return FALSE;
	}

	//---------------------------------
	// HW 전송 
	
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
// VC, I값측정,  R, DiffRate 계산 
// 1. VC, I 값 측정 (m_nMeasCount만큼 반복) , maxMeasR, minMeasR 값 획득 
// 2. avg ADC VC, avg ADC I 계산
// 3.1 ADC I를 mA(IR4, IR5는 uA)로 변환
// 3.2 ADC VC를 Volt로 변환  
// 3.3 R 계산
// 4. DiffRate 계산 
// 5. Check Invalid
#define DIFF_RATE_LIMIT		5
void CCalibDialog::Meas_CalcR_CalData(int RJigIdx) 
{
	UINT 	dwRet = TRUE;

	//----------------------------------------------------
	// 1. VC, I 값 측정  (m_nMeasCount만큼 반복) , maxMeasR, minMeasR 값 획득

	double 	dSumADI= 0, dAvgADI= 0;		
	double 	dSumADVC= 0, dAvgADVC= 0;		
	double  dMinMeasR= 0, dMaxMeasR=0;
	UINT measIdx;
	for (measIdx=0; measIdx < m_nMeasCount; measIdx++)		// ACE400 j, nCount에 해당
	{

		// VC, I 값 측정 & 개별 R 값 계산
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
	// 1.1 Meas개별 R 계산   
	// 최종 R 값은 AdI, adVC 값으로 평균을 낸 다음 VC/I 해서 계산한다. 
	// 개별 R 단계에서 중요한 것은 AdI, AdVC값이며, 
	// 개별 I, VC, R 값은  MaxR, MinR값을 찾고 csv 파일을 출력하기 위해 부가적으로 추가함.
	//
	
	// 금방 측정한 AdI, AdVc 값을 입력해서 I, Vc, R 값을 계산해 낸다.
	for (measIdx=0; measIdx < m_nMeasCount; measIdx++)		// ACE400 j, nCount에 해당
	{
		Meas_CalcR_makeR(m_nCalMode, 
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dAdI, 	// Input
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dAdVc,	// Input
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dI, 	// Output (Ref므로 pointer를 안 넘김)
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dVc, 	// Output (Ref)
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dR);	// Output (Ref)
		MyTrace(PRT_LEVEL1, "..AdRead_VI(CalMode=%d,RJig=%d,Meas=%d)=> %s\n", 
						m_nCalMode, RJigIdx, measIdx, ERR.m_strState);
		ERR.Reset();
		
		if (measIdx == 0)
		{	// 개별R min, max 초기화
			dMaxMeasR = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dR;	
			dMinMeasR = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dR;	
		}
		else // 개별R min, max 값을 골라낸다.
		{
			// 최대값보다 현재값이 크면 최대값 변경
			if (dMaxMeasR < g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dR)		
				dMaxMeasR = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dR;
			// 최소값보다 현재값이 작으면 최소값 변경
			if (dMinMeasR > g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dR)		
				dMinMeasR = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].sa_MeasData[measIdx].dR;
		}
	}

	//-------------------------------
	// 2. avg ADC VC, avg ADC I 계산
	
	// Avg
	dAvgADI  = dSumADI  / (double)m_nMeasCount;			// m_nMeasCount는 0이 될 수 없음.
	dAvgADVC = dSumADVC / (double)m_nMeasCount;
	g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasAdI_Avg = dAvgADI;
	g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasAdVc_Avg = dAvgADVC;


	//-------------------------------
	// 3. 최종 R 계산 
	double dAvgR = 9999999999.0;
	
	// 계산된 Average  AdI, AdVc 값을 입력받아서  I, Vc, 최종적으로 R 값을 계산해 낸다.
	Meas_CalcR_makeR(m_nCalMode, 
					dAvgADI, dAvgADVC, 		// Input
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasI_Avg, 	// Output (Ref)
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasVc_Avg,	// Output (Ref)
					g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_Avg);	// Output (Ref)
	dAvgR = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_Avg;
	MyTrace(PRT_LEVEL1, "Meas_CalcR_CalData(RJig=%d)=> %s\n", RJigIdx, ERR.m_strState);
	ERR.Reset();


	//---------------------------
	// 4. DiffRate 계산

	// MinR, MaxR    => // DiffRate 판정용. 최종 R값 계산과 관계없음.
	g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_Max = dMaxMeasR;
	g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_Min = dMinMeasR;

	// Meas DiffRate 계산 (Use 판정용 )  
	// dMeasR_MaxDiffRate = (Max - Avg)/Avg : + 오차율  .추후 +5%보다 크면 별도 표기(nValid=False). TBD
	// dMeasR_MinDiffRate = (Min - Avg)/Avg : - 오차율  .추후 -5%보다 작으면 별도 표기(nValid=False). TBD 
	double 	dMeasR_MaxDiffRate = 0, dMeasR_MinDiffRate = 0;
	if (dAvgR != 0)	 // check divide by zero
	{
		dMeasR_MaxDiffRate = ((dMaxMeasR - dAvgR) / dAvgR) * 100;		// % 값
		dMeasR_MinDiffRate = ((dMinMeasR - dAvgR) / dAvgR) * 100;		
	}
	g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_MaxDiffRate = dMeasR_MaxDiffRate;
	g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_MinDiffRate = dMeasR_MinDiffRate;


	//---------------------------
	// 5. Check Invalid

	// 4W는 mode마다 체크하는 range가 정해져 있으므로 range 외에서는 에러가 나더라도 에러처리할 필요가 없음.
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

		// 현재 OrgR이 해당모드의 range에 맞는 저항이라면 Invalid 체크를 한다.
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
// VC, I 값 측정 & R 값 계산
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
		// I값, VC값 측정

		dwVal[adCntIdx] = MM.Get(VI, TRUE);		
		UINT nAdIVal  = dwVal[adCntIdx] & 0xffff;
		UINT nAdVCVal = dwVal[adCntIdx] >>16 & 0xffff;

#ifdef  __AD_RAW_DATA__
		// raw data 저장.  (출력용)
		g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasRawData[nMeasId][adCntIdx].nAdI = nAdIVal;
		g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasRawData[nMeasId][adCntIdx].nAdVc = nAdVCVal;
#endif

		// 2018.02.12  AdRead 속도가 너무 느려서 아래 프린트 문을 MySnprintf로 변경. print off이면 string 생성하지 않고 리턴 
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
		else //min, max 값을 골라낸다.
		{
			// I
			if (dwMaxI < nAdIVal)		// 최대값보다 현재값이 크면 최대값 변경
			{
				dwMaxI2 = dwMaxI;
				dwMaxI = nAdIVal;
			}
			if (dwMinI > nAdIVal)		// 최소값보다 현재값이 작으면 최소값 변경
				
				dwMinI = nAdIVal;

			// VC
			if (dwMaxVC < nAdVCVal)		// 최대값보다 현재값이 크면 최대값 변경
				dwMaxVC = nAdVCVal;
			if (dwMinVC > nAdVCVal)		// 최소값보다 현재값이 작으면 최소값 변경
				dwMinVC = nAdVCVal;
		}
		dwSumI += nAdIVal;	
		dwSumVC += nAdVCVal;	

	}

	// Avg 계산
	double dAdI=0, dAdVc=0;
	if (nAdCount > 3) {
		dAdI  = (dwSumI - dwMaxI - dwMinI) / (nAdCount -2);  // min, max 제외하고 (adCount-2)회로 나눈 평균값
		dAdVc = (dwSumVC - dwMaxVC - dwMinVC) / (nAdCount -2);  
	}
	else {
		dAdI = dwSumI / nAdCount;  // min max 상관없이 adCount로 평균값
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

// 2018.02.21  평균값이 아니라 중앙값(Median)으로 AdRead값의 대표값을 선정한다.
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
		// I값, VC값 측정

		dwVal[adCntIdx] = MM.Get(VI, TRUE);		
		dwAdIVal[adCntIdx]  = dwVal[adCntIdx] & 0xffff;
		dwAdVCVal[adCntIdx] = dwVal[adCntIdx] >>16 & 0xffff;

#ifdef  __AD_RAW_DATA__
		// raw data 저장.  (출력용)
		g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasRawData[nMeasId][adCntIdx].nAdI = dwAdIVal[adCntIdx];
		g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasRawData[nMeasId][adCntIdx].nAdVc = dwAdVCVal[adCntIdx];
#endif

		// 2018.02.12  AdRead 속도가 너무 느려서 아래 프린트 문을 MySnprintf로 변경. print off이면 string 생성하지 않고 리턴 
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
	// AdIVal과 AdVcVal의 Median 계산  
	double dAdI=0, dAdVc=0;

	// Max 200개를 정렬하지 않고 nAdCount갯수만큼만 정렬해서 사용한다.
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

// 2018.02.23  김영식이사 요청
// 평균값이 아니라 Centered Moving Avg값(CMAvg)으로 AdRead값의 대표값을 선정한다.
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
		// I값, VC값 측정

		dwVal[adCntIdx] = MM.Get(VI, TRUE);		
		dwAdIVal[adCntIdx]  = dwVal[adCntIdx] & 0xffff;
		dwAdVCVal[adCntIdx] = dwVal[adCntIdx] >>16 & 0xffff;

#ifdef  __AD_RAW_DATA__
		// raw data 저장.  (출력용)
		g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasRawData[nMeasId][adCntIdx].nAdI = dwAdIVal[adCntIdx];
		g_sSystem.m_saCalData[nCalMode][nRJigId].sa_MeasRawData[nMeasId][adCntIdx].nAdVc = dwAdVCVal[adCntIdx];
#endif

		// 2018.02.12  AdRead 속도가 너무 느려서 아래 프린트 문을 MySnprintf로 변경. print off이면 string 생성하지 않고 리턴 
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
	// AdIVal과 AdVcVal의 CM(Centered Moving) Avg 계산  
	double dAdI=0, dAdVc=0;
	double dAdSumI = 0, dAdSumVc = 0;

	// 4개씩 정렬해서 가운데 2개만 평균낸 값(min, max 제외 중앙 2개 avg)을 구한다음(size 4로 Median 수행), 
	// 그걸 다시 평균낸다.  item 끝까지 수행해야 하므로  n-3회 만큼 반복 (10개 item이라면 7회 loop) 하여 
	// Sum 한 후에  1/(n-3) 수행하여 Average를 계산.
	//
	// ex1)
	// item0   item1   item2  item3   item4   item5  item6  item7  item8  item9
	//  282     275     272    275     275     279    276    274    274   277          
	//                        citem3  citem4  citm5  citem6 citem7 citem8 citem9  
	//                         (0~3)  (1~4)   (2~5)   (3~6) (4~7)  (5~8)  (6~9)
	//                         275     275     275    275.5  275.5  275    275   <= min,max 제외하고 center 2개 AVg
	//                                           => 7개 citem avg는 275.1429
	// ex2)
	// item0   item1   item2  item3   item4   item5  item6  item7  item8  item9
	//  350	    349	    355	   351     351	   354	  351	 348    356	   347
	//                        citem3  citem4  citm5  citem6 citem7 citem8 citem9  
	//                         (0~3)  (1~4)   (2~5)   (3~6) (4~7)  (5~8)  (6~9)
	//                        350.5    351 	  352.5   351 	 351 	352.5  349.5 <= min,max 제외하고 center 2개 AVg
	//                                       => 7개 citem avg는 351.1429
	
	
	for (adCntIdx=3; adCntIdx < nAdCount; adCntIdx++)	// (nAdCount - 3) 회 반복
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
	size_t center = arraySize / 2;// 요소 개수의 절반값 구하기
	if (arraySize % 2 == 1) { 	// 요소 개수가 홀수면
		return array[center]; 		// 홀수 개수인 배열에서는 중간 요소를 그대로 반환
	} 
	else {
		return (array[center - 1] + array[center]) / 2.0; // 짝수 개 요소는, 중간 두 수의 평균 반환
	}
}

// qsort 내부에서 사용되는 비교 함수
int	CompareDouble(const void *a, const void *b) 
{
	if (*(double*)a <  *(double*)b) return -1;
	if (*(double*)a == *(double*)b) return  0;

	return 1;
}
*/
double	CCalibDialog::GetMedian(int* array, size_t arraySize) 
{
	size_t center = arraySize / 2;// 요소 개수의 절반값 구하기
	if (arraySize % 2 == 1) { 	// 요소 개수가 홀수면
		return array[center]; 		// 홀수 개수인 배열에서는 중간 요소를 그대로 반환
	} 
	else {
		return (double)(array[center - 1] + array[center]) / 2.0; // 짝수 개 요소는, 중간 두 수의 평균 반환
	}
}

// qsort 내부에서 사용되는 비교 함수
int	CompareDouble(const void *a, const void *b) 
{
	if (*(int*)a <  *(int*)b) return -1;
	if (*(int*)a == *(int*)b) return  0;

	return 1;
}

double	CCalibDialog::GetMedian2(int* array, size_t arraySize) 
{
	size_t center = arraySize / 2;// 요소 개수의 절반값 구하기
	if (arraySize > 5)
	{
		if (arraySize % 2 == 1) { 	// 요소 개수가 홀수면 가운데 3개로 평균
			return (double)(array[center -1] + array[center] +array[center +1]) / 3.0; 
		}
		else {						// 요소 개수가 짝수면 가운데 4개로 평균
			return (double)(array[center -2] + array[center -1] + array[center] +array[center +1]) / 4.0; 
		}
	}

	if (arraySize % 2 == 1) { 		// 요소 개수가 홀수면
		return array[center]; 		// 홀수 개수인 배열에서는 중간 요소를 그대로 반환
	} 
	else {
		return (double)(array[center - 1] + array[center]) / 2.0; // 짝수 개 요소는, 중간 두 수의 평균 반환
	}
}

// AdI, AdVc 값을 입력받아서 I, Vc, R 값을 계산해 낸다.
// Input : nCalMode, dAdI, dAdVc    
// Output: rdI, rdVc, rdR    (Reference Type)
void CCalibDialog::Meas_CalcR_makeR(int nCalMode, double dAdI, double dAdVc, 
									double& rdI, double& rdVc, double& rdR) 
{
	CString strTemp;

	//----------------------------
	// A. Short R 계산

	// 2017.11.29일 김영필 부장 confirm.
	// Short 인 경우는 ADC I 값을 그냥 R 저항값으로 사용한다. 
	// Open의 경우는 정전류로 VC 변화를 측정해서 R이 변화하지만 
	// Short는 정전압이고, 전류의 변화를 측정해야 한다. 
	// Short는 측정 VC값이 무의미할 정도로 작아서 VC/I 로 저항계산하는 것이 어려우므로 
	// 그냥 ADC I 값을 저항수준을 측정하는데 사용.
	//if (g_sSystem.IsShortTestCase(nCalMode) == TRUE) 
	if (g_saTestCondition[nCalMode].nTest == TEST_SHORT)
	{
		rdR  = dAdI;
		rdVc = 0;
		rdI  = 0;

		// 에러는 아니지만 호출위치로 아래 string을 넘기기 위해 잠시 ERR.m_strState를 사용.
		strTemp.Format("..makeR(dAdI:%.3f, dAdVc:%.3f): CalMode is Short! Use dAdI as rdR=%.3f, rdVc=0, rdI=0\n",
				dAdI, dAdVc, rdR);
		ERR.Set(TEST_TRACE, strTemp);
		return;
	}

	//----------------------------
	// B. R 계산   
	
	// 1. ADC I 값을 mA(또는 uA)로 변환. sa_MeasData[nMeasId].dI에 저장
	double dAdI2 = dAdI, dI= 0;
	dI = UNIT_CONVERT(dAdI2, AD_TO_I0+ (g_saTestCondition[nCalMode].nIRng -1) -I_RANGE0);
	rdI = dI;		// Reference dI 값으로 리턴을 위해


	// 2. ADC VC 값을 Volt로 변환. dMeasVc_Avg 에 저장
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
			default: // default case는 이미 range check를 하였으므로 논리적으로 존재하지 않음.
				break;		
		}
	}
	rdVc = dVc;


	// 3. R 값 계산
	
	double dR = 9999999999.0;

#ifdef __NEW_CAL__
	// 2017.11.22 김영필 부장 confirm. 
	// 1) VB read 값은 4W, Open, HvOpen, Short 모두 계산식에서 사용할 필요 없음.
	//     VB-VC 필요 없이 read 값은 에러체크에서만 사용해라.
	//     고압open 에서는  VB-VC를 빼서 VC로 주고 있으며, 저압 open 의 경우에도 필요 없다. 
	//    
	// 2) ADC_B 필터가 HVO, 4W인 경우는 I 값을 고정해서 사용. read값 사용하지 마라.
	//     I값 설정하고 read 해서 흔들리는 지 여부만 에러체크하면 된다.   => 이 부분은 현재 구현된 바가 없음. 어떻게???
	//
	
	//if (dI > 0)		
	{
		if (dI < 0.0000001) // check divide by zero
			rdI = dI = 0.0000001;

		// 2018.01.29  김영필부장님 요청대로 했을 때, 
		//             HV_OPEN 케이스는 (mode 16) original ACE400 의 1/5 정도로 R 값이 작게 측정이 된다. 이게 맞을지? 
		if (g_saTestCondition[nCalMode].nFilter == FT_B_HV_OPEN )
		{
			// nISet 값은 이미 range 체크로 0이 아님을 확인함.   
			// nISet은 *10값이므로 계산시에는 /10을 해서 적용필요
			rdI = dI = (double)(g_saTestCondition[nCalMode].nISet/ 10);		// read한 I를 고정값으로 변경
			dR = dVc / dI ;		// R =   VC / I (고정값) 

			if (dVc > 325)		//sylee150126    AD VC값이 65535이상일때 에러 처리
				dR = 9999999999.0;
		}
		else if (g_saTestCondition[nCalMode].nFilter == FT_B_4WIRE )
		{
			// nISet은 *10값이므로 계산시에는 /10을 해서 적용필요. 
			// 단, 4W일 경우는 *10배.
			rdI = dI = (double)(g_saTestCondition[nCalMode].nISet/ 10);
			dR = (dVc / dI) * 10;	// R =   VC / I (고정값) *10
		}
		else // ADCB_VCFAST:  LV 2W, LV Short
		{

			dR = dVc / dI;			// 저항값 =   VC / I (Read값) 
		}
	}

#else
	// 2018.02.07 이상윤부장 코드 기준
	// 			  1. ACE400 이상윤 부장 코드에는 일단 4W인데 I Read값이 +10%, -10% 안쪽이면  VC/ISet, I Read값이 벗어나면  VC/I ,
	//            2. ADCB 필터가 2W HVO 일때에는 VC/I,  2W이지만 HVO가 아닐때에는  (VB-VC)/ I를 하고 있음.
	// 2017.03.02 김영필 부장 요청
	//            3. 4W, Open (Short 제외) 일때, I Read 값이 +1%, -1% 안쪽이면 VC/ISet, I Read값이 벗어나면 VC/I를 해달라.
	
	double	dIHigh, dILow, dISet;
	dISet = (double)(g_saTestCondition[nCalMode].nISet / 10);

	if (dI < 0.0000001)			// check divide by zero
		rdI = dI = 0.0000001;

	if ( g_saTestCondition[nCalMode].nTest == TEST_4W)
	{
		dILow  = dISet-dISet*0.1;	// 10% 하한 
		dIHigh = dISet+dISet*0.1;	// 10% 상한

		// 1. read한 I 값이   10%하한 < readI  < 10%상한  범위 이내라면 nISet 값을 I값으로 사용하여 R을 계산한다.
		if ((dILow < dI ) &&  (dI < dIHigh)) 
			rdI = dI = dISet;

		// R =   VC / I (Read값)   : 2018.01.29 일단 ACE400 원본 기준으로 Test  
		// 4W일 경우는 VC/I의 *10배.
		dR = (dVc / dI) * 10;		// R =   VC / I ( read 값)  *10  //161207-0 x10    ray ok

	}

	else // Open, Short 
	{
		dILow  = dISet-dISet*0.01;	// 1% 하한 
		dIHigh = dISet+dISet*0.01;	// 1% 상한

		if (g_saTestCondition[nCalMode].nFilter == FT_B_HV_OPEN )	// HV Open
		{
#if 1
			// 2. Open인 경우 I Read 값이 +1%, -1% 안쪽이면 VC/ISet, 아니면 VC/I
			if ((g_saTestCondition[nCalMode].nTest == TEST_OPEN) 
					&& ((dILow < dI ) &&  (dI < dIHigh)))
				rdI = dI = dISet;
#endif
			// R =   VC / I (Read값)   : 2018.01.29 일단 ACE400 원본 기준으로 Test  
			dR = dVc / dI;				// R =   VC / I ( read 값) 


			if (dVc > 325)		//sylee150126    AD VC값이 65535이상일때 에러 처리
				dR = 9999999999.0;
		}

		// ADCB_VCFAST: LV Open, Short
		else	
		{
#if 1
			// 3. Open인 경우 I Read 값이 +1%, -1% 안쪽이면 (VB-VC)/ISet, 아니면 (VB-VC)/I
			if ((g_saTestCondition[nCalMode].nTest == TEST_OPEN) 
					&& ((dILow < dI ) &&  (dI < dIHigh)))
				rdI = dI = dISet;
#endif
			// R =   (VB-VC) / I (Read값)   : 2018.01.22 일단 ACE400 원본 기준으로 Test  
			dR = (g_sSystem.m_dVb[nCalMode] - dVc) / dI;			
		}
	}
#endif



	//2017.11.24      ModeSer13.cpp 1472 라인 근처
	// ACE400 이상윤 부장 코드.  2W일 때 계산한 R 값에  IR1, IR2는 *1000,   IR3, IR4, IR5는 *1000000을 함. 
	// 4W 일 때에는 IR1, IR2는 *10000, IR3, IR4, IR5는 *1000000. 
	if (dR != 9999999999)
	{
		switch(g_saTestCondition[nCalMode].nIRng)
		{
			case IR1: case IR2: 
				dR *= 1000.;		// Ohm-> mOhm 단위로 변환 
				break;

			case IR3: case IR4: case IR5:
				dR *= 1000000.;		// Ohm-> uOhm 단위로 변환 
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

	// 1. 오차율 범위를 벗어나면 Invalid. 
	if ((g_sSystem.m_saCalData[nCalMode][nRJigId].dMeasR_MaxDiffRate > DIFF_RATE_LIMIT)				// > 5%
			|| ((-g_sSystem.m_saCalData[nCalMode][nRJigId].dMeasR_MinDiffRate) > DIFF_RATE_LIMIT) )	// < -5%
		g_sSystem.m_saCalData[nCalMode][nRJigId].nValid = FALSE;
	else
		g_sSystem.m_saCalData[nCalMode][nRJigId].nValid = TRUE;



	// 2. 저항이 증가하면 R이 감소해야함. R값이 이전 nRJigId에 비해 감소한 경우에도 Invalid. 
	int		nRJigStart, nRJigEnd;
	g_sSystem.Get_RJigPinRange(nCalMode, nRJigStart, nRJigEnd);		// nRJigStart, nRJigEnd Reference Value
	if ((nRJigId == 0)				// nRJigId=0이면 nRJigId-1은 -1이므로 메모리 에러 유발하므로 제외
	    || (nRJigId == nRJigStart))	// 첫번째 항목은 비교대상이 없으므로 제외
	    return;

	if (g_saTestCondition[nCalMode].nTest != TEST_SHORT)	// Open, 4W
	{
		// 감소체크
		if (g_sSystem.m_saCalData[nCalMode][nRJigId-1].dMeasR_Avg > g_sSystem.m_saCalData[m_nCalMode][nRJigId].dMeasR_Avg) 
			g_sSystem.m_saCalData[nCalMode][nRJigId].nValid = FALSE;
	}

	// 3. Short는 감소가 정상이므로 거꾸로 증가할 때,  Invalid처리한다.
	else 	// SHORT
	{
		// 증가체크
		if (g_sSystem.m_saCalData[nCalMode][nRJigId-1].dMeasR_Avg < g_sSystem.m_saCalData[m_nCalMode][nRJigId].dMeasR_Avg) 
			g_sSystem.m_saCalData[nCalMode][nRJigId].nValid = FALSE;
	}


}

// Calibration CalcR 결과값 화면 Display
void CCalibDialog::Meas_CalcR_Display(int RJigIdx) 
{
	CString strTemp;

/*
 *  2018.02.26: 잘 안됨. 삭제
 *  전체적으로 내부 셀의 글자색이  빨간색으로 바뀌기만 하고 특정 라인만 글자색을 빨간색으로 설정할 수는 없음.
 *  m_gridCalResult.SetForeColorFixed(RGB(200, 0, 20)) 의 경우는 가로, 세로 제목행, 제목열만 글자색이 바뀜.

	if (g_sSystem.m_saCalData[m_nCalMode][RJigIdx].nValid == FALSE)
		m_gridCalResult.SetForeColor(RGB(200, 0, 20));		// Bad인 경우 붉은색으로 설정.
	else
		m_gridCalResult.SetForeColor(RGB(0, 0, 0));			// Good인 경우 검정색으로 설정.
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

	for (UINT measIdx=0; measIdx < m_nMeasCount; measIdx++)		// ACE400 j, nCount에 해당
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

// Calibration CalcR 결과값 CSV 파일 출력 
void CCalibDialog::Meas_CalcR_FileOut() 
{

	if (g_sSystem.Save_LogCalData(m_nCalMode, m_nMeasCount) == FALSE)
	{ 	ErrMsg(); ERR.Reset(); return; }

}

//---------------------------------
// OnButtonChgRef 관련 함수
//---------------------------------
void CCalibDialog::OnButtonChgRef() 
{
	// TODO: Add your control notification handler code here
	//
	
	// 현재 측정된 MeasR, MeasAdVc, MeasAdI 값을 Ref 값으로 변경한다.
	int 	RJigIdx;
	int		nRJigStart, nRJigEnd;
	g_sSystem.Get_RJigPinRange(m_nCalMode, nRJigStart, nRJigEnd);		// nRJigStart, nRJigEnd Reference Value
	for (RJigIdx = nRJigStart; RJigIdx <= nRJigEnd; RJigIdx++)
	{
		g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dRefR_Avg = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasR_Avg;
		g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dRefVcVal = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasAdVc_Avg;
		g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dRefIVal  = g_sSystem.m_saCalData[m_nCalMode][RJigIdx].dMeasI_Avg; 
	}
	
	// 변경한 RefCalData를 CalResult Grid에 출력한다.
	SetGrid_RefCalData(m_nCalMode);
}

//---------------------------------
// ButtonSaveRef 관련 함수
//---------------------------------
void CCalibDialog::OnButtonSaveRef() 
{
	// TODO: Add your control notification handler code here
	
	// 현재 RefCalData를  정해진 file 위치에 save한다.
	if (g_sSystem.Save_CalData(m_nCalMode) == FALSE)
	{ 	ErrMsg(); ERR.Reset(); return; }
}

//---------------------------------
// ButtonClrScreen 관련 함수
//---------------------------------
void CCalibDialog::OnButtonClrScreen() 
{
	ClearGrid_CalResult();
}

void CCalibDialog::ClearGrid_CalResult()	
{
	// TODO: Add your control notification handler code here
	
	int i, j; 
	
	for (i = 1; i < (MAX_2W_RJIG+1); i++)	// 헤더는 제외하고 clear
		for (j = 0; j < NUM_CAL_RESULT_COL; j++)
			m_gridCalResult.SetTextMatrix(i, j, "           ");
}

//---------------------------------
// ButtonStopMeas 관련 함수
//---------------------------------
void CCalibDialog::OnButtonStopMeas() 
{
	// TODO: Add your control notification handler code here
	
	m_bFlagStopMeas 	= TRUE;
	m_bFlagStopAutoCal 	= TRUE;
}

//---------------------------------
// ButtonViewResultFile 관련 함수
//---------------------------------
void CCalibDialog::OnButtonViewResultFile() 
{
	// TODO: Add your control notification handler code here
	
	CString strTemp;
	
	// 2017.11.30  Output 파일을 ACE400의 위치에 overwrite하지 않고 자체적으로 보관하기로 함.
	// ACE400 LogCal 디렉토리대신 ACE400Cal/Measure 디렉토리 사용.
    //strTemp.Format("%s\\001LOGRCal%d.csv",g_sFile.ACE400_LogCalDataDir, (nCalMode +1) );
    strTemp.Format("%s\\001LOGRCal%d.csv",g_sFile.MeasureDir, (m_nCalMode +1) );

	::ShellExecute(NULL,"open","EXCEl.EXE",strTemp,"NULL",SW_SHOWNORMAL);	
	
}

//---------------------------------
// ButtonAutoCalib 관련 함수
//---------------------------------
void CCalibDialog::OnButtonAutoCalib() 
{
	m_bFlagStopAutoCal = FALSE; 
	UINT	dwRet = TRUE;

	// TODO: Add your control notification handler code here
	
	// 1. 4W Test 먼저 수행
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

	// 2. Open Test 수행
	for (nCalMode=0; nCalMode < MAX_TEST_MODE; nCalMode++)
	{
		if (g_saTestCondition[nCalMode].nTest == TEST_OPEN)
		{
			dwRet = Auto_Calib_Mode(nCalMode);
			if (dwRet == FALSE)		// Auto Calibration Stop Detected.
				return;
		}

	}

	// 3. Short(Leak) Test 수행
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

	// 현재 Test 전압 모드(nCalMode) 설정
	m_comboCalMode.SetCurSel(nCalMode);	

	//-------------------------------
	// 1. Load TestCondition
	// 해당 nCalMode의 TestCondition 로드 
	OnSelchangeComboCalibMode();

	//-------------------------------
	// 2. Load RJigData, PreCalData
	// 해당 nCalMode의 RJigData(Net Pin Data), PrevCalData 로드 
	OnButtonLoadRef(); 
	DoEvents(); 

#if 0
	ERR.Set(TEST_ERR, "test");
	ErrMsg(); ERR.Reset(); 
	DoEvents(); 
#endif

	//-------------------------------
	// 3. Calibration Measure 수행 
	OnButtonStartMeas();

	// 'Stop AutoCal' 이  감지된 경우 중단하고 리턴
	DoEvents();
	if ( m_bFlagStopAutoCal == TRUE)	
	{
		strTemp.Format("OnButtonAutoCalib() Stop!: nCalMode=%d, 'Stop AutoCal' Detected.\n", nCalMode);
		ERR.Set(TEST_ERR, strTemp);
		ErrMsg(); ERR.Reset(); 
		return FALSE;
	}
	
	//-------------------------------
	// 4. Change Reference 수행
	// => 추후 Save Reference가 확인이 완료된 시점에 풀어서 사용한다.
	//   H/W 연동 시험해서 안정화 이후.
	//OnButtonChgRef(); 


	//-------------------------------
	// 5. Save Reference 수행
	// => 추후 Save Reference가 확인이 완료된 시점에 풀어서 사용한다.
	//   H/W 연동 시험해서 안정화 이후.
	// OnButtonSaveRef(); 		
	DoEvents(); 
	Delay(400, msec);

	return TRUE;
}

//-----------------------------------
// IDC_EDIT_MEAS_CNT  관련 함수
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

