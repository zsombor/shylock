/**
 @file
 Interface of the Othello::BillMobilityDat class.
*/

#if !defined _BILLMOBILITYDAT_H_
#define _BILLMOBILITYDAT_H_
#include "Boards.h"

namespace Othello
{
    /**
       Bill mobility is a fast aproximation of the total mobility
       by counting the number of fliping directions using precomputed 
       tables.
       These aren't quite the usual BILL mobility tables
       but I use them only to in/confirm that it's Black's/White's turn */
    class BillMobilityDat
    {
    public:
        
        /**
          constructor initializes the pattern tables */
        explicit BillMobilityDat();

        /**
          table to query lines of 8, accepts negative indices */
        static bool *pattern8;

        /**
          table to query lines of 7, accepts negative indices */
        static bool *pattern7;

        /**
          table to query lines of 6, accepts negative indices */
        static bool *pattern6;

        /**
          table to query lines of 5, accepts negative indices */
        static bool *pattern5;

        /**
          table to query lines of 4, accepts negative indices */
        static bool *pattern4;

        /**
          table to query lines of 3, accepts negative indices */
        static bool *pattern3;

    private:

        /**
          table to query lines of 8 */
        static bool _pattern8[3*3*3*3*3*3*3*3];

        /**
          table to query lines of 7 */
        static bool _pattern7[3*3*3*3*3*3*3];

        /**
          table to query lines of 6 */
        static bool _pattern6[3*3*3*3*3*3];

        /**
          table to query lines of 5 */
        static bool _pattern5[3*3*3*3*3];

        /**
          table to query lines of 4 */
        static bool _pattern4[3*3*3*3];

        /**
          table to query lines of 3 */
        static bool _pattern3[3*3*3];

        /**
          this is used to generate all patterns */
        void Build(int k, squarevalue p, int length, squarevalue *pattern);

        /**
          computes the value of a given pattern with the given length */
        bool Compute(int length, squarevalue *pattern);
    };
}
#endif //_BILLMOBILITYDAT_H_

