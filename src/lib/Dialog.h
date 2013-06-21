/**
 @file
 Interface of the Win::Dialog base class.
*/
#if !defined _DIALOG_H_
#define _DIALOG_H_
#include <windows.h>

namespace Win
{
    /**
     class Dialog implements a generic Win32 (modal)dialog box, inherit 
     from this class to implement dialog boxes */
    class Dialog
    {
    public:

        /**
         use this to center a window in the parent window, if no such 
         exists then the window is centered on the desktop
         @param hwnd handle to the window to be centered */
        static void CenterWindow(HWND hwnd);

    protected:

        /**
         @param appInstance handle to the application instance
         @param templateName pointer to the dialog template used
         @param parent handle to the parent window */
        Dialog(HINSTANCE appInstance, 
               LPCSTR templateName, 
               HWND parent);

        virtual ~Dialog()
        {}

        /**
         must call this after creation in order to show the dialog
         this clumsy two step construction is necesary to allow the derived
         classes to do their initialization before showhing the dialog */
        void Show();

        /**
         override this to initialize dialog data 
         @param hwnd the handle of the dialog box, save this in _hwnd */
        virtual bool OnInitDialog(HWND hwnd);

        /**
         override this to handle command messages */
        virtual bool OnCommand(WORD command);

        /**
         dialog windows handler, this must be set in the overriden 
         OnInitDialog */
        HWND _hwnd;

    private:

        /**
         the WIN32 API DialogProc
        */
        static BOOL CALLBACK 
            DlgProc(HWND hwnd, UINT message, UINT wParam, LPARAM lParam);

        /** application instance */
        HINSTANCE _appInstance;

        /** the used dialog template name */
        LPCSTR _templateName;

        /** handle of parent windows */
        HWND _parent;
    };
}
#endif //_DIALOG_H_

