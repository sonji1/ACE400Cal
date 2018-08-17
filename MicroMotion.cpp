// MicroMotion.cpp: implementation of the CMicroMotion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ACE400Cal.h"
#include "MicroMotion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

char	saStrModule[MODULE_END][20] = {
	"LV1", 
	"LV2", 
	"HV1", 
	"HV2", 
	"PULSE", 
	"VB_OUT", 
	"SPARK", 
	"V", 
	"I", 
	"VI", 
	"PFFT", 
	"PSFT", 
	"VBFT", 
	"IFFT", 
	"ISFT", 
	"CAV", 
	"LVB", 
	"HVB", 
	"LVC", 
	"HVC", 
	"HVO", 
	"FW_GAIN", 
	"VCFFT", 
	"VCSFT", 
	"FWFT", 
	"HVOFT", 
	"BBOX", 
	"CBOX", 
	"BPOINT", 
	"CPOINT", 
	"I_GAIN", 
	"CC_CV"
};



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMicroMotion::CMicroMotion()
{

}

CMicroMotion::~CMicroMotion()
{

}

void CMicroMotion::GetModuleString(UINT module_id, char* strModule)
{
	if (module_id >= MODULE_END)
	{
		strcpy(strModule, "Undefined strModule!");
		return;
	}
		
	strcpy(strModule, saStrModule[module_id]);

}
