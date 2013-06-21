/**
 @file
 The implementation of different board representations.
*/

/* It looks ugly, it is ugly. This was more or  less generated so do 
** not edit.
*/
#include <cstring>
#include "Boards.h"

using namespace Othello;

Board::Board() 
{
    InitBoard();
    InitPrecomp();
}

void Board::InitBoard()
{
    memcpy(table, sboard, sizeof(sboard));
    isblacksturn=true;
    blacks=2;
    empties=60;
}

bool Board::IsClean() const
{
    for(unsigned int i=p11; i<s89; i++)
        switch(table[i])
        {
        case black:
        case empty:
        case white:
        case sentl:
            continue;
        default:
            return false;
        }
    return true;
}


BookBoard& BookBoard::operator= (const Board& b)
{
    memset(l, 0, 4*sizeof(l[0]));
    isblacksturn=b.isblacksturn;
    
    for(unsigned i=p11; i<s89; i++)
    {
        int x= i / 10;
        int y= i % 10;
        switch(b.table[i])
        {
        case white:
            l[(x-1)/2] |= maskwhite[y-1+8*((x-1)%2)];
            break;
        case black:
            l[(x-1)/2] &= maskblack[y-1+8*((x-1)%2)];
            break;
        case empty:
            l[(x-1)/2] |= maskempty[y-1+8*((x-1)%2)];
            break;
        }
    }
    return *this;
}

int Board::Mul4(int p1, int p2, int p3, int p4)
{
    return 3*(3*(3*table[p1]+table[p2])+table[p3])+table[p4];
}

int Board::Mul5(int p1, int p2, int p3, int p4, int p5)
{
    return 3*(3*(3*(3*table[p1]+table[p2])+table[p3])+table[p4])
           +table[p5];
}

int Board::Mul6(int p1, int p2, int p3, int p4, int p5, int p6)
{
    return 3*(3*(3*(3*(3*table[p1]+table[p2])+table[p3])+table[p4])
           +table[p5])+table[p6];
}

int Board::Mul7(int p1, int p2, int p3, int p4, int p5, int p6, int p7)
{
    return 3*(3*(3*(3*(3*(3*table[p1]+table[p2])+table[p3])+table[p4])
           +table[p5])+table[p6])+table[p7];
}

int Board::Mul8(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8)
{
    return 3*(3*(3*(3*(3*(3*(3*table[p1]+table[p2])+table[p3])+table[p4])
           +table[p5])+table[p6])+table[p7])+table[p8];
}

void Board::Syncronize()
{
    //pattern 4
    diag4A1D= Mul4(p41, p32, p23, p14);
    diag5A8D= Mul4(p51, p62, p73, p84);
    diag4H1E= Mul4(p48, p37, p26, p15);
    diag5H8E= Mul4(p58, p67, p76, p85);

    //pattern 5
    diag5A1E= Mul5(p51, p42, p33, p24, p15);
    diag4A8E= Mul5(p41, p52, p63, p74, p85);
    diag5H1D= Mul5(p58, p47, p36, p25, p14);
    diag4H8D= Mul5(p48, p57, p66, p75, p84);
    
    //pattern 6
    diag6A1F= Mul6(p61, p52, p43, p34, p25, p16);
    diag3A8F= Mul6(p31, p42, p53, p64, p75, p86);
    diag3H8C= Mul6(p38, p47, p56, p65, p74, p83);
    diag6H1C= Mul6(p68, p57, p46, p35, p24, p13);

    //pattern 7
    diag7A1G= Mul7(p71, p62, p53, p44, p35, p26, p17);
    diag2A8G= Mul7(p21, p32, p43, p54, p65, p76, p87);
    diag2H8B= Mul7(p28, p37, p46, p55, p64, p73, p82);
    diag7H1B= Mul7(p78, p67, p56, p45, p34, p23, p12);

    //pattern 8
    diag1A8H= Mul8(p11, p22, p33, p44, p55, p66, p77, p88);
    diag8A1H= Mul8(p81, p72, p63, p54, p45, p36, p27, p18);
    
    line1= Mul8(p11, p12, p13, p14, p15, p16, p17, p18);
    line2= Mul8(p21, p22, p23, p24, p25, p26, p27, p28);
    line3= Mul8(p31, p32, p33, p34, p35, p36, p37, p38);
    line4= Mul8(p41, p42, p43, p44, p45, p46, p47, p48);
    line5= Mul8(p51, p52, p53, p54, p55, p56, p57, p58);
    line6= Mul8(p61, p62, p63, p64, p65, p66, p67, p68);
    line7= Mul8(p71, p72, p73, p74, p75, p76, p77, p78);
    line8= Mul8(p81, p82, p83, p84, p85, p86, p87, p88);

    colA= Mul8(p11, p21, p31, p41, p51, p61, p71, p81);
    colB= Mul8(p12, p22, p32, p42, p52, p62, p72, p82);
    colC= Mul8(p13, p23, p33, p43, p53, p63, p73, p83);
    colD= Mul8(p14, p24, p34, p44, p54, p64, p74, p84);
    colE= Mul8(p15, p25, p35, p45, p55, p65, p75, p85);
    colF= Mul8(p16, p26, p36, p46, p56, p66, p76, p86);
    colG= Mul8(p17, p27, p37, p47, p57, p67, p77, p87);
    colH= Mul8(p18, p28, p38, p48, p58, p68, p78, p88);

    //2x4 corners
    cornA1D2= Mul8(p11, p12, p13, p14, p21, p22, p23, p24);
    cornA1B4= Mul8(p11, p21, p31, p41, p12, p22, p32, p42);
    cornA8B5= Mul8(p81, p71, p61, p51, p82, p72, p62, p52);
    cornA8D7= Mul8(p81, p82, p83, p84, p71, p72, p73, p74);
    cornH8E7= Mul8(p88, p87, p86, p85, p78, p77, p76, p75);
    cornH8G5= Mul8(p88, p78, p68, p58, p87, p77, p67, p57);
    cornH1G4= Mul8(p18, p28, p38, p48, p17, p27, p37, p47);
    cornH1E2= Mul8(p18, p17, p16, p15, p28, p27, p26, p25);
    
    empties=0;
    blacks=0;
    for(unsigned int i=p11; i<s89; i++)
    {
        switch(table[i])
        {
        case empty:
            empties++;
            break;
        case black:
            blacks++;
            break;
        }
    }
}

#if !defined NDEBUG
//debug
bool Board::AssertIsInSync()
{
    //pattern 4
    assert(diag4A1D== Mul4(p41, p32, p23, p14));
    assert(diag5A8D== Mul4(p51, p62, p73, p84));
    assert(diag4H1E== Mul4(p48, p37, p26, p15));
    assert(diag5H8E== Mul4(p58, p67, p76, p85));

    //pattern 5
    assert(diag5A1E== Mul5(p51, p42, p33, p24, p15));
    assert(diag4A8E== Mul5(p41, p52, p63, p74, p85));
    assert(diag5H1D== Mul5(p58, p47, p36, p25, p14));
    assert(diag4H8D== Mul5(p48, p57, p66, p75, p84));
    
    //pattern 6
    assert(diag6A1F== Mul6(p61, p52, p43, p34, p25, p16));
    assert(diag3A8F== Mul6(p31, p42, p53, p64, p75, p86));
    assert(diag3H8C== Mul6(p38, p47, p56, p65, p74, p83));
    assert(diag6H1C== Mul6(p68, p57, p46, p35, p24, p13));

    //pattern 7
    assert(diag7A1G== Mul7(p71, p62, p53, p44, p35, p26, p17));
    assert(diag2A8G== Mul7(p21, p32, p43, p54, p65, p76, p87));
    assert(diag2H8B== Mul7(p28, p37, p46, p55, p64, p73, p82));
    assert(diag7H1B== Mul7(p78, p67, p56, p45, p34, p23, p12));

    //pattern 8
    assert(diag1A8H== Mul8(p11, p22, p33, p44, p55, p66, p77, p88));
    assert(diag8A1H== Mul8(p81, p72, p63, p54, p45, p36, p27, p18));
    
    assert(line1== Mul8(p11, p12, p13, p14, p15, p16, p17, p18));
    assert(line2== Mul8(p21, p22, p23, p24, p25, p26, p27, p28));
    assert(line3== Mul8(p31, p32, p33, p34, p35, p36, p37, p38));
    assert(line4== Mul8(p41, p42, p43, p44, p45, p46, p47, p48));
    assert(line5== Mul8(p51, p52, p53, p54, p55, p56, p57, p58));
    assert(line6== Mul8(p61, p62, p63, p64, p65, p66, p67, p68));
    assert(line7== Mul8(p71, p72, p73, p74, p75, p76, p77, p78));
    assert(line8== Mul8(p81, p82, p83, p84, p85, p86, p87, p88));

    assert(colA== Mul8(p11, p21, p31, p41, p51, p61, p71, p81));
    assert(colB== Mul8(p12, p22, p32, p42, p52, p62, p72, p82));
    assert(colC== Mul8(p13, p23, p33, p43, p53, p63, p73, p83));
    assert(colD== Mul8(p14, p24, p34, p44, p54, p64, p74, p84));
    assert(colE== Mul8(p15, p25, p35, p45, p55, p65, p75, p85));
    assert(colF== Mul8(p16, p26, p36, p46, p56, p66, p76, p86));
    assert(colG== Mul8(p17, p27, p37, p47, p57, p67, p77, p87));
    assert(colH== Mul8(p18, p28, p38, p48, p58, p68, p78, p88));

    assert(cornA1D2==Mul8(p11, p12, p13, p14, p21, p22, p23, p24));
    assert(cornA1B4==Mul8(p11, p21, p31, p41, p12, p22, p32, p42));
    assert(cornA8B5==Mul8(p81, p71, p61, p51, p82, p72, p62, p52));
    assert(cornA8D7==Mul8(p81, p82, p83, p84, p71, p72, p73, p74));
    assert(cornH8E7==Mul8(p88, p87, p86, p85, p78, p77, p76, p75));
    assert(cornH8G5==Mul8(p88, p78, p68, p58, p87, p77, p67, p57));
    assert(cornH1G4==Mul8(p18, p28, p38, p48, p17, p27, p37, p47));
    assert(cornH1E2==Mul8(p18, p17, p16, p15, p28, p27, p26, p25));

    unsigned int empties_=0;
    unsigned int blacks_=0;
    for(unsigned int i=p11; i<s89; i++)
    {
        switch(table[i])
        {
        case empty:
            empties_++;
            break;
        case black:
            blacks_++;
            break;
        }
    }
    assert(empties==empties_);
    assert(blacks==blacks_);

    return true;
}

#include <windows.h>
void Board::DebugPrintBoard(int x)
{
    ::OutputDebugString(isblacksturn?"Black's move\n":"White's move\n");
    for(int i=p11; i<s89; i++)
    {
        switch(table[i])
        {
        case black:
            if(x==i)
                ::OutputDebugString("B");
            else
                ::OutputDebugString("b");
            break;
        case white:
            if(x==i)
                ::OutputDebugString("W");
            else
                ::OutputDebugString("w");
            break;
        case empty:
            if(x==i)
                ::OutputDebugString("*");
            else
                ::OutputDebugString(".");
            break;
        }
        if(i%10==0)
            ::OutputDebugString("\n");
    }
    ::OutputDebugString("\n\n");
}

#endif //NDEBUG

signed char TTBoard::Persil[10*10]=
{
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1, 0, 1, 2, 3, 4, 5, 6, 7,-1,
    -1, 8, 9,10,11,12,13,14,15,-1,
    -1,16,17,18,19,20,21,22,23,-1,
    -1,24,25,26,27,28,29,30,31,-1,
    -1,32,33,34,35,36,37,38,39,-1,
    -1,40,41,42,43,44,45,46,47,-1,
    -1,48,49,50,51,52,53,54,55,-1,
    -1,56,57,58,59,60,61,62,63,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
};

squareindex TTBoard::AntiPersil[8*8]=
{
    p11, p12, p13, p14, p15, p16, p17, p18,
    p21, p22, p23, p24, p25, p26, p27, p28,
    p31, p32, p33, p34, p35, p36, p37, p38,
    p41, p42, p43, p44, p45, p46, p47, p48,
    p51, p52, p53, p54, p55, p56, p57, p58,
    p61, p62, p63, p64, p65, p66, p67, p68,
    p71, p72, p73, p74, p75, p76, p77, p78,
    p81, p82, p83, p84, p85, p86, p87, p88
};

squarevalue Board::sboard[10*10]=
{
    sentl,sentl,sentl,sentl,sentl,sentl,sentl,sentl,sentl,sentl,
    sentl,empty,empty,empty,empty,empty,empty,empty,empty,sentl,
    sentl,empty,empty,empty,empty,empty,empty,empty,empty,sentl,
    sentl,empty,empty,empty,empty,empty,empty,empty,empty,sentl,
    sentl,empty,empty,empty,white,black,empty,empty,empty,sentl,
    sentl,empty,empty,empty,black,white,empty,empty,empty,sentl,
    sentl,empty,empty,empty,empty,empty,empty,empty,empty,sentl,
    sentl,empty,empty,empty,empty,empty,empty,empty,empty,sentl,
    sentl,empty,empty,empty,empty,empty,empty,empty,empty,sentl,
    sentl,sentl,sentl,sentl,sentl,sentl,sentl,sentl,sentl,sentl
};

unsigned char Board::sym_diag1[10*10]=
{
    s00, s10, s20, s30, s40, s50, s60, s70, s80, s90,
    s01, p11, p21, p31, p41, p51, p61, p71, p81, s91,
    s02, p12, p22, p32, p42, p52, p62, p72, p82, s92,
    s03, p13, p23, p33, p43, p53, p63, p73, p83, s93,
    s04, p14, p24, p34, p44, p54, p64, p74, p84, s94,
    s05, p15, p25, p35, p45, p55, p65, p75, p85, s95,
    s06, p16, p26, p36, p46, p56, p66, p76, p86, s96,
    s07, p17, p27, p37, p47, p57, p67, p77, p87, s97,
    s08, p18, p28, p38, p48, p58, p68, p78, p88, s98,
    s09, s19, s29, s39, s49, s59, s69, s79, s89, s99    
};

unsigned char Board::sym_diag2[10*10]=
{
    s99, s89, s79, s69, s59, s49, s39, s29, s19, s09,
    s98, p88, p78, p68, p58, p48, p38, p28, p18, s08,
    s97, p87, p77, p67, p57, p47, p37, p27, p17, s07,
    s96, p86, p76, p66, p56, p46, p36, p26, p16, s06,
    s95, p85, p75, p65, p55, p45, p35, p25, p15, s05,
    s94, p84, p74, p64, p54, p44, p34, p24, p14, s04,
    s93, p83, p73, p63, p53, p43, p33, p23, p13, s03,
    s92, p82, p72, p62, p52, p42, p32, p22, p12, s02,
    s91, p81, p71, p61, p51, p41, p31, p21, p11, s01,
    s90, s80, s70, s60, s50, s40, s30, s20, s10, s00
};

unsigned char Board::sym_centr[10*10]=
{
    s99, s98, s97, s96, s95, s94, s93, s92, s91, s90,
    s89, p88, p87, p86, p85, p84, p83, p82, p81, s80,
    s79, p78, p77, p76, p75, p74, p73, p72, p71, s70,
    s69, p68, p67, p66, p65, p64, p63, p62, p61, s60,
    s59, p58, p57, p56, p55, p54, p53, p52, p51, s50,
    s49, p48, p47, p46, p45, p44, p43, p42, p41, s40,
    s39, p38, p37, p36, p35, p34, p33, p32, p31, s30,
    s29, p28, p27, p26, p25, p24, p23, p22, p21, s20,
    s19, p18, p17, p16, p15, p14, p13, p12, p11, s10,
    s09, s08, s07, s06, s05, s04, s03, s02, s01, s00
};


int BookBoard::maskblack[]=
{
    0x3fffffff,0xcfffffff,
    0xf3ffffff,0xfcffffff,
    0xff3fffff,0xffcfffff,
    0xfff3ffff,0xfffcffff,
    0xffff3fff,0xffffcfff,
    0xfffff3ff,0xfffffcff,
    0xffffff3f,0xffffffcf,
    0xfffffff3,0xfffffffc
};

int BookBoard::maskempty[]=
{
    0x80000000,0x20000000,
    0x08000000,0x02000000,
    0x00800000,0x00200000,
    0x00080000,0x00020000,
    0x00008000,0x00002000,
    0x00000800,0x00000200,
    0x00000080,0x00000020,
    0x00000008,0x00000002
};

int BookBoard::maskwhite[]=
{
    0x40000000,0x10000000,
    0x04000000,0x01000000,
    0x00400000,0x00100000,
    0x00040000,0x00010000,
    0x00004000,0x00001000,
    0x00000400,0x00000100,
    0x00000040,0x00000010,
    0x00000004,0x00000001
};

namespace
{

//index updates for 8 square patterns
const int i81=3*3*3*3*3*3*3;
const int i82=3*3*3*3*3*3;
const int i83=3*3*3*3*3;
const int i84=3*3*3*3;
const int i85=3*3*3;
const int i86=3*3;
const int i87=3;
const int i88=1;

//index updates for 7 square patterns
const int i71=3*3*3*3*3*3;
const int i72=3*3*3*3*3;
const int i73=3*3*3*3;
const int i74=3*3*3;
const int i75=3*3;
const int i76=3;
const int i77=1;

//index updates for 6 square patterns
const int i61=3*3*3*3*3;
const int i62=3*3*3*3;
const int i63=3*3*3;
const int i64=3*3;
const int i65=3;
const int i66=1;

//index updates for 5 square patterns
const int i51=3*3*3*3;
const int i52=3*3*3;
const int i53=3*3;
const int i54=3;
const int i55=1;

//index updates for 4 square patterns
const int i41=3*3*3;
const int i42=3*3;
const int i43=3;
const int i44=1;
}

void Board::InitPrecomp()
{
    dummy=0;

    for(int x=0; x<10*10; x++)
        for(int i=0; i<5; i++)
            updatetable[x][i]= PreComp(&dummy, 0);
    
    ////////////////////line 1///////////////////////////////
    //11 1A
    updatetable[p11][0]= PreComp( &line1,    i81);
    updatetable[p11][1]= PreComp( &colA,     i81);
    updatetable[p11][2]= PreComp( &diag1A8H, i81);
    updatetable[p11][3]= PreComp( &cornA1D2, i81);
    updatetable[p11][4]= PreComp( &cornA1B4, i81);

    //12 1B
    updatetable[p12][0]= PreComp( &line1,    i82);
    updatetable[p12][1]= PreComp( &colB,     i81);
    updatetable[p12][2]= PreComp( &diag7H1B, i77);
    updatetable[p12][3]= PreComp( &cornA1D2, i82);
    updatetable[p12][4]= PreComp( &cornA1B4, i85);

    //13 1C
    updatetable[p13][0]= PreComp( &line1,    i83);
    updatetable[p13][1]= PreComp( &colC,     i81);
    updatetable[p13][2]= PreComp( &diag6H1C, i66);
    updatetable[p13][3]= PreComp( &cornA1D2, i83);

    //14 1D
    updatetable[p14][0]= PreComp( &line1,    i84);
    updatetable[p14][1]= PreComp( &colD,     i81);
    updatetable[p14][2]= PreComp( &diag5H1D, i55);
    updatetable[p14][3]= PreComp( &diag4A1D, i44);
    updatetable[p14][4]= PreComp( &cornA1D2, i84);

    //15 1E
    updatetable[p15][0]= PreComp( &line1,    i85);
    updatetable[p15][1]= PreComp( &colE,     i81);
    updatetable[p15][2]= PreComp( &diag4H1E, i44);
    updatetable[p15][3]= PreComp( &diag5A1E, i55);
    updatetable[p15][4]= PreComp( &cornH1E2, i84);

    //16 1F
    updatetable[p16][0]= PreComp( &line1,    i86);
    updatetable[p16][1]= PreComp( &colF,     i81);
    updatetable[p16][2]= PreComp( &diag6A1F, i66);
    updatetable[p16][3]= PreComp( &cornH1E2, i83);
    
    //17 1G
    updatetable[p17][0]= PreComp( &line1,    i87);
    updatetable[p17][1]= PreComp( &colG,     i81);
    updatetable[p17][2]= PreComp( &diag7A1G, i77);
    updatetable[p17][3]= PreComp( &cornH1E2, i82);
    updatetable[p17][4]= PreComp( &cornH1G4, i85);

    //18 1H
    updatetable[p18][0]= PreComp( &line1,    i88);
    updatetable[p18][1]= PreComp( &colH,     i81);
    updatetable[p18][2]= PreComp( &diag8A1H, i88);
    updatetable[p18][3]= PreComp( &cornH1E2, i81);
    updatetable[p18][4]= PreComp( &cornH1G4, i81);

    ///////////////////line 2///////////////////////////////
    //21 2A
    updatetable[p21][0]= PreComp( &line2,    i81);
    updatetable[p21][1]= PreComp( &colA,     i82);
    updatetable[p21][2]= PreComp( &diag2A8G, i71);
    updatetable[p21][3]= PreComp( &cornA1D2, i85);
    updatetable[p21][4]= PreComp( &cornA1B4, i82);

    //22 2B
    updatetable[p22][0]= PreComp( &line2,    i82);
    updatetable[p22][1]= PreComp( &colB,     i82);
    updatetable[p22][2]= PreComp( &diag1A8H, i82);
    updatetable[p22][3]= PreComp( &cornA1D2, i86);
    updatetable[p22][4]= PreComp( &cornA1B4, i86);

    //23 2C
    updatetable[p23][0]= PreComp( &line2,    i83);
    updatetable[p23][1]= PreComp( &colC,     i82);
    updatetable[p23][2]= PreComp( &diag4A1D, i43);
    updatetable[p23][3]= PreComp( &diag7H1B, i76);
    updatetable[p23][4]= PreComp( &cornA1D2, i87);

    //24 2D
    updatetable[p24][0]= PreComp( &line2,    i84);
    updatetable[p24][1]= PreComp( &colD,     i82);
    updatetable[p24][2]= PreComp( &diag5A1E, i54);
    updatetable[p24][3]= PreComp( &diag6H1C, i65);
    updatetable[p24][4]= PreComp( &cornA1D2, i88);

    //25 2E
    updatetable[p25][0]= PreComp( &line2,    i85);
    updatetable[p25][1]= PreComp( &colE,     i82);
    updatetable[p25][2]= PreComp( &diag6A1F, i65);
    updatetable[p25][3]= PreComp( &diag5H1D, i54);
    updatetable[p25][4]= PreComp( &cornH1E2, i88);

    //26 2F
    updatetable[p26][0]= PreComp( &line2,    i86);
    updatetable[p26][1]= PreComp( &colF,     i82);
    updatetable[p26][2]= PreComp( &diag7A1G, i76);
    updatetable[p26][3]= PreComp( &diag4H1E, i43);
    updatetable[p26][4]= PreComp( &cornH1E2, i87);

    //27 2G
    updatetable[p27][0]= PreComp( &line2,    i87);
    updatetable[p27][1]= PreComp( &colG,     i82);
    updatetable[p27][2]= PreComp( &diag8A1H, i87);
    updatetable[p27][3]= PreComp( &cornH1E2, i86);
    updatetable[p27][4]= PreComp( &cornH1G4, i86);

    //28 2H
    updatetable[p28][0]= PreComp( &line2,    i88);
    updatetable[p28][1]= PreComp( &colH,     i82);
    updatetable[p28][2]= PreComp( &diag2H8B, i71);
    updatetable[p28][3]= PreComp( &cornH1E2, i85);
    updatetable[p28][4]= PreComp( &cornH1G4, i82);
    
    ///////////////////line 3///////////////////////////////
    //31 3A nck
    updatetable[p31][0]= PreComp( &line3,    i81);
    updatetable[p31][1]= PreComp( &colA,     i83);
    updatetable[p31][2]= PreComp( &diag3A8F, i61);
    updatetable[p31][3]= PreComp( &cornA1B4, i83);

    //32 3B
    updatetable[p32][0]= PreComp( &line3,    i82);
    updatetable[p32][1]= PreComp( &colB,     i83);
    updatetable[p32][2]= PreComp( &diag2A8G, i72);
    updatetable[p32][3]= PreComp( &diag4A1D, i42);
    updatetable[p32][4]= PreComp( &cornA1B4, i87);

    //33 3C
    updatetable[p33][0]= PreComp( &line3,    i83);
    updatetable[p33][1]= PreComp( &colC,     i83);
    updatetable[p33][2]= PreComp( &diag5A1E, i53);
    updatetable[p33][3]= PreComp( &diag1A8H, i83);

    //34 3D
    updatetable[p34][0]= PreComp( &line3,    i84);
    updatetable[p34][1]= PreComp( &colD,     i83);
    updatetable[p34][2]= PreComp( &diag6A1F, i64);
    updatetable[p34][3]= PreComp( &diag7H1B, i75);
        
    //35 3E
    updatetable[p35][0]= PreComp( &line3,    i85);
    updatetable[p35][1]= PreComp( &colE,     i83);
    updatetable[p35][2]= PreComp( &diag7A1G, i75);
    updatetable[p35][3]= PreComp( &diag6H1C, i64);

    //36 3F
    updatetable[p36][0]= PreComp( &line3,    i86);
    updatetable[p36][1]= PreComp( &colF,     i83);
    updatetable[p36][2]= PreComp( &diag8A1H, i86);
    updatetable[p36][3]= PreComp( &diag5H1D, i53);

    //37 3G
    updatetable[p37][0]= PreComp( &line3,    i87);
    updatetable[p37][1]= PreComp( &colG,     i83);
    updatetable[p37][2]= PreComp( &diag2H8B, i72);
    updatetable[p37][3]= PreComp( &diag4H1E, i42);
    updatetable[p37][4]= PreComp( &cornH1G4, i87);

    //38 3H
    updatetable[p38][0]= PreComp( &line3,    i88);
    updatetable[p38][1]= PreComp( &colH,     i83);
    updatetable[p38][2]= PreComp( &diag3H8C, i61);
    updatetable[p38][3]= PreComp( &cornH1G4, i83);

    ///////////////////line 4///////////////////////////////
    //41 4A nck
    updatetable[p41][0]= PreComp( &line4,    i81);
    updatetable[p41][1]= PreComp( &colA,     i84);
    updatetable[p41][2]= PreComp( &diag4A1D, i41);
    updatetable[p41][3]= PreComp( &diag4A8E, i51);
    updatetable[p41][4]= PreComp( &cornA1B4, i84);

    //42 4B
    updatetable[p42][0]= PreComp( &line4,    i82);
    updatetable[p42][1]= PreComp( &colB,     i84);
    updatetable[p42][2]= PreComp( &diag5A1E, i52);
    updatetable[p42][3]= PreComp( &diag3A8F, i62);
    updatetable[p42][4]= PreComp( &cornA1B4, i88);

    //43 4C
    updatetable[p43][0]= PreComp( &line4,    i83);
    updatetable[p43][1]= PreComp( &colC,     i84);
    updatetable[p43][2]= PreComp( &diag6A1F, i63);
    updatetable[p43][3]= PreComp( &diag2A8G, i73);

    //44 4D
    updatetable[p44][0]= PreComp( &line4,    i84);
    updatetable[p44][1]= PreComp( &colD,     i84);
    updatetable[p44][2]= PreComp( &diag7A1G, i74);
    updatetable[p44][3]= PreComp( &diag1A8H, i84);

    //45 4E
    updatetable[p45][0]= PreComp( &line4,    i85);
    updatetable[p45][1]= PreComp( &colE,     i84);
    updatetable[p45][2]= PreComp( &diag8A1H, i85);
    updatetable[p45][3]= PreComp( &diag7H1B, i74);

    //46 4F
    updatetable[p46][0]= PreComp( &line4,    i86);
    updatetable[p46][1]= PreComp( &colF,     i84);
    updatetable[p46][2]= PreComp( &diag2H8B, i73);
    updatetable[p46][3]= PreComp( &diag6H1C, i63);

    //47 4G
    updatetable[p47][0]= PreComp( &line4,    i87);
    updatetable[p47][1]= PreComp( &colG,     i84);
    updatetable[p47][2]= PreComp( &diag3H8C, i62);
    updatetable[p47][3]= PreComp( &diag5H1D, i52);
    updatetable[p47][4]= PreComp( &cornH1G4, i88);
    
    //48 4H
    updatetable[p48][0]= PreComp( &line4,    i88);
    updatetable[p48][1]= PreComp( &colH,     i84);
    updatetable[p48][2]= PreComp( &diag4H8D, i51);
    updatetable[p48][3]= PreComp( &diag4H1E, i41);
    updatetable[p48][4]= PreComp( &cornH1G4, i84);

    ///////////////////line 5///////////////////////////////
    //51 5A
    updatetable[p51][0]= PreComp( &line5,    i81);
    updatetable[p51][1]= PreComp( &colA,     i85);
    updatetable[p51][2]= PreComp( &diag5A1E, i51);
    updatetable[p51][3]= PreComp( &diag5A8D, i41);
    updatetable[p51][4]= PreComp( &cornA8B5, i84);

    //52 5B
    updatetable[p52][0]= PreComp( &line5,    i82);
    updatetable[p52][1]= PreComp( &colB,     i85);
    updatetable[p52][2]= PreComp( &diag6A1F, i62);
    updatetable[p52][3]= PreComp( &diag4A8E, i52);
    updatetable[p52][4]= PreComp( &cornA8B5, i88);

    //53 5C
    updatetable[p53][0]= PreComp( &line5,    i83);
    updatetable[p53][1]= PreComp( &colC,     i85);
    updatetable[p53][2]= PreComp( &diag7A1G, i73);
    updatetable[p53][3]= PreComp( &diag3A8F, i63);

    //54 5D
    updatetable[p54][0]= PreComp( &line5,    i84);
    updatetable[p54][1]= PreComp( &colD,     i85);
    updatetable[p54][2]= PreComp( &diag8A1H, i84);
    updatetable[p54][3]= PreComp( &diag2A8G, i74);

    //55 5E
    updatetable[p55][0]= PreComp( &line5,    i85);
    updatetable[p55][1]= PreComp( &colE,     i85);
    updatetable[p55][2]= PreComp( &diag2H8B, i74);
    updatetable[p55][3]= PreComp( &diag1A8H, i85);

    //56 5F
    updatetable[p56][0]= PreComp( &line5,    i86);
    updatetable[p56][1]= PreComp( &colF,     i85);
    updatetable[p56][2]= PreComp( &diag3H8C, i63);
    updatetable[p56][3]= PreComp( &diag7H1B, i73);

    //57 5G
    updatetable[p57][0]= PreComp( &line5,    i87);
    updatetable[p57][1]= PreComp( &colG,     i85);
    updatetable[p57][2]= PreComp( &diag4H8D, i52);
    updatetable[p57][3]= PreComp( &diag6H1C, i62);
    updatetable[p57][4]= PreComp( &cornH8G5, i88);

    //58 5H
    updatetable[p58][0]= PreComp( &line5,    i88);
    updatetable[p58][1]= PreComp( &colH,     i85);
    updatetable[p58][2]= PreComp( &diag5H8E, i41);
    updatetable[p58][3]= PreComp( &diag5H1D, i51);
    updatetable[p58][4]= PreComp( &cornH8G5, i84);

    ///////////////////line 6///////////////////////////////
    //61 6A
    updatetable[p61][0]= PreComp( &line6,    i81);
    updatetable[p61][1]= PreComp( &colA,     i86);
    updatetable[p61][2]= PreComp( &diag6A1F, i61);
    updatetable[p61][3]= PreComp( &cornA8B5, i83);

    //62 6B
    updatetable[p62][0]= PreComp( &line6,    i82);
    updatetable[p62][1]= PreComp( &colB,     i86);
    updatetable[p62][2]= PreComp( &diag7A1G, i72);
    updatetable[p62][3]= PreComp( &diag5A8D, i42);
    updatetable[p62][4]= PreComp( &cornA8B5, i87);

    //63 6C
    updatetable[p63][0]= PreComp( &line6,    i83);
    updatetable[p63][1]= PreComp( &colC,     i86);
    updatetable[p63][2]= PreComp( &diag8A1H, i83);
    updatetable[p63][3]= PreComp( &diag4A8E, i53);

    //64 6D
    updatetable[p64][0]= PreComp( &line6,    i84);
    updatetable[p64][1]= PreComp( &colD,     i86);
    updatetable[p64][2]= PreComp( &diag2H8B, i75);
    updatetable[p64][3]= PreComp( &diag3A8F, i64);

    //65 6E
    updatetable[p65][0]= PreComp( &line6,    i85);
    updatetable[p65][1]= PreComp( &colE,     i86);
    updatetable[p65][2]= PreComp( &diag3H8C, i64);
    updatetable[p65][3]= PreComp( &diag2A8G, i75);

    //66 6F
    updatetable[p66][0]= PreComp( &line6,    i86);
    updatetable[p66][1]= PreComp( &colF,     i86);
    updatetable[p66][2]= PreComp( &diag4H8D, i53);
    updatetable[p66][3]= PreComp( &diag1A8H, i86);

    //67 6G
    updatetable[p67][0]= PreComp( &line6,    i87);
    updatetable[p67][1]= PreComp( &colG,     i86);
    updatetable[p67][2]= PreComp( &diag5H8E, i42);
    updatetable[p67][3]= PreComp( &diag7H1B, i72);    
    updatetable[p67][4]= PreComp( &cornH8G5, i87);
    
    //68 6H
    updatetable[p68][0]= PreComp( &line6,    i88);
    updatetable[p68][1]= PreComp( &colH,     i86);
    updatetable[p68][2]= PreComp( &diag6H1C, i61);
    updatetable[p68][3]= PreComp( &cornH8G5, i83);

    ///////////////////line 7///////////////////////////////
    //71 7A
    updatetable[p71][0]= PreComp( &line7,    i81);
    updatetable[p71][1]= PreComp( &colA,     i87);
    updatetable[p71][2]= PreComp( &diag7A1G, i71);
    updatetable[p71][3]= PreComp( &cornA8B5, i82);
    updatetable[p71][4]= PreComp( &cornA8D7, i85);

    //72 7B
    updatetable[p72][0]= PreComp( &line7,    i82);
    updatetable[p72][1]= PreComp( &colB,     i87);
    updatetable[p72][2]= PreComp( &diag8A1H, i82);
    updatetable[p72][3]= PreComp( &cornA8B5, i86);
    updatetable[p72][4]= PreComp( &cornA8D7, i86);

    //73 7C
    updatetable[p73][0]= PreComp( &line7,    i83);
    updatetable[p73][1]= PreComp( &colC,     i87);
    updatetable[p73][2]= PreComp( &diag5A8D, i43);
    updatetable[p73][3]= PreComp( &diag2H8B, i76);
    updatetable[p73][4]= PreComp( &cornA8D7, i87);

    //74 7D
    updatetable[p74][0]= PreComp( &line7,    i84);
    updatetable[p74][1]= PreComp( &colD,     i87);
    updatetable[p74][2]= PreComp( &diag4A8E, i54);
    updatetable[p74][3]= PreComp( &diag3H8C, i65);
    updatetable[p74][4]= PreComp( &cornA8D7, i88);

    //75 7E
    updatetable[p75][0]= PreComp( &line7,    i85);
    updatetable[p75][1]= PreComp( &colE,     i87);
    updatetable[p75][2]= PreComp( &diag3A8F, i65);
    updatetable[p75][3]= PreComp( &diag4H8D, i54);
    updatetable[p75][4]= PreComp( &cornH8E7, i88);

    //76 7F
    updatetable[p76][0]= PreComp( &line7,    i86);
    updatetable[p76][1]= PreComp( &colF,     i87);
    updatetable[p76][2]= PreComp( &diag2A8G, i76);
    updatetable[p76][3]= PreComp( &diag5H8E, i43);
    updatetable[p76][4]= PreComp( &cornH8E7, i87);

    //77 7G
    updatetable[p77][0]= PreComp( &line7,    i87);
    updatetable[p77][1]= PreComp( &colG,     i87);
    updatetable[p77][2]= PreComp( &diag1A8H, i87);
    updatetable[p77][3]= PreComp( &cornH8G5, i86);
    updatetable[p77][4]= PreComp( &cornH8E7, i86);

    //78 7H
    updatetable[p78][0]= PreComp( &line7,    i88);
    updatetable[p78][1]= PreComp( &colH,     i87);
    updatetable[p78][2]= PreComp( &diag7H1B, i71);
    updatetable[p78][3]= PreComp( &cornH8G5, i82);
    updatetable[p78][4]= PreComp( &cornH8E7, i85);

    ///////////////////line 8///////////////////////////////
    //81 8A
    updatetable[p81][0]= PreComp( &line8,    i81);
    updatetable[p81][1]= PreComp( &colA,     i88);
    updatetable[p81][2]= PreComp( &diag8A1H, i81);
    updatetable[p81][3]= PreComp( &cornA8B5, i81);
    updatetable[p81][4]= PreComp( &cornA8D7, i81);

    //82 8B
    updatetable[p82][0]= PreComp( &line8,    i82);
    updatetable[p82][1]= PreComp( &colB,     i88);
    updatetable[p82][2]= PreComp( &diag2H8B, i77);
    updatetable[p82][3]= PreComp( &cornA8B5, i85);
    updatetable[p82][4]= PreComp( &cornA8D7, i82);

    //83 8C
    updatetable[p83][0]= PreComp( &line8,    i83);
    updatetable[p83][1]= PreComp( &colC,     i88);
    updatetable[p83][2]= PreComp( &diag3H8C, i66);
    updatetable[p83][3]= PreComp( &cornA8D7, i83);

    //84 8D
    updatetable[p84][0]= PreComp( &line8,    i84);
    updatetable[p84][1]= PreComp( &colD,     i88);
    updatetable[p84][2]= PreComp( &diag4H8D, i55);
    updatetable[p84][3]= PreComp( &diag5A8D, i44);
    updatetable[p84][4]= PreComp( &cornA8D7, i84);

    //85 8E
    updatetable[p85][0]= PreComp( &line8,    i85);
    updatetable[p85][1]= PreComp( &colE,     i88);
    updatetable[p85][2]= PreComp( &diag5H8E, i44);
    updatetable[p85][3]= PreComp( &diag4A8E, i55);
    updatetable[p85][4]= PreComp( &cornH8E7, i84);

    //86 8F
    updatetable[p86][0]= PreComp( &line8,    i86);
    updatetable[p86][1]= PreComp( &colF,     i88);
    updatetable[p86][2]= PreComp( &diag3A8F, i66);
    updatetable[p86][3]= PreComp( &cornH8E7, i83);

    //87 8G
    updatetable[p87][0]= PreComp( &line8,    i87);
    updatetable[p87][1]= PreComp( &colG,     i88);
    updatetable[p87][2]= PreComp( &diag2A8G, i77);
    updatetable[p87][3]= PreComp( &cornH8G5, i85);
    updatetable[p87][4]= PreComp( &cornH8E7, i82);

    //88 8H
    updatetable[p88][0]= PreComp( &line8,    i88);
    updatetable[p88][1]= PreComp( &colH,     i88);
    updatetable[p88][2]= PreComp( &diag1A8H, i88);
    updatetable[p88][3]= PreComp( &cornH8G5, i81);
    updatetable[p88][4]= PreComp( &cornH8E7, i81);
}

