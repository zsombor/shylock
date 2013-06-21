; The name of the installer
Name "Shylock Book"

; The file to write
OutFile "booksetup.exe"

; The default installation directory
InstallDir $PROGRAMFILES\Shylock

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM SOFTWARE\Shylock "Install_Dir"

; Check installer integrity
CRCCheck on

; use compression only if there is something to gain
SetCompress auto

; No background image during install
BGGradient off

; Show install process details
ShowInstDetails show

; The text to prompt the user to enter a directory
ComponentText "You are now installing Shylock's opening book on your computer: "

; Disable directory page
DirText

; book file
Section "Book install"

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; InputFiles
  File "book.bin"
SectionEnd

Function .onInit
  ReadRegStr $INSTDIR HKLM SOFTWARE\Shylock "Install_Dir" 
  StrCmp $INSTDIR "" 0 NoAbort
      MessageBox MB_OK "An install of the Shylock binaries wasn't found, aborting! "
      Abort 
  NoAbort:
FunctionEnd
