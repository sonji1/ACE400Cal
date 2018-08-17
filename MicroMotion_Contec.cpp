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
	// CONTEC ���� �ʱ�ȭ
	// : Drv �ؿ� Group �ؿ� Board �� �ܰ� ������.
	// : ���⼱ ���� ����̹��̹Ƿ�, 2���� ���忡 ���ؼ� 2���� �׷����� �����Ѵ�.

	//WORD DrvNo[CONTEC_BOARD_NUM]={ CONTEC_BOARD_DRIVER_NO, CONTEC_BOARD_DRIVER_NO };
	//WORD GrpNo[CONTEC_BOARD_NUM]={ 1, 2 };// 1 : INPUT(DI_64T2-PCI),  2 : OUTPUT(DO-128T2-PCI)

	//son 2017.04.21 ���� ���� PC�� Input�� DIO001, Output�� DIO000 ���� ���� ����
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
			ErrMsg(-1, TRUE);		// Err�� Trace�� ���
			//AfxMessageBox(str);

			// to be used after debugging!
			//return FALSE;		
		}
	}
/*
	// ��»��� �ʱ�ȭ
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
			ErrMsg(-1, TRUE);		// Err�� Trace�� ���

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
			// psSeq->mbitBuf[order][] 128�� array�� write�ϰ� ����  ��Ʈ ��ġ�� ������ ���� �����Ͽ� ȣ���ϸ�
			// �Ʒ��� ���� write�� data count�� BitNum�� bit location�� BitNo[] �׸���
			// bit data�� Buf[]�� ���ġ�Ͽ� DioOutMultiBit()�� ȣ���ؼ� DIO�� �����ش�.

			// Sorting
			ZeroMemory(BitNo, sizeof(WORD)*(sizeof(BitNo)/sizeof(WORD)));
			ZeroMemory(Buf, sizeof(BYTE)*(sizeof(Buf)/sizeof(BYTE)));
			BitNum=0, mask_count=0;		//BitNum, mask_count ��� 128�� ���� �� ����.

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
					
					// 2018.02.08.  mbitBuf ����Ʈ�ϴµ� �ð��� ���� �ɸ��Ƿ� 
					//             prt off ���¶�� �Ʒ��ڵ带 �ƿ� �������� �ʵ��� ����.
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
							// �� �ڵ�� BitNo�� �� 0���� �������� ���� �� �����Ƿ� �ǹ� ����.
							//pStr += sprintf(pStr, "          0 1 2 3 4 5 6 7 8 9  0 1 2 3 4 5 6 7 8 9");
					#endif						
							pStr += sprintf(pStr, "Buf[]=  ");
							for (bit = 0; bit < BitNum; bit++)
							{
								if (bit != 0 && bit % 20 == 0)	// 20�� ��� �� �ٲٱ�
									pStr += sprintf(pStr, "\n        ");
								if (bit % 10 == 0)				// 10�� ��� �� ĭ ����
									pStr += sprintf(pStr, " ");
								pStr += sprintf(pStr, " %d", Buf[bit]); 
							}
							pStr += sprintf(pStr, "\n");
						#ifndef __PRT_OFF__
							MyTrace(PRT_DEVICE, "%s\n", str);		// ��¹��� 512����Ʈ�� ���� ��(BitNum �� 70�� �̻��..) �����Ƿ�  MyTrace() ���
						#endif
						}
						else 	// BitNum 8 ����
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
			// psSeq->mbitBuf[order]�� 128�� array�� read�ϰ� ����  ��Ʈ ��ġ�� (-1)�� �ƴ� ���� �����ؼ� ȣ���ϸ� 
			// �Ʒ��� ���� read�� data count�� BitNum�� bit location�� BitNo[]���� ���ġ�ϰ�
			// DioInpMultiBit()�� ȣ���ؼ� DIO�κ��� Buf[]�� �о�� ���� psSeq->mbitBuf[order]�� 
			// Buf[]���� write�Ͽ� ���� �ش�.

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
			// DLY cmd�� delay���� BYTE byteBuf[order]���� WORD mbitBuf[order][0] ���� ��ü.
			// 256 �̻��� ���� �Ҵ��� �ȵǴ� ���� ���� ���� ����.
			if(psSeq->mbitBuf[order][0] != 0)	// ���� 0�̸� delay �� �ʿ� ����
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
