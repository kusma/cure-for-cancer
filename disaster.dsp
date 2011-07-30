# Microsoft Developer Studio Project File - Name="disaster" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=disaster - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "disaster.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "disaster.mak" CFG="disaster - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "disaster - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "disaster - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "disaster - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /Gi /GX /Zi /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib user32.lib ole32.lib olepro32.lib gdi32.lib bassmod/bassmod.lib Advapi32.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386
# SUBTRACT LINK32 /profile /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "disaster - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 vfw32.lib winmm.lib user32.lib ole32.lib olepro32.lib gdi32.lib bassmod/bassmod.lib Advapi32.lib /nologo /subsystem:console /profile /debug /machine:I386

!ENDIF 

# Begin Target

# Name "disaster - Win32 Release"
# Name "disaster - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "kancr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\kancr\camera.cpp
# End Source File
# Begin Source File

SOURCE=.\kancr\camera.h
# End Source File
# Begin Source File

SOURCE=.\kancr\kancr.h
# End Source File
# Begin Source File

SOURCE=.\kancr\light.cpp
# End Source File
# Begin Source File

SOURCE=.\kancr\light.h
# End Source File
# Begin Source File

SOURCE=.\kancr\lwo.cpp
# End Source File
# Begin Source File

SOURCE=.\kancr\lwo.h
# End Source File
# Begin Source File

SOURCE=.\kancr\matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\kancr\matrix.h
# End Source File
# Begin Source File

SOURCE=.\kancr\mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\kancr\mesh.h
# End Source File
# Begin Source File

SOURCE=.\kancr\object.cpp
# End Source File
# Begin Source File

SOURCE=.\kancr\object.h
# End Source File
# Begin Source File

SOURCE=.\kancr\plugin.cpp
# End Source File
# Begin Source File

SOURCE=.\kancr\plugin.h
# End Source File
# Begin Source File

SOURCE=.\kancr\renderer.cpp
# End Source File
# Begin Source File

SOURCE=.\kancr\renderer.h
# End Source File
# Begin Source File

SOURCE=.\kancr\scene.cpp
# End Source File
# Begin Source File

SOURCE=.\kancr\scene.h
# End Source File
# Begin Source File

SOURCE=.\kancr\triangle.cpp
# End Source File
# Begin Source File

SOURCE=.\kancr\triangle.h
# End Source File
# Begin Source File

SOURCE=.\kancr\vector.h
# End Source File
# End Group
# Begin Group "tool"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\tool\font.cpp
# End Source File
# Begin Source File

SOURCE=.\tool\font.h
# End Source File
# Begin Source File

SOURCE=.\tool\linkedlist.h
# End Source File
# Begin Source File

SOURCE=.\tool\misc.cpp
# End Source File
# Begin Source File

SOURCE=.\tool\misc.h
# End Source File
# Begin Source File

SOURCE=.\tool\sortlist.h
# End Source File
# Begin Source File

SOURCE=.\tool.h
# End Source File
# End Group
# Begin Group "effects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\effects\misc.h
# End Source File
# Begin Source File

SOURCE=.\effects\tunnel.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\tunnel.h
# End Source File
# End Group
# Begin Group "generators"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\generators\sphere.h
# End Source File
# End Group
# Begin Group "file"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\file\file.cpp
# End Source File
# Begin Source File

SOURCE=.\file\file.h
# End Source File
# Begin Source File

SOURCE=.\file\urarlib.c
# End Source File
# Begin Source File

SOURCE=.\file\urarlib.h
# End Source File
# End Group
# Begin Group "tyfuus"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\tyfuus\grid.cpp
# End Source File
# Begin Source File

SOURCE=.\tyfuus\grid.h
# End Source File
# Begin Source File

SOURCE=.\tyfuus\sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\tyfuus\sprite.h
# End Source File
# Begin Source File

SOURCE=.\tyfuus\tyfuus.cpp
# End Source File
# Begin Source File

SOURCE=.\tyfuus\tyfuus.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\arebare.cpp
# End Source File
# Begin Source File

SOURCE=.\leepra.cpp
# End Source File
# Begin Source File

SOURCE=.\menu.cpp
# End Source File
# Begin Source File

SOURCE=.\Script.rc
# End Source File
# Begin Source File

SOURCE=.\test.cpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\menu.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\arebare.h
# End Source File
# Begin Source File

SOURCE=.\leepra.h
# End Source File
# Begin Source File

SOURCE=.\menu.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Target
# End Project
