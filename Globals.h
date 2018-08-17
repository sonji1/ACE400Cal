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
// �۷ι� ���� 
///////////////

enum UNIT			{ sec=0, msec=1, usec=2, mA, uA, Mohm, Kohm, ohm, mohm, uohm };// sec=0, msec=1, usec=2 ����
enum UNIT_CONVERT_TYPE	{ 
	MM_TO_UM, UM_TO_MM, SEC_TO_MSEC, MSEC_TO_SEC,
	VOLT_TO_DA, I0_TO_DA, I1_TO_DA, I2_TO_DA, I3_TO_DA, I4_TO_DA, /*I5_TO_DA,*/ I6_TO_DA,
	AD_TO_LVOLT, AD_TO_HVOLT, AD_TO_I0, AD_TO_I1, AD_TO_I2, AD_TO_I3, AD_TO_I4, AD_TO_I5, AD_TO_I6,
	VOLT_TO_DA_SPARK, AD_TO_4W_VOLT
};

enum PRT_TYPE { PRT_BASIC, PRT_LEVEL1, PRT_LEVEL2, PRT_LEVEL3, PRT_DEVICE, PRT_DEV_D64 };
extern WORD	g_wPrintState;


///////////////////
// �۷ι� ��Ʈ����
///////////////////

typedef struct sFILE_DATA
{
	CString	SystemDir;					// ACE400Cal ���α׷��� ������ ���丮
	CString ParameterDir;
	CString MeasureDir;					// "C:\\ACE400\\SETUP\\LOGCAL"�� ��ü�Ͽ� ���
//	CString ProductDir;
//	CString ModelDir;
//	CString WorkDir;

	CString ACE400_FilePathJigName;		// ACE400�� FilePathJig name ("C:\\ACE400\\SETUP\\FilePathJig.ini") 
										//     BDL ������ path, name ������ ȹ��
	CString ACE400_TestConditionPath;	// ACE400�� dRec[m_nMode][16] ���� ��ġ("C:\\ACE400\\SETUP\\SysSet21_ACE400.ini") 
	CString ACE400_4wRangePath;	        // ACE400�� dRec4w[][] ���� ��ġ("C:\\ACE400\\SETUP\\SysSet21_4W.ini") 
										// 171   dRec4w[1][3]  <= SysSet21_4W.ini  ù��° �׸�   ex) 80    1~80     mohm�� 171
										// 172   dRec4w[2][3]  <= SysSet21_4W.ini  �ι�° �׸�   ex) 800   81~800   mohm�� 172
										// 173   dRec4w[3][3]  <= SysSet21_4W.ini  ����° �׸�   ex) 5000  801~5000 mohm�� 173
										// 174   dRec4w[4][3]  <=      ex) 50000   5001 ~ 50000   mohm�� 174
										// 175   dRec4w[5][3]  <=      ex) 500000  50001 ~ 500000 mohm�� 175


//	CString ACE400_RJigData_2WPath1;	// ACE400�� RJigData1[32][18] ���� ��ġ(".\\Parameter\\RJigData_2W.TXT") 
//	CString ACE400_RJigData_4WPath1;	// ACE400�� RJigData1[32][18] ���� ��ġ(".\\Parameter\\RJigData_4W.TXT") 
//	CString ACE400_RJigData_2WPath2;	// ACE400�� RJigData1[32][18] ���� ��ġ(".\\Parameter\\RJigData_S100_2W.TXT") 
//	CString ACE400_RJigData_4WPath2;	// ACE400�� RJigData1[32][18] ���� ��ġ(".\\Parameter\\RJigData_S100_4W.TXT") 

	CString ACE400_RJigData_Path1;		// ACE400�� RJigData1[32][18] ���� ��ġ("C:\\ACE400\\BDL\\QC\\RJigReadType.TXT") 
	CString ACE400_RJigData_Path2;		// ACE400�� RJigData1[32][18] ���� ��ġ("C:\\ACE400\\BDL\\QC\\RJigReadType_S100.TXT") 

	CString ACE400_CalDataDir;			// ACE400�� Calibarion ��� data Dir ("C:\\ACE400\\SETUP\\CAL");
	CString ACE400_LogCalDataDir;		// ACE400�� Calibarion ��� Log data Dir ("C:\\ACE400\\SETUP\\LOGCAL");
	
	CString ACE400_SysInfoPath;			// ACE400�� SystemType(QD, DD, SD) ���� ��ġ ("C:\\ACE400\\SETUP\\SysSet01.ini")
	CString ACE400_PinSetupPath;		// ACE400�� PinSetup ���� ��ġ ("C:\\ACE400\\SETUP2\\SysSetPin.ini")

	char ACE400_BdlFilePath[200];		// ACE400 BDL Path(Dir+name) SysInfoView01.m_pStrFilePathJig2 �� �ش�
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
		// ���� ACE400Cal�� ������ ���丮 ��ġ�� �����´�.
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
// �۷ι� �Լ�
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
// value�� ���ڷ� INT�� �޾ƾ� �� �Լ��� �������Ѵ�.
// double�� value�� ������ value�� 0.5�� ������. (���� ��Ȯ���� ������)
// INT�� ��� ����� +0.5�ؼ� �ݿø��ϴ� ���� �ǹ̰� ������, 
// double�� ����(Parameter)�� ��쿡�� ���� �ְ�Ǵ� ����� ����. 
//
//  �����ϸ�
//  INT �μ��� ���,  ������ value�� ��������� ����ؾ� �ϰ� (�ݿø��� ����)
//  double �μ��� ���, ���ϰ��� dValue�� ��������� ����ϵ� ������ value���� ����ʿ� �����Ѵ�.
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
		dValue *= MAX_VDAD / (double)MAX_HV;		// *=200		LV, HV ������� *200 (�źμ����� confirm)
		break;

	case I0_TO_DA:
		dValue *= MAX_IDAD / (double)200000.;		// *=0.25		1~200mA: uA������ *250
		break;

	case I1_TO_DA:
		dValue *= MAX_IDAD / (double)20000.;		// *=2.5		1~20mA: uA������ *2500
		break;	

	case I2_TO_DA:
		dValue *= MAX_IDAD / (double)10000.;		// *=5			1~10mA: uA������ *5000
		break;

	case I3_TO_DA:
		dValue *= MAX_IDAD / (double)2000.;			// *=25		    1~2000uA: uA������ *25	(mA������ 0.025)
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
		//2017.11.26 => 1/1000 ���� ����ؼ� �����ϴ� ������ ����. 
		//2017.11.24  UNIT_CONVERT() ���Ŀ� 1/1000 ���(uA->mA ������ȯ) �ϱ�� ����
		//            UNIT_CONVER()������ ��� uA ������ ������.
		dValue *= 200000/(double)MAX_IDAD;		// *= 4 uA      1/1000 �ϸ�  1/250
#endif
		break;

	case AD_TO_I1://mA
#if 1
		dValue *= 20./(double)MAX_IDAD;			// *= 20/50000 (1/2500) mA   
#else
		dValue *= 20000./(double)MAX_IDAD;		// *= 2/5 uA	1/1000 �ϸ�  1/2500
#endif
		break;	

	case AD_TO_I2://mA
#if 1
		dValue *= 10./(double)MAX_IDAD;			// *= 10/50000 (1/5000) mA   
#else	
		dValue *= 10000./(double)MAX_IDAD;		// *= 1/5 uA	1/1000 �ϸ� 1/5000
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
