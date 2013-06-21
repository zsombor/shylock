/**
 @file 
 Implements the Bitmap and Blitter classes.
*/
#include "Bitmap.h"
#include "WinException.h"

using namespace Win;

void Bitmap::Load(HINSTANCE hInst, int id)
{
    Free();

    _hBitmap=static_cast<HBITMAP>( ::LoadImage(hInst, 
                                   MAKEINTRESOURCE(id),
                                   IMAGE_BITMAP, 0, 0,
                                   LR_CREATEDIBSECTION));
    if(_hBitmap==0)
        throw Win::Exception ("Cannot load bitmap from resources");
}

void Bitmap::Load(char* filename)
{
    Free();

    _hBitmap=static_cast<HBITMAP>( ::LoadImage(0, filename,
                                   IMAGE_BITMAP, 0, 0, 
                                   LR_LOADFROMFILE));
    if(_hBitmap==0)
        throw Win::Exception("Cannot load bitmap from file");
}

void Bitmap::GetSize(int &width,int &height)
{
    BITMAP bm;
    ::GetObject(_hBitmap, sizeof(bm), &bm);
    width=bm.bmWidth;
    height=bm.bmHeight;
}

void Blitter::BlitTo(Canvas &canvas)
{
    //create canvas in memory using target canvas as template
    MemCanvas memCanvas(canvas);

    //convert bitmap to the format appropriate for this canvas
    memCanvas.SelectObject(_bmp);

    //transfer bitmap from memory canvas to target canvas
    ::BitBlt(canvas, _xDst, _yDst,
             _width, _height,memCanvas,
             _xSrc,_ySrc,_mode);
}
