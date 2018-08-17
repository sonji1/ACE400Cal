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
		ErrMsg(-1, TRUE);		// Err�� Trace�� ���
//#ifndef __PC_TEST__
		return FALSE;
	}

	if (m_D64[0].InitMotion(0) == FALSE)
	{
		ErrMsg(-1, TRUE);		// Err�� Trace�� ���
		return FALSE;
	}

	if (m_D64[1].InitMotion(1) == FALSE)
	{
		ErrMsg(-1, TRUE);		// Err�� Trace�� ���
		return FALSE;
	}


	return TRUE;
}

BOOL CMicroMotion_ICP::CloseMotion()
{
	if (m_D96.CloseMotion() == FALSE)
	{
		ErrMsg(-1, TRUE);		// Err�� Trace�� ���
//#ifndef __PC_TEST__
		return FALSE;
	}

	if (m_D64[0].CloseMotion(0) == FALSE)
	{
		ErrMsg(-1, TRUE);		// Err�� Trace�� ���
		return FALSE;
	}
	
	if (m_D64[1].CloseMotion(1) == FALSE)
	{
		ErrMsg(-1, TRUE);		// Err�� Trace�� ���
		return FALSE;
	}

	//AfxMessageBox("D96, D64[0], D64[1] Card Closed.", MB_OK | MB_ICONERROR);

	return TRUE;
}

// On, Off�� �ʹ� HW dependent��.  Calibration�̳� ManualTest �� HW ���������� �����ϱ� ���� 
// On, Off ��� ��ſ�  MacroMotion���� sSET_BIT ����� �߰��ϰ� On/Off �ڵ�� �ڸ�Ʈ ó����.
// �� ���Ŀ� HW ��ü ����� �߰� ������.
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
		// psSeq�� Cmd�� �ϳ��� ī���ؼ� �´� ����� ������. 
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
				// 2��° parameter�� nBdId define���� �ƴ϶� d64 id�� ����
				if (m_D64[0].TranceiveCmd(order, &sCmd, 0) == FALSE)	
					return FALSE;
				break;

			case BD_D64_1:
				if (m_D64[1].TranceiveCmd(order, &sCmd, 1) == FALSE)	
					return FALSE;
				break;

		}

		// sGET_VI ����.  
		// _2BYTE_RCV �� ��쿡�� order3�� ����. order4�� ������ �������� �ʰ� ���� �Ʒ��� ���� �Ҵ�.
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

		// sGET_VI ����.  
		// _2BYTE_RCV �� ��쿡�� order3�� ����. order4,5,6�� ������ �������� �ʰ� ���� �Ʒ��� ���� �Ҵ�.
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


// Return code String  (UniDAQ.h ����)

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
	
	// Ixud_DriverInit�� UniDAQ ���� ��� �ڵ��߿� ���� ���� ȣ��Ǿ�� �Ѵ�. 
	// system�� ���ҽ��� üũ�ؼ� ��ġ�� DAQ board ������ ȹ��. 
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

	strcpy(strErr, "");		// �ϱ� function���� error���� ����Ǹ� strErr�� ��ĭ�̾�� TRACE�� �������.
	
	// ������Ʈ�� I/O ��带 �����Ѵ�. ����� I/O ��Ʈ�� ������. 
	wRet=Ixud_SetDIOModes32(0, 				// BoardNo   		0: First board
							0x0FFF );		// DioModeMask		CN1 Port 0,1,2 	CN2 Port 3,4,5
											// 					CN3 Port 6,7,8  CN4 Port 9,10,11
											// 					BitVal=0: Input,  BitVal=1: Output
											//      => Port0~ Port11���� 1(0x0fff)�̹Ƿ� (12 port * 8 = 96 bit) 
											//         PIO_D96�� DioModeMask�� 96 bit ��� Output�������� ������ ����. 
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
	// �� �κ��� CManualTestDlg::OnInitDialog()���� MM.m_pMicroM->InitMotion() ���� ���Ŀ� 
	// MM.Break()�� ȣ���ϹǷ� ���ʿ���. MM.Break()���� �ϱ� �ڵ� ������.	
 
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
		ErrMsg(-1, TRUE);		// Err�� Trace�� ���
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
			// DLY cmd�� delay���� BYTE byteVal���� WORD mbitBuf[0] ���� ��ü.
			// 256 �̻��� ���� �Ҵ��� �ȵǴ� ���� ���� ���� ����.
			if(psCmd->mbitBuf[0] != 0)	// ���� 0�̸� delay �� �ʿ� ����
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
			if((wRet != Ixud_NoErr) 							// 2018.02.13 ������ ���� Error �̰ų� 
					|| (g_wPrintState & (0x01 << PRT_DEVICE)))  //        print on �϶����� string�� ����
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

			// ������ �Ȱ� Ȯ���ϸ� baPrevVal[bPortNo]�� ����Ѵ�.
			baPrevPortVal[psCmd->PortNo] = psCmd->byteVal;

#if 0
			// for Test .  write���ڸ��� �ٽ� �о��.  ����� write�Ǿ����� Ȯ��
			wRet = Ixud_SoftwareReadbackDO(0, psCmd->PortNo, &dwVal);
			myIxudGetErrString(wRet , strErr);
			str.Format("wRet=%d(%s) Ixud_SoftwareReadbackDO(bd=0, port=%d): dwVal=0x%02X,  baPrevPortVal[%d]=0x%02X  ", 
							wRet, strErr, psCmd->PortNo, dwVal, psCmd->PortNo, baPrevPortVal[psCmd->PortNo]  );
			MyTrace(PRT_DEVICE, "%s\n", str);
#endif
			break;

		case MBIT_TRS:
		{	
			BYTE	bPortNum[D96_TOTAL_OUTPUT_PORT_NUM];	// 12��
			FillMemory(bPortNum, sizeof(BYTE)*(sizeof(bPortNum)/sizeof(BYTE)), -1);

			BYTE	baPortVal[D96_TOTAL_OUTPUT_PORT_NUM];	// 12��
			FillMemory(bPortNum, sizeof(BYTE)*(sizeof(bPortNum)/sizeof(BYTE)), -1);


			// wBitNum ���� Ȯ�� ���� ����Ʈ �غ�, �о�;� �� port�� ���� Ȯ��, 
			for(bit=0; bit<D96_TOTAL_OUTPUT_NUM; bit++)
			{
				// bit;							// index (bit location)  : 0~95
				// psCmd->mbitBuf[bit];			// data  (bit data)      : 96���� 0�Ǵ� 1 data
				
				if(psCmd->mbitBuf[bit] != -1)
				{
					// for print
					BitNo[BitNum] = bit;				// index (bit location)
					Buf[BitNum] = psCmd->mbitBuf[bit];	// data  (bit data)
					BitNum++;							// data count

					bPortNo = bit / 8;
					bPortNum[bPortNo] = bPortNo;		// ���� �Ҵ��� �� �� port�� -1�̾�� ��
				}
			}

			//-----------------
			// mbitBuf print
			
			// 2018.02.08.  mbitBuf ����Ʈ�ϴµ� �ð��� ���� �ɸ��Ƿ� 
			//             prt off ���¶�� �Ʒ��ڵ带 �ƿ� �������� �ʵ��� ����.
			if (g_wPrintState & (0x01 << PRT_DEVICE)) // check print bit_mask
			{
				char	str[2048] = "";	
				char	*pStr = str;
				if (BitNum >= 8)		// 8�� ���ϸ� ������ �ٲ��� �ʰ� BitNo[]�� �ٿ��� ���
					MyTrace(PRT_DEVICE, "\n");

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
				
				//pStr += sprintf(pStr, "\n          0 1 2 3 4 5 6 7 8 9  0 1 2 3 4 5 6 7 8 9\n");
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

				MyTrace(PRT_DEVICE, "%s", str);		// ��¹��� 512����Ʈ�� ���� ��(BitNum �� 70�� �̻��..) �����Ƿ�  MyTrace() ���
			}
			

			//------------------------------------------------------------
			// BitNum�� 8�� �̸��̸� mbit�� bit ������ �ٲپ ���� �Ѵ�.
			if (BitNum < 8)
			{
				for(bit=0; bit < D96_TOTAL_OUTPUT_NUM; bit++)		// < 96
				{
					if(psCmd->mbitBuf[bit] != -1)
					{
						// bit					// index (bit location)  : 0~95
						// psCmd->mbitBuf[bit]	// data  (bit data)      : 96���� 0�Ǵ� 1 data
						bPortNo = bit / 8;
						bBitNo = bit % 8;
						bBitVal =  (psCmd->mbitBuf[bit]) ? 1 : 0;

						wRet=Ixud_WriteDOBit(0, 			// wBoardNo=0 : First Board
											bPortNo,		// wPortNo
											bBitNo,			// wBitNo
											bBitVal);		// wDOVal
						if((wRet != Ixud_NoErr) 							// 2018.02.13 ������ ���� Error �̰ų� 
								|| (g_wPrintState & (0x01 << PRT_DEVICE)))  //        print on �϶����� string�� ����
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

						// bit data�� 1�� ��쿡�� bit ��ġ�� On���� ����	
						if (bBitVal) 		
							baPrevPortVal[bPortNo] |= (0x01 << bBitNo);	
			
						// bit data�� 1�� �ƴ� ���, bit ��ġ�� Off�� ����
						else
							baPrevPortVal[bPortNo] &= ~(0x01 << bBitNo);	
#if 0
						// for Test .  write���ڸ��� �ٽ� ��Ʈ������ �о��.  ����� write�Ǿ����� Ȯ��
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
			// 8 bit �̻��̸�  byte ������ ���  �����Ѵ�.
			
			// psCmd->mbitBuf[] 96�� array�� write�ϰ� ����  ��Ʈ ��ġ�� ������ ���� �����Ͽ� ȣ���ϸ�
			// �Ʒ��� ����  12���� Port�� write ������ �ٲپ 12ȸ Ixud_WriteDO�� ȣ���Ͽ� �ִ� 12 byte�� ������.
			// �� ����Ʈ��, write�� port��ȣ, write�� byte������ ���ġ�Ͽ� Ixud_WriteDO�� ȣ�� (1ȸ 1 byte)
	
			// 12 byte (96 bit) ����  �����ؾ� �� DO ��Ʈ�� byte���� read.
			// �ٲ� ��Ʈ �̿��� ���� overwrite�ϸ� �� �ǹǷ� �������� Ȯ���ؾ� �Ѵ�.
		/*	
		 *	2018.02.12 Ixud_SoftwareReadbackDO() ȣ��� �ӵ����� ������ ����
			for(port=0; port < D96_TOTAL_OUTPUT_PORT_NUM; port++)		// 12��
			{
				if (bPortNum[port] != (byte)(-1))		// portNum�� -1�� �ƴϸ� �����ؾ� �ϴ� ��Ʈ
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
			
			// ���� baPrevPortVal[bPortNo]�� ��������  
			// 96�� mbitBuf[]�� �����Ͽ� 12���� baPortVal[bPortNo]�� �籸���Ѵ�. 
			//for(port=0; port < D96_TOTAL_OUTPUT_PORT_NUM; port++)		// 12��
			//	baPortVal[port] = baPrevPortVal[port];
			memcpy(baPortVal, baPrevPortVal, sizeof(baPortVal));

			for(bit=0; bit<D96_TOTAL_OUTPUT_NUM; bit++)		// 96
			{
				// bit;							// index (bit location)  : 0~95
				// psCmd->mbitBuf[bit];			// data  (bit data)      : 96���� 0�Ǵ� 1 data
				
				bPortNo = bit / 8;
				bBitNo = bit % 8;
	
				// ���ʿ��ϰ� baPortVal[bPortNo]�� write���� �ʱ� ���� 
				// ������ ��Ʈ�� �ƴ϶�� �Ѿ��. 
				if (bPortNum[bPortNo] == (byte)(-1))		
					continue;		

				// baPrevPortVal[bPortNo]�� �������� ���� baPortVal�� �����.
				if(psCmd->mbitBuf[bit] != -1)
				{
					// bit data�� 1�� ��쿡�� bit ��ġ�� On���� ����	
					if (psCmd->mbitBuf[bit]) 		
						baPortVal[bPortNo] |= (0x01 << bBitNo);	
	
					// bit data�� 1�� �ƴ� ���, bit ��ġ�� Off�� ����
					else
						baPortVal[bPortNo] &= ~(0x01 << bBitNo);	
				}
			}
	
			// 12���� baPortVal[port]�� �����ؾ��� ����  D96����� �����Ѵ�.
			for(port=0; port < D96_TOTAL_OUTPUT_PORT_NUM; port++)		// 12��
			{
				if (bPortNum[port] != (byte)(-1))		// portNum�� -1�� �ƴϸ� �����ؾ� �ϴ� ��Ʈ
				{
					int	bitPos = port * 8;	// mbitBuf �ش� ��Ʈ�� 
					wRet = Ixud_WriteDO(0, port, baPortVal[port]);
	
					if((wRet != Ixud_NoErr) 							// 2018.02.13 ������ ���� Error �̰ų� 
							|| (g_wPrintState & (0x01 << PRT_DEVICE)))  //        print on �϶����� string�� ����
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
					// ������ �Ȱ� Ȯ���ϸ� baPrevPortVal[port]�� ����Ѵ�.
					baPrevPortVal[port] = baPortVal[port];	

#if 0
					// for Test .  write���ڸ��� �ٽ� �о��.  ����� write�Ǿ����� Ȯ��
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

			// bit data�� 1�� ��쿡�� bit ��ġ�� On���� ����	
			if (bBitVal) 		
				baPrevPortVal[bPortNo] |= (0x01 << bBitNo);	
	
			// bit data�� 1�� �ƴ� ���, bit ��ġ�� Off�� ����
			else
				baPrevPortVal[bPortNo] &= ~(0x01 << bBitNo);	

#if 0
			// for Test .  write���ڸ��� �ٽ� �о��.  ����� write�Ǿ����� Ȯ��
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
			if((wRet != Ixud_NoErr) 							// 2018.02.13 ������ ���� Error �̰ų� 
					|| (g_wPrintState & (0x01 << PRT_DEVICE)))  //        print on �϶����� string�� ����
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
// CMicroMotion_ICP_D64�� ���� ���Ϸ� �и� (MicroMotion_ICP_D64.cpp)
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
