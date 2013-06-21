/**
 @file
 Static evaluator related code
*/

#include "../lib/WinException.h"
#include "Evaluator.h"
#include "Boards.h"
#include "BillMobilityDat.h"

using namespace Othello;

//NOTE: this is just a humble and very stupid begining
Evaluator::Evaluator()
{
    InitPatterns();
}

int Evaluator::Evaluate()
{ 
    if(actualb.CountEmpty())
    {
        int ret=9999999;
        switch(actualb.isblacksturn)
        {
        case true:
            //if(IsBlacksTurn())
                ret=EvaluateForBlack();
            /*else
                if(!IsWhitesTurn())
                    ret=CountDiscs();
                else
                    ret=EvaluateForWhite();*/
            break;
        case false:
            ///if(IsWhitesTurn())
                ret=EvaluateForWhite();
            /*else
                if(!IsBlacksTurn())
                    ret=CountDiscs();
                else
                    ret=EvaluateForBlack();*/
        }
        assert(ret!=9999999);
        return ret;
    }
    return CountDiscs();
}


inline int Evaluator::CountDiscs()
{
    int diff=actualb.CountBlack()-actualb.CountWhite(); 
    if(diff>0)
        diff=diff+63;
    if(diff<0)
        diff=diff-63; 
    return diff;
}
    
inline int Evaluator::GetStage()
{
    assert(60>=actualb.CountEmpty());
    assert(0<actualb.CountEmpty());
    int tmp=stagetable[60-actualb.CountEmpty()];
    assert(tmp>=0 && tmp<Stages);
    return tmp;
}


//EXTRA WEAK
inline int Evaluator::Adjust(int t)
{
    bool ispos=true;
    if(t<0)
    {
        ispos=false;
        t=-t;
    }
    int round=t/(1<<26);
    int fract=t-round;
    int ret;
    if(fract==0)
        ret=ispos?round:-round;
    else
    {
        if(fract>=(1<<25))
            round++;
        else
            round--;
        ret=ispos?round:-round;
    }

    if(ret>63)
        ret=63;
    if(ret<-63)
        ret=-63;
    return ret;
}

int Evaluator::EvaluateForBlack()
{
    int s=GetStage();
    int ret=
        _pdiag4[s][actualb.diag4A1D]+
        _pdiag4[s][actualb.diag5A8D]+
        _pdiag4[s][actualb.diag5H8E]+
        _pdiag4[s][actualb.diag4H1E]+

        _pdiag5[s][actualb.diag5A1E]+
        _pdiag5[s][actualb.diag4A8E]+
        _pdiag5[s][actualb.diag4H8D]+
        _pdiag5[s][actualb.diag5H1D]+
    
        _pdiag6[s][actualb.diag6A1F]+
        _pdiag6[s][actualb.diag3A8F]+
        _pdiag6[s][actualb.diag3H8C]+
        _pdiag6[s][actualb.diag6H1C]+

        _pdiag7[s][actualb.diag7A1G]+
        _pdiag7[s][actualb.diag2A8G]+
        _pdiag7[s][actualb.diag2H8B]+
        _pdiag7[s][actualb.diag7H1B]+

        _pdiag8[s][actualb.diag8A1H]+
        _pdiag8[s][actualb.diag1A8H]+

        _plc1[s][actualb.line1]+
        _plc2[s][actualb.line2]+
        _plc3[s][actualb.line3]+
        _plc4[s][actualb.line4]+
        _plc4[s][actualb.line5]+
        _plc3[s][actualb.line6]+
        _plc2[s][actualb.line7]+
        _plc1[s][actualb.line8]+
        _plc1[s][actualb.colA]+
        _plc2[s][actualb.colB]+
        _plc3[s][actualb.colC]+
        _plc4[s][actualb.colD]+
        _plc4[s][actualb.colE]+
        _plc3[s][actualb.colF]+
        _plc2[s][actualb.colG]+
        _plc1[s][actualb.colH]+

        _pcorn[s][actualb.cornA1D2]+
        _pcorn[s][actualb.cornA1B4]+
        _pcorn[s][actualb.cornA8B5]+
        _pcorn[s][actualb.cornA8D7]+

        _pcorn[s][actualb.cornH8E7]+
        _pcorn[s][actualb.cornH8G5]+
        _pcorn[s][actualb.cornH1G4]+
        _pcorn[s][actualb.cornH1E2]+
        _parity[s][actualb.empties%2];

    return Adjust(ret);
}

int Evaluator::EvaluateForWhite()
{
    int s=GetStage();
    int ret=
        _pdiag4[s][-actualb.diag4A1D]+
        _pdiag4[s][-actualb.diag5A8D]+
        _pdiag4[s][-actualb.diag5H8E]+
        _pdiag4[s][-actualb.diag4H1E]+

        _pdiag5[s][-actualb.diag5A1E]+
        _pdiag5[s][-actualb.diag4A8E]+
        _pdiag5[s][-actualb.diag4H8D]+
        _pdiag5[s][-actualb.diag5H1D]+
    
        _pdiag6[s][-actualb.diag6A1F]+
        _pdiag6[s][-actualb.diag3A8F]+
        _pdiag6[s][-actualb.diag3H8C]+
        _pdiag6[s][-actualb.diag6H1C]+

        _pdiag7[s][-actualb.diag7A1G]+
        _pdiag7[s][-actualb.diag2A8G]+
        _pdiag7[s][-actualb.diag2H8B]+
        _pdiag7[s][-actualb.diag7H1B]+

        _pdiag8[s][-actualb.diag8A1H]+
        _pdiag8[s][-actualb.diag1A8H]+

        _plc1[s][-actualb.line1]+
        _plc2[s][-actualb.line2]+
        _plc3[s][-actualb.line3]+
        _plc4[s][-actualb.line4]+
        _plc4[s][-actualb.line5]+
        _plc3[s][-actualb.line6]+
        _plc2[s][-actualb.line7]+
        _plc1[s][-actualb.line8]+
        _plc1[s][-actualb.colA]+
        _plc2[s][-actualb.colB]+
        _plc3[s][-actualb.colC]+
        _plc4[s][-actualb.colD]+
        _plc4[s][-actualb.colE]+
        _plc3[s][-actualb.colF]+
        _plc2[s][-actualb.colG]+
        _plc1[s][-actualb.colH]+

        _pcorn[s][-actualb.cornA1D2]+
        _pcorn[s][-actualb.cornA1B4]+
        _pcorn[s][-actualb.cornA8B5]+
        _pcorn[s][-actualb.cornA8D7]+

        _pcorn[s][-actualb.cornH8E7]+
        _pcorn[s][-actualb.cornH8G5]+
        _pcorn[s][-actualb.cornH1G4]+
        _pcorn[s][-actualb.cornH1E2]+
        _parity[s][actualb.empties%2];
    return Adjust(-ret);
}


bool Evaluator::IsBlacksTurn()
{
    assert(actualb.IsClean());
    return
        _bmobil.pattern8[actualb.colD]     || 
        _bmobil.pattern8[actualb.colE]     ||
        _bmobil.pattern8[actualb.line4]    || 
        _bmobil.pattern8[actualb.line5]    ||
        _bmobil.pattern8[actualb.colC]     || 
        _bmobil.pattern8[actualb.colF]     ||
        _bmobil.pattern8[actualb.line3]    || 
        _bmobil.pattern8[actualb.line6]    ||
        _bmobil.pattern8[actualb.diag8A1H] || 
        _bmobil.pattern8[actualb.diag1A8H] ||
        _bmobil.pattern7[actualb.diag2A8G] || 
        _bmobil.pattern7[actualb.diag2H8B] ||
        _bmobil.pattern7[actualb.diag7A1G] || 
        _bmobil.pattern7[actualb.diag7H1B] ||
        _bmobil.pattern6[actualb.diag3A8F] || 
        _bmobil.pattern6[actualb.diag3H8C] ||
        _bmobil.pattern6[actualb.diag6A1F] || 
        _bmobil.pattern6[actualb.diag6H1C] ||    
        _bmobil.pattern5[actualb.diag4A8E] || 
        _bmobil.pattern5[actualb.diag4H8D] ||
        _bmobil.pattern5[actualb.diag5H1D] || 
        _bmobil.pattern5[actualb.diag5A1E] ||
        _bmobil.pattern8[actualb.colB]     || 
        _bmobil.pattern8[actualb.colG]     ||
        _bmobil.pattern8[actualb.line2]    || 
        _bmobil.pattern8[actualb.line7]    ||
        _bmobil.pattern8[actualb.colA]     || 
        _bmobil.pattern8[actualb.colH]     ||
        _bmobil.pattern8[actualb.line1]    || 
        _bmobil.pattern8[actualb.line8]    ||
        _bmobil.pattern4[actualb.diag4A1D] || 
        _bmobil.pattern4[actualb.diag4H1E] ||
        _bmobil.pattern4[actualb.diag5A8D] || 
        _bmobil.pattern4[actualb.diag5H8E] ||
        _bmobil.pattern3[actualb.Diag3_1()]||
        _bmobil.pattern3[actualb.Diag3_2()]||
        _bmobil.pattern3[actualb.Diag3_3()]||
        _bmobil.pattern3[actualb.Diag3_4()];
}

bool Evaluator::IsWhitesTurn()
{
    assert(actualb.IsClean());
    return
        _bmobil.pattern8[-actualb.colD]     || 
        _bmobil.pattern8[-actualb.colE]     ||
        _bmobil.pattern8[-actualb.line4]    || 
        _bmobil.pattern8[-actualb.line5]    ||
        _bmobil.pattern8[-actualb.colC]     || 
        _bmobil.pattern8[-actualb.colF]     ||
        _bmobil.pattern8[-actualb.line3]    || 
        _bmobil.pattern8[-actualb.line6]    ||
        _bmobil.pattern8[-actualb.diag8A1H] || 
        _bmobil.pattern8[-actualb.diag1A8H] ||
        _bmobil.pattern7[-actualb.diag2A8G] || 
        _bmobil.pattern7[-actualb.diag2H8B] ||
        _bmobil.pattern7[-actualb.diag7A1G] || 
        _bmobil.pattern7[-actualb.diag7H1B] ||
        _bmobil.pattern6[-actualb.diag3A8F] || 
        _bmobil.pattern6[-actualb.diag3H8C] ||
        _bmobil.pattern6[-actualb.diag6A1F] || 
        _bmobil.pattern6[-actualb.diag6H1C] ||    
        _bmobil.pattern5[-actualb.diag4A8E] || 
        _bmobil.pattern5[-actualb.diag4H8D] ||
        _bmobil.pattern5[-actualb.diag5H1D] || 
        _bmobil.pattern5[-actualb.diag5A1E] ||
        _bmobil.pattern8[-actualb.colB]     || 
        _bmobil.pattern8[-actualb.colG]     ||
        _bmobil.pattern8[-actualb.line2]    || 
        _bmobil.pattern8[-actualb.line7]    ||
        _bmobil.pattern8[-actualb.colA]     || 
        _bmobil.pattern8[-actualb.colH]     ||
        _bmobil.pattern8[-actualb.line1]    || 
        _bmobil.pattern8[-actualb.line8]    ||
        _bmobil.pattern4[-actualb.diag4A1D] || 
        _bmobil.pattern4[-actualb.diag4H1E] ||
        _bmobil.pattern4[-actualb.diag5A8D] || 
        _bmobil.pattern4[-actualb.diag5H8E] ||
        _bmobil.pattern3[-actualb.Diag3_1()]||
        _bmobil.pattern3[-actualb.Diag3_2()]||
        _bmobil.pattern3[-actualb.Diag3_3()]||
        _bmobil.pattern3[-actualb.Diag3_4()];
}


int Evaluator::ReadAndCheck(int &param, FILE *f)
{
    int ret=fread(&param, sizeof(int), 1, f);
    _crc.Put32Bit(static_cast<unsigned int>(param));
    return ret;
}

void Evaluator::InitPatterns()
{
    int i=0;
    //init pointers
    for(i=0; i<Stages; i++)
    {
        _pdiag4[i]=&_diag4[i][1+3+9+27];
        _pdiag5[i]=&_diag5[i][1+3+9+27+81];
        _pdiag6[i]=&_diag6[i][1+3+9+27+81+243];
        _pdiag7[i]=&_diag7[i][1+3+9+27+81+243+729];
        _pdiag8[i]=&_diag8[i][1+3+9+27+81+243+729+2187];
        _plc1[i]=&_lc1[i][1+3+9+27+81+243+729+2187];
        _plc2[i]=&_lc2[i][1+3+9+27+81+243+729+2187];
        _plc3[i]=&_lc3[i][1+3+9+27+81+243+729+2187];
        _plc4[i]=&_lc4[i][1+3+9+27+81+243+729+2187];
        _pcorn[i]=&_corn[i][1+3+9+27+81+243+729+2187];
    }

    //load patterns from file
    FILE *f=NULL;
    if(NULL==(f=fopen("params.bin","rb")))
        throw Win::Exception("Failed to load file \"params.bin\" !");
    
    int ret=0;
    for(i=0; i<Stages; i++)
    {
        int p=0;
        for(p=0; p<3*3*3*3; p++)
            ret+=ReadAndCheck(_diag4[i][p], f);

        for(p=0; p<3*3*3*3*3; p++)
            ret+=ReadAndCheck(_diag5[i][p], f);

        for(p=0; p<3*3*3*3*3*3; p++)
            ret+=ReadAndCheck(_diag6[i][p], f);

        for(p=0; p<3*3*3*3*3*3*3; p++)
            ret+=ReadAndCheck(_diag7[i][p], f);

        for(p=0; p<3*3*3*3*3*3*3*3; p++)
            ret+=ReadAndCheck(_diag8[i][p], f);

        for(p=0; p<3*3*3*3*3*3*3*3; p++)
            ret+=ReadAndCheck(_lc1[i][p], f);

        for(p=0; p<3*3*3*3*3*3*3*3; p++)
            ret+=ReadAndCheck(_lc2[i][p], f);

        for(p=0; p<3*3*3*3*3*3*3*3; p++)
            ret+=ReadAndCheck(_lc3[i][p], f);

        for(p=0; p<3*3*3*3*3*3*3*3; p++)
            ret+=ReadAndCheck(_lc4[i][p], f);

        for(p=0; p<3*3*3*3*3*3*3*3; p++)
            ret+=ReadAndCheck(_corn[i][p], f);

        ret+=ReadAndCheck(_parity[i][0], f);
        ret+=ReadAndCheck(_parity[i][1], f);
    }
    fclose(f);

    //BAD THING: magic numbers
    if(553904!=ret || 1522479752!=_crc.Done())
        throw Win::Exception("File \"params.bin\" is corrupted!");
}


EthernetCrc Evaluator::_crc;

int Evaluator::_diag4[Stages][3*3*3*3];
int Evaluator::_diag5[Stages][3*3*3*3*3];
int Evaluator::_diag6[Stages][3*3*3*3*3*3];
int Evaluator::_diag7[Stages][3*3*3*3*3*3*3];
int Evaluator::_diag8[Stages][3*3*3*3*3*3*3*3];
int Evaluator::_lc1[Stages][3*3*3*3*3*3*3*3];
int Evaluator::_lc2[Stages][3*3*3*3*3*3*3*3];
int Evaluator::_lc3[Stages][3*3*3*3*3*3*3*3];
int Evaluator::_lc4[Stages][3*3*3*3*3*3*3*3];
int Evaluator::_corn[Stages][3*3*3*3*3*3*3*3];

int Evaluator::_parity[Stages][2];

int * Evaluator::_pdiag4[Stages];
int * Evaluator::_pdiag5[Stages];
int * Evaluator::_pdiag6[Stages];
int * Evaluator::_pdiag7[Stages];
int * Evaluator::_pdiag8[Stages];
int * Evaluator::_plc1[Stages];
int * Evaluator::_plc2[Stages];
int * Evaluator::_plc3[Stages];
int * Evaluator::_plc4[Stages];
int * Evaluator::_pcorn[Stages];

int Evaluator::stagetable[60]=
{   
    0, 
    0, 0, 0, 0,   0, 0, 0, 0,  0, 0, 0, 0,
    1, 1, 1, 1,   2, 2, 2, 2,  3, 3, 3, 3,
    4, 4, 4, 4,   5, 5, 5, 5,  6, 6, 6, 6,
    7, 7, 7, 7,   8, 8, 8, 8,  9, 9, 9, 9,
    10,10,10,10,  11,11,11,11, 12,12,12
};

BillMobilityDat Evaluator::_bmobil;

