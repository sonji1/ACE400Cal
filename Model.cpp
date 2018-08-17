#include "stdafx.h"
#include "ACE400Cal.h"
#include "Model.h"


CModel g_sModel;


CModel::~CModel()
{


}


void CModel::InitMember() 
{
	m_bIsConExist = FALSE;
	::ZeroMemory(&m_sPinCon, sizeof(m_sPinCon));
	::ZeroMemory(&m_sPinSetup, sizeof(m_sPinSetup));

	//TRACE("CModel::InitMember(): sizeof(m_sPinCon)=%d\n", sizeof(m_sPinCon)); 
	//TRACE("CModel::InitMember(): sizeof(m_sPinSetup)=%d\n", sizeof(m_sPinSetup)); 
}


// ACE400  CChildView1::OnDataLoad_READ_ConRead()  �����ؼ� �ۼ���.
// .CON ������ �����Ѵٸ�(m_bIsConExist =>TRUE) Load�Ѵ�.
// .FPN �� ���ٸ� .CON ���ϵ� ����.(���� ����, "" �� ��) 
BOOL CModel::Load_PinConFile()
{

	FILE 	*fp; 
	char  	fName[200]; 
	CString strTemp, strTemp2;

	::ZeroMemory(&fName, sizeof(fName));
	::ZeroMemory(&m_sPinCon, sizeof(m_sPinCon));
	m_bIsConExist = FALSE;

	// SysInfoView01.m_pStrFilePathJig2 �� �ش�
	strTemp.Format("%s",g_sFile.ACE400_BdlFilePath);  //  ex) C:\ACE400\QC-JIG-S100-BHFlex\4W_YP_S100\OPEN-READ.BDL

	// '.BDL' Ȯ���ڸ� ���� 
	strTemp.TrimRight('l');	strTemp.TrimRight('L');		
	strTemp.TrimRight('d');	strTemp.TrimRight('D');
	strTemp.TrimRight('b');	strTemp.TrimRight('B');
	strTemp.TrimRight('.'); 

	// '.CON' Ȯ���ڸ� �߰�
	//  ex) C:\ACE400\QC-JIG-S100-BHFLEX\4W_YP_S100\OPEN-READ.CON
	strTemp2.Format("%s.CON",strTemp);//sylee150308
	strcat(fName , strTemp2);		

	//---------------------------
 	// .CON ���� open
	fp=fopen(fName,"rt");
	if(fp == NULL){  
		m_bIsConExist = FALSE;
    	return TRUE;		// .CON ������ ��� ������ �ƴ�. �� ���  HW�� userpin No�� �״�� sw Pin���� ���
	}

	//---------------------------
	// .CON ���� ��� read
	int nTotalPin=0, nCheckSum=0, nSum=0;
    fscanf(fp, "%10d  %10d   ", &nTotalPin, &nCheckSum);		//���ɼ�,  checkSum

    // range check1
    if (nTotalPin <= 0 || nTotalPin >= MAX_SWITCH_PIN)
	{
		strTemp.Format("Load_PinConFile(): nTotalPin=%d  Range(0 < nTotalPin < %d) Over!\n", nTotalPin, MAX_SWITCH_PIN);
		ERR.Set(RANGE_OVER, strTemp);
		//ErrMsg(); ERR.Reset();	// Errmsg����� Load_PinConFile() ȣ�� �Ϸ��Ŀ�
		return FALSE;
	}


	//---------------------------
	// .CON ���� data read
	//
	// ACE400�ڵ� nPinC1[i][0]: pinBlockPinNo		nPinC1[i][1]: ����� pinNo	     
	for(int i = 0; i < nTotalPin; i++) //FPN EXIT
	{	 
         fscanf(fp, "%10d %10d   ", &m_sPinCon.saPinNo[i].nSwitch, 		// pinBlockPinNo,  
         							&m_sPinCon.saPinNo[i].nUserJig);  	// �����PinNo(JigPinNo#) 
		 nSum = nSum + m_sPinCon.saPinNo[i].nSwitch + m_sPinCon.saPinNo[i].nUserJig;  
	}
	fclose(fp);


	//---------------------------
	// CheckSum ���
	
	// BdlConverter �ڵ忡�� CBdlConverterDlg::MakeConnectionFile() �� ���� �κ� ����.
	if(nSum > 1000000){
		nSum = nSum / 1000000;
	}
	else if(nSum > 1000){
		nSum = nSum / 1000;
	}
 	if(nSum != nCheckSum){
		strTemp.Format("Load_PinConFile(): *.CON File CheckSumErr! nSum=%d should be same with nCheckSum=%d \n", 
				nSum, nCheckSum);
		ERR.Set(FLAG_FILE_CHECK_SUM, strTemp);
		//ErrMsg(); ERR.Reset();	// Errmsg����� Load_PinConFile() ȣ�� �Ϸ��Ŀ�

		::ZeroMemory(&m_sPinCon, sizeof(m_sPinCon)); // .CON ���� read ���빰�� ����� ����
		return FALSE;
	}


	m_sPinCon.nTotalPin = nTotalPin;
	m_sPinCon.nCheckSum = nCheckSum;
	m_bIsConExist = TRUE;

	return TRUE;
}

// ACE400 CFileSysInfo::LoadSaveSet12(int type) �����ؼ� �ۼ���.
// BDL_PATH+"\\\\SETUP2\\SysSetPin.ini" ������ QD ������ Load
// "C:\\ACE400\\SETUP2\\SysSetPin.ini" ������ QD �̿� ������ Load
BOOL CModel::Load_PinSetupData()
{
	FILE 	*fp, *fp2; 
	char  	fName[200]; 		//for QD ����
	char  	fName2[200]; 		//for QD �̿� ����

	CString strTemp, strTemp2;

	::ZeroMemory(&fName, sizeof(fName));
	::ZeroMemory(&fName2, sizeof(fName2));

	//--------------------------------
	// QD �̿����� ���� Open & Read
	// ex) "C:\\ACE400\\SETUP2\\SysSetPin.ini"
	strcat(fName2, g_sFile.ACE400_PinSetupPath);

	if(!FileExists(fName2)) { 	
		strTemp2.Format("Load_PinSetupData(): File(%s) Not Exist!", fName2);
		ERR.Set(FLAG_FILE_NOT_FOUND, strTemp2); return FALSE; 
	}

	fp2=fopen(fName2,"rt");
	if(fp2 == NULL) { 	
		strTemp2.Format("Load_PinSetupData(): Cannot Open File(%s)!", fName2);
		ERR.Set(FLAG_FILE_CANNOT_OPEN, strTemp2); return FALSE; 
	}

	fscanf(fp2, "%10d\n", &m_sPinSetup.nMaxTotPin); 			// ex) 1
	fscanf(fp2, "%10d\n", &m_sPinSetup.nMaxBoxPin);				// ex) 1	
	fscanf(fp2, "%10d\n", &m_sPinSetup.nPinSwitchMode);			// ex) 2	
	fscanf(fp2, "%10d\n", &m_sPinSetup.nPinType);   			// ex) 2	
	fscanf(fp2, "%10d\n", &m_sPinSetup.nCardA_Reverse);			// ex) 1	
	fscanf(fp2, "%10d\n", &m_sPinSetup.n32k_16k);				// ex) 0	
	fscanf(fp2, "%10d\n", &m_sPinSetup.nCardB_Reverse);			// ex) 1	
	fscanf(fp2, "%10d\n", &m_sPinSetup.nLowerJigStartNo1);		// ex) 0

	fclose(fp2); 


	//----------------------------------
	// QD ���� ���� Open & Read 
	
	// ACE400 BDL Path(Dir+name) + PinSetup ����	
	// ex) "C:\\ACE400\\QC-JIG-S100-BHFlex\\4W_YP_S100\\SETUP2\\SysSetPin.ini"
	strTemp.Format("%s\\SETUP2\\SysSetPin.ini", g_sFile.ACE400_BdlFileDir);
	strcat(fName, strTemp);		

	if(!FileExists(fName)) { 	
		strTemp2.Format("Load_PinSetupData(): File(%s) Not Exist!", fName);
		ERR.Set(FLAG_FILE_NOT_FOUND, strTemp2); return FALSE; 
	} 

	fp=fopen(fName,"rt");
	if(fp == NULL) { 	
		strTemp2.Format("Load_PinSetupData(): Cannot Open File(%s)!", fName);
		ERR.Set(FLAG_FILE_CANNOT_OPEN, strTemp2); return FALSE; 
	}

	int i;
	for (i = 0; i < MAX_BOX; i++)
		fscanf(fp, "%10d\n", &m_sPinSetup.nPinBoxType[i]);	// ex) 1	
																// ex) 1	
																// ex) 1	
																// ex) 1
	fclose(fp);	

	//PinBlockType ���� ������.
	switch (g_sSystem.m_nSystemType)
	{
		case DD:
			for (i = 0; i < MAX_BOX; i++)
				if (m_sPinSetup.nPinBoxType[i] == PIN_QD)
					m_sPinSetup.nPinBoxType[i] = PIN_DD;
			break;

		case SD:  case S100_1:  case S100_2:	case YAMAHA:
			for (i = 0; i < MAX_BOX; i++)
				m_sPinSetup.nPinBoxType[i] = PIN_QD;
			break;
			
		case QD:
		default:
			break;
	}


	//--------------------------
	// nPinMap[] ����
	
	switch (g_sSystem.m_nSystemType)
	{
		case YAMAHA:	// 5
			// ACE400������ ModeSet12.OnPinConvert1_YAMAHA() ����.
			break;

		case S100_2: 	// 4
			// ACE400������ ModeSet12.OnPinConvert1() ����.
			break;

		case DD: 		// 1
			if (m_bIsConExist == FALSE)
				// ACE400������ ModeSet12.OnPinConvert1() ����.
				;
			else
				// ACE400������ ModeSet12.OnPinConvert1_READ(OnPinConvertType1);  ����.
				return PinSetupConvert();
			break;

		case QD:  		// 0
		case SD: 		// 2
		case S100_1:  	// 3
			// ACE400������ ModeSet12.OnPinConvert1_READ(OnPinConvertType1);  ����.
			return PinSetupConvert();
			break;
			 
		default:
			break;

	}


	return TRUE;
}

// nPinMap[] ����
// ACE400 CModeSet12::OnPinConvert1_READ(int nMode) �� �ش�.
BOOL CModel::PinSetupConvert()
{
	CString strTemp;
	int nMaxPin = 0, nMaxPinHalf = 0; 
	int nBoxPinFull = 0, nBoxPinHalf = 0, nBoxPinQuater = 0;
	int nBoxPin[10];	//son �� �ڽ��� Pin ��������
	int nBoxPinSum = 0;
	int i;

	::ZeroMemory(&nBoxPin,sizeof(nBoxPin));


	//son nMaxPin, nMaxPinHalf �� Ȯ��
	switch (m_sPinSetup.nMaxTotPin)
	{
		case TOT_4K: 	// 1
			nMaxPin	= NUM_4K;		// 4096
			break;

		case TOT_8K: 	// 2
			nMaxPin	= NUM_4K * 2;	// 4096 *2
			break;

		case TOT_16K: 	// 3
			nMaxPin	= NUM_4K * 4;	// 4096 *4
			break;

		case TOT_32K: 	// 4
			nMaxPin	= NUM_4K * 8;	// 4096 *8
			break;

		default:
			strTemp.Format("PinSetupConvert(): nMaxTotPin=%d. Range(1~4) Over!", m_sPinSetup.nMaxTotPin);
			ERR.Set(RANGE_OVER, strTemp); 
			return FALSE; 
			break;
	}
	nMaxPinHalf = nMaxPin / 2; 

	// nBoxPinFull, nBoxPinHalf �� Ȯ��
	if (m_sPinSetup.nMaxBoxPin == BOX_2K)	//1
	{
		nBoxPinFull   = NUM_2K;			//2048
		nBoxPinHalf   = NUM_2K /2;		//1024
		nBoxPinQuater = NUM_2K /4;		//512
	}
	else	//2
	{
		nBoxPinFull   = NUM_4K;			//4096
		nBoxPinHalf   = NUM_4K /2;		//2048
		nBoxPinQuater = NUM_4K /4;		//1024
	}


	// nTotalPin Range check

	::FillMemory(nPinMap_NET, sizeof(nPinMap_NET), -1);
	::FillMemory(nPinMap_PBLK, sizeof(nPinMap_PBLK), -1);
	::FillMemory(nPinMap_SWT, sizeof(nPinMap_SWT), -1);


	// .CON ������ �������� �ʴ´ٸ�
	if (m_bIsConExist == FALSE)
	{
		// nBoxPin[nBox2]: �� �ڽ��� Pin End ���� ��� 
		for (int nBox2 = 1; nBox2 <= MAX_BOX; nBox2++)  		// nBox2= nBox+1
		{
			switch (m_sPinSetup.nPinBoxType[nBox2-1])
			{													
				case PIN_SD:	//3 							// BoxType nMaxBoxPin		    box0	box1	box2	box3
					nBoxPinSum = nBoxPinSum + nBoxPinQuater;	// SD       2K nBoxPin[nBox2]	512		1024	1536	2048
					break;										// SD       4K nBoxPin[nBox2]   1024	2048	3072	4096

				case PIN_DD:	//2								
					nBoxPinSum = nBoxPinSum + nBoxPinHalf;		// DD       2K nBoxPin[nBox2]	1024	2048	3072	4096		
					break;                                      // DD       4K nBoxPin[nBox2]   2048	4096	6144	8192

				case PIN_QD:	//1
					nBoxPinSum = nBoxPinSum + nBoxPinFull;		// QD       2K nBoxPin[nBox2]	2048	4096	6144	8192
					break;                                      // QD       4K nBoxPin[nBox2]   4096	8192	12288	16384

				default:
					strTemp.Format("PinSetupConvert(): m_sPinSetup.nPinBoxType[%d]=%d. Range(1~3) Over!", 
							nBox2, m_sPinSetup.nPinBoxType[nBox2]);
					ERR.Set(RANGE_OVER, strTemp); 
					return FALSE; 
					break;
			}

			// 16K ��� �ڽ� ���� ó��    ==> ���� ȿ��?  ������ ����??? 
			if(m_sPinSetup.nMaxTotPin == TOT_16K)	//3
			{
				if(nBox2==2)		//�ι�° �ڽ�.
				{
					//sylee151102   Upper0 QD �̰� Upper1 QD�ΰ��  �Ϻν��۹�ȣ  2k+1�� �ƴ϶� 4k+1  
					if(m_sPinSetup.nPinBoxType[0]==PIN_QD || m_sPinSetup.nPinBoxType[1]==PIN_QD)
					{
						nBoxPinSum=4096*2;			// 8192 ���߸� �Ѵ�.
					}
					else if(m_sPinSetup.nPinBoxType[0]!=PIN_SD && m_sPinSetup.nPinBoxType[1]!=PIN_SD)
					{//sylee161024 

						nBoxPinSum=4096;
					}
				}
			}
			nBoxPin[nBox2] = nBoxPinSum;	//son  ex) nBoxPin[0]= 0, nBoxPin[1]=2048, nBoxPin[2]=4096, 	
											//son  ex)                nBoxPin[3]=6144, nBonPin[4]=8192			

		}

		int nBlkNo =-1, nJigNo = -1, nPinNo = -1;			// ACE400 a, b, No
		for (i = 0; i < nMaxPin; i++)
		{
			nPinNo = i;

			// i�� �ش��ϴ� ��ġ�� i���� write. index�� content ���빰�� ����
			nPinMap_NET[i]	= i;   // net list pin no   


			// PinBlock�� ���--------
			
			int nStep = 0;
			int boxPinNo = -1;		// Box �������� Pin��ȣ
			//son  ????? �̰� ���� �ǹ�????
			for(int nBox2 = 1; nBox2 <= 4; nBox2++)		// nBox2�� Box+1 : Box(0~3), Box2(1~4)
			{
				if( (nPinNo+1) > nBoxPin[nBox2] ){
					nStep=nBox2; 	// nStep ���
				}
				else
					break;
			}		
			boxPinNo = nPinNo - nBoxPin[nStep];		// nPinNo�� boxPinNo�� ����

			
			int cardNo;			// ACE400 Mok1
			int cardPinNo;		// ACE400 Namo,
			int Mok;

			// 64(pin) 
			if(m_sPinSetup.nPinBoxType[nStep]==PIN_DD) {//2
				cardPinNo = boxPinNo  % 64;
				Mok       = cardPinNo / 32;		// Ȧ¦?? 
				boxPinNo  = boxPinNo  * 2;
				if(Mok==1){		// Ȧ���̸� +1 ����. 
					boxPinNo++;
				}		  
			}

			// 32(pin) * 128(card) => Box(4096 pin)
			else if(m_sPinSetup.nPinBoxType[nStep]==PIN_SD){	//3
				cardPinNo = boxPinNo % 32; 
				cardNo    = boxPinNo / 32;  
				boxPinNo  = cardNo*128 +cardPinNo*2;  
			} 

			nPinNo = (nBoxPinFull*nStep)+boxPinNo;		// nPinNo for QD �� �籸��

			////////////////////////////////////////////////////////////////////////////////////////

			if(m_sPinSetup.n32k_16k==1){//3=SD->QD
				if(nPinNo>=(4096*2)){					 
					nPinNo=nPinNo+(4096*2);
				}
			} 

			// ���� ���� i ��ġ (0~nMaxPin����) �� PinBlockNo ���� write.
			nPinMap_PBLK[i] = nPinNo;//****>QD pin no 


			//------------------------------
			// Card Reverse  Mode ó��
			
			int boxNo;				// ACE400 BoxN
			int	newBoxPinNo;		
			int boxCardNo;

			if(m_sPinSetup.nCardA_Reverse == CARD_REVERSE) //2
			{
				// 2K CardReverse: 128(pin) * 16(card) => Box(2048 pin)
				if(m_sPinSetup.nMaxBoxPin == BOX_2K)		//1=2K
				{
				   	boxNo    = nPinNo / NUM_2K;		// BOX_2K�ϱ� 2048
				   	boxPinNo = nPinNo % NUM_2K;

				   	boxCardNo= boxPinNo /128;		//0~15  card
				   	cardPinNo= boxPinNo %128;		//0~127 cardPin
				   	newBoxPinNo= cardPinNo + ((15 - boxCardNo)*128);

					// nPinNo �籸��
				   	nPinNo   = newBoxPinNo + boxNo*NUM_2K;
				
				}
				
				// 4K CardReverse: 128(pin) * 32(card) => Box(4096 pin)
				else	// m_sPinSetup.m_nPinBoxMax== BOX_4K
				{
					boxNo       = nPinNo / NUM_4K;	// BOX_4K�ϱ� 4096
					boxPinNo    = nPinNo % NUM_4K;

					boxCardNo   = boxPinNo / 128;		//0~31  card
					cardPinNo   = boxPinNo % 128;		//0~127 cardPin
					newBoxPinNo = cardPinNo + ((31 - boxCardNo)*128);

					// nPinNo �籸��
					nPinNo = newBoxPinNo + boxNo*NUM_4K;
				}
			}



			//-----------------------------------------
			// Card B Reverse Mode ó��   (������ ����)
			
			//CONNECTOR  REVERSE = 64 PIN REVERSE
			if (m_sPinSetup.nCardB_Reverse==CARD_REVERSE) //2
			{
				cardNo    = nPinNo / 128;		// 1card�� pin128���� ���� �ش� pin�� cardNo (Total pin���� cardNo)
				cardPinNo = nPinNo % 128; 		// card���� pinNo

				if(cardPinNo<64){
					cardPinNo=cardPinNo+64;
				}
				else{
					cardPinNo=cardPinNo-64;//191=127+64
				}
				nPinNo= cardNo*128 + cardPinNo;
			}

			//------------------------------------------
			// Total Pin ������ ����Ͽ� nPinNo ������ 
			//
			//son  ���..
			//son PinMap[2][0] = 0
			//son PinMap[2][1] = 1
			//son PinMap[2][20] = 20
			//son PinMap[2][2047] = 2047
			//son PinMap[2][2048] = 16384		
			//son PinMap[2][2049] = 16385 
			//son PinMap[2][4095] = 18431 
	
			
			int nTemp = nBoxPin[2];				// �ι�° Box�� Pin End ��
			nPinMap_SWT[i] = nPinNo;
			switch (m_sPinSetup.nMaxTotPin)
			{
				case TOT_4K:		//1
					if(nTemp>2048)
						nTemp=2048;		//son ex) nTemp <= 2048�� ����

					//son ex) i�� 2048 �̻��� ��쿡�¿��� +16384
					if(i >= nTemp)
						nPinMap_SWT[i] = nPinNo + (2048 + 4096 + 8192);
					break;

				case TOT_8K:		//2
					if(nTemp>4096)
						nTemp=4096;
					if(i >= nTemp)
						nPinMap_SWT[i] = nPinNo + (4096 + 8192);
					break;

				case TOT_16K:		//3
					if(nTemp>8192)
						nTemp=8192;
					if(i >= nTemp)
						nPinMap_SWT[i] = nPinNo + (8192);
					break;

				case TOT_32K:		//4
				default:
					if(nTemp>(8192*2))
						nTemp=(8192*2);
					if(i >= nTemp)
						nPinMap_SWT[i] = nPinNo;
					break;
			}

		}// for

	}

	// .CON ������ �����Ѵٸ�
	if (m_bIsConExist == TRUE)
	{
		// nBoxPin[nBox2]: �� �ڽ��� Pin End ���� ��� 
		for (int nBox2 = 1; nBox2 <= MAX_BOX; nBox2++)  		// nBox2= nBox+1	
		{
			switch (m_sPinSetup.nPinBoxType[nBox2-1])
			{													
				case PIN_SD:	//3 							// BoxType nMaxBoxPin			box0	box1	box2	box3
					nBoxPinSum = nBoxPinSum + nBoxPinQuater;	// SD       2K nBoxPin[nBox2]	512		1024	1536	2048
					break;										// SD       4K nBoxPin[nBox2] 	1024	2048	3072	4096

				case PIN_DD:	//2								
					nBoxPinSum = nBoxPinSum + nBoxPinHalf;		// DD       2K nBoxPin[nBox2]	1024	2048	3072	4096		
					break;                                      // DD       4K nBoxPin[nBox2] 	2048	4096	6144	8192

				case PIN_QD:	//1
					nBoxPinSum = nBoxPinSum + nBoxPinFull;		// QD       2K nBoxPin[nBox2]	2048	4096	6144	8192
					break;                                      // QD       4K nBoxPin[nBox2] 	4096	8192	12288	16384

				default:
					strTemp.Format("PinSetupConvert(): m_sPinSetup.nPinBoxType[%d]=%d. Range(1~3) Over!", 
							nBox2, m_sPinSetup.nPinBoxType[nBox2]);
					ERR.Set(RANGE_OVER, strTemp); 
					return FALSE; 
					break;
			}
			nBoxPin[nBox2] = nBoxPinSum;	//son  ex) nBoxPin[0]= 0, nBoxPin[1]=2048, nBoxPin[2]=4096,
											//son  ex)                nBoxPin[3]=6144, nBonPin[4]=8192

		}


		int nBlkNo =-1, nJigNo = -1, nPinNo = -1;			// ACE400 a, b, No
		for (i = 0; i < m_sPinCon.nTotalPin; i++)
		{
			nBlkNo= m_sPinCon.saPinNo[i].nSwitch-1;  	// ACE400 a: block no  (1~ 
			nJigNo= m_sPinCon.saPinNo[i].nUserJig-1;  	// ACE400 b: jig no  (1~  


			if(nJigNo < NUM_16K) {//16k    i=0~16k  
				nPinNo = nBlkNo;   		//nSwitchMaxPin  33000    ,  pin no   0~
			}
			// nJigNo�� 16K �̻��̸� nBlkNo���� 8K�� ���� nPinNo�� ��� 
			else{  
				nPinNo = nBlkNo -NUM_8K;  //32k->16k  
			}

			// JigNo�� �ش��ϴ� ��ġ�� JigNo���� write. index�� content ���빰�� ����
			nPinMap_NET[nJigNo]	= nJigNo;   // net list pin no   

			// ���� ���� i ��ġ(.CON���� ���� ��ġ)�� PinBlockNo ���� write.
			nPinMap_PBLK[i]		= nPinNo;	//****>QD pin no //pin block  no  


			//------------------------------
			// Switch H Mode ó��
			
			int cardNo;			// ACE400 Mok1
			int cardPinNo;		// ACE400 Namo,
			int boxNo;			// ACE400 BoxMok2;
			int boxCardNo;		// ACE400 Mok1
			if (m_sPinSetup.nPinSwitchMode == MODE_H)		//2=H
			{  
				// ex) nPinNo=4088 -> cardNo(Mok1)=63, cardPinNo(Namo)=56
				cardNo    = nPinNo / 64;			// 1card=>pin64���� ���� �ش� pin�� cardNo (Total pin���� cardNo)
				cardPinNo = nPinNo % 64;//same 		// card���� pinNo

				// 64(pin) * 32(card) => Box(2048 pin)
				if(m_sPinSetup.nMaxBoxPin == BOX_2K)		//1=2K
				{  
					// ex) cardNo=63 -> boxNo(BoxMok2)=1, boxCardNo(Mok1)= 31
					boxNo  		= cardNo / 32;//same	// 1box=>32card�� ���� �ش� piceNo�� BoxNo
					boxCardNo   = cardNo % 32;		// box����  cardNo

					// boxCardNo�� �籸��--------------
					
					// box���� CardNo�� 16 �̸��̸� 
					if(boxCardNo<16) {			       			// boxCardNo  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15 
						boxCardNo = boxCardNo * 2;   			//    =>      0   2   4   6   8  10  12  14  16  18  20  22  24  26  28  30
					}
					// box���� CardNo�� 16�̻� 32�̸��̸� 
					else if( boxCardNo < 32) {			   		// boxCardNo 16 17  18  19  20  21  22  23  24  25  26  27  28  29  30  31 
						boxCardNo = (boxCardNo - 15) *2 -1;  	//    =>      1   3   5   7   9  11  13  15  17  19  21  23  25  27  29  31	
					}
					else
					{
						strTemp.Format("PinSetupConvert(): Error!  pinNo=%d CHECK, boxCardNo=%d Range(0 ~31) Over!  ",  
								nPinNo,boxCardNo ); 		 
						ERR.Set(RANGE_OVER, strTemp); 
						return FALSE; 
					}

					// nPinNo�� �籸��--------------
					nPinNo= ((boxNo*32) + boxCardNo)*64 + cardPinNo;					  

				}

				// Box(4096 Pin), Card (64Pin 64Card)
				// 64(pin) * 64(card) => 1box (4096 pin)
				else	// m_sPinSetup.m_nPinBoxMax== BOX_4K
				{

					// ex) nPinNo=16355 -> cardNo(Mok1)=255, cardPinNo(Namo)=35
					//      cardNo=255 -> boxNo(BoxMok2)=3, boxCardNo(Mok1)= 63
					boxNo     = cardNo / 64;//same
					boxCardNo = cardNo % 64;

					// boxCardNo�� �籸��--------------
					if(boxCardNo<32){					// bokCardNo  0   1   2   3   4    ...  16  17  18  19 ...   28  29  30  31 
						boxCardNo=boxCardNo*2;			//    =>      0   2   4   6   8    ...  32  34  36  38 ...   56  58  60  62
					}
					else if( boxCardNo<64){				// boxCardNo  32  33  34  35  36   ...  48  49  50  51 ...   60  61  62  63
						boxCardNo=(boxCardNo-31)*2-1;	//    =>      1   3   5   7   9    ...  33  35  37  39 ...   57  59  61  63
					}
					else{
						strTemp.Format("PinSetupConvert(): Error! pinNo=%d CHECK., boxCardNo=%d   ", nPinNo,boxCardNo ); 		 
						ERR.Set(RANGE_OVER, strTemp); 
						return FALSE; 
					}
					// nPinNo�� �籸��--------------
					nPinNo= ((boxNo*64) + boxCardNo)*64 + cardPinNo;
				}		   
			}

			//------------------------------
			// Card Reverse  Mode ó��
			
			int boxPinNo, newBoxPinNo;		// ACE400 Mok1
			if(m_sPinSetup.nCardA_Reverse == CARD_REVERSE) //2
			{
				// 128(pin) * 16(card) => Box(2048 pin)
				if (m_sPinSetup.nMaxBoxPin == BOX_2K)		//1=2K
				{  
					// ex) nPinNo=16355 -> boxNo=7, boxPinNo=2019
					boxNo	 = nPinNo / NUM_2K;		// BOX_2K�ϱ� 2048
					boxPinNo = nPinNo % NUM_2K;

					// ex) boxPinNo=2019 -> boxCardNo=15, cardPinNo=109
					boxCardNo = boxPinNo / 128;  // 0~15  card
					cardPinNo = boxPinNo % 128;	// 0~127 cardPin
					newBoxPinNo	 = cardPinNo+((15-boxCardNo)*128);

					// nPinNo �籸��
					nPinNo	     = newBoxPinNo + boxNo * NUM_2K;	// *2048
				}

				// 128(pin) * 32(card) => Box(4096 pin)
				else	// m_sPinSetup.nMaxBoxPin== BOX_4K
				{
					boxNo	 	= nPinNo / NUM_4K;	// BOX_4K�ϱ� 4096
					boxPinNo 	= nPinNo % NUM_4K;

					boxCardNo	= boxPinNo / 128; 	//0~31  card
					cardPinNo	= boxPinNo % 128;	//0~127 cardPin
					newBoxPinNo	= cardPinNo+((31-boxCardNo)*128);	

					// nPinNo �籸��
					nPinNo	 	= newBoxPinNo+ boxNo * NUM_4K; 		// *4096
				}
			}

			//-----------------------------------------
			// Card B Reverse Mode ó��   (������ ����)
			
			//CONNECTOR  REVERSE = 64 PIN REVERSE
			if (m_sPinSetup.nCardB_Reverse==CARD_REVERSE) //2
			{  
				cardNo    = nPinNo / 128;		// 1card�� pin128���� ���� �ش� pin�� cardNo (Total pin���� cardNo)
				cardPinNo = nPinNo % 128; 		// card���� pinNo

				if(cardPinNo<64){
					cardPinNo=cardPinNo+64;
				}
				else{
					cardPinNo=cardPinNo-64;//191=127+64
				}
				nPinNo= cardNo*128 + cardPinNo;
			}


			//--------------------------------------------------------------
			// Max TotalPin�� 16K�ε� nPinNo�� 16K �̻��̸� 8K�� ���� �����Ѵ�.
			if (m_sPinSetup.nMaxTotPin == TOT_16K)		//3
			{
				if (nPinNo >= NUM_16K)
					nPinNo = nPinNo - NUM_8K;
			}



			//------------------------------------------
			// Total Pin ������ ����Ͽ� nPinNo ������ 
			if(nPinNo>=nMaxPinHalf)
			{
				switch (m_sPinSetup.nMaxTotPin)
				{
					case TOT_4K:		//1		+ 14K
						nPinNo=nPinNo+( 2048 + 4096 + 8192 ); //****>SWITCH CARD pin no
						break;

					case TOT_8K:		//2		+ 12K
						nPinNo=nPinNo+( 4096 + 8192   );	//****>SWITCH CARD pin no  
						break;


					case TOT_16K:		//3		+ 8K
						nPinNo=nPinNo+( 8192 );			//****>SWITCH CARD pin no 
						break;

					case TOT_32K:		//4
					default:	
						break;
				}
			}

			nPinMap_SWT[nJigNo] = nPinNo; //****>SWITCH CARD pin no  

		}
	}

#if 1
	// nPinMap ���.  ��� �˻��
	FILE *fp; 
	char  fName[200];  

	::ZeroMemory(&fName, sizeof(fName));
	strcat( fName , "D:\\PinMap2.txt" );

	fp = fopen(fName,"wt");
	if(fp == NULL) 
		return FALSE;

	fprintf(fp, " \n     ==>   QD BLOCK  \n ");
	fprintf(fp, "   *User Pin* / *Switch card Pin* \n");

	for(i = 0; i < nMaxPin; i++)
	{
		fprintf(fp, "%10d, %10d\n", i+1, nPinMap_SWT[i]+1);
		//fprintf(fp, "%10d, %10d\n", i, nPinMap_SWT[i]);
	}

	fclose(fp);  
#endif


	return TRUE;
}
