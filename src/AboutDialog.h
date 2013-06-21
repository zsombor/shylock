/**
 @file
 Interface of AboutDialog class.
*/

#if !defined _ABOUTDIALOG_H_
#define _ABOUTDIALOG_H_
#include "lib/Dialog.h"

/** 
   Implements the about dialog box.
 */
class AboutDialog: public Win::Dialog
{
public:
    /**
        Constructor
        @param appInstance is a handle to the application instance
        @param parent is the handle of the parent window
    */
    AboutDialog(HINSTANCE appInstance, HWND parent);
private:

    /**
       the text of the about box, perhaps this should be
       read from a file.
    */
    static char const * const CopyrightMessage;

    /**
      this method is automatically called upon initialization with the
      dialog handle transmited on the parameter hwnd
      @param hwnd a handle to the dialog window.
      @see Win::Dialog
    */
    bool OnInitDialog(HWND hwnd);
};
#endif //_ABOUTDIALOG_H_

