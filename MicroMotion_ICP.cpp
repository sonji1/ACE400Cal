// MicroMotion_ICP.cpp: implementation of the CMicroMotion class.
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

#pragma comment(lib,"UniDAQ.lib")


CMicroMotion_ICP		g_MicroM_ICP;

//////////////////////////////////////////////////////////////////////
// CMicroMotion_ICP 
//////////////////////////////////////////////////////////////////////

CMicroMotion_ICP::CMicroMotion_ICP()
{
}


CMicroMotion_ICP::~CMicroMotion_ICP()
{

}

BOOL CMicroMotion_ICP::InitMotion()
{

	if (m_D96.InitMotion() == FALSE)
	{
		ErrMsg(-1, TRUE);		// Err를 Trace만 출력
//#ifndef __PC_TEST__
		return FALSE;
	}

	if (m_D64[0].InitMotion(0) == FALSE)
	{
		ErrMsg(-1, TRUE);		// Err를 Trace만 출력
		return FALSE;
	}

	if (m_D64[1].InitMotion(1) == FALSE)
	{
		ErrMsg(-1, TRUE);		// Err를 Trace만 출력
		return FALSE;
	}


	return TRUE;
}

BOOL CMicroMotion_ICP::CloseMotion()
{
	if (m_D96.CloseMotion() == FALSE)
	{
		ErrMsg(-1, TRUE);		// Err를 Trace만 출력
//#ifndef __PC_TEST__
		return FALSE;
	}

	if (m_D64[0].CloseMotion(0) == FALSE)
	{
		ErrMsg(-1, TRUE);		// Err를 Trace만 출력
		return FALSE;
	}
	
	if (m_D64[1].CloseMotion(1) == FALSE)
	{
		ErrMsg(-1, TRUE);		// Err를 Trace만 출력
		return FALSE;
	}

	//AfxMessageBox("D96, D64[0], D64[1] Card Closed.", MB_OK | MB_ICONERROR);

	return TRUE;
}

// On, Off는 너무 HW dependent함.  Calibration이나 ManualTest 를 HW 독립적으로 수정하기 위해 
// On, Off 기능 대신에  MacroMotion에서 sSET_BIT 기능을 추가하고 On/Off 코드는 코멘트 처리함.
// 이 이후에 HW 교체 기능이 추가 가능함.
/*
BOOL CMicroMotion_ICP::On(WORD wBitNo)
{
}

BOOL CMicroMotion_ICP::Off(WORD wBitNo)
{
	return TRUE;
}

BOOL CMicroMotion_ICP::IsOn(WORD wBitNo)
{
	return TRUE;
}

BOOL CMicroMotion_ICP::IsOff(WORD wBitNo)
{
	return TRUE;
}
*/


BOOL CMicroMotion_ICP::TranceiveSeq(psSEQ psSeq)
{
	sCMD sCmd;
	UINT order = 0; 

	MyTrace(PRT_DEVICE, "\n");

	while(order<MAX_ORDER_NUM)
	{
		// psSeq의 Cmd를 하나씩 카피해서 맞는 보드로 보낸다. 
		sCmd.bd_id = psSeq->bd_id[order];
		sCmd.cmd = psSeq->cmd[order];
		sCmd.BitNo = psSeq->BitNo[order];
		sCmd.PortNo = psSeq->PortNo[order];
		sCmd.byteVal = psSeq->byteBuf[order];
		memcpy(sCmd.mbitBuf, psSeq->mbitBuf[order], (sizeof(int)*TOTAL_OUTPUT_NUM));

		if (sCmd.cmd == ENDCMD)
			break;
		
	
		switch(sCmd.bd_id)
		{
			case BD_D96:
				if (m_D96.TranceiveCmd(order, &sCmd) == FALSE)
					return FALSE;
				break;

			case BD_D64_0:
				// 2번째 parameter는 nBdId define값이 아니라 d64 id로 리턴
				if (m_D64[0].TranceiveCmd(order, &sCmd, 0) == FALSE)	
					return FALSE;
				break;

			case BD_D64_1:
				if (m_D64[1].TranceiveCmd(order, &sCmd, 1) == FALSE)	
					return FALSE;
				break;

		}

		// sGET_VI 참고.  
		// _2BYTE_RCV 인 경우에는 order3만 수행. order4는 실제로 수행하지 않고 값만 아래와 같이 할당.
		if (sCmd.cmd == _2BYTE_RCV)
		{
#if 0
			psSeq->byteBuf[order]   = (BYTE)(sCmd.dwRcvVal & 0xff);			// 1'st byte
			psSeq->byteBuf[order+1] = (BYTE)((sCmd.dwRcvVal >> 8 ) & 0xff); // 2'nd byte
#else
			psSeq->dwRcvVal = (WORD)(sCmd.dwRcvVal & 0xffff);	// 1'st, 2'nd 2 byte

#endif
			order += 2;
			continue;
		}

		// sGET_VI 참고.  
		// _2BYTE_RCV 인 경우에는 order3만 수행. order4,5,6은 실제로 수행하지 않고 값만 아래와 같이 할당.
		if (sCmd.cmd == _4BYTE_RCV)
		{
#if 0
			psSeq->byteBuf[order]   = (BYTE)(sCmd.dwRcvVal & 0xff);			// 1'st byte
			psSeq->byteBuf[order+1] = (BYTE)((sCmd.dwRcvVal >> 8 ) & 0xff); // 2'nd byte
			psSeq->byteBuf[order+2] = (BYTE)((sCmd.dwRcvVal >> 16) & 0xff); // 3'rd byte  
			psSeq->byteBuf[order+3] = (BYTE)((sCmd.dwRcvVal >> 24) & 0xff); // 4'th byte 
#else
			psSeq->dwRcvVal = (DWORD)(sCmd.dwRcvVal & 0xffffffff);	// 4 byte

#endif
			order += 4;
			continue;
		}

		order++;

	}// while


	return TRUE;
}




//////////////////////////////////////////////////////////////////////
// CMicroMotion_ICP_D96
//////////////////////////////////////////////////////////////////////


CMicroMotion_ICP_D96::CMicroMotion_ICP_D96()
{
	ZeroMemory(baPrevPortVal, sizeof(BYTE)*(sizeof(baPrevPortVal)/sizeof(BYTE)));

}

CMicroMotion_ICP_D96::~CMicroMotion_ICP_D96()
{

}


// Return code String  (UniDAQ.h 참조)

char	saStrIxudErr[Ixud_GetMMIOMapStatus+1][40] = {
	"OK"                        ,    //  0 //NoErr, Correct
	"Ixud_OpenDriverErr"        ,    //  1 //Open driver error
	"Ixud_PnPDriverErr"     	,    //  2 //Plug & Play error
	"Ixud_DriverNoOpen"         ,    //  3 //The driver was not open
	"Ixud_GetDriverVersionErr"  ,    //  4 //Receive driver version error
	"Ixud_ExceedBoardNumber"	,    //  5 //Board number error
	"Ixud_FindBoardErr"		    ,    //  6 //No board found
	"Ixud_BoardMappingErr"	    ,    //  7 //Board Mapping error
	"Ixud_DIOModesErr"		    ,    //  8 //Digital input/output mode setting error
	"Ixud_InvalidAddress"	    ,    //  9 //Invalid address
	"Ixud_InvalidSize"		    ,    // 10 //Invalid size
	"Ixud_InvalidPortNumber"    ,    // 11 //Invalid port number
	"Ixud_UnSupportedModel"	    ,    // 12 //This board model is not supported
	"Ixud_UnSupportedFun"	    ,    // 13 //This function is not supported
	"Ixud_InvalidChannelNumber" ,    // 14 //Invalid channel number
	"Ixud_InvalidValue"		    ,    // 15 //Invalid value
	"Ixud_InvalidMode"		    ,    // 16 //Invalid mode
	"Ixud_GetAIStatusTimeOut"   ,    // 17 //A timeout occurred while receiving the status of the analog input
	"Ixud_TimeOutErr"		    ,    // 18 //Timeout error
	"Ixud_CfgCodeIndexErr"	    ,    // 19 //A compatible configuration code table index could not be found
	"Ixud_ADCCTLTimeoutErr"	    ,    // 20 //ADC controller a timeout error
	"Ixud_FindPCIIndexErr"	    ,    // 21 //A compatible PCI table index value could not be found
	"Ixud_InvalidSetting"	    ,    // 22 //Invalid setting value
	"Ixud_AllocateMemErr"	    ,    // 23 //Error while allocating the memory space
	"Ixud_InstallEventErr"	    ,    // 24 //Error while installing the interrupt event
	"Ixud_InstallIrqErr"	    ,    // 25 //Error while installing the interrupt IRQ
	"Ixud_RemoveIrqErr"		    ,    // 26 //Error while removing the interrupt IRQ
	"Ixud_ClearIntCountErr"	    ,    // 27 //Error while the clear interrupt count
	"Ixud_GetSysBufferErr"	    ,    // 28 //Error while retrieving the system buffer
	"Ixud_CreateEventErr"	    ,    // 29 //Error while create the event
	"Ixud_UnSupportedResolution",    // 30 //Resolution not supported
	"Ixud_CreateThreadErr"	    ,    // 31 //Error while create the thread
	"Ixud_ThreadTimeOutErr"	    ,    // 32 //Thread timeout error
	"Ixud_FIFOOverFlowErr"	    ,    // 33 //FIFO overflow error
	"Ixud_FIFOTimeOutErr"	    ,    // 34 //FIFO timeout error
	"Ixud_GetIntInstStatus"	    ,    // 35 //Retrieves the status of the interrupt installation
	"Ixud_GetBufStatus"		    ,    // 36 //Retrieves the status of the system buffer
	"Ixud_SetBufCountErr"	    ,    // 37 //Error while setting the buffer count
	"Ixud_SetBufInfoErr"        ,    // 38 //Error while setting the buffer data
	"Ixud_FindCardIDErr"	    ,    // 39 //Card ID code could not be found
	"Ixud_EventThreadErr"	    ,    // 40 //Event Thread error
	"Ixud_AutoCreateEventErr"   ,    // 41 //Error while automatically creating an event
	"Ixud_RegThreadErr"		    ,    // 42 //Register Thread error
	"Ixud_SearchEventErr"	    ,    // 43 //Search Event error
	"Ixud_FifoResetErr"		    ,    // 44 //Error while resetting the FIFO
	"Ixud_InvalidBlock"		    ,    // 45 //Invalid EEPROM block
	"Ixud_InvalidAddr"		    ,    // 46 //Invalid EEPROM address
	"Ixud_AcqireSpinLock"	    ,    // 47 //Error while acquiring spin lock
	"Ixud_ReleaseSpinLock"	    ,    // 48 //Error while releasing spin lock
	"Ixud_SetControlErr"	    ,    // 49 //Analog input setting error
	"Ixud_InvalidChannels"	    ,    // 50 //Invalid channel number
	"Ixud_SearchCardErr"	    ,    // 51 //Invalid model number
	"Ixud_SetMapAddressErr"	    ,    // 52 //Error while setting the mapping address
	"Ixud_ReleaseMapAddressErr" ,    // 53 //Error while releasing the mapping address
	"Ixud_InvalidOffset"	    ,    // 54 //Invalid memory offset
	"Ixud_ShareHandleErr"	    ,    // 55 //Open the share memory fail
	"Ixud_InvalidDataCount"	    ,    // 56 //Invalid data count
	"Ixud_WriteEEPErr"		    ,    // 57 //Error while writing the EEPROM
	"Ixud_CardIOErr"            ,    // 58 //CardIO error
	"Ixud_IOErr"                ,    // 59 //MemoryIO error
	"Ixud_SetScanChannelErr"    ,    // 60 //Set channel scan number error
	"Ixud_SetScanConfigErr"     ,    // 61 //Set channel scan config error
	"Ixud_GetMMIOMapStatus"	    ,    // 62 //
};

void CMicroMotion_ICP_D96::myIxudGetErrString(WORD wRet, char* errStr)
{
	if (wRet > Ixud_GetMMIOMapStatus)
	{
		strcpy(errStr, "Can't find D96_ErrStr!");
		return;
	}
		
	strcpy(errStr, saStrIxudErr[wRet]);

}


BOOL CMicroMotion_ICP_D96::InitMotion()
{
	//MyTrace(PRT_DEVICE, "%s:%d\n", __FILE__, __LINE__);
	
	CString strTemp;
	char	strErr[256] = "";
	WORD 	wTotalBoards = 0;

	//------------------------------------
	// Initial the resource for  Driver
	
	// Ixud_DriverInit은 UniDAQ 관련 모든 코드중에 가장 먼저 호출되어야 한다. 
	// system의 리소스를 체크해서 설치된 DAQ board 갯수를 획득. 
	WORD wRet=Ixud_DriverInit(&wTotalBoards);
	if(wRet != Ixud_NoErr)
	{
		myIxudGetErrString(wRet, strErr);
		strTemp.Format("\nPIO_D96 InitMotion(): Ixud_DriverInit() Error! err=%d(%s)", wRet, strErr);
		ERR.Set(DIO_INIT_ERR, strTemp);
#ifndef __PC_TEST__
		return FALSE;
#endif
	} 
	MyTrace(PRT_DEVICE, "\nPIO_D96 InitMotion(): Ixud_DriverInit() OK. err=%d(%s),  wTotalBoards=%d\n", wRet, strErr, wTotalBoards);

//	wBoardNo=0;
//	wOutPortNo=0; 

	//-------------------
	// Config the Port

	strcpy(strErr, "");		// 하기 function에서 error없이 수행되면 strErr가 빈칸이어야 TRACE가 정상출력.
	
	// 다중포트의 I/O 모드를 설정한다. 양방향 I/O 포트만 지원함. 
	wRet=Ixud_SetDIOModes32(0, 				// BoardNo   		0: First board
							0x0FFF );		// DioModeMask		CN1 Port 0,1,2 	CN2 Port 3,4,5
											// 					CN3 Port 6,7,8  CN4 Port 9,10,11
											// 					BitVal=0: Input,  BitVal=1: Output
											//      => Port0~ Port11까지 1(0x0fff)이므로 (12 port * 8 = 96 bit) 
											//         PIO_D96은 DioModeMask를 96 bit 모두 Output전용으로 설정한 것임. 
											//         (refer from 'ICP DAS UNIDAQ' Driver DLL User Manual')
	if(wRet != Ixud_NoErr)
	{
		myIxudGetErrString(wRet, strErr);
		strTemp.Format("PIO_D96: Ixud_SetDIOModes32() Error! err=%d(%s)\n", wRet, strErr);
		ERR.Set(DIO_INIT_ERR, strTemp);
#ifndef __PC_TEST__
		return FALSE;
#endif
	} 
	MyTrace(PRT_DEVICE, "PIO_D96 InitMotion(): Ixud_SetDIOModes32() OK. err=%d(%s)\n", wRet, strErr);

/*
	// 이 부분은 CManualTestDlg::OnInitDialog()에서 MM.m_pMicroM->InitMotion() 수행 이후에 
	// MM.Break()를 호출하므로 불필요함. MM.Break()에서 하기 코드 수행함.	
 
	//-------------------
	// Write DO Value   
	
	for( int i=0; i<=11; i++)
	{
		wRet=Ixud_WriteDO(0,i,0x00);
		if(wRet != Ixud_NoErr)
		{
			myIxudGetErrString(wRet, strErr);
			strTemp.Format("PIO_D96 InitMotion(): Ixud_WriteDO(i=%d) Error! err=%d(%s)\n", i, wRet, strErr);
			ERR.Set(DIO_INIT_ERR, strTemp);
#ifndef __PC_TEST__
			return FALSE;
#endif
		} 
	}
*/
	return TRUE;
}

BOOL CMicroMotion_ICP_D96::CloseMotion()
{
	CString strTemp;
	char	strErr[256] = "";

	WORD wRet=Ixud_WriteDO(0,0,0x00);
	if(wRet !=Ixud_NoErr)
	{
		myIxudGetErrString(wRet, strErr);
		strTemp.Format("PIO_D96 CloseMotion(): Ixud_WriteDO() Error! err=%d(%s)", wRet, strErr);
		ERR.Set(DIO_EXIT_ERR, strTemp);
#ifdef __PC_TEST__
		ErrMsg(-1, TRUE);		// Err를 Trace만 출력
#else
		return FALSE;
#endif
	}

	strcpy(strErr, "");
	wRet=Ixud_DriverClose();
	if(wRet !=Ixud_NoErr)
	{
		myIxudGetErrString(wRet, strErr);
		strTemp.Format("PIO_D96 CloseMotion(): Ixud_DriverClose() Error! err=%d(%s)", wRet, strErr);
		ERR.Set(DIO_EXIT_ERR, strTemp);
#ifdef __PC_TEST__
		return FALSE;
#endif
	}
	MyTrace(PRT_DEVICE, "PIO_D96 CloseMotion() OK!.  err=%d(%s)\n", wRet, strErr);

	return TRUE;	
}

BOOL CMicroMotion_ICP_D96::TranceiveCmd(UINT order, psCMD psCmd)
{
	MyTrace(PRT_DEVICE, "ICP_D96: TranceiveSeq(): order = %d      ", order);
			
	// Data Tranfer
	UINT ret=0, mask_count=0, i=0;
	WORD wRet=Ixud_NoErr;

	WORD  BitNo[128]; // for index , TOTAL_OUTPUT_NUM TOTAL_INPUT_NUM ?
	BYTE  Buf[128];   // for data  TOTAL_OUTPUT_NUM TOTAL_INPUT_NUM ?
	WORD  BitNum=0;	  // data count
	//DWORD dwVal;

	ZeroMemory(BitNo, sizeof(WORD)*(sizeof(BitNo)/sizeof(WORD)));
	ZeroMemory(Buf, sizeof(BYTE)*(sizeof(Buf)/sizeof(BYTE)));

	CString str;
	char	strErr[256] = "";
	BOOL	bPortNo, bBitNo, bBitVal;
	int 	bit, port;

	switch(psCmd->cmd)
	{
		case ENDCMD:
			MyTrace(PRT_DEVICE, "\n");
			return TRUE;
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


		case BYTE_TRS:
			wRet=Ixud_WriteDO( 0, 				// WORD  wBoardNo=0 : First Board
							psCmd->PortNo, 		// WORD  wPortNo 
							psCmd->byteVal);	// DWORD dwDOVal
			if((wRet != Ixud_NoErr) 							// 2018.02.13 성능을 위해 Error 이거나 
					|| (g_wPrintState & (0x01 << PRT_DEVICE)))  //        print on 일때에만 string을 생성
			{
				myIxudGetErrString(wRet , strErr);
				str.Format("wRet=%d(%s) Ixud_WriteDO(bd=0, PortNo=%d, byteVal=%d(0x%02x))", 
							wRet, strErr, psCmd->PortNo, 
							psCmd->byteVal, psCmd->byteVal );
				MyTrace(PRT_DEVICE, "\n%s\n", str);
				if(wRet != Ixud_NoErr) {
					ERR.Set(INVALID_RETURN_DIOOUTBYTE, str);
			#ifndef __PC_TEST__
					return FALSE;
			#endif
				}
			}

			// 전송이 된게 확실하면 baPrevVal[bPortNo]로 백업한다.
			baPrevPortVal[psCmd->PortNo] = psCmd->byteVal;

#if 0
			// for Test .  write하자마자 다시 읽어본다.  제대로 write되었는지 확인
			wRet = Ixud_SoftwareReadbackDO(0, psCmd->PortNo, &dwVal);
			myIxudGetErrString(wRet , strErr);
			str.Format("wRet=%d(%s) Ixud_SoftwareReadbackDO(bd=0, port=%d): dwVal=0x%02X,  baPrevPortVal[%d]=0x%02X  ", 
							wRet, strErr, psCmd->PortNo, dwVal, psCmd->PortNo, baPrevPortVal[psCmd->PortNo]  );
			MyTrace(PRT_DEVICE, "%s\n", str);
#endif
			break;

		case MBIT_TRS:
		{	
			BYTE	bPortNum[D96_TOTAL_OUTPUT_PORT_NUM];	// 12개
			FillMemory(bPortNum, sizeof(BYTE)*(sizeof(bPortNum)/sizeof(BYTE)), -1);

			BYTE	baPortVal[D96_TOTAL_OUTPUT_PORT_NUM];	// 12개
			FillMemory(bPortNum, sizeof(BYTE)*(sizeof(bPortNum)/sizeof(BYTE)), -1);


			// wBitNum 갯수 확인 등의 프린트 준비, 읽어와야 할 port가 뭔지 확인, 
			for(bit=0; bit<D96_TOTAL_OUTPUT_NUM; bit++)
			{
				// bit;							// index (bit location)  : 0~95
				// psCmd->mbitBuf[bit];			// data  (bit data)      : 96개의 0또는 1 data
				
				if(psCmd->mbitBuf[bit] != -1)
				{
					// for print
					BitNo[BitNum] = bit;				// index (bit location)
					Buf[BitNum] = psCmd->mbitBuf[bit];	// data  (bit data)
					BitNum++;							// data count

					bPortNo = bit / 8;
					bPortNum[bPortNo] = bPortNo;		// 값이 할당이 안 된 port는 -1이어야 함
				}
			}

			//-----------------
			// mbitBuf print
			
			// 2018.02.08.  mbitBuf 프린트하는데 시간이 많이 걸리므로 
			//             prt off 상태라면 아래코드를 아예 진입하지 않도록 수정.
			if (g_wPrintState & (0x01 << PRT_DEVICE)) // check print bit_mask
			{
				char	str[2048] = "";	
				char	*pStr = str;
				if (BitNum >= 8)		// 8개 이하면 라인을 바꾸지 않고 BitNo[]를 붙여서 출력
					MyTrace(PRT_DEVICE, "\n");

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
				
				//pStr += sprintf(pStr, "\n          0 1 2 3 4 5 6 7 8 9  0 1 2 3 4 5 6 7 8 9\n");
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

				MyTrace(PRT_DEVICE, "%s", str);		// 출력문이 512바이트를 넘을 수(BitNum 약 70개 이상시..) 있으므로  MyTrace() 사용
			}
			

			//------------------------------------------------------------
			// BitNum이 8개 미만이면 mbit를 bit 단위로 바꾸어서 전송 한다.
			if (BitNum < 8)
			{
				for(bit=0; bit < D96_TOTAL_OUTPUT_NUM; bit++)		// < 96
				{
					if(psCmd->mbitBuf[bit] != -1)
					{
						// bit					// index (bit location)  : 0~95
						// psCmd->mbitBuf[bit]	// data  (bit data)      : 96개의 0또는 1 data
						bPortNo = bit / 8;
						bBitNo = bit % 8;
						bBitVal =  (psCmd->mbitBuf[bit]) ? 1 : 0;

						wRet=Ixud_WriteDOBit(0, 			// wBoardNo=0 : First Board
											bPortNo,		// wPortNo
											bBitNo,			// wBitNo
											bBitVal);		// wDOVal
						if((wRet != Ixud_NoErr) 							// 2018.02.13 성능을 위해 Error 이거나 
								|| (g_wPrintState & (0x01 << PRT_DEVICE)))  //        print on 일때에만 string을 생성
						{
							myIxudGetErrString(wRet , strErr);
							str.Format("wRet=%d(%s) Ixud_WriteDOBit(bd=0, orgBit=%d(Port=%d BitNo=%d), bBitVal=%d)", 
										wRet, strErr, bit,
										bPortNo, bBitNo, bBitVal);
							MyTrace(PRT_DEVICE, "%s\n", str);
							if(wRet != Ixud_NoErr) {
								ERR.Set(INVALID_RETURN_DIOOUTBIT, str);
						#ifndef __PC_TEST__
								return FALSE;
						#endif
							}
						}

						// bit data가 1인 경우에만 bit 위치를 On으로 설정	
						if (bBitVal) 		
							baPrevPortVal[bPortNo] |= (0x01 << bBitNo);	
			
						// bit data가 1이 아닌 경우, bit 위치를 Off로 설정
						else
							baPrevPortVal[bPortNo] &= ~(0x01 << bBitNo);	
#if 0
						// for Test .  write하자마자 다시 포트값으로 읽어본다.  제대로 write되었는지 확인
						wRet = Ixud_SoftwareReadbackDO(0, bPortNo, &dwVal);
						myIxudGetErrString(wRet , strErr);
						str.Format("wRet=%d(%s) Ixud_SoftwareReadbackDO(bd=0, port=%d): dwVal=0x%02X, baPrevPortVal[%d]=0x%02X  ", 
										wRet, strErr, bPortNo, dwVal, bPortNo, baPrevPortVal[bPortNo]);
						MyTrace(PRT_DEVICE, "%s\n", str);
#endif

					}
				}

				MyTrace(PRT_DEVICE, "\n");
				return TRUE;
			}


			//-----------------------------------------------
			// 8 bit 이상이면  byte 단위로 묶어서  전송한다.
			
			// psCmd->mbitBuf[] 96개 array의 write하고 싶은  비트 위치에 전송할 값만 셋팅하여 호출하면
			// 아래와 같이  12개의 Port별 write 값으로 바꾸어서 12회 Ixud_WriteDO를 호출하여 최대 12 byte를 보낸다.
			// 각 바이트당, write할 port번호, write할 byte값으로 재배치하여 Ixud_WriteDO를 호출 (1회 1 byte)
	
			// 12 byte (96 bit) 중의  전송해야 할 DO 포트의 byte값을 read.
			// 바꿀 비트 이외의 값을 overwrite하면 안 되므로 이전값을 확보해야 한다.
		/*	
		 *	2018.02.12 Ixud_SoftwareReadbackDO() 호출시 속도저하 문제로 삭제
			for(port=0; port < D96_TOTAL_OUTPUT_PORT_NUM; port++)		// 12개
			{
				if (bPortNum[port] != (byte)(-1))		// portNum이 -1이 아니면 전송해야 하는 포트
				{
					wRet = Ixud_SoftwareReadbackDO(0, port, &dwVal);
					baPrevPortVal[port] = (byte)dwVal;
					myIxudGetErrString(wRet , strErr);
					str.Format("wRet=%d(%s) Ixud_SoftwareReadbackDO(bd=0, port=%d): dwVal=0x%08X => baPrevPortVal[%d]=0x%02X ", 
									wRet, strErr, port, dwVal, port, baPrevPortVal[port]);
					MyTrace(PRT_DEVICE, "%s\n", str);
					if(wRet != Ixud_NoErr) {
						ERR.Set(INVALID_RETURN_DIOOUTBIT, str);
					#ifndef __PC_TEST__
						return FALSE;
					#endif
					}
				}
			}
		*/
			
			// 기존 baPrevPortVal[bPortNo]을 바탕으로  
			// 96개 mbitBuf[]를 참고하여 12개의 baPortVal[bPortNo]을 재구성한다. 
			//for(port=0; port < D96_TOTAL_OUTPUT_PORT_NUM; port++)		// 12개
			//	baPortVal[port] = baPrevPortVal[port];
			memcpy(baPortVal, baPrevPortVal, sizeof(baPortVal));

			for(bit=0; bit<D96_TOTAL_OUTPUT_NUM; bit++)		// 96
			{
				// bit;							// index (bit location)  : 0~95
				// psCmd->mbitBuf[bit];			// data  (bit data)      : 96개의 0또는 1 data
				
				bPortNo = bit / 8;
				bBitNo = bit % 8;
	
				// 불필요하게 baPortVal[bPortNo]을 write하지 않기 위해 
				// 전송할 포트가 아니라면 넘어간다. 
				if (bPortNum[bPortNo] == (byte)(-1))		
					continue;		

				// baPrevPortVal[bPortNo]을 바탕으로 새로 baPortVal을 만든다.
				if(psCmd->mbitBuf[bit] != -1)
				{
					// bit data가 1인 경우에만 bit 위치를 On으로 설정	
					if (psCmd->mbitBuf[bit]) 		
						baPortVal[bPortNo] |= (0x01 << bBitNo);	
	
					// bit data가 1이 아닌 경우, bit 위치를 Off로 설정
					else
						baPortVal[bPortNo] &= ~(0x01 << bBitNo);	
				}
			}
	
			// 12개의 baPortVal[port]중 전송해야할 값을  D96보드로 전송한다.
			for(port=0; port < D96_TOTAL_OUTPUT_PORT_NUM; port++)		// 12개
			{
				if (bPortNum[port] != (byte)(-1))		// portNum이 -1이 아니면 전송해야 하는 포트
				{
					int	bitPos = port * 8;	// mbitBuf 해당 포트의 
					wRet = Ixud_WriteDO(0, port, baPortVal[port]);
	
					if((wRet != Ixud_NoErr) 							// 2018.02.13 성능을 위해 Error 이거나 
							|| (g_wPrintState & (0x01 << PRT_DEVICE)))  //        print on 일때에만 string을 생성
					{
						myIxudGetErrString(wRet , strErr);
						str.Format("wRet=%d(%s) Ixud_WriteDO(bd=0, port=%d, baPortVal[%d]=0x%02X ) ", 
										wRet, strErr, port, port, baPortVal[port]);
						MyTrace(PRT_DEVICE, "%s\n", str);
						if(wRet != Ixud_NoErr) 
						{
							ERR.Set(INVALID_RETURN_DIOOUTBIT, str);
						#ifndef __PC_TEST__
							return FALSE;
						#endif
						}
					}
					// 전송이 된게 확실하면 baPrevPortVal[port]로 백업한다.
					baPrevPortVal[port] = baPortVal[port];	

#if 0
					// for Test .  write하자마자 다시 읽어본다.  제대로 write되었는지 확인
					wRet = Ixud_SoftwareReadbackDO(0, port, &dwVal);
					myIxudGetErrString(wRet , strErr);
					str.Format("wRet=%d(%s) Ixud_SoftwareReadbackDO(bd=0, port=%d): dwVal=0x%02X, baPrevPortVal[%d]=0x%02X", 
									wRet, strErr, port, dwVal, port, baPrevPortVal[port]);
					MyTrace(PRT_DEVICE, "%s\n", str);
#endif
				}
			}
			MyTrace(PRT_DEVICE, "\n");
		}
	
			break;


		case BIT_TRS:
			bPortNo = psCmd->BitNo / 8;
			bBitNo = psCmd->BitNo % 8;
			bBitVal = psCmd->byteVal;

			wRet=Ixud_WriteDOBit(0, 			// WORD wBoardNo=0 : First Board
								bPortNo,		// WORD wPortNo
								bBitNo,			// WORD wBitNo
								bBitVal);		// WORD wDOVal
			if(wRet != Ixud_NoErr) // 2018.02.13 
			{
				myIxudGetErrString(wRet , strErr);
				str.Format("wRet=%d(%s) Ixud_WriteDOBit(bd=0, orgBit=%d(Port=%d BitNo=%d), bBitVal=%d)", 
									wRet, strErr, psCmd->BitNo,
									bPortNo, bBitNo, bBitVal);
				MyTrace(PRT_DEVICE, "\n%s\n", str);

				ERR.Set(INVALID_RETURN_DIOOUTBIT, str);
		#ifndef __PC_TEST__
				return FALSE;
		#endif
			}
			else	// Success 
			{
				if (g_wPrintState & (0x01 << PRT_DEVICE)) // check print bit_mask
				{
					myIxudGetErrString(wRet , strErr);
					str.Format("wRet=%d(%s) Ixud_WriteDOBit(bd=0, orgBit=%d(Port=%d BitNo=%d), bBitVal=%d)", 
										wRet, strErr, psCmd->BitNo,
										bPortNo, bBitNo, bBitVal);
					MyTrace(PRT_DEVICE, "\n%s\n", str);

				}
			}

			// bit data가 1인 경우에만 bit 위치를 On으로 설정	
			if (bBitVal) 		
				baPrevPortVal[bPortNo] |= (0x01 << bBitNo);	
	
			// bit data가 1이 아닌 경우, bit 위치를 Off로 설정
			else
				baPrevPortVal[bPortNo] &= ~(0x01 << bBitNo);	

#if 0
			// for Test .  write하자마자 다시 읽어본다.  제대로 write되었는지 확인
			wRet = Ixud_SoftwareReadbackDO(0, bPortNo, &dwVal);
			myIxudGetErrString(wRet , strErr);
			str.Format("wRet=%d(%s) Ixud_SoftwareReadbackDO(bd=0, port=%d): dwVal=0x%02X, baPrevPortVal[%d]=0x%02X ", 
							wRet, strErr, bPortNo, dwVal, bPortNo, baPrevPortVal[bPortNo]);
			MyTrace(PRT_DEVICE, "%s\n", str);
#endif
			break;


		case BYTE_RCV:
			DWORD	dwReadVal;
			wRet=Ixud_ReadDI(0, 				// WORD  wBoardNo=0 : First Board
					psCmd->PortNo, 				// WORD  wPortNo
					&dwReadVal);				// DWORD *dwDIVal
			psCmd->byteVal = (BYTE) dwReadVal;
			if((wRet != Ixud_NoErr) 							// 2018.02.13 성능을 위해 Error 이거나 
					|| (g_wPrintState & (0x01 << PRT_DEVICE)))  //        print on 일때에만 string을 생성
			{
				myIxudGetErrString(wRet , strErr);
				str.Format("wRet=%d(%s) Ixud_ReadDI(bd=0, PortNo=%d, byteVal=0x%02x)", 
								wRet, strErr, psCmd->PortNo, psCmd->byteVal);
				MyTrace(PRT_DEVICE, "\n%s\n", str);
				if(wRet != Ixud_NoErr)
				{
					ERR.Set(INVALID_RETURN_DIOINPBYTE, str);
				#ifndef __PC_TEST__
					return FALSE;
				#endif
				}
			}

			break;

	}// switch



	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CMicroMotion_ICP_D64는 별도 파일로 분리 (MicroMotion_ICP_D64.cpp)
//////////////////////////////////////////////////////////////////////

/*
//////////////////////////////////////////////////////////////////////
// CMicroMotion_ICP_D64
//////////////////////////////////////////////////////////////////////

CMicroMotion_ICP_D64::CMicroMotion_ICP_D64()
{

}

CMicroMotion_ICP_D64::~CMicroMotion_ICP_D64()
{

}

BOOL CMicroMotion_ICP_D64::InitMotion()
{

	return TRUE;
}

void CMicroMotion_ICP_D64::CloseMotion()
{
}

BOOL CMicroMotion_ICP_D64::TranceiveSeq(psSEQ psSeq)
{
	return TRUE;
}
*/
