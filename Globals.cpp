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
// �۷ι� ��Ʈ���� ����
////////////////////////

sFILE_DATA		g_sFile;


//////////////////////
// �۷ι� Ŭ���� ����
//////////////////////

//CFileOperation		g_sFILEOp;



////////////////////
// �۷ι� �Լ� ����
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
	// �Է°� ������ ����
	// 1->7.5~8.5u  2->9u 4->11u 5->12 7->13u 10->17u
	// 100->106u 200->206u 500->506u

	//static LONGLONG FreqQuadPart=0;
	static double FreqQuadPart=0;
	static LARGE_INTEGER Freq, ZeroCnt, CurCnt;

	// ���ػ� Ÿ�̸��� 1�ʴ� ������ ȹ��
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
	case usec:		// 1us(micro sec) �� �������� ��ȯ
		FreqQuadPart = (double)Freq.QuadPart/1000000.0;
		break;
	}
	
	//---------------------------------------------------
	// ex) delaytime=200, unit=usec(2) ���
	// 	    200 micro sec ���� DoEvents()�� �ݺ��Ͽ� �����Ѵ�.
	

#if 0		
	// Elapsed time�� ǥ���ϱ�
	__int64	startTime, endTime;
	startTime = GetMicroSecond();

	QueryPerformanceCounter(&ZeroCnt);	// ���۽����� counter �� ȹ��
	do
	{
		DoEvents();
		QueryPerformanceCounter(&CurCnt);	// DoEvents()�� ������ ������ counter �� ȹ��
	}
	while((CurCnt.QuadPart-ZeroCnt.QuadPart)/FreqQuadPart < delaytime);

	endTime = GetMicroSecond();

	MyTrace(PRT_LEVEL1, "			Delay(%d %s), Elapsed time=%d(usec)\n", delaytime,
						(unit == sec) ? "sec" :
					    (unit == msec) ? "msec":  
					    (unit == usec) ? "usec": "Logic Err?",  (endTime - startTime));

#else
	QueryPerformanceCounter(&ZeroCnt);	// ���۽����� counter �� ȹ��
	do
	{
		DoEvents();
		QueryPerformanceCounter(&CurCnt);	// DoEvents()�� ������ ������ counter �� ȹ��
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

// ȣ����1:  ErrMsg() 			:  MB_OK�� �޽����ڽ����� &  TRACE ���, ȣ������ �ݵ�� ERR.Reset() ����
// ȣ����2:  ErrMsg(MB_OK, FALSE) :  MB_OK�� �޽����ڽ��� ����, ȣ������ �ݵ�� ERR.Reset() ����
// ȣ����3:  ErrMsg(-1, TRUE)		:  TRACE�� ���, ȣ�� ���� ERR.Reset() �������� �ʰ� �����ܰ� return�Ŀ� �ñ�. 
//                                      ErrMsg(-1)�θ� �ᵵ �ǰ�, Errmsg(-1..) ���Ŀ��� return�Ŀ� ErrMsg(MB_OK, FALSE)�� 
//                                      �޽��� �ڽ��� ȣ���ϴ� ������� ������ ������.
//                                      ��ǻ� �� ��� ��� �׳� TRACE(PRT_BASIC, ..) �� ȣ���ص� ��.

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

	if (ERR.Get() != NO_ERR)	// Error�� ��쿡�� MsgBox ����Ѵ�.
	{
		// Error �� ���, MBoxType�� ������ ��쿡��  MsgBox�� ����Ѵ�. 		
		// MBoxType�� -1�� �����ϸ� MsgBox�� ������� �ʴ´�. 
		if (nMboxType != -1)	
			ret = AfxMessageBox(strMsg, nMboxType | MB_ICONERROR); // modal
	}

	// bTraceOn == FALSE �̸� TRACE ����� ���� �ʴ´�. 
	// ErrMsg()���� Trace ����� �����ϸ� PC���� return�� ������ ����� ���.  (for debugging)
	// ErrMsg(nMboxType==MB_OK, bTraceOn==FALSE)�� ȣ���ؾ� �Ѵ�.
	if (bTraceOn == TRUE)
	{
		MyTrace(PRT_BASIC, "%s\n", strMsg);	// ����� 512����Ʈ�� ���� �� �����Ƿ�  MyTrace() ���
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


//son "C:\ACE400\SETUP\FilePathJig.ini"�� Load
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

	// 2018.02.28  FilePathJig.ini �� bdl ���� path �̸��� ������ ���԰����ϵ��� ����.
	char tempStr[200];
	char c;

	//son ù 3���� skip
	fscanf(fp, "%[^\n]s", tempStr);	// "%[^\n]s" : ������ ������ ���ڿ� �Է�
	fscanf(fp, "%c", &c);			// "%[^\n]s" ����� ���� ���๮��(\n) ó��
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


//2017.11.06  TRACE�� buffer�� 512���� �ۿ� ������� �ʾƼ� �Ʒ��� ���� �ű� �Լ� ����
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


//2018.02.14  ���ɰ����� ���� print off�̸� snprintf�� �������� �ʴ� ��� �߰�.
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

// MyTrace�� �ٸ� ��. : pStr �����͸� �Է¹޾Ƽ� ȣ�����Ŀ� ����� str�� ������ �� �ִ�.
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

	//char* pStr = str.GetBuffer(2048);	// �̷��� �ϴϱ� �״´�.. 
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
