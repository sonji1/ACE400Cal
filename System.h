//////////////////////////////////////////////////////////////////////
// System.h: interface for the CSystem class.
//
// 2017.11.1  written by sonji
//////////////////////////////////////////////////////////////////////

#if !defined(_SYSTEM_H_INCLUDED_)
#define _SYSTEM_H_INCLUDED_


//-----------------------
// TEST CONDITION enum

enum CAL_CC_CV {
	C_CC = 1,	
	C_CV = 2
};

enum CAL_SC_VB_TYPE	{ 
	C_LV1 = 1, 
	C_LV2 = 2, 
	C_HV1 = 3, 
	C_HV2 = 4 
};

enum CAL_T_TYPE
{
	TEST_NOT_USE = 0,	
	TEST_OPEN	 = 1,
	TEST_SHORT	 = 2,
	TEST_4W	  	 = 3
};


enum CAL_ADCB_FILTER
{
	FT_B_VC_FAST = 1,
	FT_B_VC_SLOW = 2,		// ���� �̻��
	FT_B_HV_OPEN = 3,
	FT_B_4WIRE   = 4
};

enum CAL_ADCA_FILTER
{
	FT_A_I_FAST = 1,
	FT_A_I_SLOW = 2,
	FT_A_VB = 3
};
enum CAL_I_RANGE
{
	IR1 = 1,
	IR2 = 2,
	IR3 = 3,
	IR4 = 4,
	IR5 = 5
};

enum CAL_I_HIGH_GAIN
{
	I_HIGH_GAIN_NOT_USE = 0,
	I_HIGH_GAIN_OFF = 1,
	I_HIGH_GAIN_ON = 2
};

enum CAL_4W_GAIN
{
	F4W_GAIN_NOT_USE = 0,
	F4W_GAIN_Sx1000  = 1,
	F4W_GAIN_Fx10    = 2,
	F4W_GAIN_Fx100   = 3,
	F4W_GAIN_Fx1000  = 4
};


//-----------------------
// TEST CONDITION ������

typedef struct sTEST_CONDITION_RANGE
{
	int nMinVal;
	int nMaxVal;
} sTEST_CONDITION_RANGE;



typedef struct sTEST_CONDITION
{

	char szCalModeName[20]; //                  Col0
	int nCC;                //                  Col1  // 1: CC ,  2: CV 
	int nVBSrc;				// VB source(nVRel) Col2  // 1: C_LV1, 2: C_LV2, 3: C_HV1, 4: C_HV2
	int nVSet;				// Voltage Set		Col3  // 1~ 300
	int nIRng;				// I Range			Col4  // 1~ 5
	int nISet; 				// I Set *10 		Col5  // ���� ���ÿ��� ������ 10�� ���� ����ؾ� ��.
													  // /10�� ���� IRange1,2�� mA,  IRange3,4,5�� uA(micro A) ����
	int nDelay;				// Delay 			Col6  // usec ���� (micro sec)
	int nAdCnt;				// AD�ݺ�����Ƚ��   Col7  // 1ȸ ������ ���� AD���� count  (1~10)
	int nFilter;			// ADC_B Filter		Col8  // 1: VcFast, 3: HvOpen, 4: 4W 
	int nIFilter;			// ADC_A Filter  	Col9  // 1: IFast,  2: ISlow,  3: VB
	int nHvOffset;			// HV Offset		Col10 // ���� �̻��
	int nHVGain;			// HV Open Gain		Col11 // 0: HvGain �̻��. 1~ 4
	int nIGain;				// I Gain			Col12 // 0: IGain �̻��.  1: x1, 2: x10
	int n4WGain;			// 4W Gain			Col13 // 0: 4W �̻��.     1: Slow x1000, 2: Fast x10, 3:Fast x100, 4:Fast x1000
	int sCount;				// ??? �뵵?        Col14
	int nTest;				// Test ���� 		Col15 // 0:TEST_NOT_USE 1:TEST_OPEN, 2:TEST_SHORT(Leak), 3:TEST_4W



/*
	int typVC;				// LVC, HVC
	int ftADC_A;			// VB_FT, CF_FT, CS_FT
	int ftADC_B;			// VCF_FT, VCS_FT, FW_FT, HVO_FT

	int typTest;			// POWER_TEST, LV_OPEN_TEST, HV_OPEN_TEST, SHORT_TEST, LEAK_TEST, REALY_TEST, FOUR_WIRE_TEST, EMBEDED_TEST
	int vVB;				// ����, volt
	int uI;					// ����, uA
	int ohmR;				// ����, ohm
	int usDelay;			// ADC ���� ��, �����ð�, usec
	int typGet;				// V, I, VI
	int hvoOffset, hvoGain;	// HV_OPEN_TEST
	int nMeasCnt;			// �ݺ�����Ƚ��
	int scVB;				// VB source : LV1, LV2, HV1, HV2
	int rngI;				// ���� ������
	int dacI;				// ����, DAC
	int adcVC;				// VC ����, ADC
	int dacVB;				// VB ����, DAC
	BOOL bOverCurrent;		// ���� ������ ��������, �������� ����� ������ Ŭ ��� True
	int adcVTrshd[MAX_SD_MODE+1];// ������������ [0]: Auto  [1~MAX_SD_MODE]: Self-test
	int adcITrshd[MAX_SD_MODE+1];// ������������ [0]: Auto  [1~MAX_SD_MODE]: Self-test

	sTEST_CONDITION()
	{
		typVC = ftADC_A = ftADC_B = -1;

		typTest = POWER_TEST;
		vVB = uI = ohmR = 0;
		usDelay = 0;
		typGet = VI;
		hvoOffset = hvoGain = 0;
		nMeasCnt = 1;

		scVB = rngI = -1;
		dacI = adcVC = dacVB = 0;
		bOverCurrent = FALSE;

		Zero(adcVTrshd); 
		Zero(adcITrshd);
	}
*/

} sTEST_CONDITION;

#define MAX_TEST_MODE		360			
#define NUM_CAL_CONDITION_COL	16
#define CAL_MODE_SHORT_START	200

#define CAL_MODE_4W_START	(171 -1)		// 170
#define CAL_MODE_4W_END		(175 -1)		// 174

#define CAL_MODE_2W_OPEN_START	0			// 170 �̸��̸� 2W Open 
#define CAL_MODE_2W_OPEN_END	(CAL_MODE_4W_START	-1)		// 169



//-----------------------
// RJIG PIN ������

typedef struct sRJIG_DATA
{
	int 	nNo;		// Col0 
	int 	nVbTrPin;	// Col3
	int 	nVbRyPin;	// Col4
	int 	nVcTrPin;	// Col5
	int 	nVcRyPin;	// Col6
	double 	dOrgR;		// Col7 // Original R Value   (Real H/W) 

} sRJIG_DATA;
#define MAX_2W_RJIG			96
#define MAX_4W_RJIG			32


// �ϱ� ����  �迵�� ���忡�� confirm ���� ��!!!!
#define MAX_4W_JIGPIN_CNT			25	// RJigPin ��������: 4W 25��		(0~24: 1 ohm ~ 2 Mohm)

#define _2W_OPEN_JIGPIN_START		0	// RJigPin ��������: 2W Open 21��	(0~20: 1 ohm ~ 100 Kohm)
#define _2W_OPEN_JIGPIN_END			(_2W_OPEN_JIGPIN_START + 21 -1)	

// N_OPEN: Short �Ǵ� SelfTest ���̽�. nCalMode 170 �̻�
#define _2W_HV_N_OPEN_JIGPIN_START	16	// RJigPin ��������: 2W Short HV 21��  (16~36: 10 Kohm ~ 200 Mohm) 
#define _2W_HV_N_OPEN_JIGPIN_END	(_2W_HV_N_OPEN_JIGPIN_START + 21 -1)

#define _2W_LV_N_OPEN_JIGPIN_START	0	// RJigPin ��������: 2W Short LV 37��  (0~36: 1 ohm ~ 200 Mohm) 
#define _2W_LV_N_OPEN_JIGPIN_END	(_2W_LV_N_OPEN_JIGPIN_START + 37 -1)


//-------------------------
// Calibration ��� ������

//  Cal > Measure > Test
//      Cal 	: ���� Count��ŭ�� Measure�� ���� �Ϸ�� ����
//      Measure : 1ȸ Calibration ����(Test ������)
//      Test    : open, short(leak), 4W���� Measure�� Test ����

#define MAX_MEAS_COUNT		10
#define	MAX_AD_COUNT	100	

#define	CAL_AD_COUNT			(MAX_AD_COUNT*2)		
#define CAL_AD_BUF_SIZE			CAL_AD_COUNT	

//#define MAX_CAL_DATA		128
#define MAX_CAL_DATA_org	128
#define MAX_CAL_DATA		MAX_2W_RJIG
#define NUM_CAL_RESULT_COL	(22 + 60)		// 22: �⺻,   60: ���� Measure (1~10)

typedef struct sMEAS_DATA
{
						// Col22, Col28, Col34, ... Col76  <= UI ��ĭ
	double	dR;			// Col23, Col29, Col35, ... Col77
	double	dVc;		// Col24, Col30, Col36, ... Col78
	double	dI;			// Col25, Col31, Col37, ... Col79
	double	dAdVc;		// Col26, Col32, Col38, ... Col80
	double	dAdI;		// Col27, Col33, Col39, ... Col81
} sMEAS_DATA;


typedef struct sMEAS_RAW_DATA
{
	UINT	nAdVc;		// 
	UINT	nAdI;		// 
} sMEAS_RAW_DATA;

typedef struct sCAL_DATA
{
	int 	nUse;				// Col1		// 0: NotUse,   1: Use
	int 	nValid;				// Col2	    // 0: Bad(InValid), FALSE,  1: Good(Valid), TRUE
										    
	double 	dOrgR;				// Col7		//  Original R Value   (Real H/W) . sRJIG_DATA �� dOrgR�� ���� ���̾�� �Ѵ�.
	double 	dRefR_Avg;			// Col8
	double 	dRefVcVal;			// Col9		// ���� �ʿ���ٸ� �����.
	double 	dRefIVal;			// Col10	// ���� �ʿ���ٸ� �����.
								// Col11    // ���п� �� �÷�(for UI)
								
	double 	dMeasR_Avg;			// Col12
	double 	dMeasVc_Avg;		// Col16
	double 	dMeasI_Avg;			// Col17
	double 	dMeasAdVc_Avg;		// Col18
	double 	dMeasAdI_Avg;		// Col19

	double 	dMeasR_Max;			// Col20
	double 	dMeasR_MaxDiffRate;	// Col13	// (Max - Avg)/Avg : + ������  .���� +5%���� ũ�� ������ǥ��. TBD
	double 	dMeasR_Min;			// Col21
	double 	dMeasR_MinDiffRate; // Col14	// (Min - Avg)/Avg : - ������  .���� -5%���� ������ ������ǥ��. TBD



	sMEAS_DATA		sa_MeasData[MAX_MEAS_COUNT];
#ifdef  __AD_RAW_DATA__
	//2018.03.06 ������ ����� ũ�Ƿ� �� �ʿ��� ��쿡�� debug �ɼ��� ON �Ͽ� ����Ѵ�.
	sMEAS_RAW_DATA	sa_MeasRawData[MAX_MEAS_COUNT][CAL_AD_BUF_SIZE];		// raw data  (display ���� �ʰ� ��¸� �Ѵ�.)
#endif

} sCAL_DATA;


//----------------
// System ����
//  : ������ ACE400 ������ H/W�� ���� Ư���� �����ϴ� ��.   
//    ���������� �����ϱ� ���� �ʱ�ȭ ��. 
//    ��ǰ�� ��� ���� ��ü ���� ��Ʋ�� ���� Bioptro�� H/W ���� ������. 
//    ex) BlockType, RJigData �ɼ��� ������


// SysInfo  enum
enum SYSTEM_TYPE	{ 
	QD =0, 			// QD: Quad Density
	DD =1, 			// DD: Double Density
	SD =2,			// SD: Single Density
	S100_1 = 3,
	S100_2 = 4,
	YAMAHA = 5,
};

enum BOX_TYPE		{ TOP, BTM, MAX_BOX };


enum RANGE_4W		{ R_171, R_172, R_173, R_174, R_175, MAX_R_4W, };

class CSystem  
{
public:
	//for "C:\\ACE400\\SETUP\\SysSet01.ini" ���� ����. CSystem::Load_SysInfoData()
	int 			m_nBlockNumber[MAX_BOX];	// idx(0=Top, 1=Bottom)  val(m_nBlockType�� block ����)
	int 			m_nBlockType[MAX_BOX];		// idx(0=Top, 1=Bottom)  val(QD=0, DD=1, SD=2, S100_1=3, S100_2=4)

	int 			m_nSystemType;				// 0:QD, 1:DD, 2:SD, 3:S100_1, 4:S100_2

	int				m_nRJigFileNum;				// sRJIG_DATA�� s100���� ���(1), �ƴѰ�� (0)
												// �ϱ� m_saRJigData_2W[2], m_saRJigData_4W[2] �� index�� ���
	int				m_na4W_Range[MAX_R_4W];		// idx(0:R_171, 1:R_172, 2:R_173, 3:R_174, 4:R_175)												
												// ACE400�� dRec4w[][]�� �ش�. 4W ���װ��� �� ��庰 ���� ���� range
								// 171   m_na4W_Range[1]  <= SysSet21_4W.ini  ù��° �׸�   ex) 80    1~80     mohm�� 171
								// 172   m_na4W_Range[2]  <= SysSet21_4W.ini  �ι�° �׸�   ex) 800   81~800   mohm�� 172
								// 173   m_na4W_Range[3]  <= SysSet21_4W.ini  ����° �׸�   ex) 5000  801~5000 mohm�� 173
								// 174   m_na4W_Range[4]  <=      ex) 50000   5001 ~ 50000   mohm�� 174
								// 175   m_na4W_Range[5]  <=      ex) 500000  50001 ~ 500000 mohm�� 175

	//for "C:\\ACE400\\BDL\\QC\\RJigReadType.TXT", C:\\ACE400\\BDL\\QC\\RJigReadType_S100.TXT"
	sRJIG_DATA		m_saRJigData_2W[2][MAX_2W_RJIG];	
	sRJIG_DATA		m_saRJigData_4W[2][MAX_4W_RJIG];	

	//for "C:\\ACE400\\SETUP\\CAL",  CSystem::Load_PrevCalData(), 
	sCAL_DATA  		m_saCalData[MAX_TEST_MODE][MAX_CAL_DATA];
		// sRJIG_DATA�� sCAL_DATA �� ���� index�� ���.  
		// �ٵ� ACE400���� �ϳ��� max 96(MAX_2W_RJIG)�̰� �ϳ��� 128(MAX_CAL_DATA)��. �� �׷���????
		
	double			m_dAdVb[MAX_TEST_MODE];		// ���� ADC VB
	double			m_dVb[MAX_TEST_MODE];		// ���� VB 			, Col15
	double  		m_dCalTime[5];				// �ʴ���. m_dCalTime[0]: Setup Time, m_dCalTime[1]: R�� ���� Time
public: 
	CSystem() 
	{
		::ZeroMemory(m_nBlockNumber, sizeof(m_nBlockNumber));
		::ZeroMemory(m_nBlockType, sizeof(m_nBlockType));
		m_nRJigFileNum = -1;
		m_nSystemType = -1;

		::ZeroMemory(m_saRJigData_2W, sizeof(m_saRJigData_2W));
		::ZeroMemory(m_saRJigData_4W, sizeof(m_saRJigData_4W));
		::ZeroMemory(m_saCalData, sizeof(m_saCalData));

		::FillMemory(m_dAdVb, sizeof(m_dAdVb), -1);
		::FillMemory(m_dVb, sizeof(m_dVb), -1);
		::ZeroMemory(m_dCalTime, sizeof(m_dCalTime));
	}

	virtual ~CSystem();
	void	InitMember();
	void 	InitCalData();			// System���� Cal ���� data�� �ʱ�ȭ
	BOOL 	InitCal_RJigData(int nCalMode, int nRJigId);	// Ư�� CalMode, RJig�� data�� �ʱ�ȭ
	BOOL 	Check_TestCondtionRange(int nCol,  int nCheckVal);
	BOOL 	Load_TestCondtionData();
	BOOL 	Save_TestCondtionData();
	BOOL    Load_4W_RangeData();
	BOOL    Load_SysInfoData();
//	BOOL	Load_2WRJigData();
//	BOOL	Load_4WRJigData();
	BOOL	Load_RJigData();
	BOOL 	Load_PrevCalData(int nCalMode);
	BOOL 	Save_CalData(int nCalMode);
	BOOL 	Save_LogCalData(int nCalMode, int nMeasCount);
	BOOL 	IsSelfTestCase(int nCalMode);
	BOOL 	IsShortTestCase(int nCalMode);
	BOOL 	Is4WTestCase(int nCalMode);
	BOOL 	Is2WOpen_TestCase(int nCalMode);
	void 	Get_RJigPinRange(int nCalMode, int& rnStart, int& rnEnd);
	int		Get_nAdCount(int nCalMode);


};



// m_saTestCondition[MAX_TEST_MODE]���� CSystem�� ����� �ְ� �;�����
// ��ü array�� ����ÿ� �뷮���� �ʱ�ȭ �ϱ� ������� ������ ���� �������� ������
extern sTEST_CONDITION  g_saTestCondition[MAX_TEST_MODE];
extern sTEST_CONDITION_RANGE g_saTestConditionRange[NUM_CAL_CONDITION_COL];
extern CSystem g_sSystem;




#endif // !defined(_SYSTEM_H_INCLUDED_)
