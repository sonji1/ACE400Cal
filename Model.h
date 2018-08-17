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
// .CON ���� ���� ������ 

// .FPN�� ���ٸ� .CON ���ϵ� ����.(���� ����, "" �� ��) 
// �� ���� PinBlockNo�� ����� PinNo�� �����ϸ� BDL������ PinNo�� ReadFile�� PinNo�� ����

typedef struct sPIN_NO
{
	int			nSwitch;	// PinNo for Switch (HW PinBlock)	// PinBlock no
	int			nUserJig;	// PinNo for User    				// jig no,  BDL_PinNo+1, NetPinNo
} sPIN_NO;

typedef struct sPIN_CON
{
	int			nTotalPin;					// .CON ������ �� pin ��
	int			nCheckSum;					// .CON ������ CheckSum 
	sPIN_NO		saPinNo[MAX_SWITCH_PIN];	// PinNo ����Ʈ
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
// PinSetup ������ 

// ACE400 Menu->Setup->PIN ȭ��.
// BDL_PATH+"\\SETUP2\\SysSetPin.ini" ������ ������ ����.
// "C:\\ACE400\\SETUP2\\SysSetPin.ini" ������ ������ ����.
#define 	MAX_BOX		4			// PIN_BLOCK = BOX

typedef struct sPIN_SETUP
{
	int			nPinBoxType[MAX_BOX];		// 1:PIN_QD, 2:PIN_DD, 3:PIN_SD	-> ACE400�� m_nPinQD�� �ش�
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
//   - Jig�� �ѹ��� �׽�Ʈ�� �� �ִ� piece�� ����.  
//   - Bioptro �����⿡�� �ѹ��� �׽�Ʈ�� �� �ִ� block�� max 4��  (upper1,2  lower 3, 4)
// - Piece 
//   : chip ������ pin ����  , ���߿� �ϳ��� Ĩ���� �����Ǵ� �κ�. 
// - �Ʒ��� ���  Block�� 4��   Block�� piece�� 6��.
// ------------------------------
// O O | O O | O O | O O 
// O O | O O | O O | O O 
// O O | O O | O O | O O 
// -------------------------------


//------------
// Model ���� 
//   : ����ü���� �ٸ� �����ͷ� ����ڿ��� Open�Ǵ� ������
//     NetData, Recipe, ��ǰ����, PCB ������, �˻����� ���
//     ��ǰ���� ��ü���� PCB�� �˻��ϸ�, PCB�� ��翡 ���� Jig�� ����� 
//     �׸� �˻��� �� �ִ� Net�� �����Ͽ� ��ȭ�Ѵ�.

class CModel  
{
public:
	BOOL		m_bIsConExist;	// TRUE�̸� .CON ������ ����. ACE400 OnPinConvertFlag1�� �ش�.
	sPIN_CON	m_sPinCon;		// .CON ���� �����͸� ����

	sPIN_SETUP  m_sPinSetup;	// ACE400 Menu->Setup->PIN ������ ����.
								// BDL_PATH+"\\SETUP2\\SysSetPin.ini" ���� -> QD ����
								// "C:\\ACE400\\SETUP2\\SysSetPin.ini" ���� -> QD �̿� ����
								

	int			nPinMap_NET[MAX_SWITCH_PIN];		// ACE400 PinMap[0]
													// NET pin No		    idx�� content�� jigPinNo-1 	

	int			nPinMap_PBLK[MAX_SWITCH_PIN];		// ACE400 PinMap[1]
													// Pin Block No,  idx�� .CON���� ����, content�� pinBlockNo-1
													// SwitchMode, CardReverse ó�� ����  Pin Block No 
													
	int			nPinMap_SWT[MAX_SWITCH_PIN];		// ACE400 PinMap[2]
													// SWITCH CARD pin no,  idx��  jigPinNo-1
													// SwitchMode, CardReverse ó�� ����  Pin Block No

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

	BOOL    Load_PinConFile();		// BDL������ġ�� .CON ������ �����Ѵٸ�(m_bIsConExist =>TRUE) Load�Ѵ�.
	BOOL	Load_PinSetupData();
	BOOL	PinSetupConvert();

};

extern CModel g_sModel;


#endif	// !defined(_MODEL_H_INCLUDED_)

