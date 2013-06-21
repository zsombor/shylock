/**
 @file
 Interface of a generic (base) control.
*/
#if !defined _SIMPLECONTROL_H_
#define _SIMPLECONTROL_H_
#include <windows.h>
#include "Tools.h"

namespace Win
{
    /**
     SimpleControl class, base class for all controls (almost) */
    class SimpleControl
    {
    public:
        /**
         @param hwndParent handle to the parent window
         @param id the resource id of the control
         @param enabled true only if the control is in enabled state, 
                        this is the default*/
        SimpleControl(HWND hwndParent, int id, BOOL enabled=TRUE)
          :_hWnd(::GetDlgItem(hwndParent, id)),
           _hWndParent(hwndParent),
           _crect(_hWnd)
        {
            if(enabled==FALSE)
                Disable();
        }

        /**
         hides the control */
        void Hide()
        {
            ::ShowWindow(_hWnd, SW_HIDE);
        }

        /**
         shows the control */
        void Show()
        {
            ::ShowWindow(_hWnd, SW_SHOW);
        }

        /**
         @return true only if control is visible */
        BOOL IsVisible()
        {
            return(::IsWindowVisible(_hWnd));
        }

        /**
         sets the keyboard focus */
        void SetFocus()
        {
            ::SetFocus(_hWnd);
        }

        /**
         enables the window */
        void Enable()
        {
            ::EnableWindow(_hWnd, TRUE);
        }

        /**
         disables the control */
        void Disable()
        {
            ::EnableWindow(_hWnd, FALSE);
        }
        
        /**
         @return a handle to the control's window */
        HWND Hwnd () const 
        { 
            return _hWnd;
        }
        
        /**
         @return a ClientRect& with the rectangle of the control's 
         client surface */
        ClientRect& Rect()
        {
            return _crect;
        }

        /**
         adds the entire surface to the update region
         @param berase true only if the background is to be erased too, 
                       default is false*/
        void InvalidateClient(BOOL berase=FALSE)
        {
            ::InvalidateRect(_hWnd, 0, berase);
        }

        /**
          adds the control's area to the update region on the parent window
          @param berase true only if the background is to be erased too, 
                        default is false */
        void InvalidateOnParent(BOOL berase=FALSE)
        {
            RECT tmp=_crect;
            ::MapWindowPoints(_hWnd, _hWndParent, PPOINT(&tmp), 2);
            ::InvalidateRect(_hWndParent, &tmp, berase);
        }

        /**
         use this to check if after a WM_PAINT the update region intersects
         the control's client area, thus repainting is necessary
         @return true only if parent rect intersects the control's client 
                      area*/
        bool IsInsideRect(const RECT &prect)
        {
            RECT rect=_crect;
            ::MapWindowPoints(_hWnd, _hWndParent, PPOINT(&rect), 2);
            RECT intersection;
            return TRUE==::UnionRect(&intersection, &prect, &rect);
        }

    protected:
        HWND _hWnd;
        HWND _hWndParent;
        ClientRect _crect;
    };
}

#endif //_SIMPLECONTROL_H_

