// MicroMotion_ICP.h: interface for the CMicroMotion_ICP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICROMOTION_ICP_H__BABAC453_C627_42E1_993B_D6AF7B1B15BA__INCLUDED_)
#define AFX_MICROMOTION_ICP_H__BABAC453_C627_42E1_993B_D6AF7B1B15BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UniDAQ.h"//sylee170229
#include "PCI_D64H.h" //sylee170229 ㅣ

#define	BD_D96		0
#define	BD_D64_0	1		
#define	BD_D64_1	2		

class CMicroMotion_ICP_D96 : public CMicroMotion 
{
public: 
	//HANDLE	m_hDIODrv[CONTEC_BOARD_NUM];	// D96은 보드가 한개 뿐이고 id가 0으로 고정임.

	// 2018.2.12 일 추가.   
	// Ixud_WriteDO() 수행 이전에  Ixud_SoftwareReadbackDO() 호출해서 기존 값을 최대한 유지시키는 방식이
	// 두번 라이브러리를 호출하므로 HW 처리 시간이 2배 이상 걸리게 되는 문제 수정이 필요. 
	// Ixud_SoftwareReadbackDO 하는 대신에 Ixud_WriteDO 하고 나서 바로 해당 값을 baPrevPortVal[port]에 보존하고 
	// 다음번 Ixud_WriteDO 에서 참고하여 write하도록 한다.   
	// 유의사항: MBIT_TRS, BYTE_TRS, BIT_TRS시에 반드시 baPrevPortVal[port]이 업데이트되어야 한다. 
	BYTE	baPrevPortVal[D96_TOTAL_OUTPUT_PORT_NUM];	// 12

public:
	CMicroMotion_ICP_D96();
	virtual ~CMicroMotion_ICP_D96();

	BOOL	InitMotion();
	BOOL	CloseMotion();	
	void	myIxudGetErrString(WORD wRet, char* errStr);
/*
	BOOL	On(WORD wBitNo);
	BOOL	Off(WORD wBitNo);
	BOOL	IsOn(WORD wBitNo);
	BOOL	IsOff(WORD wBitNo);
*/
	BOOL	TranceiveCmd(UINT order, psCMD psCmd);

};

class CMicroMotion_ICP_D64 : public CMicroMotion 
{
public: 
	int		m_nBdId;
	BYTE	m_bBdDrv;

	// 2018.2.1 일 추가.   
	// d64h_do_writeport 수행 이전에  d64h_do_readport 해서 기존 값을 최대한 유지시키는 방식이
	// 두번 라이브러리를 호출하므로 HW 처리 시간이 2배 이상 걸리게 되는 구조적인 문제 수정이 필요. 
	// do_readport 하는 대신에 d64h_do_writeport 하고 나서 바로 해당 값을 dwPrevVal에 보존하고 
	// 다음번 d64h_do_writeport 에서 참고하여 write하도록 한다.  
	DWORD   dwPrevVal;		

public:
	CMicroMotion_ICP_D64();
	virtual ~CMicroMotion_ICP_D64();

	BOOL	InitMotion(int nBdId);
	BOOL	CloseMotion(int nBdId);	
	void    myD64hGetErrString(short wErrCode, char* errStr);
/*
	BOOL	On(WORD wBitNo);
	BOOL	Off(WORD wBitNo);
	BOOL	IsOn(WORD wBitNo);
	BOOL	IsOff(WORD wBitNo);
*/
	BOOL	TranceiveCmd(UINT order, psCMD psCmd, int nD64Id);

};


// MicoroMotion_ICP.h는 3장의 Switch카드로 구동되는 구조를 위해 3종류의  Class를 가진다. 
// 1. MicroMotion_ICP : D96 1개와  D64 2개의  Switch카드를 가지는 MicroMotion 인터페이스 Class
// 2. MicroMotion_D96 : D96 카드 관련 기능 Class    : Output(Write) 전용
// 3. MicroMotion_D64 : D64 카드 관련 기능 Class	: 0번은 Input/Output 겸용,  1번은 Output 전용

#define	PCI_D64_NUM		2
class CMicroMotion_ICP  : public CMicroMotion
{
public: 
	CMicroMotion_ICP_D96	m_D96;
	CMicroMotion_ICP_D64	m_D64[PCI_D64_NUM];		// 2


public:

	CMicroMotion_ICP();
	virtual ~CMicroMotion_ICP();

	BOOL	InitMotion();
	BOOL	CloseMotion();	
/*
	BOOL	On(WORD wBitNo);
	BOOL	Off(WORD wBitNo);
	BOOL	IsOn(WORD wBitNo);
	BOOL	IsOff(WORD wBitNo);
*/
	BOOL	TranceiveSeq(psSEQ psSeq);
};

extern CMicroMotion_ICP		g_MicroM_ICP;

#endif // !defined(AFX_MICROMOTION_ICP_H__BABAC453_C627_42E1_993B_D6AF7B1B15BA__INCLUDED_)
