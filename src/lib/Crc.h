/**
 @file
 Crc related classes.
*/

#if !defined _CRC__H_
#define _CRC__H_

//CREDIT www.relisoft.com 

/**
 Crc class provides CRC cheking services*/ 
class Crc
{
public:
    typedef unsigned long Type;

    /**
     @param key is the polynominal used */
    Crc(Type key) 
      :_key(key),
       _register(0) 
    {
        Init();
    }

    /**
     call this when all input has been processed
     @return the crc value */
    Type Done()
    {
        Type tmp = _register;
        _register = 0;
        return tmp;
    }

    /**
     runs a byte through */
    void PutByte(unsigned byte)
    {
        unsigned top = _register >> 24;
        top ^= byte;
        _register = (_register << 8) ^ _table[top];
    }

    /**
     runs a 32 bit word through */
    void Put32Bit(unsigned t);

private:
    /**
     initializes the precomputed table*/
    void Init();

    /** precomputed table */
    Type _table[256];

    /** the polynominal used*/
    Type _key;

    /** curent value */
    Type _register;
};

/**
 EthernetCrc class with the ethernet polynominal 0x04c11db7*/
class EthernetCrc: public Crc
{
public:
    explicit EthernetCrc()
      :Crc(0x04c11db7)
    {}
};

#endif //_CRC__H_

