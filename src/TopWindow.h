/**
 @file
 Interface of the TopWindow class
*/
#if !defined _TOPWINDOW_H_
#define _TOPWINDOW_H_
#include "lib/ModelessDialog.h"

/**
 TopWindow class, implements the top window (dialog) class
*/
class TopWindow: public Win::ModelessDialog
{
public:
    /**
     @param hInst handle to the application instance
    */
    TopWindow(HINSTANCE hInst);

private:
    /**
     procedure to handle the application "window||dialog"
    */
    static BOOL CALLBACK AppDialogProc(HWND hwnd,
                                       UINT message,
                                       WPARAM wParam,
                                       LPARAM lParam);
};    
#endif //_TOPWINDOW_H_
