/**
 @file
 Implements the Crc class.
*/
#include <cassert>
#include "Crc.h"

void Crc::Init()
{
    assert(_key!=0);
    //for all possible byte values
    for (unsigned int i=0; i<256; i++)
    {
        Crc::Type reg= i<<24;
        // for all bits in a byte
        for(int j=0; j<8; j++)
        {
            bool topBit= (reg & 0x80000000) != 0;
            reg <<= 1;
            if(topBit)
                reg ^= _key;
        }
        _table[i] = reg;
    }
}

void Crc::Put32Bit(unsigned t)
{
    PutByte((t & 0xff000000)>>24);
    PutByte((t & 0x00ff0000)>>16);
    PutByte((t & 0x0000ff00)>>8);
    PutByte((t & 0x000000ff));
}

