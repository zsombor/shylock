/**
 @file
 Code for the dialog displayed from the "Options/Search Settings" menuitem.
*/

#include <cstdio>
#include "SearchOptionsDialog.h"


SearchOptionsDialog::SearchOptionsDialog(HINSTANCE appInstance, 
                                         HWND parent,
                                         unsigned LogSize,
                                         unsigned Minutes)
  :Dialog(appInstance, MAKEINTRESOURCE(DLG_SEARCHOPT), parent),
   _logsize(LogSize),
   _minutes(Minutes),
   _status(IDCANCEL)
{
    Show();
}


bool SearchOptionsDialog::OnInitDialog(HWND hwnd)
{
    _hwnd=hwnd;

    //init TTsize
    HWND eHwnd=::GetDlgItem(_hwnd, IDC_EDIT1);
    char buf[20];
    sprintf(buf,"%u",_logsize);
    ::SendMessage(eHwnd, WM_SETTEXT, 0, LPARAM(buf));
    _spin_size.Init(::GetDlgItem(_hwnd, IDC_SPIN4));
    _spin_size.AttachToBuddy(eHwnd);
    _spin_size.SetRange(17, 25);
    

    //init think time
    eHwnd=::GetDlgItem(_hwnd, IDC_EDIT2);
    sprintf(buf,"%u",_minutes);
    ::SendMessage(eHwnd, WM_SETTEXT, 0, LPARAM(buf));
    _spin_minutes.Init(::GetDlgItem(_hwnd, IDC_SPIN5));
    _spin_minutes.AttachToBuddy(eHwnd);
    _spin_minutes.SetRange(1, 300);

    return TRUE;
}

bool SearchOptionsDialog::OnCommand(WORD command)
{
    switch(command)
    {
    case IDOK:
        SaveEdit();
    case IDCANCEL:
        _status=command;
        ::EndDialog(_hwnd, 0);
        return TRUE;
    }
    return FALSE;
}

void SearchOptionsDialog::SaveEdit()
{
    HWND eHwnd=::GetDlgItem(_hwnd, IDC_EDIT1);
    char buf[20];
    ::GetWindowText(eHwnd, buf, sizeof(buf));
    unsigned int t1=0;
    int ret=sscanf(buf, "%u", &t1);

    eHwnd=::GetDlgItem(_hwnd, IDC_EDIT2);
    ::GetWindowText(eHwnd, buf, sizeof(buf));
    unsigned int t2=0;

    if(1==ret && 1==sscanf(buf, "%u", &t2))
    {
        _logsize=t1;
        _minutes=t2;
    }
}
