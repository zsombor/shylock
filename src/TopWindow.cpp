/**
 @file 
 The main dialog/window procedure.
*/

#include <exception>
#include "lib/Dialog.h" //for Dialog::CenterWindow()
#include "lib/MessageBox.h"
#include "TopWindow.h"
#include "Controller.h"
#include "resource.h"


using namespace Win;
using namespace Othello;

TopWindow::TopWindow(HINSTANCE hInst)
 : ModelessDialog(hInst, MAKEINTRESOURCE(DLG_MAIN), DLGPROC(AppDialogProc))
{}

BOOL CALLBACK TopWindow::AppDialogProc
    (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Controller *control=0;
    try
    {
        switch (message)
        {
        //this, should be the first message to arrive
        case WM_INITDIALOG: 
            //create a new Controller, be aware that constuction may throw
            control=new Controller(hwnd);
            Dialog::CenterWindow(hwnd);
            return TRUE;
        case WM_COMMAND:
            //this is command issued through the menu, act accordingly
            control->OnCommand(LOWORD(wParam), HIWORD(wParam));
            return TRUE;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return TRUE;
        case WM_PAINT:
            //refresh screen
            control->OnPaint();
            return TRUE;
        case WM_LBUTTONDOWN:
            //placing discs
            control->OnLButtonDown(LOWORD(lParam),HIWORD(lParam));
            return TRUE;
        case WM_CALC_DONE:
            //thinker has results
            control->OnCalcDone();
            return TRUE;
        case WM_UPDATE_VIEW:
            //thinker had partial results
            control->OnUpdateView();
            return TRUE;
        case WM_TIMER:
            //infoview update
            control->OnTimer(wParam);
            return TRUE;
        case WM_CLOSE:
            //cleanup
            delete control;
            control = 0;
            ::DestroyWindow(hwnd);
            return TRUE;
        }
    }
    catch(Win::Exception &e)
    {
        e.Show();
        ::PostQuitMessage(0);
        return TRUE;
    }
    catch(std::exception &e)
    {
        Win::MessageBox(0, e.what(), "Shylock", MB_ICONEXCLAMATION|MB_OK);
        ::PostQuitMessage(0);
        return TRUE;
    }
    catch(...)
    {
        Win::MessageBox(0, "Unknown Exception", "Shylock", 
                        MB_ICONEXCLAMATION|MB_OK);
        ::PostQuitMessage(0);
        return TRUE;
    }
    return FALSE;
}
