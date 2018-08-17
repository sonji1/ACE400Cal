// MacroMotion.cpp: implementation of the CMacroMotion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ACE400Cal.h"
#include "MacroMotion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CMacroMotion	MM;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMacroMotion::CMacroMotion()
{

#ifdef ICP_DAS_CARD
	// ICP DAS SWTICH CARD
	m_pMicroM = (CMicroMotion*) &g_MicroM_ICP;
#else
	// CONTEC SWTICH CARD
	// dynamic binding: MicroMotion 포인터에 실제 Switch Card 관련 포인터를 할당한다.
	m_pMicroM = (CMicroMotion*) &g_MicroM_Contec;		
#endif

}

CMacroMotion::~CMacroMotion()
{

}

// 핀번호등 설정 
DWORD CMacroMotion::Set(UINT module_id, int value1, int value2, int value3)
{
	//DWORD ret=0;
	sSEQ sSeq;
	sSET_VB			sSetVb;
	sSET_VB_OUT		sSetVbOut;
	sSET_PULSE		sSetPulse;
	sSET_I			sSetI;
	sSET_ADC_A		sSetAdcA;
	sSET_VB_HL		sSetVbHL;
	sSET_VC_HL		sSetVcHL;
	sSET_HVO		sSetHVO;
	sSET_FW_GAIN	sSetFWGain;
	sSET_ADC_B		sSetAdcB;
	sSET_BPOINT		sSetBPoint;
	sSET_CPOINT		sSetCPoint;
	sSET_BIT		sSetBit;

	switch(module_id)
	{
		case LV1:
		case LV2:
		case HV1:
		case HV2:
		case SPARK:
			sSetVb.type = module_id;					// ex) LV1
			if(value1 >=0) sSetVb.da_value = value1;	// ex) 15000 ADC
			sSetVb.Init();		// TranceiveSeq()에서 보드위치를 찾기위해 module_id 전달: 2017.12.26
			sSeq = sSetVb.sSeq;
			break;

		case VB_OUT:
			sSetVbOut.type = module_id;					
			//if(value1 >=0) sSetVbOut.da_value = value1;	
			sSetVbOut.Init();
			sSeq = sSetVbOut.sSeq;
			break;

		case PULSE:										
			if(value1 >=0) sSetPulse.freq = value1;		// ex) MM.Set(PULSE, 0, -1);
			if(value2 >=0) sSetPulse.volt = value2;		// ex) MM.Set(PULSE, -1, 0); 
			if(value3 >=0) sSetPulse.cav = value3;		// ex) MM.Set(PULSE, -1, -1, 0); 
			sSetPulse.Init();
			sSeq = sSetPulse.sSeq;
			break;

		case I:
			if(value1 >=0) sSetI.da_value = value1;	// ex) OnButtonISet()=> MM.Set(I, I_value);
			if(value2 >=0) sSetI.range = value2;	// ex) OnRadioIRng0()=> MM.Set(I, -1, I_RANGE0);
			sSetI.Init();
			sSeq = sSetI.sSeq;
			break;

		case PFFT:
		case PSFT:
		case VBFT:
		case IFFT:
		case ISFT:
			sSetAdcA.type = module_id;
			sSetAdcA.Init();
			sSeq = sSetAdcA.sSeq;
			break;

		case LVB:
		case HVB:
			sSetVbHL.type = value1;		// ex) LV1
			sSetVbHL.Init();
			sSeq = sSetVbHL.sSeq;
			break;

		case LVC:
		case HVC:
			sSetVcHL.type = module_id;
			sSetVcHL.bState = (BOOL) value1;
			sSetVcHL.Init();
			sSeq = sSetVcHL.sSeq;
			break;

		case HVO:		
			//if(value1 >= 0) sSetHVO.offset = value1;	 
			if(value1 >= 0) sSetHVO.gain = value1;		// ex) OnRadioHvoGain1()=>MM.Set(HVO, 1);
			sSetHVO.Init();
			sSeq = sSetHVO.sSeq;
			break;

		case FW_GAIN:
			sSetFWGain.mode = value1;
			sSetFWGain.Init();
			sSeq = sSetFWGain.sSeq;
			break;

		case VCFFT:
//		case VCSFT:
		case FWFT:
		case HVOFT:
			sSetAdcB.type = module_id;
			sSetAdcB.Init();
			sSeq = sSetAdcB.sSeq;
			break;

		case BPOINT:		// ex) MM.Set(BPOINT, point_no, TR);
			if(value1 >=0) sSetBPoint.bpoint = value1;
			if(value2 >=0) sSetBPoint.bTRY = value2;
			sSetBPoint.bSel = TRUE;
			sSetBPoint.Init();
			sSeq = sSetBPoint.sSeq;
			break;

		case CPOINT:
			if(value1 >=0) sSetCPoint.cpoint = value1;
			if(value2 >=0) sSetCPoint.bTRY = value2;
			sSetCPoint.bSel = TRUE;
			sSetCPoint.Init();
			sSeq = sSetCPoint.sSeq;
			break;

		// 1BIT 만 On/Off 하는 케이스 : Common sSetBit 타입을 이용.
		case I_GAIN:
		case CC_CV:
			sSetBit.type = module_id;
			if(value1 >=0) sSetBit.bState = value1;		// ex) MM.Set(I_GAIN, _ON);
			sSetBit.Init();
			sSeq = sSetBit.sSeq;
			break;
	}

	// Data Tranceive
	return m_pMicroM->TranceiveSeq(&sSeq);

}

UINT CMacroMotion::Get(UINT typGet, int value1)		// value1= bSkipDelay FALSE 이면 delay
{
#ifndef __PC_TEST__
	if(ERR.Get() != NO_ERR) 
		return (UINT)(-1);
#endif


	sGET_VI	sGetVI;

	// Sequence
	sSEQ sSeq;
	UINT order=0;


	switch(typGet)
	{
	case LV1:
	case LV2:
	case HV1:
	case HV2:
	case V:
		sGetVI.typGet = V;				// 설정 전압 read
		break;

	case I:
	case VI:
		sGetVI.typGet = typGet;			// 측정 전압, 측정 전류 read

		// TRUE면 Delay를 skip
		if(value1 >=0) sGetVI.bSkipDelay = (BOOL)value1;	
		break;;
	}

	sGetVI.Init();
	sSeq = sGetVI.sSeq;

	// Data Tranceive
	UINT dwRet = m_pMicroM->TranceiveSeq(&sSeq);
	if (dwRet == FALSE)
		return (UINT)(-1);

	// ret_value에 Get Value 값을  assgin해야 하므로 이 시점에 반드시 0이어야 함.  
	// 2018.01.10  ret_value 를  TranceiveSeq() 리턴값으로 썼을 때, 
	// ret_value가 실제값에서 +1 돼서 리턴되는 문제점 관련하여 수정함.
	UINT ret_value=0;

	// Get value
	order = sSeq.rcv_start;
	switch(typGet)
	{
	case LV1:
	case LV2:
	case HV1:
	case HV2:
	case V:	// ADC_A_D0~D7: 1'st byte, ADC_A_D8~D15: 2'nd byte
	case I:	// ADC_A_D0~D7: 1'st byte, ADC_A_D8~D15: 2'nd byte
#ifdef ICP_DAS_CARD
		ret_value = sSeq.dwRcvVal;

#else
		ret_value += sSeq.byteBuf[order++];		// 1'st byte
		ret_value += sSeq.byteBuf[order++]<<8;	// 2'nd byte
#endif
		break;
	
	case VI:
#ifdef ICP_DAS_CARD
		ret_value = sSeq.dwRcvVal;

#else
		ret_value += sSeq.byteBuf[order++];		// 1'st byte
		ret_value += sSeq.byteBuf[order++]<<8;	// 2'nd byte
		ret_value += sSeq.byteBuf[order++]<<16;	// 3'rd byte
		ret_value += sSeq.byteBuf[order++]<<24;	// 4'th byte
#endif
		break;
	}

	return ret_value;
}

// 전원 인가
DWORD CMacroMotion::Apply(UINT module_id)
{
#ifndef __PC_TEST__
	if(ERR.Get() != NO_ERR) 
		return FALSE;
#endif

	sAPPLY sAply;
	sAply.type = module_id;
	sAply.Init();

	return m_pMicroM->TranceiveSeq(&sAply.sSeq);
}

// 전원 인가 중단
DWORD CMacroMotion::Break(UINT module_id)
{
	sBREAK sBreak;
	sBreak.type = module_id;
	sBreak.Init();		

	return m_pMicroM->TranceiveSeq(&sBreak.sSeq);

}

DWORD CMacroMotion::Rst(UINT module_id, UINT value1, UINT value2)
{
#ifndef __PC_TEST__
	if(ERR.Get() != NO_ERR) 
		return FALSE;
#endif

	sSEQ sSeq;
//	sRST_BBOX		sRstBBox;
//	sRST_CBOX		sRstCBox;
	sSET_BPOINT		sSetBPoint; //	sRST_BPOINT		sRstBPoint;
	sSET_CPOINT		sSetCPoint; //  sRST_CPOINT		sRstCPoint;

	switch(module_id)
	{
/*
	case BBOX:
		sRstBBox.bbox = value1;
		sRstBBox.Init();
		sSeq = sRstBBox.sSeq;
		break;

	case CBOX:
		sRstCBox.cbox = value1;
		sRstCBox.Init();
		sSeq = sRstCBox.sSeq;
		break;
*/
	case BPOINT:
/*
		sRstBPoint.bpoint = value1;
		sRstBPoint.bTRY = value2;
		sRstBPoint.Init();
		sSeq = sRstBPoint.sSeq;
*/
		// RST_BPOINT 대신 SET_BPOINT의 bSel을 FALSE로 호출 (동일 function 사용)
		sSetBPoint.bpoint = value1;
		sSetBPoint.bTRY = value2;
		sSetBPoint.bSel = FALSE;	
		sSetBPoint.Init();
		sSeq = sSetBPoint.sSeq;
		break;

	case CPOINT:
/*
		sRstCPoint.cpoint = value1;
		sRstCPoint.bTRY = value2;
		sRstCPoint.Init();
		sSeq = sRstCPoint.sSeq;
*/
		// RST_CPOINT 대신 SET_CPOINT의 bSel을 FALSE로 호출 (동일 function 사용)
		sSetCPoint.cpoint = value1;
		sSetCPoint.bTRY = value2;
		sSetCPoint.bSel = FALSE;
		sSetCPoint.Init();
		sSeq = sSetCPoint.sSeq;
		break;
	}

	return m_pMicroM->TranceiveSeq(&sSeq);

}

void CMacroMotion::SetFilter(UINT test_type)
{
	switch(test_type)
	{
	case LV_OPEN_TEST:
		Set(IFFT);
		Set(VCFFT, LVC);
		break;

	case HV_OPEN_TEST:
		Set(IFFT);
		Set(HVOFT);
		break;

	case SHORT_TEST:
		Set(ISFT);
		break;

	case LEAK_TEST:
		Set(ISFT);
		break;

	case FOUR_WIRE_TEST:
		Set(FWFT);
		break;

	case EMBEDED_TEST:
		break;
	}

	return;
}

/*
BOOL CMacroMotion::RstBox(int ptB, int ptC)
{
#ifndef __PC_TEST__
	if(ERR.Get() != NO_ERR) 
		return FALSE;
#endif

	int order=0;
	sSEQ sSeq;
	sRST_BOX sRstBox;
	sRstBox.ptB = ptB;
	sRstBox.ptC = ptC;
	sRstBox.Init();
	sSeq = sRstBox.sSeq;

	return m_pMicroM->TranceiveSeq(&sSeq);
}
*/

BOOL CMacroMotion::RstPointAll (BOOL bVbVc)
{
#ifndef __PC_TEST__
	if(ERR.Get() != NO_ERR) 
		return FALSE;
#endif

	int order=0;

	sSEQ sSeq;
	sRST_POINT_ALL sRstPointAll;
	sRstPointAll.bVbVc = bVbVc;		// PT_VB  or PT_Vc

	sRstPointAll.Init();
	sSeq = sRstPointAll.sSeq;

	return m_pMicroM->TranceiveSeq(&sSeq);
}
