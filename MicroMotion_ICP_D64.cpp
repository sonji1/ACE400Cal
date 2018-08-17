// MicroMotion_ICP_D64.cpp: implementation of the CMicroMotion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ACE400Cal.h"
#include "MicroMotion_ICP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//HANDLE	m_hDIODrv[CONTEC_BOARD_NUM];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////




CMicroMotion_ICP_D64::CMicroMotion_ICP_D64()
{
	m_bBdDrv = INVALID_CARD_ID;		// 0xFF
	dwPrevVal = 0xFFFFFFFF;

}

CMicroMotion_ICP_D64::~CMicroMotion_ICP_D64()
{

}

// Return code String   (PCI_D64H.h 참조) 

void CMicroMotion_ICP_D64::myD64hGetErrString(short wErrCode, char* errStr)
{
	if (wErrCode ==  SUCCESS_NO_ERROR)
	{
		strcpy(errStr, "OK" );		
		return;
	}

	// 2018.02.14 error 이거나 print on일 때에만 string을 생성
	WORD  bitMask = 0x01 << PRT_DEV_D64;
	if (!(g_wPrintState & bitMask)) 
		return;


	switch (wErrCode)
	{

		// indicate the unsuccessful routine-execution
		case ERROR_ROUTINE_FAIL_BASE     : strcpy(errStr,  "ERROR_ROUTINE_FAIL_BASE"    );  break;   // -100
		case ERROR_DEVICE_OPEN           : strcpy(errStr,  "ERROR_DEVICE_OPEN"          );  break;   // -102
		case ERROR_DEVICE_CLOSE          : strcpy(errStr,  "ERROR_DEVICE_CLOSE"         );  break;   // -103
		case ERROR_DOUBLE_BUFFER_MODE    : strcpy(errStr,  "ERROR_DOUBLE_BUFFER_MODE"   );  break;   // -104
		case ERROR_DI_CONFIG             : strcpy(errStr,  "ERROR_DI_CONFIG"            );  break;   // -105
		case ERROR_DI_ASYNC_CLEAR        : strcpy(errStr,  "ERROR_DI_ASYNC_CLEAR"       );  break;   // -106
		case ERROR_DI_ASYNC_CHECK        : strcpy(errStr,  "ERROR_DI_ASYNC_CHECK"       );  break;   // -107
		case ERROR_DI_ASYNC_HALF_READY   : strcpy(errStr,  "ERROR_DI_ASYNC_HALF_READY"  );  break;   // -108
		case ERROR_DI_PIO_READ           : strcpy(errStr,  "ERROR_DI_PIO_READ"          );  break;   // -109
		case ERROR_DO_CONFIG             : strcpy(errStr,  "ERROR_DO_CONFIG"            );  break;   // -110
		case ERROR_DO_ASYNC_CLEAR        : strcpy(errStr,  "ERROR_DO_ASYNC_CLEAR"       );  break;   // -111
		case ERROR_DO_ASYNC_CHECK        : strcpy(errStr,  "ERROR_DO_ASYNC_CHECK"       );  break;   // -112
		case ERROR_DO_PIO_READ           : strcpy(errStr,  "ERROR_DO_PIO_READ"          );  break;   // -113
		case ERROR_DO_PIO_WRITE          : strcpy(errStr,  "ERROR_DO_PIO_WRITE"         );  break;   // -114
		case ERROR_DO_PIO_LINE_WRITE     : strcpy(errStr,  "ERROR_DO_PIO_LINE_WRITE"    );  break;   // -115
		case ERROR_CONTINUE_DI_START     : strcpy(errStr,  "ERROR_CONTINUE_DI_START"    );  break;   // -116
		case ERROR_CONTINUE_DO_START     : strcpy(errStr,  "ERROR_CONTINUE_DO_START"    );  break;   // -117
		case ERROR_FIFO_STATUS_GET       : strcpy(errStr,  "ERROR_FIFO_STATUS_GET"      );  break;   // -118
		case ERROR_DO_EXT_TRIGLINE_WRITE : strcpy(errStr,  "ERROR_DO_EXT_TRIGLINE_WRITE");  break;   // -119
		case ERROR_DO_ASYNC_HALF_READY   : strcpy(errStr,  "ERROR_DO_ASYNC_HALF_READY"  );  break;   // -120
		case ERROR_DIGITAL_FILTER_SET    : strcpy(errStr,  "ERROR_DIGITAL_FILTER_SET"   );  break;   // -121
		case ERROR_OREQ_WIDTH_SET        : strcpy(errStr,  "ERROR_OREQ_WIDTH_SET"       );  break;   // -122

		// Parameters Error
		case ERROR_INVALID_PARAMETER_BASE     : strcpy(errStr, "ERROR_INVALID_PARAMETER_BASE"    ); break;  // -200
		case ERROR_INVALID_CARD_ID            : strcpy(errStr, "ERROR_INVALID_CARD_ID"           ); break;  // -201
		case ERROR_INVALID_SCANNED_INDEX      : strcpy(errStr, "ERROR_INVALID_SCANNED_INDEX"     ); break;  // -202
		case ERROR_CARD_ID_DUPLICATED         : strcpy(errStr, "ERROR_CARD_ID_DUPLICATED"        ); break;  // -203
		case ERROR_INVILID_DOUBLE_BUFFER_MODE : strcpy(errStr, "ERROR_INVILID_DOUBLE_BUFFER_MODE"); break;  // -204
		case ERROR_INVALID_PORT_LINE          : strcpy(errStr, "ERROR_INVALID_PORT_LINE"         ); break;  // -205
		case ERROR_INVALID_TRIGGER_SOURCE     : strcpy(errStr, "ERROR_INVALID_TRIGGER_SOURCE"    ); break;  // -206
		case ERROR_INVALID_TRIGGER_ENABLE     : strcpy(errStr, "ERROR_INVALID_TRIGGER_ENABLE"    ); break;  // -207
		case ERROR_INVALID_TRIGGER_POLARITY   : strcpy(errStr, "ERROR_INVALID_TRIGGER_POLARITY"  ); break;  // -208
		case ERROR_INVALID_IREQ_POLARITY      : strcpy(errStr, "ERROR_INVALID_IREQ_POLARITY"     ); break;  // -209
		case ERROR_INVALID_OTRIG_LEVEL        : strcpy(errStr, "ERROR_INVALID_OTRIG_LEVEL"       ); break;  // -210
		case ERROR_INVALID_OREG_ENABLE        : strcpy(errStr, "ERROR_INVALID_OREG_ENABLE"       ); break;  // -211
		case ERROR_INVALID_DOUBLE_BUFFER_MODE : strcpy(errStr, "ERROR_INVALID_DOUBLE_BUFFER_MODE"); break;  // -212
		case ERROR_INVALID_SYNCH_OP_MODE      : strcpy(errStr, "ERROR_INVALID_SYNCH_OP_MODE"     ); break;  // -213
		case ERROR_SAMPLE_COUNT_IS_ODD        : strcpy(errStr, "ERROR_SAMPLE_COUNT_IS_ODD"       ); break;  // -214
		case ERROR_INVALID_DO_ITERATIONS      : strcpy(errStr, "ERROR_INVALID_DO_ITERATIONS"     ); break;  // -215
		case ERROR_INVALID_EVENT_ACTION       : strcpy(errStr, "ERROR_INVALID_EVENT_ACTION"      ); break;  // -216
		case ERROR_INVALID_CALLBACK_ADDRESS   : strcpy(errStr, "ERROR_INVALID_CALLBACK_ADDRESS"  ); break;  // -217
		case ERROR_INVALID_BUFFER_ADDRESS     : strcpy(errStr, "ERROR_INVALID_BUFFER_ADDRESS"    ); break;  // -218
		case ERROR_INVALID_PATTERN_SIZE       : strcpy(errStr, "ERROR_INVALID_PATTERN_SIZE"      ); break;  // -219
		case ERROR_INVALID_FILTER_SETTING     : strcpy(errStr, "ERROR_INVALID_FILTER_SETTING"    ); break;  // -220
		case ERROR_INVALID_OREG_WIDTH         : strcpy(errStr, "ERROR_INVALID_OREG_WIDTH"        ); break;  // -221
		
		// RunTime Error
		case ERROR_RUNTIME_BASE               : strcpy(errStr, "ERROR_RUNTIME_BASE"              ); break;  // -300
		case ERROR_NO_CARD_FOUND              : strcpy(errStr, "ERROR_NO_CARD_FOUND"             ); break;  // -316
		case ERROR_MEMORY_MAP                 : strcpy(errStr, "ERROR_MEMORY_MAP"                ); break;  // -317
		case ERROR_MEMORY_UNMAP               : strcpy(errStr, "ERROR_MEMORY_UNMAP"              ); break;  // -318
		case ERROR_ACCESS_VIOLATION_DATA_COPY : strcpy(errStr, "ERROR_ACCESS_VIOLATION_DATA_COPY"); break;  // -319
		case ERROR_VARIABLE_PITCH_SET         : strcpy(errStr, "ERROR_VARIABLE_PITCH_SET"        ); break;  // -320
		case ERROR_DI_EVENT_ATTACH            : strcpy(errStr, "ERROR_DI_EVENT_ATTACH"           ); break;  // -321
		case ERROR_DI_EVENT_DETACH            : strcpy(errStr, "ERROR_DI_EVENT_DETACH"           ); break;  // -322
		case ERROR_DO_EVENT_ATTACH            : strcpy(errStr, "ERROR_DO_EVENT_ATTACH"           ); break;  // -323
		case ERROR_DO_EVENT_DETACH            : strcpy(errStr, "ERROR_DO_EVENT_DETACH"           ); break;  // -324
		case ERROR_EVENT_CREATE_FAILED        : strcpy(errStr, "ERROR_EVENT_CREATE_FAILED"       ); break;  // -325
		case ERROR_OVERLAP_EVENT_CREATE       : strcpy(errStr, "ERROR_OVERLAP_EVENT_CREATE"      ); break;  // -326
		case ERROR_IOCTL_FAILED               : strcpy(errStr, "ERROR_IOCTL_FAILED"              ); break;  // -361
		case ERROR_UNDEFINED_EXCEPTION        : strcpy(errStr, "ERROR_UNDEFINED_EXCEPTION"       ); break;  // -362

		// DIO RunTime Error
		case ERROR_DIO_RUNTIME_BASE                   : strcpy(errStr, "ERROR_DIO_RUNTIME_BASE"                   ); break; // -500
		case ERROR_DEVICE_POWER_DOWN                  : strcpy(errStr, "ERROR_DEVICE_POWER_DOWN"                  ); break; // -501
		case ERROR_INVALID_MAPPED_ADDRESS             : strcpy(errStr, "ERROR_INVALID_MAPPED_ADDRESS"             ); break; // -502
		case ERROR_BUFFER_NOT_ENOUGH                  : strcpy(errStr, "ERROR_BUFFER_NOT_ENOUGH"                  ); break; // -503
		case ERROR_DMA_NOT_AVAILABLE                  : strcpy(errStr, "ERROR_DMA_NOT_AVAILABLE"                  ); break; // -504
		case ERROR_DMA_NOT_COMPLETE                   : strcpy(errStr, "ERROR_DMA_NOT_COMPLETE"                   ); break; // -505
		case ERROR_DMA_NOT_STARTED                    : strcpy(errStr, "ERROR_DMA_NOT_STARTED"                    ); break; // -506
		case ERROR_DMA_NOT_PAUSED                     : strcpy(errStr, "ERROR_DMA_NOT_PAUSED"                     ); break; // -507
		case ERROR_DMA_IS_PAUSED                      : strcpy(errStr, "ERROR_DMA_IS_PAUSED"                      ); break; // -508
		case ERROR_DMA_IN_PROGRESS                    : strcpy(errStr, "ERROR_DMA_IN_PROGRESS"                    ); break; // -509
		case ERROR_INVALID_SAMPLE_RATE                : strcpy(errStr, "ERROR_INVALID_SAMPLE_RATE"                ); break; // -510
		case ERROR_SAMPLE_COUNT_TOO_LARGE		      : strcpy(errStr, "ERROR_SAMPLE_COUNT_TOO_LARGE"		      ); break; // -511
		case ERROR_SYNCH_OP_WITH_DOUBLE_BUFFER_MODE   : strcpy(errStr, "ERROR_SYNCH_OP_WITH_DOUBLE_BUFFER_MODE"   ); break; // -512
		case ERROR_SYNCH_OP_WITH_ASYNC_CHECK          : strcpy(errStr, "ERROR_SYNCH_OP_WITH_ASYNC_CHECK"          ); break; // -513
		case ERROR_UNSUPPORTED_SETTING                : strcpy(errStr, "ERROR_UNSUPPORTED_SETTING"                ); break; // -514
		case ERROR_PATTERN_OUT_WITH_DOUBLE_BUFFER_MODE: strcpy(errStr, "ERROR_PATTERN_OUT_WITH_DOUBLE_BUFFER_MODE"); break; // -515
		case ERROR_PATTERN_OUT_IN_PROGRESS            : strcpy(errStr, "ERROR_PATTERN_OUT_IN_PROGRESS"            ); break; // -516


		default:
			strcpy(errStr, "Undefined_D64h_Err!");
			return;
	}

}


BOOL CMicroMotion_ICP_D64::InitMotion(int bd_id)
{
//	MyTrace(PRT_DEV_D64, "%s:%d\n", __FILE__, __LINE__);

	short	wCardNum = 2;
	short	wRet = SUCCESS_NO_ERROR;
	CString strTemp;
	char	strErr[256] = "";

	// m_D64[0] 인지 m_D64[1]인지 구분을 위해  MicroMotion_ICP 에서 bd_id를 넘겨 받음. 
	m_nBdId = bd_id;		


	//----------------------------------
	// Check System Resource

	if (m_nBdId == 0)		// 두번 수행을 막기 위해 m_D64[0] 일 때에만 수행.
	{
#ifdef __PC_TEST__
		wRet = SUCCESS_NO_ERROR;
#else
		wRet = d64h_scan( &wCardNum, 	// pointer to available PISO-VS600 cards, 해당 카드 갯수 리턴
								NULL );		// pointer to avaliable cards array 
#endif
		myD64hGetErrString(wRet, strErr);
		strTemp.Format("ICP_D64[%d] InitMotion(): d64h_scan() %s err=%d(%s), wCardNum=%d\n", 
					m_nBdId, (wRet == SUCCESS_NO_ERROR)? "OK.":"Fail!",
					wRet, strErr, wCardNum );
		if (wRet != SUCCESS_NO_ERROR)
		{
			ERR.Set(DIO_INIT_ERR, strTemp);
#ifndef __PC_TEST__
			return FALSE;
#endif
		}
		MyTrace(PRT_DEV_D64, "%s\n", strTemp);  

		// CardNum이 2보다 작다면 문제 있음.   (2개여야 정상이지만 3개여도 일단은 pass)
		if (wCardNum < PCI_D64_NUM)
		{
			strTemp.Format("ICP_D64[%d] InitMotion(): d64h_scan() Fail! wCardNum(=%d) is < 2. \n", 
					m_nBdId, wCardNum );
			ERR.Set(DIO_INIT_ERR, strTemp);
			return FALSE;
		}
	}


	//------------------------
	// Get Card Info
	
	m_bBdDrv = INVALID_CARD_ID;		// 0xFF

#ifdef __PC_TEST__
	wRet = SUCCESS_NO_ERROR;
	m_bBdDrv = 5 + bd_id;		// test용
#else
	wRet = d64h_cardinfo_get(m_nBdId, 	// U16 ScannedIndex  start with 0
								&m_bBdDrv);	// U8* pCardID		 pointer to Card ID 
#endif
	myD64hGetErrString(wRet, strErr);
	strTemp.Format("ICP_D64[%d] InitMotion(): d64h_cardinfo_get(%d) %s err=%d(%s), m_bBdDrv=%d\n", 
				m_nBdId, m_nBdId, (wRet != SUCCESS_NO_ERROR)? "Fail!":"OK.",
				wRet, strErr, m_bBdDrv);
	if (wRet != SUCCESS_NO_ERROR)
	{
		ERR.Set(DIO_INIT_ERR, strTemp);
#ifndef __PC_TEST__
		return FALSE;
#endif
	}			
	MyTrace(PRT_DEV_D64, "%s\n", strTemp);

	if( m_bBdDrv == INVALID_CARD_ID )
	{
		strTemp.Format("ICP_D64[%d] InitMotion() Fail!: m_bBdDrv=%d, Invalid Card Id.\n", 
				bd_id, m_bBdDrv);
		MyTrace(PRT_DEV_D64, "%s", strTemp);
		ERR.Set(DIO_EXIT_ERR, strTemp);
		return FALSE;
	}



	//-------------------
	// Open Card Driver 
	
#ifdef __PC_TEST__
	wRet = SUCCESS_NO_ERROR;
#else
	wRet = d64h_open(m_bBdDrv);		// BYTE bCardID		
#endif
	myD64hGetErrString(wRet, strErr);
	strTemp.Format("ICP_D64[%d] InitMotion(): d64h_open(drv=%d) %s err=%d(%s)\n", 
					m_nBdId, m_bBdDrv,  (wRet != SUCCESS_NO_ERROR)? "Fail!":"OK.",
					wRet, strErr);
	if (wRet != SUCCESS_NO_ERROR)
	{
		ERR.Set(DIO_INIT_ERR, strTemp);
#ifndef __PC_TEST__
		return FALSE;
#endif
	}
	MyTrace(PRT_DEV_D64, "%s\n", strTemp);		


	return TRUE;
}

BOOL CMicroMotion_ICP_D64::CloseMotion(int bd_id)
{
	short	wRet = SUCCESS_NO_ERROR;
	CString strTemp;
	char	strErr[256] = "";

	if( m_bBdDrv == INVALID_CARD_ID )
	{
		strTemp.Format("ICP_D64[%d] CloseMotion() Fail!: m_bBdDrv=%d, Invalid Card Id.\n", 
				bd_id, m_bBdDrv);
		MyTrace(PRT_DEV_D64, "%s", strTemp);
		//ERR.Set(DIO_EXIT_ERR, strTemp);
		return TRUE;
	}

#ifndef __PC_TEST__
	wRet = d64h_close(m_bBdDrv);
#endif
	myD64hGetErrString(wRet, strErr);
	strTemp.Format("ICP_D64[%d] CloseMotion(): d64h_close(drv=%d) %s err=%d(%s)\n", 
				bd_id, m_bBdDrv, (wRet != SUCCESS_NO_ERROR)? "Fail!":"OK.",
				wRet, strErr);
	if(wRet != SUCCESS_NO_ERROR)
	{
		ERR.Set(DIO_EXIT_ERR, strTemp);
#ifndef __PC_TEST__
		return FALSE;
#endif
	}
	MyTrace(PRT_DEV_D64, "%s", strTemp);

	return TRUE;
}


/*
BOOL CMicroMotion_ICP_D64::On(WORD wBitNo)
{
	CString strTemp;
	if(wBitNo<OUTPUT_START || wBitNo>OUTPUT_START+TOTAL_OUTPUT_NUM) 
	{
		strTemp.Format("On(%d) Error! wBitNo=%d, Range(%d <= wbitNo <= %d) Over.",
					wBitNo, wBitNo, OUTPUT_START,  (OUTPUT_START+TOTAL_OUTPUT_NUM) );
		MyTrace(PRT_DEV_D64, "%s\n", strTemp);
		ERR.Set(RANGE_OVER, strTemp);
		return FALSE;
	}

	DWORD	dwRet=NO_ERR;
	HANDLE	hDrv=m_hDIODrv[1];

	dwRet=DioOutBit((short)hDrv, wBitNo-OUTPUT_START, 1);
	{
		CString strTemp;
		char	strErr[256] = "";
		DioGetErrorString(dwRet , strErr);

		strTemp.Format("dwRet=%d(%s), DioOutBit(hDrv=%d, bitNo=%d, val=%d)", dwRet, strErr, hDrv, (wBitNo-OUTPUT_START), 1);
		MyTrace(PRT_DEV_D64, "%s\n", strTemp);
		if(dwRet != NO_ERR)
		{
			ERR.Set(INVALID_RETURN_DIOOUTBIT, strTemp);
		#ifndef __PC_TEST__
			return FALSE;
		#endif
		}
	}
	return TRUE;
}
*/

/*
BOOL CMicroMotion_ICP_D64::Off(WORD wBitNo)
{
	CString strTemp;
	if ( wBitNo < OUTPUT_START || wBitNo > (OUTPUT_START+TOTAL_OUTPUT_NUM) ) 
	{
		strTemp.Format("Off(%d) Error! wBitNo=%d, Range(%d <= wbitNo <= %d) Over.",
					wBitNo, wBitNo, OUTPUT_START,  (OUTPUT_START+TOTAL_OUTPUT_NUM) );
		MyTrace(PRT_DEV_D64, "%s\n", strTemp);
		ERR.Set(RANGE_OVER, strTemp);
		return FALSE;
	}

	DWORD	dwRet=NO_ERR;
	HANDLE	hDrv=m_hDIODrv[1];

	dwRet=DioOutBit((short)hDrv, wBitNo-OUTPUT_START, 0);
	{
		char	strErr[256] = "";
		DioGetErrorString(dwRet , strErr);

		strTemp.Format("dwRet=%d(%s), DioOutBit(hDrv=%d, bitNo=%d, val=%d)", dwRet, strErr, hDrv, (wBitNo-OUTPUT_START), 0);
		MyTrace(PRT_DEV_D64, "%s\n", strTemp);
		if(dwRet != NO_ERR)
		{
			ERR.Set(INVALID_RETURN_DIOOUTBIT, strTemp);
		#ifndef __PC_TEST__
			return FALSE;
		#endif
		}
	}

	return TRUE;
}
*/

/*
BOOL CMicroMotion_ICP_D64::IsOn(WORD wBitNo)
{
	CString strTemp;

	if(wBitNo<INPUT_START || wBitNo>INPUT_START+TOTAL_INPUT_NUM) 
	{
		strTemp.Format("IsOn(%d) Error! wBitNo=%d, Range(%d <= wbitNo <= %d) Over.",
					wBitNo, wBitNo, INPUT_START,  (INPUT_START+TOTAL_INPUT_NUM) );
		MyTrace(PRT_DEV_D64, "%s\n", strTemp);
		ERR.Set(RANGE_OVER, strTemp);
		return FALSE;
	}

	DWORD	dwRet=NO_ERR;
	HANDLE	hDrv=m_hDIODrv[0];
	BYTE	InpBuf='0';

	dwRet=DioInpBit((short)hDrv, wBitNo-INPUT_START, &InpBuf);
	{
		char	szError[256] = "";
		DioGetErrorString(dwRet , szError);

		strTemp.Format("dwRet=%d(%s), DioInpBit(hDrv=%d, bitNo=%d)", dwRet, szError, hDrv, (wBitNo-INPUT_START));
		MyTrace(PRT_DEV_D64, "%s\n", strTemp);
		if(dwRet != NO_ERR)
		{
			ERR.Set(INVALID_RETURN_DIOINPBIT, strTemp);
		#ifndef __PC_TEST__
			return FALSE;
		#endif
		}
	}
	
	if(static_cast<int>(InpBuf) == 0) return FALSE;
	return TRUE;
}
*/

/*
BOOL CMicroMotion_ICP_D64::IsOff(WORD wBitNo)
{
	CString strTemp;
	if(wBitNo < INPUT_START || wBitNo>INPUT_START+TOTAL_INPUT_NUM) 
	{
		strTemp.Format("IsOff(%d) Error! wBitNo=%d, Range(%d <= wbitNo <= %d) Over.",
					wBitNo, wBitNo, INPUT_START,  (INPUT_START+TOTAL_INPUT_NUM) );
		MyTrace(PRT_DEV_D64, "%s\n", strTemp);
		ERR.Set(RANGE_OVER, strTemp);
		return FALSE;
	}

	DWORD	dwRet=NO_ERR;
	HANDLE	hDrv=m_hDIODrv[0];
	BYTE	InpBuf='0';

	dwRet=DioInpBit((short)hDrv, wBitNo-INPUT_START, &InpBuf);
	{
		char	szError[256] = "";
		DioGetErrorString(dwRet , szError);

		strTemp.Format("dwRet=%d(%s), DioInpBit(hDrv=%d, bitNo=%d)", dwRet, szError, hDrv, (wBitNo-INPUT_START));
		MyTrace(PRT_DEV_D64, "%s\n", strTemp);
		if(dwRet != NO_ERR)
		{
			ERR.Set(INVALID_RETURN_DIOINPBIT, strTemp);
		#ifndef __PC_TEST__
			return FALSE;
		#endif
		}
	}

	if(static_cast<int>(InpBuf) == 1) return FALSE;
	return TRUE;
}
*/

//DWORD dwReadVal = 0xFFFFFFFF;
BOOL CMicroMotion_ICP_D64::TranceiveCmd(UINT order, psCMD psCmd, int nD64Id)
{
	MyTrace(PRT_DEV_D64, "ICP_D64[%d]: TranceiveCmd(): order=%d    ", nD64Id, order);	

	// Data Tranfer
	UINT ret=0, mask_count=0, i=0;
	short wRet=SUCCESS_NO_ERROR;

	WORD  BitNo[128]; // for index , TOTAL_OUTPUT_NUM TOTAL_INPUT_NUM ?
	BYTE  Buf[128];   // for data  TOTAL_OUTPUT_NUM TOTAL_INPUT_NUM ?
	WORD  BitNum=0;	  // data count
//	BOOL  bBitVal;
	int	  bit;

	CString strTemp;
	char	strErr[256] = "";
	DWORD dwVal = 0xFFFFFFFF;

	ZeroMemory(BitNo, sizeof(WORD)*(sizeof(BitNo)/sizeof(WORD)));
	ZeroMemory(Buf, sizeof(BYTE)*(sizeof(Buf)/sizeof(BYTE)));

	switch(psCmd->cmd)
	{
		case ENDCMD:

			MyTrace(PRT_DEV_D64, "\n");
			return TRUE;
			break;


		case _2BYTE_RCV:
		case _4BYTE_RCV:
		#ifdef __PC_TEST__
			wRet = SUCCESS_NO_ERROR;
		#else
			wRet=d64h_di_readport(m_bBdDrv, 			// BYTE bCardID
									&psCmd->dwRcvVal);	// U32* pValue		// returned 4 byte DI value 
		#endif
			if ((wRet != SUCCESS_NO_ERROR) 							// 2018.02.13 성능을 위해 Error 이거나 
					|| (g_wPrintState & (0x01 << PRT_DEV_D64)) )   	//            print on 일때에만 string을 생성
			{
				myD64hGetErrString(wRet, strErr);
				strTemp.Format("wRet=%d(%s) d64h_di_readport(drv=%d, dwRcvVal= 0x%08x)", 
						wRet, strErr, m_bBdDrv, psCmd->dwRcvVal, 
						psCmd->dwRcvVal );		
				MyTrace(PRT_DEV_D64, "\n%s\n", strTemp);

				if (wRet != SUCCESS_NO_ERROR) {
					ERR.Set(INVALID_RETURN_DIOINPBYTE, strTemp);
			#ifndef __PC_TEST__
					return FALSE;
			#endif
				}
			}
			break;


		case DLY:
			//son170511  
			// DLY cmd의 delay값을 BYTE byteVal에서 WORD mbitBuf[0] 으로 교체.
			// 256 이상의 숫자 할당이 안되는 문제 관련 수정 사항.
			if(psCmd->mbitBuf[0] != 0)	// 값이 0이면 delay 할 필요 없음
			{
				if(psCmd->PortNo == sec)		Delay(psCmd->mbitBuf[0], sec);
				else if(psCmd->PortNo == msec)	Delay(psCmd->mbitBuf[0], msec);
				else if(psCmd->PortNo == usec)	Delay(psCmd->mbitBuf[0], usec);
			}
			break;


		case BYTE_TRS: // byte 단위로 전송한다.

			// 2018.2.1  d64h_do_writeport 수행 이전에  d64h_do_readport 해서 기존 값을 최대한 유지시키는 방식이
			// 두번 라이브러리를 호출하므로 HW 처리 시간이 2배 이상 걸리게 되는 구조적인 문제 수정이 필요. 
			// do_readport 하는 대신에 d64h_do_writeport 하고 나서 바로 해당 값을 dwPrevVal에 보존하고 
			// 다음번 d64h_do_writeport 에서 참고하여 write하도록 한다.  
			dwVal = dwPrevVal;

			// byteBuf[order]값만 해당 포트 바이트 위치로 overwrite해서 4byte값을 새로 만든다. 
			dwVal &= ~(0xff << (psCmd->PortNo * 8)); // 해당 port위치의 1 byte를 지우고
			dwVal += psCmd->byteVal << (psCmd->PortNo * 8); // 새로운 1 byte로 채운다.


			// 4 byte(32 bit) 를 통으로 전송한다.
		#ifdef __PC_TEST__
			wRet = SUCCESS_NO_ERROR;
		#else
			wRet=d64h_do_writeport(m_bBdDrv, dwVal);		
		#endif
			if((wRet != SUCCESS_NO_ERROR) 						// 2018.02.13 성능을 위해 Error 이거나 
					|| (g_wPrintState & (0x01 << PRT_DEV_D64))) //            print on 일때에만 string을 생성
			{
				myD64hGetErrString(wRet, strErr);
				strTemp.Format("wRet=%d(%s) port=%d, byteVal=%d(0x%02x) d64h_do_writeport(drv=%d, dwVal= 0x%08x-> 0x%08X)\n", 
							wRet, strErr, psCmd->PortNo, psCmd->byteVal, psCmd->byteVal,  m_bBdDrv, dwPrevVal, dwVal);
				MyTrace(PRT_DEV_D64, "\n%s\n", strTemp);
				if(wRet != SUCCESS_NO_ERROR) {
					ERR.Set(INVALID_RETURN_DIOOUTBIT, strTemp);
			#ifndef __PC_TEST__
					return FALSE;
			#endif
				}
			}
			dwPrevVal = dwVal;

#if 0
	    // for Test   Write 직후 바로 읽어서 제대로 write 되었는지 확인
		#ifdef __PC_TEST__
			wRet = SUCCESS_NO_ERROR;
		#else
			wRet=d64h_do_readport(m_bBdDrv, &dwVal);		// d64h_di_readport 아님.
		#endif
			myD64hGetErrString(wRet, strErr);
			strTemp.Format("wRet=%d(%s) port=%d, byteVal=%d(0x%02x) d64h_do_readport(drv=%d, dwVal=0x%08X)", 
						wRet, strErr, psCmd->PortNo, psCmd->byteVal, psCmd->byteVal, m_bBdDrv, dwVal);
			MyTrace(PRT_DEV_D64, "%s\n", strTemp);
#endif
			
			break;


		// 2018.01.18 d64h_do_writeline 을 사용하면 bit 가 제대로 설정이 안 되는 문제 때문에 아래 코드 봉인
/*		
		case BIT_TRS:
			MyTrace(PRT_DEV_D64, "\n");
			if (psCmd->BitNo >= D64_TOTAL_OUTPUT_NUM)	// Range Check
			{
				wRet = -1;
				sprintf(strErr, "BitNo=%d Range(<%d) Over!",psCmd->BitNo, D64_TOTAL_OUTPUT_NUM);
			}
			else
			{
		#ifdef __PC_TEST__
				wRet = SUCCESS_NO_ERROR;
		#else
				wRet=d64h_do_writeline(m_bBdDrv, psCmd->BitNo, psCmd->byteVal);
		#endif
				myD64hGetErrString(wRet, strErr);
			}
			
			strTemp.Format("wRet=%d(%s) d64h_do_writeline(drv=%d, BitNo=%d, byteVal=%d)", 
						wRet, strErr, m_bBdDrv, psCmd->BitNo, psCmd->byteVal );
			MyTrace(PRT_DEV_D64, "%s\n", strTemp);
			if(wRet != SUCCESS_NO_ERROR) {
				ERR.Set(INVALID_RETURN_DIOOUTBIT, strTemp);
		#ifndef __PC_TEST__
				return FALSE;
		#endif
			}

			break;
*/

		case MBIT_TRS:
			// wBitNum 갯수 확인 등의 프린트 준비
			BitNum=0; 
			for(bit=0; bit<D64_TOTAL_OUTPUT_NUM; bit++)		// 32
			{
				// bit;					// index (bit location)  : 0~31
				// psCmd->mbitBuf[bit];	// data  (bit data)      : 32개의 0또는 1 data
				
				if(psCmd->mbitBuf[bit] != -1)
				{
					BitNo[BitNum] = bit;				// index (bit location)
					Buf[BitNum] = psCmd->mbitBuf[bit];	// data  (bit data)
					BitNum++;							// data count
				}
			}

			//------------------
			// mbitBuf print 
			
			// 2018.02.08.  mbitBuf 프린트하는데 시간이 많이 걸리므로 
			//             prt off 상태라면 아래코드를 아예 진입하지 않도록 수정.
			WORD  bitMask = (0x01 << PRT_DEV_D64);
			if (g_wPrintState & bitMask) // check print bit_mask
			{
				char	str[2048] = "";	
				char	*pStr = str;
				if (BitNum >= 8)		// 8개 이하면 라인을 바꾸지 않고 BitNo[]를 붙여서 출력
					MyTrace(PRT_DEV_D64, "\n");

				pStr += sprintf(pStr, "BitNo[]=");
				for (bit = 0; bit < BitNum; bit++)
				{
					if (bit != 0 && bit % 20 == 0)	// 20개 찍고 줄 바꾸기
						pStr += sprintf(pStr, "\n        ");
					if (bit % 10 == 0)				// 10개 찍고 한칸 띄우기
						pStr += sprintf(pStr, " ");
					pStr += sprintf(pStr, " %-3d", BitNo[bit]); 
				}
				if (BitNum >= 8)		// 8개 이하면 라인을 바꾸지 않고 Buf[]를 붙여서 출력
					pStr += sprintf(pStr, "\n");
				else
					pStr += sprintf(pStr, ",  ");
				
				// 이 코드는 BitNo가 꼭 0부터 시작하지 않을 수 있으므로 의미 없음.
				//pStr += sprintf(pStr, "          0 1 2 3 4 5 6 7 8 9  0 1 2 3 4 5 6 7 8 9");
				pStr += sprintf(pStr, "Buf[]=  ");
				for (bit = 0; bit < BitNum; bit++)
				{
					if (bit != 0 && bit % 20 == 0)	// 20개 찍고 줄 바꾸기
						pStr += sprintf(pStr, "\n        ");
					if (bit % 10 == 0)
						pStr += sprintf(pStr, " ");	// 10개 찍고 한 칸 띄우기 
					pStr += sprintf(pStr, " %d", Buf[bit]); 
				}
				pStr += sprintf(pStr, "\n");

				MyTrace(PRT_DEV_D64, "%s", str);		// 출력문이 512바이트를 넘을 수(BitNum 약 70개 이상시..) 있으므로  MyTrace() 사용
			}

			//-------------------------------------------------------------
			// BitNum이 8개 미만이면 mbit를 bit 단위로 바꾸어서 전송 한다.
			//
			// 2018.01.18 d64h_do_writeline 을 사용하면 bit 가 제대로 설정이 안 되는 문제 때문에 아래 코드 봉인
			//            8bit 보다 작더라도 무조건 이전 4byte 값을 read하고 다시 한번에 4byte를 write하도록 수정. 
#if 0	
			if (BitNum < 8)
			{
				for(bit=0; bit < D64_TOTAL_OUTPUT_NUM; bit++)		// 32
				{
					// bit;					// index (bit location)  : 0~31
					// psCmd->mbitBuf[bit];	// data  (bit data)      : 32개의 0또는 1 data
					
					if(psCmd->mbitBuf[bit] != -1)
					{
						bBitVal =  (psCmd->mbitBuf[bit]) ? 1 : 0;
					#ifdef __PC_TEST__
						wRet = SUCCESS_NO_ERROR;
					#else
						wRet=d64h_do_writeline(m_bBdDrv, bit, bBitVal);
					#endif
						if((wRet != SUCCESS_NO_ERROR) 						// 2018.02.13 성능을 위해 Error 이거나 
								|| (g_wPrintState & (0x01 << PRT_DEV_D64))) //            print on 일때에만 string을 생성
						{
							myD64hGetErrString(wRet, strErr);
							strTemp.Format("wRet=%d(%s) d64h_do_writeline(drv=%d, bit=%d, bBitVal=%d)", 
										wRet, strErr, m_bBdDrv, bit, bBitVal);
							MyTrace(PRT_DEV_D64, "%s\n", strTemp);
							if(wRet != SUCCESS_NO_ERROR) {
								ERR.Set(INVALID_RETURN_DIOOUTBIT, strTemp);
						#ifndef __PC_TEST__
								return FALSE;
						#endif
							}
						}
					}

				}

				MyTrace(PRT_DEV_D64, "\n");
				return TRUE;
			}
#endif

			//-----------------------------------------------
			// 8 bit 이상이면  byte 단위로 묶어서  전송한다.

			// mbit를 4byte로 통으로 전송 한다.
			// 먼저 현재 DO 포트의 4byte 값을 read.
			
			// 2018.2.1 일 추가.   
			// d64h_do_writeport 수행 이전에  d64h_do_readport 해서 기존 값을 최대한 유지시키는 방식이
			// 두번 라이브러리를 호출하므로 HW 처리 시간이 2배 이상 걸리게 되는 구조적인 문제 수정.
			// do_readport 하는 대신에 d64h_do_writeport 하고 나서 바로 해당 값을 dwPrevVal에 보존하고 
			// 다음번 d64h_do_writeport 에서 참고하여 write하도록 한다.  
			dwVal = dwPrevVal;

			// mbitbuf에 값이 설정된 경우에만 해당 비트를 mbitBuf[bit]값으로 변경.
			ZeroMemory(BitNo, sizeof(WORD)*(sizeof(BitNo)/sizeof(WORD)));
			ZeroMemory(Buf, sizeof(BYTE)*(sizeof(Buf)/sizeof(BYTE)));
	
			for(bit=0; bit<D64_TOTAL_OUTPUT_NUM; bit++)		// 32
			{
				// bit;					// index (bit location)  : 0~31
				// psCmd->mbitBuf[bit];	// data  (bit data)      : 32개의 0또는 1 data
				
				if(psCmd->mbitBuf[bit] != -1)
				{
					// make dwVal
					if (psCmd->mbitBuf[bit]) 		
						dwVal |= (0x01 << bit);		// bit 위치를 On으로 설정	
					else
						dwVal &= ~(0x01 << bit);	// bit 위치를 Off로 설정
				}
			}

			// 4 byte(32 bit) 를 통으로 전송한다.
		#ifdef __PC_TEST__
			wRet = SUCCESS_NO_ERROR;
		#else
			wRet=d64h_do_writeport(m_bBdDrv, dwVal);		
		#endif
			if((wRet != SUCCESS_NO_ERROR) 						// 2018.02.13 성능을 위해 Error 이거나 
					|| (g_wPrintState & (0x01 << PRT_DEV_D64)))	//            print on 일때에만 string을 생성
			{
				myD64hGetErrString(wRet, strErr);
				strTemp.Format("wRet=%d(%s) d64h_do_writeport(drv=%d, dwVal= 0x%08X-> 0x%08X)\n", 
											wRet, strErr, m_bBdDrv, dwPrevVal, dwVal);
				MyTrace(PRT_DEV_D64, "%s\n", strTemp);
				if(wRet != SUCCESS_NO_ERROR) {
					ERR.Set(INVALID_RETURN_DIOOUTBIT, strTemp);
			#ifndef __PC_TEST__
					return FALSE;
			#endif
				}
			}
			dwPrevVal = dwVal; // 전송이 된게 확실하면 dwPrevVal로 백업한다.

#if 0
			// Test...    write하자마자 읽어 본다. 제대로 write되었는지 확인을 위해 
		#ifdef __PC_TEST__
			wRet = SUCCESS_NO_ERROR;
		#else
			wRet=d64h_do_readport(m_bBdDrv, &dwVal);		// d64h_di_readport 아님.
		#endif
			myD64hGetErrString(wRet, strErr);
			strTemp.Format("wRet=%d(%s) d64h_do_readport(drv=%d, dwVal=0x%08X)", 
						wRet, strErr, m_bBdDrv, dwVal);
			MyTrace(PRT_DEV_D64, "%s\n", strTemp);
#endif
			break;

		
	}	// switch(psCmd->cmd)


	return TRUE;
}
