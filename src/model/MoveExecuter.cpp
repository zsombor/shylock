/**
 @file
 The code that updates the board representation during search.
*/

#include <cassert>
#include <cstddef>
#include "MoveExecuter.h"
#include "TTable.h"

using namespace Othello;

const int MoveExecuter::directions[]=
{
    1, -1, 9, -9, 10, -10, 11, -11, 0
};

const unsigned int MoveExecuter::dir_mask[10*10]=
{
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0, 81, 81, 87, 87, 87, 87, 22, 22,  0,
    0, 81, 81, 87, 87, 87, 87, 22, 22,  0,
    0,121,121,255,255,255,255,182,182,  0,
    0,121,121,255,255,255,255,182,182,  0,
    0,121,121,255,255,255,255,182,182,  0,
    0,121,121,255,255,255,255,182,182,  0,
    0, 41, 41,171,171,171,171,162,162,  0,
    0, 41, 41,171,171,171,171,162,162,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

FullMoveExecuter::FullMoveExecuter(const int pos, UndoData &udata, int dir)
    :MoveExecuter(actualb.isblacksturn, pos),
     _backupTTBoard(actualbtt),
     _hash32(actualb.hash32),
     _backup_blaks(actualb.blacks),
     _udata(udata)
{   
    assert(actualb.table[_move]==empty);
    assert(actualb.AssertIsInSync());

    const unsigned int mask=dir_mask[_move];
    unsigned int idir=dir;
    
    switch(player)
    {
    case black:
        //scan in every direction
        for(; directions[idir]; idir++)
        {
            //if this is not a possible direction from square x then skipp
            if(! (mask & (1<<idir)) )
                continue;

            //now go trough opponents down
            int x=_move+directions[idir];
            for(; actualb.table[x]==white; x+=directions[idir])
                ;

            //if found an actual players's disc then go backwards and do flips
            if(actualb.table[x]==black)
            {
                x -= directions[idir];
                for(; x!=_move; x-=directions[idir])
                {
                    _ttable.FlipInKey32(x, black);
                    BlackUpdateIndexFlip(x);
                    actualb.table[x]=black;
                    UpdateUndoInfo(actualb.table[x]); 
                }
            }
        }

        //place the new disc
        _ttable.PlaceInKey32(pos, black);
        BlackUpdateIndexPlace(pos);
        actualb.table[pos]=black;
        actualb.blacks += _udata.count+1;
        actualb.isblacksturn=false;
        actualbtt.isblacksturn=false;
        break;
    default:
        assert(player==white);
        for(; directions[idir]; idir++)
        {
            if(! (mask & (1<<idir)))
                continue;
            
            int x=_move+directions[idir];
            for(; actualb.table[x]==black; x+=directions[idir])
                ;
            if(actualb.table[x]==white)
            {
                x -= directions[idir];
                for(; x!=_move; x-=directions[idir])
                {
                    _ttable.FlipInKey32(x, white);
                    WhiteUpdateIndexFlip(x);
                    actualb.table[x]=white;
                    UpdateUndoInfo(actualb.table[x]); 
                }
            }
        }

        //place the new disc
        _ttable.PlaceInKey32(pos, white);
        WhiteUpdateIndexPlace(pos);
        actualb.table[pos]=white;
        actualb.blacks -= _udata.count;
        actualb.isblacksturn=true;
        actualbtt.isblacksturn=true;
        break;
    }
    actualb.empties--;
    assert(actualb.AssertIsInSync());
}

GuiMoveExecuter::GuiMoveExecuter(Board &board, int pos)
    :MoveExecuter(board.isblacksturn, pos)
{   
    assert(board.table[_move]==empty || 
           board.table[_move]==_gui__possible || 
           board.table[_move]==_gui__hint);
 
    int flips=0;
    //do flips
    unsigned int idir=0;
    for(; directions[idir]; idir++)
    {
        int x;
        for(x=_move+directions[idir]; 
            board.table[x]==opponent;
            x += directions[idir])
            ;
        if(board.table[x]==player)
            for(x -= directions[idir]; x!=_move; x -= directions[idir])
            {
                board.table[x]=player;
                flips++;
            }

    }
    assert(flips);

    //place the new disc
    board.table[pos]=player;
    board.isblacksturn=!board.isblacksturn;
    board.empties--;
    if(player==black)
        board.blacks += flips+1;
    else
        board.blacks -= flips;
}

FullMoveExecuter::~FullMoveExecuter()
{
    //restore from backup    
    Undo();
    actualbtt=_backupTTBoard;
    actualb.empties++;
    actualb.blacks=_backup_blaks;
    assert(actualb.AssertIsInSync());
}


void FullMoveExecuter::Undo()
{
    //undo disc place
    actualb.table[_move]=empty;
    unsigned int i=0;
    if(_backupTTBoard.isblacksturn)
    {
        //'place a white over the black'
        //indeces (place) 
        WhiteUpdateIndexPlace(_move);
        //undo flip (disc, and indices)
        for(; i<_udata.count; i++)
        {
            //disc
            *(_udata.Prev[i])=white;
            //indices
            const int x=_udata.Prev[i]-actualb.table;
            WhiteUpdateIndexFlip(x);
        }
    }
    else
    {
        // 'place a balck over the white'
        //indeces (place)
        BlackUpdateIndexPlace(_move);
        //undo flip (disc, and indices)
        for(; i<_udata.count; i++)
        {
            //disc
            *(_udata.Prev[i])=black;
            //indices
            int const x=_udata.Prev[i]-actualb.table;
            BlackUpdateIndexFlip(x);
        }
    }
    actualb.isblacksturn= _backupTTBoard.isblacksturn;
    _udata.count=0; 
    actualb.hash32=_hash32;
}

inline void FullMoveExecuter::BlackUpdateIndexPlace(int x)
{
    for(int i=0; i<5; i++)
        *actualb.updatetable[x][i].Index += 
                 actualb.updatetable[x][i].PlaceOffset;
}

inline void FullMoveExecuter::WhiteUpdateIndexPlace(int x)
{
    for(int i=0; i<5; i++)
        *actualb.updatetable[x][i].Index -=
            actualb.updatetable[x][i].PlaceOffset;
}

inline void FullMoveExecuter::BlackUpdateIndexFlip(int x)
{
    for(int i=0; i<5; i++)
        *actualb.updatetable[x][i].Index +=
            actualb.updatetable[x][i].FlipOffset;
}

inline void FullMoveExecuter::WhiteUpdateIndexFlip(int x)
{
    for(int i=0; i<5; i++)
        *actualb.updatetable[x][i].Index -=
            actualb.updatetable[x][i].FlipOffset;
}

inline void FullMoveExecuter::UpdateUndoInfo(squarevalue &p)
{
    _udata.Prev[_udata.count] = &p;
    _udata.count++;
}

TranspositionTable& FullMoveExecuter::_ttable=TranspositionTable::Instance();
