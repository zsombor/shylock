/**
 @file
 A definition of a datastructure used to store undo data.
*/
#if !defined _UNDODATA__H_
#define _UNDODATA__H_
#include "Boards.h"

namespace Othello
{

    /**
     data structure to hold info about how a move must be undone in the
     expanded board representation. Altough I could have encapsuleted this
     in the MoveExecuter, it would have not been as efficient as a weak
     (pointer, or reference based) encapsulation, so it is neccessary to 
     have this structure defined separatly. */
    struct UndoData
    {

        squarevalue* Prev[32]; /**< pointers to the squares modified */
        unsigned int count;    /**< number of squares modified */
        UndoData()
            :count(0)
        {}
    };
}
#endif //_UNDODATA__H_

