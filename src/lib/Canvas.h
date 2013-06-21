/**
 @file
 Various classes for Win32 GDI handling
*/
#if !defined _CANVAS_H_
#define _CANVAS_H_
#include <windows.h>

namespace Win
{
    /**
     class Canvas encapsulates an win32 HDC, used as a base class 
     (constructor is protected)*/
    class Canvas
    {
    public:
        /**
          type conversion to HDC */
        operator HDC() 
        { 
            return _hdc; 
        }
    
        /** 
          sets the pixel with given coordinates to the specified color*/
        void SetPixel(int x, int y, COLORREF color)
        {
            ::SetPixel(_hdc, x, y, color);
        }
	    
        /**
         cleans a rectangular shape with white */
        void WhiteWash(const RECT &rect)
        {
            ::PatBlt(_hdc, rect.left, rect.top, 
                     rect.right, rect.bottom, WHITENESS);
        }

        /**
         cleans the rectagular shape with the curently selected brush */
        void Wash(const RECT &rect)
        {
            ::PatBlt(_hdc, rect.left, rect.top, 
                     rect.right, rect.bottom, PATCOPY);
        }

        /** 
         cleans a rectangular shape with black */
        void BlackWash(const RECT &rect)
        {
            ::PatBlt(_hdc, rect.left, rect.top,
                     rect.right, rect.bottom, BLACKNESS);
        }
	    
        /**
         changes current possition */
        void MoveTo(int x, int y)
        {
            ::MoveToEx(_hdc, x, y, 0);
        }

        /** 
         draws a line from current position to (x,y) */
        void LineTo(int x, int y)
        {
            ::LineTo (_hdc, x, y);
        }

        /**
         draws a line connecting two points */
        void Line(int x1, int y1, int x2, int y2)
        {
            MoveTo(x1, y1);
            LineTo(x2, y2);
        }

        /**
         draws an ellipse in the specified bounding rectangle */
        void Ellipse(int x1, int y1, int x2, int y2)
        {
            ::Ellipse(_hdc, x1, y1, x2, y2);
        }

        /**
         rectangle */
        void Frame(RECT const &rect, HBRUSH brush);

        /**
         fill a rectangle */
        void FillRect(RECT const &rect, HBRUSH brush);

        /**
         selects a GDI object into the canvas */
        void SelectObject(void* pObj)
        {
            ::SelectObject(_hdc, pObj);
        }

        /**
         draws fromated text in a bounding rectangle */
        void Text(const char *text, RECT &rect, UINT format)
        {
            ::DrawText(_hdc, text, -1, &rect, format);
        }

        /**
         sets the text color */
        void SetTextColor(COLORREF col)
        {
            ::SetTextColor(_hdc, col);
        }

        /**
         sets the background color */
        void SetBkrndColor(COLORREF col)
        {
            ::SetBkColor(_hdc, col);
        }

    protected:
        /**
         protected constructor, no Canvas objects can be instatieted 
         directly */
        Canvas(HDC hdc) 
            :_hdc(hdc) {}

        /** handle to the device context */
        HDC  _hdc;
    };

    /**
       class PaintCanvas is used to encapsulate in the spirit of resource 
       managment the win32 BeginPaint EndPaint Win32 API call pair. 
       Suitable for painting after a WM_PAINT message. */
    class PaintCanvas: public Canvas
    {
    public:
        /**
         @param hwnd handle to the window to be painted upon. */
        PaintCanvas(HWND hwnd)
		    : Canvas(::BeginPaint(hwnd, &_paint)),
             _hwnd(hwnd)
        {}

        /**
         destructor, automatically calls EndPaint */
        ~PaintCanvas()
        {
            ::EndPaint(_hwnd, &_paint);
        }

        /**
         returns the top of the rectangle in which the painting is 
         requested */
        int Top() const    
        { 
            return _paint.rcPaint.top; 
        }

        /**
         returns the bottom of the rectangle in which the painting is 
         requested */
        int Bottom() const 
        { 
            return _paint.rcPaint.bottom;
        }

        /**
         returns the left of the rectangle in which the painting is 
         requested */
        int Left() const   
        { 
            return _paint.rcPaint.left; 
        }

        /**
         returns the right of the rectangle in which the painting is 
         requested */
        int Right() const  
        { 
            return _paint.rcPaint.right; 
        }

        /**
         returns the rectangle in which the painting is requested */
        const RECT& PaintRect() const
        {
            return _paint.rcPaint;
        }

    protected:
        PAINTSTRUCT _paint; 
        HWND        _hwnd;
    };

    /** 
     class UpdateCanvas following the First Rule of Acquisition 
     encapsulates win32 API call pairs GetDC and ReleaseDC. 
     Suitable for painting outside the WM_PAINT message processing. */
    class UpdateCanvas: public Canvas
    {
    public:

        /**
         @param hwnd handle to the window to be painted upon. */
        UpdateCanvas(HWND hwnd)
	      :Canvas(::GetDC(hwnd)),
           _hwnd(hwnd)
        {}

        /**
         destructor, automatically calls ReleaseDC */
        ~UpdateCanvas()
        {
            ::ReleaseDC(_hwnd, _hdc);
        }
    protected:
        HWND _hwnd;
    };

    /**
     class MemCanvas used to draw on a piece of memory
     encapsulates the CreateCompatibleDC and DeleteDC Win32 API calls */
    class MemCanvas: public Canvas
    {
    public:
        /**
         @param hdc handle to the target device context */
        MemCanvas(HDC hdc)
          :Canvas(::CreateCompatibleDC(hdc))
        {}

        /**
         destructor, automatically calls DeleteDC */
        ~MemCanvas()
        {
            ::DeleteDC(_hdc); 
        }
    };

    /**
      BitmapCanvas to draw on a bitmap in found memory */
    class BitmapCanvas: public MemCanvas
    {
    public:
        /**
         @param hdc handle to the target device context
         @param hBitmap handle to the bitmap */
        BitmapCanvas(HDC hdc, HBITMAP hBitmap)
          :MemCanvas(hdc)
        {
            _hOldBitmap=reinterpret_cast<HBITMAP>
                            (::SelectObject(_hdc, hBitmap));
        }

        ~BitmapCanvas()
        {
            ::SelectObject(_hdc, _hOldBitmap);
        }

    private:
        HBITMAP _hOldBitmap;
    };
}
#endif //_CANVAS_H_

