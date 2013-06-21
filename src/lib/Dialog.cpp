/** 
 @file 
 The code for the modal Dialog class.
*/

#include "Dialog.h"

using namespace Win;

Dialog::Dialog(HINSTANCE appInstance, LPCSTR templateName, 
               HWND parent)
  :_appInstance(appInstance),
   _templateName(templateName),
   _parent(parent)
{
}

void Dialog::Show()
{
    ::DialogBoxParam(_appInstance, _templateName, 
                     _parent, DLGPROC(DlgProc),
                     long(this));
}


void Dialog::CenterWindow(HWND hwnd)
{
    //retrive the RECT of parent window
    RECT parentRect;
    {
        HWND hwndParent=::GetParent(hwnd);
        //if we have no parent then center relative to the desktop
        if(hwndParent==NULL) 
            hwndParent=::GetDesktopWindow(); 
        ::GetWindowRect(hwndParent, &parentRect); 
    }

    RECT dialogRect;
    ::GetWindowRect(hwnd, &dialogRect);
    ::OffsetRect(&dialogRect, -dialogRect.left, -dialogRect.top);

    int diffWidth=parentRect.right - parentRect.left - dialogRect.right,
        diffHeight=parentRect.bottom - parentRect.top - dialogRect.bottom;

    ::OffsetRect(&dialogRect, parentRect.left + diffWidth/2, 
                 parentRect.top + diffHeight/2);

    ::SetWindowPos(hwnd, HWND_TOP, dialogRect.left,
                   dialogRect.top, 0, 0, SWP_NOSIZE); 
}

BOOL CALLBACK 
Dialog::DlgProc(HWND hwnd, UINT message, UINT wParam, LPARAM lParam)
{
    static Dialog *pdialog=0;
    switch (message)
    {
    case WM_INITDIALOG:
        pdialog=reinterpret_cast<Dialog *>(lParam);
        CenterWindow(hwnd);
        return pdialog->OnInitDialog(hwnd);
    case WM_COMMAND:
        if(pdialog!=0) 
            return pdialog->OnCommand(LOWORD(wParam));
    }
    return FALSE;
}

bool Dialog::OnInitDialog(HWND hwnd)
{
    _hwnd=hwnd;
    return TRUE;
}

bool Dialog::OnCommand(WORD command)
{
    switch(command)
    {
    case IDOK:
    case IDCANCEL:
        ::EndDialog(_hwnd, 0);
        return TRUE;
    }        
    return FALSE;
}
