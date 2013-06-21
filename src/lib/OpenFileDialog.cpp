/**
 @file
 Win::OpenFileDialog implementation. 
*/

#include "OpenFileDialog.h"

using namespace Win;

OpenFileDialog::OpenFileDialog(HWND hwnd)
{
    ::ZeroMemory(&_ofn,sizeof(OPENFILENAME));
    _ofn.lStructSize=sizeof(OPENFILENAME);
    _ofn.hwndOwner=hwnd;
    _ofn.hInstance=NULL;
    _ofn.lpstrFilter="Position files (.pos)\0*.pos\0All files (*.*)\0*.*\0";
    _ofn.lpstrCustomFilter=NULL;
    _ofn.nMaxCustFilter=0;
    _ofn.nFilterIndex=1;
    //used for returning the filename 
    _ofn.lpstrFile=NULL;
    // used for returning the filetitle 
    _ofn.lpstrFileTitle=NULL;
    //_ofn.nMaxFileTitle=MAX_PATH;
    _ofn.lpstrInitialDir=NULL;
    _ofn.lpstrTitle=NULL;
    //promtp for owerwrite, and hide "open read only" checkbox
    _ofn.Flags=OFN_FILEMUSTEXIST| OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
    _ofn.nFileOffset=0;
    _ofn.nFileExtension=0;
    _ofn.lCustData=0L;
    _ofn.lpfnHook=NULL;
    _ofn.lpTemplateName=NULL;
    //append default extension "pos" to filename
    _ofn.lpstrDefExt=TEXT("pos"); 
}

PSTR OpenFileDialog::GetFileName()
{
    _ofn.lpstrFile=_szFileName;
    _ofn.nMaxFile=sizeof(_szFileName);
    _ofn.lpstrFile[0]=TCHAR(0);
    _ofn.lpstrFileTitle=NULL;
    _ofn.nMaxFileTitle=0;
    if(!::GetOpenFileName(&_ofn))
        return NULL;
    return _ofn.lpstrFile;
}
