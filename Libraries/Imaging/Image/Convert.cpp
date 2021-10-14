//**********************************************************************
// Copyright Patrick Sweeney 1996-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
#include "Image.h"
#include "ImageFilter.h"
#include "ImageIter.h"
#include <memory>
#include <functional>

// Namespace: Caustic
namespace Caustic
{
    //**********************************************************************
    // Class: CConvertFilter
    // Defines an image filter that converts from one pixel format to another
    //
    // Image Filter Parameters:
    // "DestFormat" : <EImageType> - pixel format to convert to
    //**********************************************************************
    class CConvertFilter : public IImageFilter, public CRefCount
    {
        CRefObj<IImage> Convert8to16(IImage* pImage);
        CRefObj<IImage> Convert8to24(IImage* pImage);
        CRefObj<IImage> Convert8to32(IImage* pImage);
        CRefObj<IImage> Convert8toFloat1(IImage* pImage);
        CRefObj<IImage> Convert24to8(IImage* pImage);
        CRefObj<IImage> Convert24to32(IImage* pImage);
        CRefObj<IImage> Convert24toFloat3(IImage* pImage);
        CRefObj<IImage> Convert32toFloat3(IImage* pImage);
        CRefObj<IImage> ConvertFloat3to24(IImage* pImage);
        CRefObj<IImage> ConvertAnyToAny(IImage* pImage, EImageType destFormat);
    public:
        CConvertFilter()
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
        virtual bool ApplyInPlace(IImage* pImage, ImageFilterParams* pParams) override { return false; }
    };

    //**********************************************************************
    // Function: CreateConvertFilter
    // Filter for converting from one image format to another
    //
    // Returns:
    // Returns a filter that will convert image formats
    //**********************************************************************
    CRefObj<IImageFilter> CreateConvertFilter()
    {
        return CRefObj<IImageFilter>(new CConvertFilter());
    }

    //**********************************************************************
    // Method: Convert8to16
    // Converts a Gray 8bpp image into a Gray 16bpp image.
    //
    // Parameters:
    // pImage - Source image to convert
    //
    // Returns:
    // Returns the converted image
    //**********************************************************************
    CRefObj<IImage> CConvertFilter::Convert8to16(IImage* pImage)
    {
        CRefObj<IImage> spDestImage = CreateImage(pImage->GetWidth(), pImage->GetHeight(), 16);
        CImageIter8 srcRow(pImage, 0, 0);
        CImageIter16 dstRow(spDestImage, 0, 0);
        int w = pImage->GetWidth();
        int h = pImage->GetHeight();
        for (int y = 0; y < h; y++)
        {
            CImageIter8 srcCol = srcRow;
            CImageIter16 dstCol = dstRow;
            for (int x = 0; x < w; x++)
            {
                dstCol.SetGray(srcCol.GetGray());
                srcCol.Step(CImageIter::EStepDirection::Right);
                dstCol.Step(CImageIter::EStepDirection::Right);
            }
            srcRow.Step(CImageIter::EStepDirection::Down);
            dstRow.Step(CImageIter::EStepDirection::Down);
        }
        return spDestImage;
    }

    //**********************************************************************
    // Method: Convert8to24
    // Converts a Gray 8bpp image into a RGB 24bpp image.
    //
    // Parameters:
    // pImage - Source image to convert
    //
    // Returns:
    // Returns the converted image
    //**********************************************************************
    CRefObj<IImage> CConvertFilter::Convert8to24(IImage* pImage)
    {
        CRefObj<IImage> spDestImage = CreateImage(pImage->GetWidth(), pImage->GetHeight(), 24);
        CImageIter8 srcRow(pImage, 0, 0);
        CImageIter24 dstRow(spDestImage, 0, 0);
        int w = pImage->GetWidth();
        int h = pImage->GetHeight();
        for (int y = 0; y < h; y++)
        {
            CImageIter8 srcCol = srcRow;
            CImageIter24 dstCol = dstRow;
            for (int x = 0; x < w; x++)
            {
                uint8 gray = srcCol.GetGray();
                dstCol.SetRed(gray);
                dstCol.SetGreen(gray);
                dstCol.SetBlue(gray);
                srcCol.Step(CImageIter::EStepDirection::Right);
                dstCol.Step(CImageIter::EStepDirection::Right);
            }
            srcRow.Step(CImageIter::EStepDirection::Down);
            dstRow.Step(CImageIter::EStepDirection::Down);
        }
        return spDestImage;
    }

    //**********************************************************************
    // Method: Convert8to32
    // Converts a Gray 8bpp image into a RGB 32bpp image.
    //
    // Parameters:
    // pImage - Source image to convert
    //
    // Returns:
    // Returns the converted image
    //**********************************************************************
    CRefObj<IImage> CConvertFilter::Convert8to32(IImage* pImage)
    {
        CRefObj<IImage> spDestImage = CreateImage(pImage->GetWidth(), pImage->GetHeight(), 32);
        CImageIter8 srcRow(pImage, 0, 0);
        CImageIter32 dstRow(spDestImage, 0, 0);
        int w = pImage->GetWidth();
        int h = pImage->GetHeight();
        for (int y = 0; y < h; y++)
        {
            CImageIter8 srcCol = srcRow;
            CImageIter32 dstCol = dstRow;
            for (int x = 0; x < w; x++)
            {
                uint8 gray = srcCol.GetGray();
                dstCol.SetRed(gray);
                dstCol.SetGreen(gray);
                dstCol.SetBlue(gray);
                dstCol.SetBlue(255);
                srcCol.Step(CImageIter::EStepDirection::Right);
                dstCol.Step(CImageIter::EStepDirection::Right);
            }
            srcRow.Step(CImageIter::EStepDirection::Down);
            dstRow.Step(CImageIter::EStepDirection::Down);
        }
        return spDestImage;
    }

    //**********************************************************************
    // Method: Convert8toFloat1
    // Converts a Gray 8bpp image into a float image.
    //
    // Parameters:
    // pImage - Source image to convert
    //
    // Returns:
    // Returns the converted image
    //**********************************************************************
    CRefObj<IImage> CConvertFilter::Convert8toFloat1(IImage* pImage)
    {
        CRefObj<IImage> spDestImage = CreateImage(pImage->GetWidth(), pImage->GetHeight(), 32);
        CImageIter8 srcRow(pImage, 0, 0);
        CImageIterFloat1 dstRow(spDestImage, 0, 0);
        int w = pImage->GetWidth();
        int h = pImage->GetHeight();
        for (int y = 0; y < h; y++)
        {
            CImageIter8 srcCol = srcRow;
            CImageIterFloat1 dstCol = dstRow;
            for (int x = 0; x < w; x++)
            {
                uint8 gray = srcCol.GetGray();
                dstCol.SetValue(gray / 255.0f);
                srcCol.Step(CImageIter::EStepDirection::Right);
                dstCol.Step(CImageIter::EStepDirection::Right);
            }
            srcRow.Step(CImageIter::EStepDirection::Down);
            dstRow.Step(CImageIter::EStepDirection::Down);
        }
        return spDestImage;
    }

    //**********************************************************************
    // Method: Convert24to8
    // Converts a RGB 24bpp image into a gray 8bpp.
    //
    // Parameters:
    // pImage - Source image to convert
    //
    // Returns:
    // Returns the converted image
    //**********************************************************************
    CRefObj<IImage> CConvertFilter::Convert24to8(IImage* pImage)
    {
        CRefObj<IImage> spDestImage = CreateImage(pImage->GetWidth(), pImage->GetHeight(), 8);
        CImageIter24 srcRow(pImage, 0, 0);
        CImageIter8 dstRow(spDestImage, 0, 0);
        int w = pImage->GetWidth();
        int h = pImage->GetHeight();
        for (int y = 0; y < h; y++)
        {
            CImageIter24 srcCol = srcRow;
            CImageIter8 dstCol = dstRow;
            for (int x = 0; x < w; x++)
            {
                RGBColor rgb;
                rgb.r = srcCol.GetRed();
                rgb.g = srcCol.GetGreen();
                rgb.b = srcCol.GetBlue();
                YIQColor yiq(rgb);
                dstCol.SetGray(yiq.y);
                srcCol.Step(CImageIter::EStepDirection::Right);
                dstCol.Step(CImageIter::EStepDirection::Right);
            }
            srcRow.Step(CImageIter::EStepDirection::Down);
            dstRow.Step(CImageIter::EStepDirection::Down);
        }
        return spDestImage;
    }

    //**********************************************************************
    // Method: Convert24to32
    // Converts a RGB 24bpp image into a RGB 32bpp.
    //
    // Parameters:
    // pImage - Source image to convert
    //
    // Returns:
    // Returns the converted image
    //**********************************************************************
    CRefObj<IImage> CConvertFilter::Convert24to32(IImage* pImage)
    {
        CRefObj<IImage> spDestImage = CreateImage(pImage->GetWidth(), pImage->GetHeight(), 32);
        CImageIter24 srcRow(pImage, 0, 0);
        CImageIter32 dstRow(spDestImage, 0, 0);
        int w = pImage->GetWidth();
        int h = pImage->GetHeight();
        for (int y = 0; y < h; y++)
        {
            CImageIter24 srcCol = srcRow;
            CImageIter32 dstCol = dstRow;
            for (int x = 0; x < w; x++)
            {
                dstCol.SetRed(srcCol.GetRed());
                dstCol.SetGreen(srcCol.GetGreen());
                dstCol.SetBlue(srcCol.GetBlue());
                dstCol.SetAlpha(255);
                srcCol.Step(CImageIter::EStepDirection::Right);
                dstCol.Step(CImageIter::EStepDirection::Right);
            }
            srcRow.Step(CImageIter::EStepDirection::Down);
            dstRow.Step(CImageIter::EStepDirection::Down);
        }
        return spDestImage;
    }

    //**********************************************************************
    // Method: Convert24toFloat3
    // Converts a RGB 24bpp image into a float3 image.
    //
    // Parameters:
    // pImage - Source image to convert
    //
    // Returns:
    // Returns the converted image
    //**********************************************************************
    CRefObj<IImage> CConvertFilter::Convert24toFloat3(IImage* pImage)
    {
        int w = pImage->GetWidth();
        int h = pImage->GetHeight();
        CRefObj<IImage> spDestImage = CreateImage(w, h, 96);
        CImageIter24 srcRow(pImage, 0, 0);
        CImageIterFloat3 dstRow(spDestImage, 0, 0);
        for (int y = 0; y < h; y++)
        {
            CImageIter24 srcCol = srcRow;
            CImageIterFloat3 dstCol = dstRow;
            for (int x = 0; x < w; x++)
            {
                float3 v;
                v.x = ((float)srcCol.GetRed()) / 255.0f;
                v.y = ((float)srcCol.GetGreen()) / 255.0f;
                v.z = ((float)srcCol.GetBlue()) / 255.0f;
                dstCol.SetValue(v);
                srcCol.Step(CImageIter::EStepDirection::Right);
                dstCol.Step(CImageIter::EStepDirection::Right);
            }
            srcRow.Step(CImageIter::EStepDirection::Down);
            dstRow.Step(CImageIter::EStepDirection::Down);
        }
        return spDestImage;
    }

    //**********************************************************************
    // Method: Convert32toFloat3
    // Converts a RGB 32bpp image into a float3 image.
    //
    // Parameters:
    // pImage - Source image to convert
    //
    // Returns:
    // Returns the converted image
    //**********************************************************************
    CRefObj<IImage> CConvertFilter::Convert32toFloat3(IImage* pImage)
    {
        int w = pImage->GetWidth();
        int h = pImage->GetHeight();
        CRefObj<IImage> spDestImage = CreateImage(w, h, 96);
        CImageIter24 srcRow(pImage, 0, 0);
        CImageIterFloat3 dstRow(spDestImage, 0, 0);
        for (int y = 0; y < h; y++)
        {
            CImageIter24 srcCol = srcRow;
            CImageIterFloat3 dstCol = dstRow;
            for (int x = 0; x < w; x++)
            {
                float3 v;
                v.x = ((float)srcCol.GetRed()) / 255.0f;
                v.y = ((float)srcCol.GetGreen()) / 255.0f;
                v.z = ((float)srcCol.GetBlue()) / 255.0f;
                dstCol.SetValue(v);
                srcCol.Step(CImageIter::EStepDirection::Right);
                dstCol.Step(CImageIter::EStepDirection::Right);
            }
            srcRow.Step(CImageIter::EStepDirection::Down);
            dstRow.Step(CImageIter::EStepDirection::Down);
        }
        return spDestImage;
    }

    //**********************************************************************
    // Method: ConvertFloat3to24
    // Converts a float3 image into a RGB 24bpp image .
    //
    // Parameters:
    // pImage - Source image to convert
    //
    // Returns:
    // Returns the converted image
    //**********************************************************************
    CRefObj<IImage> CConvertFilter::ConvertFloat3to24(IImage* pImage)
    {
        int w = pImage->GetWidth();
        int h = pImage->GetHeight();
        CRefObj<IImage> spDestImage = CreateImage(w, h, 24);
        CImageIterFloat3 srcRow(pImage, 0, 0);
        CImageIter24 dstRow(spDestImage, 0, 0);
        for (int y = 0; y < h; y++)
        {
            CImageIterFloat3 srcCol = srcRow;
            CImageIter24 dstCol = dstRow;
            for (int x = 0; x < w; x++)
            {
                float3 v = srcCol.GetValue();
                dstCol.SetRed(uint8(v.x * 255.0f));
                dstCol.SetGreen(uint8(v.y * 255.0f));
                dstCol.SetBlue(uint8(v.z * 255.0f));
                srcCol.Step(CImageIter::EStepDirection::Right);
                dstCol.Step(CImageIter::EStepDirection::Right);
            }
            srcRow.Step(CImageIter::EStepDirection::Down);
            dstRow.Step(CImageIter::EStepDirection::Down);
        }
        return spDestImage;
    }
    
    //**********************************************************************
    // Method: ConvertFloat3to24
    // Converts a float3 image into a RGB 24bpp image .
    //
    // Parameters:
    // pImage - Source image to convert
    //
    // Returns:
    // Returns the converted image
    //**********************************************************************
    CRefObj<IImage> CConvertFilter::ConvertAnyToAny(IImage* pImage, EImageType destFormat)
    {
        int w = pImage->GetWidth();
        int h = pImage->GetHeight();
        int bpp = 0;
        switch (destFormat)
        {
        case EImageType::BW_1bpp:
            bpp = 1;
            break;
        case EImageType::Gray_8bpp:
            bpp = 8;
            break;
        case EImageType::Gray_16bpp:
            bpp = 16;
            break;
        case EImageType::RGB_24bpp:
            bpp = 24;
            break;
        case EImageType::RGBA_32bpp:
        case EImageType::RGBX_32bpp:
        case EImageType::Float1_32bpp:
            bpp = 32;
            break;
        case EImageType::Float2_64bpp:
            bpp = 64;
            break;
        case EImageType::Float3_96bpp:
            bpp = 96;
            break;
        case EImageType::Float4_128bpp:
            bpp = 128;
            break;
        }
        CRefObj<IImage> spDestImage = CreateImage(w, h, bpp);
        CImageIterGeneric srcRow(pImage, 0, 0);
        CImageIterGeneric dstRow(spDestImage, 0, 0);
        for (int y = 0; y < h; y++)
        {
            CImageIterGeneric srcCol = srcRow;
            CImageIterGeneric dstCol = dstRow;
            for (int x = 0; x < w; x++)
            {
                dstRow.SetRed(srcRow.GetRed());
                dstRow.SetGreen(srcRow.GetGreen());
                dstRow.SetBlue(srcRow.GetBlue());
                dstRow.SetAlpha(srcRow.GetAlpha());
                srcCol.Step(CImageIter::EStepDirection::Right);
                dstCol.Step(CImageIter::EStepDirection::Right);
            }
            srcRow.Step(CImageIter::EStepDirection::Down);
            dstRow.Step(CImageIter::EStepDirection::Down);
        }
        return spDestImage;
    }
    
    //**********************************************************************
    // Method: Apply
    // See <IImageFilter::Apply>
    //**********************************************************************
    CRefObj<IImage> CConvertFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
    {
        EImageType destFormat = EImageType::RGB_24bpp;
        if (pParams != nullptr)
        {
            std::map<std::string, std::any>::iterator it = pParams->params.find("DestFormat");
            if (it != pParams->params.end())
                destFormat = std::any_cast<EImageType>(it->second);
        }

        CRefObj<IImage> spImage = CreateImage(pImage->GetWidth(), pImage->GetHeight(), 32);

        struct DispatchTable
        {
            EImageType sourceType;
            EImageType destType;
            std::function<CRefObj<IImage>(IImage*)> convertFunc;
        } _conversionMappings[] = {
            { EImageType::Gray_8bpp, EImageType::Gray_16bpp, [this](IImage* pSource) ->CRefObj<IImage> { return Convert8to24(pSource); }},
            { EImageType::Gray_8bpp, EImageType::RGB_24bpp, [this](IImage* pSource) ->CRefObj<IImage> { return Convert8to24(pSource); }},
            { EImageType::Gray_8bpp, EImageType::RGBA_32bpp, [this](IImage* pSource) ->CRefObj<IImage> { return Convert8to32(pSource); }},
            { EImageType::Gray_8bpp, EImageType::RGBX_32bpp, [this](IImage* pSource) ->CRefObj<IImage> { return Convert8to32(pSource); }},
            { EImageType::Gray_8bpp, EImageType::Float1_32bpp, [this](IImage* pSource) ->CRefObj<IImage> { return Convert8to32(pSource); }},
            { EImageType::RGB_24bpp, EImageType::Gray_8bpp, [this](IImage* pSource) ->CRefObj<IImage> { return Convert8to32(pSource); }},
            { EImageType::RGB_24bpp, EImageType::RGBA_32bpp, [this](IImage* pSource) ->CRefObj<IImage> { return Convert8to32(pSource); }},
            { EImageType::RGB_24bpp, EImageType::RGBX_32bpp, [this](IImage* pSource) ->CRefObj<IImage> { return Convert8to32(pSource); }},
            { EImageType::RGB_24bpp, EImageType::Float3_96bpp, [this](IImage* pSource) ->CRefObj<IImage> { return Convert8to32(pSource); }},
            { EImageType::RGBA_32bpp, EImageType::Float3_96bpp, [this](IImage* pSource) ->CRefObj<IImage> { return Convert8to32(pSource); }},
            { EImageType::Float3_96bpp, EImageType::RGB_24bpp, [this](IImage* pSource) ->CRefObj<IImage> { return Convert8to32(pSource); }},
        };
        for (int i = 0; i < _countof(_conversionMappings); i++)
        {
            if (_conversionMappings[i].sourceType == pImage->GetImageType() && _conversionMappings[i].destType == destFormat)
            {
                return _conversionMappings[i].convertFunc(pImage);
            }
        }
        return ConvertAnyToAny(pImage, destFormat);
    }
}
