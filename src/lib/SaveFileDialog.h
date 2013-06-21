/**
 @file
 Thin encapsulation of the win32 save dialog
*/
#if !defined __SAVEFILEDIALOG__H__
#define __SAVEFILEDIALOG__H__
#include <windows.h>
#include <commdlg.h>
#include <cderr.h>

namespace Win
{
    /**
     SaveFileDialog class, encapsulates the Win32 Common Controlls
     open dialogbox.
     BAD THING: this is specialized for files *.pos and *.*  */
    class SaveFileDialog  
    {
    public:
        SaveFileDialog(HWND hwnd, PSTR defext);

        PSTR GetFileName();

    private:
        OPENFILENAME _ofn;
        TCHAR _szFileName[MAX_PATH]; //owned!!!
    };
}
#endif //__SAVEFILEDIALOG__H__

