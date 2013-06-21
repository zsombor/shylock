/**
 @file
 Interface of an attachable spin control.
*/
#if !defined _SPINNER_H__
#define _SPINNER_H__
#include <windows.h>
#include <commctrl.h>
#include <cassert>

namespace Win
{
    /**
     A spin control attachable to a buddy window. Please note 
     that you must create an spin control in a resource file, this
     must be done separetaly (...).*/
    class Spinner
    {
    public:
        /** 
         default constructor, sets the spinner's handle to NULL.
         You must call Init() explicitly later.*/
        explicit Spinner()
          : _hwnd(NULL)
        {}

        /**
         this sets the spin control's handle makeking it usable
         call this like spinner.Init(::GetDlgItem(dialogHwnd, IDC_SPIN)).*/
        void Init(HWND controlhwnd)
        {
            _hwnd=controlhwnd;
        }

        /** 
         attaches the spin control to the specified buddy window */
        void AttachToBuddy(HWND buddyHwnd)
        {
            assert(_hwnd!=NULL); //checks if Init() was called
            ::SendMessage(_hwnd, UDM_SETBUDDY, LONG(buddyHwnd), 0);
        }

        /**
         sets the range of the spin control */
        void SetRange(short low, short high)
        {
            assert(_hwnd!=NULL); //checks if Init() was called
            ::SendMessage(_hwnd, UDM_SETRANGE, 0, MAKELONG(high, low));
        }
    private:
        /** spin handle */
        HWND _hwnd;
    };
}
#endif  //_SPINNER_H__

