/**
 @file
 Interface of the Othello::MSecTimer class.
*/
#if !defined _MSECTIMER_H_
#define _MSECTIMER_H_
#include "../lib/HighResolutionTimer.h"

namespace Othello
{
    /**
     MSecTimer class, provides time services in milliseconds */
    class MSecTimer
    {
    public:
        /**
         constructor, starts a stopper clock
         @exception throws Win::Exception upon failure  */
        explicit MSecTimer();
    
        /**
         reads the stopper clock  */
        unsigned int Time();
    
        /** 
         resets the stopper clock */
        void ReInit();
    
    private:
        /** internal timer object */
        Win::HighResolutionTimer _htimer;
    };
}
#endif //_MSECTIMER_H_

