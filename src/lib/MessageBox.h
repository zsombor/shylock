/**
 @file
 A thin encapsulation of the Win32 MessageBox API.
*/
#if !defined _MYLIB__MESSAGEBOX__H_
#define _MYLIB__MESSAGEBOX__H_
#include <windows.h>

namespace Win
{
    /**
     displays a win32 MessageBox */
    class MessageBox
    {
    public:
        MessageBox(HWND hWndParent, 
                   const char* Text, 
                   const char* Caption=0,
                   UINT Type=MB_OK)
        {
            _status=::MessageBox(hWndParent, Text, Caption, Type);
        }
        UINT Status() const
        {
            return _status;
        }
    private:
        UINT _status;
    };
}

#endif //_MYLIB__MESSAGEBOX__H_

