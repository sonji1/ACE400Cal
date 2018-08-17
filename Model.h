///////////////////////////////////////////////////////////////////////////
// Model.h: interface for the CModel class.
//
// 2017.12.15 written by sonji 
///////////////////////////////////////////////////////////////////////////

#if !defined(_MODEL_H_INCLUDED_)
#define _MODEL_H_INCLUDED_

#define	 MAX_MODEL_PIN		32768
//#define  MAX_SWITCH_PIN		16384
#define  MAX_SWITCH_PIN		32768



//----------------------
// .CON 파일 관련 데이터 

// .FPN이 없다면 .CON 파일도 없다.(내용 동일, "" 만 뺌) 
// 이 경우는 PinBlockNo와 사용자 PinNo가 동일하며 BDL파일의 PinNo와 ReadFile의 PinNo가 같음

typedef struct sPIN_NO
{
	int			nSwitch;	// PinNo for Switch (HW PinBlock)	// PinBlock no
	int			nUserJig;	// PinNo for User    				// jig no,  BDL_PinNo+1, NetPinNo
} sPIN_NO;

typedef struct sPIN_CON
{
	int			nTotalPin;					// .CON 파일의 총 pin 수
	int			nCheckSum;					// .CON 파일의 CheckSum 
	sPIN_NO		saPinNo[MAX_SWITCH_PIN];	// PinNo 리스트
} sPIN_CON;



//-------------------------
// PinSetup enum

enum PINBLOCK_TYPE	{ 
	PIN_QD =1, 			// QD: Quad Density
	PIN_DD =2, 			// DD: Double Density
	PIN_SD =3			// SD: Single Density
};

enum PIN_TOT_RANGE {
	TOT_4K	= 1,
	TOT_8K	= 2,
	TOT_16K	= 3,
	TOT_32K	= 4
};

enum PIN_BOX_RANGE {
	BOX_2K  = 1,
	BOX_4K  = 2
};

enum PIN_SWITCH_MODE {
	MODE_Z = 1,
	MODE_H = 2
};

enum PIN_CARD_REVERSE {
	CARD_NORMAL  = 1,
	CARD_REVERSE = 2
};


#define		NUM_2K	2048
#define		NUM_4K	4096
#define		NUM_8K	(4096 * 2)		// 8192
#define		NUM_16K	(4096 * 4)		// 16384

//-------------------------
// PinSetup 데이터 

// ACE400 Menu->Setup->PIN 화면.
// BDL_PATH+"\\SETUP2\\SysSetPin.ini" 정보와 연관이 있음.
// "C:\\ACE400\\SETUP2\\SysSetPin.ini" 정보와 연관이 있음.
#define 	MAX_BOX		4			// PIN_BLOCK = BOX

typedef struct sPIN_SETUP
{
	int			nPinBoxType[MAX_BOX];		// 1:PIN_QD, 2:PIN_DD, 3:PIN_SD	-> ACE400의 m_nPinQD에 해당
											// nPinBoxType[0]: Upper1, nPinBoxType[1]:Upper2, 
											// nPinBoxType[2]: Lower1, nPinBoxType[3]:Lower2
												
	int			nMaxTotPin;					// 1:TOT_4K,  2:TOT_8K,    3:TOT_16K,   4:TOT_32K
	int			nMaxBoxPin;					// 1:BOX_2K,  2:BOX_4K	
	int			nPinSwitchMode;				// 1:MODE_Z,  2:MODE_H 
	int			nPinType;					// 1:A,  2:B,   3:C,    4:D,   5:E
	int			nCardA_Reverse;				// 1:CARD_NORMAL,  2:CARD_REVERSE
	int			nCardB_Reverse;				// 1:CARD_NORMAL,  2:CARD_REVERSE

	int			n32k_16k;					// 
	int			nLowerJigStartNo1;			// 

} sPIN_SETUP;



// - Block > Piece  > Pin
// - Block 
//   - Jig가 한번에 테스트할 수 있는 piece의 묶음.  
//   - Bioptro 계측기에서 한번에 테스트할 수 있는 block은 max 4개  (upper1,2  lower 3, 4)
// - Piece 
//   : chip 단위의 pin 집합  , 나중에 하나의 칩으로 가공되는 부분. 
// - 아래의 경우  Block은 4개   Block당 piece는 6개.
// ------------------------------
// O O | O O | O O | O O 
// O O | O O | O O | O O 
// O O | O O | O O | O O 
// -------------------------------


//------------
// Model 정보 
//   : 사용업체마다 다른 데이터로 사용자에게 Open되는 데이터
//     NetData, Recipe, 부품정보, PCB 데이터, 검사조건 등등
//     제품별로 업체에서 PCB를 검사하며, PCB의 모양에 따라 Jig를 만들고 
//     그를 검사할 수 있는 Net을 구성하여 모델화한다.

class CModel  
{
public:
	BOOL		m_bIsConExist;	// TRUE이면 .CON 파일이 존재. ACE400 OnPinConvertFlag1에 해당.
	sPIN_CON	m_sPinCon;		// .CON 파일 데이터를 저장

	sPIN_SETUP  m_sPinSetup;	// ACE400 Menu->Setup->PIN 정보를 저장.
								// BDL_PATH+"\\SETUP2\\SysSetPin.ini" 정보 -> QD 정보
								// "C:\\ACE400\\SETUP2\\SysSetPin.ini" 정보 -> QD 이외 정보
								

	int			nPinMap_NET[MAX_SWITCH_PIN];		// ACE400 PinMap[0]
													// NET pin No		    idx도 content도 jigPinNo-1 	

	int			nPinMap_PBLK[MAX_SWITCH_PIN];		// ACE400 PinMap[1]
													// Pin Block No,  idx는 .CON파일 순서, content는 pinBlockNo-1
													// SwitchMode, CardReverse 처리 이전  Pin Block No 
													
	int			nPinMap_SWT[MAX_SWITCH_PIN];		// ACE400 PinMap[2]
													// SWITCH CARD pin no,  idx는  jigPinNo-1
													// SwitchMode, CardReverse 처리 이후  Pin Block No

public: 
	CModel() 
	{
		m_bIsConExist = FALSE;		
		::ZeroMemory(&m_sPinCon, sizeof(m_sPinCon));
		::ZeroMemory(&m_sPinSetup, sizeof(m_sPinSetup));

		::FillMemory(nPinMap_NET, sizeof(nPinMap_NET), -1);
		::FillMemory(nPinMap_PBLK, sizeof(nPinMap_PBLK), -1);
		::FillMemory(nPinMap_SWT, sizeof(nPinMap_SWT), -1);
	}

	virtual ~CModel();
	void	InitMember();

	BOOL    Load_PinConFile();		// BDL파일위치에 .CON 파일이 존재한다면(m_bIsConExist =>TRUE) Load한다.
	BOOL	Load_PinSetupData();
	BOOL	PinSetupConvert();

};

extern CModel g_sModel;


#endif	// !defined(_MODEL_H_INCLUDED_)

