/**
 @file
 Interface of the Win::Exception class.
*/
#if !defined _WINEXCEPTION_H_
#define _WINEXCEPTION_H_
#include <windows.h>

namespace Win
{
    /**
     Exception class, used to deal with system error conditions */
    class Exception
    {
    public:
        /**
         @param msg error message */
        Exception(char* msg)
          :_err(::GetLastError()),
           _msg(msg)
        {}
    
        /**
          @return errorcode retrieved with GetLastError Win32 API. */
        DWORD GetError() const 
        { 
            return _err; 
        }

        /**
         displays a nifty dialog box describing the error */
        void Show() const;

    private:
        DWORD _err;
        char* _msg;
        static char const msgFormat[];
    };
}
#endif // _WINEXCEPTION_H_

