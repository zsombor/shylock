/**
 @file
 The code precalculating the BILL mobility.
*/

#include <cassert>
#include "BillMobilityDat.h"

using namespace Othello;

BillMobilityDat::BillMobilityDat()
{
    //I'll be using negative indeces, so setup the base pointer
    pattern3 = & _pattern3[1+3+9];
    pattern4 = & _pattern4[1+3+9+27];
    pattern5 = & _pattern5[1+3+9+27+81];
    pattern6 = & _pattern6[1+3+9+27+81+243];
    pattern7 = & _pattern7[1+3+9+27+81+243+729];
    pattern8 = & _pattern8[1+3+9+27+81+243+729+2187];

    //now build the pattern database
    squarevalue pattern[10];
    for(unsigned int i=0; i<sizeof(pattern)/sizeof(pattern[0]); i++)
        pattern[i]=sentl;
    for(unsigned int length=3; length<=8; length++)
    {
        Build(1, black, length, pattern);
        Build(1, white, length, pattern);
        Build(1, empty, length, pattern);
    }
}


void BillMobilityDat::Build(int k, squarevalue p, int length, squarevalue *a)
{
    a[k]=p;
    if(k==length)
    {   
        int ndx=0;
        for(int i=1; i<=length; i++)
            ndx=3*ndx+a[i];
        bool c=Compute(length, a);
        switch(length)
        {
            case 8:
                pattern8[ndx]=c;
                break;
            case 7:
                pattern7[ndx]=c;
                break;
            case 6:
                pattern6[ndx]=c;
                break;
            case 5:
                pattern5[ndx]=c;
                break;
            case 4:
                pattern4[ndx]=c;
                break;
            case 3:
                pattern3[ndx]=c;
                break;
            default:
                assert(false);
                break;
        }
    }
    else
    {
        Build(k+1, black, length, a);
        Build(k+1, white, length, a);
        Build(k+1, empty, length, a);
    }
    a[k]=sentl;
}

bool BillMobilityDat::Compute(int length, squarevalue *a)
{
    for(int i=1; i<=length; i++)
        if(a[i]==empty)
            for(int dx=-1; dx<=1; dx++)
                if(a[i+dx]==white)
                {
                    int x=i+2*dx;
                    while(a[x]==white)
                        x+=dx;
                    if(a[x]==black)
                        return true;
                }   
    return false;
}


bool * BillMobilityDat::pattern8;
bool * BillMobilityDat::pattern7;
bool * BillMobilityDat::pattern6;
bool * BillMobilityDat::pattern5;
bool * BillMobilityDat::pattern4;
bool * BillMobilityDat::pattern3;

bool BillMobilityDat::_pattern8[3*3*3*3*3*3*3*3];
bool BillMobilityDat::_pattern7[3*3*3*3*3*3*3];
bool BillMobilityDat::_pattern6[3*3*3*3*3*3];
bool BillMobilityDat::_pattern5[3*3*3*3*3];
bool BillMobilityDat::_pattern4[3*3*3*3];
bool BillMobilityDat::_pattern3[3*3*3];
