/**
 @file
 Distributing time slices for different parts of the game.
*/

#include <cassert>
#include "TimeAllocator.h"

using namespace Othello;


TimeAllocator::TimeAllocator(unsigned int &AvaibleTime, 
                             unsigned int &InitTotalTime,
                            /* unsigned int &NodeCount,*/
                             bool &TimedOut)
    :_avaible_time(AvaibleTime),
     _init_totaltime(InitTotalTime),
/*     _nodecount(NodeCount), */
     _timed_out(TimedOut),
/*     _speed(1), */
     _last_depth_time(0),
     _search_timelimit(0)
{}

int TimeAllocator::_timeSlice[]=
{
    1, 1, 1, 5, 6, 7, 7, 8, 8, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    10, 10, 11, 7, 4, 1, 1, 1, 1, 1,
    1, 1
};


void TimeAllocator::AllocTarget(unsigned int empties)
{
    //initializations
    _search_timelimit=0; _timer.ReInit();
    _time_log[0]=_timer.Time(); _last_depth_time=_time_log[0];

    int sum=0, rsum=0, thisk=(60-empties)/2;
    unsigned int k=0;

    for(k=0; k<sizeof(_timeSlice)/sizeof(_timeSlice[0]); k++)
        sum+=_timeSlice[k];

    for(k=thisk+1; k<sizeof(_timeSlice)/sizeof(_timeSlice[0]); k++)
        rsum+=_timeSlice[k];

    double usable_time_ratio=
           double(_timeSlice[thisk])/double(rsum+_timeSlice[thisk]);
    
    double norm_time_ratio=double(_timeSlice[thisk])/double(sum);

    _search_timelimit=static_cast<unsigned int>
                          (usable_time_ratio*_avaible_time);
    
    unsigned int norm_search_limit=static_cast<unsigned int>
                          (norm_time_ratio*_init_totaltime);
    
    if(_search_timelimit < norm_search_limit)
        _search_timelimit=norm_search_limit;
}


bool TimeAllocator::TimeIsUp(int depth)
{
    assert(depth>0 && depth<60);
    _time_log[depth]=_timer.Time();

    //mark the elapsed time
    if(_time_log[depth] < _avaible_time)
    {
        _avaible_time-=_time_log[depth]-_last_depth_time;
    }
    else
    {
        _timed_out=true;
        //the game is lost but try to be polite and finish in one minute
        _avaible_time+=60*1000;
        return true;
    }
    
    _last_depth_time=_time_log[depth];
    
    if(_time_log[depth]>=_search_timelimit)
    {
        _search_timelimit=0;
        return true;
    }

    //calc the steepest increase
    double qmax=1;
    for(int i=1; i<depth; i++)
    {
        double q=double(_time_log[i+1])/double(_time_log[i]);
        if(q<20 && qmax<q)
            qmax=q;
    }
    unsigned int estimated=static_cast<unsigned int>
                           (qmax*(_time_log[depth]-_time_log[depth-1]));

    if(estimated+_time_log[depth]>=_search_timelimit)
    {
        _search_timelimit=0;
        return true;
    }
    return false;
}

unsigned int TimeAllocator::MSecRemaining()
{
    unsigned int const tmp=_avaible_time-(_timer.Time()-_last_depth_time);
    if(tmp>_init_totaltime)
    {
        _timed_out=true;
        //the game is lost but try to be polite and finish in one minute
        _avaible_time+=60*1000;
        return 0;
    }
    return tmp; 
}

void TimeAllocator::AdjustElapsed()
{
    _avaible_time-=_timer.Time()-_last_depth_time;
}


//a simplistic solution
bool TimeAllocator::TimeOk()
{
    if(_search_timelimit==0)
        return true;

    if(_timer.Time()>=_search_timelimit)
        return false;

    return true;
}

