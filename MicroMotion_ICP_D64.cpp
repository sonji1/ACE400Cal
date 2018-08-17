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

// Return code String   (PCI_D64H.h ����) 

void CMicroMotion_ICP_D64::myD64hGetErrString(short wErrCode, char* errStr)
{
	if (wErrCode ==  SUCCESS_NO_ERROR)
	{
		strcpy(errStr, "OK" );		
		return;
	}

	// 2018.02.14 error �̰ų� print on�� ������ string�� ����
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

	// m_D64[0] ���� m_D64[1]���� ������ ����  MicroMotion_ICP ���� bd_id�� �Ѱ� ����. 
	m_nBdId = bd_id;		


	//----------------------------------
	// Check System Resource

	if (m_nBdId == 0)		// �ι� ������ ���� ���� m_D64[0] �� ������ ����.
	{
#ifdef __PC_TEST__
		wRet = SUCCESS_NO_ERROR;
#else
		wRet = d64h_scan( &wCardNum, 	// pointer to available PISO-VS600 cards, �ش� ī�� ���� ����
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

		// CardNum�� 2���� �۴ٸ� ���� ����.   (2������ ���������� 3������ �ϴ��� pass)
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
	m_bBdDrv = 5 + bd_id;		// test��
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
			if ((wRet != SUCCESS_NO_ERROR) 							// 2018.02.13 ������ ���� Error �̰ų� 
					|| (g_wPrintState & (0x01 << PRT_DEV_D64)) )   	//            print on �϶����� string�� ����
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
			// DLY cmd�� delay���� BYTE byteVal���� WORD mbitBuf[0] ���� ��ü.
			// 256 �̻��� ���� �Ҵ��� �ȵǴ� ���� ���� ���� ����.
			if(psCmd->mbitBuf[0] != 0)	// ���� 0�̸� delay �� �ʿ� ����
			{
				if(psCmd->PortNo == sec)		Delay(psCmd->mbitBuf[0], sec);
				else if(psCmd->PortNo == msec)	Delay(psCmd->mbitBuf[0], msec);
				else if(psCmd->PortNo == usec)	Delay(psCmd->mbitBuf[0], usec);
			}
			break;


		case BYTE_TRS: // byte ������ �����Ѵ�.

			// 2018.2.1  d64h_do_writeport ���� ������  d64h_do_readport �ؼ� ���� ���� �ִ��� ������Ű�� �����
			// �ι� ���̺귯���� ȣ���ϹǷ� HW ó�� �ð��� 2�� �̻� �ɸ��� �Ǵ� �������� ���� ������ �ʿ�. 
			// do_readport �ϴ� ��ſ� d64h_do_writeport �ϰ� ���� �ٷ� �ش� ���� dwPrevVal�� �����ϰ� 
			// ������ d64h_do_writeport ���� �����Ͽ� write�ϵ��� �Ѵ�.  
			dwVal = dwPrevVal;

			// byteBuf[order]���� �ش� ��Ʈ ����Ʈ ��ġ�� overwrite�ؼ� 4byte���� ���� �����. 
			dwVal &= ~(0xff << (psCmd->PortNo * 8)); // �ش� port��ġ�� 1 byte�� �����
			dwVal += psCmd->byteVal << (psCmd->PortNo * 8); // ���ο� 1 byte�� ä���.


			// 4 byte(32 bit) �� ������ �����Ѵ�.
		#ifdef __PC_TEST__
			wRet = SUCCESS_NO_ERROR;
		#else
			wRet=d64h_do_writeport(m_bBdDrv, dwVal);		
		#endif
			if((wRet != SUCCESS_NO_ERROR) 						// 2018.02.13 ������ ���� Error �̰ų� 
					|| (g_wPrintState & (0x01 << PRT_DEV_D64))) //            print on �϶����� string�� ����
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
	    // for Test   Write ���� �ٷ� �о ����� write �Ǿ����� Ȯ��
		#ifdef __PC_TEST__
			wRet = SUCCESS_NO_ERROR;
		#else
			wRet=d64h_do_readport(m_bBdDrv, &dwVal);		// d64h_di_readport �ƴ�.
		#endif
			myD64hGetErrString(wRet, strErr);
			strTemp.Format("wRet=%d(%s) port=%d, byteVal=%d(0x%02x) d64h_do_readport(drv=%d, dwVal=0x%08X)", 
						wRet, strErr, psCmd->PortNo, psCmd->byteVal, psCmd->byteVal, m_bBdDrv, dwVal);
			MyTrace(PRT_DEV_D64, "%s\n", strTemp);
#endif
			
			break;


		// 2018.01.18 d64h_do_writeline �� ����ϸ� bit �� ����� ������ �� �Ǵ� ���� ������ �Ʒ� �ڵ� ����
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
			// wBitNum ���� Ȯ�� ���� ����Ʈ �غ�
			BitNum=0; 
			for(bit=0; bit<D64_TOTAL_OUTPUT_NUM; bit++)		// 32
			{
				// bit;					// index (bit location)  : 0~31
				// psCmd->mbitBuf[bit];	// data  (bit data)      : 32���� 0�Ǵ� 1 data
				
				if(psCmd->mbitBuf[bit] != -1)
				{
					BitNo[BitNum] = bit;				// index (bit location)
					Buf[BitNum] = psCmd->mbitBuf[bit];	// data  (bit data)
					BitNum++;							// data count
				}
			}

			//------------------
			// mbitBuf print 
			
			// 2018.02.08.  mbitBuf ����Ʈ�ϴµ� �ð��� ���� �ɸ��Ƿ� 
			//             prt off ���¶�� �Ʒ��ڵ带 �ƿ� �������� �ʵ��� ����.
			WORD  bitMask = (0x01 << PRT_DEV_D64);
			if (g_wPrintState & bitMask) // check print bit_mask
			{
				char	str[2048] = "";	
				char	*pStr = str;
				if (BitNum >= 8)		// 8�� ���ϸ� ������ �ٲ��� �ʰ� BitNo[]�� �ٿ��� ���
					MyTrace(PRT_DEV_D64, "\n");

				pStr += sprintf(pStr, "BitNo[]=");
				for (bit = 0; bit < BitNum; bit++)
				{
					if (bit != 0 && bit % 20 == 0)	// 20�� ��� �� �ٲٱ�
						pStr += sprintf(pStr, "\n        ");
					if (bit % 10 == 0)				// 10�� ��� ��ĭ ����
						pStr += sprintf(pStr, " ");
					pStr += sprintf(pStr, " %-3d", BitNo[bit]); 
				}
				if (BitNum >= 8)		// 8�� ���ϸ� ������ �ٲ��� �ʰ� Buf[]�� �ٿ��� ���
					pStr += sprintf(pStr, "\n");
				else
					pStr += sprintf(pStr, ",  ");
				
				// �� �ڵ�� BitNo�� �� 0���� �������� ���� �� �����Ƿ� �ǹ� ����.
				//pStr += sprintf(pStr, "          0 1 2 3 4 5 6 7 8 9  0 1 2 3 4 5 6 7 8 9");
				pStr += sprintf(pStr, "Buf[]=  ");
				for (bit = 0; bit < BitNum; bit++)
				{
					if (bit != 0 && bit % 20 == 0)	// 20�� ��� �� �ٲٱ�
						pStr += sprintf(pStr, "\n        ");
					if (bit % 10 == 0)
						pStr += sprintf(pStr, " ");	// 10�� ��� �� ĭ ���� 
					pStr += sprintf(pStr, " %d", Buf[bit]); 
				}
				pStr += sprintf(pStr, "\n");

				MyTrace(PRT_DEV_D64, "%s", str);		// ��¹��� 512����Ʈ�� ���� ��(BitNum �� 70�� �̻��..) �����Ƿ�  MyTrace() ���
			}

			//-------------------------------------------------------------
			// BitNum�� 8�� �̸��̸� mbit�� bit ������ �ٲپ ���� �Ѵ�.
			//
			// 2018.01.18 d64h_do_writeline �� ����ϸ� bit �� ����� ������ �� �Ǵ� ���� ������ �Ʒ� �ڵ� ����
			//            8bit ���� �۴��� ������ ���� 4byte ���� read�ϰ� �ٽ� �ѹ��� 4byte�� write�ϵ��� ����. 
#if 0	
			if (BitNum < 8)
			{
				for(bit=0; bit < D64_TOTAL_OUTPUT_NUM; bit++)		// 32
				{
					// bit;					// index (bit location)  : 0~31
					// psCmd->mbitBuf[bit];	// data  (bit data)      : 32���� 0�Ǵ� 1 data
					
					if(psCmd->mbitBuf[bit] != -1)
					{
						bBitVal =  (psCmd->mbitBuf[bit]) ? 1 : 0;
					#ifdef __PC_TEST__
						wRet = SUCCESS_NO_ERROR;
					#else
						wRet=d64h_do_writeline(m_bBdDrv, bit, bBitVal);
					#endif
						if((wRet != SUCCESS_NO_ERROR) 						// 2018.02.13 ������ ���� Error �̰ų� 
								|| (g_wPrintState & (0x01 << PRT_DEV_D64))) //            print on �϶����� string�� ����
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
			// 8 bit �̻��̸�  byte ������ ���  �����Ѵ�.

			// mbit�� 4byte�� ������ ���� �Ѵ�.
			// ���� ���� DO ��Ʈ�� 4byte ���� read.
			
			// 2018.2.1 �� �߰�.   
			// d64h_do_writeport ���� ������  d64h_do_readport �ؼ� ���� ���� �ִ��� ������Ű�� �����
			// �ι� ���̺귯���� ȣ���ϹǷ� HW ó�� �ð��� 2�� �̻� �ɸ��� �Ǵ� �������� ���� ����.
			// do_readport �ϴ� ��ſ� d64h_do_writeport �ϰ� ���� �ٷ� �ش� ���� dwPrevVal�� �����ϰ� 
			// ������ d64h_do_writeport ���� �����Ͽ� write�ϵ��� �Ѵ�.  
			dwVal = dwPrevVal;

			// mbitbuf�� ���� ������ ��쿡�� �ش� ��Ʈ�� mbitBuf[bit]������ ����.
			ZeroMemory(BitNo, sizeof(WORD)*(sizeof(BitNo)/sizeof(WORD)));
			ZeroMemory(Buf, sizeof(BYTE)*(sizeof(Buf)/sizeof(BYTE)));
	
			for(bit=0; bit<D64_TOTAL_OUTPUT_NUM; bit++)		// 32
			{
				// bit;					// index (bit location)  : 0~31
				// psCmd->mbitBuf[bit];	// data  (bit data)      : 32���� 0�Ǵ� 1 data
				
				if(psCmd->mbitBuf[bit] != -1)
				{
					// make dwVal
					if (psCmd->mbitBuf[bit]) 		
						dwVal |= (0x01 << bit);		// bit ��ġ�� On���� ����	
					else
						dwVal &= ~(0x01 << bit);	// bit ��ġ�� Off�� ����
				}
			}

			// 4 byte(32 bit) �� ������ �����Ѵ�.
		#ifdef __PC_TEST__
			wRet = SUCCESS_NO_ERROR;
		#else
			wRet=d64h_do_writeport(m_bBdDrv, dwVal);		
		#endif
			if((wRet != SUCCESS_NO_ERROR) 						// 2018.02.13 ������ ���� Error �̰ų� 
					|| (g_wPrintState & (0x01 << PRT_DEV_D64)))	//            print on �϶����� string�� ����
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
			dwPrevVal = dwVal; // ������ �Ȱ� Ȯ���ϸ� dwPrevVal�� ����Ѵ�.

#if 0
			// Test...    write���ڸ��� �о� ����. ����� write�Ǿ����� Ȯ���� ���� 
		#ifdef __PC_TEST__
			wRet = SUCCESS_NO_ERROR;
		#else
			wRet=d64h_do_readport(m_bBdDrv, &dwVal);		// d64h_di_readport �ƴ�.
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
