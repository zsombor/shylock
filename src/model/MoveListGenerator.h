/**
 @file
 Move generation related classes
*/
#if !defined _MOVELISTGENERATOR_H_
#define _MOVELISTGENERATOR_H_
#include <cassert>
#include <algorithm>
#include <functional>
#include <cstring>
#include "Boards.h"
#include "MoveExecuter.h"
#include "HHeuristic.h"
#include "TTable.h"

namespace Othello
{ 
    /**
     class MoveInfo stores move and their value based on History Heuristic*/
    class MoveInfo
    {
    public:
        /**
         the default construcor.
         NOTE: this doesn't initalize anyting, it is here only for
         semantic purposes (arrays need this type of constructor). */
        MoveInfo()
        {}

        /**
         a move on square X, with first flipping direction Dir 
         and value Val */
        MoveInfo(int X, int Dir ,unsigned int Val)
            :x(X),
             dir(Dir),
             value(Val)
        {}
    
        /**
         operator=, std::sort() calls this method (does he?) */
        MoveInfo& operator= (const MoveInfo& t)
        {
            x=t.x; dir=t.dir; value=t.value;
            return *this;
        }

        //not much of a privacy here!!!
        int x;              /**< the square where the new disc is placed */
        int dir;            /**< the first fliping direction of this move */  
        unsigned int value; /**< the history heuristic value of this move */
    };

    /**
     functor object used to compare two MoveInfos based on their value */
    class greather_MoveInfo: public 
         std::binary_function<const MoveInfo&, const MoveInfo&, bool>
    {
    public:
        bool operator() (const MoveInfo& x, const MoveInfo& y) 
        {
            return x.value > y.value; 
        }
    };

    
    //GRRRRRR
    enum 
    {
        MAXMOVES=60 
    }; 

    /**
      MyVector class, that stores MoveInfos.
     on some libraries std::vector<MoveInfo> caused trashing. */
    class MyVector
    {
    public:
        /**
         creates an empty vector */
        explicit MyVector()
          :_last(0)
        {}

        /**
         t is pushed back */
        void PushBack(const MoveInfo &t)
        {
            assert(_last+1<=MAXMOVES);
            _arr[_last]=t;
            _last++;
        }
    
        /**
         is vector empty? */
        bool Empty() const 
        {
            return _last==0;
        }
    
        /** 
         @return an iterator to the front of the vector */
        MoveInfo* Begin() 
        {
            return &_arr[0];
        }

        /**
         @return an iterator past the last element in the vector */
        MoveInfo* End() 
        {
            return &_arr[_last];
        }

        /**
         sorts the vector using std::sort() and 
         functor object greather_MoveInfo() */
        void Sort()
        {
            assert(_last!=0);
            std::sort(Begin(), End(), greather_MoveInfo());
            
            //paranoia
            assert(IsSorted(Begin()));
        }

        /**
          @return the size of the vetor (nr elements inserted) */
        unsigned int Size() const
        {
            return _last;
        }

        //DEBUG
        bool IsSorted(MoveInfo *first)
        {
            MoveInfo *i=first; MoveInfo *j=first;
            for(j++; i!=End() && j!=End(); j++, i++)
                if(i->value<j->value)
                    return false;
            return true;
        }

        //I like friends!
        friend class MoveListGenerator;

    private:
        void EraseAll()
        {
            _last=0;
        }

        /** fixed size array to store the vector */
        MoveInfo _arr[MAXMOVES];
    
        /** nr of elements inserted */
        unsigned int _last;
    };

    /**
     MoveSkipper class for avoiding a reasearch of the moves 
     resulted from the TTable or the killer move list.
     NOTE: my library had implemented std::bitset<100> in such fashion
     that it was impossible to avoid somewhat expensive runtime range 
     cheking. There is no need for such, because the debug build will
     protect with asserts. Otherwise it would be better to derive from 
     std::bitset<100> with protected inheritance, and provide a clean
     interface. */
    class MoveSkipper
    {
        enum{ size=4 }; //128 bits I need only 100

    public:
        /**
         initialize with all moves marked as not searched */
        MoveSkipper()
          :_valid(false)
        {
            //Make sure that we are building with 32 bit words
            assert(sizeof(_arr[0])==4);

            memset(_arr, 0, sizeof(_arr[0])*size);
        }

        /**
         mark a move as one that was already searched */
        void SkipMove(unsigned const move)
        {
            assert(move>=p11 && move<s89);
            _valid=true;
            _arr[move/32] |= 1 << (move % 32);
        }

        /**
         @return true only if this move is skipable  */
        bool WasSearched(unsigned const move) const
        {
            assert(move>=p11 && move<s89);
            return 0!=(_arr[move/32] & (1 << (move % 32)));
        }
    
        /**
         @return true only if we have skipable moves */
        bool HasMoves() const 
        {
            return _valid;
        }

    private:
        bool _valid; 
        unsigned long _arr[size]; 
    };


    /**
     IsLegalMove functor object, used to test move legalibility */
    class IsLegalMove: public std::binary_function<int ,int& , bool>
    {
    public:
        /**
         IsLegalMove operates preferably on actualb */
        explicit IsLegalMove(Board& b=actualb)
          :_player(b.isblacksturn? black : white),
           _opponent(-_player),
           _b(b)
        {}

        void ReInit()
        {
           _player = _b.isblacksturn ? black : white;
           _opponent = -_player;
        }
    
        /**
         test if move i is legal or not; 
         if yes the dir is the first flip direction */
        bool operator () (unsigned int i, int& dir);
    private:
        bool FlippableInDir(unsigned int i, int dx)
        {
            if(_b.table[i+dx]==_opponent)
            {
                i+=dx;
                if(_b.table[i+dx]==_opponent)
                {
                    i+=dx;
                    if(_b.table[i+dx]==_opponent)
                    {
                        i+=dx;
                        if(_b.table[i+dx]==_opponent)
                        {
                            i+=dx;
                            if(_b.table[i+dx]==_opponent)
                            {
                                i+=dx;
                                if(_b.table[i+dx]==_opponent)
                                {
                                    i+=dx;

                                }
                            }

                        }
                    }
                }
                if(_b.table[i+dx]==_player)
                    return true;
            }
            return false;
        }
    private:
        //the player who's is in turn, and his opponent
        squarevalue _player;
        squarevalue _opponent;

        //the board on which testing is done
        Board& _b;
    };

    /** interface for a collection of moves from position, with one 
      time pass iterator interface to the client. */
    class IMoveCollector
    {
    public:
        /** did we itareted trough all legal moves? */
        bool AtEnd()
        { 
            return _i==_movelist.End(); 
        }

        /** steps the move iterator */
        void Step() 
        { 
            _i++; 
        }

        /** do we have any legal moves at all?? */
        bool Empty() const 
        {
            return _movelist.Empty(); 
        }
            
        /** returns the current move */
        int X() const 
        { 
            return _i->x; 
        }

        /** returns the current move's first flip direction */
        int Dir() const 
        {
            return _i->dir; 
        }
    protected:
        /** the generated move list */
        MyVector _movelist;

        /** iterator for the move list */
        MoveInfo *_i;

    
        /** sort moves based on their expected value */
        void Sort() 
        { 
            _movelist.Sort();
        }

        IMoveCollector()
            :_i(_movelist.Begin())
        {}
    };

    /**
     MoveListGenerator class, generates a sorted movelist from current 
     board state, using the history heuristic. */
    class MoveListGenerator: public IMoveCollector
    {
    public:
        /** generator used by the worker thread */
        MoveListGenerator(int depth, bool RetrieveNotFailed);

        /** did we itareted trough all legal moves? */
        bool AtEnd()
        { 
            return (_phase>=bestmove_history_heuristic && _i==_movelist.End()); 
        }

        /** steps the move iterator */
        void Step() 
        { 
            assert(_phase > bestmove_transposition_table );
            switch(_phase)
            {
            case killer1:
                TryKiller1();
                break;
            case killer2:
                TryKiller2();
                break;
            case bestmove_history_heuristic:
                BuildSortedList();
                break;
            case rest:
                _i++;
            }
        }

        //DEBUG 
        void PrintList(); 
        //DEBUG
        bool ListOk(); 
        //DEBUG
        bool DBG(){ return _i!=_movelist.Begin(); } 

    private:
        /** 
          tries to obtain a move from the transposition table, if not 
          possible then uses TryKiller1() method */
        void TryTranspositionTable();

        /**
          tries to obtain a move from the killer move list, if failes then
          it will use the TryKiller2() method */
        void TryKiller1();

        /** 
          tries to obtain a legal move using the second killer move, if
          not possible then calls the BuildSortedList() member */
        void TryKiller2();
        
        /**
          builds the list of previously not tried moves, and sorts them 
          according to their history heuristic value */
        void BuildSortedList();

        /** 
          these are the phases used during move list generation*/
        enum Phase
        {
            bestmove_transposition_table,
            killer1,
            killer2,
            bestmove_history_heuristic,
            rest
        };
        /** current phase */
        int _phase;

        /** a move skipper object to avoid regenerating moves investigated
        in previous phases*/
        MoveSkipper _skip;

        static HistoryHeuristic& _hheuristic;
        static TranspositionTable& _ttable;
        IsLegalMove _legal;
        int _depth;
    };

    /** 
      enumerates all moves from a given board, GUI can use this to
      identify squares that must be highlighted. All possible moves
      are marked on the board with GUI hints.*/
    class MoveEnumerator: public IMoveCollector
    {
    public:
        MoveEnumerator(Board &board);
    };

    extern Board actualb;
    extern TTBoard actualbtt;
    extern void PrintBoards();
}

#endif //_MOVELISTGENERATOR_H_

