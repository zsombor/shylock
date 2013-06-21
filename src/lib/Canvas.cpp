/**
 @file
 Some Win::Canvas related methods.
*/
#include "canvas.h"
#include "tools.h"

using namespace Win;

void Canvas::Frame(RECT const &rect,HBRUSH brush)
{
    ::FrameRect(_hdc,&rect,brush);
}

void Canvas::FillRect(RECT const& rect,HBRUSH brush)
{
    ::FillRect(_hdc,&rect,brush);
}

