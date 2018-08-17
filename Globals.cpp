//////////////////////////////////////////////////////////////////////
// Globals.cpp: implementation of the CGlobals class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ACE400Cal.h"
#include "Globals.h"
#include <mmsystem.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlobals::CGlobals()
{

}

CGlobals::~CGlobals()
{

}


////////////////////////
// 글로벌 스트럭쳐 설정
////////////////////////

sFILE_DATA		g_sFile;


//////////////////////
// 글로벌 클래스 설정
//////////////////////

//CFileOperation		g_sFILEOp;



////////////////////
// 글로벌 함수 설정
////////////////////
void DoEvents(DWORD dwMilliseconds)
{
	MSG msg;
	if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	Sleep(dwMilliseconds);
}

void Delay(UINT delaytime, UINT unit)
{
	// 입력값 측정값 검증
	// 1->7.5~8.5u  2->9u 4->11u 5->12 7->13u 10->17u
	// 100->106u 200->206u 500->506u

	//static LONGLONG FreqQuadPart=0;
	static double FreqQuadPart=0;
	static LARGE_INTEGER Freq, ZeroCnt, CurCnt;

	// 고해상도 타이머의 1초당 진동수 획득
	if(!QueryPerformanceFrequency(&Freq)) return;

	switch(unit)
	{
	case sec:
		FreqQuadPart = (double)Freq.QuadPart;
		break;

	case msec:
		FreqQuadPart = (double)Freq.QuadPart/1000.0;
		break;

	default:
	case usec:		// 1us(micro sec) 당 진동수로 변환
		FreqQuadPart = (double)Freq.QuadPart/1000000.0;
		break;
	}
	
	//---------------------------------------------------
	// ex) delaytime=200, unit=usec(2) 라면
	// 	    200 micro sec 동안 DoEvents()를 반복하여 수행한다.
	

#if 0		
	// Elapsed time도 표시하기
	__int64	startTime, endTime;
	startTime = GetMicroSecond();

	QueryPerformanceCounter(&ZeroCnt);	// 시작시점의 counter 값 획득
	do
	{
		DoEvents();
		QueryPerformanceCounter(&CurCnt);	// DoEvents()를 수행한 이후의 counter 값 획득
	}
	while((CurCnt.QuadPart-ZeroCnt.QuadPart)/FreqQuadPart < delaytime);

	endTime = GetMicroSecond();

	MyTrace(PRT_LEVEL1, "			Delay(%d %s), Elapsed time=%d(usec)\n", delaytime,
						(unit == sec) ? "sec" :
					    (unit == msec) ? "msec":  
					    (unit == usec) ? "usec": "Logic Err?",  (endTime - startTime));

#else
	QueryPerformanceCounter(&ZeroCnt);	// 시작시점의 counter 값 획득
	do
	{
		DoEvents();
		QueryPerformanceCounter(&CurCnt);	// DoEvents()를 수행한 이후의 counter 값 획득
	}
	while((CurCnt.QuadPart-ZeroCnt.QuadPart)/FreqQuadPart < delaytime);

	MyTrace(PRT_LEVEL1, "			Delay(%d %s)\n", delaytime,
						(unit == sec) ? "sec" :
					    (unit == msec) ? "msec":  
					    (unit == usec) ? "usec": "Logic Err?" ) ;

#endif


}

__int64 GetMicroSecond()  //sylee
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER now;

    if( !QueryPerformanceFrequency(&frequency) )
        return (__int64)GetTickCount();

    if( !QueryPerformanceCounter(&now) )
        return (__int64)GetTickCount();

	return((((__int64)now.QuadPart) * ((__int64)1000000)) / ((__int64)frequency.QuadPart));
}

// 호출방법1:  ErrMsg() 			:  MB_OK로 메시지박스생성 &  TRACE 출력, 호출이후 반드시 ERR.Reset() 수행
// 호출방법2:  ErrMsg(MB_OK, FALSE) :  MB_OK로 메시지박스만 생성, 호출이후 반드시 ERR.Reset() 수행
// 호출방법3:  ErrMsg(-1, TRUE)		:  TRACE만 출력, 호출 이후 ERR.Reset() 수행하지 않고 다음단계 return후에 맡김. 
//                                      ErrMsg(-1)로만 써도 되고, Errmsg(-1..) 이후에는 return후에 ErrMsg(MB_OK, FALSE)로 
//                                      메시지 박스만 호출하는 방식으로 응용이 가능함.
//                                      사실상 이 방식 대신 그냥 TRACE(PRT_BASIC, ..) 를 호출해도 됨.

// MBoxType: 	MB_OK				(0)
// 				MB_OKCANCEL 		(1)
// 				MB_ABORTRETRYIGNORE	(2)
// 				MB_YESNOCANCEL		(3)
// 				MB_YESNO			(4)
// 				MB_RETRYCANCEL		(5)

int ErrMsg(int nMboxType, BOOL bTraceOn)
{
//	CDlgErrMsg DLG;
//	DLG.ntype = nMboxType;
//	DLG.DoModla();
//	int ret = DLG.ret;

//	if(ERR.Get() != NORMAL)	DISPMSG(strMsg);
//	if(ERR.Get() != NORMAL)	ERRMSGDLG.DoModal();

	//if(ERR.Get() == NO_ERR)	
	//	return 0;
	

//	const char* title; const char* state; const char* sol;
	CString title=""; CString state=""; CString sol="";
	ERR.GetMsg(ERR.Get(), title, state, sol);
//	state = state + "\n\n          " + ERR.m_strState;
	if(state == "") 
		state = ERR.m_strState;

	CString strMsg="", strTemp="";
	strMsg.Format("%s error. [ No. %d ]", title, ERR.Get());

	if(state != "") strTemp.Format("\n\nState : %s", state);
	strMsg += strTemp;
	if(sol != "") strTemp.Format("\n\nSolution : %s", sol), strMsg += strTemp;


	int ret = 0;	

	if (ERR.Get() != NO_ERR)	// Error인 경우에만 MsgBox 출력한다.
	{
		// Error 인 경우, MBoxType이 지정된 경우에는  MsgBox를 출력한다. 		
		// MBoxType을 -1로 지정하면 MsgBox를 출력하지 않는다. 
		if (nMboxType != -1)	
			ret = AfxMessageBox(strMsg, nMboxType | MB_ICONERROR); // modal
	}

	// bTraceOn == FALSE 이면 TRACE 출력을 하지 않는다. 
	// ErrMsg()에서 Trace 출력을 수행하면 PC에서 return값 조작이 어려운 경우.  (for debugging)
	// ErrMsg(nMboxType==MB_OK, bTraceOn==FALSE)로 호출해야 한다.
	if (bTraceOn == TRUE)
	{
		MyTrace(PRT_BASIC, "%s\n", strMsg);	// 출력이 512바이트를 넘을 수 있으므로  MyTrace() 사용
	}


// MB_ABORTRETRYIGNORE MB_OK MB_OKCANCEL MB_RETRYCANCEL MB_YESNO MB_YESNOCANCEL
// MB_ICONHAND MB_ICONSTOP MB_ICONERROR / MB_ICONQUESTION / MB_ICONEXCLAMATION MB_ICONWARNING / MB_ICONASTERISK MB_ICONINFORMATION
// IDABORT IDCANCEL IDIGNORE IDNO IDOK IDRETRY IDYES

	//ERR.Reset();

	return ret;
}


BOOL FileExists(CString strFilePath)
{
	CFileFind aFile;
	BOOL IsExist = aFile.FindFile(strFilePath);
	aFile.Close();
	return IsExist;
}


//son "C:\ACE400\SETUP\FilePathJig.ini"를 Load
//ex) ACE400  CFileSysInfo::LoadSaveView01(int type)
// C:\sonji\Bioptro\ACE400\ACE400-170816-1	<=  ACE400 SysInfoView01.m_pStrFileDri1
// C:\sonji\Bioptro\ACE400\ACE400-170816-1	<=  ACE400 SysInfoView01.m_pStrFilePath1
// C:\ACE400\QC-JIG-S100-BHFlex\4W_YP_S100\OPEN-READ.BDL  <= ACE400 SysInfoView01.m_pStrFilePathJig1
// C:\ACE400\QC-JIG-S100-BHFlex\4W_YP_S100\OPEN-READ.BDL  <= ACE400 SysInfoView01.m_pStrFilePathJig2 ****
// C:\ACE400\QC-JIG-S100-BHFlex\4W_YP_S100	<= SysInfoView01.m_pStrFileBDLPath
// OPEN-READ.BDL							<= SysInfoView01.m_pStrFileBDLName
int Read_BdlFilePath()
{
	FILE *fp; 
	char  fName[200]; 


	// "C:\ACE400\SETUP\FilePathJig.ini"
	::ZeroMemory(&fName, sizeof(fName));
	strcat( fName , g_sFile.ACE400_FilePathJigName ); //sylee121202

	if(!FileExists(fName)) { 	
		ERR.Set(FLAG_FILE_NOT_FOUND, fName); return FALSE; 
	}

	fp=fopen(fName,"rt");	
	if(fp == NULL) { 
		ERR.Set(FLAG_FILE_CANNOT_OPEN, fName); return FALSE;
	}

	// 2018.02.28  FilePathJig.ini 및 bdl 파일 path 이름에 공백이 포함가능하도록 수정.
	char tempStr[200];
	char c;

	//son 첫 3줄은 skip
	fscanf(fp, "%[^\n]s", tempStr);	// "%[^\n]s" : 공백을 포함한 문자열 입력
	fscanf(fp, "%c", &c);			// "%[^\n]s" 사용후 남은 개행문자(\n) 처리
	fscanf(fp, "%[^\n]s", tempStr);	
	fscanf(fp, "%c", &c);			
	fscanf(fp, "%[^\n]s", tempStr);	
	fscanf(fp, "%c", &c);			


	fscanf(fp, "%[^\n]s", g_sFile.ACE400_BdlFilePath);	// ex) C:\ACE400\QC-JIG-S100-BHFlex\4W_YP_S100\OPEN-READ.BDL 
	fscanf(fp, "%c", &c);			
	fscanf(fp, "%[^\n]s", g_sFile.ACE400_BdlFileDir); 	// ex) C:\ACE400\QC-JIG-S100-BHFlex\4W_YP_S100
	fscanf(fp, "%c", &c);			
	fscanf(fp, "%[^\n]s", g_sFile.ACE400_BdlFileName);	// ex) OPEN-READ.BDL 

	fclose(fp); 
	return TRUE;
} 


//2017.11.06  TRACE의 buffer가 512까지 밖에 허용하지 않아서 아래와 같이 신규 함수 생성
//  buffer size = 2048

WORD	g_wPrintState = 0x0001;		// PRT_BASIC    on 
void AFX_CDECL MyTrace(PRT_TYPE prtType, LPCTSTR lpszFormat, ...)
{
#ifdef _DEBUG // all AfxTrace output is controlled by afxTraceEnabled
	if (!afxTraceEnabled)
		return;
#endif
	WORD  bitMask = 0x01 << prtType;
	if (!(g_wPrintState & bitMask)) // check print bit_mask
		return;

	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	TCHAR szBuffer[2048];

	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);

	// was there an error? was the expanded string too long?
	ASSERT(nBuf >= 0);

	if ((afxTraceFlags & traceMultiApp) && (AfxGetApp() != NULL))
		afxDump << AfxGetApp()->m_pszExeName << ": ";
	afxDump << szBuffer;

	va_end(args);
}


//2018.02.14  성능개선을 위해 print off이면 snprintf를 수행하지 않는 기능 추가.
int AFX_CDECL MySnprintf(PRT_TYPE prtType, char* pStr, int strSize, LPCTSTR lpszFormat, ...)
{
#ifdef _DEBUG // all AfxTrace output is controlled by afxTraceEnabled
	if (!afxTraceEnabled)
		return (-1);
#endif
	WORD  bitMask = 0x01 << prtType;
	if (!(g_wPrintState & bitMask)) // check print bit_mask
	{
		return (-1);
	}

	va_list args;
	va_start(args, lpszFormat);

	int nBuf = _vsnprintf((char*)pStr, strSize, lpszFormat, args);

	// was there an error? was the expanded string too long?
	ASSERT(nBuf >= 0);

	va_end(args);

	return nBuf;
}

// MyTrace와 다른 점. : pStr 포인터를 입력받아서 호출이후에 출력한 str을 재사용할 수 있다.
int AFX_CDECL MyTrace2(PRT_TYPE prtType, char *pStr, int strSize, LPCTSTR lpszFormat, ...)
{
#ifdef _DEBUG // all AfxTrace output is controlled by afxTraceEnabled
	if (!afxTraceEnabled)
		return (-1);
#endif
	WORD  bitMask = 0x01 << prtType;
	if (!(g_wPrintState & bitMask)) // check print bit_mask
	{
		return (-1);
	}

	va_list args;
	va_start(args, lpszFormat);

	//char* pStr = str.GetBuffer(2048);	// 이렇게 하니까 죽는다.. 
	//int	  strSize = str.GetLength();
	int nBuf = _vsnprintf(pStr, strSize, lpszFormat, args);

	// was there an error? was the expanded string too long?
	ASSERT(nBuf >= 0);

	if ((afxTraceFlags & traceMultiApp) && (AfxGetApp() != NULL))
		afxDump << AfxGetApp()->m_pszExeName << ": ";
	afxDump << pStr;

	va_end(args);

	return nBuf;
}
