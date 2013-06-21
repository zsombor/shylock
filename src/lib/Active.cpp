/**
  @file
  ActiveObject's implementation
*/
#include "Active.h"

using namespace Win;

ActiveObject::ActiveObject()
  : _isDying (0),
// disable warning "'this' used before initialized"
#pragma warning(disable: 4355)   
   _thread (ThreadEntry, this)
#pragma warning(default: 4355)
{}

void ActiveObject::Kill()
{
    _isDying++;
    FlushThread();
    _thread.WaitForDeath();
}

DWORD WINAPI ActiveObject::ThreadEntry(void* pArg)
{
    ActiveObject *pActive= static_cast<ActiveObject *> (pArg);
    pActive->InitThread();
    pActive->Run();
    return 0;
}
