/**
 @file
 Implements the Win::Exception class.
*/

#include "WinException.h"

using namespace Win;

char const Exception::msgFormat[]=
   "Problem: %s\noperating system tells us: %s";

void Exception::Show() const
{
    char *buf;
    char *sysMsg;

    ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL,
                    _err,
                    MAKELANGID(LANG_NEUTRAL, 
                    SUBLANG_DEFAULT), // Default language
                    (LPTSTR) &sysMsg,
                    0,
                    NULL);

    //???BUG???: what if this is an "out of memory" exception?
    buf = new char[strlen (sysMsg) + strlen (_msg) + strlen (msgFormat) + 1];
    
    wsprintf(buf, msgFormat, _msg, sysMsg);
    ::MessageBox(0, buf, "Error", MB_ICONEXCLAMATION|MB_OK);

    // Free the buffer.
    ::LocalFree (sysMsg);
    delete buf;
}