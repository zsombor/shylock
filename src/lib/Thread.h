/**
 @file
 Interfaces of the Win::Thread and Win::Event classes
*/
#if !defined _THREAD_H_
#define _THREAD_H_
#include <windows.h>

/**
  @namespace Win 
  Reusable classes encapsulating the Win32 API.*/
namespace Win
{
    /**
     Thread class, encapsulates the Win32 API CreateThread() call */
    class Thread
    {
    public:

        /** 
         constructor, parameterized by the thread procedure and its
         argument, thread is created in a suspended state, so client  
         after must call Resume() after construction */
        Thread(DWORD(WINAPI *pFun) (void *arg), void *pArg)
        {
            _handle=::CreateThread(0, 0, pFun, pArg,
                                  CREATE_SUSPENDED, &_tid);
        }

        /** 
         destructor, closes the handle of the thread */
        ~Thread()
        { 
            ::CloseHandle(_handle);
        }

        /** 
         activates the thread from his suspended state */
        void Resume() 
        { 
            ::ResumeThread(_handle); 
        }

        void WaitForDeath()
        {
            ::WaitForSingleObject(_handle, 2000);
        }

        void SetPriority(int Priority)
        {
            ::SetThreadPriority(_handle, Priority);
        }

    private:
        //thread handle
        HANDLE _handle;

        //thread id
        DWORD _tid;
    };

    /**
     encapsulates a win32 event object */
    class Event
    {
    public:

        /* object is initialized in non-signaled state (red light) */
        Event()
        {
            _handle=::CreateEvent(0, TRUE, FALSE, 0);
        }

        ~Event()
        {
            ::CloseHandle(_handle);
        }

        /** put into signaled state */
        void GreenLight()
        { 
            ::SetEvent(_handle);
        }

        /** put into non-signaled state */
        void RedLight() 
        { 
            ::ResetEvent(_handle);
        }

        /** wait until event is in signaled (green) state */
        void Wait()
        {
            ::WaitForSingleObject(_handle, INFINITE);
        }

    private:
        HANDLE _handle;
    };
}
#endif //_THREAD_H_

