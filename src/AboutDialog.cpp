/**
 @file 
  Implements the Help/About menuitem.
*/
#include "AboutDialog.h"
#include "resource.h"

using namespace Win;

AboutDialog::AboutDialog(HINSTANCE appInstance, HWND parent)
  :Dialog(appInstance, MAKEINTRESOURCE(DLG_ABOUT), parent)
{
    Show();
}

bool AboutDialog::OnInitDialog(HWND hwnd)
{
    _hwnd=hwnd;
    
    //set about box text 
    HWND eHwnd=::GetDlgItem(_hwnd, IDC_ABOUT_TEXT);
    ::SendMessage(eHwnd, WM_SETTEXT, 0, LPARAM(CopyrightMessage));

    return TRUE;
}

//BAD THING
char const* const AboutDialog::CopyrightMessage=
"   The name Shylock was inspired by Shakespeare's play \
\"The Merchant of Venice\", the pair of \"Othello, the Moor\".\
\r\n\
    Shylock is fully object oriented, with an engine written in pure C++, \
a GUI built with a simple yet powerful OO encapsulation of the Win32 API \
(no MFC, no OWL, no crap). I've used the advanced concepts presented by \
Bartosz Milewski in his book \"C++ In Action. Industrial Strength \
Programming Techniques\". If you are interested visit www.relisoft.com, \
where you will also find an amazing Win32 tutorial.\
\r\n\
    Shylock's engine uses a self-learned evaluation function, \
a modern algorithm like MTD(f) invented by Askee Plaat. Move ordering is \
improved using killer moves, history heuristic, iterative deepening with a \
large transposition table. Also there is an opening book, containing all \
known variations. Shylock uses a wide variety of techniques, however \
this AboutBox can't take any more, so like it or not this is as much as \
you get (here).\
\r\n\
    Shylock was dreamt, designed and programmed by a true VIM maniac: Deé Zsombor (2001-2002) \
\r\n\r\n\
******************************************\r\n\
LEGAL STUFF:\r\n\
******************************************\r\n\
The author grants you a personal, limited, non-exclusive right to use this \
software program at your own risk. However you are not permitted to lease \
or rent, sublicense this software program or use it in any manner that may \
generate financial revenue. You may not enter this program in any organized \
tournament where it would compete with other programs or players. All \
exceptions must be dealt by separate mutually agreeable terms set forth in \
writing.\r\n\r\n\
OTHELLO is a registered trade mark of Tsukuda Original, licensed by Anjar Co.";
