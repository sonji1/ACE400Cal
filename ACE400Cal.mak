# Microsoft Developer Studio Generated NMAKE File, Based on ACE400Cal.dsp
!IF "$(CFG)" == ""
CFG=ACE400Cal - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ACE400Cal - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ACE400Cal - Win32 Release" && "$(CFG)" != "ACE400Cal - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ACE400Cal.mak" CFG="ACE400Cal - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ACE400Cal - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ACE400Cal - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ACE400Cal - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ACE400Cal.exe"


CLEAN :
	-@erase "$(INTDIR)\DlgNumberKeyPad.obj"
	-@erase "$(INTDIR)\Error.obj"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\Globals.obj"
	-@erase "$(INTDIR)\labelcontrol.obj"
	-@erase "$(INTDIR)\MacroMotion.obj"
	-@erase "$(INTDIR)\MicroMotion.obj"
	-@erase "$(INTDIR)\picture.obj"
	-@erase "$(INTDIR)\sspnctrl.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\ACE400Cal.obj"
	-@erase "$(INTDIR)\ACE400Cal.pch"
	-@erase "$(INTDIR)\ACE400Cal.res"
	-@erase "$(INTDIR)\ACE400CalDlg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ACE400Cal.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ACE400Cal.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ACE400Cal.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ACE400Cal.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\ACE400Cal.pdb" /machine:I386 /out:"$(OUTDIR)\ACE400Cal.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DlgNumberKeyPad.obj" \
	"$(INTDIR)\Error.obj" \
	"$(INTDIR)\font.obj" \
	"$(INTDIR)\Globals.obj" \
	"$(INTDIR)\labelcontrol.obj" \
	"$(INTDIR)\MacroMotion.obj" \
	"$(INTDIR)\MicroMotion.obj" \
	"$(INTDIR)\picture.obj" \
	"$(INTDIR)\sspnctrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ACE400Cal.obj" \
	"$(INTDIR)\ACE400CalDlg.obj" \
	"$(INTDIR)\ACE400Cal.res" \
	".\APIDIO.LIB"

"$(OUTDIR)\ACE400Cal.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ACE400Cal - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ACE400Cal.exe" "$(OUTDIR)\ACE400Cal.bsc"


CLEAN :
	-@erase "$(INTDIR)\DlgNumberKeyPad.obj"
	-@erase "$(INTDIR)\DlgNumberKeyPad.sbr"
	-@erase "$(INTDIR)\Error.obj"
	-@erase "$(INTDIR)\Error.sbr"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\font.sbr"
	-@erase "$(INTDIR)\Globals.obj"
	-@erase "$(INTDIR)\Globals.sbr"
	-@erase "$(INTDIR)\labelcontrol.obj"
	-@erase "$(INTDIR)\labelcontrol.sbr"
	-@erase "$(INTDIR)\MacroMotion.obj"
	-@erase "$(INTDIR)\MacroMotion.sbr"
	-@erase "$(INTDIR)\MicroMotion.obj"
	-@erase "$(INTDIR)\MicroMotion.sbr"
	-@erase "$(INTDIR)\picture.obj"
	-@erase "$(INTDIR)\picture.sbr"
	-@erase "$(INTDIR)\sspnctrl.obj"
	-@erase "$(INTDIR)\sspnctrl.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\ACE400Cal.obj"
	-@erase "$(INTDIR)\ACE400Cal.pch"
	-@erase "$(INTDIR)\ACE400Cal.res"
	-@erase "$(INTDIR)\ACE400Cal.sbr"
	-@erase "$(INTDIR)\ACE400CalDlg.obj"
	-@erase "$(INTDIR)\ACE400CalDlg.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ACE400Cal.bsc"
	-@erase "$(OUTDIR)\ACE400Cal.exe"
	-@erase "$(OUTDIR)\ACE400Cal.ilk"
	-@erase "$(OUTDIR)\ACE400Cal.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ACE400Cal.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\ACE400Cal.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ACE400Cal.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DlgNumberKeyPad.sbr" \
	"$(INTDIR)\Error.sbr" \
	"$(INTDIR)\font.sbr" \
	"$(INTDIR)\Globals.sbr" \
	"$(INTDIR)\labelcontrol.sbr" \
	"$(INTDIR)\MacroMotion.sbr" \
	"$(INTDIR)\MicroMotion.sbr" \
	"$(INTDIR)\picture.sbr" \
	"$(INTDIR)\sspnctrl.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\ACE400Cal.sbr" \
	"$(INTDIR)\ACE400CalDlg.sbr"

"$(OUTDIR)\ACE400Cal.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\ACE400Cal.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ACE400Cal.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\DlgNumberKeyPad.obj" \
	"$(INTDIR)\Error.obj" \
	"$(INTDIR)\font.obj" \
	"$(INTDIR)\Globals.obj" \
	"$(INTDIR)\labelcontrol.obj" \
	"$(INTDIR)\MacroMotion.obj" \
	"$(INTDIR)\MicroMotion.obj" \
	"$(INTDIR)\picture.obj" \
	"$(INTDIR)\sspnctrl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ACE400Cal.obj" \
	"$(INTDIR)\ACE400CalDlg.obj" \
	"$(INTDIR)\ACE400Cal.res" \
	".\APIDIO.LIB"

"$(OUTDIR)\ACE400Cal.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ACE400Cal.dep")
!INCLUDE "ACE400Cal.dep"
!ELSE 
!MESSAGE Warning: cannot find "ACE400Cal.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ACE400Cal - Win32 Release" || "$(CFG)" == "ACE400Cal - Win32 Debug"
SOURCE=.\DlgNumberKeyPad.cpp

!IF  "$(CFG)" == "ACE400Cal - Win32 Release"


"$(INTDIR)\DlgNumberKeyPad.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ELSEIF  "$(CFG)" == "ACE400Cal - Win32 Debug"


"$(INTDIR)\DlgNumberKeyPad.obj"	"$(INTDIR)\DlgNumberKeyPad.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ENDIF 

SOURCE=.\Error.cpp

!IF  "$(CFG)" == "ACE400Cal - Win32 Release"


"$(INTDIR)\Error.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ELSEIF  "$(CFG)" == "ACE400Cal - Win32 Debug"


"$(INTDIR)\Error.obj"	"$(INTDIR)\Error.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ENDIF 

SOURCE=.\font.cpp

!IF  "$(CFG)" == "ACE400Cal - Win32 Release"


"$(INTDIR)\font.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ELSEIF  "$(CFG)" == "ACE400Cal - Win32 Debug"


"$(INTDIR)\font.obj"	"$(INTDIR)\font.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ENDIF 

SOURCE=.\Globals.cpp

!IF  "$(CFG)" == "ACE400Cal - Win32 Release"


"$(INTDIR)\Globals.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ELSEIF  "$(CFG)" == "ACE400Cal - Win32 Debug"


"$(INTDIR)\Globals.obj"	"$(INTDIR)\Globals.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ENDIF 

SOURCE=.\labelcontrol.cpp

!IF  "$(CFG)" == "ACE400Cal - Win32 Release"


"$(INTDIR)\labelcontrol.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ELSEIF  "$(CFG)" == "ACE400Cal - Win32 Debug"


"$(INTDIR)\labelcontrol.obj"	"$(INTDIR)\labelcontrol.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ENDIF 

SOURCE=.\MacroMotion.cpp

!IF  "$(CFG)" == "ACE400Cal - Win32 Release"


"$(INTDIR)\MacroMotion.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ELSEIF  "$(CFG)" == "ACE400Cal - Win32 Debug"


"$(INTDIR)\MacroMotion.obj"	"$(INTDIR)\MacroMotion.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ENDIF 

SOURCE=.\MicroMotion.cpp

!IF  "$(CFG)" == "ACE400Cal - Win32 Release"


"$(INTDIR)\MicroMotion.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ELSEIF  "$(CFG)" == "ACE400Cal - Win32 Debug"


"$(INTDIR)\MicroMotion.obj"	"$(INTDIR)\MicroMotion.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ENDIF 

SOURCE=.\picture.cpp

!IF  "$(CFG)" == "ACE400Cal - Win32 Release"


"$(INTDIR)\picture.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ELSEIF  "$(CFG)" == "ACE400Cal - Win32 Debug"


"$(INTDIR)\picture.obj"	"$(INTDIR)\picture.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ENDIF 

SOURCE=.\sspnctrl.cpp

!IF  "$(CFG)" == "ACE400Cal - Win32 Release"


"$(INTDIR)\sspnctrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ELSEIF  "$(CFG)" == "ACE400Cal - Win32 Debug"


"$(INTDIR)\sspnctrl.obj"	"$(INTDIR)\sspnctrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ACE400Cal - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ACE400Cal.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ACE400Cal.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ACE400Cal - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ACE400Cal.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\ACE400Cal.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\ACE400Cal.cpp

!IF  "$(CFG)" == "ACE400Cal - Win32 Release"


"$(INTDIR)\ACE400Cal.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ELSEIF  "$(CFG)" == "ACE400Cal - Win32 Debug"


"$(INTDIR)\ACE400Cal.obj"	"$(INTDIR)\ACE400Cal.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ENDIF 

SOURCE=.\ACE400Cal.rc

"$(INTDIR)\ACE400Cal.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ACE400CalDlg.cpp

!IF  "$(CFG)" == "ACE400Cal - Win32 Release"


"$(INTDIR)\ACE400CalDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ELSEIF  "$(CFG)" == "ACE400Cal - Win32 Debug"


"$(INTDIR)\ACE400CalDlg.obj"	"$(INTDIR)\ACE400CalDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ACE400Cal.pch"


!ENDIF 


!ENDIF 

