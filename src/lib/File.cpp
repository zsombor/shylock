/**
 @file
 Win::File implementation, not quite used.
*/
#include <cstring>
#include <cassert>
#include "File.h"
#include "WinException.h"

using namespace Win;

File::File(PSTR filename, int mode)
{
    //win32 *&@^#$&^@# !!!
    TCHAR _fname[MAX_PATH];
    int i=0;
    do
    {
        _fname[i]=filename[i];
    }while(filename[i++]!=TEXT(0));
    
    switch(mode)
    {
    case write:
        _hfile=::CreateFile(filename, GENERIC_WRITE, DWORD(0), NULL,
                            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
                            HANDLE(NULL));
        break;
    case read:
        _hfile=::CreateFile(filename, GENERIC_READ, DWORD(0), NULL,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                            HANDLE(NULL));
        break;
    default:
        assert(false);
    }

    if(_hfile==INVALID_HANDLE_VALUE) 
        throw Win::Exception("CreateFile failure");
}

File::~File()
{
    ::CloseHandle(_hfile);
}

int File::Write(char const *buffer)
{
    DWORD bytes_written;
    BOOL ret=::WriteFile(_hfile, buffer, strlen(buffer), &bytes_written, NULL);
    if(!ret)
        throw Win::Exception("WriteFile failure");

    return bytes_written;
}

int File::Read(char *buffer, int bufferSize)
{
    DWORD bytes_read;
    BOOL ret=::ReadFile(_hfile, buffer, bufferSize, &bytes_read, NULL);
    if(!ret)
        throw Win::Exception("ReadFile failure");

    return bytes_read;
}
