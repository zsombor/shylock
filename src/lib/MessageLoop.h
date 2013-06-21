/**
 @file
 Win32 message loop idiom
*/
#if !defined _MESSAGELOOP_H_
#define _MESSAGELOOP_H_
#include <windows.h>

namespace Win
{
    /**
     class MessageLoop encapsulates the win32 message loop idiom */
    class MessageLoop
    {
    public:
        /**
         @param applicationWnd handle to the application window
         @param haccel handle to the accelerator table */
        MessageLoop(HWND applicationWnd, HACCEL haccel)
            : _appWnd(applicationWnd), 
              _haccel(haccel)
        {}

        /**
         runs the message loop */
        int Run();

    private:
        HWND _appWnd;
        HACCEL _haccel; 
    };

    //this is inline only because I am too lazzy to create another file
    inline int MessageLoop::Run()
    {
        MSG msg;
        int status;
        while((status=::GetMessage(&msg, 0, 0, 0)) != 0)
        {
            if(status==-1)
                return -1;
            if(!::TranslateAccelerator(_appWnd, _haccel, &msg))
                ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        return msg.wParam;
    }
}
#endif //_MESSAGELOOP_H_

