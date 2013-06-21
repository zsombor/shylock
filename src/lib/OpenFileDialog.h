/**
 @file
 Thin encapsulation of the win32 open dialog
*/
#if !defined _OPENFILEDIALOG_H_
#define _OPENFILEDIALOG_H_
#include <windows.h>
#include <commdlg.h>
#include <cderr.h>

namespace Win
{
    /**
     OpenFileDialog class, encapsulates the Win32 Common Controlls
     open dialogbox.
     BAD THING: this is specialized for files *.pos and *.*   */
    class OpenFileDialog
    {
    public:
        OpenFileDialog(HWND hwndParent);

        PSTR GetFileName();
    private:
        OPENFILENAME _ofn;
        TCHAR _szFileName[MAX_PATH]; 
    };
}
#endif //_OPENFILEDIALOG_H_

