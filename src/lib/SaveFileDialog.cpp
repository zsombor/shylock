/**
 @file
 Win::SaveFileDialog implementation
*/
#include <cstring>
#include "SaveFileDialog.h"


using namespace Win;

SaveFileDialog::SaveFileDialog(HWND hwnd, PSTR defext) 
{
    
    ::ZeroMemory(&_ofn,sizeof(OPENFILENAME));
    _ofn.lStructSize=sizeof(OPENFILENAME);
    _ofn.hwndOwner=hwnd;
    _ofn.hInstance=NULL;
    _ofn.lpstrFilter="Position files (.pos)\0*.pos\0Game records (.gam)\0*.gam\0All files (*.*)\0*.*\0";
    _ofn.lpstrCustomFilter=NULL;
    _ofn.nMaxCustFilter=0;
    if(!strcmp("pos",defext))
        _ofn.nFilterIndex=1;
    else
        if(!strcmp("gam",defext))
            _ofn.nFilterIndex=2;

    //used for returning the filename 
    _ofn.lpstrFile=NULL;
    // used for returning the filetitle 
    _ofn.lpstrFileTitle=NULL;
    //_ofn.nMaxFileTitle=MAX_PATH;
    _ofn.lpstrInitialDir=NULL;
    _ofn.lpstrTitle=NULL;
    //promtp for owerwrite, and hide "open read only" checkbox
    _ofn.Flags=OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY; 
    _ofn.nFileOffset=0;
    _ofn.nFileExtension=0;
    _ofn.lCustData=0L;
    _ofn.lpfnHook=NULL;
    _ofn.lpTemplateName=NULL;
    //append default extension 
    _ofn.lpstrDefExt=TEXT(defext); 
}

PSTR SaveFileDialog::GetFileName()
{
    _ofn.lpstrFile=_szFileName;
    _ofn.nMaxFile=sizeof(_szFileName);
    _ofn.lpstrFile[0]=TCHAR(0);
    //pstrTitleName;
    _ofn.lpstrFileTitle=NULL;
    _ofn.nMaxFileTitle=0;
    if(!::GetSaveFileName(&_ofn))
        return NULL;
    return _ofn.lpstrFile;
}


