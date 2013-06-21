/**
 @file
 Code for a timer with a resolution of one milisecond.
*/
#include "MSecTimer.h"
#include "../lib/WinException.h"

using namespace Othello;

MSecTimer::MSecTimer()
{
    LARGE_INTEGER freq=_htimer.Frequency();
    if(freq.QuadPart<1000)
        throw Win::Exception("Hardware does not support a timer with resolution of atleast 1 ms");
}

unsigned int MSecTimer::Time()
{
    LARGE_INTEGER ticks=_htimer.Ticks();
    LARGE_INTEGER freq=_htimer.Frequency();
    return static_cast<unsigned int>(1000*(ticks.QuadPart/freq.QuadPart))+1;
}

void MSecTimer::ReInit()
{
    _htimer.InitBase();
}

