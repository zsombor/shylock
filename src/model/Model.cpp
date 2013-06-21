/**
 @file
 Code for search routines, algortihms such as alpha-beta, iterative 
 deepening, MTD(f) and so on.
*/

#include <ctime>
#include <cstdlib>
#include "Model.h"
#include "Boards.h"
#include "MoveExecuter.h"
#include "MoveListGenerator.h"


using namespace Othello;

namespace Othello
{
    /** the board used during search */ 
    Board actualb;
    /** the current board's representation in a TT entry format */
    TTBoard actualbtt;
}

//DEBUG
unsigned int Othello::Count(squarevalue p)
{
    int ret=0;
    for(unsigned int i=p11; i<s89; i++)
        if(actualb.table[i]==p)
            ret++;
    return ret;
}

void Thinker::ResetGame()
{
    actualb.InitBoard();
    actualb.Syncronize();
    newBoard=actualb;
    gamevalue=0;
    bestMove=-1; //??
    _lastbestMove=-1; //??
    _ttable.FreeAll();
    _hheuristic.Reset();
    _stop=noStop;
    posSearched=0;
    ttHits=0;
    ttGHits=0;
    nrMoves=0;
    IdDepth=0;
    _thinking=false;
    timedout=false;
    _book.Reset(actualb);
    _avaible_time=_init_totaltime;
}

void Thinker::InitBoards()
{
    assert(sizeof(TTBoard)==8);
    actualb=newBoard;
    actualbtt.isblacksturn=actualb.isblacksturn;

    posSearched=0;
    ttHits=0;
    ttGHits=0;

    HistoryHeuristic::Instance().Reset();
    actualb.hash32=_ttable.Key32(actualb);
    actualbtt.SetLock32(_ttable.Lock32(actualb));
    actualb.Syncronize();

    _isdoublemove=false;
    _stop=noStop;
}

void Thinker::Stop()
{ 
    _stop=guiBreak;
}

Thinker::Thinker(HWND hwnd)
    :_hwnd(hwnd),
     _avaible_time(0),
     _init_totaltime(0),
     timedout(false),
     _timescheduler(_avaible_time, _init_totaltime, timedout)
{
    ResetGame();
}

Thinker::~Thinker()
{
    Kill();
}


void Thinker::Go()
{
    _event.GreenLight();
    _stop=noStop;
}

//!!!!!!!!!same o same!!!!!!!!!!!!!
void Thinker::FlushThread()
{
    _event.GreenLight();
}

void Thinker::Run()
{
    for(;;)
    {
        _thinking=false;
        _event.Wait();
        _thinking=true;
        if(_isDying)
            return;
        InitBoards();
        try
        {
            Think();
        }
        catch(STOP &stop)
        {
            //if this was a GUI request without further commands 
            //then block and wait until GUI makes a reset!
            if(stop.IsGuiBreak())
            {
                _event.RedLight();
            }
            else
                //otherwise (we had a timeout or a forced stop so send results)
                if(stop.IsForceMove() || stop.IsTimeOut())
                {
                    bestMove=_lastbestMove;
                    _timescheduler.AdjustElapsed();
                    DispatchResultsToGui();
                }
            continue;
        }
        DispatchResultsToGui();
    }
}

void Thinker::Force_Stop()
{
    _stop=forceMove;
}


/*MSC confuses std::max and std::min with his own macros
  so I'll the standard ones only on a less deviant compiler.*/
#if !defined _MSC_VER
#include <algorithm>
using std::max;
using std::min;
#endif



//TODO draw is not handled correctly
//alpha is redundant, this function is intended to be used
//exclusevly with null windows. What a mess!
int Thinker::AlphaBeta(int alpha, int beta, unsigned int depth)
{
    assert(actualb.AssertIsInSync());
    assert(actualb.hash32==_ttable.Key32(actualb));
    posSearched++;

    if(depth==0)
        return evaluator.Evaluate();

    bool leave=false;
    //DEBUG
    assert(depth<=MAXDEPTH);
    
    //current board value
    int g;

    //client peak
    if(_stop!=noStop)
        throw STOP(_stop);
     
    //try to retrive current board from transposition table
    TTBoard *prev=_ttable.Retrieve(); //TODO prev is not used properly
    
    if(prev==NULL)
        actualbtt.Clean();
    else
    {   
        //extract moveinfo
        actualbtt= *prev;

        ttHits++;
        assert((prev->GetDepth()!=0));
        
        //if we are in the same depth then extract game
        //values and use them ???????????
        //(prev->GetDepth()>=depth) seems more logical but if we will fail high using
        //deeper values then there is a chance that when doing the research this value 
        //will no longer be avaible. 
        if(prev->GetDepth()==depth)
        {
            //ttGHits++;
            if(prev->LowerBound() >= beta)
            {
                ttGHits++;
                return prev->LowerBound();
            }
            if(prev->UpperBound() <= alpha)
            {
                ttGHits++;
                return prev->UpperBound();
            }
        }
    }

#if defined _ENDGAME_TEST_
    if(actualb.CountEmpty()<9 && depth<=4 && IdDepth>4)
        return endgame.Solve(alpha, beta, posSearched);
#endif //_ENDGAME_TEST_

    bool hasBestMove=false;

    assert(depth!=0);
    //save alpha, beta (for TT stuff)
    int a=alpha;
    int b=beta;
    
    //generate moves
    MoveListGenerator moves(depth, prev!=NULL );
    if(moves.Empty())
    {   
        leave=true;
        g=evaluator.Evaluate();
    }
    else
    {
        UndoData udata;
        //MAX player (black)
        if(actualb.isblacksturn)
        {
            //at max player, the value of the node increases
            g=-infinity;
            while(g<beta)
            {
                if(!moves.AtEnd()) //!!! exec must have preccisely this scope!!!
                {
                    FullMoveExecuter exec(moves.X(), udata, moves.Dir());
                    //yap we are maximizing
                    int tmp=AlphaBeta(a, beta, depth-1);
                    //did we found a better move?, if yes then update bestmove info
                    if(g<tmp)
                    {
                        exec.MarkAsBestMove();
                        hasBestMove=true;
                        g=tmp;
                    }
                    //and tigthening bounds for the min player
                    a=max(a, g); //TODO can I place this line elsewhere?
                }
                else break;
                //if we allready have a cutoff then avoid further move genration
                if(g<beta)
                    moves.Step(); 
                else break; 
            }
        }
        //MIN player (white)
        else
        {
            //values for min player get smaller
            g=+infinity;
            while(g>alpha)
            {
                if(!moves.AtEnd()) //!!! exec must have preccisely this scope!!!
                {
                    FullMoveExecuter exec(moves.X(), udata, moves.Dir());
                    //this is what min does
                    int tmp=AlphaBeta(alpha, b, depth-1);
                    if(g>tmp)
                    {
                        exec.MarkAsBestMove();
                        hasBestMove=true;
                        g=tmp;
                    }   
                    //tigthen bound for max player
                    b=min(b, g);
                }
                else break;
                //if we allready have a cutoff then avoid further move genration
                if(g>alpha)
                    moves.Step();
                else break;
            }
        }
    } //we had some moves

    /*mark boounds*/
    //leaves always have "exact" value
    if(leave)
        actualbtt.SetExact(g, depth);
    //if failing high then we got a lower bound
    else if(g>=beta)
             actualbtt.SetLowerBound(g, depth);
         //we are failing low ==>> got an upper bound //NOTE beta-alpha==1 no chance for success 
         else 
             actualbtt.SetUpperBound(g, depth);

    //update history heuristic info and store results
    if(hasBestMove)
    {
        _hheuristic.GoodMove(actualbtt.BestMove(), actualb.isblacksturn, depth);
        _ttable.Store();
    }

    //retrieve best move;
    if(depth==IdDepth) //TODO check whether I could do this from the callee
        bestMove=actualbtt.BestMove();

    return g;
}

int Thinker::MTD(int f, int depth)
{
    int g=f;
    int upperbound=+infinity;
    int lowerbound=-infinity;
    int beta;
    do
    {
        if(g==lowerbound)
            beta=g+1;
        else
            beta=g;
        g=AlphaBeta(beta-1, beta, depth);
        if(g<beta)
            upperbound=g;
        else
            lowerbound=g;
        //assert(lowerbound<=upperbound);
    }while(lowerbound!=upperbound);

    return g;
}

void Thinker::Think()
{
    assert(actualb.empties==Count(empty));
    assert(actualb.blacks==Count(black));
    
    //first move
    if(actualb.CountEmpty()==60)
    {
        bestMove=firstmoves[rand() % 4];
        _lastbestMove=bestMove;
        return;
    }

    //book lookup
    bestMove=_book.AdvisedMove(actualb);
    _lastbestMove=bestMove;
    if(-1!=TTBoard::Persil[bestMove])
        return;


    //allocate time for this search
    _timescheduler.AllocTarget(actualb.CountEmpty());

    int startDepth=1;

    bestMove=-1;
    IsLegalMove legal; 
    int dir;
    TTBoard *prev=_ttable.Retrieve();
    if(prev!=NULL && prev->GetDepth()!=0 
       && actualb.table[prev->BestMove()]==empty
       && legal(prev->BestMove(), dir))
    {
        //assert(prev->HasExact());
        startDepth=prev->GetDepth()+1;
        gamevalue=prev->Value();

        bestMove=prev->BestMove();
        _lastbestMove=bestMove;

        //low on time situations
        if(MSecRemaining()<=3)
        {
            if(bestMove!=-1)
                return;
        }
        if(_isdoublemove && 2*_avaible_time<=_init_totaltime)
        {
            if(bestMove!=-1)
            {
                _isdoublemove=false;
                return;
            }
        }

    }
    else
        gamevalue=NaiveBestMoveSearch();

#if defined _ENDGAME_TEST_

    if(actualb.CountEmpty()<=20)
        if(actualb.CountEmpty()>17)
        {
            IdDepth=actualb.CountEmpty();
            gamevalue=AlphaBeta(-1, 1, actualb.CountEmpty());
            _lastbestMove=bestMove;
            return;
        }
        else
            if(actualb.CountEmpty()>10) //stupid (best move isn't captured in endgame)
            {
                IdDepth=actualb.CountEmpty();
                gamevalue=AlphaBeta(-infinity, infinity, actualb.CountEmpty());
                _lastbestMove=bestMove;
                return;
            }

#endif

    //can we play perfectly? 
    bool perfect_play=(gamevalue>63 || gamevalue<-63);
     
    //do iterative deepening
    unsigned int maxdepth=MAXDEPTH;
    if(maxdepth > actualb.CountEmpty())
        maxdepth = actualb.CountEmpty(); 
    
    int prevvalue=gamevalue;
    for(IdDepth=startDepth; IdDepth<=maxdepth; IdDepth++)
    {
        actualbtt.SetDepth(0);
        int tmp=MTD(prevvalue, IdDepth);
        prevvalue=gamevalue ; gamevalue=tmp; _lastbestMove=bestMove;
        if(!perfect_play)
        {
            if(gamevalue>63 || gamevalue<-63 || _timescheduler.TimeIsUp(IdDepth))
            {
                IdDepth++; break;
            }
        }
        else
        {
            if(IdDepth==actualb.CountEmpty() ||_timescheduler.TimeIsUp(IdDepth))
            {
                IdDepth++; break;
            }
        }
    }
    IdDepth--;
    _ttable.AdvanceSearchStamp();
}


void Thinker::TTChangeSize(unsigned int logsize)
{
    _ttable.ReAlloc(logsize);
}

unsigned int Thinker::TTLogSize()
{
    return _ttable.LogSize();
}


void Thinker::SetTotalTime(unsigned int min_per_game)
{
    _avaible_time=min_per_game*1000*60;
    _init_totaltime=_avaible_time;
}

unsigned int Thinker::MSecRemaining() 
{
    return _timescheduler.MSecRemaining();
}


void Thinker::DispatchResultsToGui()
{
    //stop execution until controller says GO!
    _event.RedLight();
    AddMove(bestMove, actualb.isblacksturn);
    newBoard=actualb;
    GuiMoveExecuter(newBoard, bestMove);

    //check if I can make another move
    Board tmp=newBoard;
    MoveEnumerator opponentmoves(tmp);
    if(opponentmoves.Empty())
    {
        Board tmp2=newBoard;
        tmp2.isblacksturn=!tmp2.isblacksturn;
        MoveEnumerator mymoves(tmp2);
        if(mymoves.Empty())
        {
            //game over
            ::SendMessage(_hwnd, WM_CALC_DONE, 0, 0);
            return;
        }
        //yes I can take another move 
        Go();
        newBoard.isblacksturn=!newBoard.isblacksturn;
        _isdoublemove=true;
        ::SendMessage(_hwnd, WM_UPDATE_VIEW, 0, 0);
        return;
    }
    //say: We are Done!
    ::SendMessage(_hwnd, WM_CALC_DONE, 0, 0);
}


int Thinker::Hint()
{
    assert(_thinking==false);

    if(newBoard.CountEmpty()==60)
        return firstmoves[rand() % 4];
    
    Board tmp=newBoard;
    actualb=newBoard;
    InitBoards();

    int hintmove=-1;

    if(!_book.OutOfBook())
    {
        hintmove=_book.AdvisedMove(actualb);
        if(-1!=TTBoard::Persil[hintmove])
        {
            newBoard=tmp;
            InitBoards();
            return hintmove;
        }
    }

    TTBoard *prev=_ttable.Retrieve();
    IsLegalMove legal; 
    int dir;
    hintmove=-1;
    if(prev!=NULL 
        && actualb.table[prev->BestMove()]==empty
        && legal(prev->BestMove(), dir))
        hintmove=prev->BestMove();
    newBoard=tmp;
    InitBoards();
    return hintmove;
    //add a shallow search here
}


int Thinker::NaiveBestMoveSearch()
{
    bool ismaxplayer=actualb.isblacksturn;
    IsLegalMove legal;
    int dir;
    UndoData udata;
    int act_best_move=-1;
    int act_best_val=ismaxplayer?-infinity:+infinity;
    for(unsigned int i=p11; i<s89; i++)
    {
        if(actualb.table[i]==empty && legal(i, dir))
        {
            FullMoveExecuter exec(i, udata, dir);
            int g=evaluator.Evaluate();
            if(ismaxplayer)
            {
                if(act_best_val<=g)
                {
                    act_best_val=g;
                    act_best_move=i;
                }
            }
            else
            {
                if(act_best_val>=g)
                {
                    act_best_val=g;
                    act_best_move=i;
                }
            }
        }
    }

    if(act_best_move!=-1)
    {
        bestMove=act_best_move;
        _lastbestMove=bestMove;
        return act_best_val;
    }
    return evaluator.Evaluate();
}

void Thinker::InitThread()
{
    srand(time(0));
    ResetGame();
}

int Thinker::deep_enough[]=
{
    10, 10, 10, 10, 10, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 9,
    9,  9,  9,  9,  9,  9,  9,  10,  10, 10,
    10,  9,  8,  7,  6,  5,  4,  3,  2,  1
};

squareindex Thinker::firstmoves[]=
{
    p43, p34, p56, p65
};

void Thinker::CheckTime()
{
    if(_thinking)
    {
       if(!_timescheduler.TimeOk())
            _stop=timeOut;
    }
}

HistoryHeuristic& Thinker::_hheuristic=HistoryHeuristic::Instance();
TranspositionTable& Thinker::_ttable=TranspositionTable::Instance();
