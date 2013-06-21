/** 
 @file
 Definition of the Othello::Thinker that plays the Model's role from the Model-View-Controller pattern.
*/
#if !defined _MODEL_H_
#define _MODEL_H_
#include "../lib/Active.h"
#include "TTable.h"
#include "HHeuristic.h"
#include "Evaluator.h"
#include "MoveListGenerator.h"
#include "TimeAllocator.h"
#include "Book.h"
#include "Endgame.h"


/**
  @namespace Othello All Othello related stuff is found here.*/
namespace Othello
{
    /** 
     a move record */
    struct Move
    {
       int x;          /**< move position */
       bool isblack;   /**< true only if this is black's move */
    };


    /**
     maximum searcheable depth */
    const unsigned int MAXDEPTH=30;

    /**
     stop signals */
    enum StopType
    {
        noStop,    /**< no stop, carry on */
        timeOut,   /**< time out, stop search and execute current best move*/
        forceMove, /**< user had lost it's patience,stop search execute and execute current best move, */
        guiBreak   /**< some kind of gui break */
    };


    /**
     Class Thinker (the model) derived from ActiveObject, provides 
     background thinking. This is usefull because the user can 
     interact with the GUI without disturbing the thought process.*/
    class Thinker: public Win::ActiveObject
    {
    public:
        /**
         @param hwnd handle to the window to whom we transmit the results*/
        Thinker(HWND hwnd);

        ~Thinker();

        /** 
         resets for a new game*/
        void ResetGame();
    
        /**
         Go, think!*/
        void Go();

        /** 
         Stop thinking!*/
        void Stop();

        /**
         add a new move to the game record*/
        void AddMove(int x,bool isblack)
        {
            moves[nrMoves].x=x;
            moves[nrMoves].isblack=isblack;
            nrMoves++;
        }

        /** 
         change transposition table size*/
        void TTChangeSize(unsigned int logsize);

        /** 
         how large is our transposition table?*/
        unsigned int TTLogSize();

        /**
         set time limit for the game*/
        void SetTotalTime(unsigned int min_per_game);

        /**
         how many miliseconds seconds are left?*/
        unsigned int MSecRemaining() ;

        /**
         get a hint about opponent's best move (GUI only)*/
        int Hint();
        
        /**
         for GUI only*/
        void Force_Stop();

        bool OutOfBook() const
        {
            return _book.OutOfBook();
        }

//not much of a privacy here!

        //board to send to gui
        Board newBoard;
        int bestMove;
        int gamevalue;
        unsigned int posSearched;
        unsigned int ttHits;
        unsigned int ttGHits;
        bool timedout;
        unsigned int IdDepth; 
        Move moves[60];
        int nrMoves;

        /**
         for gui (interupts the thought process if time is out)*/
        void CheckTime();
        
    private:
        void Run();
        void FlushThread();
        void InitThread();
        void Think();

        /**
         initializes boards before search*/
        void InitBoards();
    
        /** 
         the AlphaBeta search
         @exception throws STOP if a stop condition is encountered.
         @see StopType */
        int AlphaBeta(int alpha, int beta, unsigned int depth);

        /**
         mtd(f) search*/
        int MTD(int f, int depth);

        /** 
          send search results*/
        void DispatchResultsToGui();

        /**
         sets bestMove according to a simple one ply search*/
        int NaiveBestMoveSearch();

        /**handle to the window implementing the GUI, (to whom
        we send our messages)*/
        HWND _hwnd;

        /** provides semaphore services */
        Win::Event _event;

        /** variable to mark an stop conditions */
        StopType _stop;

        /** static evaluator, used in the search */
        Evaluator evaluator;

#if defined _ENDGAME_TEST_
        /** object to handle endgame */
        EndGame endgame;
#endif
        /** amount of time available for ShyLock */
        unsigned int _avaible_time;

        /** amount of time available for ShyLock at begining */
        unsigned int _init_totaltime;
 
        /** schedules the search */
        TimeAllocator _timescheduler;

        //Use the SOURCE, Luke!
        int _lastbestMove;
        bool _isdoublemove;
        bool _thinking;
        static int deep_enough[];

        /** the opening book */
        Book _book;

        /** four starting moves */
        static squareindex firstmoves[];

        static HistoryHeuristic &_hheuristic;
        static TranspositionTable &_ttable;
    };

    /** 
      all WM_USER messages sent to Controller, have values 
      greather or equal than _BASE_. */
    UINT const _BASE_ = 0x8dee;
    
    /**
      WM_USER message sent to Controller when Thinker had 
      finished the analisys  */
    UINT const WM_CALC_DONE = _BASE_ + 0;

    /**
      WM_USER message sent to Controller when Thinker had 
      made a double move, so an BoardView update is necesary.*/
    UINT const WM_UPDATE_VIEW = _BASE_ + 1;


    /**
     STOP interupt class */
    class STOP
    {
    public:
        /**
         @param s the reason why we have a stop */
        STOP(StopType s)
          :_s(s)
        {
            assert(_s!=noStop);
        }

        bool IsForceMove() const
        {
            return _s==forceMove;
        }

        bool IsTimeOut() const
        {
            return _s==timeOut;
        }

        bool IsGuiBreak() const
        {
            return _s==guiBreak;
        }
        
    private:
        StopType _s;
    };
    
    //DEBUG
    unsigned int Count(squarevalue p);
}

#endif //_MODEL_H_

