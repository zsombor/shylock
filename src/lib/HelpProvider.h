/**
 @file
 A simple html based help, based on win32 ShellExecute.
*/
#if !defined _HELPPROVIDER_H_
#define _HELPPROVIDER_H_
#include <shellapi.h>
#include <windows.h>

namespace Win
{
    /** provides HTML based help, 
      (opens a HTML file from the current directory)*/
    class HelpProvider  
    {
    public:
        /**
         @param hwnd handle to the application window
         @param pszHelpFileName the filename of document to open */
        static void Open(HWND hwnd, PSTR pszHelpFileName)
        {
            TCHAR _szDir[MAX_PATH];
            ::GetCurrentDirectory(sizeof(_szDir), _szDir);
            ::ShellExecute(hwnd, "open", pszHelpFileName
                           ,NULL, _szDir, SW_RESTORE);
        }
    };
}
#endif //_HELPPROVIDER_H_

