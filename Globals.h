// Globals.h: interface for the CGlobals class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALS_H__FDA16379_4221_4970_8471_B8FBD39D2C1E__INCLUDED_)
#define AFX_GLOBALS_H__FDA16379_4221_4970_8471_B8FBD39D2C1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <direct.h>
#include <io.h>
#include "stdafx.h"
#include <stdarg.h>



class CGlobals  
{
public:
	CGlobals();
	virtual ~CGlobals();

};



///////////////
// 글로벌 변수 
///////////////

enum UNIT			{ sec=0, msec=1, usec=2, mA, uA, Mohm, Kohm, ohm, mohm, uohm };// sec=0, msec=1, usec=2 고정
enum UNIT_CONVERT_TYPE	{ 
	MM_TO_UM, UM_TO_MM, SEC_TO_MSEC, MSEC_TO_SEC,
	VOLT_TO_DA, I0_TO_DA, I1_TO_DA, I2_TO_DA, I3_TO_DA, I4_TO_DA, /*I5_TO_DA,*/ I6_TO_DA,
	AD_TO_LVOLT, AD_TO_HVOLT, AD_TO_I0, AD_TO_I1, AD_TO_I2, AD_TO_I3, AD_TO_I4, AD_TO_I5, AD_TO_I6,
	VOLT_TO_DA_SPARK, AD_TO_4W_VOLT
};

enum PRT_TYPE { PRT_BASIC, PRT_LEVEL1, PRT_LEVEL2, PRT_LEVEL3, PRT_DEVICE, PRT_DEV_D64 };
extern WORD	g_wPrintState;


///////////////////
// 글로벌 스트럭쳐
///////////////////

typedef struct sFILE_DATA
{
	CString	SystemDir;					// ACE400Cal 프로그램을 수행한 디렉토리
	CString ParameterDir;
	CString MeasureDir;					// "C:\\ACE400\\SETUP\\LOGCAL"를 대체하여 사용
//	CString ProductDir;
//	CString ModelDir;
//	CString WorkDir;

	CString ACE400_FilePathJigName;		// ACE400의 FilePathJig name ("C:\\ACE400\\SETUP\\FilePathJig.ini") 
										//     BDL 파일의 path, name 정보를 획득
	CString ACE400_TestConditionPath;	// ACE400의 dRec[m_nMode][16] 정보 위치("C:\\ACE400\\SETUP\\SysSet21_ACE400.ini") 
	CString ACE400_4wRangePath;	        // ACE400의 dRec4w[][] 정보 위치("C:\\ACE400\\SETUP\\SysSet21_4W.ini") 
										// 171   dRec4w[1][3]  <= SysSet21_4W.ini  첫번째 항목   ex) 80    1~80     mohm은 171
										// 172   dRec4w[2][3]  <= SysSet21_4W.ini  두번째 항목   ex) 800   81~800   mohm은 172
										// 173   dRec4w[3][3]  <= SysSet21_4W.ini  세번째 항목   ex) 5000  801~5000 mohm은 173
										// 174   dRec4w[4][3]  <=      ex) 50000   5001 ~ 50000   mohm은 174
										// 175   dRec4w[5][3]  <=      ex) 500000  50001 ~ 500000 mohm은 175


//	CString ACE400_RJigData_2WPath1;	// ACE400의 RJigData1[32][18] 정보 위치(".\\Parameter\\RJigData_2W.TXT") 
//	CString ACE400_RJigData_4WPath1;	// ACE400의 RJigData1[32][18] 정보 위치(".\\Parameter\\RJigData_4W.TXT") 
//	CString ACE400_RJigData_2WPath2;	// ACE400의 RJigData1[32][18] 정보 위치(".\\Parameter\\RJigData_S100_2W.TXT") 
//	CString ACE400_RJigData_4WPath2;	// ACE400의 RJigData1[32][18] 정보 위치(".\\Parameter\\RJigData_S100_4W.TXT") 

	CString ACE400_RJigData_Path1;		// ACE400의 RJigData1[32][18] 정보 위치("C:\\ACE400\\BDL\\QC\\RJigReadType.TXT") 
	CString ACE400_RJigData_Path2;		// ACE400의 RJigData1[32][18] 정보 위치("C:\\ACE400\\BDL\\QC\\RJigReadType_S100.TXT") 

	CString ACE400_CalDataDir;			// ACE400의 Calibarion 결과 data Dir ("C:\\ACE400\\SETUP\\CAL");
	CString ACE400_LogCalDataDir;		// ACE400의 Calibarion 결과 Log data Dir ("C:\\ACE400\\SETUP\\LOGCAL");
	
	CString ACE400_SysInfoPath;			// ACE400의 SystemType(QD, DD, SD) 정보 위치 ("C:\\ACE400\\SETUP\\SysSet01.ini")
	CString ACE400_PinSetupPath;		// ACE400의 PinSetup 정보 위치 ("C:\\ACE400\\SETUP2\\SysSetPin.ini")

	char ACE400_BdlFilePath[200];		// ACE400 BDL Path(Dir+name) SysInfoView01.m_pStrFilePathJig2 에 해당
										//  ex) C:\ACE400\QC-JIG-S100-BHFlex\4W_YP_S100\OPEN-READ.BDL
	char ACE400_BdlFileDir[200];		//  ex) C:\ACE400\QC-JIG-S100-BHFlex\4W_YP_S100 
	char ACE400_BdlFileName[200];		//  ex) OPEN-READ.BDL 

	CString SystemFileName; 
//	CString ModelFolderName;
//	CString ModelFileName;
//	CString MotionFileName;
//	CString DAQFileName;
	CString ResultFileName;
	CString MeasureFileName;
	//CString _2WCalibFileName;
	CString _2WCalibNetFileName;
	
	CString SystemFile; 
//	CString ModelFile;
//	CString MotionFile;
//	CString DAQFile;
//	CString ResultFile;
//	CString MeasureFile;
//	CString _2WCalibFile;
//	CString _2WCalibNetFile;

//	CString CurDir;
//	CString CurefFullPath;

	sFILE_DATA()
	{
		SystemFileName		= "System.";

		ACE400_FilePathJigName = "C:\\ACE400\\SETUP\\FilePathJig.ini";

		ACE400_TestConditionPath = "C:\\ACE400\\SETUP\\SysSet21_ACE400.ini";
		ACE400_4wRangePath       = "C:\\ACE400\\SETUP\\SysSet21_4W.ini";
		ACE400_RJigData_Path1 = "C:\\ACE400\\BDL\\QC\\RJigReadType.TXT";
		ACE400_RJigData_Path2 = "C:\\ACE400\\BDL\\QC\\RJigReadType_S100.TXT";
		ACE400_CalDataDir       = "C:\\ACE400\\SETUP\\CAL";
		ACE400_LogCalDataDir    = "C:\\ACE400\\SETUP\\LOGCAL";

		ACE400_SysInfoPath   	= "C:\\ACE400\\SETUP\\SysSet01.ini";
		ACE400_PinSetupPath   	= "C:\\ACE400\\SETUP2\\SysSetPin.ini";
	}

	void Init()
	{
		// 현재 ACE400Cal을 실행한 디렉토리 위치를 가져온다.
		char buffer[_MAX_PATH];
		if(_getcwd( buffer, _MAX_PATH ) == NULL)
		{
			AfxMessageBox("Root directory not found.");
			return;
		}

		SystemDir.Format("%s", buffer);
		ParameterDir	= SystemDir + "\\" + "Parameter";
		MeasureDir		= SystemDir + "\\" + "Measure";

		if( (_access(ParameterDir, 0 )) == -1 )	_mkdir(ParameterDir);
		if( (_access(MeasureDir, 0 )) == -1 )	_mkdir(MeasureDir);

		SystemFile		= ParameterDir	+ "\\" + SystemFileName;
	}

} sFILE_DATA;

extern sFILE_DATA g_sFile;

////////////////
// 글로벌 함수
////////////////


extern	void		DoEvents(DWORD dwMilliseconds=0);
extern  void		Delay(UINT delaytime=0, UINT unit=usec);
extern  __int64 	GetMicroSecond();  //sylee
extern	int			ErrMsg(int nMboxType=MB_OK, BOOL bTraceOn=TRUE);
extern  BOOL		FileExists(CString strFilePath);
extern  int 		Read_BdlFilePath();
extern  void AFX_CDECL MyTrace(PRT_TYPE prtType, LPCTSTR lpszFormat, ...);
extern  int  AFX_CDECL MyTrace2(PRT_TYPE prtType, char* pStr, int strSize, LPCTSTR lpszFormat, ...);

extern  int  AFX_CDECL MySnprintf(PRT_TYPE prtType, char* pStr, int strSize, LPCTSTR lpszFormat, ...);
//extern  BOOL		CreateFolder(LPCTSTR szFolder);
//extern  BOOL		DeleteSubDir(const TCHAR* sPath);
//extern  void		DeleteFolder(char *strDirectory);


// son170522
// value는 인자로 INT만 받아야 이 함수가 정상동작한다.
// double을 value로 받으면 value에 0.5가 더해짐. (값의 정확도가 떨어짐)
// INT는 계산 결과에 +0.5해서 반올림하는 것이 의미가 있지만, 
// double이 인자(Parameter)인 경우에는 값이 왜곡되는 결과가 나옴. 
//
//  정리하면
//  INT 인수인 경우,  인자인 value를 결과값으로 사용해야 하고 (반올림을 위해)
//  double 인수인 경우, 리턴값인 dValue를 결과값으로 사용하되 인자인 value값이 변경됨에 유의한다.
#define MAX_VDAD		60000
#define MAX_IDAD		50000

#define MAX_LV			20
#define MAX_HV			300
#define	MAX_VD			MAX_HV
template <class T> double UNIT_CONVERT(T &value, UINT condition)
{
	
	double dValue = value;

	switch(condition)
	{
	case VOLT_TO_DA: 		//son170508
		if(dValue <= MAX_LV)	
			dValue *= MAX_VDAD / (double)MAX_LV;	// *=3000
		else				
			dValue *= MAX_VDAD / (double)MAX_HV;	// *=200
		break;

	case VOLT_TO_DA_SPARK: //son170510 
		dValue *= MAX_VDAD / (double)MAX_HV;		// *=200		LV, HV 상관없이 *200 (신부성차장 confirm)
		break;

	case I0_TO_DA:
		dValue *= MAX_IDAD / (double)200000.;		// *=0.25		1~200mA: uA단위로 *250
		break;

	case I1_TO_DA:
		dValue *= MAX_IDAD / (double)20000.;		// *=2.5		1~20mA: uA단위로 *2500
		break;	

	case I2_TO_DA:
		dValue *= MAX_IDAD / (double)10000.;		// *=5			1~10mA: uA단위로 *5000
		break;

	case I3_TO_DA:
		dValue *= MAX_IDAD / (double)2000.;			// *=25		    1~2000uA: uA단위로 *25	(mA단위로 0.025)
		break;

	case I4_TO_DA:
		dValue *= MAX_IDAD / (double)200.;			// *=250		1~200uA: 250
		break;

	case I6_TO_DA:
		break;
		
	/////////////////////////////////

	case AD_TO_LVOLT:
		dValue *= MAX_LV / (double)MAX_VDAD;		// *=(1/3000)
		break;

	case AD_TO_HVOLT:
		dValue *= MAX_HV / (double)MAX_VDAD;		// *=(1/200)
		break;

	case AD_TO_4W_VOLT:
		dValue *= 1/6553.5;			
		break;

	case AD_TO_I0://mA
#if 1	
		dValue *= 200/(double)MAX_IDAD;			// *= 200/50000 mA   
#else	
		//2017.11.26 => 1/1000 까지 계산해서 리턴하는 것으로 원복. 
		//2017.11.24  UNIT_CONVERT() 이후에 1/1000 계산(uA->mA 단위변환) 하기로 수정
		//            UNIT_CONVER()에서는 모두 uA 단위로 리턴함.
		dValue *= 200000/(double)MAX_IDAD;		// *= 4 uA      1/1000 하면  1/250
#endif
		break;

	case AD_TO_I1://mA
#if 1
		dValue *= 20./(double)MAX_IDAD;			// *= 20/50000 (1/2500) mA   
#else
		dValue *= 20000./(double)MAX_IDAD;		// *= 2/5 uA	1/1000 하면  1/2500
#endif
		break;	

	case AD_TO_I2://mA
#if 1
		dValue *= 10./(double)MAX_IDAD;			// *= 10/50000 (1/5000) mA   
#else	
		dValue *= 10000./(double)MAX_IDAD;		// *= 1/5 uA	1/1000 하면 1/5000
#endif
		break;

	case AD_TO_I3://uA
		dValue *= 2000./(double)MAX_IDAD;		// *= 2000/50000 (1/25) uA  
		break;

	case AD_TO_I4://uA
		dValue *= 200./(double)MAX_IDAD;		// *= 200/50000  (1/250) uA
		break;

	case AD_TO_I5://uA					
		dValue *= 20./(double)MAX_IDAD;			// *= 20/50000  (1/25000) uA
		break;

	case AD_TO_I6:
		break;
	}

	//value = (T)(dValue+0.5);// T for integer
	value = (T)(dValue);// T for integer

	return dValue;
};


#endif // !defined(AFX_GLOBALS_H__FDA16379_4221_4970_8471_B8FBD39D2C1E__INCLUDED_)
