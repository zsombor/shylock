/**
 @file
 Interface of a win32 timer with high resolution
*/
#if !defined _HIGHRESOLUTIONTIMER_H_
#define _HIGHRESOLUTIONTIMER_H_
#include <windows.h>

namespace Win
{
    /**
     HighResolutionTimer class, encapsulates the Win32 high resolution
     timer */
    class HighResolutionTimer
    {
    public:

        /** 
         start the clock 
         @exception throws Win::Exception upon failure */
        explicit HighResolutionTimer();

        /** 
         @return the ticks passed since last reset */
        LARGE_INTEGER Ticks();

        /** 
         @return the number of ticks in one second */
        LARGE_INTEGER Frequency();

        /**
         resets the clock */
        void InitBase();

    private:

        LARGE_INTEGER _frequency;
        LARGE_INTEGER _base;
    };
}
#endif //_HIGHRESOLUTIONTIMER_H_

