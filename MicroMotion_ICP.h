// MicroMotion_ICP.h: interface for the CMicroMotion_ICP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICROMOTION_ICP_H__BABAC453_C627_42E1_993B_D6AF7B1B15BA__INCLUDED_)
#define AFX_MICROMOTION_ICP_H__BABAC453_C627_42E1_993B_D6AF7B1B15BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UniDAQ.h"//sylee170229
#include "PCI_D64H.h" //sylee170229 ��

#define	BD_D96		0
#define	BD_D64_0	1		
#define	BD_D64_1	2		

class CMicroMotion_ICP_D96 : public CMicroMotion 
{
public: 
	//HANDLE	m_hDIODrv[CONTEC_BOARD_NUM];	// D96�� ���尡 �Ѱ� ���̰� id�� 0���� ������.

	// 2018.2.12 �� �߰�.   
	// Ixud_WriteDO() ���� ������  Ixud_SoftwareReadbackDO() ȣ���ؼ� ���� ���� �ִ��� ������Ű�� �����
	// �ι� ���̺귯���� ȣ���ϹǷ� HW ó�� �ð��� 2�� �̻� �ɸ��� �Ǵ� ���� ������ �ʿ�. 
	// Ixud_SoftwareReadbackDO �ϴ� ��ſ� Ixud_WriteDO �ϰ� ���� �ٷ� �ش� ���� baPrevPortVal[port]�� �����ϰ� 
	// ������ Ixud_WriteDO ���� �����Ͽ� write�ϵ��� �Ѵ�.   
	// ���ǻ���: MBIT_TRS, BYTE_TRS, BIT_TRS�ÿ� �ݵ�� baPrevPortVal[port]�� ������Ʈ�Ǿ�� �Ѵ�. 
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

	// 2018.2.1 �� �߰�.   
	// d64h_do_writeport ���� ������  d64h_do_readport �ؼ� ���� ���� �ִ��� ������Ű�� �����
	// �ι� ���̺귯���� ȣ���ϹǷ� HW ó�� �ð��� 2�� �̻� �ɸ��� �Ǵ� �������� ���� ������ �ʿ�. 
	// do_readport �ϴ� ��ſ� d64h_do_writeport �ϰ� ���� �ٷ� �ش� ���� dwPrevVal�� �����ϰ� 
	// ������ d64h_do_writeport ���� �����Ͽ� write�ϵ��� �Ѵ�.  
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


// MicoroMotion_ICP.h�� 3���� Switchī��� �����Ǵ� ������ ���� 3������  Class�� ������. 
// 1. MicroMotion_ICP : D96 1����  D64 2����  Switchī�带 ������ MicroMotion �������̽� Class
// 2. MicroMotion_D96 : D96 ī�� ���� ��� Class    : Output(Write) ����
// 3. MicroMotion_D64 : D64 ī�� ���� ��� Class	: 0���� Input/Output ���,  1���� Output ����

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
