/**
 @file
 The endgame searcher's interface. 
*/
/**************************************************************************
***************************************************************************
*                                                                         *
*                             CREDITS                                     *
*                                                                         *
*  This specialized search routine is based on the C endgame code from the*
*  NECI's FTP site. Originaly written by Jean-Christophe Weill rewritten  *
*  by  Warren D. Smith there after improved by Gunnar Andersson. I must   *
*  admit that my attempts to implement from scratch an endgame code with  *
*  comparable performance where all but unsuccesful.                      *
*                                                                         *
*      Gunnar Andersson         gunnar@nada.kth.se                        *
*      Warren D. Smith          wds@research.nec.nj.com                   *
*      Jean-Christophe Weill    jcw@seh.etca.fr.                          *
*                                                                         *  
***************************************************************************
**************************************************************************/

#include "Boards.h"
#include "HHeuristic.h"

#undef _ENDGAME_TEST_

namespace Othello
{
    const int WHITE=0;
    const int EMPTY=1;
    const int BLACK=2;
    const int DUMMY=3;

    typedef unsigned char uchar;
    typedef signed char schar;
    typedef unsigned int uint;

    const uint MAXEMPTIES=32;
    const uint USE_PARITY=4;
    const uint FASTEST_FIRST=7;

    extern Board actualb;

    /**
     this provides fast endgame searching services
    */
    class EndGame
    {
    public:
        explicit EndGame()
            :_h(HistoryHeuristic::Instance())
        {}

        /**
         use this to solve position stored in actualb
        */
        int Solve(int alpha, int beta, unsigned int &posSearched);

    private:

        HistoryHeuristic &_h;
        static unsigned int *_posSearched;

        static const int translator[];

        static void Adjust(int &t);

        static uchar board[91];

        /**
         a list of empty squares
        */
        struct EmList
        {
            int square;
            int hole_id;
            EmList *pred;
            EmList *succ;
        }; 

        static EmList EmHead;
        static EmList Ems[64];


        static uint HoleId[91];
        static uint RegionParity;

        /* The 8 legal directions: */
        static const schar dirinc[];
    
        static const uchar dirmask[];

        static const int worst2best[];

        static uchar* GlobalFlipStack[2048];
        static uchar **FlipStack;

        static void DrctnlFlips( uchar *sq, int inc, 
                                 int color, int oppcol );
        static int DoFlips( uchar *board, int sqnum, 
                            int color, int oppcol );

        static int CtDrctnlFlips( uchar *sq, int inc, 
                                  int color, int oppcol );
        static int CountFlips( uchar *board, int sqnum, 
                               int color, int oppcol );

        static int AnyDrctnlFlips( uchar *sq, int inc, int 
                                  color, int oppcol );
        static int AnyFlips( uchar *board, int sqnum, 
                                  int color, int oppcol ); 

        static void UndoFlips( int FlipCount, int oppcol );

        static uint minu(uint a, uint b);

        static void PrepareToSolve( uchar *board );

        static int NoParEndSolve (uchar *board, int alpha, int beta, 
                int color, int empties, int discdiff, int prevmove );

        static int  ParEndSolve (uchar *board, int alpha, int beta, 
                int color, int empties, int discdiff, int prevmove );

        static int count_mobility( uchar *board, int color ); 

        static int FastestFirstEndSolve( uchar *board, int alpha, 
                      int beta, int color, int empties, int discdiff,
                       int prevmove );

        static int EndSolve (uchar *board, int alpha, int beta, 
                   int color, int empties, int discdiff, int prevmove );
    };
}

