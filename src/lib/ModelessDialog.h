/**
 @file
 Interface of a generic win32 modeless dialog
*/
#if !defined _MODELESSDIALOG_H_
#define _MODELESSDIALOG_H_
#include "WinException.h"

namespace Win
{
    /**
     encapulates the win32 modeless dialog concept (ALMOST acceptable) */
    class ModelessDialog
    {
    public:
        /**
         @param hInst handle to the application instance
         @param lpTemplate pointer to the dialog template
         @param dlgProc the dialog procedure
         @exception throws Win::Exception upon failure */
        ModelessDialog(HINSTANCE hInst, LPCSTR lpTemplate, DLGPROC dlgProc)
        {
            if(0==(_hDialog=::CreateDialog(hInst, lpTemplate, 0, dlgProc)))
                throw Win::Exception("CreateDialog");
        }
        
        operator HWND()
        {
            return _hDialog;
        }
    private:
        HWND _hDialog;
    };
}
#endif //_MODELESSDIALOG_H_

