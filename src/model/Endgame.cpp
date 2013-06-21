/**
 @file
 A fast endgame searcher.
*/

#include "Endgame.h"
//no need to compile this if we aren't going to build with it anyway.
#ifdef _ENDGAME_TEST_

#include <cassert>
#include <cstdio>

using namespace Othello;

uchar EndGame::board[91];

EndGame::EmList EndGame::EmHead;
EndGame::EmList EndGame::Ems[64];

uint EndGame::HoleId[91];
uint EndGame::RegionParity;
 
const schar EndGame::dirinc[]={1, -1, 8, -8, 9, -9, 10, -10, 0};
    
const uchar EndGame::dirmask[91] = 
{
    0,0,0,0,0,0,0,0,0,
    0,81,81,87,87,87,87,22,22,
    0,81,81,87,87,87,87,22,22,
    0,121,121,255,255,255,255,182,182,
    0,121,121,255,255,255,255,182,182,
    0,121,121,255,255,255,255,182,182,
    0,121,121,255,255,255,255,182,182,
    0,41,41,171,171,171,171,162,162,
    0,41,41,171,171,171,171,162,162,
    0,0,0,0,0,0,0,0,0,0
};

const int EndGame::worst2best[64] =
{
    20 , 25 , 65 , 70 ,
    11 , 16 , 19 , 26 , 64 , 71 , 74 , 79 ,
    21 , 24 , 29 , 34 , 56 , 61 , 66 , 69 ,
    22 , 23 , 38 , 43 , 47 , 52 , 67 , 68 ,
    31 , 32 , 39 , 42 , 48 , 51 , 58 , 59 ,
    13 , 14 , 37 , 44 , 46 , 53 , 76 , 77 ,
    30 , 33 , 57 , 60 ,
    12 , 15 , 28 , 35 , 55 , 62 , 75 , 78 ,
    10 , 17 , 73 , 80 , 
    40 , 41 , 49 , 50
};

uchar* EndGame::GlobalFlipStack[2048];
uchar **EndGame::FlipStack= &(GlobalFlipStack[0]);

inline void EndGame::DrctnlFlips(uchar *sq, int inc, int color, int oppcol)
{
    uchar *pt = sq + inc;
    if(*pt == oppcol)
    {
        pt += inc;
        if(*pt == oppcol)
        {
            pt += inc;
            if(*pt == oppcol)
            {
                pt += inc;
                if(*pt == oppcol)
                {
                    pt += inc;
                    if(*pt == oppcol)
                    {
                        pt += inc;
                        if(*pt == oppcol)
                            pt += inc;
                    }
                }
            }
        }
        if(*pt == color)
        {
            pt -= inc;
            do
            {
                *pt = color;
                *(FlipStack++) = pt;
                pt -= inc;
            }while( pt != sq);
        }
    }
}


int EndGame::DoFlips( uchar *board, int sqnum, int color, int oppcol )
{
    int j=dirmask[sqnum];
    uchar **OldFlipStack = FlipStack;
    uchar *sq;
    sq = sqnum + board;
    if ( j & 128 )
        DrctnlFlips( sq, dirinc[7], color, oppcol );
    if ( j & 64 )
        DrctnlFlips( sq, dirinc[6], color, oppcol );
    if ( j & 32 )
        DrctnlFlips( sq, dirinc[5], color, oppcol );
    if ( j & 16 )
        DrctnlFlips( sq, dirinc[4], color, oppcol );
    if ( j & 8 )
        DrctnlFlips( sq, dirinc[3], color, oppcol );
    if ( j & 4 )
        DrctnlFlips( sq, dirinc[2], color, oppcol );
    if ( j & 2 )
        DrctnlFlips( sq, dirinc[1], color, oppcol );
    if ( j & 1 )
        DrctnlFlips( sq, dirinc[0], color, oppcol );

    return FlipStack - OldFlipStack;
}

inline 
int EndGame::CtDrctnlFlips( uchar *sq, int inc, int color, int oppcol )
{
    uchar *pt = sq + inc;
    if(*pt == oppcol)
    {
        int count = 1;
        pt += inc;
        if(*pt == oppcol)
        {
            count++;                
            pt += inc;
            if(*pt == oppcol)
            {
                count++;            
                pt += inc;
                if(*pt == oppcol)
                {
                    count++;        
                    pt += inc;
                    if(*pt == oppcol)
                    {
                        count++;    
                        pt += inc;
                        if(*pt == oppcol)
                        {
                            count++;  
                            pt += inc;
                        }
                    }
                }
            }
        }
        if(*pt == color) 
            return count;
    }
   return 0;
}

int EndGame::CountFlips( uchar *board, int sqnum, int color, int oppcol )
{
    int ct=0;
    int j=dirmask[sqnum];
    uchar *sq;
    sq = sqnum + board;
    if ( j & 128 )
        ct += CtDrctnlFlips( sq, dirinc[7], color, oppcol );
    if ( j & 64 )
        ct += CtDrctnlFlips( sq, dirinc[6], color, oppcol );
    if ( j & 32 )
        ct += CtDrctnlFlips( sq, dirinc[5], color, oppcol );
    if ( j & 16 )
        ct += CtDrctnlFlips( sq, dirinc[4], color, oppcol );
    if ( j & 8 )
        ct += CtDrctnlFlips( sq, dirinc[3], color, oppcol );
    if ( j & 4 )
        ct += CtDrctnlFlips( sq, dirinc[2], color, oppcol );
    if ( j & 2 )
        ct += CtDrctnlFlips( sq, dirinc[1], color, oppcol );
    if ( j & 1 )
        ct += CtDrctnlFlips( sq, dirinc[0], color, oppcol );
    return(ct);
}


inline int EndGame::AnyDrctnlFlips( uchar *sq, int inc, int color, int oppcol )
{
    uchar *pt = sq + inc;
    if(*pt == oppcol)
    {
        pt += inc;
        if(*pt == oppcol)
        {
            pt += inc;
            if(*pt == oppcol)
            {
                pt += inc;
                if(*pt == oppcol)
                {
                    pt += inc;
                    if(*pt == oppcol)
                    {
                        pt += inc;
                        if(*pt == oppcol)
                            pt += inc;
                    }
                }
            }
        }
        if(*pt == color) 
            return 1;
    }

   return 0;
}

int EndGame::AnyFlips( uchar *board, int sqnum, int color, int oppcol )
{
    int j=dirmask[sqnum];
    uchar *sq;
    sq = sqnum + board;
    if( (( j & 128 ) && AnyDrctnlFlips( sq, dirinc[7], color, oppcol )) ||
        (( j & 64 )  && AnyDrctnlFlips( sq, dirinc[6], color, oppcol )) ||
        (( j & 32 ) && AnyDrctnlFlips( sq, dirinc[5], color, oppcol )) ||
        (( j & 16 ) && AnyDrctnlFlips( sq, dirinc[4], color, oppcol )) ||
        (( j & 8 ) && AnyDrctnlFlips( sq, dirinc[3], color, oppcol )) ||
        (( j & 4 ) && AnyDrctnlFlips( sq, dirinc[2], color, oppcol )) ||
        (( j & 2 ) && AnyDrctnlFlips( sq, dirinc[1], color, oppcol )) ||
        (( j & 1 ) && AnyDrctnlFlips( sq, dirinc[0], color, oppcol )) )
        return 1;
    return 0;
}



void EndGame::UndoFlips( int FlipCount, int oppcol )
{  
    if(FlipCount%2)
    {
        FlipCount--;
        * (*(--FlipStack)) = oppcol;
    }
    while(FlipCount)
    {
        FlipCount -= 2;
        * (*(--FlipStack)) = oppcol;
        * (*(--FlipStack)) = oppcol;
    }
}

inline uint EndGame::minu(uint a, uint b)
{
    if(a<b) 
        return a;
    return b;
}

void EndGame::PrepareToSolve( uchar *board )
{
    int i,sqnum;
    uint k;
    EmList *pt;
    int z;
    // find hole IDs: 
    k = 1;
    for(i=10; i<=80; i++)
    {
        if(board[i]==EMPTY)
        {
            if( board[i-10]==EMPTY ) HoleId[i] = HoleId[i-10];
            else if( board[i-9]==EMPTY ) HoleId[i] = HoleId[i-9];
            else if( board[i-8]==EMPTY ) HoleId[i] = HoleId[i-8];
            else if( board[i-1]==EMPTY ) HoleId[i] = HoleId[i-1];
            else{ HoleId[i] = k; k<<=1; }
        }
        else HoleId[i] = 0;
    }
    const int MAXITERS=1;
   /* In some sense this is wrong, since you
    * ought to keep doing iters until reach fixed point, but in most
    * othello positions with few empties this ought to work, and besides,
    * this is justifiable since the definition of "hole" in othello
    * is somewhat arbitrary anyway. */
    for(z=MAXITERS; z>0; z--)
    {
        for(i=80; i>=10; i--)
        {
            if(board[i]==EMPTY)
            {
                k = HoleId[i];
                if( board[i+10]==EMPTY ) HoleId[i] = minu(k,HoleId[i+10]);
                if( board[i+9]==EMPTY ) HoleId[i] = minu(k,HoleId[i+9]);
                if( board[i+8]==EMPTY ) HoleId[i] = minu(k,HoleId[i+8]);
                if( board[i+1]==EMPTY ) HoleId[i] = minu(k,HoleId[i+1]);
            }
        }
        for(i=10; i<=80; i++)
        {
            if(board[i]==EMPTY)
            {
                k = HoleId[i];
                if( board[i-10]==EMPTY ) HoleId[i] = minu(k,HoleId[i-10]);
                if( board[i-9]==EMPTY ) HoleId[i] = minu(k,HoleId[i-9]);
                if( board[i-8]==EMPTY ) HoleId[i] = minu(k,HoleId[i-8]);
                if( board[i-1]==EMPTY ) HoleId[i] = minu(k,HoleId[i-1]);
            }
        }
    }
    /* find parity of holes: */
    RegionParity = 0;
    for(i=10; i<=80; i++)
        RegionParity ^= HoleId[i];

    /* create list of empty squares: */
    k = 0;
    pt = &EmHead;
    for(i=60-1; i>=0; i--)
    {
        sqnum = worst2best[i];
        if( board[sqnum]==EMPTY )
        {
            pt->succ = &(Ems[k]);
            Ems[k].pred = pt;
            k++;
            pt = pt->succ;
            pt->square = sqnum;
	        pt->hole_id = HoleId[sqnum];
        }
    }
    pt->succ = NULL;
    assert(k<=MAXEMPTIES);
}

int EndGame::NoParEndSolve (uchar *board, int alpha, int beta, 
              int color, int empties, int discdiff, int prevmove )
{
   *_posSearched++;
   int score = -infinity;
   int oppcol = 2-color;
   int sqnum,j,ev;
   EmList *em, *old_em;
   for(old_em = &EmHead, em = old_em->succ; em!=NULL;
       old_em = em, em = em->succ){
      /* go thru list of possible move-squares */
      sqnum = em->square;
      j = DoFlips( board, sqnum, color, oppcol );
      if(j){ /* legal move */
          /* place your disc: */
          *(board+sqnum) = color;
          /* delete square from empties list: */
	  old_em->succ = em->succ;
          if(empties==2){ /* So, now filled but for 1 empty: */
             int j1;
             j1 = CountFlips( board, EmHead.succ->square, oppcol, color);
             if(j1){ /* I move then he moves */
                ev = discdiff + 2*(j-j1);
             }
             else{ /* he will have to pass */
                j1 = CountFlips(board, EmHead.succ->square, color, oppcol);
		ev = discdiff + 2*j;
                if(j1){ /* I pass then he passes then I move */
		  ev += 2 * (j1 + 1);
                }
                else{ /* I move then both must pass, so game over */

                   ev++;

                }
             }
          }
          else{
             ev = -NoParEndSolve(board, -beta, -alpha, 
                      oppcol, empties-1, -discdiff-2*j-1, sqnum);
          }
          UndoFlips( j, oppcol );
          /* un-place your disc: */
          *(board+sqnum) = EMPTY;
          /* restore deleted empty square: */
	  old_em->succ = em;

          if(ev > score){ /* better move: */
             score = ev;
             if(ev > alpha){
                alpha = ev;
                if(ev >= beta){ /* cutoff */
                   return score;
                }
             }
          }
       }
    }
    if(score == -infinity){  /* No legal move */
       if(prevmove == 0){ /* game over: */
          return discdiff;
       }
       else /* I pass: */ return
          -NoParEndSolve( board, -beta, -alpha, oppcol, empties, -discdiff, 0);
    }
    return score;
}
   
int EndGame::ParEndSolve(uchar *board, int alpha, int beta, 
            int color, int empties, int discdiff, int prevmove )
{
   *_posSearched++;
   int score = -infinity;
   int oppcol = 2-color;
   int sqnum,j,ev;
   EmList *em, *old_em;
   uint parity_mask;
   int par, holepar;

   for ( par = 1, parity_mask = RegionParity; par >= 0;
	 par--, parity_mask = ~parity_mask ) {

   for(old_em = &EmHead, em=old_em->succ; em!=NULL;
       old_em = em, em = em->succ){
      /* go thru list of possible move-squares */
      holepar = em->hole_id;
      if ( holepar & parity_mask ) {
      sqnum = em->square;
      j = DoFlips( board, sqnum, color, oppcol );
      if(j){ /* legal move */
          /* place your disc: */
          *(board+sqnum) = color;
          /* update parity: */
          RegionParity ^= holepar;
          /* delete square from empties list: */
	  old_em->succ = em->succ;
          if(empties<=1+USE_PARITY)
             ev = -NoParEndSolve(board, -beta, -alpha, 
                   oppcol, empties-1, -discdiff-2*j-1, sqnum);
          else
             ev = -ParEndSolve(board, -beta, -alpha, 
                   oppcol, empties-1, -discdiff-2*j-1, sqnum);
          UndoFlips( j, oppcol );
          /* restore parity of hole */
          RegionParity ^= holepar;
          /* un-place your disc: */
          *(board+sqnum) = EMPTY;
          /* restore deleted empty square: */
	  old_em->succ = em;

          if(ev > score){ /* better move: */
             score = ev;
             if(ev > alpha){
                alpha = ev;
                if(ev >= beta){ 
                   return score;
	       }
	    }
	 }
       }
    }}
    }

    if(score == -infinity){  /* No legal move found */
       if(prevmove == 0){ /* game over: */
          return discdiff;
       }
       else /* I pass: */ return
          -ParEndSolve( board, -beta, -alpha, oppcol, empties, -discdiff, 0);
    }
    return score;
}

int EndGame::count_mobility( uchar *board, int color )
{
  int oppcol = 2 - color;
  int mobility;
  int square;
  struct EmList *em;

  mobility = 0;
  for ( em = EmHead.succ; em != NULL; em = em->succ ) {
    square = em->square;
    if ( AnyFlips( board, square, color, oppcol ) )
      mobility++;
  }

  return mobility;
}


int EndGame::FastestFirstEndSolve( uchar *board, int alpha, int beta, 
		      int color, int empties, int discdiff,int prevmove )
{
  int i, j;
  int score = -infinity;
  int oppcol = 2 - color;
  int sqnum, ev;
  int flipped;
  int moves, mobility;
  int best_value, best_index;
  EmList *em, *old_em;
  EmList *move_ptr[64];
  int holepar;
  int goodness[64];
  
  *_posSearched++;
  
  moves = 0;
  for ( old_em = &EmHead, em = old_em->succ; em != NULL;
	old_em = em, em = em->succ ) {
    sqnum = em->square;
    flipped = DoFlips( board, sqnum, color, oppcol );
    if ( flipped ) {
      board[sqnum] = color;
      old_em->succ = em->succ;
      mobility = count_mobility( board, oppcol );
      old_em->succ = em;
      UndoFlips( flipped, oppcol );
      board[sqnum] = EMPTY;
      move_ptr[moves] = em;
      goodness[moves] = -mobility;
      moves++;
    }
  }

  if ( moves != 0 ) {
    for ( i = 0; i < moves; i++ ) {
      best_value = goodness[i];
      best_index = i;
      for ( j = i + 1; j < moves; j++ )
	if ( goodness[j] > best_value ) {
	  best_value = goodness[j];
	  best_index = j;
	}
      em = move_ptr[best_index];
      move_ptr[best_index] = move_ptr[i];
      goodness[best_index] = goodness[i];

      sqnum = em->square;
      holepar = em->hole_id;
      j = DoFlips( board, sqnum, color, oppcol );
      board[sqnum] = color;
      RegionParity ^= holepar;
      em->pred->succ = em->succ;
      if ( em->succ != NULL )
	em->succ->pred = em->pred;
      if ( empties <= FASTEST_FIRST + 1 )
	ev = -ParEndSolve( board, -beta, -alpha, oppcol, empties - 1,
			   -discdiff - 2 * j - 1, sqnum);
      else
	ev = -FastestFirstEndSolve( board, -beta, -alpha, oppcol,
				    empties - 1, -discdiff - 2 * j - 1,
				    sqnum);
      UndoFlips( j, oppcol );
      RegionParity ^= holepar;
      board[sqnum] = EMPTY;
      em->pred->succ = em;
      if ( em->succ != NULL )
	em->succ->pred = em;

      if ( ev > score ) { /* better move: */
	score = ev;
	if ( ev > alpha ) {
	  alpha = ev;
	  if ( ev >= beta )
	    return score;
	}
      }
    }
  }
  else {
    if ( prevmove == 0 ) 
      return discdiff;
    else  /* I pass: */
      score = -FastestFirstEndSolve( board, -beta, -alpha, oppcol,
				     empties, -discdiff, 0);
  }

  return score;
}

int EndGame::EndSolve (uchar *board, int alpha, int beta, 
               int color, int empties, int discdiff, int prevmove )
{
  if ( empties > FASTEST_FIRST )
    return FastestFirstEndSolve(board,alpha,beta,color,empties,discdiff,prevmove);
  else {
   if(empties <= (2>USE_PARITY ? 2 : USE_PARITY) )
      return NoParEndSolve(board,alpha,beta,color,empties,discdiff,prevmove);
   else 
   return ParEndSolve(board,alpha,beta,color,empties,discdiff,prevmove);
  }
}

inline void EndGame::Adjust(int &t)
{
    if(t>63)
        t-=63;
    if(t<-63)
        t+=63;
}

#include <cstring>

const int EndGame::translator[10*10]=
{
    0, 0, 0, 0, 0, 0, 0, 0, 0,0,
    0,10,11,12,13,14,15,16,17,0,
    0,19,20,21,22,23,24,25,26,0,
    0,28,29,30,31,32,33,34,35,0,
    0,37,38,39,40,41,42,43,44,0,
    0,46,47,48,49,50,51,52,53,0,
    0,55,56,57,58,59,60,61,62,0,
    0,64,65,66,67,68,69,70,71,0,
    0,73,74,75,76,77,78,79,80,0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,0,
};

unsigned int *EndGame::_posSearched;

int EndGame::Solve(int alpha, int beta, unsigned int &posSearched)
{
    _posSearched=&posSearched;
    Adjust(alpha); 
    Adjust(beta);
    memset(board, DUMMY, sizeof(board));
    for(uint i=p11; i<s89; i++)
    {
        const int p=translator[i];
        if(p)
            board[p]=actualb.table[i]+1;
    }

    PrepareToSolve(board);

    const int color=actualb.isblacksturn?BLACK:WHITE;
    const int d_diff=actualb.isblacksturn?actualb.CountBlack()-actualb.CountWhite():
                                          actualb.CountWhite()-actualb.CountBlack();
    int ret=EndSolve(board, alpha, beta, color,
                    actualb.CountEmpty(), d_diff,1);
 
    if(ret>0)
        return ret+63;
    if(ret<0)
        return ret-63;
    return ret;
}
#endif //#if defined _ENDGAME_TEST_

