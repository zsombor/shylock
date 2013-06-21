/**
 @file
 Some history heuristic related code
*/
#include "HHeuristic.h"

using namespace Othello;

HistoryHeuristic& HistoryHeuristic::Instance()
{
    static HistoryHeuristic hh;
    return hh;
}

void HistoryHeuristic::Reset()
{
    for(unsigned int i=p11; i<s89; i++)
    {
        _tb[i][0]=0;
        _tb[i][1]=0;
    }
    for(unsigned int d=0; d<32; d++)
    {
        _kmoves[d][0].move1=NOT_A_KMOVE;
        _kmoves[d][0].move2=NOT_A_KMOVE;
        _kmoves[d][1].move1=NOT_A_KMOVE;
        _kmoves[d][1].move2=NOT_A_KMOVE;
    }
}

