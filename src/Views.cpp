/**
 @file
 Implements the View part of the Model-View-Controller pattern.
*/

#include <sstream>
#include "Views.h"
#include "lib/File.h"
#include "model/MoveListGenerator.h"
#include "model/MoveExecuter.h"

using namespace Win;
using namespace Othello;


BoardView::BoardView(HWND parentHWND)
    : SimpleControl(parentHWND, IDS_BOARD_PANE),
      _possbrush(RGB(0, 83, 166)),
      _emptybrush(RGB(0, 64, 128)),
      _hintbrush(RGB(255, 64, 128)),
      _bpiece1(RGB(35, 35, 35)),
      _ppiece1(RGB(35, 35, 35)),  
      _bpiece2(RGB(209, 209, 209)),
      _ppiece2(RGB(209, 209, 209))
{
    InitBoards();
}


void BoardView::InitBoards()
{
    _board.InitBoard();
    _game_over=false;
    _double_move=false;
    MoveEnumerator emptymarker(_board);
    UpdateCanvas canvas(_hWnd);
    Init(canvas);
}

void BoardView::Init(Canvas &canvas)
{
    const ClientRect& _rect=Rect();
    _buffer.CreateCompatible(canvas, _rect.right-_rect.left, 
                                     _rect.bottom-_rect.top);
    BitmapCanvas bmpcanvas(canvas, _buffer);
    int dx=(_rect.right-_rect.left)/8;
    int dy=(_rect.bottom-_rect.top)/8;
    RECT rect;
    for(int x=0; x<8; x++)
        for(int y=0; y<8; y++)
        {         
            //RECT(left,top,right,bottom)
            int position=TTBoard::AntiPersil[x*8+y];
            ::SetRect(&rect,x*dx,y*dy,(x+1)*dx,(y+1)*dy);
            switch(_board.table[position])
            {
            case _gui__possible:
                bmpcanvas.FillRect(rect, _possbrush);
                break;
            case _gui__hint:
                bmpcanvas.FillRect(rect, _hintbrush);
                break;
            case empty:
            case black:
            case white:
                bmpcanvas.FillRect(rect, _emptybrush);
                break;
            }
            
            if(_board.table[position]==black)
            {
                BrushHolder brhold(bmpcanvas, _bpiece1);
                PenHolder pnhold(bmpcanvas, _ppiece1);
                bmpcanvas.Ellipse(dx*x+5, dx*y+5, dx*(x+1)-5, dx*(y+1)-5);
            }
            if(_board.table[position]==white)
            {
                BrushHolder brhold(bmpcanvas, _bpiece2);
                PenHolder pnhold(bmpcanvas, _ppiece2);
                bmpcanvas.Ellipse(dx*x+5, dx*y+5, dx*(x+1)-5, dx*(y+1)-5);
            }
        }

    for(int i=1; i<=8; i++)
    {
        bmpcanvas.Line(i*dx, _rect.top, i*dx, _rect.bottom);
        bmpcanvas.Line(_rect.left, i*dy, _rect.right, i*dy);
    }
}

void BoardView::Hinted(int move)
{
    for(unsigned int i=p11; i<s89; i++)
        if(_board.table[i]==_gui__hint)
            _board.table[i]=_gui__possible;

    _board.table[move]=_gui__hint;
    UpdateCanvas canvas(_hWnd);
    Init(canvas);
    InvalidateOnParent();
}

void BoardView::Paint(Canvas &canvas)
{
    Blitter blitter(_buffer);
    blitter.BlitTo(canvas);
}

void BoardView::PixelToBoard(POINT &point)
{
    const ClientRect& _rect=Rect();
    int dx=(_rect.right-_rect.left)/8;
    int dy=(_rect.bottom-_rect.top)/8;
    point.x/=dx;
    point.y/=dy;
    point.x++;
    point.y++;
}


void BoardView::Clean()
{  
    for(unsigned int i=p11; i<s89; i++)
        switch(_board.table[i])
        {
        case black:
        case empty:
        case white:
        case sentl:
            break;
        default:
            _board.table[i]=empty;
            break;
    }
}

bool BoardView::IsGameOver()
{
    return _game_over; 
} 

//places the users piece at (x,y) returns true if is legal
//false if not
bool BoardView::PutPiece(int x,int y, squarevalue player)
{
    
    int position=TTBoard::AntiPersil[(x-1)*8+y-1];
    //if not legal then return false
    if(_board.table[position]!=_gui__possible &&
       _board.table[position]!=_gui__hint)
        return false;

    //this is a legal move so we are going to repaint the board
    UpdateCanvas canvas(_hWnd);

    //clean the board, get rid of _gui_ stuff
    Clean();

    //execute the move on my private board
    GuiMoveExecuter(_board, position);

    //generate movepossibilities for the computer
    //NOTE this is done only for the gui
    MoveEnumerator moves(_board);
    
    //if we found some moves, then return and let the computer think
    if(!moves.Empty())
    {
        _double_move=false;
        _game_over=false; //???
        Init(canvas);
        return true;
    }

    //clean the board, get rid of _gui_ stuff
    Clean();

    //nop we found no moves for computer that means that is still our turn!
    //so generate moves
    _board.isblacksturn=!_board.isblacksturn;
    MoveEnumerator moves2(_board);
            
    //if we did not found any that means that the game is over, before the
    //last square was played.
    if(moves2.Empty())
    {
        _game_over=true;
        _double_move=false; //???
        Init(canvas);
        return true;
    }

    //we have a double move 
    _double_move=true;
    _game_over=false; //????
    Init(canvas);
    //Paint(canvas);
    return true;
}

void BoardView::Get(Board &board)
{
    //update board
    _board=board;
    MoveEnumerator moves(_board);
    if(moves.Empty())
    {
        _game_over=true;
        _double_move=false; //???
    }
    //(re)initialize view
    UpdateCanvas canvas(_hWnd);
    Init(canvas);
}

void BoardView::SendTo(Board &board)
{
    for(unsigned int i=p11; i<s89; i++)
        if(_board.table[i]==_gui__possible||_board.table[i]==_gui__hint)
            board.table[i]=empty;
        else 
            board.table[i]=_board.table[i];

    board.isblacksturn=_board.isblacksturn;
    UpdateCanvas canvas(_hWnd);
    Init(canvas);
}

void BoardView::SaveToFile(PSTR filename)
{
    
    // sizeof("White")==sizeof("Black")
    char buffer[8*8+sizeof("Black")];
     
    for(int x=p11; x<s89; x++)
        switch(_board.table[x])
        {
            case empty:
            case _gui__possible:
            case _gui__hint:
                buffer[TTBoard::Persil[x]]='-';
                break;
            case black:
                buffer[TTBoard::Persil[x]]='X';
                break;
            case white:
                buffer[TTBoard::Persil[x]]='O';
                break;
        }
            
    if(_board.isblacksturn)
        strcpy(buffer+8*8,"Black");
    else
        strcpy(buffer+8*8,"White");

    File ofile(filename, File::write);
    ofile.Write(buffer);
}


bool BoardView::LoadFromFile(PSTR filename, Board &tmp)
{
    //sizeof("White")==sizeof("Black")
    char buffer[8*8+sizeof("Black")];
    ::ZeroMemory(buffer,8*8+sizeof("Black"));

    File ifile(filename, File::read);
    ifile.Read(buffer, sizeof(buffer));

    if(strlen(buffer) != sizeof(buffer)-1)
        return false;
    int empties=0;
    int blacks=0;
    for(int i=0; i<8*8; i++)
        switch(buffer[i])
        {
            case '-':
                tmp.table[TTBoard::AntiPersil[i]]=empty;
                empties++;
                break;
            case 'X':
                tmp.table[TTBoard::AntiPersil[i]]=black;
                blacks++;
                break;
            case 'O':
                tmp.table[TTBoard::AntiPersil[i]]=white;
                break;
            default:
                return false;
        }
    
    switch(buffer[8*8])
    {
        case 'B':
            tmp.isblacksturn=true;
            break;
        case 'W':
            tmp.isblacksturn=false;
            break;
        default:
            return false;
    }
    tmp.empties=empties;
    tmp.blacks=blacks;
    return true;
}


InfoView::InfoView(HWND parentHWND, 
                   bool &isUsersTurn, 
                   bool &isGameStarted, 
                   bool &isUserBlack,
                   bool &isGameOver)
    : SimpleControl(parentHWND,IDC_INFO_PANEL),
      _isUsersTurn(isUsersTurn), 
      _isGameStarted(isGameStarted),
      _isUserBlack(isUserBlack),
      _isGameOver(isGameOver),
      _pnrmoves(NULL)
{
    Clean();
    UpdateBuffer();
}


void InfoView::Clean()
{
    _gamevalue=0;
    _bestMove=-1;
    _posSearched=0;
    _ttHits=0;
    _ttGHits=0;
    _depth=0;
    _moves=NULL; 
    if(_pnrmoves!=NULL)
        *_pnrmoves=0;
    _inBook=true;
    _timedOut=false;
}

void InfoView::LookInside(Thinker &model)
{
    _gamevalue=model.gamevalue;
    _bestMove=model.bestMove;
    _posSearched=model.posSearched;
    _ttHits=model.ttHits;
    _ttGHits=model.ttGHits;
    _depth=model.IdDepth;
    _moves=model.moves;    //overkill
    _pnrmoves= &model.nrMoves;//overkill
    _timedOut=model.timedout;
    _inBook=!model.OutOfBook();
    if(!_isUsersTurn)
        _mseconds=model.MSecRemaining();
    UpdateBuffer();
}

char *InfoView::_msgShylockIsThinking="Shylock is thinking";
char *InfoView::_msgYourTurn="Your turn";
char *InfoView::_msgBestMove="best move: ";
char *InfoView::_msgDepth="depth: ";
char *InfoView::_msgGameValue="game value: ";
char *InfoView::_msgNodes="nodes: ";
char *InfoView::_msgTTHits="ttable hits: ";
char *InfoView::_msgMoves="history: ";
char *InfoView::_msgTime="time: ";
char *InfoView::_msgTimedOut="Timed out!";
char *InfoView::_msgGameOver="Game over!";
char *InfoView::_msgFromBook="In book!";

void InfoView::Paint(Canvas &canvas)
{
    Blitter blitter(_buffer);
    blitter.BlitTo(canvas);
}

void InfoView::UpdateBuffer()
{
    //TODO clean up this mess!

    UpdateCanvas canvas(Hwnd());

    ClientRect& _rect=Rect();

    _buffer.CreateCompatible(canvas, _rect.right-_rect.left, 
                                     _rect.bottom-_rect.top);

    BitmapCanvas bmpcanvas(canvas, _buffer);

    bmpcanvas.WhiteWash(_rect);
    bmpcanvas.SetTextColor(RGB(0, 0, 128));
    bmpcanvas.SetBkrndColor(RGB(255, 255, 255));

    std::stringstream out;

    if(_isGameStarted || _isGameOver)
    {
        if(!_isGameOver)
        {
            if(_isUsersTurn)
                out<<_msgYourTurn;
            else
                out<<_msgShylockIsThinking;
        }
        else 
            out<<_msgGameOver;
        
        if(!_timedOut)
        {
            if(!_isUsersTurn)
            {
                unsigned int tmp=_mseconds / 1000;
                unsigned int sec_part=tmp % 60;
                unsigned int min_part=tmp / 60;
                out<<std::endl<<_msgTime<<min_part<<":";
                out.fill('0'); out.width(2); out<<sec_part;
                out.width(0);
            }
        }
        else
            out<<std::endl<<_msgTimedOut;
        
        if(!_inBook)
        {
            out<<std::endl<<"game value: ";
            if(_gamevalue>63)
                out<<_gamevalue-63<<"!";
            else
                if(_gamevalue<-63)
                    out<<_gamevalue+63<<"!";
                else
                    out<<_gamevalue;
            out<<std::endl;

            if(!_isGameOver)
            {

                if(!_isUsersTurn && _bestMove!=-1)
                {
                    assert(SquareNames[_bestMove]!=NULL);
                    out<<_msgBestMove<<SquareNames[_bestMove]<<std::endl;
                }
        
                out<<_msgDepth<<_depth<<std::endl;

                out<<_msgNodes<<_posSearched<<std::endl;

                out<<_msgTTHits<<_ttHits<<std::endl;

                out<<"ttghits: "<<_ttGHits<<std::endl;

                if(double(_ttHits)!=0)
                    out<<"r="<<double(_posSearched)/double(_ttHits)<<std::endl;

                if(double(_ttGHits)!=0)
                    out<<"rg="<<double(_posSearched)/double(_ttGHits)<<std::endl;
            }
        }
        else
            out<<std::endl<<_msgFromBook<<std::endl;    

        if(_moves!=NULL && *_pnrmoves!=0)
        {
            out<<_msgMoves;
            for(int i=0; i< *_pnrmoves; i++)
            {
                assert(SquareNames[_moves[i].x]!=NULL);
                out<<(_moves[i].isblack?"+":"-")
                   <<SquareNames[_moves[i].x]<<" ";
            }
            out<<std::endl;
        }
    }

    bmpcanvas.Text(out.str().c_str(), _rect, 
                   DT_EDITCONTROL|DT_LEFT|DT_WORDBREAK|
                   DT_EXPANDTABS|DT_TABSTOP|0x00000400);
}


void InfoView::SaveGameToFile(PSTR filename)
{
    std::stringstream out;
    for(int i=0; i< *_pnrmoves; i++)
    {
        assert(SquareNames[_moves[i].x]!=NULL);
        out<<(_moves[i].isblack?"+":"-")
           <<SquareNames[_moves[i].x]<<" ";
    }
    out<<std::endl;
    File ofile(filename, File::write);
    ofile.Write(out.str().c_str());
}

char* InfoView::SquareNames[10*10]=
{
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,"a1","a2","a3","a4","a5","a6","a7","a8",NULL,
    NULL,"b1","b2","b3","b4","b5","b6","b7","b8",NULL,
    NULL,"c1","c2","c3","c4","c5","c6","c7","c8",NULL,
    NULL,"d1","d2","d3","d4","d5","d6","d7","d8",NULL,
    NULL,"e1","e2","e3","e4","e5","e6","e7","e8",NULL,
    NULL,"f1","f2","f3","f4","f5","f6","f7","f8",NULL,
    NULL,"g1","g2","g3","g4","g5","g6","g7","g8",NULL,
    NULL,"h1","h2","h3","h4","h5","h6","h7","h8",NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
};
