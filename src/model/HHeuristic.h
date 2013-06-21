/**
 @file
 the Othello::HistoryHeuristic class.
*/
#if !defined _HISTORYHEURISTIC_H_
#define _HISTORYHEURISTIC_H_
#include <cassert>
#include "Boards.h"

namespace Othello
{

    //not a killer move
    const int NOT_A_KMOVE=-1; 

    /**
     class HistoryHeuristic, a singelton implementing the History 
     Heuristic table, used to dinamicaly improve the move ordering 
     in the search tree. If a move is found to be good then it's
     value is increased with a value proportional with the depth
     of the search that proved this "goodnes". Moves that are
     found to be good in multiple lines of play are more promising.
     Also handles the killer table which consists of two moves 
     per player per depth which are to be considered first 
     before any other moves. Killers are overwriten. The killer 
     heuristic has a local perpecitve while the history heuristic 
     uses a wide perpective over the search tree. */
    class HistoryHeuristic
    {
    public:
        /**
          unique access point */
        static HistoryHeuristic& Instance();

        /**
         inserts a move into the table
         @param x move to be inserted
         @param isblacksmove true only if this is black's move
         @param depth from where this move was found to be good */
        void GoodMove(int x, bool isblacksmove, unsigned int depth)
        {
            assert(TTBoard::Persil[x]!=-1);
            
            //update history heuristic
            _tb[x][isblacksmove] += (1 << depth);

            //update killer list
            if(_kmoves[depth][isblacksmove].move1!=x)
            {
                _kmoves[depth][isblacksmove].move2=
                    _kmoves[depth][isblacksmove].move1;
                _kmoves[depth][isblacksmove].move1=x;
            }
        }
    
        /**
         @param x is the move
         @param isblacksmove true only if this is black's move
         @return the move's history heuristic value */
        unsigned int MoveValue(int x, bool isblacksmove)
        {
            assert(TTBoard::Persil[x]!=-1);
            return _tb[x][isblacksmove];
        }

        /**
         @param isblacksmove true only if this is black's move  
         @param depth from where we want the killer
         @return the first killer move, or NOT_A_KMOVE if none is found */
        int KillerMove1(bool isblacksmove, unsigned int depth) const
        {
            return _kmoves[depth][isblacksmove].move1;
        }

        /**
         @param isblacksmove true only if this is black's move  
         @param depth from where we want the killer
         @return the second killer move, or NOT_A_KMOVE if none is found */
        int KillerMove2(bool isblacksmove, unsigned int depth) const
        {
            return _kmoves[depth][isblacksmove].move2;
        }

        /**
         cleans the history heuristic tables */
        void Reset();

    private:
        explicit HistoryHeuristic()
        {
            Reset();
        }
        unsigned int _tb[10*10][2];

        /**
         this an entry an entry to the killer move list
         containing two killers */
        struct KillerMoves
        {
            int move1;
            int move2;
        };

        /**
         this is the actual killer move list */
        KillerMoves _kmoves[32][2];
    };
}

#endif //_HISTORYHEURISTIC_H_

