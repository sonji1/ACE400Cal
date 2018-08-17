// Error.cpp: implementation of the CError class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "ACE400Cal.h"
#include "Error.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CError ERR;


CError::CError()
{

}

CError::~CError()
{

}

void CError::Set(int error_no, CString strState)
{	
	m_error_no = error_no;

	m_strState = strState;

#ifndef __PRT_OFF__
	// optional print (to be deleted..?)
	
	//TRACE("ERR.Set: err=%d %s\n", error_no, m_strState);
	//TRACE("Err.Set: err=%d\n", error_no);
#endif

	switch(error_no)
	{
	case NO_ERR:
		break;

	default:
		break;
	}

}

void CError::Reset()
{ 
	m_error_no = NO_ERR;
}
	
BOOL CError::GetMsg(int error_no, CString &title, CString &state, CString &sol)
{
	//TRACE("ERR.GetMsg(err=%d)", error_no);		// to be deleted..

	for (int i=0; i<ERRMSG_TABLE_LEN; i++) {
		if (ErrMsgTable[i].error_no == error_no) {
			title = ErrMsgTable[i].title;
			state = ErrMsgTable[i].state;
			sol = ErrMsgTable[i].sol;
			return TRUE;
		}
	}
    return FALSE;
}
