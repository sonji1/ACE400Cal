// MicroMotion_Contec.h: interface for the CMicroMotion_Contec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICROMOTION_CONTEC_H__BABAC453_C627_42E1_993B_D6AF7B1B15BA__INCLUDED_)
#define AFX_MICROMOTION_CONTEC_H__BABAC453_C627_42E1_993B_D6AF7B1B15BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MicroMotion.h"		// Base Class


class CMicroMotion_Contec : public CMicroMotion 
{
public:
	HANDLE	m_hDIODrv[CONTEC_BOARD_NUM];

public:
	CMicroMotion_Contec();
	virtual ~CMicroMotion_Contec();

	BOOL	InitMotion();
	BOOL	CloseMotion();	//son
	BOOL	On(WORD wBitNo);
	BOOL	Off(WORD wBitNo);
	BOOL	IsOn(WORD wBitNo);
	BOOL	IsOff(WORD wBitNo);
	BOOL	TranceiveSeq(psSEQ psSeq);

};

extern CMicroMotion_Contec		g_MicroM_Contec;



#endif // !defined(AFX_MICROMOTION_CONTEC_H__BABAC453_C627_42E1_993B_D6AF7B1B15BA__INCLUDED_)
