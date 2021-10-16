//**********************************************************************
// Copyright Patrick Sweeney 1996-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Math.Point;
#include "Base\Math\IBBox.h"
#include "Image.h"
#include "ImageImpl.h"
#include "ImageFilter.h"
#include "ImageIter.h"
#include <stack>

// Namespace: Caustic
namespace Caustic
{
    template <typename T>
    struct FloodStackElement
    {
        Point2 pos;
        T iter;
        CImageIter1 fillediter;

        FloodStackElement(int seedx, int seedy, T &_iter, CImageIter1 &_fillediter)
        {
            pos.x = seedx;
            pos.y = seedy;
            iter = _iter;
            fillediter = _fillediter;
        }
    };

    //**********************************************************************
    // Class: CFloodFillFilter
    // Defines an image filter that floods an image with color.
    //
    // Image Filter Parameters:
    // "Tolerance"   : <int> - Defines the tolerance used when comparing colors
    // "StartPointX" : <int> - X coordinate of pixel where to start flood fill
    // "StartPointY" : <int> - Y coordinate of pixel where to start flood fill
    // "Color"       : <RGBColor> - Color to flood with
    // "Opacity"     : <int> - Opacity of resulting flood
    //**********************************************************************
    class CFloodFillFilter : public IImageFilter, public CRefCount
    {
        template<typename T>
        void FloodFill(IImage* pImage, int seedx, int seedy, RGBColor& clr, int tolerance, int opacity, IBBox2& bbox);
    public:
        CFloodFillFilter()
        {
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IImageFilter
        //**********************************************************************
        virtual CRefObj<IImage> Apply(IImage* pImage, ImageFilterParams* pParams) override;
        virtual bool ApplyInPlace(IImage* pImage, ImageFilterParams* pParams) override;
    };

    //**********************************************************************
    // Function: CreateFloodFillFilter
    // Creates a flood fill filter.
    //
    // Returns:
    // Returns the filter that fill flood an image with color
    //**********************************************************************
    CRefObj<IImageFilter> CreateFloodFillFilter()
    {
        return CRefObj<IImageFilter>(new CFloodFillFilter());
    }

    template <typename T>
    void CFloodFillFilter::FloodFill(IImage *pImage, int seedx, int seedy, RGBColor& clr, int tolerance, int opacity, IBBox2& bbox)
    {
        int tolerancesq = tolerance * tolerance;

        CRefObj<IImage> filled = CreateImage(pImage->GetWidth(), pImage->GetHeight(), 1);
        memset((void*)filled->GetData(), '\0', filled->GetStride() * filled->GetHeight());
        uint8 ropacitytbl[256];
        uint8 gopacitytbl[256];
        uint8 bopacitytbl[256];
        if (opacity < 255)
        {
            for (int i = 0; i < 256; i++)
            {
                ropacitytbl[i] = ((255 * i - (i * opacity)) + (clr.r * opacity)) / 255;
                gopacitytbl[i] = ((255 * i - (i * opacity)) + (clr.g * opacity)) / 255;
                bopacitytbl[i] = ((255 * i - (i * opacity)) + (clr.b * opacity)) / 255;
            }
        }
        int minx, maxx, miny, maxy, x, y;
        minx = maxx = x = seedx;
        miny = maxy = y = seedy;
        T iter(pImage, seedx, seedy);
        CImageIter1 fillediter(filled, seedx, seedy);
        RGBColor seedclr(iter.GetRed(), iter.GetGreen(), iter.GetBlue());
        HLSColor seedhls(seedclr);
        std::stack<FloodStackElement<T>> fillstack;
        FloodStackElement<T> elem(seedx, seedy, iter, fillediter);
        fillstack.push(elem);
        while (!fillstack.empty())
        {
            FloodStackElement<T> elem = fillstack.top();
            fillstack.pop();
            x = elem.pos.x;
            y = elem.pos.y;
            iter = elem.iter;
            fillediter = elem.fillediter;
            if (y < miny)
                miny = y;
            if (y > maxy)
                maxy = y;
            if (opacity < 255)
            {
                iter.SetRed(ropacitytbl[iter.GetRed()]);
                iter.SetGreen(gopacitytbl[iter.GetGreen()]);
                iter.SetBlue(bopacitytbl[iter.GetBlue()]);
            }
            else
            {
                iter.SetRed(clr.r);
                iter.SetGreen(clr.g);
                iter.SetBlue(clr.b);
            }
            fillediter.SetBit(1);
            //
            // Fill span to right
            //
            T citer = iter;
            CImageIter1 filledciter = fillediter;
            citer.Step(CImageIter::Right);
            filledciter.Step(CImageIter::Right);
            int savex = x++;
            while (1)
            {
                if (x >= (int)pImage->GetWidth())
                {
                    x--;
                    citer.Step(CImageIter::Left);
                    filledciter.Step(CImageIter::Left);
                    break;
                }
                uint8 r = citer.GetRed();
                uint8 g = citer.GetGreen();
                uint8 b = citer.GetBlue();
                //
                // Compute color distance between pixel and seed
                //
                if (tolerance)
                {
                    RGBColor rgb(r, g, b);
                    HLSColor hls(rgb);

                    int hdiff = hls.h - seedhls.h;
                    int sdiff = hls.s - seedhls.s;
                    if (hdiff * hdiff + sdiff * sdiff > tolerancesq)
                    {
                        x--;
                        citer.Step(CImageIter::Left);
                        filledciter.Step(CImageIter::Left);
                        break;
                    }
                }
                else if (r != seedclr.r || g != seedclr.g || b != seedclr.b)
                {
                    x--;
                    citer.Step(CImageIter::Left);
                    filledciter.Step(CImageIter::Left);
                    break;
                }
                if (opacity < 255)
                {
                    citer.SetRed(ropacitytbl[citer.GetRed()]);
                    citer.SetGreen(gopacitytbl[citer.GetGreen()]);
                    citer.SetBlue(bopacitytbl[citer.GetBlue()]);
                }
                else
                {
                    citer.SetRed(clr.r);
                    citer.SetGreen(clr.g);
                    citer.SetBlue(clr.b);
                }
                filledciter.SetBit(1);
                if (x < minx)
                    minx = x;
                if (x > maxx)
                    maxx = x;
                x++;
                citer.Step(CImageIter::Right);
                filledciter.Step(CImageIter::Right);
            }
            int xright = x;
            //
            // Fill span to Left
            //
            x = savex - 1;
            citer = iter;
            filledciter = fillediter;
            citer.Step(CImageIter::Left);
            filledciter.Step(CImageIter::Left);
            while (1)
            {
                if (x < 0)
                {
                    x++;
                    citer.Step(CImageIter::Right);
                    filledciter.Step(CImageIter::Right);
                    break;
                }
                uint8 r = citer.GetRed();
                uint8 g = citer.GetGreen();
                uint8 b = citer.GetBlue();
                //
                // Compute color distance between pixel and seed
                //
                if (tolerance)
                {
                    RGBColor rgb(r, g, b);
                    HLSColor hls(rgb);

                    int hdiff = hls.h - seedhls.h;
                    int sdiff = hls.s - seedhls.s;
                    if (hdiff * hdiff + sdiff * sdiff > tolerancesq)
                    {
                        x++;
                        citer.Step(CImageIter::Right);
                        filledciter.Step(CImageIter::Right);
                        break;
                    }
                }
                else if (r != seedclr.r || g != seedclr.g || b != seedclr.b)
                {
                    x++;
                    citer.Step(CImageIter::Right);
                    filledciter.Step(CImageIter::Right);
                    break;
                }
                if (opacity < 255)
                {
                    citer.SetRed(ropacitytbl[citer.GetRed()]);
                    citer.SetGreen(gopacitytbl[citer.GetGreen()]);
                    citer.SetBlue(bopacitytbl[citer.GetBlue()]);
                }
                else
                {
                    citer.SetRed(clr.r);
                    citer.SetGreen(clr.g);
                    citer.SetBlue(clr.b);
                }
                filledciter.SetBit(1);
                if (x < minx)
                    minx = x;
                if (x > maxx)
                    maxx = x;
                x--;
                citer.Step(CImageIter::Left);
                filledciter.Step(CImageIter::Left);
            }
            int xLeft = x;
            T xLeftiter = citer;
            CImageIter1 filledxLeftiter = filledciter;
            //
            // Check scanline above and below current line for new seeds
            //
            if (y < (int)pImage->GetHeight() - 1)
            {
                x = xLeft;
                citer = xLeftiter;
                filledciter = filledxLeftiter;
                y++;
                citer.Step(CImageIter::Down);
                filledciter.Step(CImageIter::Down);
                while (x <= xright)
                {
                    //
                    // Now scan right until I find a pixel that can be filled
                    //
                    bool found = false;
                    while (1)
                    {
                        if (x >= (int)pImage->GetWidth() || x > xright)
                        {
                            x--;
                            citer.Step(CImageIter::Left);
                            filledciter.Step(CImageIter::Left);
                            break;
                        }
                        uint8 r = citer.GetRed();
                        uint8 g = citer.GetGreen();
                        uint8 b = citer.GetBlue();
                        if (tolerance)
                        {
                            RGBColor rgb(r, g, b);
                            HLSColor hls(rgb);

                            int hdiff = hls.h - seedhls.h;
                            int sdiff = hls.s - seedhls.s;
                            if (hdiff * hdiff + sdiff * sdiff <= tolerancesq &&
                                !filledciter.GetBit())
                            {
                                found = true;
                                break;
                            }
                        }
                        else if (r == seedclr.r && g == seedclr.g && b == seedclr.b &&
                            !filledciter.GetBit())
                        {
                            found = true;
                            break;
                        }
                        x++;
                        citer.Step(CImageIter::Right);
                        filledciter.Step(CImageIter::Right);
                    }
                    if (!found)
                        break;
                    //
                    // Push pixel on stack
                    //
                    FloodStackElement<T> elem(x, y, citer, filledciter);
                    fillstack.push(elem);
                    //
                    // Skip pixels that are fillable
                    //
                    found = false;
                    while (1)
                    {
                        if (x >= (int)pImage->GetWidth() || x > xright)
                        {
                            x--;
                            citer.Step(CImageIter::Left);
                            filledciter.Step(CImageIter::Left);
                            break;
                        }
                        uint8 r = citer.GetRed();
                        uint8 g = citer.GetGreen();
                        uint8 b = citer.GetBlue();
                        if (tolerance)
                        {
                            RGBColor rgb(r, g, b);
                            HLSColor hls(rgb);

                            int hdiff = hls.h - seedhls.h;
                            int sdiff = hls.s - seedhls.s;
                            if (hdiff * hdiff + sdiff * sdiff > tolerancesq ||
                                filledciter.GetBit())
                            {
                                found = true;
                                break;
                            }
                        }
                        else if (r != seedclr.r || g != seedclr.g || b != seedclr.b ||
                            filledciter.GetBit())
                        {
                            found = true;
                            break;
                        }
                        x++;
                        citer.Step(CImageIter::Right);
                        filledciter.Step(CImageIter::Right);
                    }
                    if (!found)
                        break;
                }
                y--;
            }
            if (y > 0)
            {
                x = xLeft;
                citer = xLeftiter;
                filledciter = filledxLeftiter;
                y--;
                citer.Step(CImageIter::Up);
                filledciter.Step(CImageIter::Up);
                while (x <= xright)
                {
                    //
                    // Now scan right until I find a pixel that can be filled
                    //
                    bool found = false;
                    while (1)
                    {
                        if (x >= (int)pImage->GetWidth() || x > xright)
                        {
                            x--;
                            citer.Step(CImageIter::Left);
                            filledciter.Step(CImageIter::Left);
                            break;
                        }
                        uint8 r = citer.GetRed();
                        uint8 g = citer.GetGreen();
                        uint8 b = citer.GetBlue();
                        if (tolerance)
                        {
                            RGBColor rgb(r, g, b);
                            HLSColor hls(rgb);

                            int hdiff = hls.h - seedhls.h;
                            int sdiff = hls.s - seedhls.s;
                            if (hdiff * hdiff + sdiff * sdiff <= tolerancesq &&
                                !filledciter.GetBit())
                            {
                                found = true;
                                break;
                            }
                        }
                        else if (r == seedclr.r && g == seedclr.g && b == seedclr.b &&
                            !filledciter.GetBit())
                        {
                            found = true;
                            break;
                        }
                        x++;
                        citer.Step(CImageIter::Right);
                        filledciter.Step(CImageIter::Right);
                    }
                    if (!found)
                        break;
                    //
                    // Push pixel on stack
                    //
                    FloodStackElement<T> elem(x, y, citer, filledciter);
                    fillstack.push(elem);
                    //
                    // Skip pixels that are fillable
                    //
                    found = false;
                    while (1)
                    {
                        if (x >= (int)pImage->GetWidth() || x > xright)
                        {
                            x--;
                            citer.Step(CImageIter::Left);
                            filledciter.Step(CImageIter::Left);
                            break;
                        }
                        uint8 r = citer.GetRed();
                        uint8 g = citer.GetGreen();
                        uint8 b = citer.GetBlue();
                        if (tolerance)
                        {
                            RGBColor rgb(r, g, b);
                            HLSColor hls(rgb);

                            int hdiff = hls.h - seedhls.h;
                            int sdiff = hls.s - seedhls.s;
                            if (hdiff * hdiff + sdiff * sdiff > tolerancesq ||
                                filledciter.GetBit())
                            {
                                found = true;
                                break;
                            }
                        }
                        else if (r != seedclr.r || g != seedclr.g || b != seedclr.b ||
                            filledciter.GetBit())
                        {
                            found = true;
                            break;
                        }
                        x++;
                        citer.Step(CImageIter::Right);
                        filledciter.Step(CImageIter::Right);
                    }
                    if (!found)
                        break;
                }
            }
        }
        bbox.p1.x = minx;
        bbox.p1.y = miny;
        bbox.p2.x = maxx;
        bbox.p2.y = maxy;
        delete filled;
        return;
    }

    //**********************************************************************
    // Method: Apply
    // Applies flood filling to the image.
    //
    // Parameters:
    // pImage - image to perform filtering on.
    // pParams - filter parameters.
    //
    // Returns:
    // Returns the filtered image.
    //**********************************************************************
    CRefObj<IImage> CFloodFillFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
    {
        CRefObj<IImage> spDest = pImage->Clone();
        ApplyInPlace(spDest, pParams);
        return spDest;
    }
    
    bool CFloodFillFilter::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
    {
        int tolerance = 10;
        int startX = 0, startY = 0;
        RGBColor color;
        int opacity = 255;
        if (pParams != nullptr)
        {
            std::map<std::string, std::any>::iterator it;
            it = pParams->params.find("Tolerance");
            if (it != pParams->params.end())
                tolerance = std::any_cast<int>(it->second);
            it = pParams->params.find("StartPointX");
            if (it != pParams->params.end())
                startX = std::any_cast<int>(it->second);
            it = pParams->params.find("StartPointY");
            if (it != pParams->params.end())
                startY = std::any_cast<int>(it->second);
            it = pParams->params.find("Color");
            if (it != pParams->params.end())
                color = std::any_cast<RGBColor>(it->second);
            it = pParams->params.find("Opacity");
            if (it != pParams->params.end())
                opacity = std::any_cast<int>(it->second);
        }
        IBBox2 bbox;
        if (pImage->GetBPP() == 24)
            FloodFill<CImageIter24>(pImage, startX, startY, color, tolerance, opacity, bbox);
        else if (pImage->GetBPP() == 32)
            FloodFill<CImageIter32>(pImage, startX, startY, color, tolerance, opacity, bbox);
        else
            CT(E_NOTIMPL);
        return true; // Inplace transformation not supported
    }
}
