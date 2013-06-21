/**
 @file 
 Handles the transposition table
*/
#include <cstddef>
#include <cstring>
#include "TTable.h"

using namespace Othello;

TranspositionTable::TranspositionTable()
    : _size(1<<DEF_LOG_SIZE),
      _logsize(DEF_LOG_SIZE),
      _stamp(0)
{
    //allocate table
    _arr=new TTBoard[_size];
    FreeAll();
}

void TranspositionTable::ReAlloc(unsigned int logsize)
{
    _size=1<<logsize;
    _logsize=logsize;
    _stamp=0;
    delete []_arr;
    _arr=new TTBoard[_size];
    FreeAll();
}

TranspositionTable::~TranspositionTable()
{
    //deallocate table
    delete []_arr;
}

void TranspositionTable::FreeAll()
{
    memset(_arr, 0, _size*sizeof(TTBoard));
}

TranspositionTable& TranspositionTable::Instance()
{
    static TranspositionTable tt;
    return tt;
}

void TranspositionTable::AdvanceSearchStamp()
{
    _stamp=(_stamp+1)%4;
}

