# Microsoft Developer Studio Project File - Name="libsipc4" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libsipc4 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libsipc4.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libsipc4.mak" CFG="libsipc4 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libsipc4 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libsipc4 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libsipc4 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"LIB_Release\libsipc4.lib"

!ELSEIF  "$(CFG)" == "libsipc4 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"LIB_Debug\libsipc4d.lib"

!ENDIF 

# Begin Target

# Name "libsipc4 - Win32 Release"
# Name "libsipc4 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "src"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=..\src\sip_authorization.c
# End Source File
# Begin Source File

SOURCE=..\src\sip_client.c
# End Source File
# Begin Source File

SOURCE=..\src\sip_common.c
# End Source File
# Begin Source File

SOURCE=..\src\sip_cseq.c
# End Source File
# Begin Source File

SOURCE=..\src\sip_def.c
# End Source File
# Begin Source File

SOURCE=..\src\sip_message.c
# End Source File
# Begin Source File

SOURCE=..\src\sip_startline.c
# End Source File
# Begin Source File

SOURCE=..\src\sip_to.c
# End Source File
# Begin Source File

SOURCE=..\src\sip_www_authenticate.c
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "include"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\include\sip.h
# End Source File
# Begin Source File

SOURCE=..\include\sip_authorization.h
# End Source File
# Begin Source File

SOURCE=..\include\sip_client.h
# End Source File
# Begin Source File

SOURCE=..\include\sip_common.h
# End Source File
# Begin Source File

SOURCE=..\include\sip_cseq.h
# End Source File
# Begin Source File

SOURCE=..\include\sip_def.h
# End Source File
# Begin Source File

SOURCE=..\include\sip_message.h
# End Source File
# Begin Source File

SOURCE=..\include\sip_startline.h
# End Source File
# Begin Source File

SOURCE=..\include\sip_to.h
# End Source File
# Begin Source File

SOURCE=..\include\sip_www_authenticate.h
# End Source File
# End Group
# End Group
# End Target
# End Project
