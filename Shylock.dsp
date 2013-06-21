# Microsoft Developer Studio Project File - Name="Shylock" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Shylock - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Shylock.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Shylock.mak" CFG="Shylock - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Shylock - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Shylock - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Shylock (second entry)"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Shylock - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Shylock___Win32_Release"
# PROP BASE Intermediate_Dir "Shylock___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Shylock___Win32_Release"
# PROP Intermediate_Dir "Shylock___Win32_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /Zi /O2 /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /D "WIN32_LEAN_AND_MEAN" /FAs /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib COMCTL32.LIB /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Shylock - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Shylock___Win32_Debug"
# PROP BASE Intermediate_Dir "Shylock___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Shylock___Win32_Debug"
# PROP Intermediate_Dir "Shylock___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /D "WIN32_LEAN_AND_MEAN" /FAcs /YX /FD /GZ /c
# SUBTRACT CPP /Z<none>
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib COMCTL32.LIB /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Shylock - Win32 Release"
# Name "Shylock - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\src\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\src\resource.h
# End Source File
# Begin Source File

SOURCE=.\src\resource.rc
# End Source File
# End Group
# Begin Group "Othello"

# PROP Default_Filter ""
# Begin Group "Board representations"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\model\Boards.cpp
# End Source File
# Begin Source File

SOURCE=.\src\model\Boards.h
# End Source File
# End Group
# Begin Group "Executing moves"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\model\MoveExecuter.cpp
# End Source File
# Begin Source File

SOURCE=.\src\model\MoveExecuter.h
# End Source File
# Begin Source File

SOURCE=.\src\model\UndoData.h
# End Source File
# End Group
# Begin Group "Generating moves"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\model\MoveListGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\src\model\MoveListGenerator.h
# End Source File
# End Group
# Begin Group "Search"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\model\Book.cpp
# End Source File
# Begin Source File

SOURCE=.\src\model\Book.h
# End Source File
# Begin Source File

SOURCE=.\src\model\Endgame.cpp
# End Source File
# Begin Source File

SOURCE=.\src\model\Endgame.h
# End Source File
# Begin Source File

SOURCE=.\src\model\Model.cpp
# End Source File
# Begin Source File

SOURCE=.\src\model\Model.h
# End Source File
# End Group
# Begin Group "Improving move ordering"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\model\BillMobilityDat.cpp
# End Source File
# Begin Source File

SOURCE=.\src\model\Hasher.cpp
# End Source File
# Begin Source File

SOURCE=.\src\model\Hasher.h
# End Source File
# Begin Source File

SOURCE=.\src\model\HHeuristic.cpp
# End Source File
# Begin Source File

SOURCE=.\src\model\HHeuristic.h
# End Source File
# Begin Source File

SOURCE=.\src\model\TTable.cpp
# End Source File
# Begin Source File

SOURCE=.\src\model\TTable.h
# End Source File
# End Group
# Begin Group "Node evaluation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\model\BillMobilityDat.h
# End Source File
# Begin Source File

SOURCE=.\src\model\Evaluator.cpp
# End Source File
# Begin Source File

SOURCE=.\src\model\Evaluator.h
# End Source File
# End Group
# Begin Group "Time control"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\model\MSecTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\model\MSecTimer.h
# End Source File
# Begin Source File

SOURCE=.\src\model\TimeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=.\src\model\TimeAllocator.h
# End Source File
# End Group
# End Group
# Begin Group "Win"

# PROP Default_Filter ""
# Begin Group "GDI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\lib\Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\src\lib\Bitmap.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\Canvas.cpp
# End Source File
# Begin Source File

SOURCE=.\src\lib\Canvas.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\Tools.h
# End Source File
# End Group
# Begin Group "Multithreading"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\lib\Active.cpp
# End Source File
# Begin Source File

SOURCE=.\src\lib\Active.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\Thread.h
# End Source File
# End Group
# Begin Group "Dialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\lib\Dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\lib\Dialog.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\MessageBox.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\ModelessDialog.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\OpenFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\lib\OpenFileDialog.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\SaveFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\lib\SaveFileDialog.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\lib\Accelerator.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\Crc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\lib\Crc.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\File.cpp
# End Source File
# Begin Source File

SOURCE=.\src\lib\File.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\HelpProvider.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\HighResolutionTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\lib\HighResolutionTimer.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\MessageLoop.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\SimpleControl.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\Spinner.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\Timer.h
# End Source File
# Begin Source File

SOURCE=.\src\lib\WinException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\lib\WinException.h
# End Source File
# End Group
# Begin Group "Controller, Views, GUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\AboutDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AboutDialog.h
# End Source File
# Begin Source File

SOURCE=.\src\Controller.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Controller.h
# End Source File
# Begin Source File

SOURCE=.\src\SearchOptionsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SearchOptionsDialog.h
# End Source File
# Begin Source File

SOURCE=.\src\TopWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TopWindow.h
# End Source File
# Begin Source File

SOURCE=.\src\Views.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Views.h
# End Source File
# End Group
# Begin Group "Build scripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Book.nsi
# End Source File
# Begin Source File

SOURCE=.\license.txt
# End Source File
# Begin Source File

SOURCE=.\options.doxygen
# End Source File
# Begin Source File

SOURCE=.\Shylock.nsi
# End Source File
# End Group
# Begin Group "Web"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\background1.jpg
# End Source File
# Begin Source File

SOURCE=.\Logo1.JPG
# End Source File
# Begin Source File

SOURCE=.\screen1.JPG
# End Source File
# Begin Source File

SOURCE=.\shylock.css
# End Source File
# Begin Source File

SOURCE=.\Usage.htm
# End Source File
# Begin Source File

SOURCE=.\vim.vialle.love.anim.gif
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\NewHandler.h
# End Source File
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# End Target
# End Project
