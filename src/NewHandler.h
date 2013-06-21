/**
 * @file
 * Out of memory handlers for gcc 3.2 and mvc++
*/

#if defined _MSC_VER
#include <new.h>
#else
#include <new>
#endif

#include "lib/WinException.h"

/**
 * Registers the new out of memory handler, that throws Win::Exception
 */
class OutOfMemory
{
#if defined _MSC_VER
private:
    /**
     mcv++ "out of memory" handler
     @exception throws Win::Exception upon failure
    */
    static int NewHandler(size_t size)
    {
        throw Win::Exception("Out of memory");
        return 0; //dummy
    }
public:
    static void RegisterNewHandler()
    {
        _set_new_handler(&OutOfMemory::NewHandler);
    }
#else
private:
    /**
     "out of memory" handler
     @exception throws Win::Exception upon failure
    */
    static void NewHandler()
    {
        throw Win::Exception("Out of memory");
    }
public:
    static void RegisterNewHandler()
    {
        std::set_new_handler(&OutOfMemory::NewHandler);
    }
#endif
};
