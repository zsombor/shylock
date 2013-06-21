/**
 @file 
 Implements the code to generate movelists sorted by goodness, and other
 move generation related stuff.
*/

#include "MoveListGenerator.h"

using namespace Othello;

//MoveListGenerator for the Thinker thread
MoveListGenerator::MoveListGenerator(int depth, bool RetrieveNotFailed)
: _phase(RetrieveNotFailed ? bestmove_transposition_table : killer1),
  _depth(depth)
{
    //no reason to scan an end board
    if(actualb.CountEmpty()==0)
    {
        _i=_movelist.End();
        _phase=rest;
    }

    //if no moves are found for player who is in turn, then will
    //have to search the other player, if there are no moves for 
    //neither players then movelist will be empty and we are at a
    //terminal node.
    for(int players=0; players<=1; players++)
    {
        if(RetrieveNotFailed)
            TryTranspositionTable();
        else
            TryKiller1();
        //did we found any moves? if yes then: job well done, return
        if(!_movelist.Empty())
        {
            assert(ListOk());
            return;
        }
        //else restart with the other player (if is is still possible 
        //see the outher for)
        actualbtt.isblacksturn=!actualbtt.isblacksturn;
        actualb.isblacksturn=!actualb.isblacksturn;
        _phase=RetrieveNotFailed ? bestmove_transposition_table : killer1;
        _legal.ReInit();
    }
}


MoveEnumerator::MoveEnumerator(Board &board)
{
    IsLegalMove legal(board);
    int dir;
    for(unsigned int i=p11; i<s89; i++)
        if(board.table[i]==empty && legal(i, dir))
             _movelist.PushBack(MoveInfo(i, 0, 0));
        
    //help the GUI, this must be done after list gen because I modify 
    //the board
    for(_i=_movelist.Begin(); _i!=_movelist.End(); _i++)
    {
        //sanity check
        assert(TTBoard::Persil[_i->x]!=-1);
        assert(board.table[_i->x]==empty);
        //mark for GUI
        board.table[_i->x]=_gui__possible;
    }

    _i=_movelist.Begin();
}


bool IsLegalMove::operator () (unsigned int i, int& dir)
{
    assert(_b.table[i]==empty);
    for(unsigned int idir=0; MoveExecuter::directions[idir]; idir++)
    {
        if( (MoveExecuter::dir_mask[i] & (1<<idir)) && 
            FlippableInDir(i, MoveExecuter::directions[idir]))
        {
            dir=idir;
            return true;
        }
    }
    return false;
}

//DEBUG
bool MoveListGenerator::ListOk()
{
    int dir;
    IsLegalMove legal;
    for(MoveInfo *i=_movelist.Begin(); i!=_movelist.End(); i++)
        if(TTBoard::Persil[i->x]==-1 || actualb.table[i->x]!=empty
            || !legal(i->x, dir) || dir!=i->dir) 
          return false;
    return true;
}


void MoveListGenerator::TryTranspositionTable()
{
    assert(_phase==bestmove_transposition_table);
    int dir;
    int move=actualbtt.BestMove();
    if(actualb.table[move]==empty && _legal(move, dir))
    {
        //mark that are going to search this move
        _skip.SkipMove(move);
        //actualb.isblacksturn=actualbtt.isblacksturn;
        _movelist.PushBack(MoveInfo(move, dir, 0));
        //actualb.DebugPrintBoard(move);
        _i=_movelist.Begin();
        _phase++;
    }
    else
    {
        _phase++;
        TryKiller1();
    }
}

void MoveListGenerator::TryKiller1()
{
    assert(_phase==killer1);
    int dir;
    int move=_hheuristic.KillerMove1(actualb.isblacksturn, _depth);
    if( move!=NOT_A_KMOVE && actualb.table[move]==empty && 
        (!_skip.WasSearched(move)) && _legal(move, dir) )
        {
            //mark that are going to search this move
            _skip.SkipMove(move);
            _movelist.PushBack(MoveInfo(move, dir, 0));
            //actualb.DebugPrintBoard(move);
            _i=_movelist.Begin();
            _phase++;
        }
    else
    {
        _phase++;
        TryKiller2();
    }
}

void MoveListGenerator::TryKiller2()
{
    assert(_phase==killer2);
    int dir;
    int move=_hheuristic.KillerMove2(actualb.isblacksturn, _depth);
    if( move!=NOT_A_KMOVE && actualb.table[move]==empty && 
        (!_skip.WasSearched(move)) && _legal(move, dir) )
        {
            //mark that are going to search this move
            _skip.SkipMove(move);
            _movelist.PushBack(MoveInfo(move, dir, 0));
            //actualb.DebugPrintBoard(move);
            _i=_movelist.Begin();
            _phase++;
        }
    else
    {
        _phase++;
        BuildSortedList();
    }
}

void MoveListGenerator::BuildSortedList()
{
    assert(_phase==bestmove_history_heuristic);
    _movelist.EraseAll();
    int dir;
    for(unsigned i=p11; i<s89; i++)
    {
        if(actualb.table[i]==empty && (!_skip.WasSearched(i)) && _legal(i, dir))
        {
            unsigned int value=_hheuristic.MoveValue(i, actualb.isblacksturn);
            _movelist.PushBack(MoveInfo(i, dir,value));
        }
    }

    //ok so we generated the list once, make sure that the effort 
    //will not be repeated
    _phase++;

    //sort moves based on their value retrieved from History Heuristic table   
    if(!_movelist.Empty())
    {
        _movelist.Sort();    
        _i=_movelist.Begin();
    }
    //no moves were found
    else
        _i=_movelist.End();
}

HistoryHeuristic& MoveListGenerator::_hheuristic=HistoryHeuristic::Instance();
TranspositionTable& MoveListGenerator::_ttable=TranspositionTable::Instance();

/*
#include <sstream> 
#include <windows.h> //DEBUG
#if defined NDEBUG
inline void MoveListGenerator::PrintList() {}
inline bool MoveListGenerator::ListOk(){return true;}
#else
void MoveListGenerator::PrintList()
{
    ::OutputDebugString("Moves: ");
    std::stringstream out;
    for(MoveInfo *i=_movelist.Begin(); i!=_movelist.End(); i++)
        out<<"("<<i->x<<" dir:"<<i->dir<<" val:"<<i->value<<") ";
    out<<std::endl;
    ::OutputDebugString(out.str().c_str());
}
#endif
*/

