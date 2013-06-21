/**
 @file
 The transposition table's interface
*/
#if !defined _TTABLE_H_
#define _TTABLE_H_
#include "Boards.h"
#include "Hasher.h"

namespace Othello
{
    extern Board actualb;
    extern TTBoard actualbtt;

    /**
     class TranspositionTable is a singleton implementing the 
     Transposition Table as a hash table with direct adressing */
    class TranspositionTable: public Hasher
    {
    public:
        /**
         unique access point */
        static TranspositionTable& Instance();

        enum {DEF_LOG_SIZE=21}; //hard coded NO-NO

        /**
         change the TT size, during a game this can't be done  */
        void ReAlloc(unsigned int logsize);

        /**
         clean up */
        ~TranspositionTable();

        /**
         @return the current board if found. */
        TTBoard* Retrieve();

        /**
         stores the current board */
        void Store();

        /**
         makes a flip in the key of actualb */
        void FlipInKey32(int x, squarevalue player)
        {
            assert(Zorbist[x][player+1]!=0);
            assert(Zorbist2[x][player+1]!=0);
            assert(player==-actualb.table[x]);

            //NOTE: a squarevalue can have negative values (-1) 
            actualb.hash32^=Zorbist[x][-player+1]^Zorbist[x][player+1];

            actualbtt.lock32^=Zorbist2[x][-player+1]^Zorbist2[x][player+1];
        }

        /**
         makes a place in the key of actualb */
        void PlaceInKey32(int x, squarevalue player)
        {
            assert(Zorbist[x][player+1]!=0);
            assert(Zorbist2[x][player+1]!=0);
            assert(empty==actualb.table[x]);

            //NOTE: a squarevalue can have negative values (-1) 
            actualb.hash32^=Zorbist[x][empty+1]^Zorbist[x][player+1];

            actualbtt.lock32^=Zorbist2[x][empty+1]^Zorbist2[x][player+1];
        }

        /**
         frees (zeroes) all entries */
        void FreeAll();
    
        /**
         increases the search stamp */
        void AdvanceSearchStamp();

        /**
         returns the log2 size of the table */
        int LogSize() const
        {
            return _logsize;
        }

    private:

        /** 
         default constructor allocates with DEF_LOG_SIZE logsize */
        explicit TranspositionTable();

        /**
         @return the hashcode from actualb.hash32 */
        unsigned int FastHash();

        /** size of the table */
        unsigned int _size; 
        /** log2 size */
        unsigned int _logsize; 
    
        /** the table itself */
        TTBoard *_arr;
    
        /** "time" or search stamp */
        unsigned int _stamp;
    };

    inline unsigned int TranspositionTable::FastHash()
    {    
        unsigned int tmp=actualb.hash32 >> (32-_logsize);
        assert(tmp>=0 && tmp<_size);
        return tmp;
    }

    inline TTBoard* TranspositionTable::Retrieve()
    {
        unsigned int ndx=FastHash();
        assert(actualb.hash32==Key32(actualb));
        assert(actualbtt.lock32==Lock32(actualb));
    
        //check if we have a valid entry
        if(_arr[ndx].lock32!=actualbtt.lock32 ||
           _arr[ndx].isblacksturn!=actualbtt.isblacksturn)
           return NULL;

        return &_arr[ndx];
    }
    
    inline void TranspositionTable::Store()
    {
        assert(actualbtt.GetDepth()!=0);
        unsigned int ndx=FastHash();

        //if this is an update then insert into the slot
        if(_arr[ndx].lock32==actualbtt.lock32 &&
           _arr[ndx].isblacksturn==actualbtt.isblacksturn)
        {
            _arr[ndx]=actualbtt;
            return;
        }

    
        /*we have a collision, do we overwrite or not? */

        //decide in favor of the one from the greather depth
        if(_arr[ndx].GetDepth() <= actualbtt.GetDepth())
        {
            _arr[ndx]=actualbtt;
            return;
        }
    
        //if entry is from a previous search then overwrite
        if(_arr[ndx].SearchStamp()!=_stamp)
        {
            _arr[ndx]=actualbtt;
            return;
        }
        //TODO TODO TODO TODO TODO TODO TODO 
        //TODO -can I do something more here?
        //TODO TODO TODO TODO TODO TODO TODO 
    }
}
#endif //_TTABLE_H_
