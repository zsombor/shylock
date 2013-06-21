/**
 @file
 Interface of the Othello::Hasher base class, offering hash services
*/
#if !defined _HASHER_H_
#define _HASHER_H_

#include "Boards.h"

namespace Othello
{
    /**
     Hashing services */
    class Hasher
    {
    public:
        /** key generator from table */
        unsigned int Key32(const Board &board);

        /** lock generator from table */ 
        unsigned int Lock32(const Board &board);

        /** redundant (almost) */
        unsigned int Hash1(const BookBoard &b);
        /** redundant (almost) */
        unsigned int Hash2(const BookBoard &b);

    protected:

        /** zorrbist codes for every square and possible content.*/
        static unsigned int Zorbist[10*10][3];
        static unsigned int Zorbist2[10*10][3];
    };
}
#endif //_HASHER_H_

