/**
 @file
 Code for a high resolution timer.
*/

#include "HighResolutionTimer.h"
#include "WinException.h"

using namespace Win;

HighResolutionTimer::HighResolutionTimer()
{
    if(0==::QueryPerformanceFrequency(&_frequency))
        throw Win::Exception("Installed hardware does not support a high-resolution performance counter!");

    InitBase();
}   

void HighResolutionTimer::InitBase()
{
    ::QueryPerformanceCounter(&_base);
}

LARGE_INTEGER HighResolutionTimer::Frequency()
{
    return _frequency;
}

LARGE_INTEGER HighResolutionTimer::Ticks()
{
    LARGE_INTEGER tmp;
    tmp.QuadPart=0;
    ::QueryPerformanceCounter(&tmp);
    tmp.QuadPart-=_base.QuadPart;
    return tmp;
}
