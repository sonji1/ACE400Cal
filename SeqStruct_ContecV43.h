
#include "MicroMotion.h"
#include "MicroMotion_Contec.h"


#define TOTAL_DA_BIT_NUM		16	// Output : DA0~15
#define TOTAL_AD_BIT_NUM		32  // Input  : ADC_A_D0~15 + ADC_B_D0~15

enum DIGITAL_INPUT
{
	ADC_A_D0			= INPUT_START+0,	// X-00  // ADC A DATA D0			// 16bit, 0 ~ 65535
	ADC_A_D1,								// X-01  // ADC A DATA D1			// 16bit, 0 ~ 65535
	ADC_A_D2,								// X-02  // ADC A DATA D2			// 16bit, 0 ~ 65535
	ADC_A_D3,								// X-03  // ADC A DATA D3			// 16bit, 0 ~ 65535
	ADC_A_D4,								// X-04  // ADC A DATA D4			// 16bit, 0 ~ 65535
	ADC_A_D5,								// X-05  // ADC A DATA D5			// 16bit, 0 ~ 65535
	ADC_A_D6,								// X-06  // ADC A DATA D6			// 16bit, 0 ~ 65535
	ADC_A_D7,								// X-07  // ADC A DATA D7			// 16bit, 0 ~ 65535
	ADC_A_D8,								// X-10  // ADC A DATA D8			// 16bit, 0 ~ 65535
	ADC_A_D9,								// X-11  // ADC A DATA D9			// 16bit, 0 ~ 65535
	ADC_A_D10,								// X-12  // ADC A DATA D10			// 16bit, 0 ~ 65535
	ADC_A_D11,								// X-13  // ADC A DATA D11			// 16bit, 0 ~ 65535
	ADC_A_D12,								// X-14  // ADC A DATA D12			// 16bit, 0 ~ 65535
	ADC_A_D13,								// X-15  // ADC A DATA D13			// 16bit, 0 ~ 65535
	ADC_A_D14,								// X-16  // ADC A DATA D14			// 16bit, 0 ~ 65535
	ADC_A_D15,								// X-17  // ADC A DATA D15			// 16bit, 0 ~ 65535
	ADC_B_D0,								// X-20  // ADC B DATA D0			// 16bit, 0 ~ 65535
	ADC_B_D1,								// X-21  // ADC B DATA D1			// 16bit, 0 ~ 65535
	ADC_B_D2,								// X-22  // ADC B DATA D2			// 16bit, 0 ~ 65535
	ADC_B_D3,								// X-23  // ADC B DATA D3			// 16bit, 0 ~ 65535
	ADC_B_D4,								// X-24  // ADC B DATA D4			// 16bit, 0 ~ 65535
	ADC_B_D5,								// X-25  // ADC B DATA D5			// 16bit, 0 ~ 65535
	ADC_B_D6,								// X-26  // ADC B DATA D6			// 16bit, 0 ~ 65535
	ADC_B_D7,								// X-27  // ADC B DATA D7			// 16bit, 0 ~ 65535
	ADC_B_D8,								// X-30  // ADC B DATA D8			// 16bit, 0 ~ 65535
	ADC_B_D9,								// X-31  // ADC B DATA D9			// 16bit, 0 ~ 65535
	ADC_B_D10,								// X-32  // ADC B DATA D10			// 16bit, 0 ~ 65535
	ADC_B_D11,								// X-33  // ADC B DATA D11			// 16bit, 0 ~ 65535
	ADC_B_D12,								// X-34  // ADC B DATA D12			// 16bit, 0 ~ 65535
	ADC_B_D13,								// X-35  // ADC B DATA D13			// 16bit, 0 ~ 65535
	ADC_B_D14,								// X-36  // ADC B DATA D14			// 16bit, 0 ~ 65535
	ADC_B_D15,								// X-37  // ADC B DATA D15			// 16bit, 0 ~ 65535
	DT12V				= INPUT_START+32,	// X-40  // DETECTION MAIN POWER - 12V	// 0 : ERROR, 1: 정상
	DT5V,									// X-41  // DETECTION MAIN POWER - 5V	// 0 : ERROR, 1: 정상
	DT24V,									// X-42  // DETECTION MAIN POWER - 24V	// 0 : ERROR, 1: 정상
	DTHV,									// X-43  // DETECTION MAIN POWER - HV	// 0 : ERROR, 1: 정상
											// X-44  // SPARE
											// X-45  // SPARE
											// X-46  // SPARE
	SPARK_ON			= INPUT_START+39,	// X-47  // SPARK SIGNAL					// 1 : SPARK 발생
											// X-50  // SPARE
											// X-51  // SPARE
											// X-52  // SPARE
											// X-53  // SPARE
											// X-54  // SPARE
											// X-55  // SPARE
											// X-56  // SPARE
											// X-57  // SPARE
	DT5VC_D				= INPUT_START+48,	// X-60  // DETECTION BOX D - VC 5V		// 0 : ERROR, 1: 정상
	DT5VB_D,								// X-61  // DETECTION BOX D - VB 5V		// 0 : ERROR, 1: 정상
	DT5VC_C,								// X-62  // DETECTION BOX C - VC 5V		// 0 : ERROR, 1: 정상
	DT5VB_C,								// X-63  // DETECTION BOX C - VB 5V		// 0 : ERROR, 1: 정상
	DT5VC_B,								// X-64  // DETECTION BOX B - VC 5V		// 0 : ERROR, 1: 정상
	DT5VB_B,								// X-65  // DETECTION BOX B - VB 5V		// 0 : ERROR, 1: 정상
	DT5VC_A,								// X-66  // DETECTION BOX A - VC 5V		// 0 : ERROR, 1: 정상
	DT5VB_A,								// X-67  // DETECTION BOX A - VB 5V		// 0 : ERROR, 1: 정상
	PB_A0,									// X-70  // PIN BLOCK 종류 체크 A0		// 01 : QD, 10 : DD, 11 : SD
	PB_A1,									// X-71  // PIN BLOCK 종류 체크 A1		// 01 : QD, 10 : DD, 11 : SD
	BOX_A0,									// X-72  // BOX POINT 종류 체크 A0		// 01 : 16K, 10 : 8K, 11 : 4K
	BOX_A1,									// X-73  // BOX POINT 종류 체크 A1		// 01 : 16K, 10 : 8K, 11 : 4K
											// X-74  // 
											// X-75  // 
											// X-76  // 
											// X-77  // 
};

enum DIGITAL_OUTPUT
{
	//----------- new
	DA0					= OUTPUT_START+0,	// I-00  // VB, I , SPARK DA 0			// 16bit, 0 ~ 65535
	DA1,									// I-01  // VB, I , SPARK DA 1			// 16bit, 0 ~ 65535
	DA2,									// I-02  // VB, I , SPARK DA 2			// 16bit, 0 ~ 65535
	DA3,									// I-03  // VB, I , SPARK DA 3			// 16bit, 0 ~ 65535
	DA4,									// I-04  // VB, I , SPARK DA 4			// 16bit, 0 ~ 65535
	DA5,									// I-05  // VB, I , SPARK DA 5			// 16bit, 0 ~ 65535
	DA6,									// I-06  // VB, I , SPARK DA 6			// 16bit, 0 ~ 65535
	DA7,									// I-07  // VB, I , SPARK DA 7			// 16bit, 0 ~ 65535
	DA8,									// I-08  // VB, I , SPARK DA 8			// 16bit, 0 ~ 65535
	DA9,									// I-09  // VB, I , SPARK DA 9			// 16bit, 0 ~ 65535
	DA10,									// I-10  // VB, I , SPARK DA 10			// 16bit, 0 ~ 65535
	DA11,									// I-11  // VB, I , SPARK DA 11			// 16bit, 0 ~ 65535
	DA12,									// I-12  // VB, I , SPARK DA 12			// 16bit, 0 ~ 65535
	DA13,									// I-13  // VB, I , SPARK DA 13			// 16bit, 0 ~ 65535
	DA14,									// I-14  // VB, I , SPARK DA 14			// 16bit, 0 ~ 65535
	DA15,									// I-15  // VB, I , SPARK DA 15			// 16bit, 0 ~ 65535

	I_SET_CLK = OUTPUT_START+16,	 		// I-16  // 전류 DA SET Clock			// 0->1 : POSITIVE TRIGGER 
	_I_SET_RST,	      						// I-17  // 전류 DA SET RESET			// 0 : RESET, 1 : RUN(동작)
	
	LV_SET_CLK_0,							// I-18  // LV 0 DA Set Clock			// 0->1 : POSITIVE TRIGGER (_LV_SET_RST_0을 1로 설정후)
	_LV_SET_RST_0,							// I-19  // LV 0 DA Set Reset			// 0 : RESET, 1 : RUN(동작)

	HV_SET_CLK_0,							// I-20  // HV 0 DA Set Clock			// 0->1 : POSITIVE TRIGGER (_HV_SET_RST_0을 1로 설정후)
	_HV_SET_RST_0,							// I-21  // HV 0 DA Set Reset			// 0 : RESET, 1 : RUN(동작)

	LV_SET_CLK_1,							// I-22  // LV 1 DA Set Clock			// 0->1 : POSITIVE TRIGGER (_LV_SET_RST_1을 1로 설정후)
	_LV_SET_RST_1,							// I-23  // LV 1 DA Set Reset			// 0 : RESET, 1 : RUN(동작)

	HV_SET_CLK_1,							// I-24  // HV 1 DA Set Clock			// 0->1 : POSITIVE TRIGGER (_HV_SET_RST_1을 1로 설정후)
	_HV_SET_RST_1,							// I-25  // HV 1 DA Set Reset			// 0 : RESET, 1 : RUN(동작)
	
//	SPARK_SET_CE,							// I-    // SPARK DA Chip Enable		// /SPARK_SET_CLR가 1 로 설정 후 0->1 Rising edge로 동작
//	_SPARK_SET_CLR,							// I-    // SPARK DA Clear				// 0 : Clear, 1 : 동작
	SPARK_SET_CLK,							// I-26  // SPARK DA Set Clock			// 0->1 : POSITIVE TRIGGER (_SPARK_SET_RST를 1로 설정후)
	_SPARK_SET_RST,							// I-27  // SPARK DA Set Reset			// 0 : RESET, 1 : RUN(동작)

	OPEN_GAIN_A0  = OUTPUT_START + 30,		// I-30  // 오픈 전압별  GAIN 회로 4가지 선택
	OPEN_GAIN_A1,							// I-31	 // A0A1   00: GAIN1, 10: GAIN2, 01: GAIN3, 11: GAIN4
	
	RNG_SET_0 = OUTPUT_START + 32,			// I-32  // RANGE SET 0					// 1: Range Select
    RNG_SET_1,	    						// I-33  // RANGE SET 1					// 1: Range Select
    RNG_SET_2,	    						// I-34  // RANGE SET 2					// 1: Range Select
    RNG_SET_3,	    						// I-35  // RANGE SET 3					// 1: Range Select
	RNG_SET_4,	    						// I-36  // RANGE SET 4					// 1: Range Select
	
	ADC_VB_SEL = OUTPUT_START + 37,         // I-37  //ADC VB CHANNEL SENSING 		// ADC A 채널에 VB Filter 선택
	F4_WIRE_SELF_S1, 						// I-38  // 4-WIRE S1(VB) 릴레이 셀프 테스트
	F4_WIRE_SELF_S2,				    	// I-39  // 4-WIRE S2(VC) 릴레이 셀프 테스트

	VB_HV_SEN_SEL = OUTPUT_START + 40,  	// I-40	 // VB 고압 읽기 채널 선택	    // 0: HV0, 1: HV1
	VB_LV_SEN_SEL,  						// I-41	 // VB 저압 읽기 채널 선택      // 0: LV0, 2: LV1	
	VB_SEN_SEL,	   							// I-42	 // VB 고압-저압 읽기 채널 선택 // 0: LV,  1: HV	
                           
	VC_HV_SEL,      						// I-43	 // VC 고압 채널 ON/OFF     	// 0: OFF, 1: ON
	VC_LV_SEL,      						// I-44	 // VC 저압 채널 ON/OFF     	// 0: OFF, 1: ON    
	VC_SEN_SEL,     						// I-45	 // VC 고압-저압 읽기 채널 선택 // 0: HV,  1: LV

	F4_WIRE_CMD,                            // I-46  // 4-WIRE 측정 선택   
                                            //          4-WIRE 릴레이 ON, 4-WIRE필터 자동선택, VC_FAST_FT, FC_SLOW_FT OFF 할 것, 전류필터는 선택 
    ADC_I_FAST_SEL = OUTPUT_START +47,      // I-47  // 전류 필터 FAST 모드 (ADC_VB_SEL 과 ADC_I_SLOW_SEL과 동시 사용 금지)					
    ADC_I_SLOW_SEL,                         // I-48  // 전류 필터 SLOW 모드 (ADC_VB_SEL 과 ADC_I_FAST_SEL과 동시 사용 금지)					
	VC_FT,     								// I-49	 // VC 필터 모드  				// VC Filter Fast 모드 (F4_WIRE_CMD, HV_OPEN_CMD 동시 사용 금지)
	_SPARK_RST = OUTPUT_START+ 51,		    // I-51  // SPARK 동작 설정				// 0 : 미동작, 1 : 동작 및 SPARK DETECTION 신호 초기화

	HV_RELAY_IN_IO_0 = OUTPUT_START+ 52,	// I-52	 // HV 0 CHANNEL ON				// VB - HV 0 RELAY ON
	HV_RELAY_IN_IO_1,						// I-53	 // HV 1 CHANNEL ON				// VB - HV 1 RELAY ON
	LV_RELAY_IN_IO_0,						// I-54	 // LV 0 CHANNEL ON				// VB - LV 0 RELAY ON
	LV_RELAY_IN_IO_1,						// I-55	 // LV 1 CHANNEL ON				// VB - LV 1 RELAY ON
	HV_OPEN_CMD, 							// I-56	 // HV OPEN TEST 				// HV_OPEN Test 시 선택, VC_FT, 전류 필터는 선택할 것.
	I_GAIN_CMD,						        // I-57  // 전류 Gain 조정 모드			// 0: x1배 정상모드, 1: x10배(Short측정 시 사용)
	PIN_SEARCH_CMD,						    // I-58  // PIN SEARCH ON               
	HVGB_DISCHARGE,						    // I-59  // VB DISCHARGE 


	F4_WIRE_GAIN_A0 = OUTPUT_START + 61,	// I-61  // 4 WIRE GAIN A0	 A0A1  00: Slow Gain x1000 		01: Fast Gain x10
	_AD_CONVST 		= OUTPUT_START + 62,	// I-62  // ADC CONVERSION START  		// 1->O : Nagative Trigger (Falling Edge 동작)
	F4_WIRE_GAIN_A1 = OUTPUT_START + 63,	// I-63  // 4 WIRE GAIN A1	       10: Fast Gain x100		11: Fast Gain x1000

	VB_PIN_A0 = OUTPUT_START +64,			// I-64  // VB PIN SELCTION A0			// VB PIN 0~32767
	VB_PIN_A1,								// I-65  // VB PIN SELCTION A1			// VB PIN 0~32767
	VB_PIN_A2,								// I-66  // VB PIN SELCTION A2			// VB PIN 0~32767
	VB_PIN_A3,								// I-67  // VB PIN SELCTION A3			// VB PIN 0~32767
	VB_PIN_A4,								// I-68  // VB PIN SELCTION A4			// VB PIN 0~32767
	VB_PIN_A5,								// I-69  // VB PIN SELCTION A5			// VB PIN 0~32767
	VB_PIN_A6,								// I-70  // VB PIN SELCTION A6			// VB PIN 0~32767
	VB_PIN_A7,								// I-71  // VB PIN SELCTION A7			// VB PIN 0~32767
	VB_PIN_A8,								// I-72  // VB PIN SELCTION A8			// VB PIN 0~32767
	VB_PIN_A9,								// I-73  // VB PIN SELCTION A9			// VB PIN 0~32767
	VB_PIN_A10,								// I-74  // VB PIN SELCTION A10			// VB PIN 0~32767
	VB_PIN_A11,								// I-75  // VB PIN SELCTION A11			// VB PIN 0~32767
	VB_PIN_A12,								// I-76  // VB PIN SELCTION A12			// VB PIN 0~32767
	VB_PIN_A13,								// I-77  // VB PIN SELCTION A13			// VB PIN 0~32767
	VB_PIN_A14,								// I-78  // VB PIN SELCTION A14			// VB PIN 0~32767

	VC_PIN_A0 = OUTPUT_START +80,			// I-80  // VC PIN SELCTION A0			// VC PIN 0~32767
	VC_PIN_A1,								// I-81  // VC PIN SELCTION A1			// VC PIN 0~32767
	VC_PIN_A2,								// I-82  // VC PIN SELCTION A2			// VC PIN 0~32767
	VC_PIN_A3,								// I-83  // VC PIN SELCTION A3			// VC PIN 0~32767
	VC_PIN_A4,								// I-84  // VC PIN SELCTION A4			// VC PIN 0~32767
	VC_PIN_A5,								// I-85  // VC PIN SELCTION A5			// VC PIN 0~32767
	VC_PIN_A6,								// I-86  // VC PIN SELCTION A6			// VC PIN 0~32767
	VC_PIN_A7,								// I-87  // VC PIN SELCTION A7			// VC PIN 0~32767
	VC_PIN_A8,								// I-88  // VC PIN SELCTION A8			// VC PIN 0~32767
	VC_PIN_A9,								// I-89  // VC PIN SELCTION A9			// VC PIN 0~32767
	VC_PIN_A10,								// I-90  // VC PIN SELCTION A10			// VC PIN 0~32767
	VC_PIN_A11,								// I-91  // VC PIN SELCTION A11			// VC PIN 0~32767
	VC_PIN_A12,								// I-92  // VC PIN SELCTION A12			// VC PIN 0~32767
	VC_PIN_A13,								// I-93  // VC PIN SELCTION A13			// VC PIN 0~32767
	VC_PIN_A14,								// I-94  // VC PIN SELCTION A14			// VC PIN 0~32767

	VB_TR_SEL = OUTPUT_START +96,			// I-96  // VB TR 등록					// TR 등록시 1,     RY와는 배타적으로 설정된다.
	VB_RY_SEL,								// I-97  // VB Relay 등록				// Relay 등록시 1,  TR과는 배타적으로 설정된다.
	_VB_PIN_SEL,							// I-98  // VB PIN 등록/해제			// 0 : 등록, 1: : 해제
	_VB_ENA,								// I-99  // 핀등록 Trigger 신호			// 1 -> 0  Falling Edge 시 등록
	_VB_RESET,								// I-100 // VB RESET					// 0:  VB pin설정 초기화,  1: 등록

	VC_TR_SEL = OUTPUT_START +104,			// I-104 // VC TR 등록					// TR 등록시 1,     RY와는 배타적으로 설정된다.
	VC_RY_SEL,								// I-105 // VC Relay 등록				// Relay 등록시 1,  TR과는 배타적으로 설정된다.
	_VC_PIN_SEL,							// I-106 // VC PIN 등록/해제			// 0 : 등록, 1: : 해제
	_VC_ENA,								// I-107 // 핀등록 Trigger 신호			// 1 -> 0  Falling Edge 시 등록
	_VC_RESET,								// I-108 // VC RESET					// 0: VC pin 설정 초기화,  1: 등록


	CC_CV_MODE		= OUTPUT_START+120,	    // I-120 // CC/CV Mode Select			// 0: CC, 1: CV
	VB_AC_MODE,	    						// I-121 // Embedded Mode 활성화, 조합키로 사용. 
	VB_OUT_CHK 		= OUTPUT_START+122,		// I-122 // VB 전압 체크				// 현재 VB단의 전압을 측정

	VB_GUARD_ON		= OUTPUT_START+124,		// I-124 // VB 단의 TR을 이용한 Guarding
	VB_GUARD_SEL,							// I-125 // VB CC/CV Guarding 선택		// 0 : CV, 1 : CC
	VC_GUARD_ON,							// I-126 // VC 단의 TR을 이용한 Guarding
	VC_GUARD_SEL,							// I-127 // VC CC/CV Guarding 선택		// 0 : CV, 1 : CC


	//------------ old
/*	

	GPIO0,									// I-32  // General Purpose IO 0 
	GPIO1,									// I-33  // General Purpose IO 1
	GPIO2,									// I-34  // General Purpose IO 2
	GPIO3,									// I-35  // General Purpose IO 3
	GPIO4,									// I-36  // General Purpose IO 4
	GPIO5,									// I-37  // General Purpose IO 5
	FWIRE_SELF_VB,							// I-40  // VB Relay Self test
	FWIRE_SELF_VC,							// I-41  // VC Relay Self test
	VB_AC_MODE,								// I-42  // Pulse waveform signal
											// I-43  // 
											// I-50  // 
											// I-51  // 
											// I-52  // 
											// I-53  // 
	GPIO6				= OUTPUT_START+44,	// I-54  // General Purpose IO 6						//
	DC_RELAY_OUT,							// I-55  // LV 0,1의 출력단의 bypass Cap을 사용 안함	// Bypass Cap을 단락시켜 전압변동속도를 빠르게 하여 다이오드, TR측정에 사용
	BASE_I_LIMIT_CMD,						// I-57  // TR의 base단의 전류 제한을 설정(LV 1 ch 전용)// TR측정시 Base단의 전류제한을 Spark DA를 사용하여 전류를 제한시킨다


	ADC_A_CH_A1,							// I-66  // ADC A 채널 선택 A0							// 00 : Pulse 측정, 01 : VB 전압체크
	ADC_A_CH_A0,							// I-67  // ADC A 채널 선택 A1							// 10 : 전류 FAST,  11 : 전류 SLOW

	PIN_SEARCH,								// I-74  // Pin Search ON								//
											// I-76  // 
	HVGB_DISCHARGE		= OUTPUT_START+63,	// I-77  // VB 방전									// VB라인 방전
											// I-93  // SPARE
											//
	VC_CPLD_A0,								// I-A6  // VC CPLD 0								// TR(1~64)
	VC_CPLD_A1,								// I-A7  // VC CPLD 1								// TR(65~128)
	VC_CPLD_A2,								// I-B0  // VC CPLD 2								// RELAY(1~64)
	VC_CPLD_A3,								// I-B1  // VC CPLD 3								// RELAY(65~128)
	_VC_PIN_SEL,							// I-B2  // VC PIN SEL/UNSEL						// 0 : SEL, 1: : UNSEL
											// I-B3  // SPARE
	VC_CARD_A0			= OUTPUT_START+92,	// I-B4  // VC CARD A0								// VC CARD 선택 - 1번 블럭(ENA_A0), 2번 블럭(ENA_A1)
	VC_CARD_A1,								// I-B5  // VC CARD A1								// VC CARD 선택 - 1번 블럭(ENA_A0), 2번 블럭(ENA_A1)
	VC_CARD_A2,								// I-B6  // VC CARD A2								// VC CARD 선택 - 1번 블럭(ENA_A0), 2번 블럭(ENA_A1)
	VC_CARD_A3,								// I-B7  // VC CARD A3								// VC CARD 선택 - 1번 블럭(ENA_A0), 2번 블럭(ENA_A1)
	VB_ENA_A_A0,							// I-C0  // VB BOX 1번(상판) BLOCK ENABLE			// 1 -> 0 Falling Edge, SD일때 1번블럭, DD일때 2번 블럭, QD일때 1번 블럭
	VB_ENA_A_A1,							// I-C1  // VB BOX 2번(상판) BLOCK ENABLE			// 1 -> 0 Falling Edge, SD일때 2번블럭, DD일때 2번 블럭, QD일때 1번 블럭
	VB_ENA_B_A0,							// I-C2  // VB BOX 2번(상판) BLOCK ENABLE (only QD) // 1 -> 0 Falling Edge, SD,DD일때 사용 안함, QD일때 2번 블럭
	VB_ENA_B_A1,							// I-C3  // VB BOX 2번(상판) BLOCK ENABLE (only QD) // 1 -> 0 Falling Edge, SD,DD일때 사용 안함, QD일때 2번 블럭
	VB_ENA_C_A0,							// I-C4  // VB BOX 3번(상판) BLOCK ENABLE			// 1 -> 0 Falling Edge, SD일때 3번블럭, DD일때 3번 블럭, QD일때 3번 블럭
	VB_ENA_C_A1,							// I-C5  // VB BOX 4번(상판) BLOCK ENABLE			// 1 -> 0 Falling Edge, SD일때 4번블럭, DD일때 4번 블럭, QD일때 3번 블럭
	VB_ENA_D_A0,							// I-C6  // VB BOX 4번(상판) BLOCK ENABLE (only QD) // 1 -> 0 Falling Edge, SD,DD일때 사용 안함, QD일때 4번 블럭
	VB_ENA_D_A1,							// I-C7  // VB BOX 4번(상판) BLOCK ENABLE (only QD) // 1 -> 0 Falling Edge, SD,DD일때 사용 안함, QD일때 4번 블럭
	VC_ENA_A_A0,							// I-D0  // VC BOX 1번(상판) BLOCK ENABLE			// 1 -> 0 Falling Edge, SD일때 1번블럭, DD일때 2번 블럭, QD일때 1번 블럭
	VC_ENA_A_A1,							// I-D1  // VC BOX 2번(상판) BLOCK ENABLE			// 1 -> 0 Falling Edge, SD일때 2번블럭, DD일때 2번 블럭, QD일때 1번 블럭
	VC_ENA_B_A0,							// I-D2  // VC BOX 2번(상판) BLOCK ENABLE (only QD) // 1 -> 0 Falling Edge, SD,DD일때 사용 안함, QD일때 2번 블럭
	VC_ENA_B_A1,							// I-D3  // VC BOX 2번(상판) BLOCK ENABLE (only QD) // 1 -> 0 Falling Edge, SD,DD일때 사용 안함, QD일때 2번 블럭
	VC_ENA_C_A0,							// I-D4  // VC BOX 3번(상판) BLOCK ENABLE			// 1 -> 0 Falling Edge, SD일때 3번블럭, DD일때 3번 블럭, QD일때 3번 블럭
	VC_ENA_C_A1,							// I-D5  // VC BOX 4번(상판) BLOCK ENABLE			// 1 -> 0 Falling Edge, SD일때 4번블럭, DD일때 4번 블럭, QD일때 3번 블럭
	VC_ENA_D_A0,							// I-D6  // VC BOX 4번(상판) BLOCK ENABLE (only QD) // 1 -> 0 Falling Edge, SD,DD일때 사용 안함, QD일때 4번 블럭
	VC_ENA_D_A1,							// I-D7  // VC BOX 4번(상판) BLOCK ENABLE (only QD) // 1 -> 0 Falling Edge, SD,DD일때 사용 안함, QD일때 4번 블럭
	_BOX_A_VB_RST,							// I-E0  // BOX A VB RESET							// 0 : Reset, 1 : Run, SD일때 1,2번블럭, DD일때 1,2번 블럭, QD일때 1번 블록
	_BOX_B_VB_RST,							// I-E1  // BOX B VB RESET (only QD)				// 0 : Reset, 1 : Run, SD,DD일때 사용 안함, QD일때 2번 블록
	_BOX_C_VB_RST,							// I-E2  // BOX C VB RESET							// 0 : Reset, 1 : Run, SD일때 3,4번블럭, DD일때 3,4번 블럭, QD일때 3번 블록
	_BOX_D_VB_RST,							// I-E3  // BOX D VB RESET (only QD)				// 0 : Reset, 1 : Run, SD,DD일때 사용 안함, QD일때 4번 블록
	_BOX_A_VC_RST,							// I-E4  // BOX A VC RESET							// 0 : Reset, 1 : Run, SD일때 1,2번블럭, DD일때 1,2번 블럭, QD일때 1번 블록
	_BOX_B_VC_RST,							// I-E5  // BOX B VC RESET (only QD)				// 0 : Reset, 1 : Run, SD,DD일때 사용 안함, QD일때 2번 블록
	_BOX_C_VC_RST,							// I-E6  // BOX C VC RESET							// 0 : Reset, 1 : Run, SD일때 3,4번블럭, DD일때 3,4번 블럭, QD일때 3번 블록
	_BOX_D_VC_RST,							// I-E7  // BOX D VC RESET (only QD)				// 0 : Reset, 1 : Run, SD,DD일때 사용 안함, QD일때 4번 블록
*/

};

typedef struct sSET_VB
{
	int type, da_value;
	sSEQ sSeq;

	sSET_VB()
	{
		type=LV1;
		da_value = 0;
		sSeq.Init();
	}


	void Init()	// TranceiveSeq()에서 보드위치를 찾기위해 module_id 전달: 2017.12.26
	{
		int reset=0, clock=0, order=0;
		sSeq.Init();

		switch(type)
		{
		default:
		case LV1:
			reset = _LV_SET_RST_0-OUTPUT_START;		// ex) 19
			clock =  LV_SET_CLK_0-OUTPUT_START;		// ex) 18
			break;

		case LV2:
			reset = _LV_SET_RST_1-OUTPUT_START;
			clock =  LV_SET_CLK_1-OUTPUT_START;
			break;

		case HV1:
			reset = _HV_SET_RST_0-OUTPUT_START;
			clock =  HV_SET_CLK_0-OUTPUT_START;
			break;

		case HV2:
			reset = _HV_SET_RST_1-OUTPUT_START;
			clock =  HV_SET_CLK_1-OUTPUT_START;
			break;

		case SPARK:
			reset = _SPARK_SET_RST-OUTPUT_START;
			clock =  SPARK_SET_CLK-OUTPUT_START;
			break;
		}

/*
		//-------------------------------------
		// ALL Reset bit off, ALL Clock bit off
		// order 0 :
		sSeq.cmd[order] = MBIT_TRS;	
		sSeq.mbitBuf[order][_LV_SET_RST_0-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][19] = 0;
		sSeq.mbitBuf[order][LV_SET_CLK_0-OUTPUT_START] = 0; 	// sSeq.mbitBuf[0][18] = 0;
		sSeq.mbitBuf[order][_LV_SET_RST_1-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][23] = 0;
		sSeq.mbitBuf[order][LV_SET_CLK_1-OUTPUT_START] = 0; 	// sSeq.mbitBuf[0][22] = 0;
		sSeq.mbitBuf[order][_HV_SET_RST_0-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][21] = 0;
		sSeq.mbitBuf[order][HV_SET_CLK_0-OUTPUT_START] = 0; 	// sSeq.mbitBuf[0][20] = 0;
		sSeq.mbitBuf[order][_HV_SET_RST_1-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][25] = 0;
		sSeq.mbitBuf[order][HV_SET_CLK_1-OUTPUT_START] = 0; 	// sSeq.mbitBuf[0][24] = 0;
		sSeq.mbitBuf[order][_SPARK_SET_RST-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][27] = 0;
		sSeq.mbitBuf[order][SPARK_SET_CLK-OUTPUT_START] = 0; 	// sSeq.mbitBuf[0][26] = 0;
		order++;
*/
		//-------------------------------------
		// Reset bit off, Clock bit off
		// order 0 :
		sSeq.cmd[order] = MBIT_TRS;	
		sSeq.mbitBuf[order][reset] = 0;	// ex) LV1 sSeq.mbitBuf[0][19] = 0;
		sSeq.mbitBuf[order][clock] = 0; // ex) LV1 sSeq.mbitBuf[0][18] = 0;
		order++;

		// order 1 :
		sSeq.cmd[order] = DLY;			// sSeq.cmd[1] = cmd delay
		sSeq.PortNo[order] = msec;		// sSeq.PortNo[1] = msec(1)
		sSeq.mbitBuf[order][0] = 1;		// sSeq.mbitBuf[1][0] = 1 msec
		order++;
		
		//-------------------------------------
		// Reset bit  on (0 -> 1)
		// order 2 :
		sSeq.cmd[order] = BIT_TRS;			
		sSeq.BitNo[order] = reset;		// ex) LV1 sSeq.BitNo[2] = 19
		sSeq.byteBuf[order] = 1;		// ex)     sSeq.byteBuf[2] = 1;
		order++;

		//-------------------------------------
		// Value Data 전송
	/*	
		// order 2 :
		sSeq.cmd[order] = MBIT_TRS;	 
										// da_value == 1000 이라면 binary로  1111101000
										// 	sSeq.mbitBuf[2][0] = 0;
										// 	sSeq.mbitBuf[2][1] = 0;
										// 	sSeq.mbitBuf[2][2] = 0;
										// 	sSeq.mbitBuf[2][3] = 1;
										// 	sSeq.mbitBuf[2][4] = 0;
										// 	sSeq.mbitBuf[2][5] = 1;
										// 	sSeq.mbitBuf[2][6] = 1;
										// 	sSeq.mbitBuf[2][7] = 1;
										// 	sSeq.mbitBuf[2][8] = 1;
										// 	sSeq.mbitBuf[2][9] = 1;
										// 	sSeq.mbitBuf[2][10] = 0;
										// 	sSeq.mbitBuf[2][11] = 0;
										// 	sSeq.mbitBuf[2][12] = 0;
										// 	sSeq.mbitBuf[2][13] = 0;
										// 	sSeq.mbitBuf[2][14] = 0;
										// 	sSeq.mbitBuf[2][15] = 0;
		for(int i=0; i<TOTAL_DA_BIT_NUM; i++) 
			sSeq.mbitBuf[order][DA0-OUTPUT_START+i] = (da_value>>i) & 1;
		order++;
	*/

		// order 3: 
		sSeq.cmd[order] = BYTE_TRS;	 
		sSeq.PortNo[order] = (WORD)((DA0-OUTPUT_START) /8);		// sSeq.PortNo[3] = 0 
		sSeq.byteBuf[order] = (BYTE)(da_value & 0xff);			// sSeq.byteBuf[3] = da_value Low Byte 
		order++;

		// order 4: 
		sSeq.cmd[order] = BYTE_TRS;	 
		sSeq.PortNo[order] = (WORD)((DA8-OUTPUT_START) /8);		// sSeq.PortNo[4] = 1 
		sSeq.byteBuf[order] = (BYTE)((da_value >> 8) & 0xff);	// sSeq.byteBuf[4] = da_value High Byte 
		order++;

		// order 5 :
		sSeq.cmd[order] = DLY;			// sSeq.cmd[5] = cmd delay
		sSeq.PortNo[order] = usec;		// sSeq.PortNo[5] = micro sec(2)
		sSeq.mbitBuf[order][0] = 400;	// sSeq.mbitBuf[5][0] = 400 usec
		order++;

		//-------------------------------------
		// Clock bit  on (0 -> 1) 

		// order 6 : Clock bit on 
		sSeq.cmd[order] = BIT_TRS;	
		sSeq.BitNo[order] = clock;		// sSeq.BitNo[6] = 18
		sSeq.byteBuf[order] = 1;		// sSeq.byteBuf[6] = 1;
		order++;

	}

} sSET_VB;

typedef struct sSET_VB_OUT
{
	int type;
	sSEQ sSeq;

	sSET_VB_OUT()
	{
		type=VB_OUT;
		sSeq.Init();
	}

	void Init()
	{
		int order=0;
		sSeq.Init();

		//-------------------------------------
		// VB_OUT_CHK bit 1 : // VB_OUT_CHK ON 
		// order 0 :
		sSeq.cmd[order] = BIT_TRS;			
		sSeq.BitNo[order] = VB_OUT_CHK-OUTPUT_START;	// ex) LV1 sSeq.BitNo[0] = 122
		sSeq.byteBuf[order] = 1;						// ex)     sSeq.byteBuf[0] = 1;
		order++;

		//-------------------------------------
		// VB_SEN_SEL bit  1 : 회로보호를 위해 HV로 Set (0: LV,  1: HV) 
		// order 1 :
		sSeq.cmd[order] = BIT_TRS;			
		sSeq.BitNo[order] = VB_SEN_SEL-OUTPUT_START;	// ex) LV1 sSeq.BitNo[0] = 42
		sSeq.byteBuf[order] = 1;						// ex)     sSeq.byteBuf[0] = 1;
		order++;
	}

} sSET_VB_OUT;

typedef struct sSET_PULSE
{
	int freq, volt, cav;
	sSEQ sSeq;

	sSET_PULSE()
	{
		freq=volt=cav=-1;
		sSeq.Init();
	}

	void Init()
	{
		int order=0;
		sSeq.Init();
		
/*
		// 0 :
		sSeq.cmd[order] = MBIT_TRS;
		switch(freq)
		{
		case 0:// 100 Hz
			sSeq.mbitBuf[order][GPIO1-OUTPUT_START] = 0;	// PULSE_FREQ_SET_100Hz
			sSeq.mbitBuf[order][GPIO0-OUTPUT_START] = 0;
			break;

		case 1:// 1 KHz
			sSeq.mbitBuf[order][GPIO1-OUTPUT_START] = 0;	// PULSE_FREQ_SET_1KHz
			sSeq.mbitBuf[order][GPIO0-OUTPUT_START] = 1;
			break;
		
		case 2:// 10 KHz
			sSeq.mbitBuf[order][GPIO1-OUTPUT_START] = 1;	// PULSE_FREQ_SET_10KHz
			sSeq.mbitBuf[order][GPIO0-OUTPUT_START] = 0;
			break;

		case 3:// 100 KHz
			sSeq.mbitBuf[order][GPIO1-OUTPUT_START] = 1;	// PULSE_FREQ_SET_100KHz
			sSeq.mbitBuf[order][GPIO0-OUTPUT_START] = 1;
			break;
		}

		switch(volt)
		{
		case 0:// 1 V
			sSeq.mbitBuf[order][GPIO3-OUTPUT_START] = 0;	// PULSE_VOLT_SET_1V
			sSeq.mbitBuf[order][GPIO2-OUTPUT_START] = 0;
			break;

		case 1:// 2 V
			sSeq.mbitBuf[order][GPIO3-OUTPUT_START] = 0;	// PULSE_VOLT_SET_2V
			sSeq.mbitBuf[order][GPIO2-OUTPUT_START] = 1;
			break;
		
		case 2:// 5 V
			sSeq.mbitBuf[order][GPIO3-OUTPUT_START] = 1;	// PULSE_VOLT_SET_5V
			sSeq.mbitBuf[order][GPIO2-OUTPUT_START] = 0;
			break;

		case 3:// 10 V
			sSeq.mbitBuf[order][GPIO3-OUTPUT_START] = 1;	// PULSE_VOLT_SET_10V
			sSeq.mbitBuf[order][GPIO2-OUTPUT_START] = 1;
			break;
		}
		
		// Pulse Check를 위한 필터 설정
		switch(cav)
		{
		case 0:// 필터 0
			sSeq.mbitBuf[order][ADC_A_CH_A1-OUTPUT_START] = 0;	// 00 : Pulse 측정 (PULSE_ADC_CMD)
			sSeq.mbitBuf[order][ADC_A_CH_A0-OUTPUT_START] = 0;
			sSeq.mbitBuf[order][GPIO5-OUTPUT_START] = 0; 		// C_AV_SET_0
			sSeq.mbitBuf[order][GPIO4-OUTPUT_START] = 0;
			break;

		case 1:// 필터 1
			sSeq.mbitBuf[order][ADC_A_CH_A1-OUTPUT_START] = 0;	// 00 : Pulse 측정 (PULSE_ADC_CMD)
			sSeq.mbitBuf[order][ADC_A_CH_A0-OUTPUT_START] = 0;
			sSeq.mbitBuf[order][GPIO5-OUTPUT_START] = 0; 		// C_AV_SET_1
			sSeq.mbitBuf[order][GPIO4-OUTPUT_START] = 1;
			break;
		
		case 2:// 필터 2
			sSeq.mbitBuf[order][ADC_A_CH_A1-OUTPUT_START] = 0;  // 00 : Pulse 측정 (PULSE_ADC_CMD)	
			sSeq.mbitBuf[order][ADC_A_CH_A0-OUTPUT_START] = 0;
			sSeq.mbitBuf[order][GPIO5-OUTPUT_START] = 1; 		// C_AV_SET_2
			sSeq.mbitBuf[order][GPIO4-OUTPUT_START] = 0;
			break;

		case 3:// 필터 3
			sSeq.mbitBuf[order][ADC_A_CH_A1-OUTPUT_START] = 0;  // 00 : Pulse 측정 (PULSE_ADC_CMD)	
			sSeq.mbitBuf[order][ADC_A_CH_A0-OUTPUT_START] = 0;
			sSeq.mbitBuf[order][GPIO5-OUTPUT_START] = 1; 		// C_AV_SET_3
			sSeq.mbitBuf[order][GPIO4-OUTPUT_START] = 1;
			break;
		}
		order++;
*/
	}

} sSET_PULSE;

typedef struct sSET_I
{
	int da_value, range;
	sSEQ sSeq;

	sSET_I()
	{
		range = da_value = -1;
		sSeq.Init();
	}

	void Init()
	{
		int order=0;
		sSeq.Init();
		
		
		if(range >= I_RANGE0 && range <= I_RANGE4)	// ex) OnRadioIRng0()=> MM.Set(I, -1, I_RANGE0);
		{	
			// order 0 :  
			sSeq.cmd[order] = MBIT_TRS;	

			// RNG_SET_0 ~ RNG_SET_4 까지 모두 0으로 초기화
			for(int i = I_RANGE0; i <= I_RANGE4; i++) 
				sSeq.mbitBuf[order][RNG_SET_0-OUTPUT_START + i] = 0;

			// 해당 range만 1로 Set
			sSeq.mbitBuf[order][RNG_SET_0-OUTPUT_START + range] = 1;
			order++;
		}

		if(da_value >= 0)	// ex) OnButtonISet()=> MM.Set(I, I_value);
		{
			// order 0 : 
			sSeq.cmd[order] = MBIT_TRS;
			sSeq.mbitBuf[order][_I_SET_RST-OUTPUT_START] = 1;	// 전류 DA Set Reset On		// mbitBuf[0][17] = 1
			sSeq.mbitBuf[order][I_SET_CLK-OUTPUT_START] = 0;	// 전류 DA Set Clock Off	// mbitBuf[0][16] = 0
			order++;
			
	/*	
			// order 1 : 
			sSeq.cmd[order] = MBIT_TRS;
			for(int i=0; i<TOTAL_DA_BIT_NUM; i++) 		// Value Data 전송
				sSeq.mbitBuf[order][DA0-OUTPUT_START+i] = (da_value>>i) & 1;
			order++;
	*/

			// order 1: 
			sSeq.cmd[order] = BYTE_TRS;	 				// Value Data 전송
			sSeq.PortNo[order] = (WORD)((DA0-OUTPUT_START) /8);		// sSeq.PortNo[1] = 0 	 
			sSeq.byteBuf[order] = (BYTE)(da_value & 0xff);			// sSeq.byteBuf[1] = da_value Low Byte 
			order++;

			// order 2: 
			sSeq.cmd[order] = BYTE_TRS;	 
			sSeq.PortNo[order] = (WORD)((DA8-OUTPUT_START) /8);		// sSeq.PortNo[2] = 1 
			sSeq.byteBuf[order] = (BYTE)((da_value >> 8) & 0xff);	// sSeq.byteBuf[2] = da_value High Byte 
			order++;

			// order 3 :
			sSeq.cmd[order] = DLY;			// 200 micro sec delay
			sSeq.PortNo[order] = usec;
			sSeq.mbitBuf[order][0] = 200;
			order++;
		
			// order 4 :
			sSeq.cmd[order] = BIT_TRS;					// bit16  0 -> 1
			sSeq.BitNo[order] = I_SET_CLK-OUTPUT_START;	// 전류 DA Set Clock : 0 -> 1 trigger 
			sSeq.byteBuf[order] = 1;
			order++;
		}
	}

} sSET_I;


// ex)  
// CManualTestDlg::OnButtonLv1Set()
// {
//   ...
//   MM.Set(LVB, module_id);		    // sSET_VB_HL: ex) MM.Set(LVB, LV1);
//   => CMacroMotion::Set(UINT module_id, int value1, int value2, int value3)
//   {
//       ...
//     	switch(module_id)
//		{
//		case LVB:
//		case HVB:
//			sSetVbHL.type = value1;	// LV1
//			sSetVbHL.Init();
//			sSeq = sSetVbHL.sSeq;
//			break;
typedef struct sSET_VB_HL
{
	int type;
	sSEQ sSeq;

	sSET_VB_HL()
	{
		type = 0;
		sSeq.Init();
	}

	void Init()
	{
		int order=0;
		sSeq.Init();

		// 0 :
		sSeq.cmd[order] = MBIT_TRS;
		sSeq.mbitBuf[order][VB_OUT_CHK-OUTPUT_START] = 0;  	     // sSeq.mbitBuf[0][122]  = 0 (VB CH)

		//sSeq.mbitBuf[order][ADC_A_CH_A1-OUTPUT_START] = 0; // 01 : VB 전압 체크 (VB_CH_CHK_CMD)
		//sSeq.mbitBuf[order][ADC_A_CH_A0-OUTPUT_START] = 1; 
		
		switch(type)
		{
		case LV1:
			sSeq.mbitBuf[order][VB_SEN_SEL-OUTPUT_START] = 0;    // sSeq.mbitBuf[0][42]: 0 (LV)
			sSeq.mbitBuf[order][VB_LV_SEN_SEL-OUTPUT_START] = 0; // sSeq.mbitBuf[0][41]: 0 (LV0: SW LV1)
			break;

		case LV2:
			sSeq.mbitBuf[order][VB_SEN_SEL-OUTPUT_START] = 0;    // sSeq.mbitBuf[0][42]: 0 (LV)
			sSeq.mbitBuf[order][VB_LV_SEN_SEL-OUTPUT_START] = 1; // sSeq.mbitBuf[0][41]: 1 (LV1: SW LV2)
			break;

		case HV1:
			sSeq.mbitBuf[order][VB_SEN_SEL-OUTPUT_START] = 1;    // sSeq.mbitBuf[0][42]: 1 (HV) 
			sSeq.mbitBuf[order][VB_HV_SEN_SEL-OUTPUT_START] = 0; // sSeq.mbitBuf[0][40]: 0 (HV0: SW HV1)
			break;

		case HV2:
			sSeq.mbitBuf[order][VB_SEN_SEL-OUTPUT_START] = 1;    // sSeq.mbitBuf[0][42]: 1 (HV)
			sSeq.mbitBuf[order][VB_HV_SEN_SEL-OUTPUT_START] = 1; // sSeq.mbitBuf[0][40]: 1 (HV1: SW HV2)
			break;
		}
		order++;
	}

} sSET_VB_HL;

typedef struct sSET_ADC_A
{
	int type;
	sSEQ sSeq;

	sSET_ADC_A()
	{
		type = 0;
		sSeq.Init();
	}

	void Init()
	{
		int order=0;
		sSeq.Init();

		// 0 :
		sSeq.cmd[order] = MBIT_TRS;
		switch(type)
		{ 
/*
	ADC_A_CH_A1,							// Y-66  // ADC A 채널 선택 A0							// 00 : Pulse 측정, 01 : VB 전압체크
	ADC_A_CH_A0,							// Y-67  // ADC A 채널 선택 A1							// 10 : 전류 FAST,  11 : 전류 SLOW
*/

/*
//20170523  PULSE 관련 기능은 추후 구현
			// 내부조합표 참조
		case PFFT:	// PULSE_FILTER_SET(FAST)
			sSeq.mbitBuf[order][ADC_A_CH_A1-OUTPUT_START] = 0;		// 00 : Pulse 측정 (PULSE_ADC_CMD)
			sSeq.mbitBuf[order][ADC_A_CH_A0-OUTPUT_START] = 0;
			sSeq.mbitBuf[order][GPIO6-OUTPUT_START] = 0;
			break;

		case PSFT:	// PULSE_FILTER_SET(SLOW)
			sSeq.mbitBuf[order][ADC_A_CH_A1-OUTPUT_START] = 0;		// 00 : Pulse 측정  (PULSE_ADC_CMD)
			sSeq.mbitBuf[order][ADC_A_CH_A0-OUTPUT_START] = 0;
			sSeq.mbitBuf[order][GPIO6-OUTPUT_START] = 1;
			break;
*/

		case VBFT:	// ADC_VB_SEL 
			sSeq.mbitBuf[order][ADC_VB_SEL-OUTPUT_START] = 1;		// bit37 1 : ADC VB Filter Channel Sensing ON
			sSeq.mbitBuf[order][ADC_I_FAST_SEL-OUTPUT_START] = 0;	// bit47 0 : ADC_VB_SEL ON 시에 OFF
			sSeq.mbitBuf[order][ADC_I_SLOW_SEL-OUTPUT_START] = 0;	// bit48 0 : ADC_VB_SEL ON 시에 OFF
			break;


		case IFFT:	// ADC_I_FAST_SEL
			sSeq.mbitBuf[order][ADC_VB_SEL-OUTPUT_START] = 0;		// bit37 0 : ADC_I_FAST_SEL ON 시에 OFF 
			sSeq.mbitBuf[order][ADC_I_FAST_SEL-OUTPUT_START] = 1;	// bit47 1 : 전류 필터 fast mode ON
			sSeq.mbitBuf[order][ADC_I_SLOW_SEL-OUTPUT_START] = 0;	// bit48 0 : ADC_I_FAST_SEL ON 시에 OFF
			break;

		case ISFT:	// ADC_I_SLOW_SEL
			sSeq.mbitBuf[order][ADC_VB_SEL-OUTPUT_START] = 0;		// bit37 0 : ADC_I_SLOW_SEL ON 시에 OFF 
			sSeq.mbitBuf[order][ADC_I_FAST_SEL-OUTPUT_START] = 0;	// bit47 0 : ADC_I_SLOW_SEL ON 시에 OFF  
			sSeq.mbitBuf[order][ADC_I_SLOW_SEL-OUTPUT_START] = 1;	// bit48 1 : 전류 필터 slow mode ON 
			break;

		}
		order++;
	}

} sSET_ADC_A;

typedef struct sSET_VC_HL		// MM.Set(LVC, TRUE);
{
	int type;
	BOOL bState;
	sSEQ sSeq;

	sSET_VC_HL()
	{
		type = 0;
		bState = 0;
		sSeq.Init();
	}

	void Init()
	{
		int order=0;
		sSeq.Init();

		//------------------
		// order 0 :
		sSeq.cmd[order] = MBIT_TRS;	


		if (bState != TRUE)	
		{
			sSeq.mbitBuf[order][VC_HV_SEL-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][43] = 0
			sSeq.mbitBuf[order][VC_LV_SEL-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][44] = 0
			sSeq.mbitBuf[order][VC_SEN_SEL-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][45] = 0 
			//sSeq.mbitBuf[order][VC_FT-OUTPUT_START] = 0;		// sSeq.mbitBuf[0][49] = 0 

		}
		else
		{
/*
			// VC Filter Fast 모드 (F4_WIRE_CMD, HV_OPEN_CMD 동시 사용 금지)
			sSeq.mbitBuf[order][F4_WIRE_CMD-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][46] = 0 
			sSeq.mbitBuf[order][HV_OPEN_CMD-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][56] = 0
			sSeq.mbitBuf[order][VC_FT-OUTPUT_START] = 1;		// sSeq.mbitBuf[0][49] = 1 
*/
		
			// VC_HV_SEL 과 VC_LV_SEL 은 동시에 1이 될 수 없다.
			switch(type)
			{
			case HVC:
				sSeq.mbitBuf[order][VC_HV_SEL-OUTPUT_START] = 1;	// sSeq.mbitBuf[0][43] = 1
				sSeq.mbitBuf[order][VC_LV_SEL-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][44] = 0
				sSeq.mbitBuf[order][VC_SEN_SEL-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][45] = 0 (0: HV)
				break;

			case LVC:
				sSeq.mbitBuf[order][VC_HV_SEL-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][43] = 0
				sSeq.mbitBuf[order][VC_LV_SEL-OUTPUT_START] = 1;	// sSeq.mbitBuf[0][44] = 1
				sSeq.mbitBuf[order][VC_SEN_SEL-OUTPUT_START] = 1;	// sSeq.mbitBuf[0][45] = 1 (1: LV)
				break;
			}
		}
		order++;

	}

} sSET_VC_HL;

typedef struct sSET_HVO
{
	//int offset; 
	int gain;
	sSEQ sSeq;

	sSET_HVO()
	{
		//offset = gain = -1;
		gain = -1;
		sSeq.Init();
	}

	void Init()
	{
		int order=0;
		sSeq.Init();

		if(gain >= 1 && gain <= 4)
		{
			//------------------
			// order 0 :
			
			sSeq.cmd[order] = MBIT_TRS;

			// OPEN_GAIN_A0  = OUTPUT_START + 30,		// 오픈 전압별  GAIN 회로 4가지 선택
			// OPEN_GAIN_A1,							// A0A1   00: GAIN1, 10: GAIN2, 01: GAIN3, 11: GAIN4
			// 											=> A1A0   00: GAIN1, 01: GAIN2, 10: GAIN3, 11: GAIN4
			sSeq.mbitBuf[order][OPEN_GAIN_A1-OUTPUT_START] = ((gain-1)>>1) & 1;	// 2'nd bit
			sSeq.mbitBuf[order][OPEN_GAIN_A0-OUTPUT_START] = (gain-1) & 1;		// 1'st bit
			order++;
		}
	}

} sSET_HVO;

typedef struct sSET_FW_GAIN
{
	int mode;
	sSEQ sSeq;

	sSET_FW_GAIN()
	{
		mode = -1;
		sSeq.Init();
	}

	void Init()
	{
		int order=0;
		sSeq.Init();


		if(mode >= 1 && mode <= 4)
		{
			//------------------
			// order 0 :
			sSeq.cmd[order] = MBIT_TRS;

	
			/*
			sSeq.mbitBuf[order][F4_WIRE_CMD-OUTPUT_START] = 1;	// F4_WIRE_CMD ON
			sSeq.mbitBuf[order][HV_OPEN_CMD-OUTPUT_START] = 0;	// HV_OPEN_CMD OFF
			sSeq.mbitBuf[order][VC_FT-OUTPUT_START] = 0;		// VC FAST OFF
			*/
	
			// F4_WIRE_GAIN_A0 = OUTPUT_START + 61,	// A0A1  00: (1)Slow Gain x1000 	01: (2)Fast Gain x10
			// F4_WIRE_GAIN_A1 = OUTPUT_START + 63,	//       10: (3)Fast Gain x100		11: (4)Fast Gain x1000
			sSeq.mbitBuf[order][F4_WIRE_GAIN_A0-OUTPUT_START] = ((mode-1)>>1) & 1;	// 2'nd bit
			sSeq.mbitBuf[order][F4_WIRE_GAIN_A1-OUTPUT_START] = (mode-1) & 1;		// 1'st bit
			order++;
		}
	}

} sSET_FW_GAIN;

typedef struct sSET_ADC_B
{
	int type;
	sSEQ sSeq;

	sSET_ADC_B()
	{
		type=-1;
		sSeq.Init();
	}

	void Init()
	{
		int order=0;
		sSeq.Init();

		//---------------------------
		// order 0 :
		sSeq.cmd[order] = MBIT_TRS;

/*		
	ADC_B_CH_A1			= OUTPUT_START+52,	// Y-64  // ADC B 채널 선택 A1	// 00 : VC FAST , 01 : VC SLOW
	ADC_B_CH_A0,							// Y-65  // ADC B 채널 선택 A0  // 10 : HV OPEN Mode, 11: 4-Wire Mode
*/
		switch(type)
		{
	
		case VCFFT:		
			// VC Filter Fast 모드 (F4_WIRE_CMD, HV_OPEN_CMD 동시 사용 금지)
			sSeq.mbitBuf[order][F4_WIRE_CMD-OUTPUT_START] = 0;	// mbitBuf[0][46] 0
			sSeq.mbitBuf[order][VC_FT-OUTPUT_START] = 1;		// mbitBuf[0][49] 1 (VC Fast)
			sSeq.mbitBuf[order][HV_OPEN_CMD-OUTPUT_START] = 0;	// mbitBuf[0][56] 0
			break;
	
/*		
		case VCSFT:
			sSeq.mbitBuf[order][ADC_B_CH_A1-OUTPUT_START] = 0;		// 01 : VC SLOW
			sSeq.mbitBuf[order][ADC_B_CH_A0-OUTPUT_START] = 1;
			break;
*/

		case HVOFT:
			// HV OPEN 모드 (F4_WIRE_CMD, VC_FT  동시 사용 금지)
			sSeq.mbitBuf[order][F4_WIRE_CMD-OUTPUT_START] = 0;	/// mbitBuf[0][46] 0
			sSeq.mbitBuf[order][VC_FT-OUTPUT_START] = 0;		/// mbitBuf[0][49] 0
			sSeq.mbitBuf[order][HV_OPEN_CMD-OUTPUT_START] = 1;	/// mbitBuf[0][56] 1 (HV OPEN)
			break;

		case FWFT:
			// 4 Wire 모드 (HV_OPEN_CMD, VC_FT  동시 사용 금지)
			//sSeq.mbitBuf[order][VC_HV_SEL-OUTPUT_START] = 0;	// mbitBuf[0][43] 0
			//sSeq.mbitBuf[order][VC_LV_SEL-OUTPUT_START] = 0;	// mbitBuf[0][44] 0
			//sSeq.mbitBuf[order][VC_SEN_SEL-OUTPUT_START] = 1;	// mbitBuf[0][45] 1	(LV)	<=20170624 신부성차장 요구로 삭제
			sSeq.mbitBuf[order][F4_WIRE_CMD-OUTPUT_START] = 1;	// mbitBuf[0][46] 1 (4WIRE)
			sSeq.mbitBuf[order][VC_FT-OUTPUT_START] = 0;		// mbitBuf[0][49] 0
			sSeq.mbitBuf[order][HV_OPEN_CMD-OUTPUT_START] = 0;	// mbitBuf[0][56] 0
			break;
		}
		order++;
	}

} sSET_ADC_B;

typedef struct sGET_VI
{
	int typGet;
	BOOL bSkipDelay;
	sSEQ sSeq;

	sGET_VI()
	{
		typGet=VI;
		bSkipDelay = FALSE;
		sSeq.Init();
	}

	void Init()
	{
		int order=0;
		sSeq.Init();
		
		// 0 :
		sSeq.cmd[order] = BIT_TRS;
		sSeq.BitNo[order] = _AD_CONVST-OUTPUT_START;	// bit62 : 1
		sSeq.byteBuf[order] = 1;
		order++;

		// 1 :
		if(bSkipDelay == FALSE)
		{
			sSeq.cmd[order] = DLY;
			sSeq.PortNo[order] = msec;
			sSeq.mbitBuf[order][0] = 10;	// 10 msec delay
			order++;
		}

		// 2 :
		sSeq.cmd[order] = BIT_TRS;
		sSeq.BitNo[order] = _AD_CONVST-OUTPUT_START;
		sSeq.byteBuf[order] = 0;			// bit62 : 1 -> 0  Falling Edge
		order++;

		switch(typGet)
		{
		case V:			// 설정 전압(VB) 은 ADC_A의 VB Filter를 통해 획득

			//--------------------------
			// ADC A Read
		
			// 3 :
			sSeq.cmd[order] = BYTE_RCV;
			sSeq.PortNo[order] = (int)(ADC_A_D0-INPUT_START)/8;
			sSeq.rcv_start = order;
			order++;

			// 4 :
			sSeq.cmd[order] = BYTE_RCV;
			sSeq.PortNo[order] = (int)(ADC_A_D8-INPUT_START)/8;
			order++;
			break;

		case I:
			//--------------------------
			// ADC A Read
			
			// 3 :
			sSeq.cmd[order] = BYTE_RCV;
			sSeq.PortNo[order] = (int)(ADC_A_D0-INPUT_START)/8;
			sSeq.rcv_start = order;
			order++;

			// 4 :
			sSeq.cmd[order] = BYTE_RCV;
			sSeq.PortNo[order] = (int)(ADC_A_D8-INPUT_START)/8;
			order++;
			break;

		case VI:
			// 3 :			// 측정 전류를 ADC A에서 read
			sSeq.cmd[order] = BYTE_RCV;
			sSeq.PortNo[order] = (int)(ADC_A_D0-INPUT_START)/8;
			sSeq.rcv_start = order;
			order++;

			// 4 :
			sSeq.cmd[order] = BYTE_RCV;
			sSeq.PortNo[order] = (int)(ADC_A_D8-INPUT_START)/8;
			order++;

			// 5 :			// 측정 전압은(VC) ADC B 에서 read
			sSeq.cmd[order] = BYTE_RCV;
			sSeq.PortNo[order] = (int)(ADC_B_D0-INPUT_START)/8;
			order++;

			// 6 :
			sSeq.cmd[order] = BYTE_RCV;
			sSeq.PortNo[order] = (int)(ADC_B_D8-INPUT_START)/8;
			order++;
			break;
		}

		// _AD_CONVST  1로 원상 복구	(170619 신부성 차장 요청)
		sSeq.cmd[order] = BIT_TRS;
		sSeq.BitNo[order] = _AD_CONVST-OUTPUT_START;	// bit62 : 1
		sSeq.byteBuf[order] = 1;
		order++;
	}

} sGET_VI;

typedef struct sAPPLY
{
	int type;
	sSEQ sSeq;

	sAPPLY()
	{
		type = ALL_OUT;
		sSeq.Init();
	}

	void Init()
	{
		int order=0;
		sSeq.Init();

		// 0 :
		// ---HV, LV VB Relay Off
		sSeq.cmd[order] = MBIT_TRS;
		sSeq.mbitBuf[order][HV_RELAY_IN_IO_0-OUTPUT_START] = 0;
		sSeq.mbitBuf[order][HV_RELAY_IN_IO_1-OUTPUT_START] = 0;
		sSeq.mbitBuf[order][LV_RELAY_IN_IO_0-OUTPUT_START] = 0;
		sSeq.mbitBuf[order][LV_RELAY_IN_IO_1-OUTPUT_START] = 0;
		order++;

		// 1 :
		sSeq.cmd[order] = MBIT_TRS;
		switch(type)
		{
		case LV1:
			sSeq.mbitBuf[order][LV_RELAY_IN_IO_0-OUTPUT_START] = 1;
			break;

		case LV2:
			sSeq.mbitBuf[order][LV_RELAY_IN_IO_1-OUTPUT_START] = 1;
			break;

		case HV1:
			sSeq.mbitBuf[order][HV_RELAY_IN_IO_0-OUTPUT_START] = 1;
			break;

		case HV2:
			sSeq.mbitBuf[order][HV_RELAY_IN_IO_1-OUTPUT_START] = 1;
			break;

/*
		case PULSE://5
			sSeq.mbitBuf[order][VB_RY_OUT_A2-OUTPUT_START] = 1;
			sSeq.mbitBuf[order][VB_RY_OUT_A1-OUTPUT_START] = 0;
			sSeq.mbitBuf[order][VB_RY_OUT_A0-OUTPUT_START] = 1;
			break;
*/
		}
		order++;
	}

} sAPPLY;

typedef struct sBREAK
{
	int type;
	sSEQ sSeq;

	sBREAK()
	{
		type = ALL_OUT;
		sSeq.Init();
	}

	void Init()
	{
		int order=0;
		sSeq.Init();

		if(type == VB_ONLY || type == ALL_OUT)
		{
			// order 0 :
			sSeq.cmd[order] = MBIT_TRS;

			// ---HV, LV VB Relay Off
			sSeq.mbitBuf[order][HV_RELAY_IN_IO_0-OUTPUT_START] = 0;
			sSeq.mbitBuf[order][HV_RELAY_IN_IO_1-OUTPUT_START] = 0;
			sSeq.mbitBuf[order][LV_RELAY_IN_IO_0-OUTPUT_START] = 0;
			sSeq.mbitBuf[order][LV_RELAY_IN_IO_1-OUTPUT_START] = 0;
			order++;
		}

		if ( type == ALL_OUT)
		{

			/*
			// order 1 :
			// if (nMode4W==1 && nModeIR==1){//HW PROTECTION  //sylee20111013
			sSeq.cmd[order] = DLY;			// sSeq.cmd[1] = cmd delay
			sSeq.PortNo[order] = msec;		// sSeq.PortNo[1] = mili sec(1)
			sSeq.mbitBuf[order][0] = 2;		// sSeq.mbitBuf[1][0] = 2 msec
			order++;
			nMode4W = 0;
			*/


			// order 1 :
			// ---------------- All Digital Out Bit(0~127) Off
			sSeq.cmd[order] = MBIT_TRS;
			for(int i=OUTPUT_START; i<OUTPUT_START+TOTAL_OUTPUT_NUM; i++)
			{
				// 평소에 1이어야 하는 3개 bit만  1로 설정
				if(i == _VB_ENA || i == _VC_ENA || i == _AD_CONVST)
				{	sSeq.mbitBuf[order][i-OUTPUT_START] = 1;	}
				else
				{	sSeq.mbitBuf[order][i-OUTPUT_START] = 0;	}
			}
			order++;
		}
	}
} sBREAK;

typedef struct sSET_BPOINT
{
	int bpoint, bTRY;
	BOOL bSel;
	sSEQ sSeq;

	sSET_BPOINT()
	{
		bpoint = 0;
		bTRY = TR;
		bSel = TRUE;
		sSeq.Init();
	}

	void Init()
	{
		int i=0, order=0;
		BYTE Byte=0;
		sSeq.Init();

		//------------------------
		// order 0 :
		sSeq.cmd[order] = MBIT_TRS;	

		// _VB_ENA : 1 -> 0 Falling Edge를 위해 처음엔 1부터 Set
		sSeq.mbitBuf[order][_VB_ENA-OUTPUT_START] = 1;		// sSeq.mbitBuf[0][99]  = 1;
		// _VB_RESET 등록
		sSeq.mbitBuf[order][_VB_RESET-OUTPUT_START] = 1;	// sSeq.mbitBuf[0][100] = 1;
		order++;


		//------------------------
		// TR or RY 설정,  PIN_SEL 설정
		// order 1 :
		sSeq.cmd[order] = MBIT_TRS;	
		if(bTRY == TR)	// TR 등록시 1, RY와 배타적으로 설정된다.
		{
			sSeq.mbitBuf[order][VB_TR_SEL-OUTPUT_START] = 1; 	// sSeq.mbitBuf[1][96] = 1;
			sSeq.mbitBuf[order][VB_RY_SEL-OUTPUT_START] = 0; 	// sSeq.mbitBuf[1][97] = 0;
		}
		else
		{
			sSeq.mbitBuf[order][VB_TR_SEL-OUTPUT_START] = 0; 	// sSeq.mbitBuf[1][96] = 0;
			sSeq.mbitBuf[order][VB_RY_SEL-OUTPUT_START] = 1; 	// sSeq.mbitBuf[1][97] = 1;
		}

		// _VB_PIN_SEL  0: pin 등록,  1: pin 해제
		if (bSel)
			sSeq.mbitBuf[order][_VB_PIN_SEL-OUTPUT_START] = 0;		// sSeq.mbitBuf[1][98] = 0;
		else
			sSeq.mbitBuf[order][_VB_PIN_SEL-OUTPUT_START] = 1;		// sSeq.mbitBuf[1][98] = 1;
		order++;
		
		//------------------------
		// Pin Value 설정
		// order 2: 
		sSeq.cmd[order] = BYTE_TRS;	 				// port 8로 8bit Data 전송
		sSeq.PortNo[order] = (WORD)((VB_PIN_A0-OUTPUT_START) /8);	// sSeq.PortNo[2] = 8 
		sSeq.byteBuf[order] = (BYTE)(bpoint & 0xff);				// sSeq.byteBuf[2] = bpoint Low Byte 
		order++;

		// order 3: 
		sSeq.cmd[order] = BYTE_TRS;	 				// port 9로 7bit Data 전송
		sSeq.PortNo[order] = (WORD)((VB_PIN_A8-OUTPUT_START) /8);	// sSeq.PortNo[3] = 9 
		sSeq.byteBuf[order] = (BYTE)((bpoint >> 8) & 0x7f);			// sSeq.byteBuf[3] = bpoint High Byte 
		order++;

		//------------------------
		// _VB_ENA : 1 -> 0 Falling Edge 동작
		// order 4 :
		sSeq.cmd[order] = MBIT_TRS;	
		sSeq.mbitBuf[order][_VB_ENA-OUTPUT_START] = 0;		// sSeq.mbitBuf[4][99]  = 0;
		order++;

		//------------------------
		// _VB_ENA : 1 원상 복구 	(170619 신부성 차장 요청)
		//           _VB_ENA는 평소에 이상신호가 들어가지 않도록 1로 꺼 놓아야 한다.
		// _VB_RESET의 경우는 다음 pin 설정 직전에 0으로 설정해서 VC Pin 을 초기화해야 함.  
		// VB_VC_PointAllReset()을 Set_VbVcPin()보다 먼저 호출하여 해결.  측정을 해야 하므로 여기서 reset(0)하면 안된다.

		// order 5 :
		sSeq.cmd[order] = MBIT_TRS;	
		sSeq.mbitBuf[order][_VB_ENA-OUTPUT_START] = 1;		// sSeq.mbitBuf[4][99]  = 1;
		order++;
/*
		// 0 :
		// 모든 Box#에 해당되는 VB_ENA 비트를 1로 셋팅하기 위한  Byte 값을 만든다.  (for Falling Edge)
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VB_ENA_A_A0-OUTPUT_START)/8;

		Byte = 0;
		Byte += 1<<0;    // VB_ENA_A_A0 : 0x01
		Byte += 1<<1;    // VB_ENA_A_A1 : 0x02
		Byte += 1<<2;    // VB_ENA_B_A0 : 0x04
		Byte += 1<<3;    // VB_ENA_B_A1 : 0x08
		Byte += 1<<4;    // VB_ENA_C_A0 : 0x10
		Byte += 1<<5;    // VB_ENA_C_A1 : 0x20
		Byte += 1<<6;    // VB_ENA_D_A0 : 0x40
		Byte += 1<<7;    // VB_ENA_D_A1 : 0x80
		sSeq.byteBuf[order] = Byte;  //  Byte = 0xFF (1111 1111)
		order++;
		
		// 1 :
		// Box#에 해당하는 VB_RST 비트를 1로 셋팅한다.
		sSeq.cmd[order] = BIT_TRS;
		sSeq.byteBuf[order] = 1;

		// ** bpoint 비트 배치 참고 **
		// +---+---+----+----+------+
		// |BOX|ENA| PCB|CPLD|   PIN|
		// |   |BLE|    |    |      |
		// |432|  1|0987|   6|543210|
		// +---+---+----+----+------+
		// |111|  1|1111|   1|111111|
		// +---+---+----+----+------+

		//      bpoint>>12 & 0xf    				0		1		2		3			<=Box#
		// (1<<(bpoint>>12 & 0x0f))					0x01    0x02	0x04	0x08	
		if(((1<<(bpoint>>12 & 0x0f)) & 0x01)>>0) sSeq.BitNo[order] = _BOX_A_VB_RST-OUTPUT_START;//    0 -  4095
		if(((1<<(bpoint>>12 & 0x0f)) & 0x02)>>1) sSeq.BitNo[order] = _BOX_B_VB_RST-OUTPUT_START;//  4096 -  8191
		if(((1<<(bpoint>>12 & 0x0f)) & 0x04)>>2) sSeq.BitNo[order] = _BOX_C_VB_RST-OUTPUT_START;//  8192 - 12287
		if(((1<<(bpoint>>12 & 0x0f)) & 0x08)>>3) sSeq.BitNo[order] = _BOX_D_VB_RST-OUTPUT_START;// 12288 - 16383
		order++;

//		sSeq.cmd[order] = BYTE_TRS;
//		sSeq.PortNo[order] = (int)(BOX_A_VB_RST-OUTPUT_START)/8;
//
//		Byte = 0;
//		Byte += (1<<(bpoint>>12 & 0x0f)) & 0x01;// BOX_A_VB_RST      0 -  4095
//		Byte += (1<<(bpoint>>12 & 0x0f)) & 0x02;// BOX_B_VB_RST   4096 -  8191
//		Byte += (1<<(bpoint>>12 & 0x0f)) & 0x04;// BOX_C_VB_RST   8192 - 12287
//		Byte += (1<<(bpoint>>12 & 0x0f)) & 0x08;// BOX_D_VB_RST  12288 - 16383
//		sSeq.byteBuf[order] = Byte;
//		order++;

		// 2 :
		// Box#에 해당되는 VB_ENA 비트만 0으로 셋팅하는 Byte 값을 만든다.  (for Falling Edge)
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(BOX_VB_ENA_A-OUTPUT_START)/8;

		// Box = 0, (1<<(bpoint>>12 & 0x0f)) = 0x01, Byte = 0x0E
		// Box = 1, (1<<(bpoint>>12 & 0x0f)) = 0x02, Byte = 0x0D
		// Box = 2, (1<<(bpoint>>12 & 0x0f)) = 0x04, Byte = 0x0B
		// Box = 3, (1<<(bpoint>>12 & 0x0f)) = 0x08, Byte = 0x07
		Byte = 0;
		Byte += ((1<<(bpoint>>12 & 0x0f)) ^ 0x01) & 0x01;// BOX_VB_ENABLE_A      0 -  4095
		Byte += ((1<<(bpoint>>12 & 0x0f)) ^ 0x02) & 0x02;// BOX_VB_ENABLE_B   4096 -  8191
		Byte += ((1<<(bpoint>>12 & 0x0f)) ^ 0x04) & 0x04;// BOX_VB_ENABLE_C   8192 - 12287
		Byte += ((1<<(bpoint>>12 & 0x0f)) ^ 0x08) & 0x08;// BOX_VB_ENABLE_D  12288 - 16383
		sSeq.byteBuf[order] = Byte;
		order++;

		// 3 :
		// Pin비트 6개 CPLD_A0, CPLD_A1을 셋팅하는 Byte 값을 만든다.
		// CPLD=0이면 pin 수가 64개, CPLD=1이면 pin수는 128개가 된다.
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VB_PIN_A0-OUTPUT_START)/8;

		Byte = 0;
		Byte += bpoint & 0x01;// VB_PIN_A0  0 - 63
		Byte += bpoint & 0x02;// VB_PIN_A1  0 - 63 
		Byte += bpoint & 0x04;// VB_PIN_A2  0 - 63 
		Byte += bpoint & 0x08;// VB_PIN_A3  0 - 63 
		Byte += bpoint & 0x10;// VB_PIN_A4  0 - 63 
		Byte += bpoint & 0x20;// VB_PIN_A5  0 - 63 

		if(bTRY == TR)
		{
			//-------------
			// TR
			//
			// bpoint>>6 : CPLD bit
			
			// CPLD =0 이면 VB_CPLD_A0 =1, VB_CPLD_A1 = 0  (ex: bpoint=0x3f => Byte=0x7f)
			Byte += ((1<<(bpoint>>6 & 0x01)) & 0x01)<<6;// VB_CPLD_A0   0 -  63  : Byte +=0x40 (64)

			// CPLD =1 이면 VB_CPLD_A0 =0, VB_CPLD_A1 = 1  (ex: bpoint=0x7f => Byte=0xbf)
			Byte += ((1<<(bpoint>>6 & 0x01)) & 0x02)<<6;// VB_CPLD_A1  64 - 127  : Byte +=0x80 (128)
		}
		sSeq.byteBuf[order] = Byte;
		order++;

		// 4 :
		// CPLD_A2, CPLD_A3과 CARD(PCB) 비트 4개를 셋팅하는 Byte 값을 만든다.
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VB_CPLD_A2-OUTPUT_START)/8;

		Byte = 0;
		if(bTRY == RY)
		{
			//-------------
			// RELAY   
			//
			// bpoint>>6 : CPLD bit
			
			// CPLD =0 이면 VB_CPLD_A2 =1, VB_CPLD_A3 = 0  (ex: Byte=0x01)
			Byte += ((1<<(bpoint>>6 & 0x01)) & 0x01)<<0;// VB_CPLD_A2   0 -  63 : Byte +=0x01

			// CPLD =1 이면 VB_CPLD_A2 =0, VB_CPLD_A3 = 1  (ex: Byte=0x02)
			Byte += ((1<<(bpoint>>6 & 0x01)) & 0x02)<<0;// VB_CPLD_A3  64 - 127 : Byte +=0x02
		}
		Byte += 0<<2;// _VB_PIN_SEL=0 (pin 등록)	

		Byte += (bpoint & 0x080)>>3;// VB_CARD_A0  0 - 2047
		Byte += (bpoint & 0x100)>>3;// VB_CARD_A1  0 - 2047
		Byte += (bpoint & 0x200)>>3;// VB_CARD_A2  0 - 2047
		Byte += (bpoint & 0x400)>>3;// VB_CARD_A3  0 - 2047
		sSeq.byteBuf[order] = Byte;
		order++;
		
		// 5 :
		// Enable(Block) 을 위한  VB_ENA_A_A0, VB_ENA_A_A1 을 셋팅하는 Byte 값을 만든다. 
		// Enable=0이면 PCB(Card) 갯수는 16개, Enable=1이면 PCB(Card)갯수가 32개가 된다.
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VB_ENA_A_A0-OUTPUT_START)/8;

		Byte = 0;
		Byte += ((1<<(bpoint>>11 & 0x01)) ^ 0x01) & 0x01;// VB_ENA_A_A0
		Byte += ((1<<(bpoint>>11 & 0x01)) ^ 0x02) & 0x02;// VB_ENA_A_A1
		sSeq.byteBuf[order] = Byte;
		order++;
*/
	}

} sSET_BPOINT;

typedef struct sSET_CPOINT
{
	int cpoint, bTRY;
	BOOL bSel;
	sSEQ sSeq;

	sSET_CPOINT()
	{
		cpoint = 0;
		bTRY = TR;
		bSel = TRUE;
		sSeq.Init();
	}

	void Init()
	{
		int i=0, order=0;
		BYTE Byte=0;
		sSeq.Init();

		//------------------------
		// order 0 :
		sSeq.cmd[order] = MBIT_TRS;	

		// _VC_ENA : 1 -> 0 Falling Edge를 위해 처음엔 1부터 Set
		sSeq.mbitBuf[order][_VC_ENA-OUTPUT_START] = 1;		// sSeq.mbitBuf[0][107]  = 1;
		// _VC_RESET 등록
		sSeq.mbitBuf[order][_VC_RESET-OUTPUT_START] = 1;	// sSeq.mbitBuf[0][108] = 1;
		order++;


		//------------------------
		// TR or RY 설정,  PIN_SEL 설정
		// order 1 :
		sSeq.cmd[order] = MBIT_TRS;	

		if(bTRY == TR)	// TR 등록시 1, RY와 배타적으로 설정된다.
		{
			sSeq.mbitBuf[order][VC_TR_SEL-OUTPUT_START] = 1; 	// sSeq.mbitBuf[1][104] = 1;
			sSeq.mbitBuf[order][VC_RY_SEL-OUTPUT_START] = 0; 	// sSeq.mbitBuf[1][105] = 0;
		}
		else
		{
			sSeq.mbitBuf[order][VC_TR_SEL-OUTPUT_START] = 0; 	// sSeq.mbitBuf[1][104] = 0;
			sSeq.mbitBuf[order][VC_RY_SEL-OUTPUT_START] = 1; 	// sSeq.mbitBuf[1][105] = 1;
		}

		// _VC_PIN_SEL  0: pin 등록,  1: pin 해제
		if (bSel)
			sSeq.mbitBuf[order][_VC_PIN_SEL-OUTPUT_START] = 0;	// sSeq.mbitBuf[1][106] = 0;
		else
			sSeq.mbitBuf[order][_VC_PIN_SEL-OUTPUT_START] = 1;	// sSeq.mbitBuf[1][106] = 1;
		order++;
		
		//------------------------
		// Pin Value 설정
		// order 2: 
		sSeq.cmd[order] = BYTE_TRS;	 				// port10으로 Value Data 전송
		sSeq.PortNo[order] = (WORD)((VC_PIN_A0-OUTPUT_START) /8);	// sSeq.PortNo[2] = 10 
		sSeq.byteBuf[order] = (BYTE)(cpoint & 0xff);				// sSeq.byteBuf[2] = cpoint Low Byte 
		order++;

		// order 3: 
		sSeq.cmd[order] = BYTE_TRS;	 
		sSeq.PortNo[order] = (WORD)((VC_PIN_A8-OUTPUT_START) /8);	// sSeq.PortNo[3] = 11 
		sSeq.byteBuf[order] = (BYTE)((cpoint >> 8) & 0x7f);			// sSeq.byteBuf[3] = cpoint High Byte 
		order++;

		//------------------------
		// _VC_ENA : 1 -> 0 Falling Edge
		// order 4 :
		sSeq.cmd[order] = MBIT_TRS;	
		sSeq.mbitBuf[order][_VC_ENA-OUTPUT_START] = 0;		// sSeq.mbitBuf[4][107]  = 0;
		order++;

		//------------------------
		// _VC_ENA : 1  원상 복구		(170619 신부성 차장 요청)
		//           _VC_ENA는 평소에 이상신호가 들어가지 않도록 1로 꺼 놓아야 한다.
		// _VC_RESET의 경우는 다음 pin 설정 직전에 0으로 설정해서 VC Pin 을 초기화해야 함.  
		// VB_VC_PointAllReset()을 Set_VbVcPin()보다 먼저 호출하여 해결.  측정을 해야 하므로 여기서 reset(0)하면 안된다.
	
		// order 5 :
		sSeq.cmd[order] = MBIT_TRS;	
		sSeq.mbitBuf[order][_VC_ENA-OUTPUT_START] = 1;		// sSeq.mbitBuf[5][107]  = 1;
		order++;
/*
		// 0 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VC_ENA_A_A0-OUTPUT_START)/8;

		Byte = 0;
		Byte += 1<<0;// VC_ENA_A_A0
		Byte += 1<<1;// VC_ENA_A_A1
		Byte += 1<<2;// VC_ENA_B_A0
		Byte += 1<<3;// VC_ENA_B_A1
		Byte += 1<<4;// VC_ENA_C_A0
		Byte += 1<<5;// VC_ENA_C_A1
		Byte += 1<<6;// VC_ENA_D_A0
		Byte += 1<<7;// VC_ENA_D_A1
		sSeq.byteBuf[order] = Byte;
		order++;
		
		// 1 :
		sSeq.cmd[order] = BIT_TRS;
		sSeq.byteBuf[order] = 1;
		if(((1<<(cpoint>>12 & 0x0f)) & 0x01)>>0) sSeq.BitNo[order] = _BOX_A_VC_RST-OUTPUT_START;//     0 -  4095
		if(((1<<(cpoint>>12 & 0x0f)) & 0x02)>>1) sSeq.BitNo[order] = _BOX_B_VC_RST-OUTPUT_START;//  4096 -  8191
		if(((1<<(cpoint>>12 & 0x0f)) & 0x04)>>2) sSeq.BitNo[order] = _BOX_C_VC_RST-OUTPUT_START;//  8192 - 12287
		if(((1<<(cpoint>>12 & 0x0f)) & 0x08)>>3) sSeq.BitNo[order] = _BOX_D_VC_RST-OUTPUT_START;// 12288 - 16383
		order++;

//		sSeq.cmd[order] = BYTE_TRS;
//		sSeq.PortNo[order] = (int)(BOX_A_VC_RST-OUTPUT_START)/8;
//
//		Byte = 0;
//		Byte += (1<<(cpoint>>12 & 0x0f)) & 0x01;// BOX_A_VC_RST      0 -  4095
//		Byte += (1<<(cpoint>>12 & 0x0f)) & 0x02;// BOX_B_VC_RST   4096 -  8191
//		Byte += (1<<(cpoint>>12 & 0x0f)) & 0x04;// BOX_C_VC_RST   8192 - 12287
//		Byte += (1<<(cpoint>>12 & 0x0f)) & 0x08;// BOX_D_VC_RST  12288 - 16383
//		sSeq.byteBuf[order] = Byte;
//		order++;

		// 2 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(BOX_VC_ENA_A-OUTPUT_START)/8;

		Byte = 0;
		Byte += ((1<<(cpoint>>12 & 0x0f)) ^ 0x01) & 0x01;// BOX_VC_ENABLE_A      0 -  4095
		Byte += ((1<<(cpoint>>12 & 0x0f)) ^ 0x02) & 0x02;// BOX_VC_ENABLE_B   4096 -  8191
		Byte += ((1<<(cpoint>>12 & 0x0f)) ^ 0x04) & 0x04;// BOX_VC_ENABLE_C   8192 - 12287
		Byte += ((1<<(cpoint>>12 & 0x0f)) ^ 0x08) & 0x08;// BOX_VC_ENABLE_D  12288 - 16383
		sSeq.byteBuf[order] = Byte;
		order++;

		// 3 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VC_PIN_A0-OUTPUT_START)/8;

		Byte = 0;
		Byte += cpoint & 0x01;// VC_PIN_A0  0 - 63
		Byte += cpoint & 0x02;// VC_PIN_A1  0 - 63 
		Byte += cpoint & 0x04;// VC_PIN_A2  0 - 63 
		Byte += cpoint & 0x08;// VC_PIN_A3  0 - 63 
		Byte += cpoint & 0x10;// VC_PIN_A4  0 - 63 
		Byte += cpoint & 0x20;// VC_PIN_A5  0 - 63 

		if(bTRY == TR)
		{
			// TR
			Byte += ((1<<(cpoint>>6 & 0x01)) & 0x01)<<6;// VC_CPLD_A0   0 -  63
			Byte += ((1<<(cpoint>>6 & 0x01)) & 0x02)<<6;// VC_CPLD_A1  64 - 127
		}
		sSeq.byteBuf[order] = Byte;
		order++;

		// 4 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VC_CPLD_A2-OUTPUT_START)/8;

		Byte = 0;
		if(bTRY == RY)
		{
			// RELAY
			Byte += ((1<<(cpoint>>6 & 0x01)) & 0x01)<<0;// VC_CPLD_A2   0 -  63
			Byte += ((1<<(cpoint>>6 & 0x01)) & 0x02)<<0;// VC_CPLD_A3  64 - 127
		}
		Byte += 0<<2;// _VC_PIN_SEL
		Byte += (cpoint & 0x080)>>3;// VC_CARD_A0  0 - 2047
		Byte += (cpoint & 0x100)>>3;// VC_CARD_A1  0 - 2047
		Byte += (cpoint & 0x200)>>3;// VC_CARD_A2  0 - 2047
		Byte += (cpoint & 0x400)>>3;// VC_CARD_A3  0 - 2047
		sSeq.byteBuf[order] = Byte;
		order++;
		
		// 5 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VC_ENA_A_A0-OUTPUT_START)/8;

		Byte = 0;
		Byte += ((1<<(cpoint>>11 & 0x01)) ^ 0x01) & 0x01;// VC_ENA_A_A0
		Byte += ((1<<(cpoint>>11 & 0x01)) ^ 0x02) & 0x02;// VC_ENA_A_A1
		sSeq.byteBuf[order] = Byte;
		order++;
*/
	}

} sSET_CPOINT;


/*
typedef struct sRST_BBOX
{
	int bbox;
	sSEQ sSeq;

	sRST_BBOX()
	{
		bbox = -1;
		sSeq.Init();
	}
	
	void Init()
	{
		int order=0;
		BYTE Byte=0;
		sSeq.Init();
		switch(bbox)
		{
		case BOX_A:
		case BOX_B:
		case BOX_C:
		case BOX_D:
			// 0 :
			sSeq.cmd[order] = BIT_TRS;
			sSeq.byteBuf[order] = 0;

			if(bbox == BOX_A)	   sSeq.BitNo[order] = _BOX_A_VB_RST-OUTPUT_START;
			else if(bbox == BOX_B) sSeq.BitNo[order] = _BOX_B_VB_RST-OUTPUT_START;
			else if(bbox == BOX_C) sSeq.BitNo[order] = _BOX_C_VB_RST-OUTPUT_START;
			else if(bbox == BOX_D) sSeq.BitNo[order] = _BOX_D_VB_RST-OUTPUT_START;
			order++;
			break;

		case BOX_ALL:
			// 0 :
			sSeq.cmd[order] = BYTE_TRS;
			sSeq.PortNo[order] = (int)(_BOX_A_VB_RST-OUTPUT_START)/8;

			Byte = 0;
			Byte += 0<<0;
			Byte += 0<<1;
			Byte += 0<<2;
			Byte += 0<<3;
			Byte += 1<<4;
			Byte += 1<<5;
			Byte += 1<<6;
			Byte += 1<<7;
			sSeq.byteBuf[order] = Byte;
			order++;
			break;
		}
	}

} sRST_BBOX;
*/

/*
typedef struct sRST_CBOX
{
	int cbox;
	sSEQ sSeq;

	sRST_CBOX()
	{
		cbox = -1;
		sSeq.Init();
	}
	
	void Init()
	{
		int order=0;
		BYTE Byte=0;
		sSeq.Init();

		switch(cbox)
		{
		case BOX_A:
		case BOX_B:
		case BOX_C:
		case BOX_D:
			// 0 :
			sSeq.cmd[order] = BIT_TRS;
			sSeq.byteBuf[order] = 0;

			if(cbox == BOX_A)	   sSeq.BitNo[order] = _BOX_A_VC_RST-OUTPUT_START;
			else if(cbox == BOX_B) sSeq.BitNo[order] = _BOX_B_VC_RST-OUTPUT_START;
			else if(cbox == BOX_C) sSeq.BitNo[order] = _BOX_C_VC_RST-OUTPUT_START;
			else if(cbox == BOX_D) sSeq.BitNo[order] = _BOX_D_VC_RST-OUTPUT_START;
			order++;
			break;

		case BOX_ALL:
			// 0 :
			sSeq.cmd[order] = BYTE_TRS;
			sSeq.PortNo[order] = (int)(_BOX_A_VC_RST-OUTPUT_START)/8;

			Byte = 0;
			Byte += 1<<0;
			Byte += 1<<1;
			Byte += 1<<2;
			Byte += 1<<3;
			Byte += 0<<4;
			Byte += 0<<5;
			Byte += 0<<6;
			Byte += 0<<7;
			sSeq.byteBuf[order] = Byte;
			order++;
			break;
		}
	}

} sRST_CBOX;
*/

/*
typedef struct sRST_BOX
{
	int ptB, ptC;
	sSEQ sSeq;

	sRST_BOX()
	{
		ptB = ptC = -1;
		sSeq.Init();
	}
	
	void Init()
	{
		int order=0;
		BYTE Byte=0;
		sSeq.Init();

		// 0 :
		if(ptB >= 0)
		{
			sSeq.cmd[order] = BIT_TRS;
			sSeq.byteBuf[order] = 0;
			if(((1<<(ptB>>12 & 0x0f)) & 0x01)>>0) sSeq.BitNo[order] = _BOX_A_VB_RST-OUTPUT_START;
			if(((1<<(ptB>>12 & 0x0f)) & 0x02)>>1) sSeq.BitNo[order] = _BOX_B_VB_RST-OUTPUT_START;
			if(((1<<(ptB>>12 & 0x0f)) & 0x04)>>2) sSeq.BitNo[order] = _BOX_C_VB_RST-OUTPUT_START;
			if(((1<<(ptB>>12 & 0x0f)) & 0x08)>>3) sSeq.BitNo[order] = _BOX_D_VB_RST-OUTPUT_START;
			order++;
		}

		// 1 :
		if(ptC >= 0)
		{
			sSeq.cmd[order] = BIT_TRS;
			sSeq.byteBuf[order] = 0;
			if(((1<<(ptC>>12 & 0x0f)) & 0x01)>>0) sSeq.BitNo[order] = _BOX_A_VC_RST-OUTPUT_START;
			if(((1<<(ptC>>12 & 0x0f)) & 0x02)>>1) sSeq.BitNo[order] = _BOX_B_VC_RST-OUTPUT_START;
			if(((1<<(ptC>>12 & 0x0f)) & 0x04)>>2) sSeq.BitNo[order] = _BOX_C_VC_RST-OUTPUT_START;
			if(((1<<(ptC>>12 & 0x0f)) & 0x08)>>3) sSeq.BitNo[order] = _BOX_D_VC_RST-OUTPUT_START;
			order++;
		}
	}

} sRST_BOX;
*/

typedef struct sRST_POINT_ALL
{
	BOOL bVbVc;
	sSEQ sSeq;

	sRST_POINT_ALL()
	{
		bVbVc = PT_VB; 	// VB
		sSeq.Init();
	}
	
	void Init()
	{

		int order=0;
		BYTE Byte=0;
		sSeq.Init();

		//------------------------
		// order 0 :
		sSeq.cmd[order] = MBIT_TRS;	

		switch (bVbVc)
		{
			case PT_VB:
				sSeq.mbitBuf[order][VB_TR_SEL-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][96]
				sSeq.mbitBuf[order][VB_RY_SEL-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][97]
				sSeq.mbitBuf[order][_VB_PIN_SEL-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][98]

				// _VB_ENA : Falling Edge 이므로 1로 초기화 
				sSeq.mbitBuf[order][_VB_ENA-OUTPUT_START] = 1;		// sSeq.mbitBuf[0][99]

				// _VB_RESET 0: VB PIN 전체 초기화 
				sSeq.mbitBuf[order][_VB_RESET-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][100]
				break;

			case PT_VC:
				sSeq.mbitBuf[order][VC_TR_SEL-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][104]
				sSeq.mbitBuf[order][VC_RY_SEL-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][105]
				sSeq.mbitBuf[order][_VC_PIN_SEL-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][106]

				// _VC_ENA : Falling Edge 이므로 1로 초기화 
				sSeq.mbitBuf[order][_VC_ENA-OUTPUT_START] = 1;		// sSeq.mbitBuf[0][107]

				// _VC_RESET 0: VC PIN 전체 초기화 
				sSeq.mbitBuf[order][_VC_RESET-OUTPUT_START] = 0;	// sSeq.mbitBuf[0][108]
				break;
		}
		order++;
	}

} sRST_POINT_ALL;

/*
typedef struct sRST_BPOINT
{
	int bpoint, bTRY;
	sSEQ sSeq;

	sRST_BPOINT()
	{
		bpoint = 0;
		bTRY = TR;
		sSeq.Init();
	}

	void Init()
	{
		int i=0, order=0;
		BYTE Byte=0;
		sSeq.Init();

		// 0 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VB_ENA_A_A0-OUTPUT_START)/8;

		Byte = 0;
		Byte += 1<<0;// VB_ENA_A_A0
		Byte += 1<<1;// VB_ENA_A_A1
		Byte += 1<<2;// VB_ENA_B_A0
		Byte += 1<<3;// VB_ENA_B_A1
		Byte += 1<<4;// VB_ENA_C_A0
		Byte += 1<<5;// VB_ENA_C_A1
		Byte += 1<<6;// VB_ENA_D_A0
		Byte += 1<<7;// VB_ENA_D_A1
		sSeq.byteBuf[order] = Byte;
		order++;
		
		// 1 :
		sSeq.cmd[order] = BIT_TRS;
		sSeq.byteBuf[order] = 1;
		if(((1<<(bpoint>>12 & 0x0f)) & 0x01)>>0) sSeq.BitNo[order] = _BOX_A_VB_RST-OUTPUT_START;//     0 -  4095
		if(((1<<(bpoint>>12 & 0x0f)) & 0x02)>>1) sSeq.BitNo[order] = _BOX_B_VB_RST-OUTPUT_START;//  4096 -  8191
		if(((1<<(bpoint>>12 & 0x0f)) & 0x04)>>2) sSeq.BitNo[order] = _BOX_C_VB_RST-OUTPUT_START;//  8192 - 12287
		if(((1<<(bpoint>>12 & 0x0f)) & 0x08)>>3) sSeq.BitNo[order] = _BOX_D_VB_RST-OUTPUT_START;// 12288 - 16383
		order++;

//		sSeq.cmd[order] = BYTE_TRS;
//		sSeq.PortNo[order] = (int)(BOX_A_VB_RST-OUTPUT_START)/8;
//
//		Byte = 0;
//		Byte += (1<<(bpoint>>12 & 0x0f)) & 0x01;// BOX_A_VB_RST      0 -  4095
//		Byte += (1<<(bpoint>>12 & 0x0f)) & 0x02;// BOX_B_VB_RST   4096 -  8191
//		Byte += (1<<(bpoint>>12 & 0x0f)) & 0x04;// BOX_C_VB_RST   8192 - 12287
//		Byte += (1<<(bpoint>>12 & 0x0f)) & 0x08;// BOX_D_VB_RST  12288 - 16383
//		sSeq.byteBuf[order] = Byte;
//		order++;

		// 2 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(BOX_VB_ENA_A-OUTPUT_START)/8;

		Byte = 0;
		Byte += ((1<<(bpoint>>12 & 0x0f)) ^ 0x01) & 0x01;// BOX_VB_ENABLE_A      0 -  4095
		Byte += ((1<<(bpoint>>12 & 0x0f)) ^ 0x02) & 0x02;// BOX_VB_ENABLE_B   4096 -  8191
		Byte += ((1<<(bpoint>>12 & 0x0f)) ^ 0x04) & 0x04;// BOX_VB_ENABLE_C   8192 - 12287
		Byte += ((1<<(bpoint>>12 & 0x0f)) ^ 0x08) & 0x08;// BOX_VB_ENABLE_D  12288 - 16383
		sSeq.byteBuf[order] = Byte;
		order++;

		// 3 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VB_PIN_A0-OUTPUT_START)/8;

		Byte = 0;
		Byte += bpoint & 0x01;// VB_PIN_A0  0 - 63
		Byte += bpoint & 0x02;// VB_PIN_A1  0 - 63 
		Byte += bpoint & 0x04;// VB_PIN_A2  0 - 63 
		Byte += bpoint & 0x08;// VB_PIN_A3  0 - 63 
		Byte += bpoint & 0x10;// VB_PIN_A4  0 - 63 
		Byte += bpoint & 0x20;// VB_PIN_A5  0 - 63 

		if(bTRY == TR)
		{
			// TR
			Byte += ((1<<(bpoint>>6 & 0x01)) & 0x01)<<6;// VB_CPLD_A0   0 -  63
			Byte += ((1<<(bpoint>>6 & 0x01)) & 0x02)<<6;// VB_CPLD_A1  64 - 127
		}
		sSeq.byteBuf[order] = Byte;
		order++;

		// 4 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VB_CPLD_A2-OUTPUT_START)/8;

		Byte = 0;
		if(bTRY == RY)
		{
			// RELAY
			Byte += ((1<<(bpoint>>6 & 0x01)) & 0x01)<<0;// VB_CPLD_A2   0 -  63
			Byte += ((1<<(bpoint>>6 & 0x01)) & 0x02)<<0;// VB_CPLD_A3  64 - 127
		}
		Byte += 1<<2;// _VB_PIN_SEL =1 (pin 등록 해제)
		Byte += (bpoint & 0x080)>>3;// VB_CARD_A0  0 - 2047
		Byte += (bpoint & 0x100)>>3;// VB_CARD_A1  0 - 2047
		Byte += (bpoint & 0x200)>>3;// VB_CARD_A2  0 - 2047
		Byte += (bpoint & 0x400)>>3;// VB_CARD_A3  0 - 2047
		sSeq.byteBuf[order] = Byte;
		order++;
		
		// 5 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VB_ENA_A_A0-OUTPUT_START)/8;

		Byte = 0;
		Byte += ((1<<(bpoint>>11 & 0x01)) ^ 0x01) & 0x01;// VB_ENA_A_A0
		Byte += ((1<<(bpoint>>11 & 0x01)) ^ 0x02) & 0x02;// VB_ENA_A_A1
		sSeq.byteBuf[order] = Byte;
		order++;

	}

} sRST_BPOINT;
*/

/*
typedef struct sRST_CPOINT
{
	int cpoint, bTRY;
	sSEQ sSeq;

	sRST_CPOINT()
	{
		cpoint = 0;
		bTRY = TR;
		sSeq.Init();
	}

	void Init()
	{
		int i=0, order=0;
		BYTE Byte=0;
		sSeq.Init();

		// 0 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VC_ENA_A_A0-OUTPUT_START)/8;

		Byte = 0;
		Byte += 1<<0;// VC_ENA_A_A0
		Byte += 1<<1;// VC_ENA_A_A1
		Byte += 1<<2;// VC_ENA_B_A0
		Byte += 1<<3;// VC_ENA_B_A1
		Byte += 1<<4;// VC_ENA_C_A0
		Byte += 1<<5;// VC_ENA_C_A1
		Byte += 1<<6;// VC_ENA_D_A0
		Byte += 1<<7;// VC_ENA_D_A1
		sSeq.byteBuf[order] = Byte;
		order++;
		
		// 1 :
		sSeq.cmd[order] = BIT_TRS;
		sSeq.byteBuf[order] = 1;
		if(((1<<(cpoint>>12 & 0x0f)) & 0x01)>>0) sSeq.BitNo[order] = _BOX_A_VC_RST-OUTPUT_START;//     0 -  4095
		if(((1<<(cpoint>>12 & 0x0f)) & 0x02)>>1) sSeq.BitNo[order] = _BOX_B_VC_RST-OUTPUT_START;//  4096 -  8191
		if(((1<<(cpoint>>12 & 0x0f)) & 0x04)>>2) sSeq.BitNo[order] = _BOX_C_VC_RST-OUTPUT_START;//  8192 - 12287
		if(((1<<(cpoint>>12 & 0x0f)) & 0x08)>>3) sSeq.BitNo[order] = _BOX_D_VC_RST-OUTPUT_START;// 12288 - 16383
		order++;

//		sSeq.cmd[order] = BYTE_TRS;
//		sSeq.PortNo[order] = (int)(BOX_A_VC_RST-OUTPUT_START)/8;
//
//		Byte = 0;
//		Byte += (1<<(cpoint>>12 & 0x0f)) & 0x01;// BOX_A_VC_RST      0 -  4095
//		Byte += (1<<(cpoint>>12 & 0x0f)) & 0x02;// BOX_B_VC_RST   4096 -  8191
//		Byte += (1<<(cpoint>>12 & 0x0f)) & 0x04;// BOX_C_VC_RST   8192 - 12287
//		Byte += (1<<(cpoint>>12 & 0x0f)) & 0x08;// BOX_D_VC_RST  12288 - 16383
//		sSeq.byteBuf[order] = Byte;
//		order++;

		// 2 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(BOX_VC_ENA_A-OUTPUT_START)/8;

		Byte = 0;
		Byte += ((1<<(cpoint>>12 & 0x0f)) ^ 0x01) & 0x01;// BOX_VC_ENABLE_A      0 -  4095
		Byte += ((1<<(cpoint>>12 & 0x0f)) ^ 0x02) & 0x02;// BOX_VC_ENABLE_B   4096 -  8191
		Byte += ((1<<(cpoint>>12 & 0x0f)) ^ 0x04) & 0x04;// BOX_VC_ENABLE_C   8192 - 12287
		Byte += ((1<<(cpoint>>12 & 0x0f)) ^ 0x08) & 0x08;// BOX_VC_ENABLE_D  12288 - 16383
		sSeq.byteBuf[order] = Byte;
		order++;

		// 3 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VC_PIN_A0-OUTPUT_START)/8;

		Byte = 0;
		Byte += cpoint & 0x01;// VC_PIN_A0  0 - 63
		Byte += cpoint & 0x02;// VC_PIN_A1  0 - 63 
		Byte += cpoint & 0x04;// VC_PIN_A2  0 - 63 
		Byte += cpoint & 0x08;// VC_PIN_A3  0 - 63 
		Byte += cpoint & 0x10;// VC_PIN_A4  0 - 63 j
		Byte += cpoint & 0x20;// VC_PIN_A5  0 - 63 

		if(bTRY == TR)
		{
			// TR
			Byte += ((1<<(cpoint>>6 & 0x01)) & 0x01)<<6;// VC_CPLD_A0   0 -  63
			Byte += ((1<<(cpoint>>6 & 0x01)) & 0x02)<<6;// VC_CPLD_A1  64 - 127
		}
		sSeq.byteBuf[order] = Byte;
		order++;

		// 4 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VC_CPLD_A2-OUTPUT_START)/8;

		Byte = 0;
		if(bTRY == RY)
		{
			// RELAY
			Byte += ((1<<(cpoint>>6 & 0x01)) & 0x01)<<0;// VC_CPLD_A2   0 -  63
			Byte += ((1<<(cpoint>>6 & 0x01)) & 0x02)<<0;// VC_CPLD_A3  64 - 127
		}
		Byte += 1<<2;// _VC_PIN_SEL
		Byte += (cpoint & 0x080)>>3;// VC_CARD_A0  0 - 2047
		Byte += (cpoint & 0x100)>>3;// VC_CARD_A1  0 - 2047
		Byte += (cpoint & 0x200)>>3;// VC_CARD_A2  0 - 2047
		Byte += (cpoint & 0x400)>>3;// VC_CARD_A3  0 - 2047
		sSeq.byteBuf[order] = Byte;
		order++;
		
		// 5 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VC_ENA_A_A0-OUTPUT_START)/8;

		Byte = 0;
		Byte += ((1<<(cpoint>>11 & 0x01)) ^ 0x01) & 0x01;// VC_ENA_A_A0
		Byte += ((1<<(cpoint>>11 & 0x01)) ^ 0x02) & 0x02;// VC_ENA_A_A1
		sSeq.byteBuf[order] = Byte;
		order++;
	}

} sRST_CPOINT;
*/


// 기능별로 비트를 ON 하고 싶을 때 사용하는 범용 구조체
typedef struct sSET_BIT		// COMMON BIT STRUCT
{
	int  type;
	BOOL bState;
	sSEQ sSeq;

	sSET_BIT()
	{
		type   = I_GAIN;
		bState = 0;
		sSeq.Init();
	}

	void Init()
	{
		int order=0;
		sSeq.Init();

		switch (type)
		{
			case I_GAIN:			// ex) MM.Set(I_GAIN, _ON);
			//-------------------------------------
			// I_GAIN_CMD bit Set : 
			// order 0 :
			sSeq.cmd[order] = BIT_TRS;			
			sSeq.BitNo[order] = I_GAIN_CMD-OUTPUT_START;	// ex) I_GAIN sSeq.BitNo[0] = 57
			sSeq.byteBuf[order] = bState;					// ex)        sSeq.byteBuf[0] = _ON(1)
			order++;
			break;

			case CC_CV:			// ex) MM.Set(CC_CV, 0);	 0: CC, 1: CV
			//-------------------------------------
			// CC_CV_MODE bit Set : 
			// order 0 :
			sSeq.cmd[order] = BIT_TRS;			
			sSeq.BitNo[order] = CC_CV_MODE-OUTPUT_START;	// ex) CC_CV sSeq.BitNo[0] = 120
			sSeq.byteBuf[order] = bState;					// ex)       sSeq.byteBuf[0] = 0  (0: CC)
			order++;
			break;
		}

	}

} sSET_BIT;
