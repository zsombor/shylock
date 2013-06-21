/**
 @file
 The WinMain() function.
*/

#include <exception>
#include <windows.h>
#include "lib/WinException.h"
#include "lib/MessageLoop.h"
#include "lib/Accelerator.h"
#include "lib/MessageBox.h"
#include "NewHandler.h"
#include "TopWindow.h"
#include "resource.h"

using namespace Win;


/** application instance */
HINSTANCE TheInstance=0;


int WINAPI WinMain
   (HINSTANCE hInst, HINSTANCE hPrevInst, char * cmdParam, int cmdShow)
{
    try
    {
        //initialize application instance
        TheInstance=hInst;
        
        //register new out of memory handler, throwing Win::Exception
        OutOfMemory::RegisterNewHandler();

        //create main "window||dialog"
        TopWindow topWindow(hInst);
    
        //load the accelerator table
        Accelerator accel(hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));

        //our message loop, uses keyboard accelerators
        MessageLoop msgloop(topWindow, accel);

        //now run the message loop
        return msgloop.Run();
    }
    catch(Win::Exception &e)
    {
        e.Show();
    }
    catch(...)
    {
        Win::MessageBox(0, "Unknown Exception", "Shylock",
                        MB_ICONEXCLAMATION|MB_OK);
    }
    return 0;
}
