; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CManualTestDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ace400cal.h"
LastPage=0
CDK=Y

ClassCount=7
Class1=CACE400CalApp
Class2=CCalibDialog
Class3=CConfigDialog
Class4=CDlgNumberKeyPad
Class5=CMainDialog
Class6=CAboutDlg
Class7=CManualTestDlg

ResourceCount=6
Resource1=IDD_MAIN_DIALOG
Resource2=IDD_ABOUTBOX
Resource3=IDD_CALIB_DIALOG
Resource4=IDD_MANUAL_TEST_DIALOG
Resource5=IDD_DIALOG_NUM_PAD
Resource6=IDD_CONFIG_DIALOG

[CLS:CACE400CalApp]
Type=0
BaseClass=CWinApp
HeaderFile=ACE400Cal.h
ImplementationFile=ACE400Cal.cpp

[CLS:CCalibDialog]
Type=0
BaseClass=CDialog
HeaderFile=CalibDialog.h
ImplementationFile=CalibDialog.cpp
LastObject=CCalibDialog

[CLS:CConfigDialog]
Type=0
BaseClass=CDialog
HeaderFile=ConfigDialog.h
ImplementationFile=ConfigDialog.cpp

[CLS:CDlgNumberKeyPad]
Type=0
BaseClass=CDialog
HeaderFile=DlgNumberKeyPad.h
ImplementationFile=DlgNumberKeyPad.cpp

[CLS:CMainDialog]
Type=0
BaseClass=CDialog
HeaderFile=MainDialog.h
ImplementationFile=MainDialog.cpp
Filter=D
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ManualTestDlg.cpp
ImplementationFile=ManualTestDlg.cpp

[CLS:CManualTestDlg]
Type=0
BaseClass=CDialog
HeaderFile=ManualTestDlg.h
ImplementationFile=ManualTestDlg.cpp
LastObject=IDC_EDIT_LV1

[DLG:IDD_CALIB_DIALOG]
Type=1
Class=CCalibDialog
ControlCount=20
Control1=IDC_GRID_CAL_RESULT,{6262D3A0-531B-11CF-91F6-C2863C385E30},1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_COMBO_CALIB_MODE,combobox,1344340226
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_MEAS_CNT,edit,1350631552
Control6=IDC_GRID_CONDITION,{6262D3A0-531B-11CF-91F6-C2863C385E30},1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_BUTTON_LOAD_REF,button,1342242816
Control10=IDC_BUTTON_START_MEAS,button,1342242816
Control11=IDC_BUTTON_CHG_REF,button,1342251008
Control12=IDC_BUTTON_SAVE_REF,button,1342242816
Control13=IDC_BUTTON_CLR_SCREEN,button,1342242816
Control14=IDC_BUTTON_STOP_MEAS,button,1342242816
Control15=IDC_BUTTON_VIEW_RESULT_FILE,button,1342242816
Control16=IDC_BUTTON_AUTO_CALIB,button,1342242816
Control17=IDC_STATIC,button,1342177287
Control18=IDC_BUTTON_SAVE_CONDITION,button,1342242816
Control19=IDC_STATIC,static,1342308352
Control20=IDC_BUTTON_VIEW_CONDITION_FILE,button,1342242816

[DLG:IDD_CONFIG_DIALOG]
Type=1
Class=CConfigDialog
ControlCount=9
Control1=IDC_CHECK_PRT_BASIC,button,1342246915
Control2=IDC_STATIC,button,1342177287
Control3=IDC_CHECK_PRT_LEVEL1,button,1342246915
Control4=IDC_CHECK_PRT_Level2,button,1342246915
Control5=IDC_CHECK_PRT_LEVEL3,button,1342246915
Control6=IDC_CHECK_PRT_DEVICE,button,1342246915
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC_PRINT_STATE,static,1342308865
Control9=IDC_CHECK_PRT_DEV_D64,button,1342246915

[DLG:IDD_DIALOG_NUM_PAD]
Type=1
Class=CDlgNumberKeyPad
ControlCount=37
Control1=IDC_BUTTON_CANCEL,button,1342242816
Control2=IDC_BUTTON_ENTER,button,1342242816
Control3=IDC_BUTTON_KEYPAD_0,button,1342242816
Control4=IDC_BUTTON_KEYPAD_1,button,1342242816
Control5=IDC_BUTTON_KEYPAD_2,button,1342242816
Control6=IDC_BUTTON_KEYPAD_3,button,1342242816
Control7=IDC_BUTTON_KEYPAD_4,button,1342242816
Control8=IDC_BUTTON_KEYPAD_5,button,1342242816
Control9=IDC_BUTTON_KEYPAD_6,button,1342242816
Control10=IDC_BUTTON_KEYPAD_7,button,1342242816
Control11=IDC_BUTTON_KEYPAD_8,button,1342242816
Control12=IDC_BUTTON_KEYPAD_9,button,1342242816
Control13=IDC_BUTTON_DEL,button,1342242816
Control14=IDC_BUTTON_CURSOR_BACK,button,1342242816
Control15=IDC_BUTTON_CURSOR_FWD,button,1342242816
Control16=IDC_BUTTON_BACKSPACE,button,1342251008
Control17=IDC_BUTTON_CLR,button,1342242816
Control18=IDC_BUTTON_SIGN,button,1342242816
Control19=IDC_BUTTON_DOT,button,1342242816
Control20=IDC_SSPANEL_NUMPOS_1,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control21=IDC_SSPANEL_NUMPOS_2,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control22=IDC_SSPANEL_NUMPOS_3,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control23=IDC_SSPANEL_NUMPOS_4,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control24=IDC_SSPANEL_NUMPOS_5,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control25=IDC_SSPANEL_NUMPOS_6,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control26=IDC_SSPANEL_NUMPOS_7,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control27=IDC_SSPANEL_NUMPOS_8,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control28=IDC_SSPANEL_NUMPOS_9,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control29=IDC_SSPANEL_NUMPOS_10,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control30=IDC_SSPANEL_NUMPOS_11,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control31=IDC_SSPANEL_NUM_DATA,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control32=IDC_SSPANEL_NUMPOS_0,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control33=IDC_SSPANEL2,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control34=IDC_SSPANEL4,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control35=IDC_SSPANEL_MIN_VALUE,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control36=IDC_SSPANEL_MAX_VALUE,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
Control37=IDC_SSPANEL3,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280

[DLG:IDD_MAIN_DIALOG]
Type=1
Class=CMainDialog
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_BUTTON_Calib,button,1342242816
Control3=IDC_BUTTON_ManualTest,button,1342242816
Control4=IDC_BUTTON_Config,button,1342242817

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MANUAL_TEST_DIALOG]
Type=1
Class=CManualTestDlg
ControlCount=248
Control1=IDC_RADIO_HVO_GAIN1,button,1342377993
Control2=IDC_RADIO_HVO_GAIN2,button,1342246921
Control3=IDC_RADIO_HVO_GAIN3,button,1342246921
Control4=IDC_RADIO_HVO_GAIN4,button,1342246921
Control5=IDC_CHECK_VB_GUARD_ON,button,1208156163
Control6=IDC_CHECK_VC_GUARD_ON,button,1208025091
Control7=IDC_STATIC,button,1207959559
Control8=IDC_STATIC,button,1207959559
Control9=IDC_RADIO_I_CC,button,1342373897
Control10=IDC_RADIO_I_CV,button,1342242825
Control11=IDC_RADIO_PFT_FAST,button,1208156169
Control12=IDC_RADIO_PFT_SLOW,button,1208025097
Control13=IDC_RADIO_VBFT,button,1208025097
Control14=IDC_RADIO_IFFT,button,1342242825
Control15=IDC_RADIO_ISFT,button,1342242825
Control16=IDC_RADIO_PULSE_FREQ,button,1208156169
Control17=IDC_RADIO_PULSE_FREQ1,button,1208025097
Control18=IDC_RADIO_PULSE_FREQ2,button,1208025097
Control19=IDC_RADIO_PULSE_FREQ3,button,1208025097
Control20=IDC_RADIO_PULSE_VOLT,button,1208156169
Control21=IDC_RADIO_PULSE_VOLT1,button,1208025097
Control22=IDC_RADIO_PULSE_VOLT2,button,1208025097
Control23=IDC_RADIO_PULSE_VOLT3,button,1208025097
Control24=IDC_RADIO_VCFFT,button,1342377993
Control25=IDC_RADIO_VCSFT,button,1208025097
Control26=IDC_RADIO_HVOFT,button,1342246921
Control27=IDC_RADIO_FWFT,button,1342246921
Control28=IDC_BUTTON_AD_READ,button,1342373888
Control29=IDC_BUTTON_LV1_SET,button,1342242816
Control30=IDC_BUTTON_LV2_SET,button,1342242816
Control31=IDC_BUTTON_HV1_SET,button,1342242816
Control32=IDC_BUTTON_HV2_SET,button,1342242816
Control33=IDC_BUTTON_SPARK_SET,button,1342242816
Control34=IDC_BUTTON_VB_OUT_SET,button,1342242816
Control35=IDC_RADIO_PULSE_MEASURE_CAV0,button,1208160265
Control36=IDC_RADIO_PULSE_MEASURE_CAV1,button,1208029193
Control37=IDC_RADIO_PULSE_MEASURE_CAV2,button,1208029193
Control38=IDC_RADIO_PULSE_MEASURE_CAV3,button,1208029193
Control39=IDC_RADIO_I_RNG0,button,1342377993
Control40=IDC_RADIO_I_RNG1,button,1342246921
Control41=IDC_RADIO_I_RNG2,button,1342246921
Control42=IDC_RADIO_I_RNG3,button,1342246921
Control43=IDC_RADIO_I_RNG4,button,1342246921
Control44=IDC_RADIO_I_RNG6,button,1208029193
Control45=IDC_EDIT_I,edit,1350770689
Control46=IDC_BUTTON_I_SET,button,1342242816
Control47=IDC_CHECK_I_HIGH_GAIN,button,1342242819
Control48=IDC_STATIC,static,1342308866
Control49=IDC_RADIO_4WGAIN_S1000,button,1342377993
Control50=IDC_RADIO_4WGAIN_F10,button,1342246921
Control51=IDC_RADIO_4WGAIN_F100,button,1342246921
Control52=IDC_RADIO_4WGAIN_F1000,button,1342246921
Control53=IDC_RADIO_LV1,button,1342377993
Control54=IDC_RADIO_LV2,button,1342246921
Control55=IDC_RADIO_HV1,button,1342246921
Control56=IDC_RADIO_HV2,button,1342246921
Control57=IDC_RADIO_PULSE,button,1208029193
Control58=IDC_CHECK_DC_OUT,button,1208160259
Control59=IDC_BUTTON_BREAK,button,1342251008
Control60=IDC_RADIO_BBOXA,button,1208160265
Control61=IDC_RADIO_BBOXB,button,1208029193
Control62=IDC_RADIO_BBOXC,button,1208029193
Control63=IDC_RADIO_BBOXD,button,1208029193
Control64=IDC_CHECK_BPCB0,button,1208160259
Control65=IDC_CHECK_BPCB1,button,1208029187
Control66=IDC_CHECK_BPCB2,button,1208029187
Control67=IDC_CHECK_BPCB3,button,1208029187
Control68=IDC_RADIO_BPCB_ENABLE0,button,1208160265
Control69=IDC_RADIO_BPCB_ENABLE1,button,1208029193
Control70=IDC_STATIC,button,1208090631
Control71=IDC_STATIC,button,1207959559
Control72=IDC_STATIC,button,1207959559
Control73=IDC_STATIC,button,1207959559
Control74=IDC_RADIO_BCPLD0,button,1208160265
Control75=IDC_RADIO_BCPLD1,button,1208029193
Control76=IDC_RADIO_BCPLD2,button,1208029193
Control77=IDC_RADIO_BCPLD3,button,1208029193
Control78=IDC_LIST_BPOINT,listbox,1352859907
Control79=IDC_CHECK_BPIN0,button,1208029187
Control80=IDC_CHECK_BPIN1,button,1208029187
Control81=IDC_CHECK_BPIN2,button,1208029187
Control82=IDC_CHECK_BPIN3,button,1208029187
Control83=IDC_CHECK_BPIN4,button,1208029187
Control84=IDC_CHECK_BPIN5,button,1208029187
Control85=IDC_EDIT_BPIN,edit,1216421889
Control86=IDC_EDIT_BPOINT_TR,edit,1350639617
Control87=IDC_BUTTON_BPOINT_TR_SET,button,1342242816
Control88=IDC_BUTTON_BPOINT_TR_RST,button,1342242816
Control89=IDC_EDIT_BPOINT_RY,edit,1350639617
Control90=IDC_BUTTON_BPOINT_RY_SET,button,1342242816
Control91=IDC_BUTTON_BPOINT_RY_RST,button,1342242816
Control92=IDC_RADIO_BPOINT_CV,button,1208156169
Control93=IDC_RADIO_BPOINT_CC,button,1208025097
Control94=IDC_EDIT_BPOINT_CC_V,edit,1216552961
Control95=IDC_RADIO_CBOXA,button,1208160265
Control96=IDC_RADIO_CBOXB,button,1208029193
Control97=IDC_RADIO_CBOXC,button,1208029193
Control98=IDC_RADIO_CBOXD,button,1208029193
Control99=IDC_CHECK_CPCB0,button,1208160259
Control100=IDC_CHECK_CPCB1,button,1208029187
Control101=IDC_CHECK_CPCB2,button,1208029187
Control102=IDC_CHECK_CPCB3,button,1208029187
Control103=IDC_RADIO_CPCB_ENABLE0,button,1208160265
Control104=IDC_RADIO_CPCB_ENABLE1,button,1208029193
Control105=IDC_RADIO_CCPLD0,button,1208160265
Control106=IDC_RADIO_CCPLD1,button,1208029193
Control107=IDC_RADIO_CCPLD2,button,1208029193
Control108=IDC_RADIO_CCPLD3,button,1208029193
Control109=IDC_LIST_CPOINT,listbox,1352859907
Control110=IDC_CHECK_CPIN0,button,1208029187
Control111=IDC_CHECK_CPIN1,button,1208029187
Control112=IDC_CHECK_CPIN2,button,1208029187
Control113=IDC_CHECK_CPIN3,button,1208029187
Control114=IDC_CHECK_CPIN4,button,1208029187
Control115=IDC_CHECK_CPIN5,button,1208029187
Control116=IDC_EDIT_CPIN,edit,1216421889
Control117=IDC_EDIT_CPOINT_TR,edit,1350639617
Control118=IDC_BUTTON_CPOINT_TR_SET,button,1342242816
Control119=IDC_BUTTON_CPOINT_TR_RST,button,1342242816
Control120=IDC_EDIT_CPOINT_RY,edit,1350639617
Control121=IDC_BUTTON_CPOINT_RY_SET,button,1342242816
Control122=IDC_BUTTON_CPOINT_RY_RST,button,1342242816
Control123=IDC_RADIO_CPOINT_CV,button,1208156169
Control124=IDC_RADIO_CPOINT_CC,button,1208025097
Control125=IDC_EDIT_CPOINT_CC_V,edit,1216552961
Control126=IDC_EDIT_QM_BPOINT,edit,1350631553
Control127=IDC_EDIT_QM_CPOINT,edit,1350631553
Control128=IDC_BUTTON_POINT_SET,button,1342242816
Control129=IDC_BUTTON_BOX_RST,button,1342242816
Control130=IDC_LABEL_I_AMP,{978C9E23-D4B0-11CE-BF2D-00AA003F40D0},1342177280
Control131=IDC_LABEL_V_VOLT,{978C9E23-D4B0-11CE-BF2D-00AA003F40D0},1342177280
Control132=IDC_LABEL_R_OHM,{978C9E23-D4B0-11CE-BF2D-00AA003F40D0},1342177280
Control133=IDC_STATIC,button,1342177287
Control134=IDC_STATIC,button,1342177287
Control135=IDC_STATIC,button,1342177287
Control136=IDC_STATIC_I_UNIT,static,1342308864
Control137=IDC_STATIC,static,1342308866
Control138=IDC_STATIC,button,1342177287
Control139=IDC_STATIC,button,1342177287
Control140=IDC_STATIC,button,1342177287
Control141=IDC_STATIC,static,1342308866
Control142=IDC_STATIC,button,1342177287
Control143=IDC_STATIC,static,1342308865
Control144=IDC_STATIC,static,1342308865
Control145=IDC_STATIC,button,1342177287
Control146=IDC_STATIC,static,1342308865
Control147=IDC_STATIC,static,1342308866
Control148=IDC_LIST_MSG,listbox,1352663299
Control149=IDC_STATIC,static,1208091137
Control150=IDC_STATIC,static,1208091137
Control151=IDC_STATIC_I,static,1342308865
Control152=IDC_STATIC_V,static,1342308865
Control153=IDC_STATIC_R,static,1342308865
Control154=IDC_STATIC_QM_I_UNIT,static,1342308864
Control155=IDC_STATIC_QM_V_UNIT,static,1342308864
Control156=IDC_STATIC_QM_R_UNIT,static,1342308864
Control157=IDC_STATIC_QM_I_ADVALUE,static,1342181889
Control158=IDC_STATIC_QM_V_ADVALUE,static,1342181889
Control159=IDC_STATIC,static,1342308865
Control160=IDC_STATIC,static,1342308864
Control161=IDC_STATIC,static,1342308865
Control162=IDC_STATIC,static,1342308864
Control163=IDC_STATIC,static,1208091136
Control164=IDC_STATIC,static,1342308865
Control165=IDC_STATIC,static,1208091136
Control166=IDC_STATIC,static,1208090624
Control167=IDC_STATIC,static,1207959559
Control168=IDC_STATIC,static,1208090624
Control169=IDC_STATIC,static,1207959559
Control170=IDC_STATIC,static,1208090624
Control171=IDC_STATIC,static,1207959559
Control172=IDC_STATIC,static,1208090624
Control173=IDC_STATIC,static,1207959559
Control174=IDC_STATIC,static,1208090624
Control175=IDC_STATIC,static,1207959559
Control176=IDC_STATIC,static,1208090624
Control177=IDC_STATIC,static,1207959559
Control178=IDC_STATIC,static,1208090624
Control179=IDC_STATIC,static,1207959559
Control180=IDC_STATIC,static,1208090624
Control181=IDC_STATIC,static,1207959559
Control182=IDC_STATIC,static,1208090624
Control183=IDC_STATIC,static,1207959559
Control184=IDC_STATIC,static,1208090624
Control185=IDC_STATIC,static,1207959559
Control186=IDC_STATIC,static,1342308865
Control187=IDC_STATIC,static,1342308865
Control188=IDC_STATIC,static,1208091136
Control189=IDC_STATIC,button,1342177287
Control190=IDC_STATIC,static,1342177287
Control191=IDC_STATIC,static,1342177287
Control192=IDC_STATIC,static,1342177287
Control193=IDC_STATIC,static,1073741831
Control194=IDC_STATIC,static,1073741831
Control195=IDC_STATIC,static,1342177287
Control196=IDC_STATIC,button,1342177287
Control197=IDC_STATIC,button,1342177287
Control198=IDC_BUTTON_BPOINT_CCV_SET,button,1208025088
Control199=IDC_BUTTON_CPOINT_CCV_SET,button,1208025088
Control200=IDC_STATIC_BTR,static,1207960065
Control201=IDC_STATIC_BRY,static,1207960065
Control202=IDC_STATIC_CTR,static,1207960065
Control203=IDC_STATIC_CRY,static,1207960065
Control204=IDC_CHECK_SPARK_ON,button,1342242819
Control205=IDC_STATIC,button,1207959559
Control206=IDC_CHECK_PULSE,button,1208025091
Control207=IDC_STATIC,static,1208090624
Control208=IDC_STATIC,static,1342308865
Control209=IDC_STATIC,static,1342308865
Control210=IDC_STATIC,static,1342308865
Control211=IDC_STATIC,static,1342308865
Control212=IDC_STATIC_LV1,static,1342308864
Control213=IDC_STATIC,static,1342308864
Control214=IDC_STATIC,static,1342308864
Control215=IDC_STATIC_PULSE,static,1342308864
Control216=IDC_EDIT_LV1,edit,1350639617
Control217=IDC_EDIT_LV2,edit,1350639617
Control218=IDC_EDIT_HV1,edit,1350639617
Control219=IDC_EDIT_HV2,edit,1350639617
Control220=IDC_STATIC_LV1_MNT,static,1350570497
Control221=IDC_STATIC_LV2_MNT,static,1350570497
Control222=IDC_STATIC_HV1_MNT,static,1350570497
Control223=IDC_STATIC_HV2_MNT,static,1350570497
Control224=IDC_EDIT_SPARK,edit,1350639617
Control225=IDC_EDIT_LV1_DA,edit,1484857345
Control226=IDC_EDIT_LV2_DA,edit,1484857345
Control227=IDC_EDIT_HV1_DA,edit,1484857345
Control228=IDC_EDIT_HV2_DA,edit,1484857345
Control229=IDC_STATIC_LV1_MNT_AD,static,1350570497
Control230=IDC_STATIC_LV2_MNT_AD,static,1350570497
Control231=IDC_STATIC_HV1_MNT_AD,static,1350570497
Control232=IDC_STATIC_HV2_MNT_AD,static,1350570497
Control233=IDC_STATIC_VB_OUT_MNT,static,1350570497
Control234=IDC_BUTTON_INIT,button,1342242816
Control235=IDC_STATIC_I_ADC,static,1342181889
Control236=IDC_EDIT_SPARK_DA,edit,1350639617
Control237=IDC_STATIC,static,1342308352
Control238=IDC_STATIC_I_MNT,static,1342181889
Control239=IDC_STATIC_I_MNT_AD,static,1342181889
Control240=IDC_STATIC,static,1342308352
Control241=IDC_STATIC,static,1342308352
Control242=IDC_STATIC,static,1342308352
Control243=IDC_STATIC_I_UNIT2,static,1342308864
Control244=IDC_RADIO_HVC,button,1342312457
Control245=IDC_RADIO_LVC,button,1342181385
Control246=IDC_BUTTON_VB_RST_ALL,button,1342373888
Control247=IDC_BUTTON_VC_RST_ALL,button,1342373888
Control248=IDC_SSPANEL1,{0BA686B9-F7D3-101A-993E-0000C0EF6F5E},1342177280
