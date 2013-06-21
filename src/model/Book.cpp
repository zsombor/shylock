/**
 @file 
 Code for book based move recomandation.
*/
#include <cstring>
#include <cstdlib>
#include "Book.h"
#include "../lib/WinException.h"

using namespace Othello;

#if !defined NDEBUG
#include <windows.h>
#include "MoveListGenerator.h"
void Book::Print(BookBoard &b)
{
    ::OutputDebugString("***\n");
    for(int l=0; l<4; l++)
        for(int i=0; i<16; i++)
        {
            if(b.l[l] & BookBoard::maskempty[i])
                ::OutputDebugString(".");
            else
                if(b.l[l] & BookBoard::maskwhite[i])
                    ::OutputDebugString("W");
                else
                    ::OutputDebugString("B");
            if(i%8==7)
                ::OutputDebugString("\n");
        }
}
#endif

Book::Book()
{
    if(NULL==(_f=fopen("book.bin.", "rb")))
        _book_is_present=false;
    else
    {    
        setbuf(_f, NULL); //is it wort it?
        _book_is_present=true;
        _out_of_book=false;
    }
}

Book::~Book()
{
    if(_book_is_present)
        fclose(_f);
}


void Book::GenSym(const Board &b)
{
    for(unsigned int i=p11; i<s89; i++)
    {
        _sym[0].table[i] = b.table[ Board::sym_diag1[i] ];
        _sym[1].table[i] = b.table[ Board::sym_diag2[i] ];
        _sym[2].table[i] = b.table[ Board::sym_centr[i] ];
        _sym[3].table[i] = b.table[i];
    }

    for(int t=0; t<4; t++)
    {
        _sym[t].isblacksturn = b.isblacksturn;
        _symbb[t] = _sym[t];
        _h1[t]= Hash1(_symbb[t]) >> (32-_logsize) ;
        _h2[t]= (Hash2(_symbb[t]) >> (32-_logsize)) | 1;
#if !defined NDEBUG
        Print(_symbb[t]);    
#endif
    }
}

squareindex Book::AdvisedMove(const Board &b)
{
    assert(sizeof(BookBoard)==20);

    if(!_book_is_present)
        return s00;

    const int move_nr=b.CountBlack() + b.CountWhite() - 4;
    
    if(_out_of_book && move_nr>10)
        return s00;

    _out_of_book=true;

    //book has entrys for only the first 18 moves
    if(move_nr > 18)
        return s00;

    //generate symetries
    GenSym(b);

    //locate board in the book
    BookBoard cur;
    int id= -1;
    bool failed[4];
    memset(failed, false, sizeof(failed));
    for(unsigned int probes=0; 
        !(failed[0] && failed[1] && failed[2] && failed[3]) && id==-1; 
        probes++)
    {
        for(int t=3; t>=0; t--)
            if(!failed[t])
            {
                Read(cur, (_h1[t] + probes * _h2[t]) % _size);
                if(!cur.isvalid)
                {
                    failed[t]=true;
                    continue;
                }
#if !defined NDEBUG
                 Print(cur);
#endif
                if(cur==_symbb[t])
                {
                    id=t;
                    break;
                }
            }
    }

    //not found 
    if(id==-1)
        return s00;

    _out_of_book=false;

    _symbb[3].isaltmovevalid = cur.isaltmovevalid;
    _symbb[3].isvalid = cur.isvalid;
    _symbb[3].value = cur.value;

    //transform bmove, altmove to reflect the symetrics
    switch(id)
    {
    case 3:
        //identical maping
        _symbb[3].bmove = cur.bmove;
        _symbb[3].altmove = cur.altmove;
        break;
    case 2:
        //central symetrics
        _symbb[3].bmove = TTBoard::Persil[Board::sym_centr[ TTBoard::AntiPersil[ cur.bmove ]]];
        _symbb[3].altmove = TTBoard::Persil[Board::sym_centr[ TTBoard::AntiPersil[ cur.altmove ]]];
        break;
    case 1:
        //symetrics by the second diagonal
        _symbb[3].bmove = TTBoard::Persil[Board::sym_diag2[ TTBoard::AntiPersil[ cur.bmove ]]];
        _symbb[3].altmove = TTBoard::Persil[Board::sym_diag2[ TTBoard::AntiPersil[ cur.altmove ]]];
        break;
    case 0:
        //symetrics by the first diagonal
        _symbb[3].bmove = TTBoard::Persil[Board::sym_diag1[ TTBoard::AntiPersil[ cur.bmove ]]];
        _symbb[3].altmove = TTBoard::Persil[Board::sym_diag1[ TTBoard::AntiPersil[ cur.altmove ]]];
        break;
    }

#if !defined NDEBUG
    IsLegalMove legal(_sym[3]);
    int dir;
    assert(legal(TTBoard::AntiPersil[ _symbb[3].bmove ], dir));
#endif
    
    //if no alternative move avaible then return the best move
    if(!_symbb[3].isaltmovevalid)
        return TTBoard::AntiPersil[ _symbb[3].bmove ];

    double p=rand()/RAND_MAX;
    if(p > _prob_rand_depth[move_nr])
        return TTBoard::AntiPersil[ _symbb[3].altmove ];

    return TTBoard::AntiPersil[ _symbb[3].bmove ];
}

void Book::Read(BookBoard &entry, unsigned int address)
{
    fseek(_f, address*sizeof(BookBoard), SEEK_SET);
    fread(&entry, sizeof(BookBoard), 1, _f);
}

//the probability to select the best move
double Book::_prob_rand_depth[]=
{
    0.75, 0.85, 0.95,  0.95, 0.95,
    0.96, 0.96, 0.96, 0.96, 0.96,
    0.97, 0.97, 0.97, 0.97, 0.97,
    0.98, 0.98, 0.98, 0.98, 0.98,
    0.99, 0.99, 0.99, 0.99, 0.99
};

const unsigned int Book::_logsize=20;
const unsigned int Book::_size=(1<<20);

bool Book::OutOfBook() const
{
    if(_book_is_present)
        return _out_of_book;
    return true;
}

void Book::Reset(const Board& b)
{
    const int move_nr=b.CountBlack() + b.CountWhite() - 4;
    
    //book has entrys for only the first 18 moves
    _out_of_book = move_nr > 18;
}

