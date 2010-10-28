# Microsoft Developer Studio Project File - Name="libfetion_test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=libfetion_test - Win32 D_Share
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libfetion_test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libfetion_test.mak" CFG="libfetion_test - Win32 D_Share"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libfetion_test - Win32 D_Static" (based on "Win32 (x86) Console Application")
!MESSAGE "libfetion_test - Win32 D_Share" (based on "Win32 (x86) Console Application")
!MESSAGE "libfetion_test - Win32 R_Static" (based on "Win32 (x86) Console Application")
!MESSAGE "libfetion_test - Win32 R_Share" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libfetion_test - Win32 D_Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libfetion_test___Win32_D_Static"
# PROP BASE Intermediate_Dir "libfetion_test___Win32_D_Static"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "libfetion_test___Win32_D_Static"
# PROP Intermediate_Dir "libfetion_test___Win32_D_Static"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__WIN32__" /D "CURL_STATICLIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__WIN32__" /D "CURL_STATICLIB" /D "USE_SSLEAY" /D "PTW32_STATIC_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wldap32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib wldap32.lib ..\lib_link\pthread_static.lib ..\lib_link\libeay32_static.lib ..\lib_link\ssleay32_static.lib ..\lib_link\libcurl_static.lib ..\lib_link\libxml2_static.lib ws2_32.lib ../src/sip/SIPC-4/vc6/LIB_Debug/libsipc4d.lib /nologo /subsystem:console /debug /machine:I386 /out:"bin/libfetion_test_d_static.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "libfetion_test - Win32 D_Share"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libfetion_test___Win32_D_Share"
# PROP BASE Intermediate_Dir "libfetion_test___Win32_D_Share"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "libfetion_test___Win32_D_Share"
# PROP Intermediate_Dir "libfetion_test___Win32_D_Share"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__WIN32__" /D "CURL_STATICLIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__WIN32__" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wldap32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib ..\lib_link\libcurl_ssl_share.lib wldap32.lib ..\lib_link\pthread_share.lib ..\lib_link\libeay32_share.lib ..\lib_link\ssleay32_share.lib ..\lib_link\libxml2_share.lib ws2_32.lib ../src/sip/SIPC-4/vc6/LIB_Debug/libsipc4d.lib /nologo /subsystem:console /debug /machine:I386 /out:"bin/libfetion_test_d_share.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "libfetion_test - Win32 R_Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libfetion_test___Win32_R_Static"
# PROP BASE Intermediate_Dir "libfetion_test___Win32_R_Static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "libfetion_test___Win32_R_Static"
# PROP Intermediate_Dir "libfetion_test___Win32_R_Static"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "__WIN32__" /D "CURL_STATICLIB" /D "PTW32_STATIC_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib wldap32.lib ..\lib_link\pthread_static.lib ..\lib_link\libeay32_static.lib ..\lib_link\ssleay32_static.lib ..\lib_link\libcurl_static.lib ..\lib_link\libxml2_static.lib ws2_32.lib ../src/sip/SIPC-4/vc6/LIB_Debug/libsipc4d.lib /nologo /subsystem:console /machine:I386 /out:"bin/libfetion_test_r_static.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "libfetion_test - Win32 R_Share"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libfetion_test___Win32_R_Share"
# PROP BASE Intermediate_Dir "libfetion_test___Win32_R_Share"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "libfetion_test___Win32_R_Share"
# PROP Intermediate_Dir "libfetion_test___Win32_R_Share"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "__WIN32__" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib wldap32.lib ..\lib_link\pthread_share.lib ..\lib_link\libeay32_share.lib ..\lib_link\ssleay32_share.lib ..\lib_link\libcurl_share.lib ..\lib_link\libxml2_share.lib ws2_32.lib ../src/sip/SIPC-4/vc6/LIB_Debug/libsipc4d.lib /nologo /subsystem:console /machine:I386 /out:"bin/libfetion_test_r_share.exe"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "libfetion_test - Win32 D_Static"
# Name "libfetion_test - Win32 D_Share"
# Name "libfetion_test - Win32 R_Static"
# Name "libfetion_test - Win32 R_Share"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\config.c
# End Source File
# Begin Source File

SOURCE=..\src\crypto.c
# End Source File
# Begin Source File

SOURCE=..\src\fxsocket.c
# End Source File
# Begin Source File

SOURCE=..\src\helper.c
# End Source File
# Begin Source File

SOURCE=..\src\initial.c
# End Source File
# Begin Source File

SOURCE=..\src\log.c
# End Source File
# Begin Source File

SOURCE=..\src\login.c
# End Source File
# Begin Source File

SOURCE=..\src\main.c
# End Source File
# Begin Source File

SOURCE=..\src\mem.c
# End Source File
# Begin Source File

SOURCE=..\src\mutex.c
# End Source File
# Begin Source File

SOURCE=..\src\process.c
# End Source File
# Begin Source File

SOURCE=..\src\protocol.c
# End Source File
# Begin Source File

SOURCE=..\src\sipc.c
# End Source File
# Begin Source File

SOURCE=..\src\thread.c
# End Source File
# Begin Source File

SOURCE=..\src\utf8.c
# End Source File
# Begin Source File

SOURCE=..\src\xml.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\commdef.h
# End Source File
# Begin Source File

SOURCE=..\src\config.h
# End Source File
# Begin Source File

SOURCE=..\src\crypto.h
# End Source File
# Begin Source File

SOURCE=..\src\fxsocket.h
# End Source File
# Begin Source File

SOURCE=..\src\helper.h
# End Source File
# Begin Source File

SOURCE=..\src\initial.h
# End Source File
# Begin Source File

SOURCE=..\src\log.h
# End Source File
# Begin Source File

SOURCE=..\src\login.h
# End Source File
# Begin Source File

SOURCE=..\src\mem.h
# End Source File
# Begin Source File

SOURCE=..\src\mutex.h
# End Source File
# Begin Source File

SOURCE=..\src\process.h
# End Source File
# Begin Source File

SOURCE=..\src\protocol.h
# End Source File
# Begin Source File

SOURCE=..\src\sipc.h
# End Source File
# Begin Source File

SOURCE=..\src\thread.h
# End Source File
# Begin Source File

SOURCE=..\src\utf8.h
# End Source File
# Begin Source File

SOURCE=..\src\xml.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
