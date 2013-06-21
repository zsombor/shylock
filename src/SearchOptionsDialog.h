/**
 @file
 Interface of the class implementing a dialog for modifieng some search settings
*/
#if !defined _SEARCHOPTIONSDIALOG_H_
#define _SEARCHOPTIONSDIALOG_H_
#include "lib/Dialog.h"
#include "lib/Spinner.h"
#include "resource.h"


/**
 implements "the Options/Search Options" dialog
*/
class SearchOptionsDialog: public Win::Dialog
{
public:
    /**
     @param appInstance handle to the application instance
     @param parent handle to the parent window
     @param LogSize the current size of the TT in 2^x form
     @param Minutes the current timelimit for a game
    */
    SearchOptionsDialog(HINSTANCE appInstance, HWND parent,
                        unsigned int LogSize, unsigned  int Minutes);


    /**
     @return true only if the OK button was pressed
    */
    bool IsOK() const
    {
        return _status==IDOK;
    }

    /**
      @return value read from the Size editbox
    */
    unsigned int LogSize() const
    {
        return _logsize;
    }

    /**
      @return value read from the Time editbox
    */
    unsigned int Minutes() const
    {
        return _minutes;
    }

private:
    /**
     automatically called on initialization
    */
    bool OnInitDialog(HWND hwnd);

    /**
     automatically called on command message
    */
    bool OnCommand(WORD command);

    /**
     saves the content of the dialog into member variables
    */
    void SaveEdit();

    unsigned int _logsize;
    unsigned int _minutes;
    int _status;
    Win::Spinner _spin_size;
    Win::Spinner _spin_minutes;
};
#endif //_SEARCHOPTIONSDIALOG_H_

