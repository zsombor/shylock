/**
 @file
 Interface of the Win::ActiveObject class
*/
#if !defined _ACTIVEOBJECT_H_
#define _ACTIVEOBJECT_H_
#include "Thread.h"
#include <windows.h>

namespace Win
{
    /**
      Abstract base class AciveObject, owns a captive thread used
      to perform different "BIG" tasks.*/
    class ActiveObject
    {
    public:
        /**
           default constructor, starts the thread in suspended mode */
        ActiveObject();

        /**
          destructor */
        virtual ~ActiveObject()
        {}

        /**
          kills the ActiveObject*/
        void Kill();

        /**
          resumes the ActiveObject from suspended mode, call this after creation */
        void Resume()
        {
            _thread.Resume();
        }

    protected:

        /**
          overide this to initialize your ActiveObject */
        virtual void InitThread()=0;

        /**
          overide this with your ActiveObject's main task */
        virtual void Run()= 0;

        /**
          overide this with your 'clean up' code */
        virtual void FlushThread()=0;

        /**
           flag used by the main thread to signal the captive thead to
           end his task. */
        int _isDying;

        /** 
          the thread procedure */
        static DWORD WINAPI ThreadEntry ( void *pArg);

        /**
          my @see Thread object itself.*/
        Thread _thread;
    };
}
#endif //_ACTIVEOBJECT_H_

