
#include "MicroMotion.h"
#include "MicroMotion_ICP.h"


#define TOTAL_DA_BIT_NUM		16	// Output : DA0~15
#define TOTAL_AD_BIT_NUM		32  // Input  : ADC_A_D0~15 + ADC_B_D0~15

enum D96_OUTPUT
{
	//----------- new
	DA0		= D96_OUTPUT_START+0,		// DO-00  00_PA-00 // VB, I , SPARK DA 0			// 16bit, 0 ~ 65535
	DA1,								// DO-01  00_PA-01 // VB, I , SPARK DA 1			// 16bit, 0 ~ 65535
	DA2,								// DO-02  00_PA-02 // VB, I , SPARK DA 2			// 16bit, 0 ~ 65535
	DA3,								// DO-03  00_PA-03 // VB, I , SPARK DA 3			// 16bit, 0 ~ 65535
	DA4,								// DO-04  00_PA-04 // VB, I , SPARK DA 4			// 16bit, 0 ~ 65535
	DA5,								// DO-05  00_PA-05 // VB, I , SPARK DA 5			// 16bit, 0 ~ 65535
	DA6,								// DO-06  00_PA-06 // VB, I , SPARK DA 6			// 16bit, 0 ~ 65535
	DA7,								// DO-07  00_PA-07 // VB, I , SPARK DA 7			// 16bit, 0 ~ 65535


	DA8,								// DO-08  01_PB-00 // VB, I , SPARK DA 8			// 16bit, 0 ~ 65535
	DA9,								// DO-09  01_PB-01 // VB, I , SPARK DA 9			// 16bit, 0 ~ 65535
	DA10,								// DO-10  01_PB-02 // VB, I , SPARK DA 10			// 16bit, 0 ~ 65535
	DA11,								// DO-11  01_PB-03 // VB, I , SPARK DA 11			// 16bit, 0 ~ 65535
	DA12,								// DO-12  01_PB-04 // VB, I , SPARK DA 12			// 16bit, 0 ~ 65535
	DA13,								// DO-13  01_PB-05 // VB, I , SPARK DA 13			// 16bit, 0 ~ 65535
	DA14,								// DO-14  01_PB-06 // VB, I , SPARK DA 14			// 16bit, 0 ~ 65535
	DA15,								// DO-15  01_PB-07 // VB, I , SPARK DA 15			// 16bit, 0 ~ 65535


	I_SET_CLK = D96_OUTPUT_START+16,	// DO-16  02_PC-00 // 전류 DA SET Clock			// 0->1 : POSITIVE TRIGGER 
	_I_SET_RST,	      					// DO-17  02_PC-01 // 전류 DA SET RESET			// 0 : RESET, 1 : RUN(동작)
	LV_SET_CLK_0,						// DO-18  02_PC-02 // LV 0 DA Set Clock			// 0->1 : POSITIVE TRIGGER (_LV_SET_RST_0을 1로 설정후)
	_LV_SET_RST_0,						// DO-19  02_PC-03 // LV 0 DA Set Reset			// 0 : RESET, 1 : RUN(동작)
	HV_SET_CLK_0,						// DO-20  02_PC-04 // HV 0 DA Set Clock			// 0->1 : POSITIVE TRIGGER (_HV_SET_RST_0을 1로 설정후)
	_HV_SET_RST_0,						// DO-21  02_PC-05 // HV 0 DA Set Reset			// 0 : RESET, 1 : RUN(동작)
	LV_SET_CLK_1,						// DO-22  02_PC-06 // LV 1 DA Set Clock			// 0->1 : POSITIVE TRIGGER (_LV_SET_RST_1을 1로 설정후)
	_LV_SET_RST_1,						// DO-23  02_PC-07 // LV 1 DA Set Reset			// 0 : RESET, 1 : RUN(동작)


    ADC_I_SLOW_SEL,                     // DO-24  03_PA-10 // 전류 필터 SLOW 모드 (ADC_VB_SEL 과 ADC_I_FAST_SEL과 동시 사용 금지)					
	VC_FT,     							// DO-25  03_PA-11 // VC 필터 모드  				// VC Filter Fast 모드 (F4_WIRE_CMD, HV_OPEN_CMD 동시 사용 금지)
	_SPARK_RST,		                    // DO-26  03_PA-12 // SPARK 동작 설정				// 0 : 미동작, 1 : 동작 및 SPARK DETECTION 신호 초기화
	PULSE_MODE_CMD,		                // DO-27  03_PA-13 // 미사용(test용) 
	HV_RELAY_IN_IO_0,					// DO-28  03_PA-14	// HV 0 CHANNEL ON				// VB - HV 0 RELAY ON
	HV_RELAY_IN_IO_1,					// DO-29  03_PA-15	// HV 1 CHANNEL ON				// VB - HV 1 RELAY ON
	LV_RELAY_IN_IO_0,					// DO-30  03_PA-16	// LV 0 CHANNEL ON				// VB - LV 0 RELAY ON
	LV_RELAY_IN_IO_1,					// DO-31  03_PA-17	// LV 1 CHANNEL ON				// VB - LV 1 RELAY ON


	HV_OPEN_CMD, 						// DO-32  04_PB-10 // HV OPEN TEST 				// HV_OPEN Test 시 선택, VC_FT, 전류 필터는 선택할 것.
	I_GAIN_CMD,						    // DO-33  04_PB-11 // 전류 Gain 조정 모드			// 0: x1배 정상모드, 1: x10배(Short측정 시 사용)
	PIN_SEARCH_CMD,					    // DO-34  04_PB-12 // PIN SEARCH ON               
	HVGB_DISCHARGE,					    // DO-35  04_PB-13 // VB DISCHARGE 
	F4_WIRE_GAIN_A0,					// DO-36  04_PB-14 // 4 WIRE GAIN A0	 A0A1  00: Slow Gain x1000 		01: Fast Gain x10
	F4_WIRE_GAIN_A1,					// DO-37  04_PB-15 // 4 WIRE GAIN A1	       10: Fast Gain x100		11: Fast Gain x1000
	EMBED_CMD_M,					    // DO-38  04_PB-16 // VB DISCHARGE 
	EMBED_32C_CMD,					    // DO-39  04_PB-17 // VB DISCHARGE 


	CC_CV_MODE,	    					// DO-40  05_PC-10 // CC/CV Mode Select		// 0: CC, 1: CV
	VB_AC_MODE,	    					// DO-41  05_PC-11 // Embedded Mode 활성화, 조합키로 사용. 
	VB_OUT_CHK,							// DO-42  05_PC-12 // VB 전압 체크				// 현재 VB단의 전압을 측정
										// DO-43  05_PC-13 // Spare 
	VB_GUARD_ON	= D96_OUTPUT_START+44,	// DO-44  05_PC-14 // VB 단의 TR을 이용한 Guarding		(현재 안씀. test용)
										// DO-45  05_PC-15 // Spare 
	VC_GUARD_ON = D96_OUTPUT_START+46, 	// DO-46  05_PC-16 // VC 단의 TR을 이용한 Guarding		(현재 안씀. test용)
										// DO-47  05_PC-17 // Spare 

	HV_SET_CLK_1 = D96_OUTPUT_START+48,	// DO-48  06_PA-20 // HV 1 DA Set Clock			// 0->1 : POSITIVE TRIGGER (_HV_SET_RST_1을 1로 설정후)
	_HV_SET_RST_1,						// DO-49  06_PA-21 // HV 1 DA Set Reset			// 0 : RESET, 1 : RUN(동작)
	SPARK_SET_CLK,						// DO-50  06_PA-22 // SPARK DA Set Clock			// 0->1 : POSITIVE TRIGGER (_SPARK_SET_RST를 1로 설정후)
	_SPARK_SET_RST,						// DO-51  06_PA-23 // SPARK DA Set Reset			// 0 : RESET, 1 : RUN(동작)
										// DO-52  06_PC-24 // Spare 
										// DO-53  06_PC-25 // Spare 
	OPEN_GAIN_A0= D96_OUTPUT_START+54,  // DO-54  06_PA-26 // 오픈 전압별  GAIN 회로 4가지 선택
	OPEN_GAIN_A1,						// DO-55  06_PA-27 // A0A1   00: GAIN1, 10: GAIN2, 01: GAIN3, 11: GAIN4


										// DO-56  07_PB-20 // Spare 
	_AD_CONVST 	= D96_OUTPUT_START+57,	// DO-57  07_PB-21 // ADC CONVERSION START  		// 1->O : Nagative Trigger (Falling Edge 동작)
										// DO-58  07_PB-22 // Spare 
										// DO-59  07_PB-23 // Spare 
										// DO-60  07_PB-24 // Spare 
										// DO-61  07_PB-25 // Spare 
										// DO-62  07_PB-26 // Spare 
										// DO-63  07_PB-27 // Spare 


    ADC_I_FAST_SEL= D96_OUTPUT_START+64,// DO-64  08_PC-20 // 전류 필터 FAST 모드 (ADC_VB_SEL 과 ADC_I_SLOW_SEL과 동시 사용 금지)					
	F4_WIRE_CMD,                        // DO-65  08_PC-21 // 4-WIRE 측정 선택   
                                        //                 4-WIRE 릴레이 ON, 4-WIRE필터 자동선택, VC_FAST_FT, FC_SLOW_FT OFF 할 것, 전류필터는 선택 
	VC_SEN_SEL,     					// DO-66  08_PC-22	// VC 고압-저압 읽기 채널 선택 // 0: HV,  1: LV
	VC_LV_SEL,      					// DO-67  08_PC-23	// VC 저압 채널 ON/OFF     	// 0: OFF, 1: ON    
	VC_HV_SEL,      					// DO-68  08_PC-24	// VC 고압 채널 ON/OFF     	// 0: OFF, 1: ON
	VB_SEN_SEL,	   						// DO-69  08_PC-25	// VB 고압-저압 읽기 채널 선택 // 0: LV,  1: HV	
	VB_LV_SEN_SEL,  					// DO-70  08_PC-26	// VB 저압 읽기 채널 선택      // 0: LV0, 2: LV1	
	VB_HV_SEN_SEL,  					// DO-71  08_PC-27	// VB 고압 읽기 채널 선택	    // 0: HV0, 1: HV1


										// DO-72  09_PA-30 // Spare 
										// DO-73  09_PA-31 // Spare 
										// DO-74  09_PA-32 // Spare 
										// DO-75  09_PA-33 // Spare 
										// DO-76  09_PA-34 // Spare 
										// DO-77  09_PA-35 // Spare 
										// DO-78  09_PA-36 // Spare 
										// DO-79  09_PA-37 // Spare 
										
                           
	F4_WIRE_SELF_S1= D96_OUTPUT_START+80,//DO-80  10_PB-30 // 4-WIRE S1(VB) 릴레이 셀프 테스트
	F4_WIRE_SELF_S2,				   	// DO-81  10_PB-31 // 4-WIRE S2(VC) 릴레이 셀프 테스트
	ADC_VB_SEL,         				// DO-82  10_PB-32 // ADC VB CHANNEL SENSING 		// ADC A 채널에 VB Filter 선택
	RNG_SET_4,	    					// DO-83  10_PB-33 // RANGE SET 4					// 1: Range Select
    RNG_SET_3,	    					// DO-84  10_PB-34 // RANGE SET 3					// 1: Range Select
    RNG_SET_2,	    					// DO-85  10_PB-35 // RANGE SET 2					// 1: Range Select
    RNG_SET_1,	    					// DO-86  10_PB-36 // RANGE SET 1					// 1: Range Select
	RNG_SET_0,							// DO-87  10_PB-37 // RANGE SET 0					// 1: Range Select


										// DO-88  11_PC-30 // Spare 
										// DO-89  11_PC-31 // Spare 
										// DO-90  11_PC-32 // Spare 
										// DO-91  11_PC-33 // Spare 
										// DO-92  11_PC-34 // Spare 
										// DO-93  11_PC-35 // Spare 
										// DO-94  11_PC-36 // Spare 
										// DO-95  11_PC-37 // Spare 
                           

/*  Contec V43 에는 있는데 ICPDAS에는 없는 비트

	VB_GUARD_SEL,							// DO-?? // VB CC/CV Guarding 선택		// 0 : CV, 1 : CC
	VC_GUARD_SEL,							// DO-?? // VC CC/CV Guarding 선택		// 0 : CV, 1 : CC
	
//	SPARK_SET_CE,							// DO-    // SPARK DA Chip Enable		// /SPARK_SET_CLR가 1 로 설정 후 0->1 Rising edge로 동작
//	_SPARK_SET_CLR,							// DO-    // SPARK DA Clear				// 0 : Clear, 1 : 동작
*/

};

enum D64H_BD0_OUTPUT 
{
	VB_PIN_A0 = D64H_BD0_OUTPUT_START ,		// DO-00  // VB PIN SELCTION A0			// VB PIN 0~32767
	VB_PIN_A1,								// DO-01  // VB PIN SELCTION A1			// VB PIN 0~32767
	VB_PIN_A2,								// DO-02  // VB PIN SELCTION A2			// VB PIN 0~32767
	VB_PIN_A3,								// DO-03  // VB PIN SELCTION A3			// VB PIN 0~32767
	VB_PIN_A4,								// DO-04  // VB PIN SELCTION A4			// VB PIN 0~32767
	VB_PIN_A5,								// DO-05  // VB PIN SELCTION A5			// VB PIN 0~32767
	VB_PIN_A6,								// DO-06  // VB PIN SELCTION A6			// VB PIN 0~32767
	VB_PIN_A7,								// DO-07  // VB PIN SELCTION A7			// VB PIN 0~32767

	VB_PIN_A8,								// DO-08  // VB PIN SELCTION A8			// VB PIN 0~32767
	VB_PIN_A9,								// DO-09  // VB PIN SELCTION A9			// VB PIN 0~32767
	VB_PIN_A10,								// DO-10  // VB PIN SELCTION A10		// VB PIN 0~32767
	VB_PIN_A11,								// DO-11  // VB PIN SELCTION A11		// VB PIN 0~32767
	VB_PIN_A12,								// DO-12  // VB PIN SELCTION A12		// VB PIN 0~32767
	VB_PIN_A13,								// DO-13  // VB PIN SELCTION A13		// VB PIN 0~32767
	VB_PIN_A14,								// DO-14  // VB PIN SELCTION A14		// VB PIN 0~32767
	VB_PIN_A15,								// DO-15  // VB PIN SELCTION A15		// VB PIN 0~32767

	VB_TR_SEL = D64H_BD0_OUTPUT_START +16,	// DO-16  // VB TR 등록					// TR 등록시 1,     RY와는 배타적으로 설정된다.
	VB_RY_SEL,								// DO-17  // VB Relay 등록				// Relay 등록시 1,  TR과는 배타적으로 설정된다.
	_VB_PIN_SEL,							// DO-18  // VB PIN 등록/해제			// 0 : 등록, 1: : 해제
	_VB_ENA,								// DO-19  // 핀등록 Trigger 신호			// 1 -> 0  Falling Edge 시 등록
	_VB_RESET,								// DO-20  // VB RESET					// 0: VB pin 전체 초기화,  1: 등록
											// DO-21  // SPARE
	VB_PIN_A16 = D64H_BD0_OUTPUT_START +22,	// DO-22  // VB PIN SELCTION A16(예비)	// VB PIN 0~32767
	VB_PIN_A17,								// DO-23  // VB PIN SELCTION A17(예비)	// VB PIN 0~32767

	EXT_TRG,								// DO-24  // EMBEDDED TRG (테스트용) 
											// DO-25  // SPARE
											// DO-26  // SPARE
											// DO-27  // SPARE
											// DO-28  // SPARE
											// DO-29  // SPARE
											// DO-30  // SPARE
											// DO-31  // SPARE
};

enum D64H_BD0_INPUT 
{
	ADC_A_D0 = D64H_BD0_INPUT_START,		// DI-00  // ADC A DATA D0			// 16bit, 0 ~ 65535
	ADC_A_D1,								// DI-01  // ADC A DATA D1			// 16bit, 0 ~ 65535
	ADC_A_D2,								// DI-02  // ADC A DATA D2			// 16bit, 0 ~ 65535
	ADC_A_D3,								// DI-03  // ADC A DATA D3			// 16bit, 0 ~ 65535
	ADC_A_D4,								// DI-04  // ADC A DATA D4			// 16bit, 0 ~ 65535
	ADC_A_D5,								// DI-05  // ADC A DATA D5			// 16bit, 0 ~ 65535
	ADC_A_D6,								// DI-06  // ADC A DATA D6			// 16bit, 0 ~ 65535
	ADC_A_D7,								// DI-07  // ADC A DATA D7			// 16bit, 0 ~ 65535

	ADC_A_D8,								// DI-08  // ADC A DATA D8			// 16bit, 0 ~ 65535
	ADC_A_D9,								// DI-09  // ADC A DATA D9			// 16bit, 0 ~ 65535
	ADC_A_D10,								// DI-10  // ADC A DATA D10			// 16bit, 0 ~ 65535
	ADC_A_D11,								// DI-11  // ADC A DATA D11			// 16bit, 0 ~ 65535
	ADC_A_D12,								// DI-12  // ADC A DATA D12			// 16bit, 0 ~ 65535
	ADC_A_D13,								// DI-13  // ADC A DATA D13			// 16bit, 0 ~ 65535
	ADC_A_D14,								// DI-14  // ADC A DATA D14			// 16bit, 0 ~ 65535
	ADC_A_D15,								// DI-15  // ADC A DATA D15			// 16bit, 0 ~ 65535

	ADC_B_D0,								// DI-16  // ADC B DATA D0			// 16bit, 0 ~ 65535
	ADC_B_D1,								// DI-17  // ADC B DATA D1			// 16bit, 0 ~ 65535
	ADC_B_D2,								// DI-18  // ADC B DATA D2			// 16bit, 0 ~ 65535
	ADC_B_D3,								// DI-19  // ADC B DATA D3			// 16bit, 0 ~ 65535
	ADC_B_D4,								// DI-20  // ADC B DATA D4			// 16bit, 0 ~ 65535
	ADC_B_D5,								// DI-21  // ADC B DATA D5			// 16bit, 0 ~ 65535
	ADC_B_D6,								// DI-22  // ADC B DATA D6			// 16bit, 0 ~ 65535
	ADC_B_D7,								// DI-23  // ADC B DATA D7			// 16bit, 0 ~ 65535

	ADC_B_D8,								// DI-24  // ADC B DATA D8			// 16bit, 0 ~ 65535
	ADC_B_D9,								// DI-25  // ADC B DATA D9			// 16bit, 0 ~ 65535
	ADC_B_D10,								// DI-26  // ADC B DATA D10			// 16bit, 0 ~ 65535
	ADC_B_D11,								// DI-27  // ADC B DATA D11			// 16bit, 0 ~ 65535
	ADC_B_D12,								// DI-28  // ADC B DATA D12			// 16bit, 0 ~ 65535
	ADC_B_D13,								// DI-29  // ADC B DATA D13			// 16bit, 0 ~ 65535
	ADC_B_D14,								// DI-30  // ADC B DATA D14			// 16bit, 0 ~ 65535
	ADC_B_D15,								// DI-31  // ADC B DATA D15			// 16bit, 0 ~ 65535
};

enum D64H_BD1_OUTPUT 
{
	VC_PIN_A0 = D64H_BD1_OUTPUT_START,		// DO-00  // VC PIN SELCTION A0			// VC PIN 0~32767
	VC_PIN_A1,								// DO-01  // VC PIN SELCTION A1			// VC PIN 0~32767
	VC_PIN_A2,								// DO-02  // VC PIN SELCTION A2			// VC PIN 0~32767
	VC_PIN_A3,								// DO-03  // VC PIN SELCTION A3			// VC PIN 0~32767
	VC_PIN_A4,								// DO-04  // VC PIN SELCTION A4			// VC PIN 0~32767
	VC_PIN_A5,								// DO-05  // VC PIN SELCTION A5			// VC PIN 0~32767
	VC_PIN_A6,								// DO-06  // VC PIN SELCTION A6			// VC PIN 0~32767
	VC_PIN_A7,								// DO-07  // VC PIN SELCTION A7			// VC PIN 0~32767

	VC_PIN_A8,								// DO-08  // VC PIN SELCTION A8			// VC PIN 0~32767
	VC_PIN_A9,								// DO-09  // VC PIN SELCTION A9			// VC PIN 0~32767
	VC_PIN_A10,								// DO-10  // VC PIN SELCTION A10		// VC PIN 0~32767
	VC_PIN_A11,								// DO-11  // VC PIN SELCTION A11		// VC PIN 0~32767
	VC_PIN_A12,								// DO-12  // VC PIN SELCTION A12		// VC PIN 0~32767
	VC_PIN_A13,								// DO-13  // VC PIN SELCTION A13		// VC PIN 0~32767
	VC_PIN_A14,								// DO-14  // VC PIN SELCTION A14		// VC PIN 0~32767
	VC_PIN_A15,								// DO-15  // VC PIN SELCTION A15		// VC PIN 0~32767

	VC_TR_SEL = D64H_BD1_OUTPUT_START +16,	// DO-16 // VC TR 등록					// TR 등록시 1,     RY와는 배타적으로 설정된다.
	VC_RY_SEL,								// DO-17 // VC Relay 등록				// Relay 등록시 1,  TR과는 배타적으로 설정된다.
	_VC_PIN_SEL,							// DO-18 // VC PIN 등록/해제			// 0 : 등록, 1: : 해제
	_VC_ENA,								// DO-19 // 핀등록 Trigger 신호			// 1 -> 0  Falling Edge 시 등록
	_VC_RESET,								// DO-20 // VC RESET					// 0: VC pin 전체 초기화,  1: 등록
											// DO-21  // SPARE
	VC_PIN_A16 = D64H_BD1_OUTPUT_START +22,	// DO-22  // VC PIN SELCTION A16(예비)	// VC PIN 0~32767
	VC_PIN_A17,								// DO-23  // VC PIN SELCTION A17(예비)	// VC PIN 0~32767

											// DO-24  // SPARE
											// DO-25  // SPARE
											// DO-26  // SPARE
											// DO-27  // SPARE
											// DO-28  // SPARE
											// DO-29  // SPARE
											// DO-30  // SPARE
											// DO-31  // SPARE
};


enum D64H_BD1_INPUT 
{

	SPARK_ON	= D64H_BD1_INPUT_START,		// DI-00  // SPARK SIGNAL					// 1 : SPARK 발생
											// DI-01  // SPARE
											// DI-02  // SPARE
											// DI-03  // SPARE
	DTHV,									// DI-04  // DETECTION MAIN POWER - HV	// 0 : ERROR, 1: 정상
	DT24V,									// DI-05  // DETECTION MAIN POWER - 24V	// 0 : ERROR, 1: 정상
	DT5V,									// DI-06  // DETECTION MAIN POWER - 5V	// 0 : ERROR, 1: 정상
	DT12V,									// DI-07  // DETECTION MAIN POWER - 12V	// 0 : ERROR, 1: 정상

											// DI-08  // SPARE
											// DI-09  // SPARE
											// DI-10  // SPARE
											// DI-11  // SPARE
											// DI-12  // SPARE
											// DI-13  // SPARE
											// DI-14  // SPARE
											// DI-15  // SPARE
											
											// DI-16  // SPARE
											// DI-17  // SPARE
											// DI-18  // SPARE
											// DI-19  // SPARE
											// DI-20  // SPARE
											// DI-21  // SPARE
	DT_BOT_BOX  = D64H_BD1_INPUT_START+22,  // DI-22  // 하부박스 Detection 
	DT_TOP_BOX,  							// DI-23  // 상부박스 Detection 

											// DI-24  // SPARE
											// DI-25  // SPARE
											// DI-26  // SPARE
											// DI-27  // SPARE
											// DI-28  // SPARE
											// DI-29  // SPARE
											// DI-30  // SPARE
											// DI-31  // SPARE

/*  Contec V43 에는 있는데 ICPDAS에는 없는 비트
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


	void Init()	
	{
		int reset=0, clock=0, order=0;
		sSeq.Init();

		switch(type)
		{
		default:
		case LV1:
			reset = _LV_SET_RST_0-D96_OUTPUT_START;		// ex) 19
			clock =  LV_SET_CLK_0-D96_OUTPUT_START;		// ex) 18
			break;

		case LV2:
			reset = _LV_SET_RST_1-D96_OUTPUT_START;		// ex) 23
			clock =  LV_SET_CLK_1-D96_OUTPUT_START;		// ex) 22
			break;

		case HV1:
			reset = _HV_SET_RST_0-D96_OUTPUT_START;		// ex) 21
			clock =  HV_SET_CLK_0-D96_OUTPUT_START;		// ex) 20
			break;

		case HV2:
			reset = _HV_SET_RST_1-D96_OUTPUT_START;		// ex) 49
			clock =  HV_SET_CLK_1-D96_OUTPUT_START;		// ex) 48
			break;

		case SPARK:
			reset = _SPARK_SET_RST-D96_OUTPUT_START;	// ex) 51
			clock =  SPARK_SET_CLK-D96_OUTPUT_START;	// ex) 50
			break;
		}

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
		// Value Data (2 Byte) 전송

		// order 3: 
		sSeq.cmd[order] = BYTE_TRS;	 
		sSeq.PortNo[order] = (WORD)((DA0-D96_OUTPUT_START) /8);		// sSeq.PortNo[3] = 0 
		sSeq.byteBuf[order] = (BYTE)(da_value & 0xff);			// sSeq.byteBuf[3] = da_value Low Byte 
		order++;

		// order 4: 
		sSeq.cmd[order] = BYTE_TRS;	 
		sSeq.PortNo[order] = (WORD)((DA8-D96_OUTPUT_START) /8);		// sSeq.PortNo[4] = 1 
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
		sSeq.BitNo[order] = VB_OUT_CHK-D96_OUTPUT_START;	// ex) LV1 sSeq.BitNo[0] = 42
		sSeq.byteBuf[order] = 1;							// ex)     sSeq.byteBuf[0] = 1;
		order++;

		//-------------------------------------
		// VB_SEN_SEL bit  1 : 회로보호를 위해 HV로 Set (0: LV,  1: HV) 
		// order 1 :
		sSeq.cmd[order] = BIT_TRS;			
		sSeq.BitNo[order] = VB_SEN_SEL-D96_OUTPUT_START;	// ex) LV1 sSeq.BitNo[0] = 69
		sSeq.byteBuf[order] = 1;							// ex)     sSeq.byteBuf[0] = 1;
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
			sSeq.mbitBuf[order][GPIO1-D96_OUTPUT_START] = 0;	// PULSE_FREQ_SET_100Hz
			sSeq.mbitBuf[order][GPIO0-D96_OUTPUT_START] = 0;
			break;

		case 1:// 1 KHz
			sSeq.mbitBuf[order][GPIO1-D96_OUTPUT_START] = 0;	// PULSE_FREQ_SET_1KHz
			sSeq.mbitBuf[order][GPIO0-D96_OUTPUT_START] = 1;
			break;
		
		case 2:// 10 KHz
			sSeq.mbitBuf[order][GPIO1-D96_OUTPUT_START] = 1;	// PULSE_FREQ_SET_10KHz
			sSeq.mbitBuf[order][GPIO0-D96_OUTPUT_START] = 0;
			break;

		case 3:// 100 KHz
			sSeq.mbitBuf[order][GPIO1-D96_OUTPUT_START] = 1;	// PULSE_FREQ_SET_100KHz
			sSeq.mbitBuf[order][GPIO0-D96_OUTPUT_START] = 1;
			break;
		}

		switch(volt)
		{
		case 0:// 1 V
			sSeq.mbitBuf[order][GPIO3-D96_OUTPUT_START] = 0;	// PULSE_VOLT_SET_1V
			sSeq.mbitBuf[order][GPIO2-D96_OUTPUT_START] = 0;
			break;

		case 1:// 2 V
			sSeq.mbitBuf[order][GPIO3-D96_OUTPUT_START] = 0;	// PULSE_VOLT_SET_2V
			sSeq.mbitBuf[order][GPIO2-D96_OUTPUT_START] = 1;
			break;
		
		case 2:// 5 V
			sSeq.mbitBuf[order][GPIO3-D96_OUTPUT_START] = 1;	// PULSE_VOLT_SET_5V
			sSeq.mbitBuf[order][GPIO2-D96_OUTPUT_START] = 0;
			break;

		case 3:// 10 V
			sSeq.mbitBuf[order][GPIO3-D96_OUTPUT_START] = 1;	// PULSE_VOLT_SET_10V
			sSeq.mbitBuf[order][GPIO2-D96_OUTPUT_START] = 1;
			break;
		}
		
		// Pulse Check를 위한 필터 설정
		switch(cav)
		{
		case 0:// 필터 0
			sSeq.mbitBuf[order][ADC_A_CH_A1-D96_OUTPUT_START] = 0;	// 00 : Pulse 측정 (PULSE_ADC_CMD)
			sSeq.mbitBuf[order][ADC_A_CH_A0-D96_OUTPUT_START] = 0;
			sSeq.mbitBuf[order][GPIO5-D96_OUTPUT_START] = 0; 		// C_AV_SET_0
			sSeq.mbitBuf[order][GPIO4-D96_OUTPUT_START] = 0;
			break;

		case 1:// 필터 1
			sSeq.mbitBuf[order][ADC_A_CH_A1-D96_OUTPUT_START] = 0;	// 00 : Pulse 측정 (PULSE_ADC_CMD)
			sSeq.mbitBuf[order][ADC_A_CH_A0-D96_OUTPUT_START] = 0;
			sSeq.mbitBuf[order][GPIO5-D96_OUTPUT_START] = 0; 		// C_AV_SET_1
			sSeq.mbitBuf[order][GPIO4-D96_OUTPUT_START] = 1;
			break;
		
		case 2:// 필터 2
			sSeq.mbitBuf[order][ADC_A_CH_A1-D96_OUTPUT_START] = 0;  // 00 : Pulse 측정 (PULSE_ADC_CMD)	
			sSeq.mbitBuf[order][ADC_A_CH_A0-D96_OUTPUT_START] = 0;
			sSeq.mbitBuf[order][GPIO5-D96_OUTPUT_START] = 1; 		// C_AV_SET_2
			sSeq.mbitBuf[order][GPIO4-D96_OUTPUT_START] = 0;
			break;

		case 3:// 필터 3
			sSeq.mbitBuf[order][ADC_A_CH_A1-D96_OUTPUT_START] = 0;  // 00 : Pulse 측정 (PULSE_ADC_CMD)	
			sSeq.mbitBuf[order][ADC_A_CH_A0-D96_OUTPUT_START] = 0;
			sSeq.mbitBuf[order][GPIO5-D96_OUTPUT_START] = 1; 		// C_AV_SET_3
			sSeq.mbitBuf[order][GPIO4-D96_OUTPUT_START] = 1;
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
				sSeq.mbitBuf[order][RNG_SET_0-D96_OUTPUT_START - i] = 0;		// RNG_SET_0~4:  bit87(p10_7)~bit83(p10_3)

			// 해당 range만 1로 Set
			sSeq.mbitBuf[order][RNG_SET_0-D96_OUTPUT_START - range] = 1;
			order++;
		}

		if(da_value >= 0)	// ex) OnButtonISet()=> MM.Set(I, I_value);
		{
			// order 0 : 
			sSeq.cmd[order] = MBIT_TRS;
			sSeq.mbitBuf[order][_I_SET_RST-D96_OUTPUT_START] = 1;	// 전류 DA Set Reset On		// mbitBuf[0][17] = 1
			sSeq.mbitBuf[order][I_SET_CLK-D96_OUTPUT_START] = 0;	// 전류 DA Set Clock Off	// mbitBuf[0][16] = 0
			order++;
			
	/*	
			// order 1 : 
			sSeq.cmd[order] = MBIT_TRS;
			for(int i=0; i<TOTAL_DA_BIT_NUM; i++) 		// Value Data 전송
				sSeq.mbitBuf[order][DA0-D96_OUTPUT_START+i] = (da_value>>i) & 1;
			order++;
	*/

			// order 1: 
			sSeq.cmd[order] = BYTE_TRS;	 				// Value Data 전송
			sSeq.PortNo[order] = (WORD)((DA0-D96_OUTPUT_START) /8);		// sSeq.PortNo[1] = 0 	 
			sSeq.byteBuf[order] = (BYTE)(da_value & 0xff);			// sSeq.byteBuf[1] = da_value Low Byte 
			order++;

			// order 2: 
			sSeq.cmd[order] = BYTE_TRS;	 
			sSeq.PortNo[order] = (WORD)((DA8-D96_OUTPUT_START) /8);		// sSeq.PortNo[2] = 1 
			sSeq.byteBuf[order] = (BYTE)((da_value >> 8) & 0xff);	// sSeq.byteBuf[2] = da_value High Byte 
			order++;

			// order 3 :
			sSeq.cmd[order] = DLY;			// 200 micro sec delay
			sSeq.PortNo[order] = usec;
			sSeq.mbitBuf[order][0] = 200;
			order++;
		
			// order 4 :
			sSeq.cmd[order] = BIT_TRS;					// bit16  0 -> 1
			sSeq.BitNo[order] = I_SET_CLK-D96_OUTPUT_START;	// 전류 DA Set Clock : 0 -> 1 trigger 
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
		sSeq.mbitBuf[order][VB_OUT_CHK-D96_OUTPUT_START] = 0;  	     // sSeq.mbitBuf[0][42: p05_b2]  = 0 (VB CH)

		//sSeq.mbitBuf[order][ADC_A_CH_A1-D96_OUTPUT_START] = 0; // 01 : VB 전압 체크 (VB_CH_CHK_CMD)
		//sSeq.mbitBuf[order][ADC_A_CH_A0-D96_OUTPUT_START] = 1; 
		
		switch(type)
		{
		case LV1:
			sSeq.mbitBuf[order][VB_SEN_SEL-D96_OUTPUT_START] = 0;    // sSeq.mbitBuf[0][69: p08 b5]: 0 (LV)			
			sSeq.mbitBuf[order][VB_LV_SEN_SEL-D96_OUTPUT_START] = 0; // sSeq.mbitBuf[0][70: p08 b6]: 0 (LV0: SW LV1)
			break;

		case LV2:
			sSeq.mbitBuf[order][VB_SEN_SEL-D96_OUTPUT_START] = 0;    // sSeq.mbitBuf[0][69: p08 b5]: 0 (LV)	
			sSeq.mbitBuf[order][VB_LV_SEN_SEL-D96_OUTPUT_START] = 1; // sSeq.mbitBuf[0][70: p08 b6]: 1 (LV1: SW LV2)
			break;

		case HV1:
			sSeq.mbitBuf[order][VB_SEN_SEL-D96_OUTPUT_START] = 1;    // sSeq.mbitBuf[0][69: p08 b5]: 1 (HV) 	
			sSeq.mbitBuf[order][VB_HV_SEN_SEL-D96_OUTPUT_START] = 0; // sSeq.mbitBuf[0][71: p08 b7]: 0 (HV0: SW HV1)
			break;

		case HV2:
			sSeq.mbitBuf[order][VB_SEN_SEL-D96_OUTPUT_START] = 1;    // sSeq.mbitBuf[0][69: p08 b5]: 1 (HV)
			sSeq.mbitBuf[order][VB_HV_SEN_SEL-D96_OUTPUT_START] = 1; // sSeq.mbitBuf[0][71: p08 b7]: 1 (HV1: SW HV2)
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
//20170523  PULSE 관련 기능은 추후 구현
			// 내부조합표 참조
		case PFFT:	// PULSE_FILTER_SET(FAST)
			sSeq.mbitBuf[order][ADC_A_CH_A1-D96_OUTPUT_START] = 0;		// 00 : Pulse 측정 (PULSE_ADC_CMD)
			sSeq.mbitBuf[order][ADC_A_CH_A0-D96_OUTPUT_START] = 0;
			sSeq.mbitBuf[order][GPIO6-D96_OUTPUT_START] = 0;
			break;

		case PSFT:	// PULSE_FILTER_SET(SLOW)
			sSeq.mbitBuf[order][ADC_A_CH_A1-D96_OUTPUT_START] = 0;		// 00 : Pulse 측정  (PULSE_ADC_CMD)
			sSeq.mbitBuf[order][ADC_A_CH_A0-D96_OUTPUT_START] = 0;
			sSeq.mbitBuf[order][GPIO6-D96_OUTPUT_START] = 1;
			break;
*/

		case VBFT:	// ADC_VB_SEL 
			sSeq.mbitBuf[order][ADC_VB_SEL-D96_OUTPUT_START] = 1;		// bit82(p10_b2) 1 : ADC VB Filter Channel Sensing ON
			sSeq.mbitBuf[order][ADC_I_FAST_SEL-D96_OUTPUT_START] = 0;	// bit64(p08_b0) 0 : ADC_VB_SEL ON 시에 OFF
			sSeq.mbitBuf[order][ADC_I_SLOW_SEL-D96_OUTPUT_START] = 0;	// bit24(p03_b0) 0 : ADC_VB_SEL ON 시에 OFF
			break;


		case IFFT:	// ADC_I_FAST_SEL
			sSeq.mbitBuf[order][ADC_VB_SEL-D96_OUTPUT_START] = 0;		// bit82(p10 b2) 0 : ADC_I_FAST_SEL ON 시에 OFF 
			sSeq.mbitBuf[order][ADC_I_FAST_SEL-D96_OUTPUT_START] = 1;	// bit64(p08 b0) 1 : 전류 필터 fast mode ON
			sSeq.mbitBuf[order][ADC_I_SLOW_SEL-D96_OUTPUT_START] = 0;	// bit24(p03 b0) 0 : ADC_I_FAST_SEL ON 시에 OFF
			break;

		case ISFT:	// ADC_I_SLOW_SEL
			sSeq.mbitBuf[order][ADC_VB_SEL-D96_OUTPUT_START] = 0;		// bit82(p10 b2) 0 : ADC_I_SLOW_SEL ON 시에 OFF 
			sSeq.mbitBuf[order][ADC_I_FAST_SEL-D96_OUTPUT_START] = 0;	// bit64(p08 b0) 0 : ADC_I_SLOW_SEL ON 시에 OFF  
			sSeq.mbitBuf[order][ADC_I_SLOW_SEL-D96_OUTPUT_START] = 1;	// bit24(p03 b0) 1 : 전류 필터 slow mode ON 
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
			sSeq.mbitBuf[order][VC_HV_SEL-D96_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][68: p8_4] = 0
			sSeq.mbitBuf[order][VC_LV_SEL-D96_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][67: p8_3] = 0
			sSeq.mbitBuf[order][VC_SEN_SEL-D96_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][66: p8_2] = 0 

		}
		else
		{
/*
			// VC Filter Fast 모드 (F4_WIRE_CMD, HV_OPEN_CMD 동시 사용 금지)
			sSeq.mbitBuf[order][F4_WIRE_CMD-D96_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][46] = 0 
			sSeq.mbitBuf[order][HV_OPEN_CMD-D96_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][56] = 0
			sSeq.mbitBuf[order][VC_FT-D96_OUTPUT_START] = 1;		// sSeq.mbitBuf[0][49] = 1 
*/
		
			// VC_HV_SEL 과 VC_LV_SEL 은 동시에 1이 될 수 없다.
			switch(type)
			{
			case HVC:
				sSeq.mbitBuf[order][VC_HV_SEL-D96_OUTPUT_START] = 1;	// sSeq.mbitBuf[0][68: p8_4] = 1
				sSeq.mbitBuf[order][VC_LV_SEL-D96_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][67: p8_3] = 0
				sSeq.mbitBuf[order][VC_SEN_SEL-D96_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][66: p8_2] = 0 (0: HV)
				break;

			case LVC:
				sSeq.mbitBuf[order][VC_HV_SEL-D96_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][68: p8_4] = 0
				sSeq.mbitBuf[order][VC_LV_SEL-D96_OUTPUT_START] = 1;	// sSeq.mbitBuf[0][67: p8_3] = 1
				sSeq.mbitBuf[order][VC_SEN_SEL-D96_OUTPUT_START] = 1;	// sSeq.mbitBuf[0][66: p8_2] = 1 (1: LV)
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

			//OPEN_GAIN_A0= D96_OUTPUT_START+54,  // DO-54  06_PA-26 // 오픈 전압별  GAIN 회로 4가지 선택
			//OPEN_GAIN_A1,						  // DO-55  06_PA-27 // A0A1   00: GAIN1, 10: GAIN2, 01: GAIN3, 11: GAIN4
			// 							byte내  비트순서로 맞추면    => A1A0   00(1): GAIN1, 01(2): GAIN2, 10(3): GAIN3, 11(4): GAIN4
			sSeq.mbitBuf[order][OPEN_GAIN_A1-D96_OUTPUT_START] = ((gain-1)>>1) & 1;	// 2'nd bit		// 55: p6_7
			sSeq.mbitBuf[order][OPEN_GAIN_A0-D96_OUTPUT_START] = (gain-1) & 1;		// 1'st bit		// 54: p6_6
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
			sSeq.mbitBuf[order][F4_WIRE_CMD-D96_OUTPUT_START] = 1;	// F4_WIRE_CMD ON
			sSeq.mbitBuf[order][HV_OPEN_CMD-D96_OUTPUT_START] = 0;	// HV_OPEN_CMD OFF
			sSeq.mbitBuf[order][VC_FT-D96_OUTPUT_START] = 0;		// VC FAST OFF
			*/
	
			//F4_WIRE_GAIN_A0,		// DO-36  04_PB-14 // 4 WIRE GAIN A0	 A0A1  00(1): Slow Gain x1000 	01(2): Fast Gain x10
			//F4_WIRE_GAIN_A1,		// DO-37  04_PB-15 // 4 WIRE GAIN A1	       10(3): Fast Gain x100	11(4): Fast Gain x1000
			//
			//                                   byte내  비트순서로 맞추면 =>A1A0  00(1): Slow Gain x1000   10(2): Fast Gain x10
			//                                                                     01(3): Fast Gain x100    11(4): Fast Gain x1000
			sSeq.mbitBuf[order][F4_WIRE_GAIN_A0-D96_OUTPUT_START] = ((mode-1)>>1) & 1;	// 2'nd bit		// 36: p4_4
			sSeq.mbitBuf[order][F4_WIRE_GAIN_A1-D96_OUTPUT_START] = (mode-1) & 1;		// 1'st bit		// 37: p4_5
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

		switch(type)
		{
	
		case VCFFT:		
			// VC Filter Fast 모드 (F4_WIRE_CMD, HV_OPEN_CMD 동시 사용 금지)
			sSeq.mbitBuf[order][F4_WIRE_CMD-D96_OUTPUT_START] = 0;	// mbitBuf[0][65: p8_1] 0
			sSeq.mbitBuf[order][HV_OPEN_CMD-D96_OUTPUT_START] = 0;	// mbitBuf[0][32: p4_0] 0
			sSeq.mbitBuf[order][VC_FT-D96_OUTPUT_START] = 1;		// mbitBuf[0][25: p3_1] 1 (VC Fast)
			break;
	
/*		
		case VCSFT:
			sSeq.mbitBuf[order][ADC_B_CH_A1-D96_OUTPUT_START] = 0;		// 01 : VC SLOW
			sSeq.mbitBuf[order][ADC_B_CH_A0-D96_OUTPUT_START] = 1;
			break;
*/

		case HVOFT:
			// HV OPEN 모드 (F4_WIRE_CMD, VC_FT  동시 사용 금지)
			sSeq.mbitBuf[order][F4_WIRE_CMD-D96_OUTPUT_START] = 0;	/// mbitBuf[0][65: p8_1] 0
			sSeq.mbitBuf[order][HV_OPEN_CMD-D96_OUTPUT_START] = 1;	/// mbitBuf[0][32: p4_0] 1 (HV OPEN)
			sSeq.mbitBuf[order][VC_FT-D96_OUTPUT_START] = 0;		/// mbitBuf[0][25: p3_1] 0
			break;

		case FWFT:
			// 4 Wire 모드 (HV_OPEN_CMD, VC_FT  동시 사용 금지)
			//sSeq.mbitBuf[order][VC_HV_SEL-D96_OUTPUT_START] = 0;	// mbitBuf[0][43] 0
			//sSeq.mbitBuf[order][VC_LV_SEL-D96_OUTPUT_START] = 0;	// mbitBuf[0][44] 0
			//sSeq.mbitBuf[order][VC_SEN_SEL-D96_OUTPUT_START] = 1;	// mbitBuf[0][45] 1	(LV)	<=20170624 신부성차장 요구로 삭제
			
			sSeq.mbitBuf[order][F4_WIRE_CMD-D96_OUTPUT_START] = 1;	// mbitBuf[0][65: p8_1] 1 (4WIRE)
			sSeq.mbitBuf[order][HV_OPEN_CMD-D96_OUTPUT_START] = 0;	// mbitBuf[0][32: p4_0] 0
			sSeq.mbitBuf[order][VC_FT-D96_OUTPUT_START] = 0;		// mbitBuf[0][25: p3_1] 0
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
		
/*
 		//2018.02.01  평소에 이 비트는 항상 1 상태를 유지해야 하므로 넣은 안전코드임.
 		//       전체 흐름상으로 이 비트가 0으로 유지되는 케이스는 없는 것으로 확인되었으므로  
 		//       Get 시간이 기존 ACE400보다 너무 느린 현상을 개선하기 위해 이 부분은 삭제함.
		// order 0 :
		sSeq.cmd[order] = BIT_TRS;
		sSeq.BitNo[order] = _AD_CONVST-D96_OUTPUT_START;	// bit57 : 1
		sSeq.byteBuf[order] = 1;
		order++;
*/

		// order 0 :
		if(bSkipDelay == FALSE)
		{
			sSeq.cmd[order] = DLY;
			sSeq.PortNo[order] = usec;
			sSeq.mbitBuf[order][0] = 4;	// 4 usec delay : 2018.1.31 김영필 부장님 요청
			order++;
		}


		// order 1 :
		sSeq.cmd[order] = BIT_TRS;
		sSeq.BitNo[order] = _AD_CONVST-D96_OUTPUT_START;
		sSeq.byteBuf[order] = 0;			// bit62 : 1 -> 0  Falling Edge
		order++;

		// order 2 :
		// _AD_CONVST  1로 원상 복구	(170619 신부성 차장 요청)
		// 2018.02.01  기존 ACE400과 동일하게 read 이전에 _AD_CONVST를 1로 복구하도록 수정. 
		//    Falling Edge(1->0)하고 원복(0->1)한 다음에 read해도 값을 읽는데는 지장이 없음. (신부성 차장 확인)
		sSeq.cmd[order] = BIT_TRS;
		sSeq.BitNo[order] = _AD_CONVST-D96_OUTPUT_START;	// bit62 : 1
		sSeq.byteBuf[order] = 1;
		order++;

		switch(typGet)
		{
		case V:			// 설정 전압(VB) 은 ADC_A의 VB Filter를 통해 획득

			//--------------------------
			// ADC_A Read
		
			// order 3, 4 :
			sSeq.bd_id[order] = BD_D64_0;
			sSeq.cmd[order] = _2BYTE_RCV;
			sSeq.PortNo[order] = (int)(ADC_A_D0 - D64H_BD0_INPUT_START)/8;
			sSeq.rcv_start = order;
			order += 2;
			break;

		case I:
			//--------------------------
			// ADC_A Read
			
			// order 3, 4 :
			sSeq.bd_id[order] = BD_D64_0;
			sSeq.cmd[order] = _2BYTE_RCV;
			sSeq.PortNo[order] = (int)(ADC_A_D0 - D64H_BD0_INPUT_START)/8;
			sSeq.rcv_start = order;
			order += 2;
			break;

		case VI:
			//--------------------------
			// ADC_A, ADC_B Read
			// order 3, 4, 5, 6 :			
			
			// 측정 전류를 ADC A에서 read,  측정 전압은(VC) ADC B 에서 read
			sSeq.bd_id[order] = BD_D64_0;
			sSeq.cmd[order] = _4BYTE_RCV;
			sSeq.PortNo[order] = (int)(ADC_A_D0 - D64H_BD0_INPUT_START)/8;
			sSeq.rcv_start = order;
			order += 4;
			break;
		}

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

		// order 0 :
		// ---HV, LV VB Relay Off
		sSeq.cmd[order] = MBIT_TRS;
		sSeq.mbitBuf[order][HV_RELAY_IN_IO_0-D96_OUTPUT_START] = 0;		// bit28: p03_4
		sSeq.mbitBuf[order][HV_RELAY_IN_IO_1-D96_OUTPUT_START] = 0;		// bit29: p03_5
		sSeq.mbitBuf[order][LV_RELAY_IN_IO_0-D96_OUTPUT_START] = 0;		// bit30: p03_6
		sSeq.mbitBuf[order][LV_RELAY_IN_IO_1-D96_OUTPUT_START] = 0;		// bit31: p03_7
		order++;

		// order 1 :
		sSeq.cmd[order] = MBIT_TRS;
		switch(type)
		{
		case LV1:
			sSeq.mbitBuf[order][LV_RELAY_IN_IO_0-D96_OUTPUT_START] = 1;	// bit30: p03_6
			break;

		case LV2:
			sSeq.mbitBuf[order][LV_RELAY_IN_IO_1-D96_OUTPUT_START] = 1;	// bit31: p03_7
			break;

		case HV1:
			sSeq.mbitBuf[order][HV_RELAY_IN_IO_0-D96_OUTPUT_START] = 1;	// bit28: p03_4
			break;

		case HV2:
			sSeq.mbitBuf[order][HV_RELAY_IN_IO_1-D96_OUTPUT_START] = 1;	// bit29: p03_5
			break;

/*
		case PULSE://5
			sSeq.mbitBuf[order][VB_RY_OUT_A2-D96_OUTPUT_START] = 1;
			sSeq.mbitBuf[order][VB_RY_OUT_A1-D96_OUTPUT_START] = 0;
			sSeq.mbitBuf[order][VB_RY_OUT_A0-D96_OUTPUT_START] = 1;
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
			sSeq.mbitBuf[order][HV_RELAY_IN_IO_0-D96_OUTPUT_START] = 0;
			sSeq.mbitBuf[order][HV_RELAY_IN_IO_1-D96_OUTPUT_START] = 0;
			sSeq.mbitBuf[order][LV_RELAY_IN_IO_0-D96_OUTPUT_START] = 0;
			sSeq.mbitBuf[order][LV_RELAY_IN_IO_1-D96_OUTPUT_START] = 0;
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
			// ---------------- All D96 Digital Out Bit(0~95) Off
			int i;
			sSeq.cmd[order] = MBIT_TRS;
			for(i = D96_OUTPUT_START; i < (D96_OUTPUT_START + D96_TOTAL_OUTPUT_NUM); i++)
				sSeq.mbitBuf[order][i-D96_OUTPUT_START] = 0;

			// 평소에 1이어야 하는 3개 bit(_AD_CONVST, _VB_ENA, _VC_ENA) 만  1로 설정
			sSeq.mbitBuf[order][_AD_CONVST-D96_OUTPUT_START] = 1;
			order++;


			// order 2 :
			// ---------------- All D64_0 Digital Out Bit(0~31) Off
			
			// _VB_ENA와  _VC_ENA는  다른 보드이므로 transceiver 주소를 다르게 줘야 함.
			sSeq.bd_id[order] = BD_D64_0;		
			sSeq.cmd[order] = MBIT_TRS;
			for(i = D64H_BD0_OUTPUT_START; i < (D64H_BD0_OUTPUT_START+D64_TOTAL_OUTPUT_NUM); i++)
				sSeq.mbitBuf[order][i-D64H_BD0_OUTPUT_START] = 0;	

			// 평소에 1이어야 하는 3개 bit(_AD_CONVST, _VB_ENA, _VC_ENA) 만  1로 설정
			sSeq.mbitBuf[order][_VB_ENA-D64H_BD0_OUTPUT_START] = 1;			// ex) 19
			order++;


			// order 3 :
			// ---------------- All D64_0 Digital Out Bit(0~31) Off
			
			// _VC_ENA는  다른 보드이므로 transceiver 주소를 다르게 줘야 함.
			sSeq.bd_id[order] = BD_D64_1;		
			sSeq.cmd[order] = MBIT_TRS;
			for(i = D64H_BD1_OUTPUT_START; i < (D64H_BD1_OUTPUT_START+D64_TOTAL_OUTPUT_NUM); i++)
				sSeq.mbitBuf[order][i-D64H_BD1_OUTPUT_START] = 0;	

			// 평소에 1이어야 하는 3개 bit(_AD_CONVST, _VB_ENA, _VC_ENA) 만  1로 설정
			sSeq.mbitBuf[order][_VC_ENA-D64H_BD1_OUTPUT_START] = 1;			// ex) 19
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
		sSeq.bd_id[order] = BD_D64_0;
		sSeq.cmd[order] = MBIT_TRS;	

		// _VB_ENA : 1 -> 0 Falling Edge를 위해 처음엔 1부터 Set
		sSeq.mbitBuf[order][_VB_ENA - D64H_BD0_OUTPUT_START] = 1;		// sSeq.mbitBuf[0][19]  = 1;
		// _VB_RESET 등록
		sSeq.mbitBuf[order][_VB_RESET - D64H_BD0_OUTPUT_START] = 1;		// sSeq.mbitBuf[0][20] = 1;
		order++;


		//------------------------
		// TR or RY 설정,  PIN_SEL 설정
		// order 1 :
		sSeq.bd_id[order] = BD_D64_0;
		sSeq.cmd[order] = MBIT_TRS;	
		if(bTRY == TR)	// TR 등록시 1, RY와 배타적으로 설정된다.
		{
			sSeq.mbitBuf[order][VB_TR_SEL - D64H_BD0_OUTPUT_START] = 1; 	// sSeq.mbitBuf[1][16] = 1;
			sSeq.mbitBuf[order][VB_RY_SEL - D64H_BD0_OUTPUT_START] = 0; 	// sSeq.mbitBuf[1][17] = 0;
		}
		else
		{
			sSeq.mbitBuf[order][VB_TR_SEL - D64H_BD0_OUTPUT_START] = 0; 	// sSeq.mbitBuf[1][16] = 0;
			sSeq.mbitBuf[order][VB_RY_SEL - D64H_BD0_OUTPUT_START] = 1; 	// sSeq.mbitBuf[1][17] = 1;
		}

		// _VB_PIN_SEL  0: pin 등록,  1: pin 해제
		if (bSel)
			sSeq.mbitBuf[order][_VB_PIN_SEL - D64H_BD0_OUTPUT_START] = 0;		// sSeq.mbitBuf[1][18] = 0;
		else
			sSeq.mbitBuf[order][_VB_PIN_SEL - D64H_BD0_OUTPUT_START] = 1;		// sSeq.mbitBuf[1][18] = 1;
		order++;
		
		//------------------------
		// Pin Value 설정
		// order 2: 
		sSeq.bd_id[order] = BD_D64_0;
		sSeq.cmd[order] = BYTE_TRS;	 				// port 8로 8bit Data 전송
		sSeq.PortNo[order] = (WORD)((VB_PIN_A0 - D64H_BD0_OUTPUT_START) /8);	// sSeq.PortNo[2] = 8 
		sSeq.byteBuf[order] = (BYTE)(bpoint & 0xff);							// sSeq.byteBuf[2] = bpoint Low Byte 
		order++;

		// order 3: 
		sSeq.bd_id[order] = BD_D64_0;
		sSeq.cmd[order] = BYTE_TRS;	 				// port 9로 7bit Data 전송
		sSeq.PortNo[order] = (WORD)((VB_PIN_A8 - D64H_BD0_OUTPUT_START) /8);	// sSeq.PortNo[3] = 9 
		sSeq.byteBuf[order] = (BYTE)((bpoint >> 8) & 0x7f);						// sSeq.byteBuf[3] = bpoint High Byte 
		order++;

		//------------------------
		// _VB_ENA : 1 -> 0 Falling Edge 동작
		// order 4 :
		sSeq.bd_id[order] = BD_D64_0;
		sSeq.cmd[order] = MBIT_TRS;	
		sSeq.mbitBuf[order][_VB_ENA - D64H_BD0_OUTPUT_START] = 0;		// sSeq.mbitBuf[4][19]  = 0;
		order++;

		//------------------------
		// _VB_ENA : 1 원상 복구 	(170619 신부성 차장 요청)
		//           _VB_ENA는 평소에 이상신호가 들어가지 않도록 1로 꺼 놓아야 한다.
		// _VB_RESET의 경우는 다음 pin 설정 직전에 0으로 설정해서 VC Pin 을 초기화해야 함.  
		// VB_VC_PointAllReset()을 Set_VbVcPin()보다 먼저 호출하여 해결.  측정을 해야 하므로 여기서 reset(0)하면 안된다.
		// order 5 :
		sSeq.bd_id[order] = BD_D64_0;
		sSeq.cmd[order] = MBIT_TRS;	
		sSeq.mbitBuf[order][_VB_ENA - D64H_BD0_OUTPUT_START] = 1;		// sSeq.mbitBuf[5][19]  = 1;
		order++;
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
		sSeq.bd_id[order] = BD_D64_1;
		sSeq.cmd[order] = MBIT_TRS;	

		// _VC_ENA : 1 -> 0 Falling Edge를 위해 처음엔 1부터 Set
		sSeq.mbitBuf[order][_VC_ENA - D64H_BD1_OUTPUT_START] = 1;		// sSeq.mbitBuf[0][19]  = 1;
		// _VC_RESET 등록
		sSeq.mbitBuf[order][_VC_RESET - D64H_BD1_OUTPUT_START] = 1;		// sSeq.mbitBuf[0][20] = 1;
		order++;


		//------------------------
		// TR or RY 설정,  PIN_SEL 설정
		// order 1 :
		sSeq.bd_id[order] = BD_D64_1;
		sSeq.cmd[order] = MBIT_TRS;	

		if(bTRY == TR)	// TR 등록시 1, RY와 배타적으로 설정된다.
		{
			sSeq.mbitBuf[order][VC_TR_SEL - D64H_BD1_OUTPUT_START] = 1; 	// sSeq.mbitBuf[1][16] = 1;
			sSeq.mbitBuf[order][VC_RY_SEL - D64H_BD1_OUTPUT_START] = 0; 	// sSeq.mbitBuf[1][17] = 0;
		}
		else
		{
			sSeq.mbitBuf[order][VC_TR_SEL - D64H_BD1_OUTPUT_START] = 0; 	// sSeq.mbitBuf[1][16] = 0;
			sSeq.mbitBuf[order][VC_RY_SEL - D64H_BD1_OUTPUT_START] = 1; 	// sSeq.mbitBuf[1][17] = 1;
		}

		// _VC_PIN_SEL  0: pin 등록,  1: pin 해제
		if (bSel)
			sSeq.mbitBuf[order][_VC_PIN_SEL - D64H_BD1_OUTPUT_START] = 0;	// sSeq.mbitBuf[1][18] = 0;
		else
			sSeq.mbitBuf[order][_VC_PIN_SEL - D64H_BD1_OUTPUT_START] = 1;	// sSeq.mbitBuf[1][18] = 1;
		order++;
		
		//------------------------
		// Pin Value 설정
		// order 2: 
		sSeq.bd_id[order] = BD_D64_1;
		sSeq.cmd[order] = BYTE_TRS;	 				// port10으로 Value Data 전송
		sSeq.PortNo[order] = (WORD)((VC_PIN_A0 - D64H_BD1_OUTPUT_START) /8);	// sSeq.PortNo[2] = 10 
		sSeq.byteBuf[order] = (BYTE)(cpoint & 0xff);				// sSeq.byteBuf[2] = cpoint Low Byte 
		order++;

		// order 3: 
		sSeq.bd_id[order] = BD_D64_1;
		sSeq.cmd[order] = BYTE_TRS;	 
		sSeq.PortNo[order] = (WORD)((VC_PIN_A8 - D64H_BD1_OUTPUT_START) /8);	// sSeq.PortNo[3] = 11 
		sSeq.byteBuf[order] = (BYTE)((cpoint >> 8) & 0x7f);			// sSeq.byteBuf[3] = cpoint High Byte 
		order++;

		//------------------------
		// _VC_ENA : 1 -> 0 Falling Edge
		// order 4 :
		sSeq.bd_id[order] = BD_D64_1;
		sSeq.cmd[order] = MBIT_TRS;	
		sSeq.mbitBuf[order][_VC_ENA - D64H_BD1_OUTPUT_START] = 0;		// sSeq.mbitBuf[4][19]  = 0;
		order++;

		//------------------------
		// _VC_ENA : 1  원상 복구		(170619 신부성 차장 요청)
		//           _VC_ENA는 평소에 이상신호가 들어가지 않도록 1로 꺼 놓아야 한다.
		// _VC_RESET의 경우는 다음 pin 설정 직전에 0으로 설정해서 VC Pin 을 초기화해야 함.  
		// VB_VC_PointAllReset()을 Set_VbVcPin()보다 먼저 호출하여 해결.  측정을 해야 하므로 여기서 reset(0)하면 안된다.
		// order 5 :
		sSeq.bd_id[order] = BD_D64_1;
		sSeq.cmd[order] = MBIT_TRS;	
		sSeq.mbitBuf[order][_VC_ENA - D64H_BD1_OUTPUT_START] = 1;		// sSeq.mbitBuf[5][19]  = 1;
		order++;
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

			if(bbox == BOX_A)	   sSeq.BitNo[order] = _BOX_A_VB_RST-D96_OUTPUT_START;
			else if(bbox == BOX_B) sSeq.BitNo[order] = _BOX_B_VB_RST-D96_OUTPUT_START;
			else if(bbox == BOX_C) sSeq.BitNo[order] = _BOX_C_VB_RST-D96_OUTPUT_START;
			else if(bbox == BOX_D) sSeq.BitNo[order] = _BOX_D_VB_RST-D96_OUTPUT_START;
			order++;
			break;

		case BOX_ALL:
			// 0 :
			sSeq.cmd[order] = BYTE_TRS;
			sSeq.PortNo[order] = (int)(_BOX_A_VB_RST-D96_OUTPUT_START)/8;

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

			if(cbox == BOX_A)	   sSeq.BitNo[order] = _BOX_A_VC_RST-D96_OUTPUT_START;
			else if(cbox == BOX_B) sSeq.BitNo[order] = _BOX_B_VC_RST-D96_OUTPUT_START;
			else if(cbox == BOX_C) sSeq.BitNo[order] = _BOX_C_VC_RST-D96_OUTPUT_START;
			else if(cbox == BOX_D) sSeq.BitNo[order] = _BOX_D_VC_RST-D96_OUTPUT_START;
			order++;
			break;

		case BOX_ALL:
			// 0 :
			sSeq.cmd[order] = BYTE_TRS;
			sSeq.PortNo[order] = (int)(_BOX_A_VC_RST-D96_OUTPUT_START)/8;

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
			if(((1<<(ptB>>12 & 0x0f)) & 0x01)>>0) sSeq.BitNo[order] = _BOX_A_VB_RST-D96_OUTPUT_START;
			if(((1<<(ptB>>12 & 0x0f)) & 0x02)>>1) sSeq.BitNo[order] = _BOX_B_VB_RST-D96_OUTPUT_START;
			if(((1<<(ptB>>12 & 0x0f)) & 0x04)>>2) sSeq.BitNo[order] = _BOX_C_VB_RST-D96_OUTPUT_START;
			if(((1<<(ptB>>12 & 0x0f)) & 0x08)>>3) sSeq.BitNo[order] = _BOX_D_VB_RST-D96_OUTPUT_START;
			order++;
		}

		// 1 :
		if(ptC >= 0)
		{
			sSeq.cmd[order] = BIT_TRS;
			sSeq.byteBuf[order] = 0;
			if(((1<<(ptC>>12 & 0x0f)) & 0x01)>>0) sSeq.BitNo[order] = _BOX_A_VC_RST-D96_OUTPUT_START;
			if(((1<<(ptC>>12 & 0x0f)) & 0x02)>>1) sSeq.BitNo[order] = _BOX_B_VC_RST-D96_OUTPUT_START;
			if(((1<<(ptC>>12 & 0x0f)) & 0x04)>>2) sSeq.BitNo[order] = _BOX_C_VC_RST-D96_OUTPUT_START;
			if(((1<<(ptC>>12 & 0x0f)) & 0x08)>>3) sSeq.BitNo[order] = _BOX_D_VC_RST-D96_OUTPUT_START;
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
				sSeq.bd_id[order] = BD_D64_0;
				sSeq.mbitBuf[order][VB_TR_SEL   - D64H_BD0_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][16]
				sSeq.mbitBuf[order][VB_RY_SEL   - D64H_BD0_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][17]
				sSeq.mbitBuf[order][_VB_PIN_SEL - D64H_BD0_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][18]

				// _VB_ENA : Falling Edge 이므로 1로 초기화 
				sSeq.mbitBuf[order][_VB_ENA     - D64H_BD0_OUTPUT_START] = 1;	// sSeq.mbitBuf[0][19]

				// _VB_RESET 0: VB PIN 전체 초기화 
				sSeq.mbitBuf[order][_VB_RESET   - D64H_BD0_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][20]
				break;

			case PT_VC:
				sSeq.bd_id[order] = BD_D64_1;
				sSeq.mbitBuf[order][VC_TR_SEL   - D64H_BD1_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][16]
				sSeq.mbitBuf[order][VC_RY_SEL   - D64H_BD1_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][17]
				sSeq.mbitBuf[order][_VC_PIN_SEL - D64H_BD1_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][18]

				// _VC_ENA : Falling Edge 이므로 1로 초기화 
				sSeq.mbitBuf[order][_VC_ENA     - D64H_BD1_OUTPUT_START] = 1;	// sSeq.mbitBuf[0][19]

				// _VC_RESET 0: VC PIN 전체 초기화 
				sSeq.mbitBuf[order][_VC_RESET   - D64H_BD1_OUTPUT_START] = 0;	// sSeq.mbitBuf[0][20]
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
		sSeq.PortNo[order] = (int)(VB_ENA_A_A0-D96_OUTPUT_START)/8;

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
		if(((1<<(bpoint>>12 & 0x0f)) & 0x01)>>0) sSeq.BitNo[order] = _BOX_A_VB_RST-D96_OUTPUT_START;//     0 -  4095
		if(((1<<(bpoint>>12 & 0x0f)) & 0x02)>>1) sSeq.BitNo[order] = _BOX_B_VB_RST-D96_OUTPUT_START;//  4096 -  8191
		if(((1<<(bpoint>>12 & 0x0f)) & 0x04)>>2) sSeq.BitNo[order] = _BOX_C_VB_RST-D96_OUTPUT_START;//  8192 - 12287
		if(((1<<(bpoint>>12 & 0x0f)) & 0x08)>>3) sSeq.BitNo[order] = _BOX_D_VB_RST-D96_OUTPUT_START;// 12288 - 16383
		order++;

//		sSeq.cmd[order] = BYTE_TRS;
//		sSeq.PortNo[order] = (int)(BOX_A_VB_RST-D96_OUTPUT_START)/8;
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
		sSeq.PortNo[order] = (int)(BOX_VB_ENA_A-D96_OUTPUT_START)/8;

		Byte = 0;
		Byte += ((1<<(bpoint>>12 & 0x0f)) ^ 0x01) & 0x01;// BOX_VB_ENABLE_A      0 -  4095
		Byte += ((1<<(bpoint>>12 & 0x0f)) ^ 0x02) & 0x02;// BOX_VB_ENABLE_B   4096 -  8191
		Byte += ((1<<(bpoint>>12 & 0x0f)) ^ 0x04) & 0x04;// BOX_VB_ENABLE_C   8192 - 12287
		Byte += ((1<<(bpoint>>12 & 0x0f)) ^ 0x08) & 0x08;// BOX_VB_ENABLE_D  12288 - 16383
		sSeq.byteBuf[order] = Byte;
		order++;

		// 3 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VB_PIN_A0-D96_OUTPUT_START)/8;

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
		sSeq.PortNo[order] = (int)(VB_CPLD_A2-D96_OUTPUT_START)/8;

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
		sSeq.PortNo[order] = (int)(VB_ENA_A_A0-D96_OUTPUT_START)/8;

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
		sSeq.PortNo[order] = (int)(VC_ENA_A_A0-D96_OUTPUT_START)/8;

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
		if(((1<<(cpoint>>12 & 0x0f)) & 0x01)>>0) sSeq.BitNo[order] = _BOX_A_VC_RST-D96_OUTPUT_START;//     0 -  4095
		if(((1<<(cpoint>>12 & 0x0f)) & 0x02)>>1) sSeq.BitNo[order] = _BOX_B_VC_RST-D96_OUTPUT_START;//  4096 -  8191
		if(((1<<(cpoint>>12 & 0x0f)) & 0x04)>>2) sSeq.BitNo[order] = _BOX_C_VC_RST-D96_OUTPUT_START;//  8192 - 12287
		if(((1<<(cpoint>>12 & 0x0f)) & 0x08)>>3) sSeq.BitNo[order] = _BOX_D_VC_RST-D96_OUTPUT_START;// 12288 - 16383
		order++;

//		sSeq.cmd[order] = BYTE_TRS;
//		sSeq.PortNo[order] = (int)(BOX_A_VC_RST-D96_OUTPUT_START)/8;
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
		sSeq.PortNo[order] = (int)(BOX_VC_ENA_A-D96_OUTPUT_START)/8;

		Byte = 0;
		Byte += ((1<<(cpoint>>12 & 0x0f)) ^ 0x01) & 0x01;// BOX_VC_ENABLE_A      0 -  4095
		Byte += ((1<<(cpoint>>12 & 0x0f)) ^ 0x02) & 0x02;// BOX_VC_ENABLE_B   4096 -  8191
		Byte += ((1<<(cpoint>>12 & 0x0f)) ^ 0x04) & 0x04;// BOX_VC_ENABLE_C   8192 - 12287
		Byte += ((1<<(cpoint>>12 & 0x0f)) ^ 0x08) & 0x08;// BOX_VC_ENABLE_D  12288 - 16383
		sSeq.byteBuf[order] = Byte;
		order++;

		// 3 :
		sSeq.cmd[order] = BYTE_TRS;
		sSeq.PortNo[order] = (int)(VC_PIN_A0-D96_OUTPUT_START)/8;

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
		sSeq.PortNo[order] = (int)(VC_CPLD_A2-D96_OUTPUT_START)/8;

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
		sSeq.PortNo[order] = (int)(VC_ENA_A_A0-D96_OUTPUT_START)/8;

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
			sSeq.BitNo[order] = I_GAIN_CMD-D96_OUTPUT_START;	// ex) I_GAIN sSeq.BitNo[0] = 33(p04_1)
			sSeq.byteBuf[order] = bState;						// ex)        sSeq.byteBuf[0] = _ON(1)
			order++;
			break;

			case CC_CV:			// ex) MM.Set(CC_CV, 0);	 0: CC, 1: CV
			//-------------------------------------
			// CC_CV_MODE bit Set : 
			// order 0 :
			sSeq.cmd[order] = BIT_TRS;			
			sSeq.BitNo[order] = CC_CV_MODE-D96_OUTPUT_START;	// ex) CC_CV sSeq.BitNo[0] = 40(p05_0)
			sSeq.byteBuf[order] = bState;						// ex)       sSeq.byteBuf[0] = 0  (0: CC)
			order++;
			break;
		}

	}

} sSET_BIT;



