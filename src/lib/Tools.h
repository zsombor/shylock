/**
 @file
 Win32 GDI related utility classes, encapusalting pens, brushes, their use
 etc.
*/
#if !defined _CANVASTOOLS_H_
#define _CANVASTOOLS_H_
#include <windows.h>

namespace Win
{
    /**
      class stock object, selects a stock object of given type into 
      specified device context, while saving the previous stock object 
      that will be restored upon destruction */
    class StockObject
    {
    public:
        StockObject(HDC hdc, int type)
          : _hdc(hdc)
        {
            _hObjOld=::SelectObject(_hdc, ::GetStockObject(type));
        }
        
        ~StockObject()
        {
            ::SelectObject(_hdc, _hObjOld);
        }
    private:
        HGDIOBJ  _hObjOld;
        HDC      _hdc;
    };

    /**
      selects a white pen into the given device context */
    class WhitePen: public StockObject
    {
    public:
        WhitePen(HDC hdc)
          :StockObject (hdc, WHITE_PEN) 
        {}
    };

    /**
      selects a black pen into the given device context */
    class BlackPen: public StockObject
    {
    public:
        BlackPen(HDC hdc)
          :StockObject(hdc, BLACK_PEN) 
        {}
    };

    /**
      selects a pen with given color and type (default is PS_SOLID)
      into the given device context. */
    class Pen
    {
    public:
        /**
         constructor */
        Pen(COLORREF color, int style=PS_SOLID)
        {
            _hPen=::CreatePen(style, 0, color);
        }

        /**
         destructor */
        ~Pen()
        {
            ::DeleteObject(_hPen);
        }

        /**
         conversion to HPEN */
        operator HPEN() 
        { 
            return _hPen; 
        }
    private:
        HPEN    _hPen;
    };

    /**
     PenHolder replaces the pen in given device context, saving the 
     original that will be restored upon destruction */
    class PenHolder
    {
    public:
        PenHolder(HDC hdc, HPEN hPen)
            : _hdc(hdc)
        {
            _hPenOld=(HPEN) ::SelectObject(_hdc, hPen); 
        }

        ~PenHolder()
        {
            ::SelectObject(_hdc, _hPenOld);
        }
    private:
        HDC     _hdc;
        HPEN    _hPenOld;
    };


    /**
      a brush with given color or pattern from a bitmap */
    class Brush
    {
    public:
        Brush(COLORREF color)
        {
            _hBrush=::CreateSolidBrush(color);
        }

        Brush(HBITMAP hBmp)
        {
            _hBrush=::CreatePatternBrush(hBmp);
        }

        ~Brush()
        {
            ::DeleteObject(_hBrush);
        }

        operator HBRUSH() const 
        { 
            return _hBrush;
        }
    private:
        HBRUSH  _hBrush;
    };

    /**
     BrushHolder replaces the brush in given device context, saving the 
     original that will be restored upon destruction */
    class BrushHolder
    {
    public:
        BrushHolder(HDC hdc, HBRUSH hBrush)
          :_hdc(hdc)
        {
            _hBrushOld=HBRUSH(::SelectObject(_hdc, hBrush)); 
        }

        ~BrushHolder()
        {
            ::SelectObject(_hdc, _hBrushOld);
        }
    private:
        HDC     _hdc;
        HBRUSH  _hBrushOld;
    };

    /**
     selects a white brush into a given device context */
    class WhiteBrush: public StockObject
    {
    public:
        WhiteBrush(HDC hdc)
            : StockObject(hdc, WHITE_BRUSH) 
        {}
    };

    /**
     selects a black brush into a given device context */
    class BlackBrush: public StockObject
    {
    public:
        BlackBrush(HDC hdc)
            : StockObject(hdc, BLACK_BRUSH) 
        {}
    };

    /**
     the client rectangle of the specified window */
    class ClientRect: public RECT
    {
    public:
        ClientRect(HWND hwnd)
        {
            ::GetClientRect(hwnd, this);
        }
    };
}
#endif //_CANVASTOOLS_H_

