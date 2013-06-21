/**
 @file
 Interface of a naive file handling
*/
#if !defined _MY_FILE_H_
#define _MY_FILE_H_
#include <windows.h>

namespace Win
{
    /**
    I would be better of with std::fstream but if I write win32 then
    why not use win32. */
    class File
    {
    public:

        /** access types */
        enum
        {
            read,  /**< open file for reading */
            write, /**< open file for writeing */
        };

        /** 
         opens file
         @param filename the name of the file to be opened
         @param mode specifies the access mode must be File::read for reading
                     or File:write for writing
         @exception throws Win::Exception upon failure */
        File(PSTR filename, int mode);

        ~File();

        /** 
          @param buffer zero termianted string to be written  
          @return number of bytes written
          @exception throws Win::Exception upon failure */
        int Write(char const *buffer);

        /** 
          @param buffer where the data is read
          @param bufferSize the buffer's size
          @return number of bytes read
          @exception throws Win::Exception upon failure */
        int Read(char *buffer, int bufferSize);

    private:
        HANDLE _hfile;

        //the file name
        TCHAR _fname[MAX_PATH];
    };
}
#endif //_MY_FILE_H_

