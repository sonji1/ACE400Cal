// MacroMotion.h: interface for the CMacroMotion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MACROMOTION_H__EA8A1C4F_12A4_4D1E_9CA4_370F29FA8B41__INCLUDED_)
#define AFX_MACROMOTION_H__EA8A1C4F_12A4_4D1E_9CA4_370F29FA8B41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MicroMotion.h"
#ifdef ICP_DAS_CARD
	// ICP DAS SWTICH CARD
	#include "SeqStruct_ICP.h"
	#include "MicroMotion_ICP.h"
#else			
	// CONTEC SWTICH CARD
	#include "SeqStruct_ContecV43.h"
	#include "MicroMotion_Contec.h"
#endif


class CMacroMotion  
{
public:
	CMicroMotion*	m_pMicroM;


public:
	CMacroMotion();
	virtual ~CMacroMotion();

	DWORD	Set(UINT module_id, int value1=-1, int value2=-1, int value3=-1);
	UINT	Get(UINT typGet, int value1= -1);  // value1= bSkipDelay FALSE ÀÌ¸י delay	
	DWORD	Apply(UINT module_id);
	DWORD	Break(UINT module_id=ALL_OUT);
	DWORD	Rst(UINT module_id, UINT value=0, UINT value2=0);
	void	SetFilter(UINT test_type);
//	BOOL	RstBox(int ptB, int ptC);
	BOOL	RstPointAll(BOOL bVbVc=PT_VB);
};

extern CMacroMotion	MM;

#endif // !defined(AFX_MACROMOTION_H__EA8A1C4F_12A4_4D1E_9CA4_370F29FA8B41__INCLUDED_)
