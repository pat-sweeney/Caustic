//**********************************************************************
// Copyright Patrick Sweeney 1996-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <winerror.h>
#include <memory>
#include <any>
#include <string>

export module Imaging.Image.ImageFilter.DepthOfField;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Imaging.Color;
import Imaging.Image.ImageFilter;

// Namespace: Caustic
//
// Module:
// {Link:import Imaging.Image.ImageFilter.DepthOfField;{Imaging/Image/DepthOfField.ixx}}
export namespace Caustic
{
    //**********************************************************************
    // Class: CDepthOfFieldFilter
    // Defines an image filter for performing depth of field
    //
    // Image Filter Parameters:
    // "MinStrength"   : <float> - Strength of blur at min distance
    // "MaxStrength"   : <float> - Strength of blur at max distance
    // "MinDistance"   : <float> - Minimum distance where blur starts
    // "DepthImage"    : <IImage> - depth image
    //
    // Module:
    // {Link:import Imaging.Image.ImageFilter.DepthOfField;{Imaging/Image/DepthOfField.ixx}}
    //**********************************************************************
    class CDepthOfFieldFilter : public IImageFilter, public CRefCount
    {
        template <typename T>
        CRefObj<IImage> DepthOfField(IImage* pImage, float minStrength, float maxStrength, float minDistance, IImage *pDepthImage);
    public:
        CDepthOfFieldFilter()
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
    // Function: CreateDepthOfFieldFilter
    // Creates a filter that performs depth of field
    //
    // Returns:
    // Returns filter that performs depth of field.
    //**********************************************************************
    CRefObj<IImageFilter> CreateDepthOfFieldFilter()
    {
        return CRefObj<IImageFilter>(new CDepthOfFieldFilter());
    }

    template <typename T>
    CRefObj<IImage> CDepthOfFieldFilter::DepthOfField(IImage* pImage, float minStrength, float maxStrength, float minDistance, IImage* pDepthImage)
    {
        CRefObj<IImage> spImage = CreateImage(pImage->GetWidth(), pImage->GetHeight(), pImage->GetBPP());
        RGBColor clr[3][3];
        T riter[3];
        T citer[3];
        int32 avgr, avgg, avgb;

        uint32 w1 = pImage->GetWidth() - 1;
        uint32 h1 = pImage->GetHeight() - 1;
        riter[0] = T(pImage, 0, 0);
        riter[1] = T(pImage, 0, 0);
        riter[2] = T(pImage, 0, (pImage->GetHeight() > 1) ? 1 : 0);
        T dstriter(spImage, 0, 0);
        uint32 w = pImage->GetWidth();
        uint32 h = pImage->GetHeight();
        for (uint32 y = 0; y < h; y++)
        {
            T dstciter = dstriter;
            citer[0] = riter[0];
            citer[1] = riter[1];
            citer[2] = riter[2];
            clr[0][0].r = clr[0][1].r = citer[0].GetRed();
            clr[1][0].r = clr[1][1].r = citer[1].GetRed();
            clr[2][0].r = clr[2][1].r = citer[2].GetRed();
            clr[0][0].g = clr[0][1].g = citer[0].GetGreen();
            clr[1][0].g = clr[1][1].g = citer[1].GetGreen();
            clr[2][0].g = clr[2][1].g = citer[2].GetGreen();
            clr[0][0].b = clr[0][1].b = citer[0].GetBlue();
            clr[1][0].b = clr[1][1].b = citer[1].GetBlue();
            clr[2][0].b = clr[2][1].b = citer[2].GetBlue();
            if (w > 1)
            {
                citer[0].Step(CImageIter::Right);
                citer[1].Step(CImageIter::Right);
                citer[2].Step(CImageIter::Right);
            }
            uint32 x = pImage->GetWidth();
            for (uint32 x = 0; x < pImage->GetWidth(); x++)
            {
                // Check depth
                auto nx = pDepthImage->GetWidth() * x / pImage->GetWidth();
                auto ny = pDepthImage->GetHeight() * y / pImage->GetHeight();
                CImageIter16 r(pDepthImage, nx, ny);
                auto v = r.GetGray();
                if (v > 1000)
                {
                    clr[0][2].r = citer[0].GetRed();
                    clr[0][2].g = citer[0].GetGreen();
                    clr[0][2].b = citer[0].GetBlue();
                    clr[1][2].r = citer[1].GetRed();
                    clr[1][2].g = citer[1].GetGreen();
                    clr[1][2].b = citer[1].GetBlue();
                    if (y < h1)
                    {
                        clr[2][2].r = citer[2].GetRed();
                        clr[2][2].g = citer[2].GetGreen();
                        clr[2][2].b = citer[2].GetBlue();
                    }
                    avgr = (clr[0][0].r + clr[0][1].r + clr[0][2].r +
                        clr[1][0].r + clr[1][1].r + clr[1][2].r +
                        clr[2][0].r + clr[2][1].r + clr[2][2].r) / 9;
                    avgg = (clr[0][0].g + clr[0][1].g + clr[0][2].g +
                        clr[1][0].g + clr[1][1].g + clr[1][2].g +
                        clr[2][0].g + clr[2][1].g + clr[2][2].g) / 9;
                    avgb = (clr[0][0].b + clr[0][1].b + clr[0][2].b +
                        clr[1][0].b + clr[1][1].b + clr[1][2].b +
                        clr[2][0].b + clr[2][1].b + clr[2][2].b) / 9;
                    dstciter.SetRed((uint8)avgr);
                    dstciter.SetGreen((uint8)avgg);
                    dstciter.SetBlue((uint8)avgb);
                    dstciter.SetAlpha(255);
                }
                else
                {
                    dstciter.SetRed((uint8)clr[1][1].r);
                    dstciter.SetGreen((uint8)clr[1][1].b);
                    dstciter.SetBlue((uint8)clr[1][1].g);
                }
                clr[0][0] = clr[0][1];
                clr[1][0] = clr[1][1];
                clr[2][0] = clr[2][1];
                clr[0][1] = clr[0][2];
                clr[1][1] = clr[1][2];
                clr[2][1] = clr[2][2];
                if (x > 1)
                {
                    citer[0].Step(CImageIter::Right);
                    citer[1].Step(CImageIter::Right);
                    citer[2].Step(CImageIter::Right);
                }
                dstciter.Step(CImageIter::Right);
            }
            if (y > 0)
                riter[0].Step(CImageIter::Down);
            riter[1].Step(CImageIter::Down);
            if (y < h1)
                riter[2].Step(CImageIter::Down);
            dstriter.Step(CImageIter::Down);
        }
        return spImage;
    }

    //**********************************************************************
    // Method: Apply
    // See <IImageFilter::Apply>
    //**********************************************************************
    CRefObj<IImage> CDepthOfFieldFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
    {
        // "MinStrength"   : <float> - Strength of blur at min distance
        // "MaxStrength"   : <float> - Strength of blur at max distance
        // "MinDistance"   : <float> - Minimum distance where blur starts
        // "DepthImage"    : <IImage> - depth image
        auto minStrength = (pParams == nullptr) ? 30 : std::any_cast<float>(pParams->params.find("MinStrength"));
        auto maxStrength = (pParams == nullptr) ? 30 : std::any_cast<float>(pParams->params.find("MaxStrength"));
        auto minDistance = (pParams == nullptr) ? 30 : std::any_cast<float>(pParams->params.find("MinDistance"));
        auto depthImage = (pParams == nullptr) ? nullptr : std::any_cast<IImage*>(pParams->params.find("DepthImage"));
        if (pImage->GetBPP() == 24)
            return DepthOfField<CImageIter24>(pImage, minStrength, maxStrength, minDistance, depthImage);
        else if (pImage->GetBPP() == 32)
            return DepthOfField<CImageIter32>(pImage, minStrength, maxStrength, minDistance, depthImage);
        CT(E_NOTIMPL);
        return CRefObj<IImage>(nullptr);
    }

    bool CDepthOfFieldFilter::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
    {
        return false; // Not supported
    }
}
