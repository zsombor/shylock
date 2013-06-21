/**
 @file
 Specs of classes for moveexecution.
*/
#if !defined _MOVEEXECUTER_H_
#define _MOVEEXECUTER_H_
#include "Boards.h"
#include "UndoData.h"

namespace Othello
{

    extern Board actualb;
    extern TTBoard actualbtt;
    
    /**
     class MoveExecuter, naive and do nothing style class */
    class MoveExecuter
    {
    public:
        static const int directions[];
        static const unsigned int dir_mask[10*10];
    protected:
        MoveExecuter(bool isblacksturn, int move)
            :_move(move),
             player(isblacksturn ? black : white),
             opponent(-player)

        {}
        int _move;
        const squarevalue player;
        const squarevalue opponent;
    };

    /**
     class FullMoveExecuter, executes a move creating the new board 
     configuration, a backup is also made, the destructor restores 
     the previous configuration. */
    class FullMoveExecuter: public MoveExecuter
    {
    public:
        /**
         executes move(pos), player is determined from isblacksturn flag of
         current board state. FullMoveExecuter used during search */ 
        FullMoveExecuter(int const pos, UndoData &udata, int dir=0);

        /** 
         undoes the move if undo info is avaible */
        ~FullMoveExecuter();

        /**
          mark this move as the best among it's brother's */
        void MarkAsBestMove()
        {
            _backupTTBoard.SetBestMove(_move);
        }
        
    private:

        static void BlackUpdateIndexPlace(int x);
        static void BlackUpdateIndexFlip(int x);

        static void WhiteUpdateIndexPlace(int x);
        static void WhiteUpdateIndexFlip(int x);

        void Undo();

        TTBoard  _backupTTBoard;
        unsigned int _hash32;
        int _backup_blaks;

        static TranspositionTable& _ttable;

        void UpdateUndoInfo(squarevalue &p);
        UndoData _udata;
    };

    

    /**
     class GuiMoveExecuter a  slow move executer without undo support */
    class GuiMoveExecuter: public MoveExecuter
    {
    public:
        GuiMoveExecuter(Board &board, int  const pos);
    };

    //DEBUG
    void PrintBoards();
}

#endif //_MOVEEXECUTER_H_

