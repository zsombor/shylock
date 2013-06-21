/**
 @file
 The interface of the two View like classes from the MVC pattern, BoardView
 and InfoView.
*/
#if !defined _VIEWS_H_
#define _VIEWS_H_
#include <windows.h>
#include "lib/Canvas.h"
#include "lib/Tools.h"
#include "lib/SimpleControl.h"
#include "lib/Bitmap.h"
#include "model/Boards.h"
#include "model/Model.h"
#include "resource.h"


/** 
 class BoardView is a control showing the user's view over the 
 Othello board.
*/
class BoardView: public Win::SimpleControl
{
public:
    /**
     @param parentHWND handle to the parent window
    */
    BoardView(HWND parentHWND);
    
    /** 
     response to WM_PAINT 
     @param canvas upon which the painting is made
    */
    void Paint(Win::Canvas &canvas);

    /** 
     translates pixel coordinate into game coordinate ranging from 
     (0,0) to (7,7).
    */
    void PixelToBoard(POINT &point);

    /** 
     places a piece of suitable color at given game coordinate
     @return true only if succesful
    */
    bool PutPiece(int x,int y, Othello::squarevalue player);
    
    /**
     get new board to display 
    */
    void Get(Othello::Board &board);

    /** 
     sends the board resulted from the user's move
    */
    void SendTo(Othello::Board &board);

    bool IsGameOver();

    bool IsStillUserTurn() const 
    {
        return _double_move;
    }

    /**
     initializes the control with the start position
    */
    void InitBoards();

    /**
     saves the board state to the file with given filename
    */
    void SaveToFile(PSTR filename);

    /**
     loads into a position tmp, from a file with given name 
     returns false if syntax error
    */
    bool LoadFromFile(PSTR filename, Othello::Board &tmp);

    /**
     highlight the hinted move
    */
    void Hinted(int move);

private:
    
    /**
     draws the view dependent on the board state, onto the given 
     Canvas
    */
    void Init(Win::Canvas &canvas);

    /** 
     erase all gui helper trash from board 
    */
    void Clean();

    /** brush to possible squares */
    Win::Brush  _possbrush;
    /** brush to draw empty squares */
    Win::Brush  _emptybrush;
    /**brush to draw hinted squares */
    Win::Brush  _hintbrush;

    // brushes,pens to draw pieces 
    Win::Brush _bpiece1;
    Win::Pen _ppiece1;        
    Win::Brush _bpiece2;
    Win::Pen _ppiece2;

    /** board configuration buffer */   
    Othello::Board _board;

    /** flag to mark game over state */
    bool _game_over;

    /** flag to mark a double move from user part */
    bool _double_move;

    /** a Bitmap used for double buffering */
    Win::Bitmap _buffer;

};


/**
 class BoardView is a control showing the user's view over the 
 info panel
*/
class InfoView: public Win::SimpleControl
{
public:
    InfoView(HWND parentHWND, bool &isUsersTurn, bool &isGameStarted, 
             bool &isUserBlack, bool &isGameOver);

    /** response to WM_PAINT */
    void Paint(Win::Canvas &canvas);

    /** update info status */
    void LookInside(Othello::Thinker &model);

    /** reset internal data (do this if we have a new game) */
    void Clean();

    /** saves the game record (moves made) to a file given by its name */ 
    void SaveGameToFile(PSTR filename);

    /** update the buffer content */
    void UpdateBuffer();

private:

    //standard messages
    static char *_msgShylockIsThinking;
    static char *_msgYourTurn;
    static char *_msgBestMove;
    static char *_msgDepth;
    static char *_msgGameValue;
    static char *_msgNodes;
    static char *_msgTTHits;
    static char *_msgMoves;
    static char *_msgTime;
    static char *_msgTimedOut;
    static char *_msgGameOver;
    static char *_msgFromBook;

    /** current game value */
    int _gamevalue;

    /** expected best move */
    int _bestMove;

    //gues what
    bool &_isUsersTurn;
    bool &_isGameStarted;
    bool &_isUserBlack;
    bool &_isGameOver;
    unsigned int _mseconds;
    bool _inBook;

    /** history */
    Othello::Move const *_moves;
    /** nr entrys into history @see _moves */
    int	*_pnrmoves;
    
    /** positions serched by Model */
    unsigned int _posSearched;
        
    //hits in transposition table hits
    unsigned int _ttHits;
    unsigned int _ttGHits;

    /** depth where Model is searching */
    int _depth;

    /** is Model in time out condition */
    bool _timedOut;

    /** a Bitmap used for double buffering */
    Win::Bitmap _buffer;

    /** square names, lazzy style */
    static char* SquareNames[10*10];
};
#endif //_VIEWS_H_
