/**
 @file
 Interface of the Win::Bitmap class.
*/
#if !defined _BITMAP_H_
#define _BITMAP_H_
#include <windows.h>
#include "Canvas.h"

namespace Win
{
    /**
      provides a strong pointer interface to win32 resource HBITMAP. */
    class Bitmap
    {
    public:
        /**
         default constructor, no actual bitmap is created*/
        Bitmap()
            : _hBitmap(0)
        {}

        /** 
         copy contructor using transfer semantics the new BitMap object 
         will be the sole owner of the actual bitmap */ 
        Bitmap (Bitmap &bmp)
            : _hBitmap(bmp.Release ())
        {}
    
        /**
         creates a compatible bitmap of size dx*dy associated to a 
         a Canvas */
        Bitmap::Bitmap(Canvas &canvas, int dx, int dy)
            : _hBitmap (0)
        {
            CreateCompatible(canvas, dx, dy);
        }
        
        /**
          opeator= current bitmap is is freed and the new bitmap becames
          the sole owner of the actual bitmap.*/
        void operator = (Bitmap &bmp)
        {
            if(bmp._hBitmap!=_hBitmap)
            {
                Free();
                _hBitmap=bmp.Release();
            }
        }

        /**
          deletes the actual bitmap from memory */
        ~Bitmap()
        {
            Free();
        }
        
        /**
          type conversion to HBITMAP */
        operator HBITMAP() 
        { 
            return _hBitmap;
        }

        /**
          load bitmap from resource
          @param hInst application instance
          @param id resource id of the bitmap to be loaded
          @exception throws Win::Exception upon failure */
        void Load(HINSTANCE hInst, int id);

        /**
          load bitmap from a given file
          @param filename name of the bitmap file to load
          @exception throws Win::Exception upon failure */
        void Load(char *filename);

        /**
          query bitmap size */
        void GetSize(int &width, int &height);
    
        /**
          frees current storage and creates a compatible bitmap with canvas
          of the given size */        
        void Bitmap::CreateCompatible(Canvas &canvas, int width, int height)
        {
            Free();
            _hBitmap=::CreateCompatibleBitmap(canvas, width, height);
        }

    protected:
        /**
         (!!!protected!!!) type conversion from HBITMAP */
        Bitmap(HBITMAP hBitmap)
            :_hBitmap(hBitmap)
        {}
    
        /**
         deletes the actual bitmap from memory */
        void Free()
        {
            if(_hBitmap) 
                ::DeleteObject(_hBitmap);
        }

        /**
         release, just like std::aut_ptr<T>.release() */
        HBITMAP Release()
        {
            HBITMAP h=_hBitmap;
            _hBitmap=0;
            return h;
        }

        /**
         handle to the actual bitmap in memory */
        HBITMAP    _hBitmap;
    };

    /**
      blits a Bitmap to a Canvas */
    class Blitter
    {
    public:
        /**
          constructor
          @param bmp the Bitmap to blit
          the default blit mode is SRCCOPY*/
        Blitter(Bitmap &bmp)
            : _bmp(bmp),
              _xDst(0),
              _yDst(0),
              _xSrc(0),
              _ySrc(0),
              _mode(SRCCOPY)
        {
            bmp.GetSize(_width, _height);
        }

        /**
          sets the blit mode */
        void SetMode(DWORD mode)
        {
            _mode=mode;
        }
        
        /**
          sets the destination */
        void SetDest(int x, int y)
        {
            _xDst=x;
            _yDst=y;
        }

        /**
          sets the source */
        void SetSrc(int x, int y)
        {
            _xSrc=x;
            _ySrc=y;
        }
        
        /**
         sets the destination rectangle's size */
        void SetArea(int width, int height)
        {
            _width = width;
            _height = height;
        }

        /**
           transfer bitmap to canvas */
        void BlitTo (Canvas & canvas);

    private:
        /**
          the bitmap to be transfered */
        Bitmap & _bmp;

        int _xDst, _yDst;
        int _xSrc, _ySrc;
        int _width, _height;
        DWORD _mode;
    };
}
#endif //_BITMAP_H_

