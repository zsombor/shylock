/**
 @file 
 The code for the Controller object from the Model-View-Controller pattern,
 this is where all user input is accepted and interpreted. 
*/

#include "Controller.h"
#include "resource.h"
#include "lib/HelpProvider.h"
#include "lib/SaveFileDialog.h"
#include "lib/OpenFileDialog.h"
#include "lib/WinException.h"
#include "lib/MessageBox.h"
#include "AboutDialog.h"
#include "SearchOptionsDialog.h"

using namespace Win;
using namespace Othello;

//export from Main.cpp
extern HINSTANCE TheInstance;

Controller::Controller (HWND hwnd)
    : _hwnd(hwnd),
      _isUserTurn(false),
      _isUserBlack(false),
      _isGameStarted(false),
      _isGameOver(false),
      _minutesperside(DEF_MINUTES_PER_SIDE),
      _thinker(hwnd),
      _boardview(hwnd),
      _infoview(hwnd, _isUserTurn, _isGameStarted, _isUserBlack, _isGameOver),
      _timer(hwnd, 1) 
{
    // Attach icon to main dialog
    HICON hIcon=::LoadIcon(TheInstance, MAKEINTRESOURCE(IDI_ICON));
    ::SendMessage(hwnd, WM_SETICON, WPARAM(TRUE), LPARAM(hIcon));
    hIcon=::LoadIcon(TheInstance, MAKEINTRESOURCE(IDI_ICON));
    ::SendMessage(hwnd, WM_SETICON, WPARAM(FALSE), LPARAM(hIcon));


    _thinker.SetTotalTime(_minutesperside);
    _infoview.LookInside(_thinker);
    _infoview.UpdateBuffer();
    InvalidateViews(); 
    MenuStatusUpdate();
    //start think thread
    _thinker.Resume();
}


void Controller::OnCommand(int controlID, int command)
{
    switch (controlID)
    {
        case ID_GAME_NEW:       
            //start new game
            OnNew();
            break;
        case ID_GAME_ABORT:
            OnGameAbort();
            break;
        case ID_GAME_SAVEPOSITION:
            //save positon to a file
            OnSavePosition();
            break;
        case ID_GAME_SAVEGAME:
            OnSaveGame();
            break;
        case ID_GAME_LOADPOSITION:
            OnLoadPosition();
            break;
        case ID_GAME_EXIT:      
            //leave program
            OnExit();
            break;
        case ID_ACTIONS_HINT:
            //get a hint
            OnActionsHint();
            break;
        case ID_ACTIONS_SWITCHSIDES:
            OnActionsSwitchSides();
            break;
        case ID_ACTIONS_UNDO:
            OnActionsUndo();
            break;
        case ID_ACTIONS_FORCEMOVE:
            OnActionsForceMove();
            break;
        case ID_OPTIONS_SEARCH:
            OnOptionsSearch();
            break;
        case ID_HELP_CONTENTS:     
            OnHelpContents();
            break;
        case ID_HELP_ABOUT:
            OnHelpAbout();
            break;
    }    
}

Controller::MenuItem Controller::MenuStats[]=
{
    {ID_OPTIONS_SEARCH, &Controller::CanOptionsSearch},
    {ID_GAME_ABORT, &Controller::CanGameAbort},
    {ID_ACTIONS_HINT, &Controller::CanActionsHint},
    {ID_ACTIONS_UNDO, &Controller::CanActionsUndo},
    {ID_ACTIONS_SWITCHSIDES, &Controller::CanActionsSwitchSides},
    {ID_ACTIONS_FORCEMOVE, &Controller::CanActionsForceMove},
    {0, 0}
};

void Controller::OnLButtonDown(int x,int y)
{
    if(!_isGameStarted)
        return;
    if(_isUserTurn)
    {
        POINT point;
        point.x=x; point.y=y;
        HWND hwndBoard=_boardview.Hwnd();
        if(hwndBoard!=::ChildWindowFromPoint(_hwnd, point))
            return;

        ::MapWindowPoints(_hwnd, hwndBoard, &point,1);

        if(point.x<0) //patching a strange winbug.
            point.x=0; 
        if(point.y<0) //same thing.
            point.y=0; 
        _boardview.PixelToBoard(point);
        if(_boardview.PutPiece(point.x,point.y,_isUserBlack?black:white))
        {
            _thinker.AddMove(TTBoard::AntiPersil[(point.x-1)*8+point.y-1], _isUserBlack);
            if(!_boardview.IsStillUserTurn() && !_boardview.IsGameOver())
            {
                _boardview.SendTo(_thinker.newBoard);
                _infoview.UpdateBuffer();
                _isUserTurn=false;
                InvalidateViews(); 
                MenuStatusUpdate();
                _thinker.Go();
                return;
            }
            if(_boardview.IsStillUserTurn())
            {
                _infoview.UpdateBuffer();
                _isUserTurn=true; 
                InvalidateViews(); 
                MenuStatusUpdate();
                return;
            }
            if(_boardview.IsGameOver())
            {
                _thinker.Stop(); //!!!!!!!!!!!!!!!!!!!!1
                _isGameOver=true;
                _isGameStarted=false;
                _infoview.UpdateBuffer();
                MenuStatusUpdate();
                InvalidateViews(); 
                Win::MessageBox(_hwnd,"Game Over", "Shylock");
                _timer.Kill(); 
                _thinker.ResetGame();
                return;
            }
        }
    }
}

void Controller::OnTimer(int id)
{
    _timer.Kill();
    _thinker.CheckTime();
    //TODO change this protocol
    _infoview.LookInside(_thinker);
    _infoview.UpdateBuffer();
    _infoview.InvalidateOnParent(); //::InvalidateRect(_hwnd, 0, FALSE);
    _timer.Set(1000);
}

void Controller::OnUpdateView()
{
    //HEY I'm making more than one move
    BeepAndFlashWindow();    
    _boardview.Get(_thinker.newBoard);
    _infoview.UpdateBuffer();
    InvalidateViews(); 
}

void Controller::OnCalcDone()
{
    _boardview.Get(_thinker.newBoard);
    _isUserTurn=true;
    BeepAndFlashWindow();
    MenuStatusUpdate();
    if(_boardview.IsGameOver())
    {
        _thinker.Stop();
        _isGameStarted=false;
        _isGameOver=true; 
        _infoview.UpdateBuffer();
        MenuStatusUpdate();
        _timer.Kill();
        _thinker.ResetGame();
        InvalidateViews(); 
        Win::MessageBox(_hwnd, "Game Over", "Shylock");
        return;
    }
    _infoview.UpdateBuffer();
    InvalidateViews(); 
    //say: GO!
    //NOTE disabled for debuging purposes
    //_thinker.Go();
}

void Controller::OnNew()
{
    if(_isGameStarted)
    {
        Win::MessageBox mbox(_hwnd, "Do you wish to abort current game?",
                        "ShyLock", MB_YESNO|MB_DEFBUTTON2);
        if(mbox.Status()==IDNO)
            return;
    }
    _thinker.Stop();
    _timer.Kill(); //??? what happens if it is not started?
    _boardview.InitBoards();
    _infoview.Clean();
    _thinker.SetTotalTime(_minutesperside);

    InvalidateViews(); //entire window

    Win::MessageBox mbox(_hwnd, "Do you wish to play Black?",
                    "ShyLock",MB_YESNO);
    _isUserBlack=(IDYES==mbox.Status());
    _isUserTurn=_isUserBlack;
    _isGameStarted=true;
    _isGameOver=false;
    MenuStatusUpdate();
    _timer.Set(1000);
    _thinker.ResetGame();
    //say: GO!
    if(!_isUserTurn)  
        _thinker.Go();
}


void Controller::OnLoadPosition()
{
    OpenFileDialog opener(_hwnd);
    PSTR filename=opener.GetFileName();
    if(filename==NULL)
        return;

    Board tmp;
    if(!_boardview.LoadFromFile(filename,tmp))
    {
        Win::MessageBox(_hwnd, "Syntax failure");
        return;
    }

    if(_isGameStarted)
    {
        Win::MessageBox mbox(_hwnd, "Do you wish to abort current game?",
                        "ShyLock", MB_YESNO|MB_DEFBUTTON2);
        if(IDNO==mbox.Status())
            return;
    }

    _isGameOver=false;

    _timer.Kill(); //??? what happens if it is not started?
    _thinker.Stop();
    _boardview.Get(tmp);
    _boardview.SendTo(_thinker.newBoard);

    InvalidateViews(); //entire window
    
    Win::MessageBox mbox(_hwnd, "Do you wish to play Black?",
                    "ShyLock", MB_YESNO);

    _isUserBlack=(IDYES==mbox.Status());
    
    _isUserTurn=(_isUserBlack==tmp.isblacksturn);
    _timer.Set(1000);
    _isGameStarted=true;
    MenuStatusUpdate();

    //say: GO!
    if(!_isUserTurn)
        _thinker.Go();
    _infoview.Clean();
    return;
}


void Controller::OnActionsSwitchSides()
{
    _timer.Kill();
    //TODO add _thinker.Stop here, background thinking is done
    _isUserBlack=!_isUserBlack;
    _isUserTurn=!_isUserTurn;
    _thinker.Stop();
    _boardview.SendTo(_thinker.newBoard);
    InvalidateViews(); 
    MenuStatusUpdate();
    _timer.Set(1000);
    //say: GO!
    if(!_isUserTurn) 
        _thinker.Go();
}


void Controller::MenuStatusUpdate()
{
    HMENU  myMenu=::GetMenu(_hwnd);
    for(int i=0; MenuStats[i].CanBeEnabled!=0; i++)
    {
        bool (Controller::*pmem)() const = MenuStats[i].CanBeEnabled; 
        if( (this->*pmem)() )
            ::EnableMenuItem(myMenu, MenuStats[i].id, MF_ENABLED);
        else
            ::EnableMenuItem(myMenu, MenuStats[i].id, MF_GRAYED);
    }
}

void Controller::OnPaint()
{
    PaintCanvas canvasAll(_hwnd);
    if(_boardview.IsInsideRect(canvasAll.PaintRect()))
    {
        PaintCanvas canvasBoard(_boardview.Hwnd());
        _boardview.Paint(canvasBoard);
    }
    if(_infoview.IsInsideRect(canvasAll.PaintRect()))
    {
        PaintCanvas canvasInfo(_infoview.Hwnd());
        _infoview.Paint(canvasInfo);
    }
}

bool Controller::CanOptionsSearch() const
{
    return !_isGameStarted;
}

bool Controller::CanGameAbort() const
{
    return _isGameStarted;
}

bool Controller::CanActionsHint() const
{
    return _isGameStarted && _isUserTurn;
}

bool Controller::CanActionsUndo() const
{
    return _isGameStarted;
}

bool Controller::CanActionsSwitchSides() const
{
    return _isGameStarted && _isUserTurn;
}

bool Controller::CanActionsForceMove() const
{
    return _isGameStarted && !_isUserTurn;
}

void Controller::OnHelpContents()
{
    HelpProvider::Open(_hwnd, "Usage.htm");
}

void Controller::OnSaveGame()
{
    SaveFileDialog saver(_hwnd,"gam");
    PSTR filename=saver.GetFileName();
    if(filename!=NULL)
        _infoview.SaveGameToFile(filename);
}

void Controller::OnSavePosition()
{
    SaveFileDialog saver(_hwnd,"pos");
    PSTR filename=saver.GetFileName();
    if(filename!=NULL)
        _boardview.SaveToFile(filename);
}

void Controller::OnOptionsSearch()
{    
    SearchOptionsDialog dialog(TheInstance, _hwnd, 
                               _thinker.TTLogSize(),
                               _minutesperside);

    //if OK was pressed and there was a change
    if(dialog.IsOK() && 
        (dialog.LogSize()!=_thinker.TTLogSize() ||
         dialog.Minutes()!=_minutesperside))
    {
        if(dialog.LogSize()<17)
        {
            Win::MessageBox(_hwnd, "Less then 1 Mb is by far not enough, I'll just ignore that.",
                            "Why are you so cheap?", MB_ICONQUESTION|MB_OK);
            return;
        }
        if(dialog.Minutes()<1)
        {
            Win::MessageBox(_hwnd, "Less then 1 minute is by far not fair, since there is no time limit for your thinking. I'll just ignore that.",
                           "Why are you so cheap?", MB_ICONQUESTION|MB_OK);
            return;
        }
        _thinker.TTChangeSize(dialog.LogSize());
        _minutesperside=dialog.Minutes();
        _thinker.SetTotalTime(_minutesperside);
    }      
}


void Controller::OnHelpAbout()            
{
    //display ABOUT dialogbox            
    AboutDialog dialog(TheInstance,_hwnd);
}

void Controller::OnGameAbort()
{
    assert(_isGameStarted);
    Win::MessageBox mbox(_hwnd, "Abort this game.",
                   "Confirm:", MB_YESNO|MB_ICONINFORMATION);

    if(IDNO==mbox.Status())
        return;
    _timer.Kill(); //??? what happens if it is not started?
    _thinker.Stop();
    _isGameStarted=false;
    MenuStatusUpdate();
}

void Controller::OnExit()
{
    //No "save xxx" nag message, simply exit
    ::PostMessage(_hwnd, WM_CLOSE, 0, 0);
}


void Controller::InvalidateViews(BOOL berase)
{
    _boardview.InvalidateOnParent(berase);
    _infoview.InvalidateOnParent(berase);
}


void Controller::BeepAndFlashWindow()
{
    ::MessageBeep(0xffffffff);
    ::FlashWindow(_hwnd, FALSE);
}

void Controller::OnActionsHint()
{
    assert(_isUserTurn);
    //TODO add _thinker.Stop here, background thinking is done
    int bmove=_thinker.Hint();
    if(bmove!=-1)
        _boardview.Hinted(bmove);
    else
    {
        Win::MessageBox(_hwnd, "No hint avaible!", "Sory ...");
    }   
}

void Controller::OnActionsUndo()
{
    Win::MessageBox(_hwnd, "Not yet implemented");
}


void Controller::OnActionsForceMove()
{
    //Win::MessageBox(_hwnd, "Not yet implemented");
    _thinker.Force_Stop();
}
