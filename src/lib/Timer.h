/**
 @file
 Win32 WM_TIMER timer object.
*/
#if !defined _TIMER_H_
#define _TIMER_H_
#include <windows.h>

namespace Win
{
    /**
     encapsulates the win32 (WM_TIMER) timer */
    class Timer
    {
    public:
        /**
         @param hwnd handle to the window receiving the WM_TIMER messages
         @param id an optional timer id */
        Timer(HWND hwnd=0, int id=-1) 
            : _hwnd(hwnd),
              _id(id)
        {}

        ~Timer()
        {
            Kill();
        }

        /**
         @param milliSec number of miliseconds before sending WM_TIMER */
        void Set(int milliSec)
        {
            int ret=::SetTimer(_hwnd, _id, milliSec, 0);
            if(_hwnd==NULL)
                _id=ret;
        }

        /**
         stop sending messages! */
        void Kill()
        {
            ::KillTimer(_hwnd, _id);
        }

    private:
        HWND    _hwnd;
        int     _id;
    };
}
#endif //_TIMER_H_

