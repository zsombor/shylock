/**
 @file
 Interface of the oppening book, class Othello::Book
*/
#if !defined _BOOK_H_
#define _BOOK_H_
#include <cstdio>
#include "Boards.h"
#include "Hasher.h"

namespace Othello
{
    /**
     implements the opening book file( and it's logic) as a
     direct double adressing hashtable
     This is a dummy version: no book updates, naive searches, etc. */
    class Book: public Hasher
    {
    public:
        explicit Book();
        
        ~Book();

        /** 
         if no move is avaible then returns a sentinel square */
        squareindex AdvisedMove(const Board& b);

        /**
         @return true only if game is out of the book */
        bool OutOfBook() const;

        /**
         resets the book, use this if a new game has started
        @param b the new board from wich the game is started */
        void Reset(const Board& b);
    private:

        /**
         generates symetries, update hash codes */
        void GenSym(const Board& b);

        /**
         read an entry from a specified address */
        void Read(BookBoard &entry, unsigned int address);

        //symetries of the last (and current) analized board
        BookBoard _symbb[4];
        Board _sym[4];

        //hash keys, board 4 is the original board
        unsigned int _h1[4];
        unsigned int _h2[4];

        //the book file
        FILE* _f;

        //for book randomization
        static double _prob_rand_depth[];
        static const unsigned int _logsize;
        static const unsigned int _size;

        //DEBUG
        static void Print(BookBoard &b);

        bool _out_of_book;

        bool _book_is_present;
    };
}
#endif //_BOOK_H_

