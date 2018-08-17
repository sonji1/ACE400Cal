// MicroMotion_Contec.cpp: implementation of the CMicroMotion_Contec class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ACE400Cal.h"
#include "MicroMotion_Contec.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//HANDLE	m_hDIODrv[CONTEC_BOARD_NUM];
CMicroMotion_Contec		g_MicroM_Contec;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMicroMotion_Contec::CMicroMotion_Contec()
{

}

CMicroMotion_Contec::~CMicroMotion_Contec()
{

}

BOOL CMicroMotion_Contec::InitMotion()
{
	// CONTEC 보드 초기화
	// : Drv 밑에 Group 밑에 Board 로 단계 구성됨.
	// : 여기선 단일 드라이버이므로, 2장의 보드에 대해서 2개의 그룹으로 구분한다.

	//WORD DrvNo[CONTEC_BOARD_NUM]={ CONTEC_BOARD_DRIVER_NO, CONTEC_BOARD_DRIVER_NO };
	//WORD GrpNo[CONTEC_BOARD_NUM]={ 1, 2 };// 1 : INPUT(DI_64T2-PCI),  2 : OUTPUT(DO-128T2-PCI)

	//son 2017.04.21 현재 설정 PC에 Input은 DIO001, Output은 DIO000 으로 잡혀 있음
	char	szDeviceName[CONTEC_BOARD_NUM][10] = {"DIO001", "DIO000" }; 


	int Ret=NO_ERR;
	for(int i=0; i<CONTEC_BOARD_NUM; i++)
	{
//		if(dwRet=DioOpen(&m_hDIODrv[i], DrvNo[i], GrpNo[i]) != NO_ERR)
		Ret = DioInit(szDeviceName[i], (short *)&m_hDIODrv[i]);
		if(Ret != NO_ERR)
		{
			CString str;
			char	szError[256] = "";
			DioGetErrorString(Ret , szError);
			str.Format("MicroMotion_Contec(): DioInit(%s) Error! err=%ld(%s)!", szDeviceName[i], Ret, szError);

			ERR.Set(DIO_INIT_ERR, str);
			ErrMsg(-1, TRUE);		// Err를 Trace만 출력
			//AfxMessageBox(str);

			// to be used after debugging!
			//return FALSE;		
		}
	}
/*
	// 출력상태 초기화
	sBREAK	sBreak;
	sBreak.type = ALL_OUT;
	sBreak.Init();
	TranceiveSeq(&sBreak.sSeq);
*/

	return TRUE;
}

BOOL CMicroMotion_Contec::CloseMotion()
{
	int Ret = NO_ERR;
	for(int i=0; i<CONTEC_BOARD_NUM; i++)	
	{
		//DioClose(m_hDIODrv[i]);
		Ret = DioExit((short)m_hDIODrv[i]);
		if(Ret != NO_ERR)	
		{
			CString str;
			char	szError[256] = "";
			DioGetErrorString(Ret , szError);

			str.Format("MicroMotion_Contec(): DioExit(m_hDIODrv[%d]) Error! err=%ld(%s))!", i, Ret, szError);
			//AfxMessageBox(str);
			ERR.Set(DIO_EXIT_ERR, str);
			ErrMsg(-1, TRUE);		// Err를 Trace만 출력

			return FALSE;
		}
	}
	return TRUE;
}

BOOL CMicroMotion_Contec::On(WORD wBitNo)
{
	CString str;
	if(wBitNo<OUTPUT_START || wBitNo>OUTPUT_START+TOTAL_OUTPUT_NUM) 
	{
		str.Format("MicroMotion_Contec(): On(%d) Error! wBitNo=%d, Range(%d <= wbitNo <= %d) Over.",
					wBitNo, wBitNo, OUTPUT_START,  (OUTPUT_START+TOTAL_OUTPUT_NUM) );
	#ifndef __PRT_OFF__
		MyTrace(PRT_DEVICE, "%s\n", str);
	#endif
		ERR.Set(RANGE_OVER, str);
		return FALSE;
	}

	DWORD	dwRet=NO_ERR;
	HANDLE	hDrv=m_hDIODrv[1];

	dwRet=DioOutBit((short)hDrv, wBitNo-OUTPUT_START, 1);
	{
		CString str;
		char	szError[256] = "";
		DioGetErrorString(dwRet , szError);

		str.Format("dwRet=%d(%s), DioOutBit(hDrv=%d, bitNo=%d, val=%d)", dwRet, szError, hDrv, (wBitNo-OUTPUT_START), 1);
	#ifndef __PRT_OFF__
		MyTrace(PRT_DEVICE, "%s\n", str);
	#endif
		if(dwRet != NO_ERR)
		{
			ERR.Set(INVALID_RETURN_DIOOUTBIT, str);
		#ifndef __PC_TEST__
			return FALSE;
		#endif
		}
	}
	return TRUE;
}

BOOL CMicroMotion_Contec::Off(WORD wBitNo)
{
	CString str;
	if ( wBitNo < OUTPUT_START || wBitNo > (OUTPUT_START+TOTAL_OUTPUT_NUM) ) 
	{
		str.Format("Off(%d) Error! wBitNo=%d, Range(%d <= wbitNo <= %d) Over.",
					wBitNo, wBitNo, OUTPUT_START,  (OUTPUT_START+TOTAL_OUTPUT_NUM) );
	#ifndef __PRT_OFF__
		MyTrace(PRT_DEVICE, "%s\n", str);
	#endif
		ERR.Set(RANGE_OVER, str);
		return FALSE;
	}

	DWORD	dwRet=NO_ERR;
	HANDLE	hDrv=m_hDIODrv[1];

	dwRet=DioOutBit((short)hDrv, wBitNo-OUTPUT_START, 0);
	{
		char	szError[256] = "";
		DioGetErrorString(dwRet , szError);

		str.Format("dwRet=%d(%s), DioOutBit(hDrv=%d, bitNo=%d, val=%d)", dwRet, szError, hDrv, (wBitNo-OUTPUT_START), 0);
	#ifndef __PRT_OFF__
		MyTrace(PRT_DEVICE, "%s\n", str);
	#endif
		if(dwRet != NO_ERR)
		{
			ERR.Set(INVALID_RETURN_DIOOUTBIT, str);
		#ifndef __PC_TEST__
			return FALSE;
		#endif
		}
	}

	return TRUE;
}

BOOL CMicroMotion_Contec::IsOn(WORD wBitNo)
{
	CString str;

	if(wBitNo<INPUT_START || wBitNo>INPUT_START+TOTAL_INPUT_NUM) 
	{
		str.Format("IsOn(%d) Error! wBitNo=%d, Range(%d <= wbitNo <= %d) Over.",
					wBitNo, wBitNo, INPUT_START,  (INPUT_START+TOTAL_INPUT_NUM) );
	#ifndef __PRT_OFF__
		MyTrace(PRT_DEVICE, "%s\n", str);
	#endif
		ERR.Set(RANGE_OVER, str);
		return FALSE;
	}

	DWORD	dwRet=NO_ERR;
	HANDLE	hDrv=m_hDIODrv[0];
	BYTE	InpBuf='0';

	dwRet=DioInpBit((short)hDrv, wBitNo-INPUT_START, &InpBuf);
	{
		char	szError[256] = "";
		DioGetErrorString(dwRet , szError);

		str.Format("dwRet=%d(%s), DioInpBit(hDrv=%d, bitNo=%d)", dwRet, szError, hDrv, (wBitNo-INPUT_START));
	#ifndef __PRT_OFF__
		MyTrace(PRT_DEVICE, "%s\n", str);
	#endif
		if(dwRet != NO_ERR)
		{
			ERR.Set(INVALID_RETURN_DIOINPBIT, str);
		#ifndef __PC_TEST__
			return FALSE;
		#endif
		}
	}
	
	if(static_cast<int>(InpBuf) == 0) return FALSE;

	return TRUE;
}

BOOL CMicroMotion_Contec::IsOff(WORD wBitNo)
{
	CString str;
	if(wBitNo < INPUT_START || wBitNo>INPUT_START+TOTAL_INPUT_NUM) 
	{
		str.Format("IsOff(%d) Error! wBitNo=%d, Range(%d <= wbitNo <= %d) Over.",
					wBitNo, wBitNo, INPUT_START,  (INPUT_START+TOTAL_INPUT_NUM) );
	#ifndef __PRT_OFF__
		MyTrace(PRT_DEVICE, "%s\n", str);
	#endif
		ERR.Set(RANGE_OVER, str);
		return FALSE;
	}

	DWORD	dwRet=NO_ERR;
	HANDLE	hDrv=m_hDIODrv[0];
	BYTE	InpBuf='0';

	dwRet=DioInpBit((short)hDrv, wBitNo-INPUT_START, &InpBuf);
	{
		char	szError[256] = "";
		DioGetErrorString(dwRet , szError);

		str.Format("dwRet=%d(%s), DioInpBit(hDrv=%d, bitNo=%d)", dwRet, szError, hDrv, (wBitNo-INPUT_START));
	#ifndef __PRT_OFF__
		MyTrace(PRT_DEVICE, "%s\n", str);
	#endif
		if(dwRet != NO_ERR)
		{
			ERR.Set(INVALID_RETURN_DIOINPBIT, str);
		#ifndef __PC_TEST__
			return FALSE;
		#endif
		}
	}

	if(static_cast<int>(InpBuf) == 1) return FALSE;

	return TRUE;
}

BOOL CMicroMotion_Contec::TranceiveSeq(psSEQ psSeq)
{
	// Data Tranfer
	UINT order=0, ret=0, mask_count=0, i=0;
	DWORD dwRet=NO_ERR;

	WORD  BitNo[128]; // for index , TOTAL_OUTPUT_NUM TOTAL_INPUT_NUM ?
	BYTE  Buf[128];   // for data  TOTAL_OUTPUT_NUM TOTAL_INPUT_NUM ?
	WORD  BitNum=0;	  // data count

	ZeroMemory(BitNo, sizeof(WORD)*(sizeof(BitNo)/sizeof(WORD)));
	ZeroMemory(Buf, sizeof(BYTE)*(sizeof(Buf)/sizeof(BYTE)));

	while(order<MAX_ORDER_NUM)
	{
		switch(psSeq->cmd[order])
		{
		case ENDCMD:

			return TRUE;
			break;

		case BIT_TRS:
//			if(CheckInterlock(psSeq->BitNo[order], psSeq->byteBuf[order]) == FALSE) return FALSE;
			dwRet=DioOutBit((short)m_hDIODrv[1], psSeq->BitNo[order], psSeq->byteBuf[order]);
			{
				CString str;
				char	szError[256] = "";
				DioGetErrorString(dwRet , szError);
				str.Format("dwRet=%d(%s) order=%d DioOutBit(drv=%d, BitNo[%d]=%d, byteBuf[%d]=%d)", 
							dwRet, szError, order, (short)m_hDIODrv[1], order, psSeq->BitNo[order], order, psSeq->byteBuf[order] );
			#ifndef __PRT_OFF__
				MyTrace(PRT_DEVICE, "%s\n", str);
			#endif
				if(dwRet != NO_ERR) {
					ERR.Set(INVALID_RETURN_DIOOUTBIT, str);
				#ifndef __PC_TEST__
					return FALSE;
				#endif
				}
			}
			break;

		case BYTE_TRS:
			dwRet=DioOutByte((short)m_hDIODrv[1], psSeq->PortNo[order], psSeq->byteBuf[order]);
			{
				CString str;
				char	szError[256] = "";
				DioGetErrorString(dwRet , szError);
				str.Format("dwRet=%d(%s) order=%d DioOutByte(drv=%d, PortNo[%d]=%d, byteBuf[%d]=%d(0x%02x))", 
							dwRet, szError, order, (short)m_hDIODrv[1], order, 
							psSeq->PortNo[order], order, psSeq->byteBuf[order], psSeq->byteBuf[order] );
			#ifndef __PRT_OFF__
				MyTrace(PRT_DEVICE, "%s\n", str);
			#endif
				if(dwRet != NO_ERR) {
					ERR.Set(INVALID_RETURN_DIOOUTBYTE, str);
				#ifndef __PC_TEST__
					return FALSE;
				#endif
				}
			}
			break;


		case MBIT_TRS:
			// psSeq->mbitBuf[order][] 128개 array의 write하고 싶은  비트 위치에 전송할 값만 셋팅하여 호출하면
			// 아래와 같이 write할 data count인 BitNum과 bit location인 BitNo[] 그리고
			// bit data인 Buf[]로 재배치하여 DioOutMultiBit()를 호출해서 DIO로 보내준다.

			// Sorting
			ZeroMemory(BitNo, sizeof(WORD)*(sizeof(BitNo)/sizeof(WORD)));
			ZeroMemory(Buf, sizeof(BYTE)*(sizeof(Buf)/sizeof(BYTE)));
			BitNum=0, mask_count=0;		//BitNum, mask_count 모두 128을 넘을 수 없다.

			for(i=0; i<TOTAL_OUTPUT_NUM; i++)
			{
				if(psSeq->mbitBuf[order][i] != -1)
				{
					BitNo[BitNum] = i;						// index (bit location)
					Buf[BitNum] = psSeq->mbitBuf[order][i];	// data  (bit data)
					BitNum++;								// data count
				}
				else mask_count++;	 // mask_count == (128 - BitNum)
			}

			// Exit condition
			if(mask_count < TOTAL_OUTPUT_NUM)
			{
				// Tranmit multi bit
				HANDLE	hDrv=m_hDIODrv[1];
				dwRet = DioOutMultiBit((short)hDrv, 
										(short*)&BitNo[0],	// bit location
										BitNum,				// data count
										&Buf[0]);			// bit data
				{
					char	str[2048] = "";	
					char	*pStr = str;
					char	szError[256] = "";
					DioGetErrorString(dwRet , szError);

					//-----------------
					// mbitBuf print
					
					// 2018.02.08.  mbitBuf 프린트하는데 시간이 많이 걸리므로 
					//             prt off 상태라면 아래코드를 아예 진입하지 않도록 수정.
					WORD  bitMask = 0x01 << PRT_DEVICE;
					if (g_wPrintState & bitMask) // check print bit_mask
					{

						if (BitNum > 8)
						{
							int bit;
							pStr += sprintf(pStr, "dwRet=%d(%s) order=%d DioOutMultiBit(drv=%d, BitNum=%d)\n", dwRet, szError, order, (short)hDrv, BitNum );
					#if 1
							pStr += sprintf(pStr, "BitNo[]=");
							for (bit = 0; bit < BitNum; bit++)
							{
								if (bit != 0 && bit % 20 == 0)
									pStr += sprintf(pStr, "\n        ");
								if (bit % 10 == 0)
									pStr += sprintf(pStr, " ");
								pStr += sprintf(pStr, " %-3d", BitNo[bit]); 
							}
							pStr += sprintf(pStr, "\n");
					#else
							// 이 코드는 BitNo가 꼭 0부터 시작하지 않을 수 있으므로 의미 없음.
							//pStr += sprintf(pStr, "          0 1 2 3 4 5 6 7 8 9  0 1 2 3 4 5 6 7 8 9");
					#endif						
							pStr += sprintf(pStr, "Buf[]=  ");
							for (bit = 0; bit < BitNum; bit++)
							{
								if (bit != 0 && bit % 20 == 0)	// 20개 찍고 줄 바꾸기
									pStr += sprintf(pStr, "\n        ");
								if (bit % 10 == 0)				// 10개 찍고 한 칸 띄우기
									pStr += sprintf(pStr, " ");
								pStr += sprintf(pStr, " %d", Buf[bit]); 
							}
							pStr += sprintf(pStr, "\n");
						#ifndef __PRT_OFF__
							MyTrace(PRT_DEVICE, "%s\n", str);		// 출력문이 512바이트를 넘을 수(BitNum 약 70개 이상시..) 있으므로  MyTrace() 사용
						#endif
						}
						else 	// BitNum 8 이하
						{
							sprintf(pStr, "dwRet=%d(%s) order=%d DioOutMultiBit(drv=%d, BitNum=%d, BitNo[]=%d %d %d %d %d, Buf[]=%d %d %d %d %d)\n", 
								dwRet, szError, order, (short)hDrv, BitNum, 
								BitNo[0], BitNo[1], BitNo[2], BitNo[3], BitNo[4], 
								Buf[0], Buf[1], Buf[2], Buf[3], Buf[4] );
						#ifndef __PRT_OFF__
							MyTrace(PRT_DEVICE, "%s\n", str);
						#endif
						}
					}

					if(dwRet != NO_ERR)
					{ 	ERR.Set(INVALID_RETURN_DIOOUT_MBIT, str);
				#ifndef __PC_TEST__
						return FALSE;
				#endif
					}
				}
			}
			break;

		/////////////////////////////////////////////////////////////////////////////////////////////
		case BIT_RCV:
			dwRet=DioInpBit((short)m_hDIODrv[0], psSeq->BitNo[order], &psSeq->byteBuf[order]);
			{
				CString str;
				char	szError[256] = "";
				DioGetErrorString(dwRet , szError);
				str.Format("dwRet=%d(%s) order=%d DioInpBit(drv=%d, BitNo[%d]=%d, Buf[%d]=%d)", 
								dwRet, szError, order, (short)m_hDIODrv[0], 
								order, psSeq->BitNo[order], order, psSeq->byteBuf[order]);
			#ifndef __PRT_OFF__
				MyTrace(PRT_DEVICE, "%s\n", str);
			#endif
				if(dwRet != NO_ERR)
				{
					ERR.Set(INVALID_RETURN_DIOINPBIT, str);
				#ifndef __PC_TEST__
					return FALSE;
				#endif
				}
			}
			break;
		
		case BYTE_RCV:
			dwRet=DioInpByte((short)m_hDIODrv[0], psSeq->PortNo[order], &psSeq->byteBuf[order]);
			{
				CString str;
				char	szError[256] = "";
				DioGetErrorString(dwRet , szError);
				str.Format("dwRet=%d(%s) order=%d DioInpByte(drv=%d, PortNo[%d]=%d, byteBuf[%d]=%d(0x%02x))", 
							dwRet, szError, order, (short)m_hDIODrv[0], 
							order, psSeq->PortNo[order], order, psSeq->byteBuf[order], psSeq->byteBuf[order] );
			#ifndef __PRT_OFF__
				MyTrace(PRT_DEVICE, "%s\n", str);
			#endif
				if(dwRet != NO_ERR)
				{
					ERR.Set(INVALID_RETURN_DIOINPBYTE, str);
				#ifndef __PC_TEST__
					return FALSE;
				#endif
				}
			}

			break;

		case MBIT_RCV:
			// psSeq->mbitBuf[order]의 128개 array의 read하고 싶은  비트 위치에 (-1)이 아닌 값을 셋팅해서 호출하면 
			// 아래와 같이 read할 data count인 BitNum과 bit location인 BitNo[]으로 재배치하고
			// DioInpMultiBit()를 호출해서 DIO로부터 Buf[]를 읽어온 다음 psSeq->mbitBuf[order]에 
			// Buf[]값을 write하여 돌려 준다.

			// Sorting
			ZeroMemory(BitNo, sizeof(WORD)*(sizeof(BitNo)/sizeof(WORD)));
			ZeroMemory(Buf, sizeof(BYTE)*(sizeof(Buf)/sizeof(BYTE)));
			BitNum=0, mask_count = 0;

			for(i=0; i<TOTAL_INPUT_NUM; i++)
			{
				if(psSeq->mbitBuf[order][i] != -1)
				{
					BitNo[BitNum] = i;
					BitNum++;
				}
				else mask_count++;		// mask_count == (128 - BitNum)
			}

			// Exit condition
			if(mask_count < TOTAL_INPUT_NUM)
			{
				// Receive multi bit
				HANDLE	hDrv=m_hDIODrv[0];
				dwRet = DioInpMultiBit((short)hDrv, (short*)&BitNo[0], BitNum, &Buf[0]);
				{
					CString str;
					char	szError[256] = "";
					DioGetErrorString(dwRet , szError);
					str.Format("dwRet=%d(%s) order=%d DioInpMultiBit(drv=%d, BitNum=%d, BitNo[]=%d %d %d %d %d, Buf[]=%d %d %d %d %d)", 
								dwRet, szError, order, (short)hDrv, BitNum, 
											BitNo[0], BitNo[1], BitNo[2], BitNo[3], BitNo[4],
											Buf[0], Buf[1], Buf[2], Buf[3], Buf[4], Buf[5]);
			#ifndef __PRT_OFF__
					MyTrace(PRT_DEVICE, "%s\n", str);
			#endif
					if(dwRet != NO_ERR)
					{
						ERR.Set(INVALID_RETURN_DIOINP_MBIT, str);
				#ifndef __PC_TEST__
					return FALSE;
				#endif
					}
				}
				for(i=0; i<BitNum; i++) 
					psSeq->mbitBuf[order][BitNo[i]] = Buf[i];
			}

			break;
		
		// DioInpMultiByte( Id , PortNo , PortNum , Data )


		/////////////////////////////////////////////////////////////////////////////////////////////
		case DLY:

			//son170511  
			// DLY cmd의 delay값을 BYTE byteBuf[order]에서 WORD mbitBuf[order][0] 으로 교체.
			// 256 이상의 숫자 할당이 안되는 문제 관련 수정 사항.
			if(psSeq->mbitBuf[order][0] != 0)	// 값이 0이면 delay 할 필요 없음
			{
				if(psSeq->PortNo[order] == sec)			Delay(psSeq->mbitBuf[order][0], sec);
				else if(psSeq->PortNo[order] == msec)	Delay(psSeq->mbitBuf[order][0], msec);
				else if(psSeq->PortNo[order] == usec)	Delay(psSeq->mbitBuf[order][0], usec);

				CString str;
				str.Format("							order=%d Delay()", order );
			#ifndef __PRT_OFF__
				MyTrace(PRT_DEVICE, "%s\n", str);
			#endif
			}

			break;
		}

		order++;
	}

	return TRUE;
}
