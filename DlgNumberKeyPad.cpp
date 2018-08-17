// DlgNumberKeyPad.cpp : implementation file
//

#include "stdafx.h"
#include "ACE400Cal.h"
#include "DlgNumberKeyPad.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNumberKeyPad dialog

#define DEFAULT_BACK_COLOR	0xFFFFFF
#define CURSOR_BACK_COLOR	0x0000FFFF

CDlgNumberKeyPad NKEYPAD;


CDlgNumberKeyPad::CDlgNumberKeyPad(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNumberKeyPad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNumberKeyPad)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nNumLength = 0;
}


void CDlgNumberKeyPad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNumberKeyPad)
	DDX_Control(pDX, IDC_SSPANEL_NUM_DATA, m_NumPadData);
	DDX_Control(pDX, IDC_SSPANEL_NUMPOS_0, m_NumPos0);
	DDX_Control(pDX, IDC_SSPANEL_NUMPOS_1, m_NumPos1);
	DDX_Control(pDX, IDC_SSPANEL_NUMPOS_10, m_NumPos10);
	DDX_Control(pDX, IDC_SSPANEL_NUMPOS_11, m_NumPos11);
	DDX_Control(pDX, IDC_SSPANEL_NUMPOS_2, m_NumPos2);
	DDX_Control(pDX, IDC_SSPANEL_NUMPOS_3, m_NumPos3);
	DDX_Control(pDX, IDC_SSPANEL_NUMPOS_4, m_NumPos4);
	DDX_Control(pDX, IDC_SSPANEL_NUMPOS_5, m_NumPos5);
	DDX_Control(pDX, IDC_SSPANEL_NUMPOS_6, m_NumPos6);
	DDX_Control(pDX, IDC_SSPANEL_NUMPOS_7, m_NumPos7);
	DDX_Control(pDX, IDC_SSPANEL_NUMPOS_8, m_NumPos8);
	DDX_Control(pDX, IDC_SSPANEL_NUMPOS_9, m_NumPos9);
	DDX_Control(pDX, IDC_SSPANEL_MAX_VALUE, m_sMaxLimit);
	DDX_Control(pDX, IDC_SSPANEL_MIN_VALUE, m_sMinLimit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNumberKeyPad, CDialog)
	//{{AFX_MSG_MAP(CDlgNumberKeyPad)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_0, OnButtonKeypad0)
	ON_BN_CLICKED(IDC_BUTTON_BACKSPACE, OnButtonBackspace)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_CLR, OnButtonClr)
	ON_BN_CLICKED(IDC_BUTTON_CURSOR_BACK, OnButtonCursorBack)
	ON_BN_CLICKED(IDC_BUTTON_CURSOR_FWD, OnButtonCursorFwd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DOT, OnButtonDot)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_1, OnButtonKeypad1)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_2, OnButtonKeypad2)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_3, OnButtonKeypad3)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_4, OnButtonKeypad4)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_5, OnButtonKeypad5)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_6, OnButtonKeypad6)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_7, OnButtonKeypad7)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_8, OnButtonKeypad8)
	ON_BN_CLICKED(IDC_BUTTON_KEYPAD_9, OnButtonKeypad9)	
	ON_BN_CLICKED(IDC_BUTTON_SIGN, OnButtonSign)
	ON_BN_CLICKED(IDC_BUTTON_ENTER, OnButtonEnter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNumberKeyPad message handlers

BOOL CDlgNumberKeyPad::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN){
		switch(pMsg->wParam){
			case VK_RETURN:
				OnButtonEnter();
				break;
			case VK_ESCAPE:
				OnButtonCancel();
				break;
			case VK_NUMPAD0:
			case 0x30 :
				OnButtonKeypad0();
				break;
			case VK_NUMPAD1:
			case 0x31 :
				OnButtonKeypad1();
				break;
			case VK_NUMPAD2:
			case 0x32 :
				OnButtonKeypad2();
				break;
			case VK_NUMPAD3:
			case 0x33 :
				OnButtonKeypad3();
				break;
			case VK_NUMPAD4:
			case 0x34 :
				OnButtonKeypad4();
				break;
			case VK_NUMPAD5:
			case 0x35 :
				OnButtonKeypad5();
				break;
			case VK_NUMPAD6:
			case 0x36 :
				OnButtonKeypad6();
				break;
			case VK_NUMPAD7:
			case 0x37 :
				OnButtonKeypad7();
				break;
			case VK_NUMPAD8:
			case 0x38 :
				OnButtonKeypad8();
				break;
			case VK_NUMPAD9:
			case 0x39 :
				OnButtonKeypad9();
				break;
			case VK_LEFT:
				OnButtonCursorBack();
				break;
			case VK_RIGHT:
				OnButtonCursorFwd();
				break;
			case VK_DELETE:
				OnButtonDel();
				break;
			case VK_BACK:
				OnButtonBackspace();
				break;
			case 0xBE:
			case VK_DECIMAL:
				OnButtonDot();
				break;
			case 0xBB:
			case 0xBD:
				OnButtonSign();
				break;
		}
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgNumberKeyPad::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nCursorPos = 1;
	DisplayNum();
	SetCursorColor(m_nCursorPos, CURSOR_BACK_COLOR);
	
//	Delay(100);

	if(m_bInteger) GetDlgItem(IDC_BUTTON_DOT)->EnableWindow(FALSE);

	m_bFirst = TRUE;
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNumberKeyPad::OnButtonBackspace() 
{
	BackspaceBit();
}

void CDlgNumberKeyPad::OnButtonCancel() 
{
	m_dCalValue = m_dBackupValue;
	m_bInteger = FALSE;

	CDialog::OnCancel();	
}

void CDlgNumberKeyPad::OnButtonClr() 
{
	int i;
	CString strBuffer;

	//Data를 초기화 한다.
	m_dCalValue = 0;

	for(i=0; i<12; i++)	m_strPanelBit[i].Format("");

	strBuffer.Format("%d", 0);
	m_nNumLength = strBuffer.GetLength();
	m_strPanelBit[1].Format("%c", strBuffer.GetAt(m_nNumLength-1));

	m_nCursorPos = 1;
	DisplayNum();
	for(i=0; i<12; i++) SetCursorColor(i, DEFAULT_BACK_COLOR);
	SetCursorColor(m_nCursorPos, CURSOR_BACK_COLOR);		
}

void CDlgNumberKeyPad::OnButtonCursorBack() 
{
	if(m_nCursorPos < m_nNumLength)
	{
		SetCursorColor(m_nCursorPos, DEFAULT_BACK_COLOR);
		m_nCursorPos++;
		SetCursorColor(m_nCursorPos, CURSOR_BACK_COLOR);
	}	
}

void CDlgNumberKeyPad::OnButtonCursorFwd() 
{
	if(m_nCursorPos >= 1)
	{
		SetCursorColor(m_nCursorPos, DEFAULT_BACK_COLOR);
		m_nCursorPos--;
		SetCursorColor(m_nCursorPos, CURSOR_BACK_COLOR);
	}	
}

void CDlgNumberKeyPad::OnButtonDel() 
{
	DeleteBit();	
}

void CDlgNumberKeyPad::OnButtonDot() 
{
	if(m_bInteger == TRUE) return;
	if(m_bFirst == TRUE) OnButtonClr();
	m_bFirst = FALSE;
	
	CString strBit;
	BOOL bFind = FALSE;
	strBit.Format(".");
	// 앞쪽에 .이 있으면 수행하지 않는다.
	for(int i=m_nNumLength; i>m_nCursorPos; i--)
	{
		if(m_strPanelBit[i] == strBit)
		{
			bFind = TRUE;
			break;
		}
	}
	if(bFind == FALSE)
		AddBit(strBit);		
}

void CDlgNumberKeyPad::OnButtonKeypad0() 
{
	CString strBit;
	strBit.Format("0");
	AddBit(strBit);	
}

void CDlgNumberKeyPad::OnButtonKeypad1() 
{
	CString strBit;
	strBit.Format("1");
	AddBit(strBit);	
}

void CDlgNumberKeyPad::OnButtonKeypad2() 
{
	CString strBit;
	strBit.Format("2");
	AddBit(strBit);	
}

void CDlgNumberKeyPad::OnButtonKeypad3() 
{
	CString strBit;
	strBit.Format("3");
	AddBit(strBit);	
}

void CDlgNumberKeyPad::OnButtonKeypad4() 
{
	CString strBit;
	strBit.Format("4");
	AddBit(strBit);	
}

void CDlgNumberKeyPad::OnButtonKeypad5() 
{
	CString strBit;
	strBit.Format("5");
	AddBit(strBit);	
}

void CDlgNumberKeyPad::OnButtonKeypad6() 
{
	CString strBit;
	strBit.Format("6");
	AddBit(strBit);	
}

void CDlgNumberKeyPad::OnButtonKeypad7() 
{
	CString strBit;
	strBit.Format("7");
	AddBit(strBit);	
}

void CDlgNumberKeyPad::OnButtonKeypad8() 
{
	CString strBit;
	strBit.Format("8");
	AddBit(strBit);	
}

void CDlgNumberKeyPad::OnButtonKeypad9() 
{
	CString strBit;
	strBit.Format("9");
	AddBit(strBit);	
}

void CDlgNumberKeyPad::OnButtonEnter() 
{
	//**************************************
	// 화면 Data를 숫자로 변환시킨다.
	//**************************************
	int nSign = 1;
	double dMultiple = 10.0;
	double dValue;

	dValue = 0.0;
	if(m_strPanelBit[11] == "-") nSign = -1;
	else nSign = 1;

	CString strValue("");
	for(int i=0; i<m_nNumLength; i++) strValue += m_strPanelBit[m_nNumLength-i].GetAt(0);

	char *endptr;
	dValue = strtod(strValue, &endptr);

	// 부호를 계산한다.
	dValue *= nSign;

	if(dValue > m_dMaxLimitValue){
//		if(g_sSysData.iLanguage == KOREAN)			DISPMSG("입력 최대 범위 벗어남.");
//		else if(g_sSysData.iLanguage == ENGLISH)	DISPMSG("The maximum input range over.");
		return;
	}
	else if(dValue < m_dMinLimitValue){
//		if(g_sSysData.iLanguage == KOREAN)			DISPMSG("입력 최소 범위 벗어남.");
//		else if(g_sSysData.iLanguage == ENGLISH)	DISPMSG("The minimum input range over.");
		return;
	}

	m_dCalValue = dValue;
	m_bInteger = FALSE;

	CDialog::OnOK();		
}

void CDlgNumberKeyPad::OnButtonSign() 
{
	if(m_bFirst == TRUE) OnButtonClr();
	m_bFirst = FALSE;

	if(strcmp(m_strPanelBit[11], "-") == 0)
		m_strPanelBit[11] = "+";
	else
		m_strPanelBit[11] = "-";
	DisplayNum();		
}

BOOL CDlgNumberKeyPad::SetValue(double dValue, double dMinLimitValue, double dMaxLimitValue)
{
	int i, iIndex(-1), iPreIndex(0);
	CString strBuffer;

	if(dValue >= 100000000. && dValue <= -100000000.) dValue = 0;
	if((dMaxLimitValue < -100000000 || dMaxLimitValue < dMinLimitValue) || 
		(dMinLimitValue >  100000000 || dMinLimitValue > dMaxLimitValue)){ 	
//		if(g_sSysData.iLanguage == KOREAN)			DISPMSG("최대/최소 값이 적절하지 않습니다.");
//		else if(g_sSysData.iLanguage == ENGLISH)	DISPMSG("Unproper min/max input value.");
		return FALSE; 
	}
	m_bInteger = FALSE;
	m_dMaxLimitValue = dMaxLimitValue;
	m_dMinLimitValue = dMinLimitValue;

	// 초기화
	if(dValue < dMinLimitValue || dValue > dMaxLimitValue) dValue = 0;
	m_dCalValue = dValue;
	m_dBackupValue = dValue;
	for(i=0; i<12; i++) m_strPanelBit[i].Format("");

	// 부호를 얻어낸다.
	if(dValue < 0.0)
	{
		dValue = fabs(dValue);
		m_strPanelBit[11] = "-";
	}

	strBuffer.Format("%lf", dValue);
	m_nNumLength = strBuffer.GetLength();

	// 마지막 유효 숫자를 찾는다.
	for(i=m_nNumLength; i>0; i--)
	{
		// 우->좌로 0 숫자를 찾아낸다.
		iIndex = strBuffer.Find("0", i);
		if(iIndex == iPreIndex) 
		{
			// 0이 아닌 유효한 자리가 나온 경우

			// .(dot) 인지 확인한다.
			int ii = strBuffer.Find(".", i);
			if(ii == -1) break;
			else {
				// .(dot) 인 경우 추가 한자리를 삭제한다.
				iIndex -= 1;
				break;
			}
		}
		else
			iPreIndex = iIndex;
	}
		
	strBuffer = strBuffer.Left(iIndex);
	m_nNumLength = strBuffer.GetLength();
	for(i=0; i<m_nNumLength; i++)
	{
		m_strPanelBit[i + 1].Format("%c", strBuffer.GetAt(m_nNumLength - i - 1));
	}

	return TRUE;
}

BOOL CDlgNumberKeyPad::SetValue(int nValue, double dMinLimitValue, double dMaxLimitValue)
{
	int i;
	CString strBuffer;

	if(nValue >= 100000000 && nValue <= -100000000)	nValue = 0;
	if((dMaxLimitValue < -100000000 || dMaxLimitValue < dMinLimitValue) || 
		(dMinLimitValue >  100000000 || dMinLimitValue > dMaxLimitValue)){ 	
//		if(g_sSysData.iLanguage == KOREAN)		DISPMSG("최대/최소 값이 적절하지 않습니다.");
//		else if(g_sSysData.iLanguage == ENGLISH) DISPMSG("Unproper min/max input value.");
		return FALSE; 
	}

	m_bInteger = TRUE;
	m_dMaxLimitValue = dMaxLimitValue;
	m_dMinLimitValue = dMinLimitValue;

	//Data를 초기화 한다.
	m_dCalValue = nValue;
	m_dBackupValue = nValue;

	for(i=0; i<12; i++)
		m_strPanelBit[i].Format("");

	// 부호를 얻어낸다.
	if(nValue == 0)	m_nNumLength = 0;

	if(nValue < 0)
	{
		nValue = abs(nValue);
		m_strPanelBit[11] = "-";
	}

	strBuffer.Format("%d", nValue);
	m_nNumLength = strBuffer.GetLength();
	for(i=0; i<m_nNumLength; i++)
	{
		m_strPanelBit[i + 1].Format("%c", strBuffer.GetAt(m_nNumLength - i - 1));
	}

	return TRUE;
}

void CDlgNumberKeyPad::DisplayNum()
{
	m_NumPos0.SetCaption(m_strPanelBit[0]);
	m_NumPos1.SetCaption(m_strPanelBit[1]);
	m_NumPos2.SetCaption(m_strPanelBit[2]);
	m_NumPos3.SetCaption(m_strPanelBit[3]);
	m_NumPos4.SetCaption(m_strPanelBit[4]);
	m_NumPos5.SetCaption(m_strPanelBit[5]);
	m_NumPos6.SetCaption(m_strPanelBit[6]);
	m_NumPos7.SetCaption(m_strPanelBit[7]);
	m_NumPos8.SetCaption(m_strPanelBit[8]);
	m_NumPos9.SetCaption(m_strPanelBit[9]);
	m_NumPos10.SetCaption(m_strPanelBit[10]);
	m_NumPos11.SetCaption(m_strPanelBit[11]);

	CString Msg("");
	Msg.Format("%.1f", m_dMaxLimitValue); m_sMaxLimit.SetCaption(Msg);
	Msg.Format("%.1f", m_dMinLimitValue); m_sMinLimit.SetCaption(Msg);
}

void CDlgNumberKeyPad::SetCursorColor(int nNo, long lColor)
{
	switch(nNo)
	{
		case 0:
			m_NumPos0.SetBackColor(lColor);
			break;
		case 1:
			m_NumPos1.SetBackColor(lColor);
			break;
		case 2:
			m_NumPos2.SetBackColor(lColor);
			break;
		case 3:
			m_NumPos3.SetBackColor(lColor);
			break;
		case 4:
			m_NumPos4.SetBackColor(lColor);
			break;
		case 5:
			m_NumPos5.SetBackColor(lColor);
			break;
		case 6:
			m_NumPos6.SetBackColor(lColor);
			break;
		case 7:
			m_NumPos7.SetBackColor(lColor);
			break;
		case 8:
			m_NumPos8.SetBackColor(lColor);
			break;
		case 9:
			m_NumPos9.SetBackColor(lColor);
			break;
		case 10:
			m_NumPos10.SetBackColor(lColor);
			break;
		case 11:
			m_NumPos11.SetBackColor(lColor);
			break;
	}
}

void CDlgNumberKeyPad::AddBit(CString strBit)
{
	CString strBuf;

	if(m_bFirst == TRUE) OnButtonClr();
	m_bFirst = FALSE;

	// *******************************************
	// Cursor가 0위치에 있으면 Tail에 추가 
	// *******************************************
	if(m_nCursorPos == 0)
	{
		if(m_nNumLength < 10)
			m_nNumLength++;
		else
			return;
		for(int i=9; i>0; i--)
			m_strPanelBit[i+1] = m_strPanelBit[i];

		m_strPanelBit[1] = strBit;
	}
	// *******************************************
	// Cursor가 0위치에 없으면 중간에 Insert한다.
	// *******************************************
	else
	{
		m_strPanelBit[m_nCursorPos] = strBit;
		SetCursorColor(m_nCursorPos, DEFAULT_BACK_COLOR);
		m_nCursorPos--;
		SetCursorColor(m_nCursorPos, CURSOR_BACK_COLOR);
	}
	DisplayNum();
	SetCursorColor(m_nCursorPos, CURSOR_BACK_COLOR);
}

void CDlgNumberKeyPad::DeleteBit()
{
	if(m_nCursorPos == 0 || m_nNumLength == 0)
		return;
	else
	{
		for(int i=m_nCursorPos+1; i<=m_nNumLength; i++)
			m_strPanelBit[i-1] = m_strPanelBit[i];
		
	}
	m_strPanelBit[m_nNumLength] = "";
	OnButtonCursorFwd();
	m_nNumLength--;
	DisplayNum();
}

void CDlgNumberKeyPad::BackspaceBit()
{
	if(m_nCursorPos >= m_nNumLength){//m_nCursorPos == 0 || 
		OnButtonClr();
		return;
	}
	else
	{
		for(int i=m_nCursorPos; i<m_nNumLength; i++)
			m_strPanelBit[i] = m_strPanelBit[i+1];
		
	}
	m_strPanelBit[m_nNumLength] = "";
	m_nNumLength--;
	if(m_nNumLength == 0) OnButtonClr();
	DisplayNum();
}

BEGIN_EVENTSINK_MAP(CDlgNumberKeyPad, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgNumberKeyPad)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
