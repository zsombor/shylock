/**
 @file
 Interface for time control object.
*/
#if !defined _TIMEALLOCATOR_H_
#define _TIMEALLOCATOR_H_
#include "MSecTimer.h"

namespace Othello
{
    /**
     TimeAllocator class, provides time control services */
    class TimeAllocator
    {
    public:
        TimeAllocator(unsigned int &AvaibleTime, 
                      unsigned int &InitTotalTime,
                      bool &TimedOut);

        /**
         call this before commencing the iterative deepening process
         @param empties the nomber of empty squares on the board */
        void AllocTarget(unsigned int empties);

        /**
         call this after searching for given depth, to check if a deeper
         search is possible */
        bool TimeIsUp(int depth);

        /**
         Anytime style time control, this is very expensive */
        bool TimeOk();

        /**
         adjust the elapsed time */
        void AdjustElapsed();

        /**
         for GUI only */
        unsigned int MSecRemaining();
    private:

         /** amount of time currently available for ShyLock */
        unsigned int &_avaible_time;
        
        /** amount of time initialy available for ShyLock */
        unsigned int &_init_totaltime;

        /** game was lost, but continuing with a new time slice */
        bool &_timed_out;
        
        /** the stoper's position when search to last depth was finished */
        unsigned int _last_depth_time;

        /** amount of time expandable for the best move determination */
        unsigned int _search_timelimit;
        
        /** timer object with exactly one milisecond resolution */
        MSecTimer _timer;

        /** records the time when an iteration (to a depth) was finished */
        unsigned int _time_log[60];

        /** constants that determine the amount of time (in slices) that are
        available for the ~30 decissions left to be made during the game */
        static int _timeSlice[];

        ////////////////////?????????????????//////////////////
        //an estimate for the amount of time required to complete 
        //the search should we decide to seach one ply further
        //unsigned int _estimated_time;
        ////////////////////?????????????????//////////////////
    };
}
#endif //_TIMEALLOCATOR_H_

