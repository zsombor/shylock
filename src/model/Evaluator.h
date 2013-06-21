/**
 @file
 Interface of the static evaluator
*/
#if !defined _EVALUATOR_H_
#define _EVALUATOR_H_
#include <cstdio>
#include "BillMobilityDat.h"
#include "../lib/Crc.h"

namespace Othello
{
    enum
    {
        Stages=13
    };

    /**
      class Evaluator, used to staticaly evaluate a board. */
    class Evaluator
    {
    public:

        /**
         @exception throws Win::Exception upon failure */
        explicit Evaluator();

        /**
         @return the value of actualb */
        int Evaluate();

        /**
         @return true only if next move belongs to black */
        bool IsBlacksTurn();

        /**
         @return true only if next move belongs to white */
        bool IsWhitesTurn();

        /**
         @return evaluation of actualb from black's view */
        int EvaluateForBlack();

        /**
         @return evaluation of actualb from white's view */
        int EvaluateForWhite();
    private:

        /**
         @return discdifference */
        int CountDiscs();

        /**
          laods the pattern values */
        void InitPatterns();

        /**
         @return the stage based on actualb */
        int GetStage();

        /**
         @return rounded value of packed real value found in t */
        int Adjust(int t);

        /**
         @param param to be read
         @param f the file to be read from
         @return 1 if no I/O error, 0 otherwise */
        int ReadAndCheck(int &param, FILE *f);

        //paramter, tables
        static int _diag4[Stages][3*3*3*3];
        static int _diag5[Stages][3*3*3*3*3];
        static int _diag6[Stages][3*3*3*3*3*3];
        static int _diag7[Stages][3*3*3*3*3*3*3];
        static int _diag8[Stages][3*3*3*3*3*3*3*3];
        static int _lc1[Stages][3*3*3*3*3*3*3*3];
        static int _lc2[Stages][3*3*3*3*3*3*3*3];
        static int _lc3[Stages][3*3*3*3*3*3*3*3];
        static int _lc4[Stages][3*3*3*3*3*3*3*3];
        static int _corn[Stages][3*3*3*3*3*3*3*3];

        static int _parity[Stages][2];

        //parameter tables, for each stage
        //these are indexable by negative pattern ids too
        static int *_pdiag4[Stages];
        static int *_pdiag5[Stages];
        static int *_pdiag6[Stages];
        static int *_pdiag7[Stages];
        static int *_pdiag8[Stages];
        static int *_plc1[Stages];
        static int *_plc2[Stages];
        static int *_plc3[Stages];
        static int *_plc4[Stages];
        static int *_pcorn[Stages];

        static int stagetable[60];

        /** an object to confirm or infirm black's turn*/
        static BillMobilityDat _bmobil;

        /** crc with same key as used by the Ethernet */
        static EthernetCrc _crc;
    };

    extern Board actualb;
}
#endif //_EVALUATOR_H_

