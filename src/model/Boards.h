/**
 @file
 Various board representations.
*/
#if !defined _BOARDS_H_
#define _BOARDS_H_
#include <cassert>
#include <cstring>

namespace Othello
{
    
    /**
      a large value, the maximum score possible */
    const int infinity=128;

    /**
      square names, s mean sentinel, p means playable */
    enum squareindex
    {
        s00, s01, s02, s03, s04, s05, s06, s07, s08, s09,
        s10, p11, p12, p13, p14, p15, p16, p17, p18, s19,
        s20, p21, p22, p23, p24, p25, p26, p27, p28, s29,
        s30, p31, p32, p33, p34, p35, p36, p37, p38, s39,
        s40, p41, p42, p43, p44, p45, p46, p47, p48, s49,
        s50, p51, p52, p53, p54, p55, p56, p57, p58, s59,
        s60, p61, p62, p63, p64, p65, p66, p67, p68, s69,
        s70, p71, p72, p73, p74, p75, p76, p77, p78, s79,
        s80, p81, p82, p83, p84, p85, p86, p87, p88, s89,
        s90, s91, s92, s93, s94, s95, s96, s97, s98, s99
    };

    /**
      square values are stored using one byte */
    typedef signed char squarevalue;

    /**
      an empty square, encoded with 0 */
    const squarevalue empty=0;

    /**
      a square with a black disc, encoded with 1 */
    const squarevalue black=1;

    /**
      a square with a white disc, encoded with -1 */
    const squarevalue white= -black;

    /**
      a sentinel square, this is placed around the board */
    const squarevalue sentl=3;
    
    /**
      gui helper trash, means a legal square for the side in turn */
    const squarevalue _gui__possible=4; 

    /**
      gui helper trash, means a hinted square for the player */
    const squarevalue _gui__hint=5;  


    /**
     see Michael Buro: "An Evaluation Function for Othello Based on 
     Statistics" */
    struct PreComp
    {
        int *Index;      /**< pointer to a pattern index*/
        int PlaceOffset; /**< distance from current pattern index and that after a place */
        int FlipOffset;  /**< distance from current pattern index and that after a flip*/
    
        PreComp(int *index, int place)
            :Index(index), PlaceOffset(place), FlipOffset(2*place)
        {}
    
        PreComp()
        {}
    };

    /** 
      @enum GameValueType different game value types */
    enum GameValueType
    {
        notvalid,   /**< not a valid value. */
        lowerbound, /**< a lower bound on the position value. */
        upperbound, /**< an upper bound on the position value. */
        exact       /**< an exact position value. */
    };


    /**
     describes a TTBoard (entry into transposition table).
     NOTE: sizeof(TTBoard) MUST BE EXACTLY 8 bytes !!!!!!!!!
     convincing the VC++ 6.0 compiler to use no aligment on this structure
     had proven to be a genuine nightmare, pack options are all but useless 
     and who would have thought that rearanging the members would produce 
     differents sizes, not a hint of that in MSDN. Hope that gcc will be more
     helpfull. */
    #pragma pack(push, 1)
    class TTBoard
    {
    private:
        /** identifies this board, almost unique */
        unsigned int lock32;

        /** best move is, 6 bits (if position is a leave then this 
         field has no meaning */
        unsigned char _bmove:6;

        /** what type of value do we strore here:
           notvalid, lowerbound, upperbound, exactvalue */
        unsigned char _vtype:2;

        /**from how deep did we got the value of this node
          NOTE: I want to prevent a re-search of a node searched in a previous
          pass (MT) in the current ID iteration; range (0..31)  5 bites */
        unsigned char _depth:5;

    public:
        /** whos turn is it */
        unsigned char isblacksturn:1;

    private:
        /** "time" or search stamp */
        unsigned char _stamp:2;

        /** board configuration value: it may be the true value
         an upper bound or a lower bound over it. range is -64:64, 
         bigger or less marks a win; 8 bits */
        signed char _value;

        /**
          private trash, 8 bites to make this structure nicely word aligned
          TODO: use this to store the other bound */
        char _trash:8;

    public:

        /**
         @return search stamp of this entry */
        unsigned int SearchStamp() const
        {
            return _stamp;
        }
 
        /**
         sets the lock to a given value */
        void SetLock32(unsigned int lock)
        {
            lock32=lock;
        }

        /**
         sets the best move */
        void SetBestMove(int x)
        {
            assert(Persil[x]!=-1);
            _bmove=Persil[x];
        }

        /**
         @return the best move */
        unsigned int BestMove() const
        {
            //best move is stored 0..63, but the engine uses 0..99 so
            //do a conversion
            return AntiPersil[_bmove];
        }
    
        /**
          @return the value of this position */
        int Value() const
        {
            return _value;
        }
    
        /**
          @return an upper bound over the value of the node
          if there is no such defined then returns +infinity */
        int UpperBound() const
        {
            //if exact upper bound or value then return value
            if(_vtype==upperbound || _vtype==exact) 
                return _value;
            //else (not valid or lower bound) return infinity
            return infinity;
        }

        /**
         sets an upperbound, from the given depth */ 
        void SetUpperBound(int value, unsigned int depth)
        {
            assert(value>=-127 && value<=127);
            //if new value is shalower then no update is needed
            if(_depth>depth)
                return;
            //if new value is deeper then always perform the update
            else if(_depth<depth)
            {
                _value=static_cast<signed char>(value);
                //mark that we have an upper bound
                _vtype=upperbound;
                //set new dept
                _depth=static_cast<unsigned char>(depth);
                return;
            }
            //we have an update from equal depth
            switch(_vtype)
            {
                case notvalid:
                    //do update
                    _value=static_cast<signed char>(value);
                    //mark that we have an upper bound
                    _vtype=upperbound;
                    break;
                case lowerbound:
                    //if bounds collide then we have an exact value
                    if(_value==value)
                        _vtype=exact;
                    else
                    {	
                        //owerwrite old entry
                        //TODO can I do something more here ???
                        _value=static_cast<signed char>(value);
                        //mark that we have an upper bound
                        _vtype=upperbound;
                    }
                    break;
                case upperbound:
                    //if new bound is tighter then update
                    if(_value>value)
                        _value=static_cast<signed char>(value);
                    break;
                case exact:
                    //we have an exact value, do nothing
                    break;
            }
        }
    
        /**
         sets an exact game value for this position */
        void SetExact(int value, int depth)
        {
            _value=static_cast<signed char>(value);
            _vtype=exact;
            _depth=static_cast<unsigned char>(depth);
        }

        /** 
          @return a lower bound over the value of the node 
          if undefined then returns -infinity */
        int LowerBound() const
        {
            //if exact value or lower bound then return value
            if(_vtype==lowerbound || _vtype==exact)
                return _value;
            //else (if not valid or upper bound) return -infnity;
            return -infinity;
        }

        /**
         sets a lowerbound, from the given depth */ 
        void SetLowerBound(int value, unsigned int depth)
        {
            assert(value>=-127 && value<=127);
            //if new value is shalower then no need for an update
            if(_depth>depth)
                return;
            //if new value is deeper then always perform the update
            else if(_depth<depth)
            {
                _value=static_cast<signed char>(value);
                //mark that we have an lower bound
                _vtype=lowerbound;
                //set new depth
                _depth=static_cast<unsigned char>(depth);
                return;
            }

            //we have an update from equal depth
            switch(_vtype)
            {
                case notvalid:
                    //no value was stored here, so perform the update
                    _value=static_cast<signed char>(value);
                    //mark that we have an lower bound
                    _vtype=lowerbound;
                    break;
                case lowerbound:
                    //if new bound is tighter then update
                    if(_value<value)
                        _value=static_cast<signed char>(value);
                    break;
                case upperbound:
                    //if bounds collide then we have an exact value
                    if(_value==value)
                        _vtype=exact;
                    else
                    {	
                        //owerwrite old entry
                        //TODO can I do somthing more here ???
                        _value=static_cast<signed char>(value);
                        //mark that we have an lower bound
                        _vtype=lowerbound;
                    }
                    break;
                case exact:
                    //we have an exact value, do nothing
                    break;
            };
        }
    
        /**
        sets the depth of this position */
        void SetDepth(unsigned int depth)
        {
            assert(depth>=0 && depth<=31);
            _depth=static_cast<unsigned char>(depth);
        }
    
        /**
         @return how deep was this position searched */
        unsigned int GetDepth() const
        {
            return static_cast<unsigned int>(_depth);
        }

        /**
         strips this entry of (almost) all info */
        void Clean()
        {
            _depth=0;
            _vtype=notvalid;
        }
    
        /**
         @return true only if position has an exact value */
        bool HasExact() const
        {
            return _vtype==exact;
        }

        /** translates: 10*10 ==> 8*8 */
        static signed char Persil[10*10]; //TODO this doesn't quite belong here

        /** translates: 8*8 ==> 10*10 */
        static squareindex AntiPersil[8*8];  //TODO this doesn't quite belong here

        //Friends will be friends...
        friend class TranspositionTable;
    };
    //reset pack option
    #pragma pack(pop)


    /**
      class Board, stores the board configuration */ 
    class Board
    {
    //no privacy here neither
    public:	        
        /** 8 by 8 table surounded with sentinel values, to speedup
        movegeneration. */
        squarevalue table[10*10];

        /**8x8 ought to be enough, but indexing would differ
         altough I waist almost 2kb this way */
        PreComp updatetable[10*10][5];
                                 
        explicit Board();
    
        /**in Othello it is posible that two or more moves are done
         by one side if the other does not have any options. */
        bool isblacksturn;

        /** hash code */
        unsigned int hash32;

        /** number of empties on this board */
        unsigned int empties;

        /** number of blacks on this board */
        unsigned int blacks;
    
        /**
          initializes the precomputed data */
        void InitPrecomp();

        /**
         syncronize pattern indices with the current board state
         this is an expensive method. */
        void Syncronize();

#if !defined NDEBUG
        //DEBUG
        bool AssertIsInSync();
        void DebugPrintBoard(int x);
#endif
        /**
         @return number of empty squares */
        unsigned int CountEmpty() const
        {
            return empties;
        }

        /**
         @return number of black discs */
        unsigned int CountBlack() const
        {
            return blacks;
        }

        /**
         @return number of white discs */
        unsigned int CountWhite() const
        {
            return 64-empties-blacks;
        }
    
        /**
         @return true only if this board is not infested with GUI 
         helper values */
        bool IsClean() const;

        /** 
         initalize the board with the start position */
        void InitBoard();
    
        //@{
        /** pattern indeces,  lines of length 8*/
        int line1, line2, line3, line4, line5, line6, line7, line8; //@}
        //@{
        /** pattern indeces,  colunms of length 8*/
        int colA, colB, colC, colD, colE, colF, colG, colH; //@}
        //@{
        /** pattern indeces, diagonals of length 8 */
        int diag8A1H, diag1A8H; //@}
        //@{
        /** pattern indeces, diagonals of length 7 */
        int diag7A1G, diag2A8G, diag2H8B, diag7H1B; //@}
        //@{
        /** pattern indeces, diagonals of length 6 */
        int diag6A1F, diag3A8F, diag3H8C, diag6H1C; //@}
        //@{
        /** pattern indeces, diagonals of length 5 */
        int diag5A1E, diag4A8E, diag4H8D, diag5H1D; //@}
        //@{
        /** pattern indeces, diagonals of length 4 */
        int diag4A1D, diag5A8D, diag5H8E, diag4H1E; //@}
        //@{
        /** pattern indeces, 2x4 corners */
        int cornA1D2, cornA1B4, cornA8B5, cornA8D7, 
            cornH8E7, cornH8G5, cornH1G4, cornH1E2; //@}
        //dummy, is this necessary?
        int dummy;

        
        // multiplications by powers of 3 to calcualte the pattern indeces. 
        int Mul4(int p1, int p2, int p3, int p4);
        int Mul5(int p1, int p2, int p3, int p4, int p5);
        int Mul6(int p1, int p2, int p3, int p4, int p5, int p6);
        int Mul7(int p1, int p2, int p3, int p4, int p5, int p6, int p7);
        int Mul8(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8);
        int Diag3_1() const
        {
            return 3*(3*table[p31]+table[p22])+table[p13];
        }
        int Diag3_2() const
        {
            return 3*(3*table[p61]+table[p72])+table[p83];
        }
        int Diag3_3() const
        {
            return 3*(3*table[p38]+table[p27])+table[p16];
        }
        int Diag3_4() const
        {
            return 3*(3*table[p68]+table[p77])+table[p86];
        }
    
        /** the start up board */
        static squarevalue sboard[10*10];
        
        /** table to generate symetrics by the first diagonal */
        static unsigned char sym_diag1[10*10];
        /** table to generate symetrics by the second diagonal */
        static unsigned char sym_diag2[10*10];
        /** table to generate central symetrics */
        static unsigned char sym_centr[10*10]; 
    };

    /**
     Board representation in the book file, packed in 20 bytes
     this is an actual entry into the Book. */
    class BookBoard
    { //perhaps this should be an inner class of Book
    private:
       
        /**
         only fields l and isblacksturn are initialized */
        BookBoard& operator= (const Board& b);

        /**
         only fields l and isblacksturn are compared */
        bool operator== (const BookBoard &bb)
        {
            return (!memcmp(l, bb.l, sizeof(l))) && 
                    bb.isblacksturn==isblacksturn;
        }
        
        /** 
         bitboard representation 00 means black, 
          01 means white, 10 means empty */
        int l[4];

        /** who's turn is it? */
        unsigned char  isblacksturn:1;

        /** is this a valid entry? (see class Book) */
        unsigned char isvalid:1;

        /** best move in 0..63 format*/
        unsigned char bmove:6;
        
        /** for now trash */
        unsigned char ischanged:1; 

        /** do we have an alternative (second best) move */
        unsigned char isaltmovevalid:1;

        /** the alternative move */
        unsigned char altmove:6;
        
        /** minmax value */
        signed char value;

        /** for now trash */
        char trash;

        //masks to extract board from bitboard representation
        static int maskwhite[];
        static int maskblack[];
        static int maskempty[];

        //I like friends
        friend class Hasher; 
        friend class Book;
    };
}
#endif //_BOARDS_H_

