/**
 @file
 The interface of the Controller class.
*/
#if !defined _CONTROLLER_H_
#define _CONTROLLER_H_
#include <windows.h>
#include "lib/Timer.h"
#include "model/Model.h"
#include "Views.h"


/**
 class Controller, from the Model-View-Controller pattern
 The user controls the application through a Controller, who has 
 the resposibility to react on each (win32) message accordingly. 
*/
class Controller
{
public:

    //TODO: read this from the registry
    enum 
    {
        DEF_MINUTES_PER_SIDE=10
    };

    /**
     @param hwnd handle to window where the messages will arrive 
    */
    Controller(HWND hwnd);

    /**
     clean up
    */
    ~Controller()
    {
        _timer.Kill();
        _thinker.Stop();
    }

    /** WM_COMMAND message handler */
    void OnCommand(int controlID, int command);

    /** WM_PAINT message handler */
    void OnPaint();
    
    /** WM_LBUTTONDOWN message handler */
    void OnLButtonDown(int x,int y);
    
    /** WM_CALC_DONE message handler */
    void OnCalcDone();

    /** WM_UPDATE_VIEW message handler */
    void OnUpdateView();

    /** WM_TIMER message handler */
    void OnTimer(int id);
private:

    /** "Game/New" menu message handler */
    void OnNew();

    /** "Game/Abort" menu message handler */
    void OnGameAbort();
    
    /** "Game/Save Position" menu message handler */
    void OnSavePosition();
    
    /** "Game/Load Position" menu message handler */
    void OnLoadPosition();

    /** "Game/Save Game" menu message handler */
    void OnSaveGame();

public:
    /** "Game/Exit" menu message handler */
    void OnExit();
private:

    /** "Actions/Hint" menu message handler */
    void OnActionsHint();

    /** "Actions/Switch sides" menu message handler */
    void OnActionsSwitchSides();

    /** "Actions/Undo" menu message handler */
    void OnActionsUndo();

    /** "Actions/Force move" menu message handler */
    void OnActionsForceMove();


    /** "Options/Search settings" menu message handler */
    void OnOptionsSearch();
    
    /** "Help/Contents" menu message handler */
    void OnHelpContents();

    /** "Help/About" menu message handler */
    void OnHelpAbout();

    /** enable/disable menu items depending on _isGameStarted and _isUserTurn; */
    void MenuStatusUpdate();

    /** 
     invalidates all Views 
     @param berase true only if background is to be erased too; default is false
    */
    void InvalidateViews(BOOL berase=FALSE);

    /** beeps and flashes the window */
    void BeepAndFlashWindow();

    /** application hwnd */
    HWND _hwnd;

    //gues what?
    bool _isUserTurn;
    bool _isUserBlack;
    bool _isGameStarted;
    bool _isGameOver;
    unsigned int _minutesperside;

    /** Model running on a separate thread */
    Othello::Thinker _thinker;

    /** the View over the board */
    BoardView _boardview;

    /** the View over the info panel */
    InfoView _infoview;

    /* timer to refresh @see _infoview */
    Win::Timer _timer; 
    
    //menu status functions
    bool CanOptionsSearch() const;
    bool CanGameAbort() const;
    bool CanActionsHint() const;
    bool CanActionsUndo() const;
    bool CanActionsSwitchSides() const;
    bool CanActionsForceMove() const;
    
    /**
     a menu entry, with a command id and a pointer to a memberfunction
     to query menustatus enabled/disabled based on the Controller's state
    */
    struct MenuItem
    {
        UINT id;
        bool (Controller::*CanBeEnabled)() const;
    };
    
    static MenuItem MenuStats[];    
};

#endif //_CONTROLLER_H_

