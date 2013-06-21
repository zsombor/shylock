/**
 @file
 Interface of the Win::Accelerator class
*/
#if !defined _ACCELERATOR_H_
#define _ACCELERATOR_H_
#include "WinException.h"

namespace Win
{
    /**
      realizes the win32 keyboard accelerator. */ 
    class Accelerator
    {
    public:
        /**
           Constructor
           @param hInstance is a handle to the application instance.
           @param lpTableName identifies the accelerator table used.
           @exception throws Win::Exception upon failure.*/
        Accelerator(HINSTANCE hInstance, LPCTSTR lpTableName)
        {
            if(0==(_haccel=::LoadAccelerators(hInstance, lpTableName)))
                throw Win::Exception("Failed to load accelerator table.");
        }

        /**
            type conversion to HACCEL */
        operator HACCEL() 
        { 
            return _haccel; 
        }
    private:

        /**
            accelerator handle */
        HACCEL _haccel;
    };
}
#endif //_ACCELERATOR_H_

