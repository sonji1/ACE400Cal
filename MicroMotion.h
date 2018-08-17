// MicroMotion.h: interface for the CMicroMotion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICROMOTION_H__BABAC453_C627_42E1_993B_D6AF7B1B15BA__INCLUDED_)
#define AFX_MICROMOTION_H__BABAC453_C627_42E1_993B_D6AF7B1B15BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Define for Contec 
#define INPUT_START				1000
#define OUTPUT_START			2000
#define CONTEC_BOARD_NUM		2
#define CONTEC_BOARD_DRIVER_NO	2
#define TOTAL_INPUT_NUM			64
#define TOTAL_OUTPUT_NUM		128
#define MAX_ORDER_NUM			20
#define	UNKNOWN					-1

// Define for ICP DAS 
#define D96_OUTPUT_START		1000
#define D64H_BD0_OUTPUT_START	2000
#define D64H_BD0_INPUT_START	3000
#define D64H_BD1_OUTPUT_START	4000
#define D64H_BD1_INPUT_START 	5000

#define D96_TOTAL_OUTPUT_NUM		96
#define D96_TOTAL_OUTPUT_PORT_NUM	(D96_TOTAL_OUTPUT_NUM /8)		// 12
#define D64_TOTAL_OUTPUT_NUM		32
#define D64_TOTAL_INTPUT_NUM		32

enum SEQ_CMD		{ ENDCMD, BIT_TRS, BYTE_TRS, MBIT_TRS, MBYTE_TRS, BIT_RCV, BYTE_RCV, MBIT_RCV, MBYTE_RCV, DLY, _4BYTE_RCV, _2BYTE_RCV }; // TRS ; transmit  RCV : receive  DLY : delay  M : multi bit
enum TEST_TYPE		{ LV_OPEN_TEST, HV_OPEN_TEST, SHORT_TEST, LEAK_TEST, FOUR_WIRE_TEST, EMBEDED_TEST };

enum MODULE_TYPE	{ LV1, LV2, HV1, HV2, PULSE, VB_OUT, SPARK, V, I, VI, PFFT, PSFT, VBFT, IFFT, ISFT, CAV, LVB, HVB, LVC, HVC, HVO, FW_GAIN, VCFFT, VCSFT, FWFT, HVOFT, BBOX, CBOX, BPOINT, CPOINT, I_GAIN, CC_CV, MODULE_END };
extern char	saStrModule[MODULE_END][20];		// MODULE_TYPE의 str. 짝을 맞추어서 같이 수정해야 함.

enum BOX_ID			{ BOX_ALL, BOX_A, BOX_B, BOX_C, BOX_D, BOX_E, BOX_F, BOX_G, BOX_H };
enum BREAK_TYPE		{ ALL_OUT, VB_ONLY };
enum GATE_TYPE		{ TR, RY };
enum SET_POINT		{ PT_VB, PT_VC };
enum I_RANGE		{ I_RANGE0, I_RANGE1, I_RANGE2, I_RANGE3, I_RANGE4 /*, I_RANGE5*/, I_RANGE6 };
enum VB_APP_TYPE	{ VB_LV1, VB_LV2, VB_HV1, VB_HV2, VB_PULSE };
enum ADC_A_FT_TYPE  { ADCA_PFAST, ADCA_PSLOW, ADCA_VB, ADCA_IFAST, ADCA_ISLOW};
enum ADC_B_FT_TYPE  { ADCB_VCFAST, ADCB_VCSLOW, ADCB_HVOPEN, ADCB_4WIRE};
enum VCFFT_HL_TYPE	{ VCFFT_HVC, VCFFT_LVC };
//enum MM_TYPE		{ MM_SET, MM_GET, MM_BREAK };

#define		_ON		1
#define		_OFF	0


typedef struct sCMD
{
	// cmd를 보내야 할 bd_id. 0으로 초기화해두고 필요한 경우에만 다르게 설정한다. 
	UINT bd_id;	// 0: BD_D96 or Contec, 1: D64_0,  2: D64_1

	UINT 	cmd;
	WORD 	BitNo;// BIT_TRS, BIT_RCV
	WORD 	PortNo;// BYTE_TRS, BYTE_RCV, DELAY
	BYTE 	byteVal;// BIT_TRS, BYTE_TRS, BIT_RCV, BYTE_RCV 
	int	 	mbitBuf[TOTAL_OUTPUT_NUM];// MBIT_TRS, MBYTE_TRS, MBIT_RCV, MBYTE_RCV, DELAY

	DWORD	dwRcvVal;		// D64 보드 전용. (_4BYTE_RCV)
	sCMD()
	{
		bd_id = 0;
		cmd = 0;
		BitNo = 0;
		PortNo = 0;
		byteVal = 0;
		FillMemory(mbitBuf, sizeof(int)*(sizeof(mbitBuf)/sizeof(int)), -1);
		dwRcvVal = 0;
	}
} sCMD, *psCMD;

typedef struct sSEQ
{

	// cmd를 보내야 할 bd_id. 0으로 초기화해두고 필요한 경우에만 다르게 설정한다. 
	UINT 	bd_id[MAX_ORDER_NUM];	// 0: BD_D96 or Contec, 1: D64_0,  2: D64_1

	UINT 	cmd[MAX_ORDER_NUM];
	WORD 	BitNo[MAX_ORDER_NUM];// BIT_TRS, BIT_RCV
	WORD 	PortNo[MAX_ORDER_NUM];// BYTE_TRS, BYTE_RCV, DELAY
	BYTE 	byteBuf[MAX_ORDER_NUM];// BIT_TRS, BYTE_TRS, BIT_RCV, BYTE_RCV 
	int	 	mbitBuf[MAX_ORDER_NUM][TOTAL_OUTPUT_NUM];// MBIT_TRS, MBYTE_TRS, MBIT_RCV, MBYTE_RCV, DELAY

	DWORD	dwRcvVal;		// D64 보드 전용. (_2BYTE_RCV, _4BYTE_RCV)
	UINT 	rcv_start;

	sSEQ()
	{
		ZeroMemory(bd_id, sizeof(UINT)*(sizeof(bd_id)/sizeof(UINT)));
		ZeroMemory(cmd, sizeof(UINT)*(sizeof(cmd)/sizeof(UINT)));
		ZeroMemory(BitNo, sizeof(WORD)*(sizeof(BitNo)/sizeof(WORD)));
		ZeroMemory(PortNo, sizeof(WORD)*(sizeof(PortNo)/sizeof(WORD)));
		ZeroMemory(byteBuf, sizeof(BYTE)*(sizeof(byteBuf)/sizeof(BYTE)));
		FillMemory(mbitBuf, sizeof(int)*(sizeof(mbitBuf)/sizeof(int)), -1);
		rcv_start = 0;
		dwRcvVal = 0;
	}
	void Init() 
	{

		ZeroMemory(bd_id, sizeof(UINT)*(sizeof(bd_id)/sizeof(UINT)));		
		ZeroMemory(cmd, sizeof(UINT)*(sizeof(cmd)/sizeof(UINT)));
		ZeroMemory(BitNo, sizeof(WORD)*(sizeof(BitNo)/sizeof(WORD)));
		ZeroMemory(PortNo, sizeof(WORD)*(sizeof(PortNo)/sizeof(WORD)));
		ZeroMemory(byteBuf, sizeof(BYTE)*(sizeof(byteBuf)/sizeof(BYTE)));
		FillMemory(mbitBuf, sizeof(int)*(sizeof(mbitBuf)/sizeof(int)), -1);
		rcv_start = 0;
		dwRcvVal = 0;
	}

} sSEQ, *psSEQ;



class CMicroMotion  
{
public:
	CMicroMotion();
	virtual ~CMicroMotion();
	void    GetModuleString(UINT module_id, char* strModule);

	virtual	BOOL	InitMotion()  {	return TRUE; }
	virtual BOOL	CloseMotion() { return TRUE; }
	virtual BOOL	On(WORD wBitNo) { return TRUE;}
	virtual	BOOL	Off(WORD wBitNo) { return TRUE;}
	virtual	BOOL	IsOn(WORD wBitNo) { return TRUE;}
	virtual	BOOL	IsOff(WORD wBitNo) { return TRUE;}
	virtual BOOL	TranceiveSeq(psSEQ psSeq) { return TRUE;}

};

#endif // !defined(AFX_MICROMOTION_H__BABAC453_C627_42E1_993B_D6AF7B1B15BA__INCLUDED_)
